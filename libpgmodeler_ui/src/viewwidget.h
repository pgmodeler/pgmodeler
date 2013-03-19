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

#include <QtGui>
#include "baseobjectwidget.h"
#include "ui_viewwidget.h"
#include "objecttablewidget.h"

/* Declaring the Reference class as a Qt metatype in order to permit
	 that instances of the class be used as data of QVariant and QMetaType */
#include <QMetaType>
Q_DECLARE_METATYPE(Reference)

class ViewWidget: public BaseObjectWidget, public Ui::ViewWidget {
	private:
		Q_OBJECT

		QFrame *frame_info;

		//! \brief Auxiliary view used to validate all the attributes configured on the form
		View aux_view;

		//! \brief Stores all the view references
		ObjectTableWidget *references_tab;

		SyntaxHighlighter *expression_hl,
											*code_hl,
											*cte_expression_hl;

		ObjectSelectorWidget *table_sel,
												*column_sel;

		//! \brief Shows the reference at the reference's table
		void showReferenceData(Reference refer, bool selec_from, bool from_where,
													 bool after_where, bool view_def, unsigned row);

		void clearReferenceForm(void);
		void hideEvent(QHideEvent *);

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

	public slots:
		void applyConfiguration(void);
};

#endif
