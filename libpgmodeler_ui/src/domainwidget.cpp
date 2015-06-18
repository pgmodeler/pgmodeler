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

#include "domainwidget.h"

DomainWidget::DomainWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_DOMAIN)
{
	try
	{
		Ui_DomainWidget::setupUi(this);

		check_expr_hl=nullptr;
		check_expr_hl=new SyntaxHighlighter(check_expr_txt, false);
    check_expr_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		data_type=nullptr;
		data_type=new PgSQLTypeWidget(this);
		domain_grid->addWidget(data_type,4,0,1,2);

		configureFormLayout(domain_grid, OBJ_DOMAIN);
		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->setMinimumSize(540, 570);
		parent_form->setMaximumHeight(570);

		setRequiredField(data_type);
    configureTabOrder({ def_value_edt, constr_name_edt, not_null_chk,
                        check_expr_txt, data_type });
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DomainWidget::hideEvent(QHideEvent *event)
{
	def_value_edt->clear();
	check_expr_txt->clear();
	constr_name_edt->clear();
	not_null_chk->setChecked(false);
	BaseObjectWidget::hideEvent(event);
}

void DomainWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Domain *domain)
{
	PgSQLType type;

	BaseObjectWidget::setAttributes(model, op_list, domain, schema);

	if(domain)
	{
		type=domain->getType();
    def_value_edt->setText(domain->getDefaultValue());
    check_expr_txt->setPlainText(domain->getExpression());
    constr_name_edt->setText(domain->getConstraintName());
		not_null_chk->setChecked(domain->isNotNull());
	}

	data_type->setAttributes(type, model);
}

void DomainWidget::applyConfiguration(void)
{
	try
	{
		Domain *domain=nullptr;
		startConfiguration<Domain>();

		domain=dynamic_cast<Domain *>(this->object);
		domain->setType(data_type->getPgSQLType());
		domain->setDefaultValue(def_value_edt->text());
		domain->setExpression(check_expr_txt->toPlainText());
    domain->setConstraintName(constr_name_edt->text());
		domain->setNotNull(not_null_chk->isChecked());

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

