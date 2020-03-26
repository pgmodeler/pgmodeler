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

#include "schemaparser.h"
#include "attributes.h"

const char SchemaParser::CharComment='#';
const char SchemaParser::CharLineEnd='\n';
const char SchemaParser::CharTabulation='\t';
const char SchemaParser::CharSpace=' ';
const char SchemaParser::CharIniAttribute='{';
const char SchemaParser::CharEndAttribute='}';
const char SchemaParser::CharIniConditional='%';
const char SchemaParser::CharIniMetachar='$';
const char SchemaParser::CharIniPlainText='[';
const char SchemaParser::CharEndPlainText=']';
const char SchemaParser::CharIniCompExpr='(';
const char SchemaParser::CharEndCompExpr=')';
const char SchemaParser::CharValueDelim='"';
const char SchemaParser::CharValueOf='@';

const QString SchemaParser::TokenIf=QString("if");
const QString SchemaParser::TokenThen=QString("then");
const QString SchemaParser::TokenElse=QString("else");
const QString SchemaParser::TokenEnd=QString("end");
const QString SchemaParser::TokenOr=QString("or");
const QString SchemaParser::TokenAnd=QString("and");
const QString SchemaParser::TokenNot=QString("not");
const QString SchemaParser::TokenSet=QString("set");
const QString SchemaParser::TokenUnset=QString("unset");

const QString SchemaParser::TokenMetaSp=QString("sp");
const QString SchemaParser::TokenMetaBr=QString("br");
const QString SchemaParser::TokenMetaTb=QString("tb");
const QString SchemaParser::TokenMetaOb=QString("ob");
const QString SchemaParser::TokenMetaCb=QString("cb");
const QString SchemaParser::TokenMetaOc=QString("oc");
const QString SchemaParser::TokenMetaCc=QString("cc");

const QString SchemaParser::TokenEqOper=QString("==");
const QString SchemaParser::TokenNeOper=QString("!=");
const QString SchemaParser::TokenGtOper=QString(">");
const QString SchemaParser::TokenLtOper=QString("<");
const QString SchemaParser::TokenGtEqOper=QString(">=");
const QString SchemaParser::TokenLtEqOper=QString("<=");

const QRegExp SchemaParser::AttribRegExp=QRegExp("^([a-z])([a-z]*|(\\d)*|(\\-)*|(_)*)+", Qt::CaseInsensitive);

SchemaParser::SchemaParser()
{
	line=column=comment_count=0;
	ignore_unk_atribs=ignore_empty_atribs=false;
	pgsql_version=PgSqlVersions::DefaulVersion;
}

