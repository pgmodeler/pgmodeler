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

#include "permissionwidget.h"

PermissionWidget::PermissionWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_PERMISSION)
{
	QGridLayout *grid=nullptr;
	QFrame *frame=nullptr;
	QFont font;
	QCheckBox *check=nullptr;
	unsigned i;
	QString privs[]={ ParsersAttributes::SELECT_PRIV, ParsersAttributes::INSERT_PRIV,
										ParsersAttributes::UPDATE_PRIV, ParsersAttributes::DELETE_PRIV,
										ParsersAttributes::TRUNCATE_PRIV, ParsersAttributes::REFERENCES_PRIV,
										ParsersAttributes::TRIGGER_PRIV, ParsersAttributes::CREATE_PRIV,
										ParsersAttributes::CONNECT_PRIV, ParsersAttributes::TEMPORARY_PRIV,
										ParsersAttributes::EXECUTE_PRIV, ParsersAttributes::USAGE_PRIV };

	Ui_PermissionWidget::setupUi(this);

	objectselection_wgt=new ModelObjectsWidget(true);
	permission=nullptr;

	parent_form->generalwidget_wgt->insertWidget(0, this);
	parent_form->generalwidget_wgt->setCurrentIndex(0);
	parent_form->setButtonConfiguration(Messagebox::OK_BUTTON);
	connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), parent_form, SLOT(close(void)));

	parent_form->setMinimumSize(670, 550);
	parent_form->resize(670, 550);

	comment_lbl->setText(trUtf8("Type:"));
	font=name_edt->font();
	font.setItalic(true);
	comment_edt->setFont(font);
	comment_edt->setReadOnly(true);
	name_edt->setFont(font);
	name_edt->setReadOnly(true);

	configureFormLayout(permission_grid, OBJ_PERMISSION);

	roles_tab=new ObjectTableWidget(ObjectTableWidget::ADD_BUTTON |
																	ObjectTableWidget::REMOVE_BUTTON |
																	ObjectTableWidget::EDIT_BUTTON, false, this);
	roles_tab->setColumnCount(1);
	roles_tab->setHeaderLabel(trUtf8("Role"),0);
	roles_tab->setHeaderIcon(QPixmap(":/icones/icones/role.png"),0);

	grid=new QGridLayout;
	grid->addWidget(roles_tab,0,0,1,1);
	grid->setContentsMargins(2,2,2,2);
	roles_gb->setLayout(grid);

	permissions_tab=new ObjectTableWidget(ObjectTableWidget::REMOVE_BUTTON |
																				ObjectTableWidget::EDIT_BUTTON |
																				ObjectTableWidget::REMOVE_ALL_BUTTON, true, this);
	permissions_tab->setColumnCount(3);
	permissions_tab->setHeaderLabel(trUtf8("Id"),0);
	permissions_tab->setHeaderIcon(QPixmap(":/icones/icones/uid.png"),0);
	permissions_tab->setHeaderLabel(trUtf8("Roles"),1);
	permissions_tab->setHeaderIcon(QPixmap(":/icones/icones/role.png"),1);
	permissions_tab->setHeaderLabel(trUtf8("Privileges"),2);
	permissions_tab->setHeaderIcon(QPixmap(":/icones/icones/grant.png"),2);

	grid=new QGridLayout;
	grid->addWidget(permissions_tab,0,0,1,1);
	grid->setContentsMargins(2,2,2,2);
	permissions_gb->setLayout(grid);

	for(i=Permission::PRIV_SELECT; i<=Permission::PRIV_USAGE; i++)
	{
		check=new QCheckBox;
		check->setText(privs[i].toUpper());
		privileges_tbw->insertRow(i);
		privileges_tbw->setCellWidget(i,0,check);
		connect(check, SIGNAL(clicked(bool)), this, SLOT(checkPrivilege(void)));

		check=new QCheckBox;
		check->setText("GRANT OPTION");
		check->setEnabled(false);
		privileges_tbw->setCellWidget(i,1,check);
		connect(check, SIGNAL(clicked(bool)), this, SLOT(checkPrivilege(void)));
	}

	frame=generateInformationFrame(trUtf8("Leave the <em><strong>Roles</strong></em> empty to create a permission applicable to <strong><em>PUBLIC</em></strong>."));
	permission_grid->addWidget(frame, permission_grid->count()+1, 0, 1, 0);
	frame->setParent(this);

	connect(roles_tab, SIGNAL(s_rowAdded(int)), roles_tab, SLOT(selectRow(int)));
	connect(roles_tab, SIGNAL(s_rowEdited(int)), this, SLOT(selectRole(void)));
	connect(roles_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(enableEditButtons(void)));
	connect(roles_tab, SIGNAL(s_rowAdded(int)), this, SLOT(enableEditButtons(void)));

	connect(roles_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(disableGrantOptions(void)));
	connect(roles_tab, SIGNAL(s_rowAdded(int)), this, SLOT(disableGrantOptions(void)));

	connect(permissions_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editPermission(void)));
	connect(permissions_tab, SIGNAL(s_rowSelected(int)), this, SLOT(selectPermission(int)));
	connect(permissions_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removePermission(void)));

	connect(cancel_tb, SIGNAL(clicked(bool)), this, SLOT(cancelOperation(void)));
	connect(add_perm_tb, SIGNAL(clicked(bool)), this, SLOT(addPermission(void)));
	connect(upd_perm_tb, SIGNAL(clicked(bool)), this, SLOT(updatePermission(void)));

	connect(revoke_rb, SIGNAL(toggled(bool)), cascade_chk, SLOT(setEnabled(bool)));
	connect(revoke_rb, SIGNAL(toggled(bool)), this, SLOT(disableGrantOptions(void)));
	connect(grant_rb, SIGNAL(toggled(bool)), this, SLOT(disableGrantOptions(void)));
}

