/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class SyntaxHighlighter
\brief Implements a syntax hightlighter with user-defined markup patterns (xml configuration).
*/

#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include "guiglobal.h"
#include <QtWidgets>
#include <map>
#include <vector>
#include "xmlparser.h"
#include <algorithm>

class __libgui SyntaxHighlighter: public QSyntaxHighlighter {
	private:
		Q_OBJECT

		/*! \brief This struct stores the configuration of enclosing characters
		 *  and their respective foreground/background color */
		struct EnclosingCharsCfg {
				QChar open_char, close_char;
				QColor fg_color, bg_color;
		};

		QPlainTextEdit *code_field_txt;

		/*! \brief The default name of the group related to unformatted words.
		 * This is just a dummy group and just serves to force the non-formatting of
		 * any word that doesn't fit the configured groups */
		static const QString UnformattedGroup;

		//! \brief XML parser used to parse configuration files
		XmlParser xmlparser;

		//! \brief Default font configuratoin for all instances os syntax highlighter
		static QFont default_font;

		//! \brief Indicates that the current block has no special meaning
		static constexpr int SimpleBlock=-1,

		/*! \brief Indicates that the current block has an open (but still to close) expression (e.g. multline comments)
		When the highlighter finds this const it'll do special operation like highlight next blocks with the same
		configuration as the current one */
		OpenExprBlock=0;

		/*! \brief Stores the regexp used to identify keywords, identifiers, strings, numbers.
		Also stores initial regexps used to identify a multiline group */
		std::map<QString, std::vector<QRegularExpression> > initial_exprs;

		/*! \brief Stores the regexps that indicates the end of a group. This regexps are
		used mainly to identify the end of multiline comments */
		std::map<QString, std::vector<QRegularExpression> > final_exprs;

		//! \brief Stores the text formatting to each group
		std::map<QString, QTextCharFormat> formats;

		//! \brief Stores the completion allowed status for each group
		std::map<QString, bool> allow_completion;

		//! \brief Stores the char used to break the highlight for a group. This char is not highlighted itself.
		std::map<QString, QChar> lookahead_char;

		//! \brief Stores the order in which the groups must be applied
		std::vector<QString> groups_order;

		//! \brief Stores the enclosing characters config read from file
		std::vector<EnclosingCharsCfg> enclosing_chrs;

		//! \brief Indicates if the configuration is loaded or not
		bool conf_loaded,

		/*! \brief This causes the highlighter to ignores any RETURN/ENTER press on QTextEdit causing
		 *  the text to be in a single line. Also, in single line mode, pressing tab on a focused
		 *  input causes the focus to jump to the next widget in the hierarchy */
		single_line_mode,

		/*! \brief Indicates that nearby (contiguous) word separators must be captured
		 * The default behavior is to stop character capture when the first word separator
		 * is found. Setting this flag to true causes all the contiguous word separators
		 * to be appended to the detected word. An example of contigous capture is for
		 * SQL comment in the for /(slash)*(asterisk) if the nearby capture is not enabled
		 * then the highlighting will not be able to identify multi line comments properly. */
				capt_nearby_separators;

		//! \brief Stores the custom font size to be used instead of default_font size
		qreal custom_font_size;

		//! \brief Stores the chars that indicates word separators
		QString word_separators,

		//! \brief Stores the chars that indicates word delimiters
		word_delimiters,

		//! \brief Stores the chars ignored by the highlighter during the word reading
		ignored_chars;

		//! \brief Stores the char that triggers the code completion
		QChar	completion_trigger;

		QTimer highlight_timer;

		//! \brief Configures the initial attributes of the highlighter
		void configureAttributes();

		/*! \brief Indentifies the group which the word belongs to.  The other parameters indicates, respectively,
	the lookahead char for the group, the current index (column) on the buffer, the initial match index and the
		match length. */
		QString identifyWordGroup(const QString &palavra, const QChar &lookahead_chr, int &match_idx, int &match_len);

		/*! \brief This event filter is used to nullify the line breaks when the highlighter
		 is created in single line edit model */
		bool eventFilter(QObject *object, QEvent *event);

		//! \brief Returns if the specified group contains both initial and final expressions
		bool hasInitialAndFinalExprs(const QString &group);

		//! \brief Renders the block format using the configuration of the specified group
		void setFormat(int start, int count, const QString &group);

		/*! \brief Check if the word matches the specified group by searching the vector of expressions related to it.
		If the word matches then the match_idx and match_len parameters will be configured with the index and length of chars that
		the expression could match. Additionally this method returns a boolean indicating the if the match was successful */
		bool isWordMatchGroup(const QString &word, const QString &group, bool use_final_expr, const QChar &lookahead_chr, int &match_idx, int &match_len);

		//! \brief Applies the enclosing char formats based on the current cursor position on the parent input
		void highlightEnclosingChars(const EnclosingCharsCfg &cfg);

	public:
		/*! \brief Install the syntax highlighter in a QPlainTextEdit.
		 * If single_line_mode is true the highlighter prevents the parent text field to process line breaks.
		 * If use_custom_tab_width is true the highlighter will use the same tab size as NumberedTextEdit class.
		 * If custom_fnt_size is greater than 0 the higlighter will apply that font size instead of the one set in default_font */
		SyntaxHighlighter(QPlainTextEdit *parent, bool single_line_mode=false, bool use_custom_tab_width=false, qreal custom_fnt_size = 0);

		//! \brief Loads a highlight configuration from a XML file
		void loadConfiguration(const QString &filename);

		//! \brief Returns if the configuration were successfully loaded
		bool isConfigurationLoaded();

		/*! \brief Returns the regexp vector of the specified group. The 'final_expr' bool parameter indicates
		that the final expressions must be returned instead of initial expression (default) */
		std::vector<QRegularExpression> getExpressions(const QString &group_name, bool final_expr=false);

		//! \brief Returns the current configured code completion trigger char
		QChar getCompletionTrigger();

		/*! \brief Returns the current font size according to the custom_font_size attribute.
		 *  If it is greater than 0 then its value is returned otherwise the default_font size is returned */
		qreal getCurrentFontSize();

		//! \brief Sets the default font for all instances of this class
		static void setDefaultFont(const QFont &fnt);

	private slots:
		//! \brief Highlight a line of the text
		void highlightBlock(const QString &txt);

		//! \brief Clears the loaded configuration
		void clearConfiguration();
};

#endif

