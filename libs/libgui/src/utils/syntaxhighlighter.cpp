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

QFont SyntaxHighlighter::default_font = QFont("Source Code Pro", 12);
const QString SyntaxHighlighter::UnformattedGroup("__unformatted__");

SyntaxHighlighter::SyntaxHighlighter(QPlainTextEdit *parent, bool single_line_mode, bool use_custom_tab_width, qreal custom_fnt_size) : QSyntaxHighlighter(parent)
{
	if(!parent)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	code_field_txt = parent;
	capt_nearby_separators = false;
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

		if(k_event->key()==Qt::Key_Return || k_event->key()==Qt::Key_Enter)
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
	conf_loaded=false;
}

void SyntaxHighlighter::highlightBlock(const QString &txt)
{
	TextBlockInfo *info=nullptr;
	TextBlockInfo *prev_info=dynamic_cast<TextBlockInfo *>(currentBlock().previous().userData());

	if(!currentBlockUserData())
	{
		info=new TextBlockInfo;
		setCurrentBlockUserData(info);
		setCurrentBlockState(currentBlock().previous().userState());
	}
	else
	{
		//Reset the block's info to permit the rehighlighting
		info=dynamic_cast<TextBlockInfo *>(currentBlockUserData());
		info->reset();
		setCurrentBlockState(SimpleBlock);
	}

	/* If the previous block info is an open multiline expression the current block will inherit this settings
	 * to force the same text formatting.
	 *
	 * There is a special case for empty texts that are inserted right after a
	 * multiline expression, here we force the inheritance of the formatting from the previous block anyway.
	 * This is done in order to avoid the next line (after the blank one) to lost track of the previous formatting */
	if(prev_info && currentBlock().previous().userState()==OpenExprBlock &&
		 (currentBlockState() == OpenExprBlock || (txt.isEmpty() && currentBlockState() < 0)))
	{
		info->setGroup(prev_info->getGroup());
		info->setAllowCompletion(prev_info->isCompletionAllowed());
		info->setMultiExpr(prev_info->isMultiExpr());
		info->setClosed(false);
		setCurrentBlockState(OpenExprBlock);
	}

	if(!txt.isEmpty())
	{
		QString text, word, group;
		unsigned i=0, len, idx=0, i1;
		int match_idx, match_len, aux_len, start_col;
		QChar chr_delim, lookahead_chr;
		bool force_disable_compl = false;

		text = txt + "\n";
		len = text.length();

		do
		{
			//Ignoring the char listed as ingnored on configuration
			while(i < len && ignored_chars.contains(text[i])) i++;

			if(i < len)
			{
				//Stores the curret text positon
				idx=i;

				//If the char is a word separator
				if(word_separators.contains(text[i]))
				{
					while(i < len && word_separators.contains(text[i]))
					{
						word += text[i];
						i++;

						/* If the nearby separators capture is not enabled we must
						 * stop the char capture in order to return the currently formed word */
						if(!capt_nearby_separators)
							break;
					}
				}
				//If the char is a word delimiter
				else if(word_delimiters.contains(text[i]))
				{
					chr_delim = text[i++];
					word += chr_delim;

					while(i < len && chr_delim != text[i])
					{
						word += text[i];
						i++;
					}

					if(i < len && text[i] == chr_delim)
					{
						word += chr_delim;
						i++;
					}
				}
				else
				{
					TextBlockInfo *prev_info = dynamic_cast<TextBlockInfo *>(currentBlock().previous().userData());

					while(i < len &&
						  !word_separators.contains(text[i]) &&
							!ignored_chars.contains(text[i]) &&
							!word_delimiters.contains(text[i]))
					{
						word += text[i];
						i++;
					}

					/* This is an workaround for multi lined groups which use word delimiters
					in their final expressions. In some cases the highlighter can't undertand that
					a multi line group was closed and right after another group starts, this way it
					continues to highlight text as the previous multi lined group.

					An example of that situation is for multi lined string group:

					word delimiter: ' (apostrophe)
					initial-exp: (')(.)*(\n)
					final-exp: (.)*(')(\n)*

					String:	'lorem\n ipsum' nextword

					In the example above, without the workaround, the highlighter would highlight the first line
					"'lorem\n" as string and continue to hightlight the " ipsum' nextword" in the same way as well,
					this because the final expression of the group contains the word delimiter '. In order to force the highlight stop
					in the last ' we include it in the current evaluated word and increment the position in the text so the next
					word starts without the word delimiter. */
					if(i < len && word_delimiters.contains(text[i]) &&
						 prev_info && !prev_info->getGroup().isEmpty() && prev_info->isMultiExpr())
					{
						for(auto &exp : final_exprs[prev_info->getGroup()])
						{
							if(exp.pattern().contains(text[i]))
							{
								word += text[i];
								i++;
								break;
							}
						}
					}
				}
			}

			//If the word is not empty try to identify the group
			if(!word.isEmpty())
			{
				i1=i;
				while(i1 < len && ignored_chars.contains(text[i1])) i1++;

				if(i1 < len)
					lookahead_chr=text[i1];
				else
					lookahead_chr='\0';

				match_idx=-1;
				match_len=0;

				group=identifyWordGroup(word, lookahead_chr, match_idx, match_len);

				if(!group.isEmpty())
				{
					start_col=idx + match_idx;
					setFormat(start_col, match_len, group);

					/* Workaround to avoid the code completion to be triggered inside a delimited string when typing the
					 * completion trigger char. If the cursor is within a delimited string like:
					 *
					 * 'some [cursor]string' other text outside delimiter)
					 *
					 * And there are other text typed ahead of the end of the delimited string, we need to force the code
					 * completion disabling inside that block delimited even the other texts (outside the delimiters) are
					 * from a group in which the completion is allowed in the settings. */
					int cursor_pos = code_field_txt->textCursor().positionInBlock();
					if(!info->isCompletionAllowed() && !force_disable_compl &&
						 cursor_pos >= static_cast<int>(idx) && cursor_pos <= static_cast<int>(idx + match_len))
					{
						force_disable_compl = true;
					}
					else if(force_disable_compl)
						info->setAllowCompletion(false);
				}

				if(info->isMultiExpr() && !info->isClosed() && hasInitialAndFinalExprs(group))
					setCurrentBlockState(OpenExprBlock);
				else
					setCurrentBlockState(SimpleBlock);

				aux_len=(match_idx + match_len);
				if(match_idx >=0 &&  aux_len != word.length())
					i-=word.length() - aux_len;

				word.clear();
			}
		}
		while(i < len);
	}
}

