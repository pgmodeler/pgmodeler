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
#include "textblockinfo.h"
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

bool SyntaxHighlighter::hasInitialAndFinalExprs(const QString &group)
{
	return (initial_exprs.count(group) && final_exprs.count(group));
}

void SyntaxHighlighter::configureAttributes()
{
	conf_loaded = false;
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
	QRegularExpression regexp;
	QRegularExpressionMatch match;
	int pos = 0, idx = -1, match_len = 0, patt_len = 0;

	if(text.isEmpty())
		return;

	for(auto &fmt_grp : fmt_groups)
	{
		for(auto &elem : fmt_grp.elements)
		{
			pos = 0;

			do
			{
				if(elem.exact)
				{
					patt_len = elem.pattern.length();
					idx = text.indexOf(elem.pattern, pos,
															elem.case_sensitive ?
																	Qt::CaseSensitive : Qt::CaseInsensitive);

					if(idx >= 0)
					{
						setFormat(idx, patt_len, fmt_grp.format);
						pos += idx + patt_len;
					}
					else
						pos++;
				}
				else
				{
					regexp.setPattern(elem.pattern);
					regexp.setPatternOptions(!elem.case_sensitive ?
																		QRegularExpression::CaseInsensitiveOption :
																				QRegularExpression::NoPatternOption);

					match = regexp.match(text, pos);
					match_len = match.capturedLength();

					if(match.hasMatch() && match_len > 0)
					{
						setFormat(match.capturedStart(), match_len, fmt_grp.format);
						pos = match.capturedEnd();
					}
					else
						pos++;
				}
			}
			while(pos < text.length());
		}
	}
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

QString SyntaxHighlighter::getExpressionId(const QString &group, const QRegularExpression *expr)
{
	if(group.isEmpty() || !expr)
		return "";

	return QString("%1_%2").arg(group,
															UtilsNs::getStringHash(
																QString::number(reinterpret_cast<uintptr_t>(expr))).mid(0,6));
}

void SyntaxHighlighter::loadConfiguration(const QString &filename)
{
	if(!filename.isEmpty())
	{
		attribs_map attribs;
		QString elem, group, pattern;
		bool groups_decl = false,
				bold = false, italic = false, strikeout = false,
				underline = false, exact_match = false;
		QTextCharFormat format;
		QColor bg_color, fg_color;
		FormatGroup *fmt_grp = nullptr;
		QRegularExpression regexp;

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

						/*	If the element is what defines the order of application of the groups
								highlight in the (highlight-order). Is in this block that are declared
								the groups used to highlight the source code. ALL groups
								in this block must be declared before they are built
								otherwise an error will be triggered. */
						else if(elem == Attributes::HighlightOrder)
						{
							//Marks a flag indication that groups are being declared
							groups_decl = true;
							xmlparser.savePosition();
							xmlparser.accessElement(XmlParser::ChildElement);
							elem = xmlparser.getElementName();
						}

						if(elem == Attributes::Group)
						{
							xmlparser.getElementAttributes(attribs);
							group = attribs[Attributes::Name];

							/* If the parser is on the group declaration block and not in the build block
								 some validations are made. */
							if(groups_decl)
							{
								//Raises an error if the group was declared before
								if(fmt_groups_order.contains(group))
								{
									throw Exception(Exception::getErrorMessage(ErrorCode::InvRedeclarationGroup).arg(group),
																	ErrorCode::InvRedeclarationGroup,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								//Raises an error if the group is being declared and build at the declaration statment (not permitted)
								else if(attribs.size() > 1 || xmlparser.hasElement(XmlParser::ChildElement))
								{
									throw Exception(Exception::getErrorMessage(ErrorCode::InvGroupDeclaration)
																	.arg(group).arg(Attributes::HighlightOrder),
																	ErrorCode::InvRedeclarationGroup,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}

								fmt_groups_order.append(group);
							}
							//Case the parser is on the contruction block and not in declaration of groups
							else
							{
								//Raises an error if the group is being constructed by a second time
								if(fmt_groups.contains(group))
								{
									throw Exception(Exception::getErrorMessage(ErrorCode::DefDuplicatedGroup).arg(group),
																	 ErrorCode::DefDuplicatedGroup,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								//Raises an error if the group is being constructed without being declared
								else if(!fmt_groups_order.contains(group))
								{
									throw Exception(Exception::getErrorMessage(ErrorCode::DefNotDeclaredGroup)
																	.arg(group).arg(Attributes::HighlightOrder),
																	ErrorCode::DefNotDeclaredGroup,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								//Raises an error if the group does not have children element
								else if(!xmlparser.hasElement(XmlParser::ChildElement))
								{
									throw Exception(Exception::getErrorMessage(ErrorCode::DefEmptyGroup).arg(group),
																	ErrorCode::DefEmptyGroup,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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

								fmt_groups[group] = FormatGroup(group, format,
																								attribs[Attributes::AllowCompletion] != Attributes::False);
								fmt_grp = &fmt_groups[group];

								xmlparser.savePosition();
								xmlparser.accessElement(XmlParser::ChildElement);

								do
								{
									if(xmlparser.getElementType()==XML_ELEMENT_NODE)
									{
										xmlparser.getElementAttributes(attribs);

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
																			ErrorCode::InvGroupRegExpPattern,
																			__PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr,
																			tr("Pattern: %1").arg(regexp.pattern()));
										}

										fmt_grp->elements.append(ExprElement(pattern,
																												 attribs[Attributes::Persistent] == Attributes::True,
																												 attribs[Attributes::Initial] == Attributes::True,
																												 attribs[Attributes::Final] == Attributes::True,
																												 exact_match,
																												 attribs[Attributes::CaseSensitive] == Attributes::True));
									}
								}
								while(xmlparser.accessElement(XmlParser::NextElement));

								xmlparser.restorePosition();
							}
						}
					}

					/* Check if there are some other groups to be declared, if not,
							continues to reading to the other part of configuration */
					if(groups_decl && !xmlparser.hasElement(XmlParser::NextElement))
					{
						groups_decl = false;
						xmlparser.restorePosition();
					}

				}
				while(xmlparser.accessElement(XmlParser::NextElement));
			}

			for(auto &grp : fmt_groups_order)
			{		
				if(!fmt_groups.contains(grp))
				{
					//Raises an error if the group was declared but not constructed
					throw Exception(Exception::getErrorMessage(ErrorCode::InvGroupDeclarationNotDefined).arg(group),
													ErrorCode::InvGroupDeclarationNotDefined,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

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
		for(auto &elem : fmt_groups[group_name].elements)
			exprs.append(elem.pattern);
	}

	return exprs;
}

QChar SyntaxHighlighter::getCompletionTrigger()
{
	return completion_trigger;
}

void SyntaxHighlighter::setFormat(int start, int count, const QTextCharFormat &fmt)
{
	QTextCharFormat format = fmt;
	format.setFontFamily(default_font.family());
	format.setFontPointSize(getCurrentFontSize());
	QSyntaxHighlighter::setFormat(start, count, format);
}

void SyntaxHighlighter::setDefaultFont(const QFont &fnt)
{
	SyntaxHighlighter::default_font = fnt;
}