void SchemaParser::setPgSQLVersion(const QString &pgsql_ver)
{
	unsigned curr_ver = QString(pgsql_ver).remove('.').toUInt(),
			version90 = QString(PgSqlVersions::PgSqlVersion90).remove('.').toUInt(),
			default_ver = QString(PgSqlVersions::DefaulVersion).remove('.').toUInt();

	if(curr_ver != 0 && (curr_ver < version90))
		throw Exception(Exception::getErrorMessage(ErrorCode::InvPostgreSQLVersion)
						.arg(pgsql_ver)
						.arg(PgSqlVersions::PgSqlVersion90)
						.arg(PgSqlVersions::DefaulVersion),
						ErrorCode::InvPostgreSQLVersion,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(curr_ver > 0 && curr_ver <= default_ver)
		pgsql_version=pgsql_ver;
	else
		pgsql_version=PgSqlVersions::DefaulVersion;
}

QString SchemaParser::getPgSQLVersion()
{
	return pgsql_version;
}

QStringList SchemaParser::extractAttributes()
{
	QStringList attribs;
	int start=0, end=0;

	for(QString line : buffer)
	{
		//Find the first occurrence of '{' in the line
		start=line.indexOf(CharIniAttribute, start);

		while(start >= 0 && start < line.size())
		{
			end=line.indexOf(CharEndAttribute, start);
			if(end >= 0)
			{
				//Extract the name between {} and push it into the list
				attribs.push_back(line.mid(start + 1, end - start -1));
				//Start searching new attribute start now from the last position
				start=line.indexOf(CharIniAttribute, end);
			}
			else
				break;
		}

		start=end=0;
	}

	attribs.removeDuplicates();
	return attribs;
}

void SchemaParser::restartParser()
{
	/* Clears the buffer and resets the counters for line,
		column and amount of comments */
	buffer.clear();
	attributes.clear();
	line=column=comment_count=0;
}

void SchemaParser::loadBuffer(const QString &buf)
{
	QString buf_aux=buf, lin,
			escaped_comm_chr=QString("\\%1").arg(CharComment),
			placeholder = QString(QChar::ReplacementCharacter);
	QTextStream ts(&buf_aux);
	int pos=0;
	bool comm_holder_used = false;

	//Prepares the parser to do new reading
	restartParser();

	filename="[memory buffer]";

	//While the input file doesn't reach the end
	while(!ts.atEnd())
	{
		//Get one line from stream (until the last char before \n)
		lin = ts.readLine();

		/* Special treatment for escaped comment characters (e.g.: \#):
		 * In order to avoid removing wrongly the # from the the line where it appear in the form \#
		 * we need to replace it temporarily by a placeholder <?> and remove other portions of the line
		 * the is considered a real comment and then replace back that placeholder by the comment char again.
		 * This is useful if the user intend to represent the hash (#) char in the schema code and not use it as comment. */
		if(lin.indexOf(escaped_comm_chr) >= 0)
		{
			lin.replace(escaped_comm_chr, placeholder);
			comm_holder_used = true;
		}

		/* Since the method getline discards the \n when the line was just a line break
		its needed to treat it in order to not lost it */
		if(lin.isEmpty()) lin+=CharLineEnd;

		//If the entire line is commented out increases the comment lines counter
		if(lin[0]==CharComment) comment_count++;

		//Looking for the position of other comment characters for deletion
		pos=lin.indexOf(CharComment);

		//Removes the characters from the found position
		if(pos >= 0)
			lin.remove(pos, lin.size());

		//Replacing the comment placeholder by the comment char causing that character to be printed to the code
		if(comm_holder_used)
		{
			lin.replace(placeholder, QString(CharComment));
			comm_holder_used = false;
		}

		if(!lin.isEmpty())
		{
			//Add a line break in case the last character is not
			if(lin[lin.size()-1]!=CharLineEnd)
				lin+=CharLineEnd;

			//Add the treated line in the buffer
			buffer.push_back(lin);
		}
	}
}

void SchemaParser::loadFile(const QString &filename)
{
	if(!filename.isEmpty())
	{
		QFile input;
		QString buf;

		//Open the file for reading
		input.setFileName(filename);
		input.open(QFile::ReadOnly);

		if(!input.isOpen())
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(filename),
							ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		buf=input.readAll();
		input.close();

		//Loads the parser buffer
		loadBuffer(buf);
		SchemaParser::filename=filename;
	}
}

QString SchemaParser::getAttribute()
{
	QString atrib, current_line;
	bool start_attrib, end_attrib, error=false;

	//Get the current line from the buffer
	current_line=buffer[line];

	/* Only start extracting an attribute if it starts with a {
		even if the current character is an attribute delimiter */
	if(current_line[column]!=CharIniAttribute)
		error=true;
	else
	{
		//Step to the next column in the line
		column++;

		//Marks the flag indicating start of attribute
		start_attrib=true;
		//Unmarks the flag indicating end of attribute
		end_attrib=false;

		/* Attempt to extract an attribute until a space, end of line
	  or attribute is encountered */
		while(current_line[column]!=CharLineEnd &&
			  current_line[column]!=CharSpace &&
			  current_line[column]!=CharTabulation &&
			  !end_attrib && !error)
		{
			if(current_line[column]!=CharEndAttribute)
				atrib+=current_line[column];
			else if(current_line[column]==CharEndAttribute && !atrib.isEmpty())
				end_attrib=true;
			else
				error=true;
			column++;
		}

		/* If the attribute has been started but not finished
	  ie absence of the } in its statement (ie. {attr),
	  generates an error. */
		if(start_attrib && !end_attrib) error=true;
	}

	if(error)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
						.arg(filename).arg((line + comment_count + 1)).arg((column+1)),
						ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(!AttribRegExp.exactMatch(atrib))
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidAttribute)
						.arg(atrib).arg(filename).arg((line + comment_count + 1)).arg((column+1)),
						ErrorCode::InvalidAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return atrib;
}

QString SchemaParser::getWord()
{
	QString word, current_line;

	//Gets the current line buffer
	current_line=buffer[line];

	/* Attempt to extract a word if the first character is not
		a special character. */
	if(!isSpecialCharacter(current_line[column].toLatin1()))
	{
		/* Extract the word while it is not end of line, space or
		 special character */
		while(current_line[column]!=CharLineEnd &&
			  !isSpecialCharacter(current_line[column].toLatin1()) &&
			  current_line[column]!=CharSpace &&
			  current_line[column]!=CharTabulation)
		{
			word+=current_line[column];
			column++;
		}
	}

	return word;
}

