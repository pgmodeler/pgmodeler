/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "syntaxhighlighter.h"
#include "widgets/numberedtexteditor.h"
#include "attributes.h"
#include "exception.h"
#include "globalattributes.h"
#include "utilsns.h"

QFont SyntaxHighlighter::default_font = QFont("Source Code Pro", 12);
const QString SyntaxHighlighter::UnformattedGroup("__unformatted__");

SyntaxHighlighter::SyntaxHighlighter(QPlainTextEdit *parent, bool single_line_mode, bool use_custom_tab_width, qreal custom_fnt_size) : QSyntaxHighlighter(parent)
{
	if(!parent)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	code_field_txt = parent;
	this->setDocument(parent->document());
	this->single_line_mode = single_line_mode;
	custom_font_size = custom_fnt_size;
	configureAttributes();
	parent->installEventFilter(this);

	if(use_custom_tab_width)
		parent->setTabStopDistance(NumberedTextEditor::getTabDistance());

	//Adjusting the size of the parent input according to the current font size
	if(single_line_mode)
	{
		QFont fnt(default_font.family(), getCurrentFontSize());
		QFontMetrics fm = QFontMetrics(fnt);
		int height=fm.height() + fm.lineSpacing()/2;
		parent->setMinimumHeight(height);
		parent->setMaximumHeight(height);
		parent->setSizePolicy(parent->sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);
		parent->adjustSize();
		parent->setTabChangesFocus(true);
		parent->setLineWrapMode(QPlainTextEdit::NoWrap);
		parent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		connect(parent, &QPlainTextEdit::textChanged, this, [parent](){
			// Avoiding pasting code with line breaks when the single line mode is activated
			QString txt = parent->toPlainText();
			if(!txt.isEmpty() && txt.contains(QChar::LineFeed))
			{
				txt.remove(QChar::LineFeed);
				parent->blockSignals(true);
				parent->setPlainText(txt);
				parent->blockSignals(false);
			}
		});
	}

	highlight_timer.setInterval(300);
	connect(parent, &QPlainTextEdit::cursorPositionChanged, &highlight_timer, qOverload<>(&QTimer::start));

	connect(&highlight_timer, &QTimer::timeout, this, [this](){
		highlight_timer.stop();

		if(this->single_line_mode)
			return;

		for(auto &cfg : enclosing_chrs)
			highlightEnclosingChars(cfg);
	});
}

bool SyntaxHighlighter::eventFilter(QObject *object, QEvent *event)
{
	//Filters the ENTER/RETURN avoiding line breaks
	if(this->single_line_mode && event->type() == QEvent::KeyPress)
	{
		QKeyEvent *k_event = dynamic_cast<QKeyEvent *>(event);

		if(k_event->key() == Qt::Key_Return || k_event->key() == Qt::Key_Enter)
		{
			event->ignore();
			return true;
		}
	}

	/* If the user is about press Control to paste contents or Right mouse button in
	 order to call the context menu to paste text */
	if(event->type()==QEvent::MouseButtonPress || event->type() == QEvent::KeyPress)
	{
		QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);
		QMouseEvent *m_event=dynamic_cast<QMouseEvent *>(event);

		//Remove the formatting from the clipboard buffer to paste only pure text
		if(qApp->clipboard() && qApp->clipboard()->mimeData()->hasHtml() &&
				((m_event && m_event->button()==Qt::RightButton) ||
				 (k_event && k_event->modifiers()==Qt::ControlModifier)))
			qApp->clipboard()->setText(qApp->clipboard()->mimeData()->text());
	}

	return QSyntaxHighlighter::eventFilter(object, event);
}

