/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2021 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "columnpickerwidget.h"

ColumnPickerWidget::ColumnPickerWidget(QWidget *parent) :	QWidget(parent)
{
	setupUi(this);

	parent_obj = nullptr;

	columns_tab=new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^
																		 (ObjectsTableWidget::EditButton |
																			ObjectsTableWidget::UpdateButton |
																			ObjectsTableWidget::DuplicateButton), true, this);

	columns_tab->setColumnCount(2);
	columns_tab->setHeaderLabel(tr("Column"), 0);
	columns_tab->setHeaderIcon(QPixmap(GuiUtilsNs::getIconPath("column")),0);
	columns_tab->setHeaderLabel(tr("Type"), 1);
	columns_tab->setHeaderIcon(QPixmap(GuiUtilsNs::getIconPath("usertype")),1);

	col_picker_grid->addWidget(columns_tab, 1, 0, 1, 3);

	connect(columns_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addColumn(int)));
	connect(columns_tab, &ObjectsTableWidget::s_rowRemoved, [&](int){ 	updateColumnsCombo(); });
	connect(columns_tab, &ObjectsTableWidget::s_rowsRemoved, [&](){ 	updateColumnsCombo(); });
}

void ColumnPickerWidget::setParentObject(BaseObject *p_obj)
{
	if(p_obj &&
		 !PhysicalTable::isPhysicalTable(p_obj->getObjectType()) &&
		 p_obj->getObjectType() != ObjectType::Relationship)
	{
		throw Exception(ErrorCode::AsgObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	parent_obj = p_obj;
	setEnabled(p_obj != nullptr);

	columns_tab->blockSignals(true);
	columns_tab->removeRows();
	columns_tab->blockSignals(false);

	updateColumnsCombo();
}

void ColumnPickerWidget::setColumns(const vector<Column *> &cols)
{
	int row = 0;

	columns_tab->blockSignals(true);
	columns_tab->removeRows();

	for(auto &col : cols)
	{
		if(parent_obj &&
				(col->getParentRelationship() == parent_obj ||
				 col->getParentTable() == parent_obj))
		{
			columns_tab->addRow();
			addColumn(col, row++);
			columns_cmb->removeItem(columns_cmb->findData(QVariant::fromValue<void *>(col)));
		}
	}

	columns_tab->clearSelection();
	columns_tab->blockSignals(false);
}

void ColumnPickerWidget::updateColumnsCombo()
{
	try
	{
		Column *column=nullptr;
		vector<TableObject *> columns;
		PhysicalTable *table = dynamic_cast<PhysicalTable *>(parent_obj);
		Relationship *relationship = dynamic_cast<Relationship *>(parent_obj);

		if(table)
			columns = *table->getObjectList(ObjectType::Column);
		else if(relationship)
			columns = relationship->getAttributes();

		columns_cmb->clear();

		for(auto &col : columns)
		{
			column = dynamic_cast<Column *>(col);

			//If the column does not exists on the column's table, adds it
			if(columns_tab->getRowIndex(QVariant::fromValue<void *>(column)) < 0)
			{
				columns_cmb->addItem(QString("%1 (%2)").arg(column->getName(), ~column->getType()),
														 QVariant::fromValue<void *>(column));
			}
		}

		columns_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, columns_cmb->count() != 0);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ColumnPickerWidget::addColumn(int row)
{
	Column *column=nullptr;

	try
	{
		//Gets the reference to the selected column
		column = reinterpret_cast<Column *>(columns_cmb->itemData(columns_cmb->currentIndex(), Qt::UserRole).value<void *>());

		//When the column is selected it will be removed from combo
		columns_cmb->removeItem(columns_cmb->currentIndex());

		//Adds the column into table
		addColumn(column, row);

		//When there is no items con the combo the insert button of the table is disabled
		columns_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, (columns_cmb->count()!=0));
	}
	catch(Exception &e)
	{
		columns_tab->removeRow(row);
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ColumnPickerWidget::addColumn(Column *column, int row)
{
	if(!column || row < 0)
		return;

	columns_tab->setCellText(column->getName(), row, 0);
	columns_tab->setCellText(~column->getType(), row, 1);
	columns_tab->setRowData(QVariant::fromValue<void *>(column), row);

	//Change the table row background color if the column is protected or added by relationship
	if(column->isAddedByRelationship() || column->isProtected())
	{
		QFont fonte;
		fonte=columns_tab->font();
		fonte.setItalic(true);

		if(column->isProtected())
			columns_tab->setRowFont(row, fonte, GuiUtilsNs::ProtRowFgColor, GuiUtilsNs::ProtRowBgColor);
		else
			columns_tab->setRowFont(row, fonte, GuiUtilsNs::RelAddedRowFgColor, GuiUtilsNs::RelAddedRowBgColor);
	}
}

vector<Column *> ColumnPickerWidget::getColumns()
{
	vector<Column *> cols;

	for(unsigned row = 0; row < columns_tab->getRowCount(); row++)
		cols.push_back(reinterpret_cast<Column *>(columns_tab->getRowData(row).value<void *>()));

	return cols;
}