QString SyntaxHighlighter::identifyWordGroup(const QString &word, const QChar &lookahead_chr, int &match_idx, int &match_len)
{
	QString group;
	bool match = false;
	int block_st = currentBlockState();
	bool match_final_expr= block_st == OpenExprBlock;
	TextBlockInfo *info = dynamic_cast<TextBlockInfo *>(currentBlockUserData()),
			*prev_info = dynamic_cast<TextBlockInfo *>(currentBlock().previous().userData());

	/* Trying to match the word against the gropus
	 * If the block is marked as OpenExprBlock we'll match the word agains final expression
	 * in order to check if the word is token that closes a multi line expression */
	for(auto &itr_group : groups_order)
	{
		if(isWordMatchGroup(word, itr_group, match_final_expr, lookahead_chr, match_idx, match_len))
		{
			group = itr_group;
			match = true;
			break;
		}
	}

	/* If the word doesn't match any group or it matches but the found group is differnt
	 * from the current block info group, we need to return a group name that considers
	 * the state of the previous block (which can be previous line or previous word in the current line) */
	if(!match || (match && block_st == OpenExprBlock && info->getGroup() != group))
	{
		match_idx = 0;
		match_len = word.length();

		/* If the current block info is a multi expression one and it is not closed yet (the closing
		 * expresion wasn't found previously) we automatically return the info's group to enforce the
		 * highlighting of the word using the formatting of the info's group no matter if the
		 * word was matched against another group. The fact of the current info is a multi line expression
		 * that is still open has precedence over any other situation. */
		if(!info->getGroup().isEmpty() && info->isMultiExpr() && !info->isClosed())
		{
			return info->getGroup();
		}
		/* If the word is a simple line feed (\n) and the previous block info is a multi expression one
		 * that is closed we force the current block to be unformatted. This will avoid that the next line
		 * starts with the formatting of the previous block info. */
		else if((word == QChar::LineFeed && prev_info &&
						 !prev_info->getGroup().isEmpty() && prev_info->isMultiExpr() && prev_info->isClosed()) ||

						/* If the word doens't match any group and the current block info was closed at least once
						 * in the current line we also need to force the unformatted mode so the next words in the
						 * line can be correctly highlighted. For example, supposing the following line in XML syntax:
						 *  <!-- comment --> <tag>
						 * Without forcing the format reseting after the token '-->' the next one '<tag>' would receive
						 * the same highlighting as a XML comment which is not desired. */
						(group.isEmpty() && info->isClosedOnce()) ||
						(!info->getGroup().isEmpty() && info->isMultiExpr() && info->isClosed()))
		{
			/* We force unformmated group to stay closed so in the next interaction (word extraction / validation)
			 * the group that matches the word can be properly found */
			info->setGroup(UnformattedGroup);
			info->setAllowCompletion(allow_completion[UnformattedGroup]);
			info->setMultiExpr(true);
			info->setClosed(true);
			return info->getGroup();
		}
		/* In case of the word doesn't match no group and none of the conditions regarding the current block info are
		 * met we force the inheritance of formatting style of the previous block (if existent) and if the previous
		 * block is a multi expression one and is already open. An example of this situation would be adding a new
		 * text inside a multi line comment:
		 *
		 * <!--
		 *  some comment
		 * -->
		 *
		 * In this case the words "some comment" don't match any group this way they need to be commented since
		 * they were placed between the comment tokens */
		else if(prev_info && !prev_info->getGroup().isEmpty() && prev_info->isMultiExpr() && !prev_info->isClosed())
		{
			info->setGroup(prev_info->getGroup());
			info->setAllowCompletion(prev_info->isCompletionAllowed());

			/* We force the current info to be a multi expression one (like the previous)
			 * and close it only if the word is a closing token of the group */
			info->setMultiExpr(true);
			info->setClosed(isWordMatchGroup(word, prev_info->getGroup(), true, lookahead_chr, match_idx, match_len));

			return info->getGroup();
		}

		return UnformattedGroup;
	}
	else
	{
		/* If no group is found related to the word we inherit the previous block
		 * formatting if and only if the previous block is a multi line expressoin that
		 * is not closed */
		if(!group.isEmpty() && info->getGroup().isEmpty() &&
			 prev_info && prev_info->isMultiExpr() && !prev_info->isClosed())
		{
			info->setGroup(prev_info->getGroup());
			info->setAllowCompletion(prev_info->isCompletionAllowed());
			info->setMultiExpr(true);

			/* We try to check if the current work matches a final expression of the previous block group
			 * if so, the current block info will be automatically closed */
			info->setClosed(isWordMatchGroup(word, prev_info->getGroup(), true, lookahead_chr, match_idx, match_len));

			return info->getGroup();
		}

		info->setGroup(group);
		info->setAllowCompletion(group.isEmpty() ? true : allow_completion[group]);
		info->setMultiExpr(hasInitialAndFinalExprs(group));
		info->setClosed(match && match_final_expr);

		return group;
	}
}