void SyntaxHighlighter::configureAttributes()
{
	conf_loaded = false;
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
	int last_pos = -1;
	QString open_group;
	ExprElement open_expr, expr;
	TextBlockInfo *blk_info = nullptr,
			*prev_blk_info = dynamic_cast<TextBlockInfo *>(currentBlock().previous().userData());
	int prev_blk_state = currentBlock().previous().userState();

	/* Creating a text block info so we can register
	 * each position where there is a text formatting */
	if(!currentBlockUserData())
	{
		blk_info = new TextBlockInfo;
		setCurrentBlockUserData(blk_info);
		setCurrentBlockState(prev_blk_state == PersistentBlock ? SimpleBlock : prev_blk_state);
	}
	else
	{
		//Reset the block's info to permit the rehighlighting
		blk_info = dynamic_cast<TextBlockInfo *>(currentBlockUserData());
		blk_info->reset();
		setCurrentBlockState(SimpleBlock);
	}

	/* If we have a previous block that is pending to close a multiline expression
	 * We get the open group associated to the previous block and iterate over
	 * the expressions of that group until we find the final expression that
	 * closes it */
	if(prev_blk_info && prev_blk_state == OpenExprBlock)
	{
		open_group = 	prev_blk_info->getOpenGroup();
		last_pos = 0;
		setCurrentBlockState(OpenExprBlock);
	}

	if(!text.isEmpty())
	{
		int pos = 0, match_end = -1, match_start = -1;
		FormatGroup fmt_grp;
		FragmentInfo fg_info;

		/* If we have an open group inherited from the previous block,
		 * we try to find the closing expression in the current block */
		if(!open_group.isEmpty() && currentBlockState() == OpenExprBlock)
		{
			for(auto &expr : fmt_groups[open_group].expr_elements)
			{
				if(expr.final && matchExpression(text, pos, expr, match_start, match_end))
				{
					last_pos = match_end + 1;
					setFormat(pos, match_end, open_group, expr, blk_info);
					open_group.clear();
					setCurrentBlockState(SimpleBlock);
				}
			}
		}

		for(int grp_id = 0; grp_id < fmt_groups_order.size(); grp_id++)
		{
			if(currentBlockState() == PersistentBlock)
				break;

			fmt_grp = fmt_groups[fmt_groups_order[grp_id]];
			last_pos = 0;

			for(int expr_id = 0; expr_id < fmt_grp.expr_elements.size(); expr_id++)
			{
				pos = last_pos >= 0 ? last_pos : 0;

				do
				{
					expr = fmt_grp.expr_elements[expr_id];

					if(matchExpression(text, pos, expr, match_start, match_end))
					{
						/* Moving to the character right after the matching end position
						 * so we can keep matching the rest of the text */
						pos = match_end + 1;
						fg_info = blk_info->getFragmentInfo(match_start);

						/* If we have an open group and the current expression is the final one
						 * of the group (closing an expression) we reset the control variables
						 * of open group since the expression is now closed */
						if(currentBlockState() == OpenExprBlock && fmt_grp.name == open_group && expr.final)
						{
							setCurrentBlockState(SimpleBlock);
							open_group.clear();
							open_expr.clear();
							match_start = last_pos;
							last_pos = pos;
							expr_id = 0;
						}
						/* If the matching expression is related to a expression opening
						 * we use the group name / expression to highlight the rest of the
						 * text until we find the closing expression of that group */
						else if(currentBlockState() == SimpleBlock && open_group.isEmpty() && expr.initial)
						{
							setCurrentBlockState(OpenExprBlock);
							open_group = fmt_grp.name;
							open_expr = expr;
							last_pos = pos;
							expr_id = 0;
						}
						else if(fmt_grp.isPersistent())
						{
							if(currentBlockState() == OpenExprBlock &&
								 prev_blk_state != OpenExprBlock && !fg_info.isValid())
							{
								setCurrentBlockState(PersistentBlock);
								match_end = text.length();
							}
							else if(fg_info.isValid() && fg_info.isClosed())
							{
								fmt_grp = fmt_groups[fg_info.getGroup()];
								match_end = fg_info.getEnd();
							}

							open_group.clear();
						}

						setFormat(match_start, match_end,
											 open_group.isEmpty() ? fmt_grp.name : open_group,
											 open_group.isEmpty() ? expr : open_expr, blk_info);

						if(currentBlockState() != SimpleBlock)
							break;
					}
					else
						pos++;
				}
				while(pos < text.length());
			}
		}
	}

	/* If the highlight iterated until the end of the block
	 * but there still an open group expression without being closed
	 * we apply the highlight from the last position until the end of
	 * the block */
	if(!open_group.isEmpty() && currentBlockState() != PersistentBlock)
	{
		if(!text.isEmpty())
			setFormat(last_pos, text.length(), open_group, open_expr, blk_info);

		blk_info->setOpenGroup(open_group);
		setCurrentBlockState(OpenExprBlock);
	}
}

