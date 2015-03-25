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
\class OperatorClassWidget
\brief Implements the operations to create/edit operator class via form.
*/

#ifndef OPERATOR_CLASS_WIDGET_H
#define OPERATOR_CLASS_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_operatorclasswidget.h"
#include "pgsqltypewidget.h"
#include "objecttablewidget.h"

/* Declaring the OperatorClassElement class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(OperatorClassElement)

class LIBPGMODELER_UI_EXPORT OperatorClassWidget: public BaseObjectWidget, public Ui::OperatorClassWidget {
	private:
		Q_OBJECT

		//! \brief Family, funciton and operator selectors
		ObjectSelectorWidget *family_sel,
												*function_sel,
												*operator_sel,
												*elem_family_sel;

		PgSQLTypeWidget *data_type,
										*storage_type;

		//! \brief Stores the elements of operator class
		ObjectTableWidget *elements_tab;

		void hideEvent(QHideEvent *event);

	public:
		OperatorClassWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, OperatorClass *op_class);

	private slots:
		//! \brief Configures the element form according to the passed element type
		void selectElementType(int elem_type);

		//! \brief Shows the data from the passed element on the element table
		void showElementData(OperatorClassElement elem, int lin_idx);

		/*! \brief Configures an instance of OperatorClassElement using the data on the form, additionally
			the configured element  is shown on the specified line on table */
		void handleElement(int lin_idx);

		//! \brief Fill the form fields with the element data on the specified table line
		void editElement(int lin_idx);

	public slots:
		void applyConfiguration(void);
};

#endif
