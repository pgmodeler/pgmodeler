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
\class OperatorWidget
\brief Implements the operations to create/edit operators via form.
*/

#ifndef OPERATOR_WIDGET_H
#define OPERATOR_WIDGET_H

#include "baseobjectwidget.h"
#include "pgsqltypewidget.h"
#include "ui_operatorwidget.h"

class LIBPGMODELER_UI_EXPORT OperatorWidget: public BaseObjectWidget, public Ui::OperatorWidget {
	private:
		Q_OBJECT

		PgSQLTypeWidget *arg_types[2];

		ObjectSelectorWidget *functions_sel[3],
												 *operators_sel[2];

	public:
		OperatorWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Operator *oper);

	private slots:
		void hideEvent(QHideEvent *event);

	public slots:
		void applyConfiguration(void);
};

#endif