QString SchemaParser::getPureText()
{
	QString text, current_line;
	bool error=false;

	current_line=buffer[line];

	//Attempt to extract a pure text if the first character is a [
	if(current_line[column]==CharIniPlainText)
	{
		//Moves to the next character that contains the beginning of the text
		column++;

		/* Extracts the text while the end of pure text (]), end of buffer or
		 beginning of other pure text ([) is reached */
		while(current_line[column]!=CharEndPlainText &&
			  line < buffer.size() &&
			  current_line[column]!=CharIniPlainText)
		{
			text+=current_line[column];

			/* Special case to end of line. Unlike other elements of
			language, a pure text can be extracted until the end of the buffer,
			thus, this method also controls the lines transitions */
			if(current_line[column]==CharLineEnd)
			{
				//Step to the next line
				line++;
				column=0;

				if(line < buffer.size())
					current_line=buffer[line];
			}
			else column++;
		}

		if(current_line[column]==CharEndPlainText)
			column++;
		else
			error=true;
	}
	else error=true;

	if(error)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
						.arg(filename).arg((line + comment_count + 1)).arg((column+1)),
						ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return text;
}

QString SchemaParser::getConditional()
{
	QString conditional, current_line;
	bool error=false;

	current_line=buffer[line];

	//Will initiate extraction if a % is found
	if(current_line[column]==CharIniConditional)
	{
		/* Passa para o próximo caractere que é o início do
		 do nome da palavra condicional */
		column++;

		/* Moves to the next character that is the beginning of
		 the name of the conditional word */
		while(current_line[column]!=CharLineEnd &&
			  current_line[column]!=CharSpace &&
			  current_line[column]!=CharTabulation)
		{
			conditional+=current_line[column];
			column++;
		}

		//If no word was extracted an error is raised
		if(conditional.isEmpty()) error=true;
	}
	else error=true;

	if(error)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
						.arg(filename).arg(line + comment_count + 1).arg(column+1),
						ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return conditional;
}

QString SchemaParser::getMetaCharacter()
{
	QString meta, current_line;
	bool error=false;

	current_line=buffer[line];

	//Begins the extraction in case of a $ is found
	if(current_line[column]==CharIniMetachar)
	{
		//Moves to the next character that is the beginning of the metacharacter
		column++;

		//Extracts the metacharacter until doesn't finds a space or end of line
		while(current_line[column]!=CharLineEnd &&
			  current_line[column]!=CharSpace &&
			  current_line[column]!=CharTabulation)
		{
			meta+=current_line[column];
			column++;
		}

		//If no metacharacter was extracted an error is raised
		if(meta.isEmpty()) error=true;
	}
	else error=true;

	if(error)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
						.arg(filename).arg(line + comment_count + 1).arg(column+1),
						ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return meta;
}

bool SchemaParser::isSpecialCharacter(char chr)
{
	return chr==CharIniAttribute || chr==CharEndAttribute ||
			chr==CharIniConditional || chr==CharIniMetachar ||
			chr==CharIniPlainText || chr==CharEndPlainText;
}

bool SchemaParser::evaluateComparisonExpr()
{
	QString curr_line, attrib, value, oper, valid_op_chrs="=!<>fi";
	bool error=false, end_eval=false, expr_is_true=true;
	static QStringList opers = { TokenEqOper, TokenNeOper, TokenGtOper,
															 TokenLtOper, TokenGtEqOper, TokenLtEqOper };

	try
	{
		curr_line=buffer[line];
		column++;

		while(!end_eval && !error)
		{
			ignoreBlankChars(curr_line);

			/* If the scan reached the end of the line and the expression was not closed raises an syntax error
		 Comparison expr must start and end in the same line */
			if(curr_line[column]==CharLineEnd && !end_eval)
				error=true;

			switch(curr_line[column].toLatin1())
			{
				case CharIniAttribute:
					/* Extract the attribute (the first element in the expression) only
			 if the comparison operator and values aren't extracted */
					if(attrib.isEmpty() && oper.isEmpty() && value.isEmpty())
						attrib=getAttribute();
					else
						error=true;
				break;

				case CharValueDelim:
					/* Extract the value (the last element in the expression) only
			 if the attribute and operator were extracted */
					if(value.isEmpty() && !attrib.isEmpty() && !oper.isEmpty())
					{
						value+=curr_line[column++];

						while(column < curr_line.size())
						{
							value+=curr_line[column++];

							if(curr_line[column]==CharValueDelim)
							{
								value+=CharValueDelim;
								column++;
								break;
							}
						}
					}
					else
						error=true;

				break;

				case CharEndCompExpr:
					column++;

					//If one of the elements are missing, raise an syntax error
					if(attrib.isEmpty() || oper.isEmpty() || value.isEmpty())
						error=true;
					else if(!opers.contains(QString(oper).remove('f').remove('i')))
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::InvalidOperatorInExpression)
										.arg(oper).arg(filename).arg((line + comment_count + 1)).arg((column+1)),
										ErrorCode::InvalidOperatorInExpression,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(attributes.count(attrib)==0 && !ignore_unk_atribs)
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::UnkownAttribute)
										.arg(attrib).arg(filename).arg((line + comment_count +1)).arg((column+1)),
										ErrorCode::UnkownAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else
					{
						QVariant left_val, right_val;
						value.remove(CharValueDelim);

						//Evaluating the attribute value against the one captured on the expression without casting
						if(oper.endsWith('f'))
						{
							left_val = QVariant(attributes[attrib].toFloat());
							right_val = QVariant(value.toFloat());
							oper.remove('f');
						}
						else if(oper.endsWith('i'))
						{
							left_val = QVariant(attributes[attrib].toInt());
							right_val = QVariant(value.toInt());
							oper.remove('i');
						}
						else
						{
							left_val = QVariant(attributes[attrib]);
							right_val = QVariant(value);
						}

						expr_is_true=((oper==TokenEqOper && (left_val == right_val)) ||
													(oper==TokenNeOper && (left_val != right_val)) ||
													(oper==TokenGtOper && (left_val > right_val)) ||
													(oper==TokenLtOper && (left_val < right_val)) ||
													(oper==TokenGtEqOper && (left_val >= right_val)) ||
													(oper==TokenLtEqOper && (left_val <= right_val)));

						end_eval=true;
					}
				break;

				default:
					/* Extract the operator (the second element in the expression) only
			 if the attribute was extracted and the value not */
					if(oper.size() <= 3 && !attrib.isEmpty() && value.isEmpty())
					{
						//If the current char is a valid operator capture it otherwise raise an error
						if(valid_op_chrs.indexOf(curr_line[column]) >= 0)
							oper+=curr_line[column++];
						else
							error=true;
					}
					else
						error=true;

				break;
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}

	if(error)
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
						.arg(filename).arg((line + comment_count + 1)).arg((column+1)),
						ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return expr_is_true;
}

