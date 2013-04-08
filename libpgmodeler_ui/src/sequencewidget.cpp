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

#include "sequencewidget.h"

SequenceWidget::SequenceWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_SEQUENCE)
{
	try
	{
		Ui_SequenceWidget::setupUi(this);

		column_sel=NULL;
		column_sel=new ObjectSelectorWidget(OBJ_COLUMN, true, this);
		sequence_grid->addWidget(column_sel,3,1,1,3);

		configureFormLayout(sequence_grid, OBJ_SEQUENCE);
		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->setMinimumSize(500, 340);
		parent_form->setMaximumHeight(340);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SequenceWidget::hideEvent(QHideEvent *event)
{
	column_sel->clearSelector();
	BaseObjectWidget::hideEvent(event);
}

void SequenceWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Sequence *sequence)
{
	column_sel->setModel(model);

	if(sequence)
	{
		column_sel->setSelectedObject(sequence->getOwnerColumn());
		cyclic_chk->setChecked(sequence->isCycle());
		start_edt->setText(sequence->getStart());
		maximum_edt->setText(sequence->getMaxValue());
		minimum_edt->setText(sequence->getMinValue());
		cache_edt->setText(sequence->getCache());
		increment_edt->setText(sequence->getIncrement());
	}
	else
	{
		cyclic_chk->setChecked(false);
		start_edt->setText("1");
		maximum_edt->setText(Sequence::MAX_POSITIVE_VALUE);
		minimum_edt->setText("0");
		cache_edt->setText("1");
		increment_edt->setText("1");
	}

	BaseObjectWidget::setAttributes(model,op_list,sequence,schema);
}

void SequenceWidget::applyConfiguration(void)
{
	try
	{
		Sequence *sequence=NULL;
		startConfiguration<Sequence>();

		sequence=dynamic_cast<Sequence *>(this->object);
		sequence->setCycle(cyclic_chk->isChecked());
		sequence->setValues(minimum_edt->text(), maximum_edt->text(), increment_edt->text(),
												 start_edt->text(), cache_edt->text());
		sequence->setOwnerColumn(dynamic_cast<Column *>(column_sel->getSelectedObject()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