PermissionWidget::~PermissionWidget(void)
{
	delete(objectselection_wgt);
}

void PermissionWidget::hideEvent(QHideEvent *event)
{
	disconnect(objectselection_wgt,0,this,0);
	cancelOperation();

	permissions_tab->blockSignals(true);
	permissions_tab->removeRows();
	permissions_tab->blockSignals(false);

	if(perms_changed)
		emit s_objectManipulated();

	BaseObjectWidget::hideEvent(event);
}

void PermissionWidget::setAttributes(DatabaseModel *model, BaseObject *parent_obj, BaseObject *object)
{
	BaseObjectWidget::setAttributes(model,object,parent_obj);

	perms_changed=false;
	protected_obj_frm->setVisible(false);
	parent_form->apply_ok_btn->setEnabled(true);

	if(object)
	{
		unsigned priv;
		QCheckBox *chk=nullptr, *chk1=nullptr;

		connect(objectselection_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedRoleData(void)));
		connect(roles_tab, SIGNAL(s_rowAdded(int)), this, SLOT(selectRole(void)));
		connect(permissions_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removePermissions(void)));

		name_edt->setText(Utf8String::create(object->getName(true)));
		comment_edt->setText(Utf8String::create(object->getTypeName()));

		for(priv=Permission::PRIV_SELECT; priv<=Permission::PRIV_USAGE; priv++)
		{
			//Gets the checkboxes that represents the privilege and the GRANT OPTION
			chk=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,0));
			chk1=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,1));

			chk->setChecked(false);
			chk1->setChecked(false);

			//Enabling the checkboxes using a validation of privilege type against the curret object type.
			privileges_tbw->setRowHidden(priv, !Permission::objectAcceptsPermission(object->getObjectType(), priv));
		}

		listPermissions();
		permissions_tab->clearSelection();
	}
}

void PermissionWidget::selectRole(void)
{
	objectselection_wgt->setObjectVisible(OBJ_ROLE, true);
	objectselection_wgt->setModel(this->model);
	objectselection_wgt->show();
}

void PermissionWidget::selectPermission(int perm_id)
{
	if(perm_id >= 0)
		permission=reinterpret_cast<Permission *>(permissions_tab->getRowData(perm_id).value<void *>());
	else
		permission=nullptr;
}

void PermissionWidget::disableGrantOptions(void)
{
	QCheckBox *check=nullptr;

	for(unsigned i=Permission::PRIV_SELECT; i<=Permission::PRIV_USAGE; i++)
	{
		check=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(i,1));
		check->setEnabled(roles_tab->getRowCount() > 0);

		if(!check->isEnabled())
			check->setChecked(false);
	}

	cascade_chk->setEnabled(roles_tab->getRowCount() > 0);

	if(!cascade_chk->isEnabled())
		cascade_chk->setChecked(false);
}

