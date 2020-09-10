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

#include "transformwidget.h"

TransformWidget::TransformWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Transform)
{
	Ui_TransformWidget::setupUi(this);

	configureFormLayout(nullptr, ObjectType::Transform);

	setMinimumSize(640, 480);
}

void TransformWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Transform *transform)
{
	BaseObjectWidget::setAttributes(model, op_list, transform);
}

void TransformWidget::applyConfiguration()
{
	try
	{
		Transform *transform = nullptr;

		startConfiguration<Transform>();
		transform = dynamic_cast<Transform *>(this->object);

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

