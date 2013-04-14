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
\class ConstraintWidget
\brief Implements the operations to create/edit constraints via form.
*/

#ifndef CONSTRAINT_WIDGET_H
#define CONSTRAINT_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_constraintwidget.h"
#include "objecttablewidget.h"
#include "messagebox.h"
#include "elementswidget.h"

class ConstraintWidget: public BaseObjectWidget, public Ui::ConstraintWidget {
	private:
		Q_OBJECT

		QFrame *info_frm, *warn_frm;

		SyntaxHighlighter *check_expr_hl;

		ElementsWidget *excl_elems_wgt;

		/*! \brief Table widgets used to store the columns that forms the constraint as well the
		referenced columns (only for foreign keys) */
		ObjectTableWidget *columns_tab,
											*ref_columns_tab;

		//! \brief Referenced table selector
		ObjectSelectorWidget *ref_table_sel;

		//! \brief Updates the column combo according to the column id. (Constraint::[SOURCE_COLS | REFERENCED_COLS])
		void updateColumnsCombo(unsigned col_id);

		//! \brief Adds the column to the column's table at the specified row
		void addColumn(Column *column, unsigned col_id, int row);

		void hideEvent(QHideEvent *event);

	public:
		ConstraintWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, BaseObject *parent_obj, OperationList *op_list, Constraint *constr);

	private slots:
		//! \brief Shows only the fields related to the selected constraint type
		void selectConstraintType(void);

		//! \brief Selects the referenced table as well updates the combo containing the referenced table columns
		void selectReferencedTable(void);

		void addColumn(int row);
		void removeColumn(int);
		void removeColumns(void);

	public slots:
		void applyConfiguration(void);
};

#endif
