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
\class RelationshipWidget
\brief Implements the operations to create/edit relationships via form.
*/

#ifndef RELATIONSHIP_WIDGET_H
#define RELATIONSHIP_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_relationshipwidget.h"
#include "objecttablewidget.h"

class RelationshipWidget: public BaseObjectWidget, public Ui::RelationshipWidget {
	private:
		Q_OBJECT

		/*! \brief Operation list element count before editing the relationship. This attribute
		is used to know, in case of cancel the edition, the operation (count) that is needed to
		be removed. See: cancelConfiguration() */
		unsigned operation_count;

		SyntaxHighlighter *table1_hl,
											*table2_hl,
											*patterns_hl[6];

		//! \brief Table widgets that stores the attributes, constraint and advanced objects of relationship
		ObjectTableWidget *attributes_tab,
											*constraints_tab,
											*advanced_objs_tab;

		//! \brief Lists the objects of relationship (attributes/constraints) on the respective table widget
		void listObjects(ObjectType obj_type);

		//! \brief Lists the advanced objects in the repective table widget
		void listAdvancedObjects(void);

		/*! \brief Shows the object data in the specified table row. The table widget is idenfied by
		 the current object type */
		void showObjectData(TableObject *object, int row);

		static constexpr unsigned GENERAL_TAB=0,
															ATTRIBUTES_TAB=1,
															CONSTRAINTS_TAB=2,
															SPECIAL_PK_TAB=3,
															ADVANCED_TAB=4;

	protected:
		void setAttributes(DatabaseModel *model, OperationList *op_list, Table *src_tab, Table *dst_tab, unsigned rel_type);

	public:
		RelationshipWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, BaseRelationship *base_rel);

	private slots:
		void hideEvent(QHideEvent *event);

		void addObject(void);
		void editObject(int row);
		void removeObject(int row);
		void removeObjects(void);
		void showAdvancedObject(int row);
		void selectCopyOptions(void);

	public slots:
		void applyConfiguration(void);
		void cancelConfiguration(void);

		friend class ModelWidget;
};

#endif
