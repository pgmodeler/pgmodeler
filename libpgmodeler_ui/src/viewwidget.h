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
\class ViewWidget
\brief Implements the operations to create/edit views via form.
*/

#ifndef VIEW_WIDGET_H
#define VIEW_WIDGET_H

#include <QtWidgets>
#include "baseobjectwidget.h"
#include "ui_viewwidget.h"
#include "objecttablewidget.h"
#include "codecompletionwidget.h"

/* Declaring the Reference class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(Reference)

class ViewWidget: public BaseObjectWidget, public Ui::ViewWidget {
	private:
		Q_OBJECT

		QFrame *frame_info;

		/*! \brief Operation list element count before editing the view. This attribute
		is used to know, in case of cancel the edition, the operation (count) that is needed to
		be removed. See: cancelConfiguration() */
		unsigned operation_count;

		//! \brief Stores all the view references
		ObjectTableWidget *references_tab;

		map<ObjectType, ObjectTableWidget *> objects_tab_map;

		SyntaxHighlighter *expression_hl,
											*code_hl,
											*cte_expression_hl;

		CodeCompletionWidget *cte_expression_cp,
												 *expression_cp;

		ObjectSelectorWidget *table_sel,
												*column_sel;

		//! \brief Shows the reference at the reference's table
		void showReferenceData(Reference refer, bool selec_from, bool from_where,
													 bool after_where, bool view_def, unsigned row);

		void clearReferenceForm(void);

		void hideEvent(QHideEvent *);

		//! \brief Returns the object table according with the child type
		ObjectTableWidget *getObjectTable(ObjectType obj_type);
		ObjectType getObjectType(QObject *sender);

		void showObjectData(TableObject *object, int row);

		void listObjects(ObjectType obj_type);

	public:
		ViewWidget(QWidget * parent = 0);

		void setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, View *view, float px, float py);

	private slots:
		//! \brief Shows the field according to the selected reference type
		void selectReferenceType(void);

		//! \brief Creates a reference from the values filled on the form
		void handleReference(int ref_idx);

		//! \brief Edits the selected reference
		void editReference(int ref_idx);

		//! \brief Controls the form which are show the table and column names of the reference
		void showObjectName(void);

		//! \brief Updates the sql code filed of the view form
		void updateCodePreview(void);

		void showTableObjectForm(ObjectType obj_type);

		//! \brief Adds or edit a object on the object table that calls the slot
		void handleObject(void);

		//! \brief Removes the selected object from the table that calls the slot
		void removeObject(int row);

		//! \brief Removes all objects from the table that calls the slot
		void removeObjects(void);

	public slots:
		void applyConfiguration(void);		
		void cancelConfiguration(void);
};

#endif
