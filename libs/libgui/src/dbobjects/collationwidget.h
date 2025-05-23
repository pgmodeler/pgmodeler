/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2025 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class CollationWidget
\brief Implements the operations to create/edit collations via form.
*/

#ifndef COLLATION_WIDGET_H
#define COLLATION_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_collationwidget.h"

class __libgui CollationWidget: public BaseObjectWidget, public Ui::CollationWidget {
	Q_OBJECT

	public:
		CollationWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Collation *collation);
		
	public slots:
		void applyConfiguration();
		
	private slots:
		void resetFields();
};

#endif
