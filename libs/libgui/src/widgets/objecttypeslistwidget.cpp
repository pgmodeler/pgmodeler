/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "objecttypeslistwidget.h"
#include "guiutilsns.h"

ObjectTypesListWidget::ObjectTypesListWidget(QWidget *parent)	: QWidget(parent)
{
	setupUi(this);

	QListWidgetItem *item = nullptr;

	for(auto &type : BaseObject::getObjectTypes(true, { ObjectType::BaseRelationship }))
	{
		item = new QListWidgetItem(BaseObject::getTypeName(type));
		item->setIcon(QIcon(GuiUtilsNs::getIconPath(type)));
		item->setData(Qt::UserRole, QVariant::fromValue<ObjectType>(type));
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
		obj_types_lst->addItem(item);
	}

	obj_types_lst->adjustSize();

	connect(check_all_tb, &QToolButton::clicked, this, [this](){
		setItemsCheckState(Qt::Checked);
	});

	connect(uncheck_all_tb, &QToolButton::clicked, this, [this](){
		setItemsCheckState(Qt::Unchecked);
	});

	connect(obj_types_lst, &QListWidget::itemChanged, this, [this](QListWidgetItem *item){
		emit s_typeCheckStateChanged(item->data(Qt::UserRole).value<ObjectType>(), item->checkState());
	});
}

void ObjectTypesListWidget::setItemsCheckState(Qt::CheckState state)
{
	QListWidgetItem *item = nullptr;

	for(int idx = 0; idx < obj_types_lst->count(); idx++)
	{
		item = obj_types_lst->item(idx);
		item->setCheckState(state);
	}

	emit s_typesCheckStateChanged(state);
}

void ObjectTypesListWidget::setTypeNamesCheckState(const QStringList &obj_types, Qt::CheckState state)
{
	std::vector<ObjectType> types;

	for(auto &obj_type : obj_types)
		types.push_back(BaseObject::getObjectType(obj_type));

	setTypesCheckState(types, state);
}

void ObjectTypesListWidget::setTypesCheckState(const std::vector<ObjectType> &obj_types, Qt::CheckState state)
{
	ObjectType obj_type;
	QListWidgetItem *item = nullptr;

	for(int idx = 0; idx < obj_types_lst->count(); idx++)
	{
		item = obj_types_lst->item(idx);
		obj_type = item->data(Qt::UserRole).value<ObjectType>();

		if(std::find(obj_types.cbegin(), obj_types.cend(), obj_type) != obj_types.cend())
			item->setCheckState(state);
	}
}

std::vector<ObjectType> ObjectTypesListWidget::getTypesPerCheckState(Qt::CheckState state)
{
	std::vector<ObjectType> types;
	QListWidgetItem *item = nullptr;

	for(int idx = 0; idx < obj_types_lst->count(); idx++)
	{
		item = obj_types_lst->item(idx);

		if(item->checkState() == state)
			types.push_back(item->data(Qt::UserRole).value<ObjectType>());
	}

	return types;
}

QStringList ObjectTypesListWidget::getTypeNamesPerCheckState(Qt::CheckState state)
{
	QStringList types;
	QListWidgetItem *item = nullptr;

	for(int idx = 0; idx < obj_types_lst->count(); idx++)
	{
		item = obj_types_lst->item(idx);

		if(item->checkState() == state)
			types.append(BaseObject::getSchemaName(item->data(Qt::UserRole).value<ObjectType>()));
	}

	return types;
}
