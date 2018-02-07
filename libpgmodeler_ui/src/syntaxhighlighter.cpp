/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "numberedtexteditor.h"

QFont SyntaxHighlighter::default_font=QFont(QString("Source Code Pro"), 10);

SyntaxHighlighter::SyntaxHighlighter(QPlainTextEdit *parent, bool single_line_mode, bool use_custom_tab_width) : QSyntaxHighlighter(parent)
{
	if(!parent)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->setDocument(parent->document());
	this->single_line_mode=single_line_mode;
	configureAttributes();
	parent->installEventFilter(this);

	if(use_custom_tab_width)
		parent->setTabStopWidth(NumberedTextEditor::getTabWidth());

	//Adjusting the size of the parent input according to the current font size
	if(single_line_mode)
	{
		QFontMetrics fm=QFontMetrics(default_font);
		int height=fm.height() + (fm.lineSpacing()/static_cast<float>(1.3));
		parent->setMinimumHeight(height);
		parent->setMaximumHeight(height);
		parent->setSizePolicy(parent->sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);
		parent->adjustSize();
	}
}

bool SyntaxHighlighter::eventFilter(QObject *object, QEvent *event)
{
	//Filters the ENTER/RETURN avoiding line breaks
	if(this->single_line_mode &&
			event->type() == QEvent::KeyPress &&
			(dynamic_cast<QKeyEvent *>(event)->key()==Qt::Key_Return ||
			 dynamic_cast<QKeyEvent *>(event)->key()==Qt::Key_Enter))
	{
		event->ignore();
		return(true);
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

	return(QSyntaxHighlighter::eventFilter(object, event));
}

bool SyntaxHighlighter::hasInitialAndFinalExprs(const QString &group)
{
	return(initial_exprs.count(group) && final_exprs.count(group));
}

void SyntaxHighlighter::configureAttributes(void)
{
	conf_loaded=false;
}

void SyntaxHighlighter::highlightBlock(const QString &txt)
{
	BlockInfo *info=nullptr;
	BlockInfo *prev_info=dynamic_cast<BlockInfo *>(currentBlock().previous().userData());

	if(!currentBlockUserData())
	{
		info=new BlockInfo;
		setCurrentBlockUserData(info);
	}
	else
	{
		//Reset the block's info to permit the rehighlighting
		info=dynamic_cast<BlockInfo *>(currentBlockUserData());
		info->resetBlockInfo();
		setCurrentBlockState(SIMPLE_BLOCK);
	}

	/* If the previous block info is a open multiline expression the current block will inherit this settings
	 to force the same text formatting */
	if(prev_info && currentBlock().previous().userState()==OPEN_EXPR_BLOCK)
	{
		info->group=prev_info->group;
		info->has_exprs=prev_info->has_exprs;
		info->is_expr_closed=false;
		setCurrentBlockState(OPEN_EXPR_BLOCK);
	}

	if(!txt.isEmpty())
	{
		QString text=txt + QChar('\n'), word, group;
		unsigned i=0, len, idx=0, i1;
		int match_idx, match_len, aux_len, start_col;
		QChar chr_delim, lookahead_chr;
		len=text.length();

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
						word+=text[i++];
				}
				//If the char is a word delimiter
				else if(word_delimiters.contains(text[i]))
				{
					chr_delim=text[i++];
					word+=chr_delim;

					while(i < len && chr_delim!=text[i])
						word+=text[i++];

					if(i < len && text[i]==chr_delim)
					{
						word+=chr_delim;
						i++;
					}
				}
				else
				{
					while(i < len &&
						  !word_separators.contains(text[i]) &&
						  !word_delimiters.contains(text[i]) &&
						  !ignored_chars.contains(text[i]))
					{
						word+=text[i++];
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
				}

				if(info->has_exprs && !info->is_expr_closed && hasInitialAndFinalExprs(group))
					setCurrentBlockState(OPEN_EXPR_BLOCK);
				else
					setCurrentBlockState(SIMPLE_BLOCK);

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
	bool match=false;
	BlockInfo *info=dynamic_cast<BlockInfo *>(currentBlockUserData()),
			*prev_info=dynamic_cast<BlockInfo *>(currentBlock().previous().userData());

	if((info->has_exprs && !info->is_expr_closed && hasInitialAndFinalExprs(info->group)) ||
			(prev_info && !info->has_exprs && prev_info->has_exprs && !prev_info->is_expr_closed))
	{
		if(prev_info && !info->has_exprs)
			group=prev_info->group;
		else
			group=info->group;

		match=isWordMatchGroup(word, group, true, lookahead_chr, match_idx, match_len);

		//If the word match one final expression marks the current block info as closed
		if(match)
			info->is_expr_closed=true;
		else
		{
			match_idx=0;
			match_len=word.length();
		}

		info->has_exprs=hasInitialAndFinalExprs(group);
		info->group=group;

		return(group);
	}
	else
	{
		for(auto &itr_group : groups_order)
		{
			group=itr_group;
			if(isWordMatchGroup(word, group, false, lookahead_chr, match_idx, match_len))
			{
				match=true;
				break;
			}
		}

		if(!match)
			return(QString());
		else
		{
			info->group=group;

			if(!info->has_exprs)
				info->has_exprs=hasInitialAndFinalExprs(group);

			info->is_expr_closed=false;
			return(group);
		}
	}
}

bool SyntaxHighlighter::isWordMatchGroup(const QString &word, const QString &group, bool use_final_expr, const QChar &lookahead_chr, int &match_idx, int &match_len)
{
	vector<QRegExp> *vet_expr=nullptr;
	bool match=false, part_match=partial_match[group];

	if(use_final_expr && final_exprs.count(group))
		vet_expr=&final_exprs[group];
	else
		vet_expr=&initial_exprs[group];

	for(auto &expr : *vet_expr)
	{
		if(part_match)
		{
			match_idx=word.indexOf(expr);
			match_len=expr.matchedLength();
			match=(match_idx >= 0);
		}
		else
		{
			if(expr.patternSyntax()==QRegExp::FixedString)
				match=((expr.pattern().compare(word, expr.caseSensitivity())==0));
			else
				match=expr.exactMatch(word);

			if(match)
			{
				match_idx=0;
				match_len=word.length();
			}
		}

		if(match && lookahead_char.count(group) > 0 && lookahead_chr!=lookahead_char.at(group))
			match=false;

		if(match) break;
	}

	return(match);
}

bool SyntaxHighlighter::isConfigurationLoaded(void)
{
	return(conf_loaded);
}

void SyntaxHighlighter::clearConfiguration(void)
{
	initial_exprs.clear();
	final_exprs.clear();
	formats.clear();
	partial_match.clear();
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
				bold=false, italic=false,
				underline=false, partial_match=false;
		QTextCharFormat format;
		QRegExp regexp;
		QColor bg_color, fg_color;
		vector<QString>::iterator itr, itr_end;

		try
		{
			clearConfiguration();
			xmlparser.restartParser();
			xmlparser.setDTDFile(GlobalAttributes::TMPL_CONFIGURATIONS_DIR +
								 GlobalAttributes::DIR_SEPARATOR +
								 GlobalAttributes::OBJECT_DTD_DIR +
								 GlobalAttributes::DIR_SEPARATOR +
								 GlobalAttributes::CODE_HIGHLIGHT_CONF +
								 GlobalAttributes::OBJECT_DTD_EXT,
								 GlobalAttributes::CODE_HIGHLIGHT_CONF);

			xmlparser.loadXMLFile(filename);

			if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
			{
				do
				{
					if(xmlparser.getElementType()==XML_ELEMENT_NODE)
					{
						elem=xmlparser.getElementName();

						if(elem==ParsersAttributes::WORD_SEPARATORS)
						{
							xmlparser.getElementAttributes(attribs);
							word_separators=attribs[ParsersAttributes::VALUE];
						}
						else if(elem==ParsersAttributes::WORD_DELIMITERS)
						{
							xmlparser.getElementAttributes(attribs);
							word_delimiters=attribs[ParsersAttributes::VALUE];
						}
						else if(elem==ParsersAttributes::IGNORED_CHARS)
						{
							xmlparser.getElementAttributes(attribs);
							ignored_chars=attribs[ParsersAttributes::VALUE];
						}
						else if(elem==ParsersAttributes::COMPLETION_TRIGGER)
						{
							xmlparser.getElementAttributes(attribs);

							if(attribs[ParsersAttributes::VALUE].size() >= 1)
								completion_trigger=attribs[ParsersAttributes::VALUE].at(0);
						}

						/*	If the element is what defines the order of application of the groups
								highlight in the (highlight-order). Is in this block that are declared
								the groups used to highlight the source code. ALL groups
								in this block must be declared before they are built
								otherwise an error will be triggered. */
						else if(elem==ParsersAttributes::HIGHLIGHT_ORDER)
						{
							//Marks a flag indication that groups are being declared
							groups_decl=true;
							xmlparser.savePosition();
							xmlparser.accessElement(XMLParser::CHILD_ELEMENT);
							elem=xmlparser.getElementName();
						}

						if(elem==ParsersAttributes::GROUP)
						{
							xmlparser.getElementAttributes(attribs);
							group=attribs[ParsersAttributes::NAME];

							/* If the parser is on the group declaration block and not in the build block
								 some validations are made. */
							if(groups_decl)
							{
								//Raises an error if the group was declared before
								if(find(groups_order.begin(), groups_order.end(), group)!=groups_order.end())
								{
									throw Exception(Exception::getErrorMessage(ERR_REDECL_HL_GROUP).arg(group),
													ERR_REDECL_HL_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								//Raises an error if the group is being declared and build at the declaration statment (not permitted)
								else if(attribs.size() > 1 || xmlparser.hasElement(XMLParser::CHILD_ELEMENT))
								{
									throw Exception(Exception::getErrorMessage(ERR_DEF_INV_GROUP_DECL)
													.arg(group).arg(ParsersAttributes::HIGHLIGHT_ORDER),
													ERR_REDECL_HL_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}

								groups_order.push_back(group);
							}
							//Case the parser is on the contruction block and not in declaration of groups
							else
							{
								//Raises an error if the group is being constructed by a second time
								if(initial_exprs.count(group)!=0)
								{
									throw Exception(Exception::getErrorMessage(ERR_DEF_DUPLIC_GROUP).arg(group),
													ERR_DEF_DUPLIC_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								//Raises an error if the group is being constructed without being declared
								else if(find(groups_order.begin(), groups_order.end(), group)==groups_order.end())
								{
									throw Exception(Exception::getErrorMessage(ERR_DEF_NOT_DECL_GROUP)
													.arg(group).arg(ParsersAttributes::HIGHLIGHT_ORDER),
													ERR_DEF_NOT_DECL_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								//Raises an error if the group does not have children element
								else if(!xmlparser.hasElement(XMLParser::CHILD_ELEMENT))
								{
									throw Exception(Exception::getErrorMessage(ERR_DEF_EMPTY_GROUP).arg(group),
													ERR_DEF_EMPTY_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}

								chr_sensitive=(attribs[ParsersAttributes::CASE_SENSITIVE]==ParsersAttributes::_TRUE_);
								italic=(attribs[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_);
								bold=(attribs[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_);
								underline=(attribs[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_);
								partial_match=(attribs[ParsersAttributes::PARTIAL_MATCH]==ParsersAttributes::_TRUE_);
								fg_color.setNamedColor(attribs[ParsersAttributes::FOREGROUND_COLOR]);

								//If the attribute isn't defined the bg color will be transparent
								if(attribs[ParsersAttributes::BACKGROUND_COLOR].isEmpty())
									bg_color.setRgb(0,0,0,0);
								else
									bg_color.setNamedColor(attribs[ParsersAttributes::BACKGROUND_COLOR]);

								if(!attribs[ParsersAttributes::LOOKAHEAD_CHAR].isEmpty())
									lookahead_char[group]=attribs[ParsersAttributes::LOOKAHEAD_CHAR][0];

								format.setFontFamily(default_font.family());
								format.setFontPointSize(default_font.pointSizeF());
								format.setFontItalic(italic);
								format.setFontUnderline(underline);

								if(bold)
									format.setFontWeight(QFont::Bold);
								else
									format.setFontWeight(QFont::Normal);

								format.setForeground(fg_color);
								format.setBackground(bg_color);
								formats[group]=format;


								xmlparser.savePosition();
								xmlparser.accessElement(XMLParser::CHILD_ELEMENT);

								if(chr_sensitive)
									regexp.setCaseSensitivity(Qt::CaseSensitive);
								else
									regexp.setCaseSensitivity(Qt::CaseInsensitive);

								this->partial_match[group]=partial_match;

								do
								{
									if(xmlparser.getElementType()==XML_ELEMENT_NODE)
									{
										xmlparser.getElementAttributes(attribs);
										expr_type=attribs[ParsersAttributes::TYPE];
										regexp.setPattern(attribs[ParsersAttributes::VALUE]);

										if(attribs[ParsersAttributes::REGULAR_EXP]==ParsersAttributes::_TRUE_)
											regexp.setPatternSyntax(QRegExp::RegExp2);
										else if(attribs[ParsersAttributes::WILDCARD]==ParsersAttributes::_TRUE_)
											regexp.setPatternSyntax(QRegExp::Wildcard);
										else
											regexp.setPatternSyntax(QRegExp::FixedString);

										if(expr_type.isEmpty() ||
												expr_type==ParsersAttributes::SIMPLE_EXP ||
												expr_type==ParsersAttributes::INITIAL_EXP)
											initial_exprs[group].push_back(regexp);
										else
											final_exprs[group].push_back(regexp);
									}
								}
								while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
								xmlparser.restorePosition();
							}
						}
					}

					/* Check if there are some other groups to be declared, if not,
							continues to reading to the other part of configuration */
					if(groups_decl && !xmlparser.hasElement(XMLParser::NEXT_ELEMENT))
					{
						groups_decl=false;
						xmlparser.restorePosition();
					}

				}
				while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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
					throw Exception(Exception::getErrorMessage(ERR_GROUP_DECL_NOT_DEFINED).arg(group),
									ERR_GROUP_DECL_NOT_DEFINED,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			conf_loaded=true;
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

vector<QRegExp> SyntaxHighlighter::getExpressions(const QString &group_name, bool final_expr)
{
	map<QString, vector<QRegExp> > *expr_map=(!final_expr ? &initial_exprs : &final_exprs);

	if(expr_map->count(group_name) > 0)
		return(expr_map->at(group_name));
	else
		return(vector<QRegExp>());
}

QChar SyntaxHighlighter::getCompletionTrigger(void)
{
	return(completion_trigger);
}

void SyntaxHighlighter::setFormat(int start, int count, const QString &group)
{
	QTextCharFormat format=formats[group];
	format.setFontFamily(default_font.family());
	format.setFontPointSize(default_font.pointSizeF());
	QSyntaxHighlighter::setFormat(start, count, format);
}

void SyntaxHighlighter::setDefaultFont(const QFont &fnt)
{
	SyntaxHighlighter::default_font=fnt;
}
