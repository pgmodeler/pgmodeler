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

#include "schemaparser.h"

const char SchemaParser::CHR_COMMENT='#';
const char SchemaParser::CHR_LINE_END='\n';
const char SchemaParser::CHR_TABULATION='\t';
const char SchemaParser::CHR_SPACE=' ';
const char SchemaParser::CHR_INI_ATTRIB='@';
const char SchemaParser::CHR_MID_ATTRIB='{';
const char SchemaParser::CHR_END_ATTRIB='}';
const char SchemaParser::CHR_INI_CONDITIONAL='%';
const char SchemaParser::CHR_INI_METACHAR='$';
const char SchemaParser::CHR_INI_PURETEXT='[';
const char SchemaParser::CHR_END_PURETEXT=']';

const QString SchemaParser::TOKEN_IF="if";
const QString SchemaParser::TOKEN_THEN="then";
const QString SchemaParser::TOKEN_ELSE="else";
const QString SchemaParser::TOKEN_END="end";
const QString SchemaParser::TOKEN_OR="or";
const QString SchemaParser::TOKEN_AND="and";
const QString SchemaParser::TOKEN_NOT="not";

const QString SchemaParser::TOKEN_META_SP="sp";
const QString SchemaParser::TOKEN_META_BR="br";
const QString SchemaParser::TOKEN_META_TB="tb";
const QString SchemaParser::TOKEN_META_OB="ob";
const QString SchemaParser::TOKEN_META_CB="cb";

const QString SchemaParser::PGSQL_VERSION_90="9.0";
const QString SchemaParser::PGSQL_VERSION_91="9.1";
const QString SchemaParser::PGSQL_VERSION_92="9.2";

vector<QString> SchemaParser::buffer;
map<QString,QString> SchemaParser::attributes;
QString SchemaParser::filename="";
unsigned SchemaParser::line=0;
unsigned SchemaParser::column=0;
unsigned SchemaParser::comment_count=0;
bool SchemaParser::ignore_unk_atribs=false;

QString SchemaParser::pgsql_version=SchemaParser::PGSQL_VERSION_92;

void SchemaParser::setPgSQLVersion(const QString &pgsql_ver)
{
	if(!pgsql_ver.isEmpty() &&
		 (pgsql_ver < PGSQL_VERSION_90 || pgsql_ver > PGSQL_VERSION_92))
		throw Exception(Exception::getErrorMessage(ERR_INV_POSTGRESQL_VERSION)
										.arg(pgsql_ver)
										.arg(PGSQL_VERSION_90 + ", " + PGSQL_VERSION_91 + ", " + PGSQL_VERSION_92),
										ERR_INV_POSTGRESQL_VERSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!pgsql_ver.isEmpty())
		SchemaParser::pgsql_version=pgsql_ver;
	else
		SchemaParser::pgsql_version=PGSQL_VERSION_92;
}

QString SchemaParser::getPgSQLVersion(void)
{
	return(SchemaParser::pgsql_version);
}

void SchemaParser::getPgSQLVersions(vector<QString> &versions)
{
	versions.clear();
	versions.push_back(PGSQL_VERSION_92);
	versions.push_back(PGSQL_VERSION_91);
	versions.push_back(PGSQL_VERSION_90);
}

void SchemaParser::restartParser(void)
{
	/* Clears the buffer and resets the counters for line,
		column and amount of comments */
	buffer.clear();
	attributes.clear();
	line=column=comment_count=0;
}

void SchemaParser::loadBuffer(const QString &buf)
{
	QString buf_aux=buf, lin;
	QTextStream ts(&buf_aux);
	int pos=0;

	//Prepares the parser to do new reading
	restartParser();

	//While the input file doesn't reach the end
	while(!ts.atEnd())
	{
		//Get one line from stream (until the last char before \n)
		lin=ts.readLine();

		/* Since the method getline discards the \n when the line was just a line break
		its needed to treat it in order to not lost it */
		if(lin=="") lin+=CHR_LINE_END;

		//If the entire line is commented out increases the comment lines counter
		if(lin[0]==CHR_COMMENT) comment_count++;

		//Looking for the position of other comment characters for deletion
		pos=lin.indexOf(CHR_COMMENT);

		//Removes the characters from the found position
		if(pos >= 0)
			lin.remove(pos, lin.size());

		if(lin!="")
		{
			//Add a line break in case the last character is not
			if(lin[lin.size()-1]!=CHR_LINE_END)
				lin+=CHR_LINE_END;

			//Add the treated line in the buffer
			buffer.push_back(lin);
		}
	}
}

