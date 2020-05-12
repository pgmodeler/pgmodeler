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

#include "objectsfilterwidget.h"
#include "pgmodeleruins.h"
#include <QComboBox>

ObjectsFilterWidget::ObjectsFilterWidget(QWidget *parent) : QWidget(parent)
{
	vector<ObjectType> obj_types = BaseObject::getObjectTypes(true, { ObjectType::Relationship,
																																		ObjectType::BaseRelationship,
																																		ObjectType::Textbox,
																																		ObjectType::GenericSql,
																																		ObjectType::Permission,
																																		ObjectType::Database,
																																		ObjectType::Tag});
	setupUi(this);

	for(auto &obj_type : obj_types)
		addObjectsComboItem(obj_type, false);

	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addFilter()));
	//connect(objects_tbw, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(removeFilter()));
	objects_tbw->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void ObjectsFilterWidget::addObjectsComboItem(ObjectType obj_type, bool sort)
{
	objects_cmb->addItem(QIcon(PgModelerUiNs::getIconPath(obj_type)),
											 BaseObject::getTypeName(obj_type),
											 QVariant(enum_cast(obj_type)));

	if(sort)
		objects_cmb->model()->sort(0);
}

void ObjectsFilterWidget::addFilter()
{
	if(objects_cmb->count() == 0)
		return;

	int row = objects_tbw->rowCount();
	ObjectType obj_type = static_cast<ObjectType>(objects_cmb->currentData().toUInt());
	QString obj_name = objects_cmb->currentText();
	QTableWidgetItem *item = nullptr;
	QComboBox *combo = nullptr;
	QToolButton *rem_tb = nullptr;

	objects_cmb->removeItem(objects_cmb->currentIndex());
	objects_tbw->insertRow(row);

	item = new QTableWidgetItem;
	item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	objects_tbw->setItem(row, 0, item);

	item = new QTableWidgetItem;
	item->setFlags(Qt::ItemIsEnabled);
	item->setText(obj_name);
	item->setIcon(QIcon(PgModelerUiNs::getIconPath(obj_type)));
	item->setData(Qt::UserRole, enum_cast(obj_type));
	objects_tbw->setItem(row, 1, item);

	combo = new QComboBox;
	combo->setStyleSheet("border: 0px");
	combo->addItems({ tr("Wildcard"), tr("Exact match"), tr("Regular expression")});
	objects_tbw->setCellWidget(row, 2, combo);

	rem_tb = new QToolButton;
	rem_tb->setIcon(QIcon(PgModelerUiNs::getIconPath("excluir")));
	rem_tb->setToolTip(tr("Remove filter"));
	rem_tb->setAutoRaise(true);
	connect(rem_tb, SIGNAL(clicked(bool)), this, SLOT(removeFilter()));
	objects_tbw->setCellWidget(row, 3, rem_tb);

	add_tb->setEnabled(objects_cmb->count() != 0);
}

void ObjectsFilterWidget::removeFilter()
{
	QToolButton *btn = qobject_cast<QToolButton *>(sender());
	int curr_row = objects_tbw->currentRow();
	ObjectType obj_type;

	if(!btn && curr_row < 0)
		return;

	if(btn && curr_row < 0)
	{
		for(int row = 0; row < objects_tbw->rowCount(); row++)
		{
			if(objects_tbw->cellWidget(row, 3) == btn)
			{
				curr_row = row;
				break;
			}
		}
	}

	obj_type = static_cast<ObjectType>(objects_tbw->item(curr_row, 1)->data(Qt::UserRole).toUInt());
	addObjectsComboItem(obj_type, true);
	objects_tbw->removeRow(curr_row);
	add_tb->setEnabled(true);
}