void SyntaxHighlighter::setFormat(int start, int end, const QString &group, const ExprElement &expr_elem, TextBlockInfo *blk_info)
{
	FormatGroup fmt_group = fmt_groups[group];
	QTextCharFormat fmt = fmt_group.format;
	FragmentInfo f_info;
	int count = (end - start) + 1;

	blk_info->addFragmentInfo(FragmentInfo(fmt_group.name, start, end, fmt_group.persistent,
																				 expr_elem.initial, expr_elem.final,
																				 fmt_group.allow_completion));

	fmt.setFontFamily(default_font.family());
	fmt.setFontPointSize(getCurrentFontSize());

	QSyntaxHighlighter::setFormat(start, count, fmt);
}

bool SyntaxHighlighter::matchExpression(const QString &text, int txt_pos, const ExprElement &expr, int &start, int &end)
{
	if(expr.exact)
	{
		start = text.indexOf(expr.pattern, txt_pos,
																expr.case_sensitive ?
																		Qt::CaseSensitive : Qt::CaseInsensitive);
		end = start + expr.pattern.length() - 1;
	}
	else
	{
		QRegularExpression regexp;
		QRegularExpressionMatch match;

		regexp.setPattern(expr.pattern);
		regexp.setPatternOptions(QRegularExpression::DontCaptureOption |
															(!expr.case_sensitive ?
																	 QRegularExpression::CaseInsensitiveOption :
																	 QRegularExpression::NoPatternOption));

		match = regexp.match(text, txt_pos);
		start = match.capturedStart();
		end = match.capturedEnd() - 1;
	}

	int len = (end - start) + 1;
	return start >= 0 && end >= 0 && len > 0;
}

void SyntaxHighlighter::highlightEnclosingChars(const EnclosingCharsCfg &cfg)
{
	QTextCursor tc;
	QString curr_txt;
	QPlainTextEdit *code_txt = qobject_cast<QPlainTextEdit *>(parent());

	tc = code_txt->textCursor();
	tc.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	curr_txt = tc.selectedText();
	tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);

	if(curr_txt != cfg.open_char && curr_txt != cfg.close_char)
	{
		tc = code_txt->textCursor();
		tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
		curr_txt = tc.selectedText();
	}

	if(curr_txt != cfg.open_char && curr_txt != cfg.close_char)
		return;

	QChar inc_chr, dec_chr;
	QString code = code_txt->toPlainText();
	int chr_cnt = 0,
			pos = tc.position(),
			ini_pos = pos,
			inc = curr_txt == cfg.open_char ? 1 : -1;

	inc_chr = curr_txt == cfg.open_char ? cfg.open_char : cfg.close_char;
	dec_chr = curr_txt == cfg.open_char ? cfg.close_char : cfg.open_char;

	while(pos >= 0 && pos < code.size())
	{
		if(code[pos] == inc_chr)
			chr_cnt++;
		else if(code[pos] == dec_chr)
			chr_cnt--;

		if(chr_cnt == 0)
			break;

		pos += inc;
	}

	if(ini_pos >= 0)
	{
		QTextCharFormat fmt;
		QList<QTextEdit::ExtraSelection> selections;
		QTextEdit::ExtraSelection sel;

		if(NumberedTextEditor::isHighlightLines() && !single_line_mode)
		{
			sel.format.setBackground(NumberedTextEditor::getLineHighlightColor());
			sel.format.setProperty(QTextFormat::FullWidthSelection, true);
			sel.cursor = tc;
			sel.cursor.clearSelection();
			selections.append(sel);
		}

		fmt = tc.charFormat();

		if(pos >= 0 && pos < code.size())
		{
			// Color config for balanced enclosing chars
			fmt.setBackground(cfg.bg_color);
			fmt.setForeground(cfg.fg_color);
		}
		else
		{
			// Color config for unbalanced enclosing chars
			fmt.setBackground(QColor(200, 0, 0));
			fmt.setForeground(Qt::white);
		}

		sel.format = fmt;
		tc.setPosition(ini_pos);
		tc.setPosition(ini_pos + 1, QTextCursor::KeepAnchor);
		sel.cursor = tc;
		selections.append(sel);

		if(pos >= 0 && pos < code.size())
		{
			tc.setPosition(pos);
			tc.setPosition(pos + 1, QTextCursor::KeepAnchor);
			sel.cursor = tc;
			selections.append(sel);
		}

		code_txt->setExtraSelections(selections);
	}
}

