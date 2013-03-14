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
\class IndexWidget
\brief Implements the operations to create/edit indexes via form.
*/

#ifndef INDEX_WIDGET_H
#define INDEX_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_indexwidget.h"
#include "objecttablewidget.h"

/* Declaring the IndexElement class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(IndexElement)

class IndexWidget: public BaseObjectWidget, public Ui::IndexWidget {
	private:
		Q_OBJECT

		//! \brief Syntax highlighter for conditional expression
		SyntaxHighlighter *cond_expr_hl,

											//! \brief Syntax highlighter for element expression
											*elem_expr_hl;

		//! \brief Table widget used to control the index elements
		ObjectTableWidget *elements_tab;

		//! \brief Operator class selector
		ObjectSelectorWidget *op_class_sel;

		//! \brief Updates the column combobox with the existent columns on parent table
		void updateColumnsCombo(void);

		//! \brief Shows the element data on the elements table at the specified line
		void showElementData(IndexElement elem, int elem_idx);

	public:
		IndexWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, Table *parent_obj, OperationList *op_list, Index *index);

	private slots:
		void hideEvent(QHideEvent *event);
		void selectElementObject(void);
		void selectIndexingType(void);
		void handleElement(int elem_idx);
		void editElement(int elem_idx);

	public slots:
		void applyConfiguration(void);
};

#endif
