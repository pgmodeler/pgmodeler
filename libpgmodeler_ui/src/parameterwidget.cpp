/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "parameterwidget.h"

ParameterWidget::ParameterWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_PARAMETER)
{
	try
	{
		QGridLayout *parameter_grid=nullptr;
		QSpacerItem *hspacer=nullptr;

		Ui_ParameterWidget::setupUi(this);

		data_type=new PgSQLTypeWidget(this);
		parameter_grid=new QGridLayout(this);
		hspacer=new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		parameter_grid->setContentsMargins(0, 0, 0, 0);

		parameter_grid->addWidget(default_value_lbl, 0, 0, 1, 1);
		parameter_grid->addWidget(default_value_edt, 0, 1, 1, 3);
		parameter_grid->addWidget(mode_lbl, 1, 0, 1, 1);
		parameter_grid->addWidget(param_in_chk, 1, 1, 1, 1);
		parameter_grid->addWidget(param_out_chk, 1, 2, 1, 1);
		parameter_grid->addWidget(param_variadic_chk, 1, 3, 1, 1);
		parameter_grid->addItem(hspacer, 1, 3, 1, 1);
		parameter_grid->addWidget(data_type,2,0,1,4);

		configureFormLayout(parameter_grid, OBJ_PARAMETER);
		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(param_variadic_chk, SIGNAL(toggled(bool)), param_in_chk, SLOT(setDisabled(bool)));
		connect(param_variadic_chk, SIGNAL(toggled(bool)), param_out_chk, SLOT(setDisabled(bool)));
		connect(param_in_chk, SIGNAL(toggled(bool)), this, SLOT(enableVariadic(void)));
		connect(param_out_chk, SIGNAL(toggled(bool)), this, SLOT(enableVariadic(void)));

		parent_form->setMinimumSize(500, 300);
		parent_form->setMaximumHeight(300);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ParameterWidget::enableVariadic(void)
{
	param_variadic_chk->setChecked(!param_in_chk->isChecked() &&
																 !param_out_chk->isChecked());
}

void ParameterWidget::hideEvent(QHideEvent *event)
{
	param_in_chk->setChecked(false);
	param_out_chk->setChecked(false);
	default_value_edt->clear();
	BaseObjectWidget::hideEvent(event);
}

void ParameterWidget::setAttributes(Parameter param, DatabaseModel *model)
{
	this->parameter=param;

	param_in_chk->setChecked(param.isIn());
	param_out_chk->setChecked(param.isOut());
	param_variadic_chk->setChecked(param.isVariadic());
  default_value_edt->setText(param.getDefaultValue());
	data_type->setAttributes(param.getType(), model);

	BaseObjectWidget::setAttributes(model,&this->parameter, nullptr);
}

void ParameterWidget::applyConfiguration(void)
{
	try
	{
		parameter.setDefaultValue(default_value_edt->text());
		parameter.setType(data_type->getPgSQLType());
		parameter.setIn(param_in_chk->isChecked());
		parameter.setOut(param_out_chk->isChecked());
		parameter.setVariadic(param_variadic_chk->isChecked());

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

Parameter ParameterWidget::getParameter(void)
{
	return(parameter);
}

