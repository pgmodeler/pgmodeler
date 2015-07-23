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

/**
\ingroup libpgmodeler_ui
\class SyntaxHighlighter
\brief Implements a syntax hightlighter with user-defined markup patterns (xml configuration).
*/

#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include <QtWidgets>
#include <map>
#include <vector>
#include "exception.h"
#include "xmlparser.h"
#include "globalattributes.h"
#include "parsersattributes.h"
#include <algorithm>

class SyntaxHighlighter: public QSyntaxHighlighter {
	private:
		Q_OBJECT

    XMLParser xmlparser;

    static QFont default_font;

    static const int SIMPLE_BLOCK=-1,
    OPEN_EXPR_BLOCK=0;

    class BlockInfo: public QTextBlockUserData {
      public:
        QString group;
        bool has_block_expr;
        bool is_expr_closed;

        BlockInfo(void)
        {
          resetBlockInfo();
        }

        void resetBlockInfo(void)
        {
          group.clear();
          has_block_expr=false;
          is_expr_closed=false;
        }
    };

		/*! \brief Stores the regexp used to identify keywords, identifiers, strings, numbers.
		Also stores initial regexps used to identify a multiline group */
		map<QString, vector<QRegExp> > initial_exprs;

		/*! \brief Stores the regexps that indicates the end of a group. This regexps are
		used mainly to identify the end of multiline comments */
		map<QString, vector<QRegExp> > final_exprs;

		//! \brief Stores the text formatting to each group
		map<QString, QTextCharFormat> formats;

		//! \brief Stores the groups related to partial matching
		map<QString, bool> partial_match;

		//! \brief Stores the char used to break the highlight for a group. This char is not highlighted itself.
		map<QString, QChar> lookahead_char;

		//! \brief Stores the order in which the groups must be applied
		vector<QString> groups_order;

		//! \brief Indicates if the configuration is loaded or not
		bool conf_loaded,

					/*! \brief Indicates that the code must be rehighlighted in real time (as the user types).
					If this attribute is set to false, the user must always call the rehighlight method */
					auto_rehighlight,

					/*! \brief This causes the highlighter to ignores any RETURN/ENTER press on QTextEdit causing
							the text to be in a single line. */
          single_line_mode,

          is_rehighlighting;

		//! \brief Stores the chars that indicates word separators
		QString word_separators,

						//! \brief Stores the chars that indicates word delimiters
						word_delimiters,

						//! \brief Stores the chars ignored by the highlighter during the word reading
						ignored_chars;

		//! \brief Stores the char that triggers the code completion
		QChar	completion_trigger;

		//! \brief Configures the initial attributes of the highlighter
		void configureAttributes(void);

		/*! \brief Indentifies the group which the word belongs to.  The other parameters indicates, respectively,
		the lookahead char for the group, the current index (column) on the buffer, the initial match indixe and the
		match length. */
    QString identifyWordGroup(const QString &palavra, const QChar &lookahead_chr, int &match_idx, int &match_len, bool &expr_closed);

		/*! \brief This event filter is used to nullify the line breaks when the highlighter
		 is created in single line edit model */
		bool eventFilter(QObject *object, QEvent *event);

    bool isBlockExpressionGroup(const QString &group);

    bool hasBlockExpression(const QString &txt, const QString &group);

    BlockInfo *createBlockInfo(const QString &group);

    void setFormat(int start, int count, const QString &group);

  public:
		/*! \brief Install the syntax highlighter in a QTextEdit. The boolean param is used to
		enable the auto rehighlight. If this is set to false the user must call the rehighlight method
		every time he modifies the text */
    SyntaxHighlighter(QPlainTextEdit *parent, bool auto_rehighlight, bool single_line_mode=false);

		//! \brief Loads a highlight configuration from a XML file
		void loadConfiguration(const QString &filename);

		//! \brief Returns if the configuration were successfully loaded
		bool isConfigurationLoaded(void);

		/*! \brief Returns the regexp vector of the specified group. The 'final_expr' bool parameter indicates
		that the final expressions must be returned instead of initial expression (default) */
		vector<QRegExp> getExpressions(const QString &group_name, bool final_expr=false);

		//! \brief Returns the current configured code completion trigger char
		QChar getCompletionTrigger(void);

    static void setDefaultFont(const QFont &fnt);

	private slots:
		//! \brief Highlight a line of the text
		void highlightBlock(const QString &txt);

		//! \brief Clears the loaded configuration
    void clearConfiguration(void);
};

#endif

