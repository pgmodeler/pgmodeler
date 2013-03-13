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

#include "operatorfamilywidget.h"

OperatorFamilyWidget::OperatorFamilyWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_OPFAMILY)
{
	QStringList tipos;

	Ui_OperatorFamilyWidget::setupUi(this);
	configureFormLayout(opfamily_grid, OBJ_OPFAMILY);

	connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

	IndexingType::getTypes(tipos);
	indexing_cmb->addItems(tipos);

	parent_form->setMinimumSize(480, 270);
	parent_form->setMaximumHeight(270);
}

void OperatorFamilyWidget::hideEvent(QHideEvent *event)
{
	indexing_cmb->setCurrentIndex(0);
	BaseObjectWidget::hideEvent(event);
}

void OperatorFamilyWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, OperatorFamily *op_family)
{
	BaseObjectWidget::setAttributes(model, op_list, op_family, schema);

	if(op_family)
		indexing_cmb->setCurrentIndex(indexing_cmb->findText(~(op_family->getIndexingType())));
}

void OperatorFamilyWidget::applyConfiguration(void)
{
	try
	{
		OperatorFamily *op_family=NULL;

		startConfiguration<OperatorFamily>();
		op_family=dynamic_cast<OperatorFamily *>(this->object);
		op_family->setIndexingType(IndexingType(indexing_cmb->currentText()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

