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

#include "operationlistwidget.h"
#include "taskprogresswidget.h"

extern TaskProgressWidget *task_prog_wgt;

OperationListWidget::OperationListWidget(QWidget *parent, Qt::WindowFlags f) : QDockWidget(parent, f)
{
	setupUi(this);
	setModel(NULL);

	operations_tw->headerItem()->setHidden(true);
	connect(undo_tb,SIGNAL(clicked()),this,SLOT(undoOperation(void)));
	connect(redo_tb,SIGNAL(clicked()),this,SLOT(redoOperation(void)));
	connect(rem_operations_tb,SIGNAL(clicked()),this,SLOT(removeOperations(void)));
	connect(operations_tw,SIGNAL(itemClicked(QTreeWidgetItem *, int)),
					this,SLOT(selectItem(QTreeWidgetItem *, int)));
}

void OperationListWidget::selectItem(QTreeWidgetItem *item, int)
{
	operations_tw->clearSelection();

	if(item)
	{
		if(item->parent())
			item=item->parent();

		item->setSelected(true);
		operations_tw->setCurrentItem(item);
	}
}

void OperationListWidget::updateOperationList(void)
{
	if(!model_wgt)
	{
		operations_tw->clear();
		dockWidgetContents->setEnabled(false);
		op_count_lbl->setText("-");
		current_pos_lbl->setText("-");
	}
	else
	{
		unsigned count, i, op_type;
		ObjectType obj_type;
		QString obj_name, str_aux, op_name, op_icon;
		QTreeWidgetItem *item=NULL,*item1=NULL, *item2=NULL;
		QFont font=this->font();
		bool value=false;

		dockWidgetContents->setEnabled(true);
		op_count_lbl->setText(QString("%1").arg(model_wgt->op_list->getCurrentSize()));
		current_pos_lbl->setText(QString("%1").arg(model_wgt->op_list->getCurrentIndex()));
		redo_tb->setEnabled(model_wgt->op_list->isRedoAvailable());
		undo_tb->setEnabled(model_wgt->op_list->isUndoAvailable());

		count=model_wgt->op_list->getCurrentSize();

		operations_tw->clear();
		rem_operations_tb->setEnabled(count > 0);

		for(i=0; i < count; i++)
		{
			model_wgt->op_list->getOperationData(i,op_type,obj_name,obj_type);

			value=(i==static_cast<unsigned>(model_wgt->op_list->getCurrentIndex()-1));
			font.setBold(value);
			font.setItalic(value);

			item=new QTreeWidgetItem;
			str_aux=QString(BaseObject::getSchemaName(obj_type));
			item->setData(0, Qt::UserRole, QVariant(obj_type));

			if(obj_type==BASE_RELATIONSHIP)
				str_aux+="tv";

			item->setIcon(0,QPixmap(QString(":/icones/icones/") + str_aux + QString(".png")));

			operations_tw->insertTopLevelItem(i,item);
			item->setFont(0,font);
			item->setText(0,trUtf8("Object: %1").arg(BaseObject::getTypeName(obj_type)));

			item2=new QTreeWidgetItem(item);
			item2->setIcon(0,QPixmap(QString(":/icones/icones/uid.png")));
			item2->setFont(0,font);
			item2->setText(0,trUtf8("Name: %1").arg(Utf8String::create(obj_name)));

			if(op_type==Operation::OBJECT_CREATED)
			{
				op_icon="criado";
				op_name=trUtf8("created");
			}
			else if(op_type==Operation::OBJECT_REMOVED)
			{
				op_icon="removido";
				op_name=trUtf8("removed");
			}
			else if(op_type==Operation::OBJECT_MODIFIED)
			{
				op_icon="modificado";
				op_name=trUtf8("modified");
			}
			else if(op_type==Operation::OBJECT_MOVED)
			{
				op_icon="movimentado";
				op_name=trUtf8("moved");
			}

			item1=new QTreeWidgetItem(item);
			item1->setIcon(0,QPixmap(QString(":/icones/icones/") + op_icon + QString(".png")));
			item1->setFont(0,font);
			item1->setText(0,trUtf8("Operation: %1").arg(op_name));

			operations_tw->expandItem(item);

			if(value)
				operations_tw->scrollToItem(item1);
		}
	}

	emit s_operationListUpdated();
}

void OperationListWidget::setModel(ModelWidget *model)
{
	operations_tw->clear();
	this->model_wgt=model;
	updateOperationList();
}

void OperationListWidget::undoOperation(void)
{
	try
	{
		connect(model_wgt->op_list, SIGNAL(s_operationExecuted(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
		task_prog_wgt->setWindowTitle(trUtf8("Undoing operations..."));
		task_prog_wgt->show();

		model_wgt->op_list->undoOperation();

		task_prog_wgt->close();
		disconnect(model_wgt->op_list, NULL, task_prog_wgt, NULL);

		notifyUpdateOnModel();

		model_wgt->scene->clearSelection();
	}
	catch(Exception &e)
	{
		task_prog_wgt->close();
		disconnect(model_wgt->op_list, NULL, task_prog_wgt, NULL);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void OperationListWidget::redoOperation(void)
{
	try
	{
		connect(model_wgt->op_list, SIGNAL(s_operationExecuted(int,QString,unsigned)), task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
		task_prog_wgt->setWindowTitle(trUtf8("Redoing operations..."));
		task_prog_wgt->show();

		model_wgt->op_list->redoOperation();

		task_prog_wgt->close();
		disconnect(model_wgt->op_list, NULL, task_prog_wgt, NULL);

		notifyUpdateOnModel();

		model_wgt->scene->clearSelection();
	}
	catch(Exception &e)
	{
		task_prog_wgt->close();
		disconnect(model_wgt->op_list, NULL, task_prog_wgt, NULL);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void OperationListWidget::removeOperations(void)
{
	MessageBox msg_box;

	msg_box.show(trUtf8("Operation history exclusion"),
									trUtf8("Delete the executed operations history is an irreversible action, do you want to continue?"),
									MessageBox::CONFIRM_ICON,
									MessageBox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		model_wgt->op_list->removeOperations();
		updateOperationList();
		rem_operations_tb->setEnabled(false);
	}
}

void OperationListWidget::notifyUpdateOnModel(void)
{
	updateOperationList();
	emit s_operationExecuted();
}

