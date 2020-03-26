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
\ingroup libpgmodeler_ui
\class CodeCompletionWidget
\brief Widget that handles the code completion (keywords and model object names) on a field that has
the syntax highlighter installed on it.
*/

#ifndef CODE_COMPLETION_WIDGET_H
#define CODE_COMPLETION_WIDGET_H

#include <QWidget>
#include "syntaxhighlighter.h"
#include "databasemodel.h"

class CodeCompletionWidget: public QWidget
{
	private:
		Q_OBJECT

		//! \brief A timer that controls the completion popup
		QTimer popup_timer;
		
		QWidget *completion_wgt;
		
		//! \brief Marks the completion widget as persistent (available only when there is no a database model assinged)
		QCheckBox *persistent_chk;
		
		//! \brief Input field that is associated with the code completion
		QPlainTextEdit *code_field_txt;
		
		//! \brief This widget stores the list of completion items
		QListWidget *name_list;
		
		/*! \brief Stores the keywords retrieved from the syntax highlighter.
		These key words are listed whenever the user call the completion */
		QStringList keywords;
		
		/*! \brief This cursor object is configured to use as a replacer when the user
		selects an item on the completion list */
		QTextCursor new_txt_cur,
		
		//! \brief Stores the current text cursor position on the code field
		prev_txt_cur,
		
		lvl_cur;
		
		//! \brief Current typed word
		QString word;
		
		/*! \brief Stores the char used to trigger the completion widget. If the completion
		was instantiated using a SyntaxHighlighter object this char is retrieved from this last
		or the default (.) is used */
		QChar completion_trigger;
		
		//! \brief Stores the database model used to search for objects and list them on completion
		DatabaseModel *db_model;
		
		/*! \brief This is used to simulate an history of selected object
		whenever the user types the completion trigger char. An example of qualifying is access a column
		of a table by typing the full path to it: public[0].table[1].column[2]. The numbers between brace
		are the qualifying level. */
		int qualifying_level;
		
		//! \brief Indicates if the completion was triggered by typing the completion char
		bool auto_triggered,

		enable_snippets;
		
		//! \brief Store the objects selected for each qualifying level
		vector<BaseObject *> sel_objects;
		
		map<QString, QPixmap> custom_items;
		
		attribs_map custom_items_tips;
		
		//! \brief Puts the selected object name on the current cursor position.
		void insertObjectName(BaseObject *obj);
		
		//! \brief Filters the necessary events to trigger the completion as well to control/select items
		bool eventFilter(QObject *object, QEvent *event);
		
		/*! \brief Insert the objects of the vector into the name listing. The filter parameter is used to
		insert only the object which names matches the filter */
		void populateNameList(vector<BaseObject *> &objects, QString filter=QString());
		
		//! \brief Configures the current qualifying level according to the passed object
		void setQualifyingLevel(BaseObject *obj);
		
	public:
		CodeCompletionWidget(QPlainTextEdit *code_field_txt, bool enable_snippets = false);
		
		/*! \brief Configures the completion. If an syntax highlighter is specified, the completion widget will
		retrive the keywords and the trigger char from it. The keyword group name can be also specified in case the
		highlighter uses an different configuration */
		void configureCompletion(DatabaseModel *db_model, SyntaxHighlighter *syntax_hl=nullptr, const QString &keywords_grp=QString("keywords"));
		
		//! \brief Inserts a custom named item on the list with a custom icon. Custom item will always appear at the beggining of the list
		void insertCustomItem(const QString &name, const QString &tooltip, const QPixmap &icon);
		
		//! \brief Inserts several custom named item on the list with a custom icon. Custom item will always appear at the beggining of the list
		void insertCustomItems(const QStringList &names, const QStringList &tooltips, const QPixmap &icon);

//! \brief Inserts several custom named items on the list with an icon related to the obj_type. Custom item will always appear at the beggining of the list
		void insertCustomItems(const QStringList &names, const QString &tooltip, ObjectType obj_type);
		
		//! \brief Clear the custom added items
		void clearCustomItems();
		
	public slots:
		//! \brief Updates the completion list based upon the typed word
		void updateList();
		
		//! \brief Shows the configured completion list
		void show();
		
		//! \brief Close without select any item on completion list
		void close();
		
		//! \brief Selects an item and closes the completion list
		void selectItem();
		
		void showItemTooltip();
		
	signals:
		//! \brief This signal is emitted whenever a word is placed into the parent textbox through the completion popup.
		void s_wordSelected(QString);

	private slots:
		void handleSelectedWord(QString word);
};

#endif
