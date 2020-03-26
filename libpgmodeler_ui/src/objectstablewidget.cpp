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

#include "objectstablewidget.h"

ObjectsTableWidget::ObjectsTableWidget(unsigned button_conf, bool conf_exclusion, QWidget *parent): QWidget(parent)
{
	setupUi(this);
	connect(move_down_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows()));
	connect(move_up_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows()));
	connect(move_first_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows()));
	connect(move_last_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows()));
	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addRow()));
	connect(remove_tb, SIGNAL(clicked(bool)), this, SLOT(removeRow()));
	connect(edit_tb, SIGNAL(clicked(bool)), this, SLOT(editRow()));
	connect(update_tb, SIGNAL(clicked(bool)), this, SLOT(updateRow()));
	connect(duplicate_tb, SIGNAL(clicked(bool)), this, SLOT(duplicateRow()));
	connect(remove_all_tb, SIGNAL(clicked(bool)), this, SLOT(removeRows()));
	connect(table_tbw, SIGNAL(cellClicked(int,int)), this, SLOT(setButtonsEnabled()));
	connect(table_tbw, SIGNAL(cellActivated(int,int)), this, SLOT(setButtonsEnabled()));
	connect(table_tbw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editRow()));
	connect(table_tbw, SIGNAL(itemSelectionChanged()), this, SLOT(setButtonsEnabled()));
	connect(table_tbw, SIGNAL(itemSelectionChanged()), this, SLOT(emitRowSelected()));

	connect(table_tbw, &QTableWidget::cellClicked, [&](int row, int col){
		emit s_cellClicked(row, col);
	});

	connect(resize_cols_tb, &QToolButton::clicked, [&](){
	  table_tbw->resizeColumnsToContents();
	  table_tbw->resizeRowsToContents();
	  table_tbw->horizontalHeader()->setSectionResizeMode(table_tbw->horizontalHeader()->count() - 1, QHeaderView::Stretch);
	});

	this->conf_exclusion=conf_exclusion;
	cells_editable = false;

	setButtonConfiguration(button_conf);
	setColumnCount(1);

	add_tb->setToolTip(add_tb->toolTip() + QString(" (%1)").arg(add_tb->shortcut().toString()));
	remove_tb->setToolTip(remove_tb->toolTip() + QString(" (%1)").arg(remove_tb->shortcut().toString()));
	remove_all_tb->setToolTip(remove_all_tb->toolTip() + QString(" (%1)").arg(remove_all_tb->shortcut().toString()));
	update_tb->setToolTip(update_tb->toolTip() + QString(" (%1)").arg(update_tb->shortcut().toString()));
	edit_tb->setToolTip(edit_tb->toolTip() + QString(" (%1)").arg(edit_tb->shortcut().toString()));
	duplicate_tb->setToolTip(duplicate_tb->toolTip() + QString(" (%1)").arg(duplicate_tb->shortcut().toString()));
	move_last_tb->setToolTip(move_last_tb->toolTip() + QString(" (%1)").arg(move_last_tb->shortcut().toString()));
	move_first_tb->setToolTip(move_first_tb->toolTip() + QString(" (%1)").arg(move_first_tb->shortcut().toString()));
	move_up_tb->setToolTip(move_up_tb->toolTip() + QString(" (%1)").arg(move_up_tb->shortcut().toString()));
	move_down_tb->setToolTip(move_down_tb->toolTip() + QString(" (%1)").arg(move_down_tb->shortcut().toString()));
}

