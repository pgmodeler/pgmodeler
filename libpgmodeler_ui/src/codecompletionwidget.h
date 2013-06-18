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

		SyntaxHighlighter *syntax_hl;

		QListWidget *name_list;

		QStringList keywords;

		QTextCursor new_txt_cur, prev_txt_cur;

		QString word;

		DatabaseModel *db_model;

		BaseObject *last_sel_obj;

		bool close_on_select;

		QString expandObjectName(BaseObject *obj);

		bool eventFilter(QObject *object, QEvent *event);

		void populateNameList(vector<BaseObject *> &objects);

		void show(void){}

	public:
		CodeCompletionWidget(SyntaxHighlighter *syntax_hl, const QString &keywords_grp="keywords");

		void setModel(DatabaseModel *db_model);

	public slots:
		void updateList(void);
		void popUp(void);
		void close(void);
};

#endif
