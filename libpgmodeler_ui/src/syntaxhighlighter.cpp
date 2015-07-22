/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

QFont SyntaxHighlighter::default_font=QFont(QString("DejaVu Sans Mono"), 10);

SyntaxHighlighter::SyntaxHighlighter(QPlainTextEdit *parent, bool auto_rehighlight, bool single_line_mode) : QSyntaxHighlighter(parent)
{
  if(!parent)
    throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  this->setDocument(parent->document());
  this->auto_rehighlight=auto_rehighlight;
	this->single_line_mode=single_line_mode;
	configureAttributes();
  parent->installEventFilter(this);
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

bool SyntaxHighlighter::isDualExpressionGroup(const QString &group)
{
  return(initial_exprs.count(group) && final_exprs.count(group));
}

SyntaxHighlighter::BlockInfo *SyntaxHighlighter::getBlockInfo(int block)
{
 /* BlockInfo *info=nullptr;

  for(auto &inf : block_infos)
  {
    if(inf->id==block)
    {
      info=inf;
      break;
    }
  }

  return(info); */
}

void SyntaxHighlighter::configureAttributes(void)
{
	conf_loaded=false;
  is_rehighlighting=false;
  connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(validateTextModification(int,int,int)));
}

void SyntaxHighlighter::validateTextModification(int, int chars_rem, int chars_add)
{
  if(chars_rem > 0 || chars_add > 0)
  {
    /*QTextBlock block=current_block;

    while(block.isValid())
    {
      block.setUserState(UNDEF_BLOCK);
      rehighlightBlock(block);
      block=block.next();
    } */
  }
}

bool SyntaxHighlighter::hasDualExpression(const QString &txt, const QString &group)
{
  if(initial_exprs.count(group)==0 || final_exprs.count(group)==0)
    return(false);
  else
  {
    bool found=false;
    vector<vector<QRegExp> *> exprs={ &initial_exprs[group], &final_exprs[group] };

    for(auto vect : exprs)
    {
      for(auto expr=vect->begin(); expr!=vect->end() && !found; expr++)
        found=txt.contains(*expr);
    }

    return(found);
  }
}

void SyntaxHighlighter::highlightBlock(const QString &txt)
{
  QTextStream ts(stdout);
  current_block=currentBlock();
  ts << "highlightBlock >> " << txt << endl;

  if(!txt.isEmpty())
  {
    QString text=txt + QChar('\n'), word, group;
    unsigned i=0, len, idx=0, i1;
    int match_idx, match_len, aux_len, start_col;
    QChar chr_delim, lookahead_chr;
    bool has_dual_expr=false;
    BlockInfo *info=dynamic_cast<BlockInfo *>(current_block.userData()),
        *prev_info=dynamic_cast<BlockInfo *>(current_block.previous().userData());

    len=text.length();

    if(info && info->has_dual_expr)
    {
      removeBlockInfo(info);
      setCurrentBlockUserData(nullptr);
      setCurrentBlockState(UNDEF_BLOCK);
      info=nullptr;
    }

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
        bool expr_closed=false;
        int block_st=UNDEF_BLOCK;

        i1=i;
        while(i1 < len && ignored_chars.contains(text[i1])) i1++;

        if(i1 < len)
          lookahead_chr=text[i1];
        else
          lookahead_chr='\0';

        match_idx=-1;
        match_len=0;
        group=identifyWordGroup(word, lookahead_chr, match_idx, match_len, expr_closed);

        QTextStream ts(stdout);
        ts << "block state:       " << currentBlockState() << endl;
        ts << "prev. block state: " <<  current_block.previous().userState() << endl;
        ts << "---" << endl;

        if(current_block.previous().userState()==OPEN_EXPR_BLOCK &&
           (!info || (info && !info->has_dual_expr)))
          group=prev_info->group;

        if(!group.isEmpty())
        {
          bool is_dual_expr=isDualExpressionGroup(group);

          if(!has_dual_expr && is_dual_expr && hasDualExpression(text, group))
            has_dual_expr=true;

          start_col=idx + match_idx;
          setFormat(start_col, match_len, group);

          if(!info)
          {
            info=createBlockInfo(group);
            setCurrentBlockUserData(info);
          }
          else
            info->group=group;

          info->has_dual_expr=has_dual_expr;

          if(is_dual_expr)
            block_st=(expr_closed ? CLOSED_EXPR_BLOCK : OPEN_EXPR_BLOCK);
          else
            block_st=SIMPLE_BLOCK;

          setCurrentBlockState(block_st);
        }

        aux_len=(match_idx + match_len);
        if(match_idx >=0 &&  aux_len != word.length())
          i-=word.length() - aux_len;

        word=QString();
      }
    }
    while(i < len);      
  }
}

QString SyntaxHighlighter::identifyWordGroup(const QString &word, const QChar &lookahead_chr, int &match_idx, int &match_len, bool &expr_closed)
{
	QRegExp expr;
	vector<QString>::iterator itr, itr_end;
	vector<QRegExp>::iterator itr_exp, itr_exp_end;
	vector<QRegExp> *vet_expr=nullptr;
	QString group;
	bool match=false, part_mach=false;
  BlockInfo *info=dynamic_cast<BlockInfo *>(current_block.userData());
  int block_st=currentBlockState();

  if(info && block_st==OPEN_EXPR_BLOCK)
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
      expr_closed=true;
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

      if(match && info && isDualExpressionGroup(group))
        expr_closed=false;
		}

    if(!match)
      return(QString());
    else
      return(group);
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

SyntaxHighlighter::BlockInfo *SyntaxHighlighter::createBlockInfo(const QString &group)
{
  BlockInfo *info=nullptr;

  info=new BlockInfo(group);
  block_infos.push_back(info);

  return(info);
}

void SyntaxHighlighter::removeBlockInfo(BlockInfo *info)
{
  block_infos.erase(std::find(block_infos.begin(), block_infos.end(), info));
}

void SyntaxHighlighter::setDefaultFont(const QFont &fnt)
{
  SyntaxHighlighter::default_font=fnt;
}
