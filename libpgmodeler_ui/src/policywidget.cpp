/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "policywidget.h"

PolicyWidget::PolicyWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_POLICY)
{
	Ui_PolicyWidget::setupUi(this);
	configureFormLayout(policy_grid, OBJ_POLICY);
}

void PolicyWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *parent_obj, Policy *policy)
{
	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, policy, parent_obj);
}