bool SyntaxHighlighter::isWordMatchGroup(const QString &word, const QString &group, bool use_final_expr, const QChar &lookahead_chr, int &match_idx, int &match_len)
{
	std::vector<QRegularExpression> *vet_expr = nullptr;
	bool has_match = false;
	QRegularExpressionMatch match;

	if(use_final_expr && final_exprs.count(group))
		vet_expr = &final_exprs[group];
	else
		vet_expr = &initial_exprs[group];

	for(auto &expr : *vet_expr)
	{
		if(expr.match(word).hasMatch())
		{
			match_idx = 0;
			match_len = word.length();
			has_match = true;
		}

		if(has_match && lookahead_char.count(group) > 0 && lookahead_chr!=lookahead_char.at(group))
			has_match =false;

		if(has_match)
			break;
	}

	return has_match;
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
	initial_exprs.clear();
	final_exprs.clear();
	formats.clear();
	groups_order.clear();
	word_separators.clear();
	word_delimiters.clear();
	ignored_chars.clear();
	lookahead_char.clear();

	configureAttributes();
}

void SyntaxHighlighter::loadConfiguration(const QString &filename)
{
	if(!filename.isEmpty())
	{
		attribs_map attribs;
		QString elem, expr_type, group;
		bool groups_decl=false, chr_sensitive=false,
				bold=false, italic=false, strikeout = false,
				underline=false;
		QTextCharFormat format;
		QRegularExpression regexp;
		QColor bg_color, fg_color;
		std::vector<QString>::iterator itr, itr_end;

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
						elem=xmlparser.getElementName();

						if(elem==Attributes::WordSeparators)
						{
							xmlparser.getElementAttributes(attribs);
							word_separators=attribs[Attributes::Value];
							capt_nearby_separators = attribs[Attributes::CaptureNearby] == Attributes::True;
						}
						else if(elem==Attributes::WordDelimiters)
						{
							xmlparser.getElementAttributes(attribs);
							word_delimiters=attribs[Attributes::Value];
						}
						else if(elem==Attributes::IgnoredChars)
						{
							xmlparser.getElementAttributes(attribs);
							ignored_chars=attribs[Attributes::Value];
						}
						else if(elem==Attributes::CompletionTrigger)
						{
							xmlparser.getElementAttributes(attribs);

							if(attribs[Attributes::Value].size() >= 1)
								completion_trigger=attribs[Attributes::Value].at(0);
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
						else if(elem==Attributes::HighlightOrder)
						{
							//Marks a flag indication that groups are being declared
							groups_decl=true;
							xmlparser.savePosition();
							xmlparser.accessElement(XmlParser::ChildElement);
							elem=xmlparser.getElementName();
						}

						if(elem==Attributes::Group)
						{
							xmlparser.getElementAttributes(attribs);
							group=attribs[Attributes::Name];

							/* If the parser is on the group declaration block and not in the build block
								 some validations are made. */
							if(groups_decl)
							{
								//Raises an error if the group was declared before
								if(find(groups_order.begin(), groups_order.end(), group)!=groups_order.end())
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

								groups_order.push_back(group);
							}
							//Case the parser is on the contruction block and not in declaration of groups
							else
							{
								//Raises an error if the group is being constructed by a second time
								if(initial_exprs.count(group)!=0)
								{
									throw Exception(Exception::getErrorMessage(ErrorCode::DefDuplicatedGroup).arg(group),
																	ErrorCode::DefDuplicatedGroup,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								//Raises an error if the group is being constructed without being declared
								else if(find(groups_order.begin(), groups_order.end(), group)==groups_order.end())
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

								chr_sensitive=(attribs[Attributes::CaseSensitive]==Attributes::True);
								italic=(attribs[Attributes::Italic]==Attributes::True);
								bold=(attribs[Attributes::Bold]==Attributes::True);
								underline=(attribs[Attributes::Underline]==Attributes::True);
								strikeout=(attribs[Attributes::Stikeout]==Attributes::True);
								fg_color.setNamedColor(attribs[Attributes::ForegroundColor]);

								//If the attribute isn't defined the bg color will be transparent
								if(attribs[Attributes::BackgroundColor].isEmpty())
									bg_color.setRgb(0,0,0,0);
								else
									bg_color.setNamedColor(attribs[Attributes::BackgroundColor]);

								if(!attribs[Attributes::LookaheadChar].isEmpty())
									lookahead_char[group]=attribs[Attributes::LookaheadChar][0];

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
								formats[group]=format;

								allow_completion[group] =
										attribs[Attributes::AllowCompletion] != Attributes::False;

								xmlparser.savePosition();
								xmlparser.accessElement(XmlParser::ChildElement);

								if(!chr_sensitive)
									regexp.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

								do
								{
									if(xmlparser.getElementType()==XML_ELEMENT_NODE)
									{
										xmlparser.getElementAttributes(attribs);
										expr_type=attribs[Attributes::Type];

										if(attribs[Attributes::RegularExp] == Attributes::True)
											regexp.setPattern(attribs[Attributes::Value]);
										else if(attribs[Attributes::Wildcard] == Attributes::True)
											regexp.setPattern(QRegularExpression::wildcardToRegularExpression(attribs[Attributes::Value]));
										else
											regexp.setPattern(QRegularExpression::anchoredPattern(QRegularExpression::escape(attribs[Attributes::Value])));

										// We thrown an error aborting the loading if the regepx has an invalid pattern
										if(!regexp.isValid())
										{
											throw Exception(Exception::getErrorMessage(ErrorCode::InvGroupRegExpPattern).arg(group, filename, regexp.errorString()),
																			ErrorCode::InvGroupRegExpPattern,
																			__PRETTY_FUNCTION__, __FILE__, __LINE__, nullptr, tr("Pattern: %1").arg(regexp.pattern()));
										}

										if(expr_type.isEmpty() ||
											 expr_type==Attributes::SimpleExp ||
											 expr_type==Attributes::InitialExp)
											initial_exprs[group].push_back(regexp);
										else
											final_exprs[group].push_back(regexp);
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
						groups_decl=false;
						xmlparser.restorePosition();
					}

				}
				while(xmlparser.accessElement(XmlParser::NextElement));
			}

			itr=groups_order.begin();
			itr_end=groups_order.end();

			while(itr!=itr_end)
			{
				group=(*itr);
				itr++;

				if(initial_exprs[group].size()==0)
				{
					//Raises an error if the group was declared but not constructed
					throw Exception(Exception::getErrorMessage(ErrorCode::InvGroupDeclarationNotDefined).arg(group),
													ErrorCode::InvGroupDeclarationNotDefined,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			conf_loaded=true;
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e, filename);
		}
	}
}

std::vector<QRegularExpression> SyntaxHighlighter::getExpressions(const QString &group_name, bool final_expr)
{
	std::map<QString, std::vector<QRegularExpression> > *expr_map=(!final_expr ? &initial_exprs : &final_exprs);

	if(expr_map->count(group_name) > 0)
		return expr_map->at(group_name);
	else
		return std::vector<QRegularExpression>();
}

QChar SyntaxHighlighter::getCompletionTrigger()
{
	return completion_trigger;
}

void SyntaxHighlighter::setFormat(int start, int count, const QString &group)
{
	QTextCharFormat format=formats[group];
	format.setFontFamily(default_font.family());
	//format.setFontPointSize(default_font.pointSizeF());
	format.setFontPointSize(getCurrentFontSize());
	QSyntaxHighlighter::setFormat(start, count, format);
}

void SyntaxHighlighter::setDefaultFont(const QFont &fnt)
{
	SyntaxHighlighter::default_font=fnt;
}