void ObjectsTableWidget::setButtonConfiguration(unsigned button_conf)
{
	bool move_btn = false;

	//Checking via bitwise operation the buttons available on the 'button_conf'
	move_btn=(button_conf & MoveButtons) == MoveButtons;

	move_down_tb->setVisible(move_btn);
	move_up_tb->setVisible(move_btn);
	move_first_tb->setVisible(move_btn);
	move_last_tb->setVisible(move_btn);

	edit_tb->setVisible((button_conf & EditButton) == EditButton);
	remove_all_tb->setVisible((button_conf & RemoveAllButton) == RemoveAllButton);

	add_tb->setVisible((button_conf & AddButton) == AddButton);
	remove_tb->setVisible((button_conf & RemoveButton) == RemoveButton);
	update_tb->setVisible((button_conf & UpdateButton) == UpdateButton);
	duplicate_tb->setVisible((button_conf & DuplicateButton) == DuplicateButton);

	//Disabling the horizontal spacers when no buttons are visible
	if(button_conf==NoButtons)
	{
		left_spc->changeSize(0,0,QSizePolicy::Ignored,QSizePolicy::Ignored);
		right_spc->changeSize(0,0,QSizePolicy::Ignored,QSizePolicy::Ignored);
	}
	//Restoring the horizontal spacers when some buttons are visible
	else
	{
		left_spc->changeSize(10,10,QSizePolicy::Expanding,QSizePolicy::Preferred);
		right_spc->changeSize(10,10,QSizePolicy::Expanding,QSizePolicy::Preferred);
	}
}

QTableWidgetItem *ObjectsTableWidget::getItem(unsigned row_idx, unsigned col_idx)
{
	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ErrorCode::RefRowObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ErrorCode::RefColObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return table_tbw->item(row_idx, col_idx);
}

void ObjectsTableWidget::adjustColumnToContents(int col)
{
	table_tbw->resizeColumnToContents(col);
	table_tbw->resizeRowsToContents();
}

void ObjectsTableWidget::setColumnCount(unsigned col_count)
{
	if(col_count > 0)
	{
		unsigned i;
		QTableWidgetItem *item=nullptr;

		i=table_tbw->columnCount();
		table_tbw->setColumnCount(col_count);

		for(;i < col_count; i++)
		{
			item=new QTableWidgetItem;
			item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			table_tbw->setHorizontalHeaderItem(static_cast<int>(i),item);
		}
	}
}

void ObjectsTableWidget::setHeaderLabel(const QString &label, unsigned col_idx)
{
	QTableWidgetItem *item=nullptr;

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ErrorCode::RefColObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->horizontalHeaderItem(col_idx);
	item->setText(label);
}

void ObjectsTableWidget::setHeaderVisible(unsigned col_idx, bool visible)
{
  if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ErrorCode::RefColObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  table_tbw->horizontalHeader()->setSectionHidden(col_idx, !visible);
}

void ObjectsTableWidget::setHeaderIcon(const QIcon &icon, unsigned col_idx)
{
	QTableWidgetItem *item=nullptr;

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ErrorCode::RefColObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->horizontalHeaderItem(col_idx);
	item->setIcon(icon);
}

void ObjectsTableWidget::setCellIcon(const QIcon &icon, unsigned row_idx, unsigned col_idx)
{
	getItem(row_idx, col_idx)->setIcon(icon);
}

void ObjectsTableWidget::setCellText(const QString &text, unsigned row_idx, unsigned col_idx)
{
	getItem(row_idx, col_idx)->setText(text);
}

