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

#include "domainwidget.h"
#include "numberedtexteditor.h"

DomainWidget::DomainWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Domain)
{
	try
	{
		Ui_DomainWidget::setupUi(this);

		check_expr_hl=nullptr;
		check_expr_hl=new SyntaxHighlighter(check_expr_txt, false, true);
		check_expr_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

		data_type=nullptr;
		data_type=new PgSQLTypeWidget(this);

		QGridLayout *grid = dynamic_cast<QGridLayout *>(dom_attribs_tbw->widget(0)->layout());
		grid->addWidget(data_type, 1, 0, 1, 2);
		grid->addItem(new QSpacerItem(10, 1, QSizePolicy::Fixed,QSizePolicy::Expanding), 2, 0, 1, 1);

		constr_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^ (ObjectsTableWidget::DuplicateButton), true, this);
		constr_tab->setColumnCount(2);

		constr_tab->setHeaderLabel(tr("Name"), 0);
		constr_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("constraint_ck")), 0);

		constr_tab->setHeaderLabel(tr("Expression"), 1);
		constr_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("codigofonte")), 1);

		grid = dynamic_cast<QGridLayout *>(dom_attribs_tbw->widget(1)->layout());
		grid->addWidget(constr_tab, 2, 0, 1, 2);

		connect(constr_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleConstraint(int)));
		connect(constr_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleConstraint(int)));
		connect(constr_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editConstraint(int)));

		configureFormLayout(domain_grid, ObjectType::Domain);
		setRequiredField(data_type);
		configureTabOrder({ def_value_edt, not_null_chk,	data_type, constr_name_edt, check_expr_txt });

		setMinimumSize(580, 580);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DomainWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Domain *domain)
{
	PgSqlType type;

	BaseObjectWidget::setAttributes(model, op_list, domain, schema);

	if(domain)
	{
		type=domain->getType();
		def_value_edt->setText(domain->getDefaultValue());
		not_null_chk->setChecked(domain->isNotNull());

		constr_tab->blockSignals(true);

		for(auto itr : domain->getCheckConstraints())
		{
			constr_tab->addRow();
			constr_tab->setCellText(itr.first, constr_tab->getRowCount() - 1, 0);
			constr_tab->setCellText(itr.second, constr_tab->getRowCount() - 1, 1);
		}

		constr_tab->clearSelection();
		constr_tab->blockSignals(false);
	}

	data_type->setAttributes(type, model);
}

void DomainWidget::handleConstraint(int row)
{
	if(!constr_name_edt->text().isEmpty() && !check_expr_txt->toPlainText().isEmpty())
	{
		constr_tab->setCellText(constr_name_edt->text(), row, 0);
		constr_tab->setCellText(check_expr_txt->toPlainText(), row, 1);
		constr_name_edt->clear();
		check_expr_txt->clear();
	}
	else if(constr_tab->getCellText(row, 0).isEmpty())
		constr_tab->removeRow(row);
}

void DomainWidget::editConstraint(int row)
{
	constr_name_edt->setText(constr_tab->getCellText(row, 0));
	check_expr_txt->setPlainText(constr_tab->getCellText(row, 1));
}

void DomainWidget::applyConfiguration()
{
	try
	{
		Domain *domain=nullptr;
		startConfiguration<Domain>();

		domain=dynamic_cast<Domain *>(this->object);
		domain->setType(data_type->getPgSQLType());
		domain->setDefaultValue(def_value_edt->text());
		domain->setNotNull(not_null_chk->isChecked());
		domain->removeCheckConstraints();

		for(unsigned row = 0; row < constr_tab->getRowCount(); row++)
			domain->addCheckConstraint(constr_tab->getCellText(row, 0), constr_tab->getCellText(row, 1));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

