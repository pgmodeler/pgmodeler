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
#include "catalog.h"

ObjectsFilterWidget::ObjectsFilterWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addFilter()));
	connect(clear_all_tb, SIGNAL(clicked(bool)), this, SLOT(removeAllFilters()));

	filters_tbw->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	filters_tbw->installEventFilter(this);
}

QComboBox *ObjectsFilterWidget::createObjectsCombo()
{
	QComboBox *combo = new QComboBox;
	vector<ObjectType> obj_types = BaseObject::getObjectTypes(true, { ObjectType::Relationship,
																																		ObjectType::BaseRelationship,
																																		ObjectType::Textbox,
																																		ObjectType::GenericSql,
																																		ObjectType::Permission,
																																		ObjectType::Database,
																																		ObjectType::Cast,
																																		ObjectType::UserMapping,
																																		ObjectType::Tag});

	for(auto &obj_type : obj_types)
		combo->addItem(QIcon(PgModelerUiNs::getIconPath(obj_type)),
												 BaseObject::getTypeName(obj_type),
												 QVariant(enum_cast(obj_type)));

	combo->setStyleSheet("border: 0px");
	combo->model()->sort(0);

	return combo;
}

QStringList ObjectsFilterWidget::getFilterString()
{
	QStringList filters,
			curr_filter,
			modes = { Catalog::FilterExact, Catalog::FilterRegExp };
	ObjectType obj_type;
	QString pattern, mode;
	QComboBox *mode_cmb = nullptr, *object_cmb = nullptr;

	for(int row = 0; row < filters_tbw->rowCount(); row++)
	{
		object_cmb = qobject_cast<QComboBox *>(filters_tbw->cellWidget(row, 0));
		mode_cmb = qobject_cast<QComboBox *>(filters_tbw->cellWidget(row, 2));

		obj_type = static_cast<ObjectType>(object_cmb->currentData().toUInt());
		curr_filter.append(BaseObject::getSchemaName(obj_type));
		curr_filter.append(filters_tbw->item(row, 1)->text());
		curr_filter.append(modes[mode_cmb->currentIndex()]);

		filters.append(curr_filter.join(Catalog::FilterSeparator));
		curr_filter.clear();
	}

	return filters;
}

bool ObjectsFilterWidget::eventFilter(QObject *object, QEvent *event)
{
	// Removing a filter when the user hit delete over an item in the grid
	if(event->type() == QEvent::KeyPress &&
		 dynamic_cast<QKeyEvent *>(event)->key() == Qt::Key_Delete &&
		 object == filters_tbw && filters_tbw->currentItem() &&
		 !filters_tbw->isPersistentEditorOpen(filters_tbw->currentItem()))
	{
		removeFilter();
		return false;
	}

	return QWidget::eventFilter(object, event);
}

void ObjectsFilterWidget::addFilter()
{
	int row = filters_tbw->rowCount();
	QTableWidgetItem *item = nullptr;
	QComboBox *combo = nullptr;
	QToolButton *rem_tb = nullptr;

	filters_tbw->insertRow(row);
	filters_tbw->setCellWidget(row, 0, createObjectsCombo());

	item = new QTableWidgetItem;
	item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	filters_tbw->setItem(row, 1, item);

	combo = new QComboBox;
	combo->setStyleSheet("border: 0px");
	combo->addItems({ tr("Exact match"), tr("Regular expression") });
	filters_tbw->setCellWidget(row, 2, combo);

	rem_tb = new QToolButton;
	rem_tb->setIcon(QIcon(PgModelerUiNs::getIconPath("excluir")));
	rem_tb->setToolTip(tr("Remove filter"));
	rem_tb->setAutoRaise(true);
	connect(rem_tb, SIGNAL(clicked(bool)), this, SLOT(removeFilter()));
	filters_tbw->setCellWidget(row, 3, rem_tb);

	clear_all_tb->setEnabled(true);
}

void ObjectsFilterWidget::removeFilter()
{
	QToolButton *btn = qobject_cast<QToolButton *>(sender());
	int curr_row = filters_tbw->currentRow();

	if(!btn && curr_row < 0)
		return;

	if(btn)
	{
		for(int row = 0; row < filters_tbw->rowCount(); row++)
		{
			if(filters_tbw->cellWidget(row, 3) == btn)
			{
				curr_row = row;
				break;
			}
		}
	}

	filters_tbw->removeRow(curr_row);
	filters_tbw->clearSelection();
	clear_all_tb->setEnabled(filters_tbw->rowCount() != 0);
}

void ObjectsFilterWidget::removeAllFilters()
{
	while(filters_tbw->rowCount() != 0)
	{
		filters_tbw->setCurrentCell(0, 0);
		removeFilter();
	}

	clear_all_tb->setEnabled(false);
}
