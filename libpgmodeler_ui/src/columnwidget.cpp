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

#include "columnwidget.h"

ColumnWidget::ColumnWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_COLUMN)
{
	try
	{
		QSpacerItem *spacer=new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding);

		Ui_ColumnWidget::setupUi(this);

		hl_default_value=nullptr;
		hl_default_value=new SyntaxHighlighter(def_value_txt, false);
		hl_default_value->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																				GlobalAttributes::DIR_SEPARATOR +
																				GlobalAttributes::SQL_HIGHLIGHT_CONF +
																				GlobalAttributes::CONFIGURATION_EXT);

		data_type=nullptr;
		data_type=new PgSQLTypeWidget(this);
		column_grid->addWidget(data_type,0,0,1,0);
		column_grid->addWidget(def_value_lbl,1,0,1,1);
		column_grid->addWidget(def_value_txt,1,1,1,1);
		column_grid->addWidget(notnull_chk,1,2,1,1);
		column_grid->addItem(spacer,column_grid->count(),0);

		configureFormLayout(column_grid, OBJ_COLUMN);
		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->setMinimumSize(530, 430);
		parent_form->setMaximumHeight(430);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ColumnWidget::hideEvent(QHideEvent *event)
{
	def_value_txt->clear();
	notnull_chk->setChecked(false);
	BaseObjectWidget::hideEvent(event);
}

void ColumnWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj, OperationList *op_list, Column *column)
{
	PgSQLType type;

	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, column, parent_obj);

	if(column)
	{
		type=column->getType();
		notnull_chk->setChecked(column->isNotNull());
		def_value_txt->setPlainText(Utf8String::create(column->getDefaultValue()));
	}

	data_type->setAttributes(type, model, UserTypeConfig::BASE_TYPE | UserTypeConfig::DOMAIN_TYPE | UserTypeConfig::EXTENSION_TYPE,true,false);
}

void ColumnWidget::applyConfiguration(void)
{
	try
	{
		Column *column=nullptr;

		startConfiguration<Column>();

		column=dynamic_cast<Column *>(this->object);
		column->setNotNull(notnull_chk->isChecked());
		column->setDefaultValue(def_value_txt->toPlainText());
		column->setType(data_type->getPgSQLType());

		BaseObjectWidget::applyConfiguration();

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

