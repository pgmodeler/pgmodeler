/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "tablespacewidget.h"

TablespaceWidget::TablespaceWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TABLESPACE)
{
	Ui_TablespaceWidget::setupUi(this);
	configureFormLayout(tablespace_grid, OBJ_TABLESPACE);

	tablespace_grid->addItem(new QSpacerItem(10,0,QSizePolicy::Minimum,QSizePolicy::Expanding), tablespace_grid->count(), 0);

	setRequiredField(directory_lbl);
	setRequiredField(directory_edt);

	configureTabOrder();

  setIdealSize(500, 230);
  setSizePadding(30);
}

void TablespaceWidget::hideEvent(QHideEvent *event)
{
	directory_edt->clear();
	BaseObjectWidget::hideEvent(event);
}

void TablespaceWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Tablespace *tablespc)
{
	BaseObjectWidget::setAttributes(model, op_list, tablespc);

	if(tablespc)
		directory_edt->setText(tablespc->getDirectory());
}

void TablespaceWidget::applyConfiguration(void)
{
	try
	{
		Tablespace *tablespc=nullptr;

		startConfiguration<Tablespace>();

		tablespc=dynamic_cast<Tablespace *>(this->object);
		tablespc->setDirectory(directory_edt->text());
		BaseObjectWidget::applyConfiguration();

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

