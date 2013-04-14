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
\class TableWidget
\brief  Implements the operations to create/edit tables via form.
*/

#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_tablewidget.h"
#include "objecttablewidget.h"
#include "tableview.h"

class TableWidget: public BaseObjectWidget, public Ui::TableWidget {
	private:
		Q_OBJECT

		/*! \brief Operation list element count before editing the table. This attribute
		is used to know, in case of cancel the edition, the operation (count) that is needed to
		be removed. See: cancelConfiguration() */
		unsigned operation_count;

		ObjectTableWidget *parent_tables;

		//! \brief Stores the objects tables used to handle columns, constraints, indexes, rules and triggers
		map<ObjectType, ObjectTableWidget *> objects_tab_map;

		//! \brief Lists (on the correct object table) the table objects according to the specified type
		void listObjects(ObjectType obj_type);

		//! \brief Shows the specified object data at the correct object table, at specified row
		void showObjectData(TableObject *object, int row);

		//! \brief Returns the object table according with the child type
		ObjectTableWidget *getObjectTable(ObjectType obj_type);

		//! \brief Returns the object type according to the widget (in this case a object table) that called the method
		ObjectType getObjectType(QObject *sender);

		//! \brief Shows the table object editing form according to the specified object type
		void showTableObjectForm(ObjectType obj_type);

		void hideEvent(QHideEvent *event);

	public:
		TableWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Table *table, float pos_x, float pos_y);

	private slots:
		//! \brief Adds or edit a object on the object table that calls the slot
		void handleObject(void);

		//! \brief Removes the selected object from the table that calls the slot
		void removeObject(int row);

		//! \brief Removes all objects from the table that calls the slot
		void removeObjects(void);

		//! \brief Swap the index between two rows of the table that calls the slot
		void swapObjects(int idx1, int idx2);

	public slots:
		void applyConfiguration(void);
		void cancelConfiguration(void);
};

#endif