qreal SyntaxHighlighter::getCurrentFontSize()
{
	return custom_font_size > 0 ?
					custom_font_size : default_font.pointSizeF();
}

bool SyntaxHighlighter::isConfigurationLoaded()
{
	return conf_loaded;
}

void SyntaxHighlighter::clearConfiguration()
{
	fmt_groups_order.clear();
	fmt_groups.clear();
	configureAttributes();
}

void SyntaxHighlighter::loadConfiguration(const QString &filename)
{
	if(!filename.isEmpty())
	{
		attribs_map attribs;
		QString elem, group, pattern;
		bool bold = false, italic = false, strikeout = false,
				underline = false, exact_match = false,
				initial_expr = false, final_expr = false;
		QTextCharFormat format;
		QColor bg_color, fg_color;
		FormatGroup fmt_group;
		QRegularExpression regexp;
		QStringList groups, persist_groups;

		try
		{
			clearConfiguration();
			xmlparser.restartParser();
			xmlparser.setDTDFile(GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::ObjectDTDDir,
																																					GlobalAttributes::CodeHighlightConf +
																																					GlobalAttributes::ObjectDTDExt),
													 GlobalAttributes::CodeHighlightConf);

			xmlparser.loadXMLFile(filename);

			if(xmlparser.accessElement(XmlParser::ChildElement))
			{
				do
				{
					if(xmlparser.getElementType()==XML_ELEMENT_NODE)
					{
						elem = xmlparser.getElementName();

						if(elem == Attributes::CompletionTrigger)
						{
							xmlparser.getElementAttributes(attribs);

							if(attribs[Attributes::Value].size() >= 1)
								completion_trigger = attribs[Attributes::Value].at(0);
						}
						else if(elem == Attributes::EnclosingChars)
						{
							xmlparser.getElementAttributes(attribs);

							EnclosingCharsCfg cfg;
							cfg.open_char = attribs[Attributes::OpenChar].front();
							cfg.close_char = attribs[Attributes::CloseChar].front();

							#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
								cfg.fg_color = QColor::fromString(attribs[Attributes::ForegroundColor]);
								cfg.bg_color = QColor::fromString(attribs[Attributes::BackgroundColor]);
							#else
								cfg.fg_color.setNamedColor(attribs[Attributes::ForegroundColor]);
								cfg.bg_color.setNamedColor(attribs[Attributes::BackgroundColor]);
							#endif

							enclosing_chrs.push_back(cfg);
						}

						if(elem == Attributes::Group)
						{
							xmlparser.getElementAttributes(attribs);
							group = attribs[Attributes::Name];

							//Raises an error if the group is being constructed by a second time
							if(fmt_groups.contains(group))
							{
								throw Exception(Exception::getErrorMessage(ErrorCode::DefDuplicatedGroup).arg(group),
																 ErrorCode::DefDuplicatedGroup, __PRETTY_FUNCTION__, __FILE__, __LINE__);
							}
							//Raises an error if the group does not have children element
							else if(!xmlparser.hasElement(XmlParser::ChildElement))
							{
								throw Exception(Exception::getErrorMessage(ErrorCode::DefEmptyGroup).arg(group),
																ErrorCode::DefEmptyGroup, __PRETTY_FUNCTION__, __FILE__, __LINE__);
							}

							italic = (attribs[Attributes::Italic] == Attributes::True);
							bold = (attribs[Attributes::Bold] == Attributes::True);
							underline = (attribs[Attributes::Underline] == Attributes::True);
							strikeout = (attribs[Attributes::Stikeout] == Attributes::True);
							fg_color.setNamedColor(attribs[Attributes::ForegroundColor]);

							//If the attribute isn't defined the bg color will be transparent
							if(attribs[Attributes::BackgroundColor].isEmpty())
								bg_color.setRgb(0,0,0,0);
							else
								bg_color.setNamedColor(attribs[Attributes::BackgroundColor]);

							format.setFontFamily(default_font.family());
							format.setFontPointSize(default_font.pointSizeF());
							format.setFontItalic(italic);
							format.setFontUnderline(underline);
							format.setFontStrikeOut(strikeout);

							if(bold)
								format.setFontWeight(QFont::Bold);
							else
								format.setFontWeight(QFont::Normal);

							format.setForeground(fg_color);
							format.setBackground(bg_color);

							fmt_group = FormatGroup(group, format,
																			attribs[Attributes::AllowCompletion] != Attributes::False,
																			attribs[Attributes::Persistent] == Attributes::True);

							xmlparser.savePosition();
							xmlparser.accessElement(XmlParser::ChildElement);

							do
							{
								if(xmlparser.getElementType()==XML_ELEMENT_NODE)
								{
									xmlparser.getElementAttributes(attribs);

									initial_expr = attribs[Attributes::Initial] == Attributes::True;
									final_expr = attribs[Attributes::Final] == Attributes::True;

									exact_match = attribs[Attributes::Type] != Attributes::Wildcard &&
																attribs[Attributes::Type] != Attributes::RegularExp;

									pattern = attribs[Attributes::Type] == Attributes::Wildcard ?
															QRegularExpression::wildcardToRegularExpression(attribs[Attributes::Value]) :
															attribs[Attributes::Value];

									regexp.setPattern(pattern);

									// We throw an error aborting the loading if the regepx has an invalid pattern
									if(!exact_match && !regexp.isValid())
									{
										throw Exception(Exception::getErrorMessage(ErrorCode::InvGroupRegExpPattern).arg(group, filename, regexp.errorString()),
																		ErrorCode::InvGroupRegExpPattern, __PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
																		tr("Pattern: %1").arg(regexp.pattern()));
									}
									else if(fmt_group.isPersistent() && (initial_expr || final_expr))
									{
										throw Exception(Exception::getErrorMessage(ErrorCode::InvExprPersistentGroup).arg(group, filename),
																		ErrorCode::InvExprPersistentGroup, __PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
																		tr("Pattern: %1").arg(regexp.pattern()));
									}
									else if(initial_expr && final_expr)
									{
										throw Exception(Exception::getErrorMessage(ErrorCode::InvExprMultilineGroup).arg(group, filename),
																		 ErrorCode::InvExprMultilineGroup, __PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
																		 tr("Pattern: %1").arg(regexp.pattern()));
									}

									fmt_group.expr_elements.append(ExprElement(pattern, initial_expr,
																														 final_expr, exact_match,
																														 attribs[Attributes::CaseSensitive] == Attributes::True));
								}
							}
							while(xmlparser.accessElement(XmlParser::NextElement));

							fmt_groups[fmt_group.name] = fmt_group;

							if(fmt_group.isPersistent())
								persist_groups.append(fmt_group.name);
							else if(fmt_group.isMultiline())
								groups.append(fmt_group.name);
							else
								groups.prepend(fmt_group.name);

							xmlparser.restorePosition();
						}
					}
				}
				while(xmlparser.accessElement(XmlParser::NextElement));
			}

			fmt_groups_order.append(groups);
			fmt_groups_order.append(persist_groups);
			conf_loaded = true;
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e, filename);
		}
	}
}

QStringList SyntaxHighlighter::getExpressions(const QString &group_name)
{
	QStringList exprs;

	if(fmt_groups.contains(group_name))
	{
		for(auto &elem : fmt_groups[group_name].expr_elements)
			exprs.append(elem.pattern);
	}

	return exprs;
}

QChar SyntaxHighlighter::getCompletionTrigger()
{
	return completion_trigger;
}

void SyntaxHighlighter::setDefaultFont(const QFont &fnt)
{
	SyntaxHighlighter::default_font = fnt;
}
