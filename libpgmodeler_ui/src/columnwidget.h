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
\class ColumnWidget
\brief Implements the operations to create/edit columns via form.
*/

#ifndef COLUMN_WIDGET_H
#define COLUMN_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_columnwidget.h"
#include "pgsqltypewidget.h"

class ColumnWidget: public BaseObjectWidget, public Ui::ColumnWidget {
	private:
		Q_OBJECT
		
		SyntaxHighlighter *hl_default_value;
		PgSQLTypeWidget *data_type;
		ObjectSelectorWidget *sequence_sel;
		Sequence ident_col_seq;

	public:
		ColumnWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *parent_obj, Column *column);
		
	private slots:
		void editSequenceAttributes();
		void enableDefaultValueFields();

	public slots:
		void applyConfiguration();
};

#endif
