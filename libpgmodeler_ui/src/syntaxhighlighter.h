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

		//! \brief Auxiliary class used by the highlighter that stores informations	about multiline code blocks
		class MultiLineInfo: public QTextBlockUserData {
			public:
				//! \brief Column where starts the multiline info
				int start_col,

						//! \brief Block (line) where the multiline starts
						start_block,

						/*! \brief Column where the multiline info ends it can be -1 while the
						highlighter does not find the multiline end char */
						end_col,

						/*! \brief Block (line) where the multiline info ends it can be -1 while
						the highlighter does not find the multiline end char */
						end_block;

				//! \brief Highlight group used to highlight the matching words on multiline
				QString group;

				MultiLineInfo(void)
				{
					this->group="";
					this->start_col=-1;
					this->start_block=-1;
					this->end_col=-1;
					this->end_block=-1;
				}
		};

		/*! \brief Stores the multiline infos and is used to check if the text being typed
		by the user is on a multiline block */
		vector<MultiLineInfo *> multi_line_infos;

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
					single_line_mode;

		//! \brief Stores the chars that indicates word separators
		QString word_separators,

						//! \brief Stores the chars that indicates word delimiters
						word_delimiters,

						//! \brief Stores the chars ignored by the highlighter during the word reading
						ignored_chars;

		//! \brief Stores the char that triggers the code completion
		QChar	completion_trigger;

		//! \brief Current block in which the highlighter is positioned
		int current_block;

		/*! \brief Multiline info count on the current block. This attribute is used to know when
		the highlighter must call the rehighlight method to highlight all the document again */
		unsigned curr_blk_info_count;

		//! \brief Configures the initial attributes of the highlighter
		void configureAttributes(void);

		/*! \brief Indentifies the group which the word belongs to.  The other parameters indicates, respectively,
		the lookahead char for the group, the current index (column) on the buffer, the initial match indixe and the
		match length. */
		QString identifyWordGroup(const QString &palavra, const QChar &lookahead_chr, int idx, int &match_idx, int &match_len);

		/*! \brief Returns the multiline info for the specified start and end column and for the specified block.
		Returns null when no such info could be found. */
		MultiLineInfo *getMultiLineInfo(int col_ini, int end_col, int block);

		//! \brief Removes the multiline info for the specified block index
		void removeMultiLineInfo(int block);

		//! \brief Returns the multiline info count for the specified block
		unsigned getMultiLineInfoCount(int block);

		/*! \brief This event filter is used to nullify the line breaks when the highlighter
		 is created in single line edit model */
		bool eventFilter(QObject *object, QEvent *event);

	public:
		/*! \brief Install the syntax highlighter in a QTextEdit. The boolean param is used to
		enable the auto rehighlight. If this is set to false the user must call the rehighlight method
		every time he modifies the text */
		SyntaxHighlighter(QTextEdit *parent, bool auto_rehighlight, bool single_line_mode=false);

		//! \brief Loads a highlight configuration from a XML file
		void loadConfiguration(const QString &filename);

		//! \brief Returns if the configuration were successfully loaded
		bool isConfigurationLoaded(void);

		/*! \brief Returns the regexp vector of the specified group. The 'final_expr' bool parameter indicates
		that the final expressions must be returned instead of initial expression (default) */
		vector<QRegExp> getExpressions(const QString &group_name, bool final_expr=false);

		//! \brief Returns the current configured code completion trigger char
		QChar getCompletionTrigger(void);

	public slots:
		//! \brief Rehighlight all the document
		void rehighlight(void);

	private slots:
		//! \brief Highlight a line of the text
		void highlightBlock(const QString &txt);

		/*! \brief Validates the text modification made by the user doing the highlight if needed.
		This slot is linked with the contentsChanged() of document because is in it that are
		captured the character count removed and added on the text */
		void validateTextModification(int, int removed, int added);

		//! \brief Clears the loaded configuration
		void clearConfiguration(void);
};

#endif

