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

#include "rolewidget.h"
#include "modelobjectswidget.h"

RoleWidget::RoleWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_ROLE)
{
	ObjectTableWidget *obj_tab=nullptr;
	QGridLayout *grid=nullptr;
	QFrame *frame=nullptr;
	map<QString, vector<QWidget *> > fields_map;
	unsigned i;

	Ui_RoleWidget::setupUi(this);
	configureFormLayout(role_grid, OBJ_ROLE);

	object_selection_wgt=new ModelObjectsWidget(true);

	frame=generateInformationFrame(trUtf8("Assigning <strong><em>-1</em></strong> to <strong><em>Connections</em></strong> creates a role without connection limit.<br/>\
																				 Unchecking <strong><em>Validity</em></strong> creates an role that never expires."));

	role_grid->addWidget(frame, role_grid->count()+1, 0, 1, 4);
	frame->setParent(this);

  fields_map[generateVersionsInterval(AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_90)].push_back(can_replicate_chk);
	frame=generateVersionWarningFrame(fields_map);
	role_grid->addWidget(frame, role_grid->count()+1, 0, 1, 0);
	frame->setParent(this);

	connect(validity_chk, SIGNAL(toggled(bool)), validity_dte, SLOT(setEnabled(bool)));
	connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(members_twg, SIGNAL(currentChanged(int)), this, SLOT(configureRoleSelection(void)));
	connect(superusr_chk, SIGNAL(toggled(bool)), this, SLOT(uncheckOptions(void)));

	//Alocation of the member role tables
	for(i=0; i < 3; i++)
	{
		obj_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																		ObjectTableWidget::UPDATE_BUTTON, true, this);
		members_tab[i]=obj_tab;

		obj_tab->setColumnCount(5);

		obj_tab->setHeaderLabel(trUtf8("Role"),0);
    obj_tab->setHeaderIcon(QPixmap(QString(":/icones/icones/role.png")),0);

		obj_tab->setHeaderLabel(trUtf8("Validity"),1);
    obj_tab->setHeaderIcon(QPixmap(QString(":/icones/icones/validade.png")),1);

		obj_tab->setHeaderLabel(trUtf8("Member of"),2);
    obj_tab->setHeaderIcon(QPixmap(QString(":/icones/icones/role.png")),2);

		obj_tab->setHeaderLabel(trUtf8("Members"),3);
    obj_tab->setHeaderIcon(QPixmap(QString(":/icones/icones/role.png")),3);

		obj_tab->setHeaderLabel(trUtf8("Members (Admin.)"),4);
    obj_tab->setHeaderIcon(QPixmap(QString(":/icones/icones/role.png")),4);

		grid=new QGridLayout;
		grid->addWidget(obj_tab,0,0,1,1);
		grid->setContentsMargins(4,4,4,4);
		members_twg->widget(i)->setLayout(grid);
	}

	parent_form->setMinimumSize(580, 650);

	connect(object_selection_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedRoleData(void)));
}

RoleWidget::~RoleWidget()
{
	parent_form->generalwidget_wgt->removeWidget(this);
	delete(object_selection_wgt);
}

void RoleWidget::uncheckOptions(void)
{
	can_login_chk->setChecked(false);
	can_login_chk->setEnabled(!superusr_chk->isChecked());
	can_replicate_chk->setChecked(false);
	can_replicate_chk->setEnabled(!superusr_chk->isChecked());
	inh_perm_chk->setChecked(false);
	inh_perm_chk->setEnabled(!superusr_chk->isChecked());
	create_db_chk->setChecked(false);
	create_db_chk->setEnabled(!superusr_chk->isChecked());
	create_role_chk->setChecked(false);
	create_role_chk->setEnabled(!superusr_chk->isChecked());
}

