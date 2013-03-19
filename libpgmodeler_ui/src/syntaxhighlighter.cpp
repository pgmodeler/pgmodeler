/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, bool auto_rehighlight) : QSyntaxHighlighter(parent)
{
	this->auto_rehighlight=auto_rehighlight;
	configureAttributes();
}

SyntaxHighlighter::SyntaxHighlighter(QTextEdit *parent, bool auto_rehighlight) : QSyntaxHighlighter(parent)
{
	this->auto_rehighlight=auto_rehighlight;
	configureAttributes();
}

void SyntaxHighlighter::configureAttributes(void)
{
	conf_loaded=false;
	current_block=-1;
	curr_blk_info_count=0;

	if(auto_rehighlight)
	{
		connect(document(), SIGNAL(blockCountChanged(int)), this, SLOT(rehighlight(void)));
		connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(validateTextModification(int,int,int)));
	}
}

void SyntaxHighlighter::validateTextModification(int, int removed, int added)
{
	if(getMultiLineInfoCount(current_block)!=curr_blk_info_count ||
		 added > 0 || removed > 0)
		rehighlight();
}

SyntaxHighlighter::MultiLineInfo *SyntaxHighlighter::getMultiLineInfo(int start_col, int end_col, int block)
{
	unsigned i, count;
	bool found=false;
	MultiLineInfo *info=NULL;

	//Checking if the passed parameters is inside a multiline info
	count=multi_line_infos.size();
	for(i=0; i < count; i++)
	{
		info=multi_line_infos[i];

		/* First we need to check if the block passed is within the limits
		established by the first and last blocks of multiline info.
		Importantly, when a multi block is opened, for example,	the user opened a
		multiline with '/ *' and not closed with '* /' the attributes end_block and end_col
		possess the value of -1 until the user closes the multiline and the highlighter identifies
		which part this text is closing */
		if(block >= info->start_block && (info->end_block < 0 || block <= info->end_block))
		{
			/* Next, some conditions are tested in order to verify if the passed parameters
			are inside a mulitline block */

			/* Condition 1: The passed block is the same as the current info and this latter
											is a multiline but opened and closed on the same line of text,
											will be checked	if parameter start_col and end_col are within
											the limits stablished bythe starting and ending columns on the info */
			if(block==info->start_block && info->start_block==info->end_block)
				found=(start_col >= info->start_col && end_col <= info->end_col);

			/* Condition 2: The passed block is the same as current info and this latter is a opened
											multiline. Tests only if the initial column from parameter is after the
											initial column of info.	This indicates that the text is entered after the
											opening of multiline and consequently	within the same */
			else if(block == info->start_block)
				found=(start_col >= info->start_col);

			/* Condition 3: The passed block is the same as the last block of the current info and this latter is
											a closed multiline. Tests only if the parameter is the final column before the final
											column of the info indicating that the current multiline text is inserted
											into the block multiline */
			else if(info->end_block >=0 && block == info->end_block)
				found=(end_col <= info->end_col);

			/* Condition 4: The current information is a opened multiline. Only tests if the passed block is in
											the same initial block as the information or after without needing to test the columns
											and the final block. This is done because if the text is inserted in the middle
											of multiline block after opening, and as the block is open, all text entered after
											the block is considered an open multiblock */
			else if(info->end_block < 0)
				found=(block >= info->start_block);

			/* Conditional 5:	 The current info is a closed multiline. Tests only if the passed block is in
												 the middle of the range established by the first and last blocks of multiline info.
												 This is done because if the text is inserted in the middle of multiline after opening block
												 and before closing the whole line of text is considered a multiblock */
			else if(info->end_block >=0 && info->start_block!=info->end_block)
				found=(block >= info->start_block && block <= info->end_block);
		}
	}

	if(found)
		return(info);
	else
		return(NULL);
}

void SyntaxHighlighter::removeMultiLineInfo(int block)
{
	vector<MultiLineInfo *>::iterator itr, itr_end;

	itr=multi_line_infos.begin();
	itr_end=multi_line_infos.end();

	while(itr!=itr_end)
	{
		if((*itr)->start_block==block)
		{
			delete(*itr);
			multi_line_infos.erase(itr);
			itr=multi_line_infos.begin();
			itr_end=multi_line_infos.end();
		}
		else
			itr++;
	}
}

