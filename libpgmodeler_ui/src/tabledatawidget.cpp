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

	add_row_tb->setToolTip(add_row_tb->toolTip() + QString(" (%1)").arg(add_row_tb->shortcut().toString()));
	del_rows_tb->setToolTip(del_rows_tb->toolTip() + QString(" (%1)").arg(del_rows_tb->shortcut().toString()));
	dup_rows_tb->setToolTip(dup_rows_tb->toolTip() + QString(" (%1)").arg(dup_rows_tb->shortcut().toString()));
	clear_tb->setToolTip(clear_tb->toolTip() + QString(" (%1)").arg(clear_tb->shortcut().toString()));

	setMinimumSize(640, 480);

	connect(add_row_tb, SIGNAL(clicked(bool)), this, SLOT(addRow()));
	connect(dup_rows_tb, SIGNAL(clicked(bool)), this, SLOT(duplicateRows()));
	connect(del_rows_tb, SIGNAL(clicked(bool)), this, SLOT(deleteRows()));
	connect(del_cols_tb, SIGNAL(clicked(bool)), this, SLOT(deleteColumns()));
	connect(clear_tb, SIGNAL(clicked(bool)), this, SLOT(clearRows()));
	connect(data_tbw, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(insertRowOnTabPress(int,int,int,int)), Qt::QueuedConnection);

	connect(data_tbw, &QTableWidget::itemSelectionChanged,
	[=](){
		del_rows_tb->setEnabled(false);
		dup_rows_tb->setEnabled(false);
		del_cols_tb->setEnabled(false);
	});

	connect(data_tbw->verticalHeader(), &QHeaderView::sectionPressed,
		[=](){
		del_rows_tb->setEnabled(true);
		dup_rows_tb->setEnabled(true);
	});

	connect(data_tbw->verticalHeader(), &QHeaderView::sectionEntered,
		[=](){
		del_rows_tb->setEnabled(true);
		dup_rows_tb->setEnabled(true);
	});

	connect(data_tbw->horizontalHeader(), &QHeaderView::sectionPressed,
		[=](){
		del_cols_tb->setEnabled(true);
	});

	connect(data_tbw->horizontalHeader(), &QHeaderView::sectionEntered,
		[=](){
		del_cols_tb->setEnabled(true);
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

void TableDataWidget::deleteColumns(void)
{
	Messagebox msg_box;

	msg_box.show(trUtf8("Delete columns is an irreversible action! Do you really want to proceed?"),
							 Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{

		del_cols_tb->setEnabled(false);
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

void TableDataWidget::fixInvalidColumn(int col_idx)
{
	QTableWidgetItem *item=data_tbw->horizontalHeaderItem(col_idx);

	if(item && item->flags()==Qt::NoItemFlags)
	{
		QMenu menu, submenu;

		submenu.addAction("Column1");
		submenu.addAction("Column2");
		submenu.addAction("Column3");
		menu.addAction("Set column")->setMenu(&submenu);
		menu.addSeparator();
		menu.addAction("Drop column");
		menu.exec(QCursor::pos());
	}
}

void TableDataWidget::setAttributes(DatabaseModel *model, Table *table)
{
	BaseObjectWidget::setAttributes(model, table, nullptr);
	bool enable=(object != nullptr);

	protected_obj_frm->setVisible(false);
	obj_id_lbl->setVisible(false);
	data_tbw->setEnabled(enable);
	add_row_tb->setEnabled(enable);

	if(object)
		populateDataGrid();
}

void TableDataWidget::populateDataGrid(void)
{
	Table *table=dynamic_cast<Table *>(this->object);
	QTableWidgetItem *item=nullptr;
	QString ini_data;
	int col=0, row=0;
	QStringList columns, buffer_cols, buffer, values;
	QVector<int> disabled_cols;

	while(data_tbw->rowCount() > 0)
		data_tbw->removeRow(0);

	ini_data=table->getInitialData();

	if(!ini_data.isEmpty())
	{
		buffer=ini_data.split(Table::DATA_LINE_BREAK);

		if(!buffer.isEmpty())
		{
			buffer_cols=buffer[0].split(Table::DATA_SEPARATOR);
			columns.append(buffer_cols);
		}
	}
	else
	{
		for(auto object : *table->getObjectList(OBJ_COLUMN))
			columns.push_back(object->getName());
	}

	columns.removeDuplicates();
	data_tbw->setColumnCount(columns.size());

	for(QString col_name : columns)
	{
		item=new QTableWidgetItem(col_name);

		if(table->getObjectIndex(col_name, OBJ_COLUMN) < 0)
			disabled_cols.push_back(col);

		data_tbw->setHorizontalHeaderItem(col++, item);
	}

	buffer.removeAt(0);
	row=0;

	for(QString buf_row : buffer)
	{
		addRow();
		values = buf_row.split(Table::DATA_SEPARATOR);
		col = 0;

		for(QString val : values)
			data_tbw->item(row, columns.indexOf(buffer_cols[col++]))->setText(val);

		row++;
	}

	if(!disabled_cols.isEmpty())
	{
		for(int dis_col : disabled_cols)
		{
			for(row = 0; row < data_tbw->rowCount(); row++)
				setItemInvalid(data_tbw->item(row, dis_col));

			item=data_tbw->horizontalHeaderItem(dis_col);
			setItemInvalid(item, Qt::NoItemFlags);
		}

		warn_frm->setVisible(!disabled_cols.isEmpty());
		connect(data_tbw->horizontalHeader(), SIGNAL(sectionPressed(int)), this, SLOT(fixInvalidColumn(int)));
	}

	data_tbw->resizeRowsToContents();
}

void TableDataWidget::setItemInvalid(QTableWidgetItem *item, Qt::ItemFlags flags)
{
	if(item)
	{
		item->setData(Qt::UserRole, item->backgroundColor().name());
		item->setBackground(QColor(QString("#FFC0C0")));
		item->setFlags(flags);
	}
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

		if(data_tbw->horizontalHeaderItem(col)->flags()==Qt::NoItemFlags)
			setItemInvalid(item);
		else
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