void SchemaParser::loadFile(const QString &file)
{
	if(file!="")
	{
		ifstream input;
		char lin[1025];
		QString buf;

		//Open the file for reading
		input.open(file.toStdString().c_str());

		if(!input.is_open())
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(file),
											ERR_FILE_DIR_NOT_ACCESSED,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//While the input file doesn't reach the end
		while(!input.eof())
		{
			input.getline(lin, 1024);
			buf+=QString(lin) + CHR_LINE_END;
		}

		//Loads the parser buffer
		loadBuffer(buf);

		SchemaParser::filename=file;

		//Close the file stream
		input.close();
	}
}

QString SchemaParser::getAttribute(void)
{
	QString atrib, current_line, str_aux;
	bool start_attrib, end_attrib, error=false;

	//Get the current line from the buffer
	current_line=buffer[line];

	/* Only start extracting an attribute if it starts with a @
		even if the current character is an attribute delimiter */
	if(current_line[column]==CHR_INI_ATTRIB)
	{
		//Step to the next column in the line
		column++;

		/* The next character must be one { because this starts the delimitation
		 of the attribute name. If not this indicates an error */
		if(current_line[column]!=CHR_MID_ATTRIB)
			error=true;
		else
		{
			//Marks the flag indicating start of attribute
			start_attrib=true;
			//Unmarks the flag indicating end of attribute
			end_attrib=false;
			//Step to the next column that contains the first letter of the attribute name
			column++;

			/* Attempt to extract an attribute until a space, end of line
			or attribute is encountered */
			while(current_line[column]!=CHR_LINE_END &&
						current_line[column]!=CHR_SPACE &&
						current_line[column]!=CHR_TABULATION &&
						!end_attrib && !error)
			{
				if(current_line[column]!=CHR_END_ATTRIB)
					atrib+=current_line[column];
				else if(current_line[column]==CHR_END_ATTRIB && atrib!="")
					end_attrib=true;
				else
					error=true;
				column++;
			}

			/* If the attribute has been started but not finished
			ie absence of the } in its statement (ie. @{attr),
			generates an error. */
			if(start_attrib && !end_attrib) error=true;
		}
	}
	else error=true;

	if(error)
	{
		//Raise a syntax error
		str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
						.arg(filename).arg((line + comment_count + 1)).arg((column+1));
		throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return(atrib);
}

QString SchemaParser::getWord(void)
{
	QString word, current_line;

	//Gets the current line buffer
	current_line=buffer[line];

	/* Attempt to extract a word if the first character is not
		a special character. */
	if(!isSpecialCharacter(current_line[column].toAscii()))
	{
		/* Extract the word while it is not end of line, space or
		 special character */
		while(current_line[column]!=CHR_LINE_END &&
					!isSpecialCharacter(current_line[column].toAscii()) &&
					current_line[column]!=CHR_SPACE &&
					current_line[column]!=CHR_TABULATION)
		{
			word+=current_line[column];
			column++;
		}
	}

	return(word);
}

QString SchemaParser::getPureText(void)
{
	QString text, current_line, str_aux;
	bool error=false;

	current_line=buffer[line];

	//Attempt to extract a pure text if the first character is a [
	if(current_line[column]==CHR_INI_PURETEXT)
	{
		//Moves to the next character that contains the beginning of the text
		column++;

		/* Extracts the text while the end of pure text (]), end of buffer or
		 beginning of other pure text ([) is reached */
		while(current_line[column]!=CHR_END_PURETEXT &&
					line < buffer.size() &&
					current_line[column]!=CHR_INI_PURETEXT)
		{
			text+=current_line[column];

			/* Special case to end of line. Unlike other elements of
			language, a pure text can be extracted until the end of the buffer,
			thus, this method also controls the lines transitions */
			if(current_line[column]==CHR_LINE_END)
			{
				//Step to the next line
				line++;
				column=0;
				current_line=buffer[line];
			}
			else column++;
		}

		if(current_line[column]==CHR_END_PURETEXT)
			column++;
		else
			error=true;
	}
	else error=true;

	if(error)
	{
		str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
						.arg(filename).arg((line + comment_count + 1)).arg((column+1));

		throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return(text);
}

QString SchemaParser::getConditional(void)
{
	QString conditional, current_line, str_aux;
	bool error=false;

	current_line=buffer[line];

	//Will initiate extraction if a % is found
	if(current_line[column]==CHR_INI_CONDITIONAL)
	{
		/* Passa para o próximo caractere que é o início do
		 do nome da palavra condicional */
		column++;

		/* Moves to the next character that is the beginning of
		 the name of the conditional word */
		while(current_line[column]!=CHR_LINE_END &&
					current_line[column]!=CHR_SPACE &&
					current_line[column]!=CHR_TABULATION)
		{
			conditional+=current_line[column];
			column++;
		}

		//If no word was extracted an error is raised
		if(conditional=="") error=true;
	}
	else error=true;

	if(error)
	{
		str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
						.arg(filename).arg(line + comment_count + 1).arg(column+1);
		throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return(conditional);
}

QString SchemaParser::getMetaCharacter(void)
{
	QString meta, current_line, str_aux;
	bool error=false;

	current_line=buffer[line];

	//Begins the extraction in case of a $ is found
	if(current_line[column]==CHR_INI_METACHAR)
	{
		//Moves to the next character that is the beginning of the metacharacter
		column++;

		//Extracts the metacharacter until doesn't finds a space or end of line
		while(current_line[column]!=CHR_LINE_END &&
					current_line[column]!=CHR_SPACE &&
					current_line[column]!=CHR_TABULATION)
		{
			meta+=current_line[column];
			column++;
		}

		//If no metacharacter was extracted an error is raised
		if(meta=="") error=true;
	}
	else error=true;

	if(error)
	{
		str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
						.arg(filename).arg(line + comment_count + 1).arg(column+1);
		throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return(meta);
}

bool SchemaParser::isSpecialCharacter(char chr)
{
	return(chr==CHR_INI_ATTRIB || chr==CHR_MID_ATTRIB ||
				 chr==CHR_END_ATTRIB || chr==CHR_INI_CONDITIONAL ||
				 chr==CHR_INI_METACHAR || chr==CHR_INI_PURETEXT ||
				 chr==CHR_END_PURETEXT);
}

bool SchemaParser::evaluateExpression(void)
{
	QString current_line, cond, attrib, prev_cond;
	bool error=false, end_eval=false, expr_is_true=true, attrib_true=true;
	unsigned attrib_count=0, and_or_count=0;

	try
	{
		current_line=buffer[line];

		while(!end_eval && !error)
		{
			//Eliminates any black space
			while(current_line[column]==CHR_SPACE ||
						current_line[column]==CHR_TABULATION) column++;

			switch(current_line[column].toAscii())
			{
				//Extract the next conditional token
				case CHR_INI_CONDITIONAL:
					prev_cond=cond;
					cond=getConditional();

								//Error 1: %if @{a} %or %or %then
					error=(cond==prev_cond ||
								 //Error 2: %if @{a} %and %or %then
								 (cond==TOKEN_AND && prev_cond==TOKEN_OR) ||
								 //Error 3: %if @{a} %or %and %then
								 (cond==TOKEN_OR && prev_cond==TOKEN_AND) ||
								 //Error 4: %if %and @{a} %then
								 (attrib_count==0 && (cond==TOKEN_AND || cond==TOKEN_OR)));

					if(cond==TOKEN_THEN)
					{
						/* Returns the parser to the token %then because additional
						operations is done whe this token is found */
						column-=cond.length()+1;
						end_eval=true;

									//Error 1: %if @{a} %not %then
						error=(prev_cond==TOKEN_NOT ||
									//Error 2: %if %then
									attrib_count==0 ||
									//Error 3: %if @{a} %and %then
									(and_or_count!=attrib_count-1));
					}
					else if(cond==TOKEN_OR || cond==TOKEN_AND)
						and_or_count++;
				break;

				case CHR_INI_ATTRIB:
					attrib=getAttribute();

					//Raises an error if the attribute does is unknown
					if(attributes.count(attrib)==0 && !ignore_unk_atribs)
					{
						throw Exception(Exception::getErrorMessage(ERR_UNK_ATTRIBUTE)
														.arg(attrib).arg(filename).arg((line + comment_count +1)).arg((column+1)),
														ERR_UNK_ATTRIBUTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}

					//Error 1: A conditional token other than %or %not %and if found on conditional expression
					error=(!cond.isEmpty() && cond!=TOKEN_OR && cond!=TOKEN_AND && cond!=TOKEN_NOT) ||
								//Error 2: A %not token if found after an attribute: %if @{a} %not %then
								(attrib_count > 0 && cond==TOKEN_NOT && prev_cond.isEmpty()) ||
								//Error 3: Two attributes not separated by any conditional token: %if @{a} @{b} %then
								(attrib_count > 0 && cond.isEmpty());

					//Increments the extracted attribute counter
					attrib_count++;

					if(!error)
					{
						//Appliyng the NOT operator if found
						attrib_true=(cond==TOKEN_NOT ? attributes[attrib].isEmpty() : !attributes[attrib].isEmpty());

						//Executing the AND operation if the token is found
						if(cond==TOKEN_AND || prev_cond==TOKEN_AND)
							expr_is_true=(expr_is_true && attrib_true);
						else if(cond==TOKEN_OR || prev_cond==TOKEN_OR)
							expr_is_true=(expr_is_true || attrib_true);
						else
							expr_is_true=attrib_true;

						cond.clear();
						prev_cond.clear();
					}
				break;

				default:
					error=true;
				break;
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),	__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	if(error)
	{
		throw Exception(QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
										.arg(filename).arg((line + comment_count + 1)).arg((column+1)),
										ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	return(expr_is_true);
}

QString SchemaParser::getCodeDefinition(const QString & obj_name, map<QString, QString> &attribs, unsigned def_type)
{
	try
	{
		if(obj_name!="")
		{
			QString filename;

			if(def_type==SQL_DEFINITION)
			{
				vector<QString> vers;

				//Formats the filename
				filename=GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
								 GlobalAttributes::SQL_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR + obj_name + GlobalAttributes::SCHEMA_EXT;

				getPgSQLVersions(vers);
				while(!vers.empty())
				{
					//Setting the @{pgsql[VERSION]} attribute
					attribs[QString("pgsql" + vers.back()).remove(".")]=(vers.back()==pgsql_version ? pgsql_version : "");
					vers.pop_back();
				}

				//Try to get the object definitin from the specified path
				return(getCodeDefinition(filename, attribs));
			}
			else
			{
				filename=GlobalAttributes::SCHEMAS_ROOT_DIR + GlobalAttributes::DIR_SEPARATOR +
								 GlobalAttributes::XML_SCHEMA_DIR + GlobalAttributes::DIR_SEPARATOR + obj_name +
								 GlobalAttributes::SCHEMA_EXT;

				return(convertCharsToXMLEntities(getCodeDefinition(filename, attribs)));
			}
		}
		else return("");
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),	__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void SchemaParser::setIgnoreUnkownAttributes(bool ignore)
{
	ignore_unk_atribs=ignore;
}

QString SchemaParser::convertCharsToXMLEntities(QString buf)
{
	//Configures a text stream to read the entire buffer line by line
	QTextStream ts(&buf);
	QString lin, buf_aux;
	bool xml_header=false, in_comment=false;

	//Sets the text steam to detect UTF8 encoding
	ts.setAutoDetectUnicode(true);

	while(!ts.atEnd())
	{
		lin=ts.readLine();

		//Checks if the current line is a XML header (<?xml...)
		xml_header=(lin.indexOf("<?xml") >= 0);

		//Checks if the current line is a comment start tag
		if(!in_comment)
			in_comment=(lin.indexOf("<!--") >= 0);

		//Case the line is empty, is a xml header or a comment line and does not treat XML entities on it
		if(lin.isEmpty() || xml_header || in_comment)
			lin+="\n";
		else
		{
			QRegExp regexp_vect[]={
				QRegExp("(=\")+"), //Regexp to find the attribute start (attrib=")
				QRegExp("(\")(([\r\n\t])+|(\\ )+|(/>)+|(>)+)") //Regexp to find the attribute end ("\n|\r|\t/>)
			};

			int pos=0, pos1=0, count=0;
			QString str_aux;

			lin+="\n";

			do
			{
				//Try to extract the values using regular expressions
				pos=regexp_vect[0].indexIn(lin, pos);
				pos+=regexp_vect[0].matchedLength();
				pos1=regexp_vect[1].indexIn(lin, pos);

				//Calculates the amount of extracted characters
				count=(pos > 0 ? (pos1-pos) : 0);

				if(pos >= 0 && count > 0)
				{
					//Gets the substring extracted using regexp
					str_aux=lin.mid(pos, count);

					//Replaces the char by the XML entities
					str_aux.replace('\"',XMLParser::CHAR_QUOT);
					str_aux.replace('<',XMLParser::CHAR_LT);
					str_aux.replace('>',XMLParser::CHAR_GT);

					//Puts on the original XML definition the modified string
					lin.replace(pos,count,str_aux);
					pos+=count;
				}
			}

			/* Iterates while the positions of the expressions found is valid.
			 Positions less than 0 indicates that no regular expressions
			 managed to find values */
			while(pos >=0 && pos1 >=0);
		}

		buf_aux+=lin;

		//Reseting the in_comment flag when the current line has a end comment tag
		if(in_comment && lin.indexOf("-->") >= 0)
			in_comment=false;
	}

	return(buf_aux);
}

QString SchemaParser::getCodeDefinition(map<QString,QString> &attribs)
{
	QString object_def;
	unsigned end_cnt, if_cnt;
	int if_level, prev_if_level;
	QString atrib, cond, prev_cond, word, meta, str_aux;
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
			chr=buffer[line][column].toAscii();
			switch(chr)
			{
				/* Increments the number of rows causing the parser
				to get the next line buffer for analysis */
				case CHR_LINE_END:
					line++;
					column=0;
				break;

				case CHR_TABULATION:
				case CHR_SPACE:
					//The parser will ignore the spaces that are not within pure texts
					while(buffer[line][column]==CHR_SPACE ||
								buffer[line][column]==CHR_TABULATION) column++;
				break;

					//Metacharacter extraction
				case CHR_INI_METACHAR:
					meta=getMetaCharacter();

					//Checks whether the extracted token is valid metacharacter
					if(meta!=TOKEN_META_SP && meta!=TOKEN_META_TB &&
						 meta!=TOKEN_META_BR && meta!=TOKEN_META_OB &&
						 meta!=TOKEN_META_CB)
					{
						str_aux=QString(Exception::getErrorMessage(ERR_INV_METACHARACTER))
										.arg(meta).arg(filename).arg(line + comment_count +1).arg(column+1);


						throw Exception(str_aux,ERR_INV_METACHARACTER,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					//Checks whether the metacharacter is part of the  'if' expression (this is an error)
					else if(if_level>=0 && vet_tk_if[if_level] && !vet_tk_then[if_level])
					{
						str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
										.arg(filename).arg(line + comment_count +1).arg(column+1);

						throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else
					{
						//Converting the metacharacter drawn to the character that represents this
						if(meta==TOKEN_META_SP) chr=CHR_SPACE;
						else if(meta==TOKEN_META_TB) chr=CHR_TABULATION;
						else if(meta==TOKEN_META_OB) chr=CHR_INI_PURETEXT; //Currently this constant is used since it returns '[' (open bracket)
						else if(meta==TOKEN_META_CB) chr=CHR_END_PURETEXT; //Currently this constant is used since it returns ']' (close bracket)
						else chr=CHR_LINE_END;

						meta="";
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
				case CHR_INI_ATTRIB:
				case CHR_MID_ATTRIB:
				case CHR_END_ATTRIB:
					atrib=getAttribute();

					//Checks if the attribute extracted belongs to the passed list of attributes
					if(attribs.count(atrib)==0)
					{
						if(!ignore_unk_atribs)
						{
							str_aux=QString(Exception::getErrorMessage(ERR_UNK_ATTRIBUTE))
											.arg(atrib).arg(filename).arg((line + comment_count +1)).arg((column+1));
							throw Exception(str_aux,ERR_UNK_ATTRIBUTE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
						else
							attribs[atrib]="";
					}

					//If the parser is inside an 'if / else' extracting tokens
					if(if_level>=0)
					{
						//If the parser evaluated the 'if' conditional and is inside the current if block
						if(!(!if_expr && vet_tk_if[if_level] && !vet_tk_then[if_level]))
						{
							word=atrib;
							atrib="";
							atrib+=CHR_INI_ATTRIB;
							atrib+=CHR_MID_ATTRIB;
							atrib+=word;
							atrib+=CHR_END_ATTRIB;

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
						if(attribs[atrib]=="")
						{
							str_aux=QString(Exception::getErrorMessage(ERR_UNDEF_ATTRIB_VALUE))
											.arg(atrib).arg(filename).arg(line + comment_count +1).arg(column+1);

							throw Exception(str_aux,ERR_UNDEF_ATTRIB_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}

						/* If the parser is not in an if / else, concatenates the value of the attribute
							directly in definition in sql */
						object_def+=attribs[atrib];
					}
				break;

					//Conditional instruction extraction
				case CHR_INI_CONDITIONAL:
					prev_cond=cond;
					cond=getConditional();

					//Checks whether the extracted token is a valid conditional
					if(cond!=TOKEN_IF && cond!=TOKEN_ELSE &&
						 cond!=TOKEN_THEN && cond!=TOKEN_END &&
						 cond!=TOKEN_OR && cond!=TOKEN_NOT &&
						 cond!=TOKEN_AND)
					{
						str_aux=QString(Exception::getErrorMessage(ERR_INV_CONDITIONAL))
										.arg(cond).arg(filename).arg(line + comment_count +1).arg(column+1);
						throw Exception(str_aux,ERR_INV_CONDITIONAL,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else
					{
						//If the toke is an 'if'
						if(cond==TOKEN_IF)
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
						else if(cond==TOKEN_THEN && if_level>=0)
						{
							//Marks the then thoke flag of the current 'if'
							vet_tk_then[if_level]=true;

							/* Clears the  expression extracted flag from the 'if - then',
								 so that the parser does not generate an error when it encounters another
								 'if - then' with an expression still unextracted */
							if_expr=false;
						}
						//If the parser is in 'if / else' and a 'else' token is found
						else if(cond==TOKEN_ELSE && if_level>=0)
							//Mark the  o flag do token else do if atual
							vet_tk_else[if_level]=true;
						//Case the parser is in 'if/else' and a 'end' token was found
						else if(cond==TOKEN_END && if_level>=0)
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
								vet_aux=NULL;

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
							/* Caso a parte else do if atual exista
								 então o parser varrerá a lista de palavras da parte else
								 do if atual */

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

									//Check if the work is not an attribute
									if(word[0]==CHR_INI_ATTRIB)
									{
										/* If its an attribute, extracts the name and checks if the same
										has empty value */
										atrib=word.mid(2,word.size()-3);
										word=attribs[atrib];

										//If the attribute has no value set raises an exception
										if(word=="")
										{
											str_aux=QString(Exception::getErrorMessage(ERR_UNDEF_ATTRIB_VALUE))
															.arg(atrib).arg(filename).arg(line + comment_count +1).arg(column+1);
											throw Exception(str_aux,ERR_UNDEF_ATTRIB_VALUE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
							if((prev_cond==TOKEN_IF && cond!=TOKEN_THEN) ||
								 (prev_cond==TOKEN_ELSE && cond!=TOKEN_IF && cond!=TOKEN_END) ||
								 (prev_cond==TOKEN_THEN && cond==TOKEN_THEN))
								error=true;
						}

						if(error)
						{
							str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
											.arg(filename).arg(line + comment_count +1).arg(column+1);
							throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
						}
					}
				break;

					//Extraction of pure text or simple words
				default:
					if(chr==CHR_INI_PURETEXT ||
						 chr==CHR_END_PURETEXT)
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
							str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
											.arg(filename).arg(line + comment_count +1).arg(column+1);
							throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
			str_aux=QString(Exception::getErrorMessage(ERR_INVALID_SYNTAX))
							.arg(filename).arg(line + comment_count +1).arg(column+1);
			throw Exception(str_aux,ERR_INVALID_SYNTAX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}


	restartParser();
	ignore_unk_atribs=false;
	return(object_def);
}


QString SchemaParser::getCodeDefinition(const QString &filename, map<QString,QString> &attribs)
{
	try
	{
		loadFile(filename);
		return(getCodeDefinition(attribs));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

