/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "objecttablewidget.h"

ObjectTableWidget::ObjectTableWidget(unsigned button_conf, bool conf_exclusion, QWidget *parent): QWidget(parent)
{
	setupUi(this);
	connect(move_down_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(move_up_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(move_first_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(move_last_tb, SIGNAL(clicked(bool)), this, SLOT(moveRows(void)));
	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(addRow(void)));
	connect(remove_tb, SIGNAL(clicked(bool)), this, SLOT(removeRow(void)));
	connect(edit_tb, SIGNAL(clicked(bool)), this, SLOT(editRow(void)));
	connect(update_tb, SIGNAL(clicked(bool)), this, SLOT(updateRow(void)));
	connect(remove_all_tb, SIGNAL(clicked(bool)), this, SLOT(removeRows(void)));
	connect(table_tbw, SIGNAL(cellClicked(int,int)), this, SLOT(setButtonsEnabled(void)));
	connect(table_tbw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editRow(void)));

	this->conf_exclusion=conf_exclusion;

	setButtonConfiguration(button_conf);
	setColumnCount(1);

	add_tb->setToolTip(add_tb->toolTip() + QString(" (%1)").arg(add_tb->shortcut().toString()));
	remove_tb->setToolTip(remove_tb->toolTip() + QString(" (%1)").arg(remove_tb->shortcut().toString()));
	remove_all_tb->setToolTip(remove_all_tb->toolTip() + QString(" (%1)").arg(remove_all_tb->shortcut().toString()));
	update_tb->setToolTip(update_tb->toolTip() + QString(" (%1)").arg(update_tb->shortcut().toString()));
	edit_tb->setToolTip(edit_tb->toolTip() + QString(" (%1)").arg(edit_tb->shortcut().toString()));
	move_last_tb->setToolTip(move_last_tb->toolTip() + QString(" (%1)").arg(move_last_tb->shortcut().toString()));
	move_first_tb->setToolTip(move_first_tb->toolTip() + QString(" (%1)").arg(move_first_tb->shortcut().toString()));
	move_up_tb->setToolTip(move_up_tb->toolTip() + QString(" (%1)").arg(move_up_tb->shortcut().toString()));
	move_down_tb->setToolTip(move_down_tb->toolTip() + QString(" (%1)").arg(move_down_tb->shortcut().toString()));
}

void ObjectTableWidget::setButtonConfiguration(unsigned button_conf)
{
	bool move_btn, edt_btn, add_btn, rem_all_btn, rem_btn, upd_btn;

	//Checking via bitwise operation the buttons available on the 'button_conf'
	move_btn=(button_conf & MOVE_BUTTONS) == MOVE_BUTTONS;
	edt_btn=(button_conf & EDIT_BUTTON) == EDIT_BUTTON;
	add_btn=(button_conf & ADD_BUTTON) == ADD_BUTTON;
	rem_btn=(button_conf & REMOVE_BUTTON) == REMOVE_BUTTON;
	rem_all_btn=(button_conf & REMOVE_ALL_BUTTON) == REMOVE_ALL_BUTTON;
	upd_btn=(button_conf & UPDATE_BUTTON) == UPDATE_BUTTON;

	move_down_tb->setVisible(move_btn);
	move_up_tb->setVisible(move_btn);
	move_first_tb->setVisible(move_btn);
	move_last_tb->setVisible(move_btn);

	edit_tb->setVisible(edt_btn);
	remove_all_tb->setVisible(rem_all_btn);

	add_tb->setVisible(add_btn);
	remove_tb->setVisible(rem_btn);
	update_tb->setVisible(upd_btn);

	//Disabling the horizontal spacers when no buttons are visible
	if(button_conf==NO_BUTTONS)
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

void ObjectTableWidget::setColumnCount(unsigned col_count)
{
	if(col_count > 0)
	{
		unsigned i;
		QTableWidgetItem *item=NULL;

		i=table_tbw->columnCount();
		table_tbw->setColumnCount(col_count);

		for(;i < col_count; i++)
		{
			item=new QTableWidgetItem;
			table_tbw->setHorizontalHeaderItem(static_cast<int>(i),item);
		}
	}
}

void ObjectTableWidget::setHeaderLabel(const QString &label, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->horizontalHeaderItem(col_idx);
	item->setText(label);
}

void ObjectTableWidget::setHeaderIcon(const QIcon &icon, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->horizontalHeaderItem(col_idx);
	item->setIcon(icon);
}

void ObjectTableWidget::setCellIcon(const QIcon &icon, unsigned row_idx, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->item(row_idx, col_idx);
	item->setIcon(icon);
}

void ObjectTableWidget::setCellText(const QString &text, unsigned row_idx, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->item(row_idx,col_idx);
	item->setText(text);
}

void ObjectTableWidget::clearCellText(unsigned row_idx, unsigned col_idx)
{
 try
	{
		setCellText("", row_idx, col_idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ObjectTableWidget::setRowFont(int row_idx, const QFont &font, const QColor &fg_color, const QColor &bg_color)
{
	QTableWidgetItem *item=NULL;
	int col_count, i;

	if(row_idx >= table_tbw->rowCount())
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	col_count=table_tbw->columnCount();
	for(i=0; i < col_count; i++)
	{
		item=table_tbw->item(row_idx, i);
		item->setFont(font);
		item->setForeground(fg_color);
		item->setBackgroundColor(bg_color);
	}
}

void ObjectTableWidget::setRowData(const QVariant &data, unsigned row_idx)
{
	QTableWidgetItem *item=NULL;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	//Gets the vertical header of the row. This header stores the whole row data.
	item=table_tbw->verticalHeaderItem(row_idx);	
	item->setData(Qt::UserRole, data);
}

unsigned ObjectTableWidget::getColumnCount(void)
{
	return(table_tbw->columnCount());
}

unsigned ObjectTableWidget::getRowCount(void)
{
	return(table_tbw->rowCount());
}

QString ObjectTableWidget::getHeaderLabel(unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->horizontalHeaderItem(col_idx);
	return(item->text());
}

QString ObjectTableWidget::getCellText(unsigned row_idx, unsigned col_idx)
{
	QTableWidgetItem *item=NULL;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->item(row_idx,col_idx);
	return(item->text());
}

QVariant ObjectTableWidget::getRowData(unsigned row_idx)
{
	QTableWidgetItem *item=NULL;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	item=table_tbw->verticalHeaderItem(row_idx);
	return(item->data(Qt::UserRole));
}

int ObjectTableWidget::getSelectedRow(void)
{
	return(table_tbw->currentRow());
}

int ObjectTableWidget::getRowIndex(const QVariant &data)
{
	unsigned i, count;
	QTableWidgetItem *item=NULL;
	bool found=false;

	count=table_tbw->rowCount();

	for(i=0; !found && i < count; i++)
	{
		item=table_tbw->verticalHeaderItem(i);

		//Checking if the item data is equal to the passed data
		found=(item && item->data(Qt::UserRole).value<void *>() == data.value<void *>());
	}

	if(!found)
		return(-1);
	else
		return(i);
}

void ObjectTableWidget::addColumn(unsigned col_idx)
{
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		col_idx=table_tbw->columnCount();

	table_tbw->insertColumn(col_idx);
	table_tbw->clearSelection();
	setButtonsEnabled();

	emit s_columnAdded(col_idx);
}

void ObjectTableWidget::selectRow(int lin_idx)
{
	QTableWidgetItem *item=NULL;

	item=table_tbw->item(0,lin_idx);

	if(item)
	{
		item=table_tbw->item(lin_idx,0);
		item->setSelected(true);
		table_tbw->setCurrentItem(item);
		setButtonsEnabled();
	}
}

void ObjectTableWidget::addRow(unsigned lin_idx)
{
	QTableWidgetItem *item=NULL;
	unsigned i, col_cont=table_tbw->columnCount();

	table_tbw->insertRow(lin_idx);

	item=new QTableWidgetItem;
	item->setText(QString("%1").arg(lin_idx+1));
	table_tbw->setVerticalHeaderItem(lin_idx,item);

	for(i=0; i < col_cont; i++)
	{
		item=new QTableWidgetItem;
		table_tbw->setItem(lin_idx,i,item);
	}

	item=table_tbw->item(lin_idx,0);
	item->setSelected(true);
	table_tbw->setCurrentItem(item);
}

void ObjectTableWidget::addRow(void)
{
	this->addRow(table_tbw->rowCount());
	setButtonsEnabled();

	emit s_rowAdded(table_tbw->rowCount()-1);
}

void ObjectTableWidget::removeRow(unsigned row_idx)
{
	unsigned i, count;
	bool conf;

	if(row_idx >= static_cast<unsigned>(table_tbw->rowCount()))
		throw Exception(ERR_REF_LIN_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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

void ObjectTableWidget::removeRow(void)
{
	if(table_tbw->currentRow()>=0)
	{
		MessageBox msg_box;
		unsigned 	row_idx=table_tbw->currentRow();
		QTableWidgetItem *item=table_tbw->currentItem();

		if(item->isSelected())
		{
			if(conf_exclusion)
				msg_box.show(trUtf8("Confirmação"),trUtf8("Do you really want to remove the selected item?"),
												MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

			if(!conf_exclusion || (conf_exclusion && msg_box.result()==QDialog::Accepted))
			{
				table_tbw->removeRow(row_idx);
				table_tbw->setCurrentItem(NULL);
				setButtonsEnabled();

				emit s_rowRemoved(row_idx);
			}
		}
	}
}

void ObjectTableWidget::removeRows(void)
{
	if(table_tbw->rowCount() > 0)
	{
		QObject *sender_obj=sender();
		MessageBox msg_box;

		/* Only shows the confirmation message if the conf_exclusion is set and the user called the method
			 activating the 'remove_all_tb' button */
		if(conf_exclusion && sender_obj==remove_all_tb)
			msg_box.show(trUtf8("Confirmação"),trUtf8("Do you really want to remove the all items?"),
											MessageBox::CONFIRM_ICON, MessageBox::YES_NO_BUTTONS);

		if(!conf_exclusion || (conf_exclusion && sender_obj!=remove_all_tb) ||
			 (conf_exclusion &&  sender_obj==remove_all_tb && msg_box.result()==QDialog::Accepted))
		{
			while(table_tbw->rowCount() > 0)
				table_tbw->removeRow(0);

			setButtonsEnabled();

			emit s_rowsRemoved();
		}
	}
}

void ObjectTableWidget::removeColumn(unsigned col_idx)
{
	if(col_idx >= static_cast<unsigned>(table_tbw->columnCount()))
		throw Exception(ERR_REF_COL_OBJTAB_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	table_tbw->removeColumn(col_idx);
	table_tbw->clearSelection();
	setButtonsEnabled();

	emit s_columnRemoved(col_idx);
}

void ObjectTableWidget::moveRows(void)
{
	QObject *sender_obj=sender();
	QTableWidgetItem *item=NULL, *item1=NULL;
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
		emit s_rowsMoved(row, row1);
	}
}

void ObjectTableWidget::editRow(void)
{
	emit s_rowEdited(table_tbw->currentRow());
}

void ObjectTableWidget::updateRow(void)
{
	emit s_rowUpdated(table_tbw->currentRow());
}

void ObjectTableWidget::clearSelection(void)
{
	table_tbw->clearSelection();
	table_tbw->setCurrentItem(NULL);
	setButtonsEnabled();
}

void ObjectTableWidget::setButtonsEnabled(unsigned button_conf, bool value)
{
	int lin=-1;
	QTableWidgetItem *item=table_tbw->currentItem();

	if(item)
		lin=item->row();

	if((button_conf & MOVE_BUTTONS) == MOVE_BUTTONS)
	{
		move_up_tb->setEnabled(value && lin > 0);
		move_down_tb->setEnabled(value && lin >= 0 && lin < table_tbw->rowCount()-1);
		move_first_tb->setEnabled(value && lin > 0 && lin<=table_tbw->rowCount()-1);
		move_last_tb->setEnabled(value && lin >=0 && lin < table_tbw->rowCount()-1);
	}

	if((button_conf & EDIT_BUTTON) == EDIT_BUTTON)
		edit_tb->setEnabled(value && lin >= 0);

	if((button_conf & ADD_BUTTON) == ADD_BUTTON)
		add_tb->setEnabled(value);

	if((button_conf & REMOVE_BUTTON) == REMOVE_BUTTON)
		remove_tb->setEnabled(value && lin >= 0);

	if((button_conf & REMOVE_ALL_BUTTON) == REMOVE_ALL_BUTTON)
		remove_all_tb->setEnabled(value && table_tbw->rowCount() > 0);

	if((button_conf & UPDATE_BUTTON) == UPDATE_BUTTON)
		update_tb->setEnabled(value && lin >= 0);
}

void ObjectTableWidget::setButtonsEnabled(void)
{
	QTableWidgetItem *item=table_tbw->currentItem();

	setButtonsEnabled(ALL_BUTTONS, true);

	if(item && item->row() >= 0)
		emit s_rowSelected(item->row());
}

