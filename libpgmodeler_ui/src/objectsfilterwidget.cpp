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
	vector<ObjectType> types = BaseObject::getChildObjectTypes(ObjectType::Table);
	QAction *act = nullptr;

	setupUi(this);
	types.erase(std::find(types.begin(), types.end(), ObjectType::Column));

	for(auto &type : types)
	{
		act = tab_objs_menu.addAction(BaseObject::getTypeName(type));
		act->setIcon(QIcon(PgModelerUiNs::getIconPath(type)));
		act->setData(BaseObject::getSchemaName(type));
		act->setCheckable(true);
		act->setChecked(false);
	}

	forced_filter_tb->setMenu(&tab_objs_menu);

	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addFilter()));
	connect(clear_all_tb, SIGNAL(clicked(bool)), this, SLOT(removeAllFilters()));
	connect(only_matching_chk, SIGNAL(toggled(bool)), this, SLOT(enableForcedFilterButton()));

	connect(&tab_objs_menu, &QMenu::triggered, [&](){
		int checked_acts = 0;
		for(auto &act : tab_objs_menu.actions())
		{
			if(act->isChecked())
				checked_acts++;
		}

		forced_filter_tb->setText(tr("Forced filter") + QString(" (%1)").arg(checked_acts));
	});

	connect(apply_tb, &QToolButton::clicked, [&](){
		emit s_filterApplyingRequested();
	});

	filters_tbw->horizontalHeader()->resizeSection(0, 130);
	filters_tbw->horizontalHeader()->resizeSection(1, 100);
	filters_tbw->horizontalHeader()->resizeSection(2, 100);
}

QComboBox *ObjectsFilterWidget::createObjectsCombo()
{
	QComboBox *combo = new QComboBox;
	QStringList obj_types = Catalog::getFilterableObjectNames();

	for(auto &obj_type : obj_types)
		combo->addItem(QIcon(PgModelerUiNs::getIconPath(obj_type)),
												 BaseObject::getTypeName(obj_type),
												 obj_type);

	combo->setStyleSheet("border: 0px");
	combo->model()->sort(0);
	connect(combo, SIGNAL(activated(int)), this, SLOT(enableForcedFilterButton()));

	return combo;
}

QStringList ObjectsFilterWidget::getObjectFilters()
{
	QStringList filters,
			curr_filter,
			modes = { Catalog::FilterExact, Catalog::FilterLike, Catalog::FilterRegExp };
	QString pattern, mode, type_name;
	QComboBox *mode_cmb = nullptr, *object_cmb = nullptr;

	/* Workround: Forcing any uncommitted data on the filters_tbw to be commited
	 * by changing the current model index. This seems force the calling of commitData()
	 * on QTableWidget. This was needed because if the user activates other widgets somewhere
	 * and there's still an item in edition (in this case the pattern) that text being edit
	 * is never commited causing an outdated pattern to be provided even if the text of the item
	 *	shows the updated text */
	filters_tbw->setCurrentIndex(QModelIndex());

	for(int row = 0; row < filters_tbw->rowCount(); row++)
	{
		object_cmb = qobject_cast<QComboBox *>(filters_tbw->cellWidget(row, 0));
		mode_cmb = qobject_cast<QComboBox *>(filters_tbw->cellWidget(row, 2));
		type_name = object_cmb->currentData().toString();

		curr_filter.append(type_name);
		curr_filter.append(filters_tbw->item(row, 1)->text());
		curr_filter.append(modes[mode_cmb->currentIndex()]);

		filters.append(curr_filter.join(Catalog::FilterSeparator));
		curr_filter.clear();
	}

	return filters;
}

QStringList ObjectsFilterWidget::getForceObjectsFilter()
{
	QStringList types;

	if(forced_filter_tb->isEnabled())
	{
		for(auto &act : tab_objs_menu.actions())
		{
			if(act->isChecked())
				types.append(act->data().toString());
		}
	}

	return types;
}

bool ObjectsFilterWidget::isOnlyMatching()
{
	return only_matching_chk->isChecked();
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
	combo->addItems({ tr("Exact"), tr("Like"), tr("Regexp") });
	filters_tbw->setCellWidget(row, 2, combo);

	rem_tb = new QToolButton;
	rem_tb->setIcon(QIcon(PgModelerUiNs::getIconPath("excluir")));
	rem_tb->setToolTip(tr("Remove filter"));
	rem_tb->setAutoRaise(true);
	connect(rem_tb, SIGNAL(clicked(bool)), this, SLOT(removeFilter()));
	filters_tbw->setCellWidget(row, 3, rem_tb);

	clear_all_tb->setEnabled(true);
	apply_tb->setEnabled(filters_tbw->rowCount() != 0);
	enableForcedFilterButton();
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
	apply_tb->setEnabled(filters_tbw->rowCount() != 0);
	enableForcedFilterButton();
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

void ObjectsFilterWidget::enableForcedFilterButton()
{
	bool has_tab_filter = false;
	int row_cnt = filters_tbw->rowCount();
	QString type_name;

	for(int row = 0; row < row_cnt && !has_tab_filter; row++)
	{
		type_name = qobject_cast<QComboBox *>(filters_tbw->cellWidget(row, 0))->currentData(Qt::UserRole).toString();
		has_tab_filter = BaseTable::isBaseTable(BaseObject::getObjectType(type_name));
	}

	forced_filter_tb->setEnabled(only_matching_chk->isChecked() && has_tab_filter);
}
