/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "tabledatawidget.h"
#include "htmlitemdelegate.h"

TableDataWidget::TableDataWidget(QWidget *parent): BaseObjectWidget(parent, BASE_OBJECT)
{
	Ui_TableDataWidget::setupUi(this);
	configureFormLayout(tabledata_grid, BASE_OBJECT);

	obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
									BaseObject::getSchemaName(OBJ_TABLE) + QString(".png")));

	comment_lbl->setVisible(false);
	comment_edt->setVisible(false);

	QFont font=name_edt->font();
	font.setItalic(true);
	name_edt->setReadOnly(true);
	name_edt->setFont(font);

	add_tb->setToolTip(add_tb->toolTip() + QString(" (%1)").arg(add_tb->shortcut().toString()));
	delete_tb->setToolTip(delete_tb->toolTip() + QString(" (%1)").arg(delete_tb->shortcut().toString()));
	duplicate_tb->setToolTip(duplicate_tb->toolTip() + QString(" (%1)").arg(duplicate_tb->shortcut().toString()));
	clear_tb->setToolTip(clear_tb->toolTip() + QString(" (%1)").arg(clear_tb->shortcut().toString()));

	setMinimumSize(640, 480);

	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addRow()));
	connect(duplicate_tb, SIGNAL(clicked(bool)), this, SLOT(duplicateRows()));
	connect(delete_tb, SIGNAL(clicked(bool)), this, SLOT(deleteRows()));
	connect(clear_tb, SIGNAL(clicked(bool)), this, SLOT(clearRows()));
	connect(data_tbw, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(insertRowOnTabPress(int,int,int,int)), Qt::QueuedConnection);

	connect(data_tbw, &QTableWidget::itemSelectionChanged,
	[=](){
		QList<QTableWidgetSelectionRange> sel_ranges=data_tbw->selectedRanges();
		delete_tb->setEnabled(!sel_ranges.isEmpty());
		duplicate_tb->setEnabled(!sel_ranges.isEmpty());
	});
}

void TableDataWidget::insertRowOnTabPress(int curr_row, int curr_col, int prev_row, int prev_col)
{
	if(qApp->mouseButtons()==Qt::NoButton &&
			curr_row==0 && curr_col==0 &&
			prev_row==data_tbw->rowCount()-1 && prev_col==data_tbw->columnCount()-1)
	{
		addRow();
	}
}

void TableDataWidget::duplicateRows(void)
{
	QList<QTableWidgetSelectionRange> sel_ranges=data_tbw->selectedRanges();

	if(!sel_ranges.isEmpty())
	{
		for(auto &sel_rng : sel_ranges)
		{
			for(int row=sel_rng.topRow(); row <= sel_rng.bottomRow(); row++)
			{
				addRow();

				for(int col=0; col < data_tbw->columnCount(); col++)
				{
					data_tbw->item(data_tbw->rowCount() - 1, col)
							->setText(data_tbw->item(row, col)->text());
				}
			}
		}
	}
}

void TableDataWidget::deleteRows(void)
{
	QTableWidgetSelectionRange sel_range;

	while(!data_tbw->selectedRanges().isEmpty())
	{
		sel_range=data_tbw->selectedRanges().at(0);

		for(int i = 0; i < sel_range.rowCount(); i++)
			data_tbw->removeRow(sel_range.topRow());
	}
}

void TableDataWidget::clearRows(void)
{
	Messagebox msg_box;

	msg_box.show(trUtf8("Clear the grid is an irreversible action! Do you really want to proceed?"),
							 Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		while(data_tbw->rowCount() > 0)
			data_tbw->removeRow(0);

		clear_tb->setEnabled(false);
	}
}

void TableDataWidget::setAttributes(DatabaseModel *model, Table *table)
{
	BaseObjectWidget::setAttributes(model, table, nullptr);
	bool enable=(object != nullptr);

	protected_obj_frm->setVisible(false);
	obj_id_lbl->setVisible(false);
	data_tbw->setEnabled(enable);
	add_tb->setEnabled(enable);

	if(object)
		populateDataGrid();
}

void TableDataWidget::populateDataGrid(void)
{
	Table *table=dynamic_cast<Table *>(this->object);
	QTableWidgetItem *item=nullptr;
	QString ini_data;
	int col=0;
	QSet<QString, int> col_indexes;

	while(data_tbw->rowCount() > 0)
		data_tbw->removeRow(0);

	data_tbw->setColumnCount(table->getColumnCount());

	for(auto object : *table->getObjectList(OBJ_COLUMN))
	{
		item=new QTableWidgetItem(object->getName());
		col_indexes[object->getName()]=col;
		data_tbw->setHorizontalHeaderItem(col++, item);
	}

	ini_data=table->getInitialData();

	if(!ini_data.isEmpty())
	{
		QStringList buffer=ini_data.split(Table::DATA_LINE_BREAK);
	}

	data_tbw->resizeRowsToContents();
}

QString TableDataWidget::generateDataBuffer(void)
{
	QStringList values;
	QStringList buffer;
	int col = 0, col_count = data_tbw->horizontalHeader()->count();

	for(int col=0; col < col_count; col++)
		values.push_back(data_tbw->horizontalHeaderItem(col)->text());

	buffer.push_back(values.join(Table::DATA_SEPARATOR));

	for(int row = 0; row < data_tbw->rowCount(); row++)
	{
		values.clear();

		for(col = 0; col < col_count; col++)
			values.push_back(data_tbw->item(row, col)->text());

		buffer.push_back(values.join(Table::DATA_SEPARATOR));
	}

	return(buffer.join(Table::DATA_LINE_BREAK));
}

void TableDataWidget::addRow(void)
{
	int row=data_tbw->rowCount();
	QTableWidgetItem *item = nullptr;

	data_tbw->blockSignals(true);
	data_tbw->insertRow(row);

	for(int col=0; col < data_tbw->columnCount(); col++)
	{
		item=new QTableWidgetItem;
		item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		data_tbw->setItem(row, col, item);
	}

	data_tbw->clearSelection();
	data_tbw->setCurrentCell(row, 0, QItemSelectionModel::ClearAndSelect);
	data_tbw->editItem(data_tbw->item(row, 0));
	data_tbw->blockSignals(false);

	clear_tb->setEnabled(true);
}

void TableDataWidget::applyConfiguration(void)
{
	try
	{
		Table *table = dynamic_cast<Table *>(this->object);
		table->setInitialData(generateDataBuffer());
		emit s_closeRequested();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

