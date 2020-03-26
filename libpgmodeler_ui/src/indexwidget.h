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
\class IndexWidget
\brief Implements the operations to create/edit indexes via form.
*/

#ifndef INDEX_WIDGET_H
#define INDEX_WIDGET_H

#include "baseobjectwidget.h"
#include "ui_indexwidget.h"
//#include "objectstablewidget.h"
#include "elementstablewidget.h"

class IndexWidget: public BaseObjectWidget, public Ui::IndexWidget {
	private:
		Q_OBJECT

		//! \brief Syntax highlighter for predicate
		SyntaxHighlighter *predicate_hl;

		//ElementWidget *elements_wgt;
		ElementsTableWidget *elements_tab;

	public:
		IndexWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, BaseTable *parent_obj, Index *index);

	private slots:
		void selectIndexingType();
		//void enableSortingOptions();

	public slots:
		void applyConfiguration();
};

#endif
