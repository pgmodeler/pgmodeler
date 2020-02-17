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

#ifndef POLICY_WIDGET_H
#define POLICY_WIDGET_H

#include <QWidget>
#include "baseobjectwidget.h"
#include "ui_policywidget.h"
#include "policy.h"
#include "objectstablewidget.h"
#include "modelobjectswidget.h"

class PolicyWidget : public BaseObjectWidget, Ui::PolicyWidget {
	private:
		Q_OBJECT

		ModelObjectsWidget *model_objs_wgt;

		ObjectsTableWidget *roles_tab;

		SyntaxHighlighter *using_hl, *check_hl;

		NumberedTextEditor *using_edt, *check_edt;

	public:
		PolicyWidget(QWidget *parent = nullptr);

		void setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *parent_obj, Policy *policy);

	public slots:
		void applyConfiguration();

	private slots:
		void selectRole(BaseObject *role, bool show_wgt);
};

#endif
