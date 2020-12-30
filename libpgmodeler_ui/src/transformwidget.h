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
\class TransformWidget
\brief Implements the operations to create/edit transforms via form.
*/

#ifndef TRANSFORM_WIDGET_H
#define TRANSFORM_WIDGET_H

#include "baseobjectwidget.h"
#include "pgsqltypewidget.h"
#include "objectselectorwidget.h"
#include "ui_transformwidget.h"

class TransformWidget: public BaseObjectWidget, public Ui::TransformWidget {
	private:
		Q_OBJECT

		PgSQLTypeWidget *type_wgt;

		ObjectSelectorWidget *from_sql_func_sel, *to_sql_func_sel, *language_sel;

	public:
		TransformWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Transform *transform);

	public slots:
		void applyConfiguration();
};

#endif
