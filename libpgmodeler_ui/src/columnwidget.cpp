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

#include "columnwidget.h"

ColumnWidget::ColumnWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_COLUMN)
{
	try
	{
		QSpacerItem *spacer=new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding);


		data_type=nullptr;
		data_type=new PgSQLTypeWidget(this);

		Ui_ColumnWidget::setupUi(this);

		hl_default_value=nullptr;
		hl_default_value=new SyntaxHighlighter(def_value_txt, true);
		hl_default_value->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		sequence_sel=new ObjectSelectorWidget(OBJ_SEQUENCE, true, this);
		sequence_sel->setEnabled(false);


		column_grid->addWidget(data_type,0,0,1,0);
		column_grid->addWidget(default_value_grp,1,0,1,1);

		column_grid->addItem(spacer,column_grid->count(),0);
		dynamic_cast<QGridLayout *>(default_value_grp->layout())->addWidget(sequence_sel, 1, 1);

		configureFormLayout(column_grid, OBJ_COLUMN);
		configureTabOrder({ data_type });

		parent_form->setMinimumSize(530, 480);
		//parent_form->setMaximumHeight(480);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		connect(sequence_rb, &QRadioButton::clicked,
				[=](){ sequence_sel->setEnabled(true); def_value_txt->setEnabled(false); });

		connect(expression_rb, &QRadioButton::clicked,
				[=](){ sequence_sel->setEnabled(false); def_value_txt->setEnabled(true); });

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
	sequence_sel->clearSelector();
	expression_rb->setChecked(true);
	BaseObjectWidget::hideEvent(event);
}

void ColumnWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj, OperationList *op_list, Column *column)
{
	PgSQLType type;

	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, column, parent_obj);
	sequence_sel->setModel(model);

	if(column)
	{
		type=column->getType();
		notnull_chk->setChecked(column->isNotNull());
		def_value_txt->setPlainText(column->getDefaultValue());

		if(column->getSequence())
		{
			sequence_rb->setChecked(true);
			sequence_sel->setEnabled(true);
			sequence_sel->setSelectedObject(column->getSequence());
		}
	}

	data_type->setAttributes(type, model, UserTypeConfig::BASE_TYPE | UserTypeConfig::TABLE_TYPE | UserTypeConfig::VIEW_TYPE |
							 UserTypeConfig::DOMAIN_TYPE | UserTypeConfig::EXTENSION_TYPE, true,false);
}

void ColumnWidget::applyConfiguration(void)
{
	try
	{
		Column *column=nullptr;
		startConfiguration<Column>();

		column=dynamic_cast<Column *>(this->object);
		column->setNotNull(notnull_chk->isChecked());
		column->setType(data_type->getPgSQLType());

		if(expression_rb->isChecked())
			column->setDefaultValue(def_value_txt->toPlainText());
		else
			column->setSequence(sequence_sel->getSelectedObject());

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

