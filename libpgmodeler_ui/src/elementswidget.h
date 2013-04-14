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
\class ElementWidget
\brief Implements the operations to create/edit constraints (exclude) and indexes elements via form.
*/

#ifndef ELEMENT_WIDGET_H
#define ELEMENT_WIDGET_H

#include <QtGui>
#include "ui_elementswidget.h"
#include "objecttablewidget.h"
#include "objectselectorwidget.h"
#include "baseobjectwidget.h"

/* Declaring the IndexElement and ExcludeElement class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(IndexElement)
Q_DECLARE_METATYPE(ExcludeElement)

class ElementsWidget: public QWidget, public Ui::ElementsWidget {
	private:
		Q_OBJECT
		//! \brief Parent object (table or relationship) from which the columns will be referenced on the elements
		BaseObject *parent_obj;

		//! \brief Syntax highlighter for element expression
		SyntaxHighlighter	*elem_expr_hl;

		//! \brief Table widget used to control the index elements
		ObjectTableWidget *elements_tab;

		//! \brief Operator class selector
		ObjectSelectorWidget *op_class_sel,

		//! \brief Collation selector (only for index elements)
		*collation_sel,

		//! \brief Operator selector (only for exclude elements)
		*operator_sel;

		//! \brief Updates the column combobox with the existent columns on parent table
		void updateColumnsCombo(void);

		//! \brief Shows the element data on the elements table at the specified line
		void showElementData(Element *elem, int elem_idx);
		void showElementData(IndexElement elem, int elem_idx);
		void showElementData(ExcludeElement elem, int elem_idx);
		void setAttributes(DatabaseModel *model, BaseObject *parent_obj);

	public:
		ElementsWidget(QWidget *parent = 0);

		//! \brief Enables the widget to handle index elements
		void setAttributes(DatabaseModel *model, Table *table, vector<IndexElement> &elems);

		//! \brief Enables the widget to handle exclude constraint elements
		void setAttributes(DatabaseModel *model, BaseObject *parent_obj, vector<ExcludeElement> &elems);

		//! \brief Copy the current elements into the list
		void getElements(vector<IndexElement> &elems);

		//! \brief Copy the current elements into the list
		void getElements(vector<ExcludeElement> &elems);

	public slots:
		void clear(void);

	private slots:
		void selectElementObject(void);
		void handleElement(int elem_idx);
		void editElement(int elem_idx);
};

#endif