void RoleWidget::configureRoleSelection(void)
{
	unsigned i;

	//Disconnects all signals from the member role tables
	for(i=0; i < 3; i++)
		disconnect(members_tab[i],0,this,0);

	//Connects the signal/slots only on the current table
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_rowAdded(int)), this, SLOT(selectMemberRole(void)));
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_rowEdited(int)), this, SLOT(selectMemberRole(void)));
}

void RoleWidget::selectMemberRole(void)
{
	object_selection_wgt->setObjectVisible(OBJ_ROLE, true);
	object_selection_wgt->setModel(this->model);
	object_selection_wgt->show();
}

void RoleWidget::hideEvent(QHideEvent *event)
{
	unsigned i;

	for(i=0; i < 3; i++)
			members_tab[i]->blockSignals(true);

	for(i=0; i < 3; i++)
	{
		members_tab[i]->removeRows();
		members_tab[i]->blockSignals(false);
	}

	members_twg->setCurrentIndex(0);
	passwd_edt->clear();
	conn_limit_sb->setValue(conn_limit_sb->minimum());
	superusr_chk->setChecked(false);
	inh_perm_chk->setChecked(false);
	create_db_chk->setChecked(false);
	can_login_chk->setChecked(false);
	create_role_chk->setChecked(false);
	encrypt_pass_chk->setChecked(false);

	BaseObjectWidget::hideEvent(event);
}

void RoleWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Role *role)
{
	if(role)
	{
		conn_limit_sb->setValue(role->getConnectionLimit());
		passwd_edt->setText(role->getPassword());

		validity_chk->setChecked(!role->getValidity().isEmpty());
    validity_dte->setDateTime(QDateTime::fromString(role->getValidity(), QString("yyyy-MM-dd hh:mm:ss")));

		superusr_chk->setChecked(role->getOption(Role::OP_SUPERUSER));
		create_db_chk->setChecked(role->getOption(Role::OP_CREATEDB));
		create_role_chk->setChecked(role->getOption(Role::OP_CREATEROLE));
		encrypt_pass_chk->setChecked(role->getOption(Role::OP_ENCRYPTED));
		inh_perm_chk->setChecked(role->getOption(Role::OP_INHERIT));
		can_login_chk->setChecked(role->getOption(Role::OP_LOGIN));
		can_replicate_chk->setChecked(role->getOption(Role::OP_REPLICATION));
	}

	BaseObjectWidget::setAttributes(model, op_list, role);

	fillMembersTable();
	configureRoleSelection();
}

void RoleWidget::showRoleData(Role *role, unsigned table_id, unsigned row)
{
	if(role)
	{
		QString str_aux;
		Role *aux_role=nullptr;
		unsigned count, i, type_id,
							role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

		if(table_id > 3)
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		members_tab[table_id]->setRowData(QVariant::fromValue(reinterpret_cast<void *>(role)), row);
    members_tab[table_id]->setCellText(role->getName(), row, 0);
		members_tab[table_id]->setCellText(role->getValidity(), row, 1);

		for(type_id=0; type_id < 3; type_id++)
		{
			count=role->getRoleCount(role_types[type_id]);

			for(i=0; i < count; i++)
			{
				aux_role=role->getRole(role_types[type_id], i);
				str_aux+=aux_role->getName();
        if(i < count-1) str_aux+=QString(", ");
			}

      members_tab[table_id]->setCellText(str_aux, row, 2 + type_id);
			str_aux.clear();
		}
	}
}

void RoleWidget::fillMembersTable(void)
{
	if(this->object)
	{
		Role *aux_role=nullptr, *role=nullptr;
		unsigned count, i, type_id,
				role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

		role=dynamic_cast<Role *>(this->object);

		for(type_id=0; type_id < 3; type_id++)
		{
			count=role->getRoleCount(role_types[type_id]);
			members_tab[type_id]->blockSignals(true);

			for(i=0; i < count; i++)
			{
				aux_role=role->getRole(role_types[type_id], i);
				members_tab[type_id]->addRow();
				showRoleData(aux_role, type_id, i);
			}

			members_tab[type_id]->blockSignals(false);
			members_tab[type_id]->clearSelection();
		}
	}
}

