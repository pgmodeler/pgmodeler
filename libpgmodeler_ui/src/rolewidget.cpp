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

#include "rolewidget.h"
#include "modelobjectswidget.h"

RoleWidget::RoleWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Role)
{
	ObjectsTableWidget *obj_tab=nullptr;
	QGridLayout *grid=nullptr;
	QFrame *frame=nullptr;
	map<QString, vector<QWidget *> > fields_map;
	unsigned i;

	Ui_RoleWidget::setupUi(this);
	configureFormLayout(role_grid, ObjectType::Role);

	object_selection_wgt=new ModelObjectsWidget(true);

	frame=generateInformationFrame(tr("Assigning <strong><em>-1</em></strong> to <strong><em>Connections</em></strong> creates a role without connection limit.<br/>\
										  Unchecking <strong><em>Validity</em></strong> creates an role that never expires."));

	role_grid->addWidget(frame, role_grid->count()+1, 0, 1, 4);
	frame->setParent(this);

	fields_map[generateVersionsInterval(AfterVersion, PgSqlVersions::PgSqlVersion91)].push_back(can_replicate_chk);
	fields_map[generateVersionsInterval(AfterVersion, PgSqlVersions::PgSqlVersion95)].push_back(bypass_rls_chk);
	frame=generateVersionWarningFrame(fields_map);
	role_grid->addWidget(frame, role_grid->count()+1, 0, 1, 0);
	frame->setParent(this);

	connect(validity_chk, SIGNAL(toggled(bool)), validity_dte, SLOT(setEnabled(bool)));
	connect(members_twg, SIGNAL(currentChanged(int)), this, SLOT(configureRoleSelection()));

	//Alocation of the member role tables
	for(i=0; i < 3; i++)
	{
		obj_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^
																	(ObjectsTableWidget::UpdateButton | ObjectsTableWidget::DuplicateButton), true, this);
		members_tab[i]=obj_tab;

		obj_tab->setColumnCount(5);

		obj_tab->setHeaderLabel(tr("Role"),0);
		obj_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("role")),0);

		obj_tab->setHeaderLabel(tr("Validity"),1);
		obj_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("validade")),1);

		obj_tab->setHeaderLabel(tr("Member of"),2);
		obj_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("role")),2);

		obj_tab->setHeaderLabel(tr("Members"),3);
		obj_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("role")),3);

		obj_tab->setHeaderLabel(tr("Members (Admin.)"),4);
		obj_tab->setHeaderIcon(QPixmap(PgModelerUiNs::getIconPath("role")),4);

		grid=new QGridLayout;
		grid->addWidget(obj_tab,0,0,1,1);
		grid->setContentsMargins(4,4,4,4);
		members_twg->widget(i)->setLayout(grid);
	}

	connect(object_selection_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedRoleData()));

	setMinimumSize(580, 550);
}

RoleWidget::~RoleWidget()
{
	delete object_selection_wgt;
}

void RoleWidget::configureRoleSelection()
{
	unsigned i;

	//Disconnects all signals from the member role tables
	for(i=0; i < 3; i++)
		disconnect(members_tab[i], nullptr,this, nullptr);

	//Connects the signal/slots only on the current table
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_rowAdded(int)), this, SLOT(selectMemberRole()));
	connect(members_tab[members_twg->currentIndex()], SIGNAL(s_rowEdited(int)), this, SLOT(selectMemberRole()));
}

void RoleWidget::selectMemberRole()
{
	object_selection_wgt->setObjectVisible(ObjectType::Role, true);
	object_selection_wgt->setModel(this->model);
	object_selection_wgt->show();
}

void RoleWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Role *role)
{
	if(role)
	{
		conn_limit_sb->setValue(role->getConnectionLimit());
		passwd_edt->setText(role->getPassword());

		validity_chk->setChecked(!role->getValidity().isEmpty());
		validity_dte->setDateTime(QDateTime::fromString(role->getValidity(), QString("yyyy-MM-dd hh:mm:ss")));

		superusr_chk->setChecked(role->getOption(Role::OpSuperuser));
		create_db_chk->setChecked(role->getOption(Role::OpCreateDb));
		create_role_chk->setChecked(role->getOption(Role::OpCreateRole));
		encrypt_pass_chk->setChecked(role->getOption(Role::OpEncrypted));
		inh_perm_chk->setChecked(role->getOption(Role::OpInherit));
		can_login_chk->setChecked(role->getOption(Role::OpLogin));
		can_replicate_chk->setChecked(role->getOption(Role::OpReplication));
		bypass_rls_chk->setChecked(role->getOption(Role::OpBypassRls));
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
				role_types[3]={ Role::RefRole, Role::MemberRole, Role::AdminRole };

		if(table_id > 3)
			throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

void RoleWidget::fillMembersTable()
{
	if(this->object)
	{
		Role *aux_role=nullptr, *role=nullptr;
		unsigned count, i, type_id,
				role_types[3]={ Role::RefRole, Role::MemberRole, Role::AdminRole };

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

void RoleWidget::showSelectedRoleData()
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

		msg_box.show(Exception(Exception::getErrorMessage(ErrorCode::AsgRoleReferenceRedundancy)
													 .arg(obj_sel->getName())
													 .arg(name_edt->text()),
													 ErrorCode::AsgRoleReferenceRedundancy,__PRETTY_FUNCTION__,__FILE__,__LINE__));
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
			msg_box.show( Exception(Exception::getErrorMessage(ErrorCode::InsDuplicatedRole)
															.arg(obj_sel->getName())
															.arg(name_edt->text()),
															ErrorCode::InsDuplicatedRole,__PRETTY_FUNCTION__,__FILE__,__LINE__));
		}
	}
}

void RoleWidget::applyConfiguration()
{
	Role *role=nullptr, *aux_role=nullptr;
	unsigned count, i, type_id,
			role_types[3]={ Role::RefRole, Role::MemberRole, Role::AdminRole };

	try
	{
		startConfiguration<Role>();

		role=dynamic_cast<Role *>(this->object);
		role->setConnectionLimit(conn_limit_sb->value());
		role->setPassword(passwd_edt->text());

		if(validity_chk->isChecked())
			role->setValidity(validity_dte->dateTime().toString(QString("yyyy-MM-dd hh:mm")));
		else
			role->setValidity(QString());

		role->setOption(Role::OpSuperuser, superusr_chk->isChecked());
		role->setOption(Role::OpCreateDb, create_db_chk->isChecked());
		role->setOption(Role::OpCreateRole, create_role_chk->isChecked());
		role->setOption(Role::OpEncrypted, encrypt_pass_chk->isChecked());
		role->setOption(Role::OpInherit, inh_perm_chk->isChecked());
		role->setOption(Role::OpLogin, can_login_chk->isChecked());
		role->setOption(Role::OpReplication, can_replicate_chk->isChecked());
		role->setOption(Role::OpBypassRls, bypass_rls_chk->isChecked());

		for(type_id=0; type_id < 3; type_id++)
		{
			count = members_tab[type_id]->getRowCount();
			role->removeRoles(role_types[type_id]);

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
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