void PermissionWidget::listPermissions(void)
{
	if(model)
	{
		vector<Permission *> permissions;
		Permission *perm=nullptr;
		unsigned i, count, i1, count1;
		QString str_aux;

		model->getPermissions(this->object, permissions);
		count=permissions.size();

		permissions_tab->blockSignals(true);
		permissions_tab->removeRows();
		permissions_tab->blockSignals(false);

		for(i=0; i < count; i++)
		{
			perm=permissions[i];

			permissions_tab->addRow();
			permissions_tab->setRowData(QVariant::fromValue<void *>(reinterpret_cast<void *>(perm)),i);
			permissions_tab->setCellText(perm->getName(),i,0);
			permissions_tab->setCellText(perm->getPrivilegeString(),i,2);

			count1=perm->getRoleCount();
			for(i1=0; i1 < count1; i1++)
			{
				str_aux+=Utf8String::create(perm->getRole(i1)->getName());
				str_aux+=",";
			}
			str_aux.remove(str_aux.size()-1,1);
			permissions_tab->setCellText(str_aux,i,1);
			str_aux.clear();
		}
	}
}

void PermissionWidget::showSelectedRoleData(void)
{
	int row, row_idx=-1;
	Role *role=nullptr;

	role=dynamic_cast<Role *>(objectselection_wgt->getSelectedObject());
	row=roles_tab->getSelectedRow();


	if(role)
		row_idx=roles_tab->getRowIndex(QVariant::fromValue<void *>(dynamic_cast<void *>(role)));

	if(role && row_idx < 0)
	{
		roles_tab->setCellText(Utf8String::create(role->getName()), row, 0);
		roles_tab->setRowData(QVariant::fromValue<void *>(dynamic_cast<void *>(role)), row);
	}
	else
	{
		if(!roles_tab->getRowData(row).value<void *>())
			roles_tab->removeRow(row);

		//Raise an error if the role already exists on selected role table
		if(role && row_idx >= 0)
		{
			throw Exception(Exception::getErrorMessage(ERR_ASG_DUPL_OBJ_CONTAINER)
											.arg(Utf8String::create(role->getName()))
											.arg(role->getTypeName())
											.arg(roles_gb->title()),
											ERR_INS_DUPLIC_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
	}
}

void PermissionWidget::addPermission(void)
{
	Permission *perm=nullptr;

	try
	{
		perm=new Permission(this->object);
		configurePermission(perm);
		model->addPermission(perm);
		listPermissions();
		cancelOperation();
		perms_changed=true;
	}
	catch(Exception &e)
	{
		if(perm)
		{
			model->removePermission(perm);
			delete(perm);
		}

		cancelOperation();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PermissionWidget::updatePermission(void)
{
	Permission *perm=nullptr,*perm_bkp=nullptr;
	int perm_idx;

	try
	{
		perm=new Permission(this->object);

		/* Creates a backup permission. This will receive the current values of the
		editing permission, in case of errors these values can be restored */
		perm_bkp=new Permission(this->object);
		(*perm_bkp)=(*permission);

		configurePermission(perm);

		//Checking if the permission already exists on model
		perm_idx=model->getPermissionIndex(perm);

		if(perm_idx < 0 || (perm_idx >=0 && model->getObject(perm_idx,OBJ_PERMISSION)==permission))
		{
			(*permission)=(*perm);
			listPermissions();
			cancelOperation();
		}
		else
		{
			//Raises an error if the configured permission already exists
			throw Exception(Exception::getErrorMessage(ERR_ASG_DUPLIC_PERMISSION)
											.arg(Utf8String::create(permission->getObject()->getName()))
											.arg(permission->getObject()->getTypeName()),
											ERR_ASG_DUPLIC_PERMISSION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		delete(perm_bkp);
		perms_changed=true;
	}
	catch(Exception &e)
	{
		(*permission)=(*perm_bkp);

		delete(perm);
		delete(perm_bkp);

		cancelOperation();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PermissionWidget::editPermission(void)
{
	if(permission)
	{
		unsigned priv, i, count;
		QCheckBox *chk=nullptr, *chk1=nullptr;
		Role *role=nullptr;

		roles_tab->blockSignals(true);
		roles_tab->removeRows();

		perm_disable_sql_chk->setChecked(permission->isSQLDisabled());
		perm_id_edt->setText(permission->getName());
		revoke_rb->setChecked(permission->isRevoke());
		cascade_chk->setChecked(permission->isCascade());

		count=permission->getRoleCount();
		for(i=0; i < count; i++)
		{
			roles_tab->addRow();
			role=permission->getRole(i);
			roles_tab->setRowData(QVariant::fromValue<void *>(reinterpret_cast<void *>(role)), i);
			roles_tab->setCellText(Utf8String::create(role->getName()),i,0);
		}

		roles_tab->blockSignals(false);

		for(priv=Permission::PRIV_SELECT; priv<=Permission::PRIV_USAGE; priv++)
		{
			chk=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,0));
			chk1=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,1));

			chk->setChecked(permission->getPrivilege(priv));
			chk1->setChecked(permission->getGrantOption(priv));
		}

		enableEditButtons();
	}
}

void PermissionWidget::removePermission(void)
{
	model->removePermission(permission);
	cancelOperation();
	permission=nullptr;
	permissions_tab->clearSelection();
	perms_changed=true;
}

void PermissionWidget::removePermissions(void)
{
	model->removePermissions(object);
	cancelOperation();
	perms_changed=true;
}

void PermissionWidget::configurePermission(Permission *perm)
{
	if(perm)
	{
		unsigned count, i, priv;
		QCheckBox *chk=nullptr, *chk1=nullptr;

		perm->setSQLDisabled(perm_disable_sql_chk->isChecked());
		perm->setCascade(cascade_chk->isChecked());
		perm->setRevoke(revoke_rb->isChecked());

		perm->removeRoles();
		count=roles_tab->getRowCount();

		for(i=0; i < count; i++)
			perm->addRole(reinterpret_cast<Role *>(roles_tab->getRowData(i).value<void *>()));

		for(priv=Permission::PRIV_SELECT; priv <= Permission::PRIV_USAGE; priv++)
		{
			if(!privileges_tbw->isRowHidden(priv))
			{
				chk=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,0));
				chk1=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,1));
				perm->setPrivilege(priv, chk->isChecked(), chk1->isChecked());
			}
		}
	}
}

