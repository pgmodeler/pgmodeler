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
\class ObjectRenameWidget
\brief Widget used to quickly rename objects without open the editing form
*/

#ifndef OBJECT_RENAME_WIDGET_H
#define OBJECT_RENAME_WIDGET_H

#include <QtWidgets>
#include "ui_objectrenamewidget.h"
#include "databasemodel.h"
#include "operationlist.h"
#include "messagebox.h"

class ObjectRenameWidget: public QDialog, public Ui::ObjectRenameWidget {
	private:
		Q_OBJECT

		//! \brief Objects to be renamed
		vector<BaseObject *> objects;

		//! \brief Operation list used to register the modification over object
		OperationList *op_list;

		//! \brief Model used to made duplicity name validations
		DatabaseModel *model;

		void hideEvent(QHideEvent *);

	public:
		ObjectRenameWidget(QWidget *parent);
		void setAttributes(vector<BaseObject *> objs, DatabaseModel *model, OperationList *op_list);

	public slots:
		int exec();

	private slots:
		void applyRenaming();
};

#endif
