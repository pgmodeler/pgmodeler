/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->setDocument(parent->document());
	this->single_line_mode=single_line_mode;
	configureAttributes();
	parent->installEventFilter(this);

	if(use_custom_tab_width)
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
			parent->setTabStopWidth(NumberedTextEditor::getTabDistance());
		#else
			parent->setTabStopDistance(NumberedTextEditor::getTabDistance());
		#endif
	}

	//Adjusting the size of the parent input according to the current font size
	if(single_line_mode)
	{
		QFontMetrics fm=QFontMetrics(default_font);
		int height=fm.height() + (fm.lineSpacing()/static_cast<double>(1.3));
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
		return true;
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
		setCurrentBlockState(SimpleBlock);
	}

	/* If the previous block info is a open multiline expression the current block will inherit this settings
	 to force the same text formatting */
	if(prev_info && currentBlock().previous().userState()==OpenExprBlock &&
		 currentBlockState() == OpenExprBlock)
	{
		info->group=prev_info->group;
		info->has_exprs=prev_info->has_exprs;
		info->is_expr_closed=false;
		setCurrentBlockState(OpenExprBlock);
	}

	if(!txt.isEmpty())
	{
		QString text, word, group;
		unsigned i=0, len, idx=0, i1;
		int match_idx, match_len, aux_len, start_col;
		QChar chr_delim, lookahead_chr;

		text = txt + QString("\n");
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
					BlockInfo *prev_info = dynamic_cast<BlockInfo *>(currentBlock().previous().userData());

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
					if(i < len && word_delimiters.contains(text[i]) && prev_info && !prev_info->group.isEmpty() && prev_info->has_exprs)
					{
						for(auto exp : final_exprs[prev_info->group])
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
				}

				if(info->has_exprs && !info->is_expr_closed && hasInitialAndFinalExprs(group))
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

		return group;
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
			return QString();
		else
		{
			info->group=group;

			if(!info->has_exprs)
				info->has_exprs=hasInitialAndFinalExprs(group);

			info->is_expr_closed=false;
			return group;
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

	return match;
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
								partial_match=(attribs[Attributes::PartialMatch]==Attributes::True);
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

								if(bold)
									format.setFontWeight(QFont::Bold);
								else
									format.setFontWeight(QFont::Normal);

								format.setForeground(fg_color);
								format.setBackground(bg_color);
								formats[group]=format;


								xmlparser.savePosition();
								xmlparser.accessElement(XmlParser::ChildElement);

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
										expr_type=attribs[Attributes::Type];
										regexp.setPattern(attribs[Attributes::Value]);

										if(attribs[Attributes::RegularExp]==Attributes::True)
											regexp.setPatternSyntax(QRegExp::RegExp2);
										else if(attribs[Attributes::Wildcard]==Attributes::True)
											regexp.setPatternSyntax(QRegExp::Wildcard);
										else
											regexp.setPatternSyntax(QRegExp::FixedString);

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
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

vector<QRegExp> SyntaxHighlighter::getExpressions(const QString &group_name, bool final_expr)
{
	map<QString, vector<QRegExp> > *expr_map=(!final_expr ? &initial_exprs : &final_exprs);

	if(expr_map->count(group_name) > 0)
		return expr_map->at(group_name);
	else
		return vector<QRegExp>();
}

QChar SyntaxHighlighter::getCompletionTrigger()
{
	return completion_trigger;
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
