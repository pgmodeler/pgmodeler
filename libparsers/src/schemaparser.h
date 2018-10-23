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
#include "attribsmap.h"
#include "pgsqlversions.h"

class SchemaParser {
	private:
		/*! \brief Indicates that the parser should ignore unknown
		 attributes avoiding raising exceptions */
		bool ignore_unk_atribs;

		/*! \brief Indicates that the parser should ignore empty
		 attributes avoiding raising exceptions */
		bool ignore_empty_atribs;

		static const char CHR_COMMENT,			//! \brief Character that starts a comment
		CHR_LINE_END,			//! \brief Character that indicates end of line
		CHR_SPACE,        //! \brief Character that indicates spacing
		CHR_TABULATION,   //! \brief Character that indicates tabulation
		CHR_INI_ATTRIB,   //! \brief Character that indicates a reference to an attribute
		CHR_END_ATTRIB,   //! \brief Character that delimits on the right the attribute name
		CHR_INI_CONDITIONAL,//! \brief Character that starts a conditional instruction
		CHR_INI_METACHAR,   //! \brief Character that starts a metacharacter
		CHR_INI_PURETEXT,   //! \brief Character that starts a puretext
		CHR_END_PURETEXT,   //! \brief Character that ends a puretext
		CHR_INI_CEXPR,      //! \brief Character that starts a comparison expression
		CHR_END_CEXPR,      //! \brief Character that ends a comparison expression
		CHR_VAL_DELIM,      //! \brief Character that delimiters a value (string)
		CHR_VALUE_OF;      /*! \brief Character that is used on %set instructions to
											  create an attribute name based upon another attribute value */

		//! \brief Tokens related to conditional instructions and operators
		static const QString	TOKEN_IF,  // %if
		TOKEN_THEN,// %then
		TOKEN_ELSE,// %else
		TOKEN_END, // %end
		TOKEN_OR,  // %or
		TOKEN_NOT, // %not
		TOKEN_AND, // %and
		TOKEN_SET, //%set
		TOKEN_UNSET; //%unset


		//! \brief Tokens related to metacharacters
		static const QString	TOKEN_META_SP,// $sp (space)
		TOKEN_META_BR,// $br (line break)
		TOKEN_META_TB,// $tb (tabulation)
		TOKEN_META_OB,// $ob (open square bracket '[')
		TOKEN_META_CB,// $cb (close square bracket ']')
		TOKEN_META_OC,// $ob (open curly bracket '{')
		TOKEN_META_CC;// $cb (close curly bracket '}')

		//! \brief Tokens related to comparison expressions
		static const QString	TOKEN_EQ_OP,// == (equal)
		TOKEN_NE_OP,// != (not equal)
		TOKEN_GT_OP,// > (greater than)
		TOKEN_LT_OP,// < (less than)
		TOKEN_GT_EQ_OP,// >= (greater or equal to)
		TOKEN_LT_EQ_OP;// <= (less or equal to)

		//! \brief RegExp used to validate attribute names
		static const QRegExp ATTR_REGEXP;

		//! \brief Get an attribute name from the buffer on the current position
		QString getAttribute(void);

		//! \brief Get an conditional instruction from the buffer on the current position
		QString getConditional(void);

		//! \brief Get an metacharacter from the buffer on the current position
		QString getMetaCharacter(void);

		/*! \brief Returns the result (true|false) of conditional expression evaluation.
		The expression is evaluated from the left to the right and not support Polish Notation, so
		as the parser finds an attribute evaluates it and stores the result, when find another one
		evaluates and compare with previous evaluation, and so on. Example:

		Let's suppose a1 and a2 has values and a3 is empty, the following expression
		results will be:

		%if {a1} %or {a2} %and {a3} %then --> FALSE
		%if {a1} %or {a2} %and %not {a3} %then --> TRUE
		%if {a1} %or %not {a3} %then --> TRUE
		%if {a1} %and {a3} %then --> FALSE
		*/
		bool evaluateExpression(void);

		/*! \brief Returns the result (true|false) of a comparison expression. A comparison expression
		have the form: ( {attribute} [operator] "value" ), where:

		(           --> Starts the expression
		{attribute} --> Is the attribute to be compared to a value
		[operator]  --> A valid comparison operator:
						== (equal), != (not equal), < (less than), > (greater than)
						<= (less or equal to), >= (greater or equal to)
		)           --> Closes the expression

		The parenthesis are mandatory otherwise the parser will not recognize the expression
		and raise an exception. Multiple expressions combined with logical operators
		%not %and %or in the same () are not supported. */
		bool evaluateComparisonExpr(void);

