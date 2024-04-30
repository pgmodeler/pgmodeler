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
#include "textblockinfo.h"

class __libgui SyntaxHighlighter: public QSyntaxHighlighter {
	private:
		Q_OBJECT

		struct ExprElement {
			QString pattern;
			bool initial, final,
					exact, case_sensitive;

			ExprElement()
			{
				clear();
			}

			ExprElement(const QString &_pattern, bool _initial, bool _final, bool _exact, bool _case_sensitive)
			{
				pattern = _pattern;
				initial = _initial;
				final = _final;
				exact = _exact;
				case_sensitive = _case_sensitive;

				// An expression can't be both initial and final
				if(initial && final)
					initial = final = false;
			}

			void clear()
			{
				pattern.clear();
				initial = final =
				exact = case_sensitive = false;
			}

			bool isValid()
			{
				return !pattern.isEmpty();
			}

			bool isInitial()
			{
				return initial;
			}

			bool isFinal()
			{
				return final;
			}
		};

		struct FormatGroup {
			QString name;
			QTextCharFormat format;
			bool allow_completion, persistent;
			QList<ExprElement> expr_elements;

			FormatGroup()
			{
				allow_completion = false;
			}

			FormatGroup(const QString &_name, const QTextCharFormat &_format, bool _allow_compl, bool _persistent)
			{
				name = _name;
				format = _format;
				allow_completion = _allow_compl;
				persistent = _persistent;
			}

			bool isPersistent()
			{
				return persistent;
			}

			bool isMultiline()
			{
				bool has_initial = false,
						has_final = false;

				for(auto &expr : expr_elements)
				{
					if(expr.isInitial())
						has_initial = true;

					if(expr.isFinal())
						has_final = true;

					if(has_initial && has_final)
						return true;
				}

				return false;
			}
		};

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
		static constexpr int SimpleBlock = -1,

		/*! \brief Indicates that the current block has an open (but still to close) expression (e.g. multline comments)
		When the highlighter finds this const it'll do special operation like highlight next blocks with the same
		configuration as the current one */
		OpenExprBlock = 0,

		PersistentBlock = 1;

		//! \brief Stores the order in which the groups must be applied
		QStringList fmt_groups_order;

		QMap<QString, FormatGroup> fmt_groups;

		//! \brief Stores the enclosing characters config read from file
		QList<EnclosingCharsCfg> enclosing_chrs;

		//! \brief Indicates if the configuration is loaded or not
		bool conf_loaded,

		/*! \brief This causes the highlighter to ignores any RETURN/ENTER press on QTextEdit causing
		 *  the text to be in a single line. Also, in single line mode, pressing tab on a focused
		 *  input causes the focus to jump to the next widget in the hierarchy */
		single_line_mode;

		//! \brief Stores the custom font size to be used instead of default_font size
		qreal custom_font_size;

		//! \brief Stores the char that triggers the code completion
		QChar	completion_trigger;

		QTimer highlight_timer;

		//! \brief Configures the initial attributes of the highlighter
		void configureAttributes();

		/*! \brief This event filter is used to nullify the line breaks when the highlighter
		 is created in single line edit model */
		bool eventFilter(QObject *object, QEvent *event);

		void setFormat(int start, int end, const QString &group, const ExprElement &expr_elem, TextBlockInfo *blk_info);

		bool matchExpression(const QString &text, int txt_pos, const ExprElement &expr, int &start, int &end);

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
		QStringList getExpressions(const QString &group_name);

		//! \brief Returns the current configured code completion trigger char
		QChar getCompletionTrigger();

		/*! \brief Returns the current font size according to the custom_font_size attribute.
		 *  If it is greater than 0 then its value is returned otherwise the default_font size is returned */
		qreal getCurrentFontSize();

		//! \brief Sets the default font for all instances of this class
		static void setDefaultFont(const QFont &fnt);

	private slots:
		//! \brief Highlight a line of the text
		void highlightBlock(const QString &text);

		//! \brief Clears the loaded configuration
		void clearConfiguration();
};

#endif

