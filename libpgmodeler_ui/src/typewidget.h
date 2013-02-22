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
\class TypeWidget
\brief Implements the operations to create/edit user defined data types via form.
*/

#ifndef TYPE_WIDGET_H
#define TYPE_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_typewidget.h"
#include "pgsqltypewidget.h"
#include "objecttablewidget.h"

/*! \brief Declaring the Parameter class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(Parameter)

class TypeWidget: public BaseObjectWidget, public Ui::TypeWidget {
	private:
		Q_OBJECT

		//! \brief Function selectors widgets
		ObjectSelectorWidget *functions_sel[7];

		//! \brief Type configuration widgets, used to handle "like type" and "element type"
		PgSQLTypeWidget *like_type,
										*element_type;

		//! \brief Tabelas para armazenamento das enumerações e atributos de tipos compostos
		ObjectTableWidget *enumerations_tab,
											*attributes_tab;

	public:
		TypeWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Type *type);

	private slots:
		void hideEvent(QHideEvent *event);

		//! \brief Handles the form showing the fields according to the selected type configuration (base, composite, enumaration)
		void selectTypeConfiguration(void);

		//! \brief Handles the type enumarations
		void handleEnumeration(int row);

		//! \brief Shows the form used to edit/create type attributes
		void showAttributeForm(void);

		//! \brief Shows the attribute configured onto the attributes table
		void showAttributeData(int res);

	public slots:
		void applyConfiguration(void);
};

#endif
