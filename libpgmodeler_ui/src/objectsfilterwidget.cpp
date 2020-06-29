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
#include "pgmodelerns.h"

ObjectsFilterWidget::ObjectsFilterWidget(QWidget *parent) : QWidget(parent)
{
	vector<ObjectType> types = BaseObject::getChildObjectTypes(ObjectType::Table);
	QAction *act = nullptr;

	setupUi(this);

	hint_lbl->setText(tr("Using a pattern in <strong>%1</strong> mode in which no wildcard character <strong>%2</strong> \
is present has the same effect as performing an exact match searching on the names or signatures.")
											 .arg(PgModelerNs::FilterWildcard)
											 .arg(PgModelerNs::WildcardChar));

	add_tb->setToolTip(add_tb->toolTip() + QString(" (%1)").arg(add_tb->shortcut().toString()));
	clear_all_tb->setToolTip(clear_all_tb->toolTip() + QString(" (%1)").arg(clear_all_tb->shortcut().toString()));
	apply_tb->setToolTip(apply_tb->toolTip() + QString(" (%1)").arg(apply_tb->shortcut().toString()));

	types.erase(std::find(types.begin(), types.end(), ObjectType::Column));

	for(auto &type : types)
	{
		act = tab_objs_menu.addAction(BaseObject::getTypeName(type));
		act->setIcon(QIcon(PgModelerUiNs::getIconPath(type)));
		act->setData(BaseObject::getSchemaName(type));
		act->setCheckable(true);
		act->setChecked(true);
	}

	action_only_matching = new QAction(tr("Only macthing"));
	action_only_matching->setCheckable(true);
	action_only_matching->setChecked(true);

	action_match_signature = new QAction(tr("Match by signature"));
	action_match_signature->setCheckable(true);
	action_match_signature->setChecked(true);

	options_menu.addAction(action_match_signature);
	options_menu.addAction(action_only_matching);
	action_forced_filter = options_menu.addAction(tr("Forced filtering"));
	action_forced_filter->setMenu(&tab_objs_menu);

	options_tb->setMenu(&options_menu);

	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addFilter()));
	connect(clear_all_tb, SIGNAL(clicked(bool)), this, SLOT(removeAllFilters()));
	connect(action_only_matching, SIGNAL(toggled(bool)), action_forced_filter, SLOT(setEnabled(bool)));

	connect(apply_tb, &QToolButton::clicked, [&](){
		emit s_filterApplyingRequested();
	});

	filters_tbw->horizontalHeader()->resizeSection(0, 130);
	filters_tbw->horizontalHeader()->resizeSection(1, 100);
	filters_tbw->horizontalHeader()->resizeSection(2, 100);
}

void ObjectsFilterWidget::setModelFilteringMode(bool value, const vector<ObjectType> &extra_types)
{
	for(auto &act : tab_objs_menu.actions())
		act->setChecked(true);

	action_forced_filter->setDisabled(value);
	action_only_matching->setChecked(true);
	action_only_matching->setDisabled(value);

	if(!value)
		extra_obj_types.clear();
	else
		extra_obj_types = extra_types;
}

void ObjectsFilterWidget::addFilters(const QStringList &filters)
{
	QStringList values, types;
	QComboBox *combo = nullptr;
	int row = 0;

	filters_tbw->setRowCount(0);
	types = Catalog::getFilterableObjectNames();

	for(auto &ext_type : extra_obj_types)
		types.append(BaseObject::getSchemaName(ext_type));

	for(auto &filter : filters)
	{
		values = filter.split(PgModelerNs::FilterSeparator);

		// Rejecting invalid filters: malformed (< 3 fields), empty values or invalid object types
		if(values.size() != 3 || values.indexOf("") >= 0 || !types.contains(values[0]))
			continue;

		addFilter();
		row = filters_tbw->rowCount() - 1;

		combo = qobject_cast<QComboBox *>(filters_tbw->cellWidget(row, 0));
		combo->setCurrentIndex(types.indexOf(values[0]));

		filters_tbw->item(row, 1)->setText(values[1]);

		combo = qobject_cast<QComboBox *>(filters_tbw->cellWidget(row, 2));
		combo->setCurrentText(values[2]);
	}
}

QComboBox *ObjectsFilterWidget::createObjectsCombo()
{
	QComboBox *combo = new QComboBox;
	QStringList obj_types = Catalog::getFilterableObjectNames();

	for(auto &obj_type : obj_types)
	{
		combo->addItem(QIcon(PgModelerUiNs::getIconPath(obj_type)),
												 BaseObject::getTypeName(obj_type),
												 obj_type);
	}

	for(auto &obj_type : extra_obj_types)
	{
		if(combo->findText(BaseObject::getTypeName(obj_type)) < 0)
		{
			combo->addItem(QIcon(PgModelerUiNs::getIconPath(obj_type)),
													 BaseObject::getTypeName(obj_type),
													 BaseObject::getSchemaName(obj_type));
		}
	}

	combo->setStyleSheet("border: 0px");

	return combo;
}

QStringList ObjectsFilterWidget::getObjectFilters()
{
	QStringList filters,
			curr_filter,
			modes = { PgModelerNs::FilterWildcard, PgModelerNs::FilterRegExp };
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

		filters.append(curr_filter.join(PgModelerNs::FilterSeparator));
		curr_filter.clear();
	}

	return filters;
}

QStringList ObjectsFilterWidget::getForceObjectsFilter()
{
	QStringList types;

	if(action_only_matching->isChecked())
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
	return action_only_matching->isChecked();
}

bool ObjectsFilterWidget::isMatchSignature()
{
	return action_match_signature->isChecked();
}

bool ObjectsFilterWidget::hasFiltersConfigured()
{
	return filters_tbw->rowCount() > 0;
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
	combo->addItems({ tr("Wildcard"), tr("Regexp") });
	filters_tbw->setCellWidget(row, 2, combo);

	rem_tb = new QToolButton;
	rem_tb->setIcon(QIcon(PgModelerUiNs::getIconPath("excluir")));
	rem_tb->setToolTip(tr("Remove filter"));
	rem_tb->setAutoRaise(true);
	connect(rem_tb, SIGNAL(clicked(bool)), this, SLOT(removeFilter()));
	filters_tbw->setCellWidget(row, 3, rem_tb);

	clear_all_tb->setEnabled(true);
	apply_tb->setEnabled(filters_tbw->rowCount() != 0);
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

	if(filters_tbw->rowCount() == 0)
		emit s_filtersRemoved();
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
