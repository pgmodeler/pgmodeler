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
\class SQLAppendWidget
\brief Widget that handles insertion of free SQL commands into object's definition
*/

#ifndef SQL_APPEND_WIDGET_H
#define SQL_APPEND_WIDGET_H

#include "baseobjectwidget.h"
#include "codecompletionwidget.h"
#include "ui_sqlappendwidget.h"

class SQLAppendWidget: public BaseObjectWidget, public Ui::SQLAppendWidget {
	private:
		Q_OBJECT

		SyntaxHighlighter *sqlcode_hl;

		CodeCompletionWidget *sqlcode_cp;

		QMenu insert_menu,
					delete_menu,
					update_menu,
					select_menu;

		QAction *action_gen_insert,
						*action_gen_select,
						*action_inc_serials,
						*action_exc_serials,
						*action_tab_select,
						*action_tab_update,
						*action_gen_update,
						*action_gen_delete,
						*action_tab_delete;

		//! \brief Configures the code template menus according to the loaded object
		void configureMenus(void);

	public:
		SQLAppendWidget(QWidget *parent=0);

		void setAttributes(DatabaseModel *model, BaseObject *object);
		
	public slots:
		void applyConfiguration(void);

	private slots:
		void addCommand(void);
		void clearCode(void);
};

#endif
