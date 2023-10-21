/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "simplecolumnswidget.h"
#include "baseobjectwidget.h"
#include "guiutilsns.h"

SimpleColumnsWidget::SimpleColumnsWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	model = nullptr;

	pgsqltype_wgt = new PgSQLTypeWidget(this);

	columns_tab = new ObjectsTableWidget(ObjectsTableWidget::AllButtons, true, this);
	columns_tab->setColumnCount(3);
	columns_tab->setHeaderLabel(tr("Name"), 0);
	columns_tab->setHeaderIcon(QPixmap(GuiUtilsNs::getIconPath("uid")),0);
	columns_tab->setHeaderLabel(tr("Type"), 1);
	columns_tab->setHeaderIcon(QPixmap(GuiUtilsNs::getIconPath("usertype")),1);
	columns_tab->setHeaderLabel(tr("Alias"), 2);

	QFrame *info_frm=BaseObjectWidget::generateInformationFrame(tr("This tab can be used to inform additional columns that the view owns and were not generated from the object references. This is just a convenience to make the visualization of this kind of object more intuitive."));

	simple_cols_grid->addWidget(pgsqltype_wgt, 2, 0);
	simple_cols_grid->addWidget(columns_tab, 3, 0);

	info_frm->setParent(this);
	simple_cols_grid->addWidget(info_frm, 4, 0, 1, 2);

	setMinimumSize(640, 480);

	connect(columns_tab, &ObjectsTableWidget::s_rowAdded, this, &SimpleColumnsWidget::addColumn);
	connect(columns_tab, &ObjectsTableWidget::s_rowUpdated, this, &SimpleColumnsWidget::updateColumn);
	connect(columns_tab, &ObjectsTableWidget::s_rowEdited, this, &SimpleColumnsWidget::editColumn);
	connect(columns_tab, &ObjectsTableWidget::s_rowDuplicated, this, &SimpleColumnsWidget::duplicateColumn);
}

void SimpleColumnsWidget::setAttributes(DatabaseModel *model, const std::vector<SimpleColumn> &cols)
{
	this->model = model;
	pgsqltype_wgt->setAttributes(PgSqlType(), model, true,
															 UserTypeConfig::AllUserTypes ^ UserTypeConfig::SequenceType, true, false);

	unsigned row = 0;
	columns_tab->blockSignals(true);

	for(auto &col : cols)
	{
		columns_tab->addRow();
		columns_tab->setCellText(col.getName(), row, 0);
		columns_tab->setCellText(col.getType(), row, 1);
		columns_tab->setCellText(col.getAlias(), row, 2);
		columns_tab->setRowData(QVariant::fromValue<PgSqlType>(PgSqlType::parseString(col.getType())), row);
		row++;
	}

	columns_tab->clearSelection();
	columns_tab->blockSignals(false);
}

void SimpleColumnsWidget::handleColumn(int row)
{
	PgSqlType type = pgsqltype_wgt->getPgSQLType();
	columns_tab->setCellText(name_edt->text(), row, 0);
	columns_tab->setCellText(*type, row, 1);
	columns_tab->setCellText(alias_edt->text(), row, 2);
	columns_tab->setRowData(QVariant::fromValue<PgSqlType>(type), row);
	name_edt->clear();
	alias_edt->clear();
	name_edt->setFocus();
}

void SimpleColumnsWidget::addColumn(int row)
{
	if(!name_edt->text().isEmpty())
		handleColumn(row);
	else
		columns_tab->removeRow(row);
}

void SimpleColumnsWidget::updateColumn(int row)
{
	if(!name_edt->text().isEmpty())
		handleColumn(row);
}

void SimpleColumnsWidget::editColumn(int row)
{
	name_edt->setText(columns_tab->getCellText(row, 0));
	alias_edt->setText(columns_tab->getCellText(row, 2));
	pgsqltype_wgt->setAttributes(columns_tab->getRowData(row).value<PgSqlType>(), model, true,
															 UserTypeConfig::AllUserTypes ^ UserTypeConfig::SequenceType, true, false);
}

void SimpleColumnsWidget::duplicateColumn(int src_row, int new_row)
{
	columns_tab->setRowData(columns_tab->getRowData(src_row), new_row);
}