unsigned SyntaxHighlighter::getMultiLineInfoCount(int block)
{
	vector<MultiLineInfo *>::iterator itr, itr_end;
	unsigned count=0;

	itr=multi_line_infos.begin();
	itr_end=multi_line_infos.end();

	while(itr!=itr_end)
	{
		if((*itr)->start_block==block) count++;
		itr++;
	}

	return(count);
}

QString SyntaxHighlighter::identifyWordGroup(const QString &word, const QChar &lookahead_chr, int idx, int &match_idx, int &match_len)
{
	QRegExp expr;
	vector<QString>::iterator itr, itr_end;
	vector<QRegExp>::iterator itr_exp, itr_exp_end;
	vector<QRegExp> *vet_expr=NULL;
	QString group;
	bool match=false, part_mach=false;
	MultiLineInfo *info=NULL;

	//Try to get the multiline info for the current block
	info=getMultiLineInfo(idx, idx, current_block);

	/* Case the highlighter is in the middle of a multiline code block,
		 a different action is executed: check if the current word does not
		 matches with one of final expresion of the group indicating that the
		 group highlighting must be interrupted after the current word */
	if(info)
	{
		group=info->group;

		//Checking if the word is not a highlight ending for the group
		itr_exp=final_exprs[group].begin();
		itr_exp_end=final_exprs[group].end();
		part_mach=partial_match[group];

		while(itr_exp!=itr_exp_end && !match)
		{
			expr=(*itr_exp);

			if(part_mach)
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

			itr_exp++;
		}

		/* If the word matches configures a multiline info with the
			 values retrieved from the regexp matching */
		if(match)
		{
			info->end_col=idx + match_idx + match_len-1;
			info->end_block=current_block;
		}
		else
		{
			match_idx=0;
			match_len=word.length();
		}

		return(group);
	}
	else
	{
		itr=groups_order.begin();
		itr_end=groups_order.end();

		while(itr!=itr_end && !match)
		{
			group=(*itr);
			vet_expr=&initial_exprs[group];
			itr++;

			itr_exp=vet_expr->begin();
			itr_exp_end=vet_expr->end();
			part_mach=partial_match[group];

			while(itr_exp!=itr_exp_end && !match)
			{
				expr=(*itr_exp);

				if(part_mach)
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

				itr_exp++;
			}

			/* Case the word matches with one of group regexp check if this latter
				 has final expressions which indicates that the group treats multiline blocks.
				 This way alocates a info with the initial configurations */
			if(match && final_exprs.count(group))
			{
				if(!info)
				{
					info=new MultiLineInfo;
					info->group=group;
					info->start_col=idx + match_idx + match_len;
					info->start_block=current_block;
					multi_line_infos.push_back(info);
				}
			}
		}

		if(!match) group="";
		return(group);
	}
}

