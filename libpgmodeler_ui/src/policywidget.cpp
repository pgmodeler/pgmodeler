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
	try
	{
		Ui_PolicyWidget::setupUi(this);

		model_objs_wgt = new ModelObjectsWidget(true, this);
		model_objs_wgt->setObjectVisible(OBJ_ROLE, true);

		using_edt = PgModelerUiNS::createNumberedTextEditor(using_wgt);
		using_edt->setTabChangesFocus(true);
		using_hl = new SyntaxHighlighter(using_edt);
		using_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		check_edt = PgModelerUiNS::createNumberedTextEditor(check_wgt);
		check_edt->setTabChangesFocus(true);
		check_hl = new SyntaxHighlighter(check_edt);
		check_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		roles_tab = new ObjectsTableWidget(ObjectsTableWidget::ALL_BUTTONS ^
																			 (ObjectsTableWidget::DUPLICATE_BUTTON |
																				ObjectsTableWidget::UPDATE_BUTTON |
																				ObjectsTableWidget::EDIT_BUTTON), true, this);
		roles_tab->setColumnCount(1);
		roles_tab->setHeaderLabel(trUtf8("Name"), 0);
		roles_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("uid")), 0);

		QVBoxLayout *vbox = new QVBoxLayout;
		vbox->addWidget(roles_tab);

		QFrame *frame=generateInformationFrame(trUtf8("Leave the <em><strong>Roles</strong></em> grid empty in order to create a %1 applicable to <strong><em>PUBLIC</em></strong>.")
																					 .arg(BaseObject::getTypeName(OBJ_POLICY).toLower()));
		vbox->addWidget(frame);
		frame->setParent(this);
		vbox->setContentsMargins(4,4,4,4);
		attribs_tbw->widget(0)->setLayout(vbox);

		QStringList cmds;
		PolicyCmdType::getTypes(cmds);
		command_cmb->addItems(cmds);

		configureFormLayout(policy_grid, OBJ_POLICY);
		configureTabOrder({ basics_grp, attribs_tbw });

		connect(roles_tab, SIGNAL(s_rowAdded(int)), model_objs_wgt, SLOT(show()));
		connect(model_objs_wgt, SIGNAL(s_visibilityChanged(BaseObject*, bool)), this, SLOT(selectRole(BaseObject*, bool)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PolicyWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseObject *parent_obj, Policy *policy)
{
	if(!parent_obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, policy, parent_obj);
	model_objs_wgt->setModel(model);

	if(policy)
	{
		command_cmb->setCurrentText(~policy->getPolicyCommand());
		permissive_chk->setChecked(policy->isPermissive());
		check_edt->setPlainText(policy->getCheckExpression());
		using_edt->setPlainText(policy->getUsingExpression());

		roles_tab->blockSignals(true);

		for(auto role : policy->getRoles())
		{
			roles_tab->addRow();
			roles_tab->setCellText(role->getName(), roles_tab->getRowCount() - 1, 0);
			roles_tab->setRowData(QVariant::fromValue<void *>(reinterpret_cast<void*>(role)), roles_tab->getRowCount() - 1);
		}

		roles_tab->blockSignals(false);
	}
}

void PolicyWidget::selectRole(BaseObject *role, bool show_wgt)
{
	if(!show_wgt)
	{
		if(!role)
			roles_tab->removeRow(roles_tab->getRowCount() - 1);
		else
		{
			roles_tab->setCellText(role->getName(), roles_tab->getRowCount() - 1, 0);
			roles_tab->setRowData(QVariant::fromValue<void *>(reinterpret_cast<void*>(role)), roles_tab->getRowCount() - 1);
		}
	}
}

void PolicyWidget::applyConfiguration(void)
{
	try
	{
		Policy *policy = nullptr;
		unsigned count, i;

		startConfiguration<Policy>();

		policy =dynamic_cast<Policy *>(this->object);
		policy->removeRoles();
		policy->setUsingExpression(using_edt->toPlainText());
		policy->setCheckExpression(check_edt->toPlainText());
		policy->setPermissive(permissive_chk->isChecked());
		policy->setPolicyCommand(command_cmb->currentText());

		count=roles_tab->getRowCount();

		for(i=0; i < count; i++)
			policy->addRole(reinterpret_cast<Role *>(roles_tab->getRowData(i).value<void *>()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