void ObjectsTableWidget::clearCellText(unsigned row_idx, unsigned col_idx)
{
	try
	{
		setCellText(QString(), row_idx, col_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ObjectsTableWidget::setRowFont(int row_idx, const QFont &font, const QColor &fg_color, const QColor &bg_color)
{
	QTableWidgetItem *item=nullptr;
	int col_count, i;

	if(row_idx >= table_tbw->rowCount())
		throw Exception(ErrorCode::RefRowObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	col_count=table_tbw->columnCount();
	for(i=0; i < col_count; i++)
	{
		item=table_tbw->item(row_idx, i);
		item->setFont(font);
		item->setForeground(fg_color);
		item->setBackground(bg_color);
	}
}

void ObjectsTableWidget::setRowData(const QVariant &data, unsigned row_idx)
{
	QTableWidgetItem *item=nullptr;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ErrorCode::RefRowObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Gets the vertical header of the row. This header stores the whole row data.
	item=table_tbw->verticalHeaderItem(row_idx);
	item->setData(Qt::UserRole, data);
}

unsigned ObjectsTableWidget::getColumnCount()
{
	return table_tbw->columnCount();
}

unsigned ObjectsTableWidget::getRowCount()
{
	return table_tbw->rowCount();
}

QString ObjectsTableWidget::getHeaderLabel(unsigned col_idx)
{
	QTableWidgetItem *item=nullptr;

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ErrorCode::RefColObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->horizontalHeaderItem(col_idx);
	return item->text();
}

QString ObjectsTableWidget::getCellText(unsigned row_idx, unsigned col_idx)
{
	return getItem(row_idx, col_idx)->text();
}

Qt::CheckState ObjectsTableWidget::getCellCheckState(unsigned row_idx, unsigned col_idx)
{
	return getItem(row_idx, col_idx)->checkState();
}

void ObjectsTableWidget::setCellCheckState(unsigned row_idx, unsigned col_idx, Qt::CheckState check_state)
{
	getItem(row_idx, col_idx)->setCheckState(check_state);
}

void ObjectsTableWidget::setCellDisabled(unsigned row_idx, unsigned col_idx, bool disabled)
{
	QTableWidgetItem *item = getItem(row_idx, col_idx);

	if(disabled)
		item->setFlags(Qt::NoItemFlags);
	else
		item->setFlags(Qt::ItemIsEnabled);
}

bool ObjectsTableWidget::isCellDisabled(unsigned row_idx, unsigned col_idx)
{
	return (getItem(row_idx, col_idx)->flags() == Qt::NoItemFlags);
}

QVariant ObjectsTableWidget::getRowData(unsigned row_idx)
{
	QTableWidgetItem *item=nullptr;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ErrorCode::RefRowObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->verticalHeaderItem(row_idx);
	return item->data(Qt::UserRole);
}

int ObjectsTableWidget::getSelectedRow()
{
	return table_tbw->currentRow();
}

int ObjectsTableWidget::getRowIndex(const QVariant &data)
{
	unsigned i, count;
	QTableWidgetItem *item=nullptr;
	bool found=false;

	count=table_tbw->rowCount();

	for(i=0; !found && i < count; i++)
	{
		item=table_tbw->verticalHeaderItem(i);

		//Checking if the item data is equal to the passed data
		found=(item && item->data(Qt::UserRole).value<void *>() == data.value<void *>());
	}

	if(!found)
		return -1;
	else
		return i;
}

void ObjectsTableWidget::addColumn(unsigned col_idx)
{
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		col_idx=table_tbw->columnCount();

	table_tbw->insertColumn(col_idx);
	table_tbw->clearSelection();
	setButtonsEnabled();

	emit s_columnAdded(col_idx);
}

void ObjectsTableWidget::selectRow(int lin_idx)
{
	QTableWidgetItem *item=nullptr;

	item=table_tbw->item(0,lin_idx);

	if(item)
	{
		item=table_tbw->item(lin_idx,0);
		item->setSelected(true);
		table_tbw->setCurrentItem(item);
		setButtonsEnabled();
	}
}

void ObjectsTableWidget::addRow(unsigned lin_idx)
{
	QTableWidgetItem *item=nullptr;
	unsigned col_idx, col_cont=table_tbw->columnCount();

	table_tbw->insertRow(lin_idx);

	item=new QTableWidgetItem;
	item->setText(QString("%1").arg(lin_idx+1));
	table_tbw->setVerticalHeaderItem(lin_idx,item);

	for(col_idx=0; col_idx < col_cont; col_idx++)
	{
		item=new QTableWidgetItem;		
		table_tbw->setItem(lin_idx,col_idx,item);
	}

	item=table_tbw->item(lin_idx,0);
	item->setSelected(true);
	table_tbw->setCurrentItem(item);
}

void ObjectsTableWidget::addRow()
{
	this->addRow(table_tbw->rowCount());
	setButtonsEnabled();
	table_tbw->resizeRowsToContents();

	emit s_rowAdded(table_tbw->rowCount()-1);
}

void ObjectsTableWidget::removeRow(unsigned row_idx)
{
	unsigned i, count;
	bool conf;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ErrorCode::RefRowObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Before remove the row, clears the selection
	table_tbw->clearSelection();
	count=table_tbw->columnCount();

	//Selects all the columns of the row to be removed
	for(i=0; i < count; i++)
		table_tbw->item(row_idx, i)->setSelected(true);

	table_tbw->setCurrentItem(table_tbw->item(row_idx,0));

	//Disable temporarily the exclusion confirmation
	conf=conf_exclusion;
	conf_exclusion=false;
	removeRow();
	conf_exclusion=conf;
}

void ObjectsTableWidget::removeRow()
{
	if(table_tbw->currentRow()>=0)
	{
		Messagebox msg_box;
		unsigned 	row_idx=table_tbw->currentRow();
		QTableWidgetItem *item=table_tbw->currentItem();

		if(item->isSelected())
		{
			if(conf_exclusion)
				msg_box.show(tr("Confirmation"),tr("Do you really want to remove the selected item?"),
							 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

			if(!conf_exclusion || (conf_exclusion && msg_box.result()==QDialog::Accepted))
			{
				setRowData(QVariant::fromValue<void *>(nullptr), row_idx);
				item->setData(Qt::UserRole, QVariant::fromValue<void *>(nullptr));

				emit s_rowAboutToRemove(row_idx);

				table_tbw->removeRow(row_idx);
				table_tbw->setCurrentItem(nullptr);
				setButtonsEnabled();

				emit s_rowRemoved(row_idx);
			}
		}
	}
}

void ObjectsTableWidget::duplicateRow()
{
	if(table_tbw->currentRow() >= 0)
	{
		int row = table_tbw->rowCount(),
				curr_row = table_tbw->currentRow();

		QTableWidgetItem *curr_item = nullptr,
				*dup_item=nullptr;

		addRow(row);

		for(int col = 0; col < table_tbw->columnCount(); col++)
		{
			curr_item = table_tbw->item(curr_row, col);
			dup_item = table_tbw->item(row, col);
			dup_item->setText(curr_item->text());
		}

		emit s_rowDuplicated(curr_row, row);
	}
}

void ObjectsTableWidget::removeRows()
{
	if(table_tbw->rowCount() > 0)
	{
		QObject *sender_obj=sender();
		Messagebox msg_box;

		/* Only shows the confirmation message if the conf_exclusion is set and the user called the method
			 activating the 'remove_all_tb' button */
		if(conf_exclusion && sender_obj==remove_all_tb)
			msg_box.show(tr("Confirmation"),tr("Do you really want to remove all the items?"),
						 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

		if(!conf_exclusion || (conf_exclusion && sender_obj!=remove_all_tb) ||
				(conf_exclusion &&  sender_obj==remove_all_tb && msg_box.result()==QDialog::Accepted))
		{
			table_tbw->clearContents();
			table_tbw->setRowCount(0);
			setButtonsEnabled();
			emit s_rowsRemoved();
		}
	}
}

void ObjectsTableWidget::removeColumn(unsigned col_idx)
{
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ErrorCode::RefColObjectTabInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	table_tbw->removeColumn(col_idx);
	table_tbw->clearSelection();
	setButtonsEnabled();

	emit s_columnRemoved(col_idx);
}

void ObjectsTableWidget::moveRows()
{
	QObject *sender_obj=sender();
	QTableWidgetItem *item=nullptr, *item1=nullptr;
	int row=-1, row1=-1;
	unsigned col, col_count=table_tbw->columnCount();
	QVariant aux_data;

	//Get the current selected row. Based upon this index that rows are swaped
	row=table_tbw->currentRow();

	//If the user click "move down" swap the selected row with the row imediately below
	if(sender_obj==move_down_tb)
		row1=row+1;
	//If the user click "move up" swap the selected row with the row imediately above
	else if(sender_obj==move_up_tb)
		row1=row-1;
	//If the user click "move first" swap the selected row with the first row
	else if(sender_obj==move_first_tb)
	{
		//Add a blank line to swap the data later
		this->addRow(0);
		row1=0;
		row++;
	}
	//If the user click "move last" swap the selected row with the last row
	else if(sender_obj==move_last_tb)
	{
		//Add a row at the end of the table
		this->addRow(table_tbw->rowCount());
		row1=table_tbw->rowCount()-1;
	}

	//Checking if the row indexes are valid
	if(row >= 0 && row < table_tbw->rowCount() &&
			row1 >= 0 && row1 < table_tbw->rowCount() &&
			row != row1)
	{
		//To swap the rows is necessary to swap all columns one by one
		for(col=0; col < col_count; col++)
		{
			item=table_tbw->item(row, col);
			table_tbw->takeItem(row, col);

			item1=table_tbw->item(row1, col);
			table_tbw->takeItem(row1, col);

			table_tbw->setItem(row, col, item1);
			table_tbw->setItem(row1, col, item);

			item1->setSelected(false);
			item->setSelected(true);
		}

		table_tbw->setCurrentItem(item);

		//Swapping the row data
		item=table_tbw->verticalHeaderItem(row);
		item1=table_tbw->verticalHeaderItem(row1);

		if(item && item1)
		{
			aux_data=item->data(Qt::UserRole);
			item->setData(Qt::UserRole, item1->data(Qt::UserRole));
			item1->setData(Qt::UserRole, aux_data);
		}

		//Special case for "move last" and "move first"
		if(sender_obj==move_last_tb || sender_obj==move_first_tb)
		{
			table_tbw->removeRow(row);

			if(sender_obj==move_first_tb)
			{
				row1=row-1;
				row=table_tbw->rowCount();
			}
		}

		setButtonsEnabled();
		table_tbw->resizeRowsToContents();
		emit s_rowsMoved(row, row1);
	}
}

void ObjectsTableWidget::editRow()
{
	emit s_rowEdited(table_tbw->currentRow());
}

void ObjectsTableWidget::updateRow()
{
	emit s_rowUpdated(table_tbw->currentRow());
}

void ObjectsTableWidget::clearSelection()
{
	table_tbw->clearSelection();
	table_tbw->setCurrentItem(nullptr);
	setButtonsEnabled();
}

void ObjectsTableWidget::setButtonsEnabled(unsigned button_conf, bool value)
{
	int lin=-1;
	QTableWidgetItem *item=table_tbw->currentItem();

	if(item)
		lin=item->row();

	if((button_conf & MoveButtons) == MoveButtons)
	{
		move_up_tb->setEnabled(value && lin > 0);
		move_down_tb->setEnabled(value && lin >= 0 && lin < table_tbw->rowCount()-1);
		move_first_tb->setEnabled(value && lin > 0 && lin<=table_tbw->rowCount()-1);
		move_last_tb->setEnabled(value && lin >=0 && lin < table_tbw->rowCount()-1);
	}

	if((button_conf & EditButton) == EditButton)
		edit_tb->setEnabled(value && lin >= 0);

	if((button_conf & AddButton) == AddButton)
		add_tb->setEnabled(value);

	if((button_conf & RemoveButton) == RemoveButton)
		remove_tb->setEnabled(value && lin >= 0);

	if((button_conf & RemoveAllButton) == RemoveAllButton)
		remove_all_tb->setEnabled(value && table_tbw->rowCount() > 0);

	if((button_conf & UpdateButton) == UpdateButton)
		update_tb->setEnabled(value && lin >= 0);

	if((button_conf & DuplicateButton) == DuplicateButton)
		duplicate_tb->setEnabled(value && lin >= 0);

	if((button_conf & ResizeColsButton) == ResizeColsButton)
		resize_cols_tb->setEnabled(value && table_tbw->rowCount() > 0);
}

void ObjectsTableWidget::setCellsEditable(bool value)
{
	table_tbw->setSelectionBehavior(value ? QAbstractItemView::SelectItems : QAbstractItemView::SelectRows);
	table_tbw->setEditTriggers(value ? QAbstractItemView::AllEditTriggers : QAbstractItemView::NoEditTriggers);
}

void ObjectsTableWidget::setButtonsEnabled()
{
	setButtonsEnabled(AllButtons, true);
}

void ObjectsTableWidget::emitRowSelected()
{
	QTableWidgetItem *item=table_tbw->currentItem();

	if(item && item->row() >= 0)
		emit s_rowSelected(item->row());
}

void ObjectsTableWidget::resizeEvent(QResizeEvent *)
{
	table_tbw->resizeRowsToContents();
}
