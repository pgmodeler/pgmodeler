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

/**
\ingroup libparsers
\brief ParserEsquema class definition used to create SQL definition for model objects from schemas files (stored on disk).
\note <strong>Creation date:</strong> 19/06/2008
*/

#ifndef SCHEMA_PARSER_H
#define SCHEMA_PARSER_H

#include "globalattributes.h"
#include "exception.h"
#include <map>
#include <vector>
#include <QDir>
#include <QTextStream>
#include "xmlparser.h"

class SchemaParser {
	private:
		/*! \brief Indicates that the parser should ignore unknown
		 attributes avoiding raising exceptions */
		static bool ignore_unk_atribs;

		static const char CHR_COMMENT,			//! \brief Character that starts a comment
											CHR_LINE_END,			//! \brief Character that indicates end of line
											CHR_SPACE,        //! \brief Character that indicates spacing
											CHR_TABULATION,   //! \brief Character that indicates tabulation
											CHR_INI_ATTRIB,   //! \brief Character that indicates a reference to an attribute
											CHR_MID_ATTRIB,   //! \brief Character that delimits on the left the attribute name
											CHR_END_ATTRIB,   //! \brief Character that delimits on the right the attribute name
											CHR_INI_CONDITIONAL,//! \brief Character that starts a conditional instruction
											CHR_INI_METACHAR,   //! \brief Character that starts a metacharacter
											CHR_INI_PURETEXT,   //! \brief Character that starts a puretext
											CHR_END_PURETEXT;   //! \brief Character that ends a puretext

		//! \brief Tokens related to conditional instructions
		static const QString	TOKEN_IF,  // %if
													TOKEN_THEN,// %then
													TOKEN_ELSE,// %else
													TOKEN_END, // %end
													TOKEN_OR,  // %or
													TOKEN_NOT, // %not
													TOKEN_AND; // %and

		//! \brief Tokens related to metacharacters
		static const QString	TOKEN_META_SP,// $sp (space)
													TOKEN_META_BR,// $br (line break)
													TOKEN_META_TB,// $tb (tabulation)
													TOKEN_META_OB,// $ob (open bracket '[')
													TOKEN_META_CB;// $cb (close bracket ']')


		//! \brief Get an attribute name from the buffer on the current position
		static QString getAttribute(void);

		//! \brief Get an conditional instruction from the buffer on the current position
		static QString getConditional(void);

		//! \brief Get an metacharacter from the buffer on the current position
		static QString getMetaCharacter(void);

		/*! \brief Returns the result (true|false) of conditional expression evaluation.
		The expression is evaluated from the left to the right and not support Polish Notation, so
		as the parser finds an attribute evaluates it and stores the result, when find another one
		evaluates and compare with previous evaluation, and so on. Example:

		Let's suppose a1 and a2 has values and a3 is empty, the following expression
		results will be:

				%if @{a1} %or @{a2} %and {a3} %then --> FALSE
				%if @{a1} %or @{a2} %and %not {a3} %then --> TRUE
				%if @{a1} %or %not @{a3} %then --> TRUE
				%if @{a1} %and @{a3} %then --> FALSE
		*/
		static bool evaluateExpression(void);

		/*! \brief Get an word from the buffer on the current position (word is any string that isn't
		 a conditional instruction or comment) */
		static QString getWord(void);

		//! \brief Gets a pure text, ignoring elements of the language
		static QString getPureText(void);

		/*! \brief Returns whether a character is special i.e. indicators of attributes
		 or conditional instructions */
		static bool isSpecialCharacter(char chr);

		/*! \brief Converts any chars (operators) < > " to the respective XML entities. This method is only
		called when generating XML code and only tag attributes are treated.*/
		static QString convertCharsToXMLEntities(QString buf);

		//! \brief Filename that was loaded by the parser
		static QString filename;

		/*! \brief Vectorial representation of the loaded file. This is the buffer that is
		 analyzed by de parser */
		static vector<QString> buffer;

		static unsigned line, //! \brief Current line where the parser reading is
										column, //! \brief Current column where the parser reading is

										/*! \brief Comment line ammout extracted. This attribute is used to make the correct
										 reference to the line on file that has syntax errors */
										comment_count;

		static map<QString, QString> attributes;

		//! \brief PostgreSQL version currently used by the parser
		static QString pgsql_version;

	public:

		//! \brief Constants used to refernce PostgreSQL versions.
		static const QString PGSQL_VERSION_90,
												 PGSQL_VERSION_91,
												 PGSQL_VERSION_92;

		//! \brief Constants used to get a specific object definition
		static constexpr unsigned SQL_DEFINITION=0,
															XML_DEFINITION=1;

		/*! \brief Set the version of PostgreSQL to be adopted by the parser in obtaining
		 the definition of the objects. This function should always be called at
		 software startup or when the user wants to change the default version
		 of the database */
		static void setPgSQLVersion(const QString &pgsql_ver);

		/*! \brief Returns the complete xml/sql definition for an database object represented by the
		 map 'attributes'. For SQL definition is necessary to indicate the version of PostgreSQL
		 in order to the to correct schema be loaded */
		static QString getCodeDefinition(const QString &obj_name, map<QString, QString> &attribs, unsigned def_type);

		/*! \brief Generic method that loads a schema file and for a given map of attributes
		 this method returns the data of the file analyzed and filled with the values ​​of the
		 attributes map */
		static QString getCodeDefinition(const QString &filename, map<QString, QString> &attribs);

		/*! \brief Generic method that interprets a pre-specified buffer (see loadBuffer()) and for a given map
		 of attributes this method returns the data of the buffer analyzed and filled with the values ​​of the
		 attributes map */
		static QString getCodeDefinition(map<QString, QString> &attribs);

		//! \brief Loads the buffer with a string
		static void loadBuffer(const QString &buf);

		//! \brief Loads a schema file and inserts its line into the parser's buffer
		static void loadFile(const QString &filename);

		/*! \brief Returns the PostgreSQL version available based on the subdirectories on schema/sql.
		 To be recognized as a version, the directory name must follow the rule: [NUMBER].[NUMBER](.[NUMBER])*/
		static void getPgSQLVersions(vector<QString> &versions);

		//! \brief Retorns the current PostgreSQL version used by the parser
		static QString getPgSQLVersion(void);

		//! \brief Resets the parser in order to do new analysis
		static void restartParser(void);

		//! \brief Set if the parser must ignore unknown attributes avoiding expcetion throwing
		static void setIgnoreUnkownAttributes(bool ignore);
};

#endif
