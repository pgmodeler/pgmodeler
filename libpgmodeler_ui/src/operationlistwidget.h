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
\class OperationListWidget
\brief Implements the widget representation for the operation list, giving access to the undo/redo action over the model.
*/

#ifndef OPERATION_LIST_WIDGET_H
#define OPERATION_LIST_WIDGET_H

#include <QtGui>
#include "ui_operationlistwidget.h"
#include "modelwidget.h"
#include "messagebox.h"

class OperationListWidget: public QWidget, public Ui::OperationListWidget {
	private:
		Q_OBJECT

		ModelWidget *model_wgt;

		//! \brief Updates the operation list and emits the signal s_operationListUpdated to the connected objects
		void notifyUpdateOnModel(void);

	public:
		OperationListWidget(QWidget * parent = 0);

	public slots:
		void updateOperationList(void);
		void setModel(ModelWidget *model);
		void undoOperation(void);
		void redoOperation(void);
		void removeOperations(void);
		void hide(void);

	private slots:
		void selectItem(QTreeWidgetItem *item, int coluna);

	signals:
		void s_operationExecuted(void);
		void s_operationListUpdated(void);
		void s_visibilityChanged(bool);
};

#endif