void SyntaxHighlighter::rehighlight(void)
{
	MultiLineInfo *info=NULL;

	/* Remove all the multiline infos because during the rehighlight
		 all them all gathered again */
	while(!multi_line_infos.empty())
	{
		info=multi_line_infos.back();
		multi_line_infos.pop_back();
		delete(info);
	}

	QSyntaxHighlighter::rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString &txt)
{
	current_block=currentBlock().blockNumber();

	if(!txt.isEmpty())
	{
		QString word, group, text;
		unsigned i=0, len, idx=0, i1;
		int match_idx, match_len, aux_len, start_col;
		QChar chr_delim, lookahead_chr;

		text=txt + '\n';
		len=text.length();
		removeMultiLineInfo(current_block);

		do
		{
			//Ignoring the char listed as ingnored on configuration
			while(i < len && ignored_chars.indexOf(text[i])>=0) i++;

			if(i < len)
			{
				//Stores the curret text positon
				idx=i;

				//If the char is a word separator
				if(word_separators.indexOf(text[i])>=0)
				{
					while(i < len && word_separators.indexOf(text[i])>=0)
						word+=text[i++];
				}
				//If the char is a word delimiter
				else if(word_delimiters.indexOf(text[i])>=0)
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
								word_separators.indexOf(text[i]) < 0 &&
								word_delimiters.indexOf(text[i]) < 0 &&
								ignored_chars.indexOf(text[i]) < 0)
					{
						word+=text[i++];
					}
				}
			}

			//If the word is not empty try to identify the group
			if(!word.isEmpty())
			{
				i1=i;
				while(i1 < len && ignored_chars.indexOf(text[i1])>=0) i1++;

				if(i1 < len)
					lookahead_chr=text[i1];
				else
					lookahead_chr='\0';

				match_idx=-1;
				match_len=0;
				group=identifyWordGroup(word,lookahead_chr, idx, match_idx, match_len);

				if(!group.isEmpty())
				{
					start_col=idx + match_idx;
					setFormat(start_col, match_len, formats[group]);
				}

				aux_len=(match_idx + match_len);
				if(match_idx >=0 &&  aux_len != word.length())
					i-=word.length() - aux_len;

				word="";
			}
		}
		while(i < len);

		curr_blk_info_count=getMultiLineInfoCount(current_block);
	}
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
	if(filename!="")
	{
		map<QString, QString> attribs;
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
			XMLParser::restartParser();
			XMLParser::setDTDFile(GlobalAttributes::CONFIGURATIONS_DIR +
														GlobalAttributes::DIR_SEPARATOR +
														GlobalAttributes::OBJECT_DTD_DIR +
														GlobalAttributes::DIR_SEPARATOR +
														GlobalAttributes::CODE_HIGHLIGHT_CONF +
														GlobalAttributes::OBJECT_DTD_EXT,
														GlobalAttributes::CODE_HIGHLIGHT_CONF);

			XMLParser::loadXMLFile(filename);

			if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
			{
				do
				{
					if(XMLParser::getElementType()==XML_ELEMENT_NODE)
					{
						elem=XMLParser::getElementName();

						if(elem==ParsersAttributes::WORD_SEPARATORS)
						{
							XMLParser::getElementAttributes(attribs);
							word_separators=attribs[ParsersAttributes::VALUE];
						}
						else if(elem==ParsersAttributes::WORD_DELIMITERS)
						{
							XMLParser::getElementAttributes(attribs);
							word_delimiters=attribs[ParsersAttributes::VALUE];
						}
						else if(elem==ParsersAttributes::IGNORED_CHARS)
						{
							XMLParser::getElementAttributes(attribs);
							ignored_chars=attribs[ParsersAttributes::VALUE];
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
							XMLParser::savePosition();
							XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
							elem=XMLParser::getElementName();
						}

						if(elem==ParsersAttributes::GROUP)
						{
							XMLParser::getElementAttributes(attribs);
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
								else if(attribs.size() > 1 || XMLParser::hasElement(XMLParser::CHILD_ELEMENT))
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
								else if(!XMLParser::hasElement(XMLParser::CHILD_ELEMENT))
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
								bg_color.setNamedColor(attribs[ParsersAttributes::BACKGROUND_COLOR]);

								if(!attribs[ParsersAttributes::LOOKAHEAD_CHAR].isEmpty())
									lookahead_char[group]=attribs[ParsersAttributes::LOOKAHEAD_CHAR][0];

								format.setFontItalic(italic);
								format.setFontUnderline(underline);

								if(bold)
									format.setFontWeight(QFont::Bold);
								else
									format.setFontWeight(QFont::Normal);

								format.setForeground(fg_color);
								format.setBackground(bg_color);
								formats[group]=format;


								XMLParser::savePosition();
								XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

								if(chr_sensitive)
									regexp.setCaseSensitivity(Qt::CaseSensitive);
								else
									regexp.setCaseSensitivity(Qt::CaseInsensitive);

								this->partial_match[group]=partial_match;

								do
								{
									if(XMLParser::getElementType()==XML_ELEMENT_NODE)
									{
										XMLParser::getElementAttributes(attribs);
										expr_type=attribs[ParsersAttributes::TYPE];
										regexp.setPattern(attribs[ParsersAttributes::VALUE]);

										if(attribs[ParsersAttributes::REGULAR_EXP]==ParsersAttributes::_TRUE_)
											regexp.setPatternSyntax(QRegExp::RegExp);
										else
											regexp.setPatternSyntax(QRegExp::FixedString);

										if(expr_type=="" ||
											 expr_type==ParsersAttributes::SIMPLE_EXP ||
											 expr_type==ParsersAttributes::INITIAL_EXP)
											initial_exprs[group].push_back(regexp);
										else
											final_exprs[group].push_back(regexp);
									}
								}
								while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
								XMLParser::restorePosition();
							}
						}
					}

					/* Check if there are some other groups to be declared, if not,
							continues to reading to the other part of configuration */
					if(groups_decl && !XMLParser::hasElement(XMLParser::NEXT_ELEMENT))
					{
						groups_decl=false;
						XMLParser::restorePosition();
					}

				}
				while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
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