void RoleWidget::showSelectedRoleData(void)
{
	unsigned idx_tab;
	int lin, idx_lin=-1;
	BaseObject *obj_sel=nullptr;
	Messagebox msg_box;

	//Get the selected role
	obj_sel=object_selection_wgt->getSelectedObject();

	//Gets the index of the table where the role data is displayed
	idx_tab=members_twg->currentIndex();
	lin=members_tab[idx_tab]->getSelectedRow();

	if(obj_sel)
		idx_lin=members_tab[idx_tab]->getRowIndex(QVariant::fromValue<void *>(dynamic_cast<void *>(obj_sel)));

	//Raises an error if the user try to assign the role as member of itself
	if(obj_sel && obj_sel==this->object)
	{
		/* If the current row does not has a value indicates that it is recently added and does not have
			 data, in this case it will be removed */
		if(!members_tab[idx_tab]->getRowData(lin).value<void *>())
			members_tab[idx_tab]->removeRow(lin);

		msg_box.show(Exception(Exception::getErrorMessage(ERR_ROLE_REF_REDUNDANCY)
                    .arg(obj_sel->getName())
                    .arg(name_edt->text()),
										ERR_ROLE_REF_REDUNDANCY,__PRETTY_FUNCTION__,__FILE__,__LINE__));
	}
	//If the role does not exist on table, show its data
	else if(obj_sel && idx_lin < 0)
		showRoleData(dynamic_cast<Role *>(obj_sel), idx_tab, lin);
	else
	{
		/* If the current row does not has a value indicates that it is recently added and does not have
			 data, in this case it will be removed */
		if(!members_tab[idx_tab]->getRowData(lin).value<void *>())
			members_tab[idx_tab]->removeRow(lin);

		//Raises an error if the role already is in the table
		if(obj_sel && idx_lin >= 0)
		{
			msg_box.show( Exception(Exception::getErrorMessage(ERR_INS_DUPLIC_ROLE)
                      .arg(obj_sel->getName())
                      .arg(name_edt->text()),
											ERR_INS_DUPLIC_ROLE,__PRETTY_FUNCTION__,__FILE__,__LINE__));
		}
	}
}

void RoleWidget::applyConfiguration(void)
{
	Role *role=nullptr, *aux_role=nullptr;
	unsigned count, i, type_id,
			role_types[3]={ Role::REF_ROLE, Role::MEMBER_ROLE, Role::ADMIN_ROLE };

	try
	{
		startConfiguration<Role>();

		role=dynamic_cast<Role *>(this->object);
		//role->setSysid(sysid_sb->value());
		role->setConnectionLimit(conn_limit_sb->value());
		role->setPassword(passwd_edt->text());

		if(validity_chk->isChecked())
      role->setValidity(validity_dte->dateTime().toString(QString("yyyy-MM-dd hh:mm")));
		else
      role->setValidity(QString());

		role->setOption(Role::OP_SUPERUSER, superusr_chk->isChecked());
		role->setOption(Role::OP_CREATEDB, create_db_chk->isChecked());
		role->setOption(Role::OP_CREATEROLE, create_role_chk->isChecked());
		role->setOption(Role::OP_ENCRYPTED, encrypt_pass_chk->isChecked());
		role->setOption(Role::OP_INHERIT, inh_perm_chk->isChecked());
		role->setOption(Role::OP_LOGIN, can_login_chk->isChecked());
		role->setOption(Role::OP_REPLICATION, can_replicate_chk->isChecked());

		for(type_id=0; type_id < 3; type_id++)
		{
			count=members_tab[type_id]->getRowCount();
			if(count > 0) role->removeRoles(role_types[type_id]);

			for(i=0; i < count; i++)
			{
				aux_role=reinterpret_cast<Role *>(members_tab[type_id]->getRowData(i).value<void *>());
				role->addRole(role_types[type_id], aux_role);
			}
		}

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