void PermissionWidget::cancelOperation(void)
{
	unsigned priv;
	QCheckBox *chk=nullptr;

	permission=nullptr;

	for(priv=Permission::PRIV_SELECT; priv<=Permission::PRIV_USAGE; priv++)
	{
		chk=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,0));
		chk->setChecked(false);
		chk=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,1));
		chk->setChecked(false);
	}

	roles_tab->removeRows();
	perm_id_edt->clear();
	enableEditButtons();
	cancel_tb->setEnabled(false);
	permissions_tab->clearSelection();
	perm_disable_sql_chk->setChecked(false);
}

void PermissionWidget::checkPrivilege(void)
{
	QObject *obj_sender=sender();

	if(obj_sender && obj_sender->metaObject()->className()==QString("QCheckBox"))
	{
		QCheckBox *chk=nullptr, *chk_priv=nullptr, *chk_gop=nullptr;
		unsigned priv;

		chk=dynamic_cast<QCheckBox *>(obj_sender);

		for(priv=Permission::PRIV_SELECT; priv<=Permission::PRIV_USAGE; priv++)
		{
			chk_priv=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,0));
			chk_gop=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,1));

			if(chk==chk_gop)
			{
				chk_priv->setChecked(chk_gop->isChecked());
				break;
			}
			else if(chk==chk_priv && !chk->isChecked())
			{
				chk_gop->setChecked(false);
				break;
			}
		}

		enableEditButtons();
	}
}

void PermissionWidget::enableEditButtons(void)
{
	bool checked_privs=false;
	unsigned priv;
	QCheckBox *chk=nullptr, *chk1=nullptr;

	for(priv=Permission::PRIV_SELECT; priv<=Permission::PRIV_USAGE && !checked_privs; priv++)
	{
		chk=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,0));
		chk1=dynamic_cast<QCheckBox *>(privileges_tbw->cellWidget(priv,1));
		checked_privs=(chk->isChecked() || chk1->isChecked());
	}

	upd_perm_tb->setEnabled(checked_privs && permission!=nullptr);
	add_perm_tb->setEnabled(checked_privs);
	cancel_tb->setEnabled(add_perm_tb->isEnabled() || upd_perm_tb->isEnabled() || permissions_tab->getRowCount() > 0);
}

