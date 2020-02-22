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
\class ForeignDataWrapperWidget
\brief Implements the operations to create/edit foreign data wrappers via form.
*/

#ifndef FOREIGN_DATA_WRAPPER_WIDGET_H
#define FOREIGN_DATA_WRAPPER_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_foreigndatawrapperwidget.h"
#include "objectstablewidget.h"

class ForeignDataWrapperWidget: public BaseObjectWidget, public Ui::ForeignDataWrapperWidget {
	private:
		Q_OBJECT

		ObjectSelectorWidget *func_handler_sel,
		*func_validator_sel;

		ObjectsTableWidget *options_tab;

	public:
		ForeignDataWrapperWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, ForeignDataWrapper *fdw);

	public slots:
		void applyConfiguration();
};

#endif