		/*! \brief Creates a new attribute when finding:
		1) %set  {attrib-name} [expr]  or
		2) %set @{existing-attrib} [expr]

		Where [expr] can be pure texts, meta chars or other attributes exists overwrite its value.

		In the second form note the @ at the beginnig of 'existing-attribute', this variation creates
		a new attribute which the name is the value of {existing-attribute}. In this case the parser
		may return error if the value used is an invalid name.

		The %set construction must be the only one in the line otherwise the parser will return
		errors if another instruction starting with % is found. */
		void defineAttribute(void);

		//! \brief Clears the value of attributes when finding the instruction: %unset {attr1} {attr2}...
		void unsetAttribute(void);

		//! \brief Increments the column counter while blank chars (space and tabs) are found on the line
		void ignoreBlankChars(const QString &line);

		//! \brief Translates the meta char token to the real character
		char translateMetaCharacter(const QString &meta);

		/*! \brief Get an word from the buffer on the current position (word is any string that isn't
		 a conditional instruction or comment) */
		QString getWord(void);

		//! \brief Gets a pure text, ignoring elements of the language
		QString getPureText(void);

		/*! \brief Returns whether a character is special i.e. indicators of attributes
		 or conditional instructions */
		bool isSpecialCharacter(char chr);

		//! \brief Filename that was loaded by the parser
		QString filename;

		/*! \brief Vectorial representation of the loaded file. This is the buffer that is
		 analyzed by de parser */
		QStringList buffer;

		int line, //! \brief Current line where the parser reading is
		column, //! \brief Current column where the parser reading is

		/*! \brief Comment line ammout extracted. This attribute is used to make the correct
				  reference to the line on file that has syntax errors */
		comment_count;

		attribs_map attributes;

		//! \brief PostgreSQL version currently used by the parser
		QString pgsql_version;

	public:
		//! \brief Constants used to get a specific object definition
		static constexpr unsigned SQL_DEFINITION=0,
		XML_DEFINITION=1;

		SchemaParser(void);

		/*! \brief Set the version of PostgreSQL to be adopted by the parser in obtaining
		 the definition of the objects. This function should always be called at
		 software startup or when the user wants to change the default version
		 of the database */
		void setPgSQLVersion(const QString &pgsql_ver);

		/*! \brief Returns the complete xml/sql definition for an database object represented by the
		 map 'attributes'. For SQL definition is necessary to indicate the version of PostgreSQL
		 in order to the to correct schema be loaded */
		QString getCodeDefinition(const QString &obj_name, attribs_map &attribs, unsigned def_type);

		/*! \brief Generic method that loads a schema file and for a given map of attributes
		 this method returns the data of the file analyzed and filled with the values ​​of the
		 attributes map */
		QString getCodeDefinition(const QString &filename, attribs_map &attribs);

		/*! \brief Generic method that interprets a pre-specified buffer (see loadBuffer()) and for a given map
		 of attributes this method returns the data of the buffer analyzed and filled with the values ​​of the
		 attributes map */
		QString getCodeDefinition(attribs_map &attribs);

		//! \brief Loads the buffer with a string
		void loadBuffer(const QString &buf);

		//! \brief Loads a schema file and inserts its line into the parser's buffer
		void loadFile(const QString &filename);

		//! \brief Resets the parser in order to do new analysis
		void restartParser(void);

		//! \brief Set if the parser must ignore unknown attributes avoiding exception throwing
		void ignoreUnkownAttributes(bool ignore);

		//! \brief Set if the parser must ignore empty attributes avoiding exception throwing
		void ignoreEmptyAttributes(bool ignore);

		//! \brief Retorns the current PostgreSQL version used by the parser
		QString getPgSQLVersion(void);

		//! \brief Extracts the attributes names from the currently loaded buffer
		QStringList extractAttributes(void);

		/*! \brief Converts any chars (operators) < > " to the respective XML entities. This method is only
	called when generating XML code and only tag attributes are treated.*/
		static QString convertCharsToXMLEntities(QString buf);
};

#endif
