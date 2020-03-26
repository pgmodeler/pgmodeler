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

		//! \brief RegExp used to validate attribute names
		static const QRegExp AttribRegExp;

		//! \brief Get an attribute name from the buffer on the current position
		QString getAttribute();

		//! \brief Get an conditional instruction from the buffer on the current position
		QString getConditional();

		//! \brief Get an metacharacter from the buffer on the current position
		QString getMetaCharacter();

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
		bool evaluateExpression();

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
		bool evaluateComparisonExpr();

		/*! \brief Creates a new attribute when finding:
		1) %set  {attrib-name} [expr]  or
		2) %set @{existing-attrib} [expr]

		Where [expr] can be pure texts, meta chars or other attributes exists overwrite its value.

		In the second form note the @ at the beginnig of 'existing-attribute', this variation creates
		a new attribute which the name is the value of {existing-attribute}. In this case the parser
		may return error if the value used is an invalid name.

		The %set construction must be the only one in the line otherwise the parser will return
		errors if another instruction starting with % is found. */
		void defineAttribute();

		//! \brief Clears the value of attributes when finding the instruction: %unset {attr1} {attr2}...
		void unsetAttribute();

		//! \brief Increments the column counter while blank chars (space and tabs) are found on the line
		void ignoreBlankChars(const QString &line);

		//! \brief Translates the meta char token to the real character
		char translateMetaCharacter(const QString &meta);

		/*! \brief Get an word from the buffer on the current position (word is any string that isn't
		 a conditional instruction or comment) */
		QString getWord();

		//! \brief Gets a pure text, ignoring elements of the language
		QString getPureText();

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
		static const char CharComment,	//! \brief Character that starts a comment
		CharLineEnd,	//! \brief Character that indicates end of line
		CharSpace,		//! \brief Character that indicates spacing
		CharTabulation,	//! \brief Character that indicates tabulation
		CharIniAttribute,	//! \brief Character that indicates a reference to an attribute
		CharEndAttribute,	//! \brief Character that delimits on the right the attribute name
		CharIniConditional,	//! \brief Character that starts a conditional instruction
		CharIniMetachar,	//! \brief Character that starts a metacharacter
		CharIniPlainText,	//! \brief Character that starts a plain text
		CharEndPlainText,	//! \brief Character that ends a plain text
		CharIniCompExpr,	//! \brief Character that starts a comparison expression
		CharEndCompExpr,	//! \brief Character that ends a comparison expression
		CharValueDelim,	//! \brief Character that delimiters a value (string)
		CharValueOf;	/*! \brief Character that is used on %set instructions to
										create an attribute name based upon another attribute value */

		//! \brief Tokens related to conditional instructions and operators
		static const QString	TokenIf,  // %if
		TokenThen,// %then
		TokenElse,// %else
		TokenEnd, // %end
		TokenOr,  // %or
		TokenNot, // %not
		TokenAnd, // %and
		TokenSet, //%set
		TokenUnset; //%unset

		//! \brief Tokens related to metacharacters
		static const QString	TokenMetaSp,// $sp (space)
		TokenMetaBr,// $br (line break)
		TokenMetaTb,// $tb (tabulation)
		TokenMetaOb,// $ob (open square bracket '[')
		TokenMetaCb,// $cb (close square bracket ']')
		TokenMetaOc,// $ob (open curly bracket '{')
		TokenMetaCc;// $cb (close curly bracket '}')

		//! \brief Tokens related to comparison expressions
		static const QString	TokenEqOper,// == (equal)
		TokenNeOper,// != (not equal)
		TokenGtOper,// > (greater than)
		TokenLtOper,// < (less than)
		TokenGtEqOper,// >= (greater or equal to)
		TokenLtEqOper;// <= (less or equal to)

		//! \brief Constants used to get a specific object definition
		static constexpr unsigned SqlDefinition=0,
		XmlDefinition=1;

		SchemaParser();

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
		void restartParser();

		//! \brief Set if the parser must ignore unknown attributes avoiding exception throwing
		void ignoreUnkownAttributes(bool ignore);

		//! \brief Set if the parser must ignore empty attributes avoiding exception throwing
		void ignoreEmptyAttributes(bool ignore);

		//! \brief Retorns the current PostgreSQL version used by the parser
		QString getPgSQLVersion();

		//! \brief Extracts the attributes names from the currently loaded buffer
		QStringList extractAttributes();
};

#endif