void SchemaParser::defineAttribute()
{
	QString curr_line, attrib, value, new_attrib;
	bool error=false, end_def=false, use_val_as_name=false;

	try
	{
		curr_line=buffer[line];

		while(!end_def && !error)
		{
			ignoreBlankChars(curr_line);

			switch(curr_line[column].toLatin1())
			{
				case CharLineEnd:
					end_def=true;
				break;

				case CharValueOf:
					if(!use_val_as_name)
					{
						use_val_as_name=true;
						column++;
						new_attrib=getAttribute();
					}
					else
						error=true;
				break;

				case CharIniConditional:
					error=true;
				break;

				case CharIniAttribute:
					if(new_attrib.isEmpty())
						new_attrib=getAttribute();
					else
					{
						//Get the attribute in the middle of the value
						attrib=getAttribute();

						if(attributes.count(attrib)==0 && !ignore_unk_atribs)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::UnkownAttribute)
											.arg(attrib).arg(filename).arg((line + comment_count +1)).arg((column+1)),
											ErrorCode::UnkownAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						value+=attributes[attrib];
					}
				break;

				case CharIniPlainText:
					value+=getPureText();
				break;

				case CharIniMetachar:
					value+=translateMetaCharacter(getMetaCharacter());
				break;

				default:
					value+=getWord();
				break;
			}

			//If the attribute name was not extracted yet returns a error
			if(new_attrib.isEmpty())
				error=true;
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}

	if(!error)
	{
		attrib=(use_val_as_name ? attributes[new_attrib] : new_attrib);

		//Checking if the attribute has a valid name
		if(!AttribRegExp.exactMatch(attrib))
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::InvalidAttribute)
							.arg(attrib).arg(filename).arg((line + comment_count + 1)).arg((column+1)),
							ErrorCode::InvalidAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		/* Creates the attribute in the attribute map of the schema, making the attribute
	   available on the rest of the script being parsed */

		attributes[attrib]=value;
	}
	else
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
						.arg(filename).arg((line + comment_count + 1)).arg((column+1)),
						ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void SchemaParser::unsetAttribute()
{
	QString curr_line, attrib;
	bool end_def=false;

	try
	{
		curr_line=buffer[line];

		while(!end_def)
		{
			ignoreBlankChars(curr_line);

			switch(curr_line[column].toLatin1())
			{
				case CharLineEnd:
					end_def=true;
				break;

				case CharIniAttribute:
					attrib=getAttribute();

					if(attributes.count(attrib)==0 && !ignore_unk_atribs)
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::UnkownAttribute)
										.arg(attrib).arg(filename).arg((line + comment_count +1)).arg((column+1)),
										ErrorCode::UnkownAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(!AttribRegExp.exactMatch(attrib))
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::InvalidAttribute)
										.arg(attrib).arg(filename).arg((line + comment_count + 1)).arg((column+1)),
										ErrorCode::InvalidAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					attributes[attrib]=QString();
				break;

				default:
					throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
									.arg(filename).arg((line + comment_count + 1)).arg((column+1)),
									ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
bool SchemaParser::evaluateExpression()
{
	QString current_line, cond, attrib, prev_cond;
	bool error=false, end_eval=false, expr_is_true=true, attrib_true=true, comp_true=true;
	unsigned attrib_count=0, and_or_count=0;

	try
	{
		current_line=buffer[line];

		while(!end_eval && !error)
		{
			ignoreBlankChars(current_line);

			if(current_line[column]==CharLineEnd)
			{
				line++;
				if(line < buffer.size())
				{
					current_line=buffer[line];
					column=0;
					ignoreBlankChars(current_line);
				}
				else if(!end_eval)
					error=true;
			}

			switch(current_line[column].toLatin1())
			{
				//Extract the next conditional token
				case CharIniConditional:
					prev_cond=cond;
					cond=getConditional();

					//Error 1: %if {a} %or %or %then
					error=(cond==prev_cond ||
						   //Error 2: %if {a} %and %or %then
						   (cond==TokenAnd && prev_cond==TokenOr) ||
						   //Error 3: %if {a} %or %and %then
						   (cond==TokenOr && prev_cond==TokenAnd) ||
						   //Error 4: %if %and {a} %then
						   (attrib_count==0 && (cond==TokenAnd || cond==TokenOr)));

					if(cond==TokenThen)
					{
						/* Returns the parser to the token %then because additional
						operations is done whe this token is found */
						column-=cond.length()+1;
						end_eval=true;

						//Error 1: %if {a} %not %then
						error=(prev_cond==TokenNot ||
							   //Error 2: %if %then
							   attrib_count==0 ||
							   //Error 3: %if {a} %and %then
							   (and_or_count!=attrib_count-1));
					}
					else if(cond==TokenOr || cond==TokenAnd)
						and_or_count++;
				break;

				case CharIniAttribute:
					attrib=getAttribute();

					//Raises an error if the attribute does is unknown
					if(attributes.count(attrib)==0 && !ignore_unk_atribs)
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::UnkownAttribute)
										.arg(attrib).arg(filename).arg((line + comment_count +1)).arg((column+1)),
										ErrorCode::UnkownAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					//Error 1: A conditional token other than %or %not %and if found on conditional expression
					error=(!cond.isEmpty() && cond!=TokenOr && cond!=TokenAnd && cond!=TokenNot) ||
						  //Error 2: A %not token if found after an attribute: %if {a} %not %then
						  (attrib_count > 0 && cond==TokenNot && prev_cond.isEmpty()) ||
						  //Error 3: Two attributes not separated by any conditional token: %if {a} {b} %then
						  (attrib_count > 0 && cond.isEmpty());

					//Increments the extracted attribute counter
					attrib_count++;

					if(!error)
					{
						//Appliyng the NOT operator if found
						attrib_true=(cond==TokenNot ? attributes[attrib].isEmpty() : !attributes[attrib].isEmpty());

						//Executing the AND operation if the token is found
						if(cond==TokenAnd || prev_cond==TokenAnd)
							expr_is_true=(expr_is_true && attrib_true);
						else if(cond==TokenOr || prev_cond==TokenOr)
							expr_is_true=(expr_is_true || attrib_true);
						else
							expr_is_true=attrib_true;

						cond.clear();
						prev_cond.clear();
					}
				break;

				case CharIniCompExpr:
					comp_true=evaluateComparisonExpr();

					//Appliyng the NOT operator if found
					if(cond==TokenNot) comp_true=!comp_true;

					//Executing the AND operation if the token is found
					if(cond==TokenAnd || prev_cond==TokenAnd)
						expr_is_true=(expr_is_true && comp_true);
					else if(cond==TokenOr || prev_cond==TokenOr)
						expr_is_true=(expr_is_true || comp_true);
					else
						expr_is_true=comp_true;

					//Consider the comparison expression as an attribute evaluation
					attrib_count++;
					cond.clear();
					prev_cond.clear();
				break;

				default:
					error=true;
				break;
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),	__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	if(error)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
						.arg(filename).arg((line + comment_count + 1)).arg((column+1)),
						ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return expr_is_true;
}

void SchemaParser::ignoreBlankChars(const QString &line)
{
	while(column < line.size() &&
		  (line[column]==CharSpace ||
		   line[column]==CharTabulation)) column++;
}

char SchemaParser::translateMetaCharacter(const QString &meta)
{
	static map<QString, char> metas={{ TokenMetaSp, CharSpace },
									 { TokenMetaTb, CharTabulation },
									 { TokenMetaBr, CharLineEnd },
									 { TokenMetaOb, CharIniPlainText },
									 { TokenMetaCb, CharEndPlainText },
									 { TokenMetaOc, CharIniAttribute },
									 { TokenMetaCc, CharEndAttribute }};

	if(metas.count(meta)==0)
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::InvalidMetacharacter)
						.arg(meta).arg(filename).arg(line + comment_count +1).arg(column+1),
						ErrorCode::InvalidMetacharacter,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return metas.at(meta);
}

QString SchemaParser::getCodeDefinition(const QString & obj_name, attribs_map &attribs, unsigned def_type)
{
	try
	{
		QString filename;

		if(def_type==SqlDefinition)
		{
			//Formats the filename
			filename = GlobalAttributes::getSchemaFilePath(GlobalAttributes::SQLSchemaDir, obj_name);
			attribs[Attributes::PgSqlVersion]=pgsql_version;

			//Try to get the object definitin from the specified path
			return getCodeDefinition(filename, attribs);
		}
		else
		{
			filename = GlobalAttributes::getSchemaFilePath(GlobalAttributes::XMLSchemaDir, obj_name);
			return XmlParser::convertCharsToXMLEntities(getCodeDefinition(filename, attribs));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),	__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void SchemaParser::ignoreUnkownAttributes(bool ignore)
{
	ignore_unk_atribs=ignore;
}

void SchemaParser::ignoreEmptyAttributes(bool ignore)
{
	ignore_empty_atribs=ignore;
}

QString SchemaParser::getCodeDefinition(attribs_map &attribs)
{
	QString object_def;
	unsigned end_cnt, if_cnt;
	int if_level, prev_if_level;
	QString atrib, cond, prev_cond, word, meta;
	bool error, if_expr;
	char chr;
	vector<bool> vet_expif, vet_tk_if, vet_tk_then, vet_tk_else;
	map<int, vector<QString> > if_map, else_map;
	vector<QString>::iterator itr, itr_end;
	vector<int> vet_prev_level;
	vector<QString> *vet_aux;

	//In case the file was successfuly loaded
	if(buffer.size() > 0)
	{
		//Init the control variables
		attributes=attribs;
		error=if_expr=false;
		if_level=-1;
		end_cnt=if_cnt=0;

		while(line < buffer.size())
		{
			chr=buffer[line][column].toLatin1();
			switch(chr)
			{
				/* Increments the number of rows causing the parser
				to get the next line buffer for analysis */
				case CharLineEnd:
					line++;
					column=0;
				break;

				case CharTabulation:
				case CharSpace:
					//The parser will ignore the spaces that are not within pure texts
					while(buffer[line][column]==CharSpace ||
								buffer[line][column]==CharTabulation) column++;
				break;

					//Metacharacter extraction
				case CharIniMetachar:
					meta=getMetaCharacter();

					//Checks whether the metacharacter is part of the  'if' expression (this is an error)
					if(if_level>=0 && vet_tk_if[if_level] && !vet_tk_then[if_level])
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
										.arg(filename).arg(line + comment_count +1).arg(column+1),
										ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else
					{
						//Converting the metacharacter drawn to the character that represents this
						chr=translateMetaCharacter(meta);
						meta=QString();
						meta+=chr;

						//If the parser is inside an 'if / else' extracting tokens
						if(if_level>=0)
						{
							/* If the parser is in 'if' section,
								 places the metacharacter on the word map of the current 'if' */
							if(vet_tk_if[if_level] &&
									vet_tk_then[if_level] &&
									!vet_tk_else[if_level])
								if_map[if_level].push_back(meta);

							/* If the parser is in 'else' section,
								 places the metacharacter on the word map of the current 'else'*/
							else if(vet_tk_else[if_level])
								else_map[if_level].push_back(meta);
						}
						else
							/* If the parsers is not in a 'if / else', puts the metacharacter
								 in the definition sql */
							object_def+=meta;
					}

				break;

					//Attribute extraction
				case CharIniAttribute:
				case CharEndAttribute:
					atrib=getAttribute();

					//Checks if the attribute extracted belongs to the passed list of attributes
					if(attributes.count(atrib)==0)
					{
						if(!ignore_unk_atribs)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::UnkownAttribute)
											.arg(atrib).arg(filename).arg((line + comment_count +1)).arg((column+1)),
											ErrorCode::UnkownAttribute,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
							attributes[atrib]=QString();
					}

					//If the parser is inside an 'if / else' extracting tokens
					if(if_level>=0)
					{
						//If the parser evaluated the 'if' conditional and is inside the current if block
						if(!(!if_expr && vet_tk_if[if_level] && !vet_tk_then[if_level]))
						{
							word=atrib;
							atrib=QString();
							atrib+=CharIniAttribute;
							atrib+=word;
							atrib+=CharEndAttribute;

							//If the parser is in the 'if' section
							if(vet_tk_if[if_level] &&
									vet_tk_then[if_level] &&
									!vet_tk_else[if_level])
								//Inserts the attribute value in the map of the words of current the 'if' section
								if_map[if_level].push_back(atrib);
							else if(vet_tk_else[if_level])
								//Inserts the attribute value in the map of the words of current the 'else' section
								else_map[if_level].push_back(atrib);
						}
					}
					else
					{
						/* If the attribute has no value set and parser must not ignore empty values
						raises an exception */
						if(attributes[atrib].isEmpty() && !ignore_empty_atribs)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::UndefinedAttributeValue)
											.arg(atrib).arg(filename).arg(line + comment_count +1).arg(column+1),
											ErrorCode::UndefinedAttributeValue,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						/* If the parser is not in an if / else, concatenates the value of the attribute
							directly in definition in sql */
						object_def+=attributes[atrib];
					}
				break;

					//Conditional instruction extraction
				case CharIniConditional:
					prev_cond=cond;
					cond=getConditional();

					//Checks whether the extracted token is a valid conditional
					if(cond!=TokenIf && cond!=TokenElse &&
							cond!=TokenThen && cond!=TokenEnd &&
							cond!=TokenOr && cond!=TokenNot &&
							cond!=TokenAnd && cond!=TokenSet &&
							cond!=TokenUnset)
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::InvalidInstruction)
										.arg(cond).arg(filename).arg(line + comment_count +1).arg(column+1),
										ErrorCode::InvalidInstruction,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(cond==TokenSet || cond==TokenUnset)
					{
						bool extract=false;

						/* Extracts or unset the attribute only if the process is not in the middle of a 'if-then-else' or
			   if the parser is inside the 'if' part and the expression is evaluated as true, or in the 'else' part
			   and the related 'if' is false. Otherwise the line where %set is located will be completely ignored */
						extract=(if_level < 0 || vet_expif.empty());

						if(!extract && if_level >= 0)
						{
							//If in 'else' the related 'if' is false, extracts the attribute
							if(prev_cond == TokenElse && !vet_expif[if_level])
								extract=true;
							else if(prev_cond != TokenElse)
							{
								//If in the 'if' part all the previous ifs until the current must be true
								extract=true;
								for(int i=0; i <= if_level && extract; i++)
								{
									extract=(vet_expif[i] && !vet_tk_else[i]) ||
													(!vet_expif[i] && vet_tk_else[i]);
								}
							}
						}

						if(extract)
						{
							if(cond==TokenSet)
								defineAttribute();
							else
								unsetAttribute();
						}
						else
						{
							column=0;
							line++;
						}
					}
					else
					{
						//If the toke is an 'if'
						if(cond==TokenIf)
						{
							//Evaluates the if expression storing the result on the vector
							if_expr=true;
							vet_expif.push_back(evaluateExpression());

							/* Inserts the value of the current 'if' level  in the previous 'if' levels vector.
							 This vector is used to know which 'if' the parser was in before entering current if */
							vet_prev_level.push_back(if_level);

							/* Mark the flags indicating that an  'if' was found and a
							 'then' and 'else' have not been found yet */
							vet_tk_if.push_back(true);
							vet_tk_then.push_back(false);
							vet_tk_else.push_back(false);

							//Defines the current if level as the size of list 'if' tokens found  -1
							if_level=(vet_tk_if.size()-1);

							//Increases the number of found 'if's
							if_cnt++;
						}
						//If the parser is in 'if / else' and one 'then' token is found
						else if(cond==TokenThen && if_level>=0)
						{
							//Marks the then token flag of the current 'if'
							vet_tk_then[if_level]=true;

							/* Clears the  expression extracted flag from the 'if - then',
								 so that the parser does not generate an error when it encounters another
								 'if - then' with an expression still unextracted */
							if_expr=false;
						}
						//If the parser is in 'if / else' and a 'else' token is found
						else if(cond==TokenElse && if_level>=0)
							//Mark the  o flag do token else do if atual
							vet_tk_else[if_level]=true;
						//Case the parser is in 'if/else' and a 'end' token was found
						else if(cond==TokenEnd && if_level>=0)
						{
							//Increments the number of 'end' tokes found
							end_cnt++;

							//Get the level of the previous 'if' where the parser was
							prev_if_level=vet_prev_level[if_level];

							//In case the current 'if' be internal (nested) (if_level > 0)
							if(if_level > 0)
							{
								//In case the current 'if' doesn't in the 'else' section of the above 'if' (previous if level)
								if(!vet_tk_else[prev_if_level])
									//Get the extracted word vector on the above 'if'
									vet_aux=&if_map[prev_if_level];
								else
									//Get the extracted word vector on the above 'else'
									vet_aux=&else_map[prev_if_level];
							}
							else
								vet_aux=nullptr;

							/* Initializes the iterators to scan
								 the auxiliary vector if necessary */
							itr=itr_end=if_map[0].end();

							/* In case the expression of the current 'if' has the value true
								 then the parser will scan the list of words on the 'if' part of the
								 current 'if' */
							if(vet_expif[if_level])
							{
								itr=if_map[if_level].begin();
								itr_end=if_map[if_level].end();
							}

							/* If there is a 'else' part on the current 'if'
								 then the parser will scan the list of words on the 'else' part */
							else if(else_map.count(if_level)>0)
							{
								itr=else_map[if_level].begin();
								itr_end=else_map[if_level].end();
							}

							/* This iteration scans the list of words selected above
								 inserting them in 'if' part  of the 'if' or 'else' superior to current.
								 This is done so that only the words extracted based on
								 ifs expressions of the buffer are embedded in defining sql */
							while(itr!=itr_end)
							{
								//If the auxiliary vector is allocated, inserts the word on above 'if / else'
								if(vet_aux)
									vet_aux->push_back((*itr));
								else
								{
									word=(*itr);

									//Check if the word is not an attribute
									if(!word.isEmpty() && word.startsWith(CharIniAttribute) && word.endsWith(CharEndAttribute))
									{
										//If its an attribute, extracts the name between { } and checks if the same has empty value
										atrib=word.mid(1, word.size()-2);
										word=attributes[atrib];

										/* If the attribute has no value set and parser must not ignore empty values
										raises an exception */
										if(word.isEmpty() && !ignore_empty_atribs)
										{
											throw Exception(Exception::getErrorMessage(ErrorCode::UndefinedAttributeValue)
															.arg(atrib).arg(filename).arg(line + comment_count +1).arg(column+1),
															ErrorCode::UndefinedAttributeValue,__PRETTY_FUNCTION__,__FILE__,__LINE__);
										}
									}

									//Else, insert the work directly on the object definition
									object_def+=word;
								}
								itr++;
							}

							//Case the current if is nested (internal)
							if(if_level > 0)
								//Causes the parser to return to the earlier 'if'
								if_level=prev_if_level;

							/* In case the 'if' be the uppermost (level 0) indicates that all
								 the if's  has already been checked, so the parser will clear the
								 used auxiliary structures*/
							else
							{
								if_map.clear();
								else_map.clear();
								vet_tk_if.clear();
								vet_tk_then.clear();
								vet_tk_else.clear();
								vet_expif.clear();
								vet_prev_level.clear();

								//Resets the ifs levels
								if_level=prev_if_level=-1;
							}
						}
						else
							error=true;

						if(!error)
						{
							/* Verifying that the conditional words appear in a valid  order if not
							 the parser generates an error. Correct order means IF before THEN,
							 ELSE after IF and before END */
							if((prev_cond==TokenIf && cond!=TokenThen) ||
									(prev_cond==TokenElse && cond!=TokenIf && cond!=TokenEnd) ||
									(prev_cond==TokenThen && cond==TokenThen))
								error=true;
						}

						if(error)
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
											.arg(filename).arg(line + comment_count +1).arg(column+1),
											ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}
				break;

					//Extraction of pure text or simple words
				default:
					if(chr==CharIniPlainText ||
							chr==CharEndPlainText)
						word=getPureText();
					else
						word=getWord();

					//Case the parser is in 'if/else'
					if(if_level>=0)
					{
						/* In case the word/text be inside 'if' expression, the parser returns an error
						 because only an attribute must be on the 'if' expression  */
						if(vet_tk_if[if_level] && !vet_tk_then[if_level])
						{
							throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
											.arg(filename).arg(line + comment_count +1).arg(column+1),
											ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						//Case the parser is in 'if' section
						else if(vet_tk_if[if_level] &&
								vet_tk_then[if_level] &&
								!vet_tk_else[if_level])
							//Inserts the word on the words map extracted on 'if' section
							if_map[if_level].push_back(word);
						else if(vet_tk_else[if_level])
							//Inserts the word on the words map extracted on 'else' section
							else_map[if_level].push_back(word);
					}
					else
						//Case the parser is not in 'if/else' concatenates the word/text directly on the object definition
						object_def+=word;
				break;
			}
		}

		/* If has more 'if' toknes than  'end' tokens, this indicates that some 'if' in code
		was not closed thus the parser returns an error */
		if(if_cnt!=end_cnt)
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::InvalidSyntax)
							.arg(filename).arg(line + comment_count +1).arg(column+1),
							ErrorCode::InvalidSyntax,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}

	restartParser();
	ignore_unk_atribs=false;
	ignore_empty_atribs=false;
	return object_def;
}

QString SchemaParser::getCodeDefinition(const QString &filename, attribs_map &attribs)
{
	try
	{
		loadFile(filename);
		attribs[Attributes::PgSqlVersion]=pgsql_version;
		return getCodeDefinition(attribs);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
