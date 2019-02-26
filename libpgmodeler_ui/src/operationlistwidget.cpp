/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "pgmodeleruins.h"

OperationListWidget::OperationListWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setModel(nullptr);

	operations_tw->headerItem()->setHidden(true);
	connect(undo_tb,SIGNAL(clicked()),this,SLOT(undoOperation(void)));
	connect(redo_tb,SIGNAL(clicked()),this,SLOT(redoOperation(void)));
	connect(rem_operations_tb,SIGNAL(clicked()),this,SLOT(removeOperations(void)));
	connect(operations_tw,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(selectItem(QTreeWidgetItem *, int)));
	connect(hide_tb, SIGNAL(clicked(bool)), this, SLOT(hide(void)));
}

void OperationListWidget::hide(void)
{
	QWidget::hide();
	emit s_visibilityChanged(false);
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
	content_wgt->setEnabled(this->model_wgt!=nullptr);

	if(!model_wgt)
	{
		operations_tw->clear();
		op_count_lbl->setText(QString("-"));
		current_pos_lbl->setText(QString("-"));
	}
	else
	{
		unsigned count, i, op_type;
		ObjectType obj_type;
		QString obj_name, str_aux, op_name, op_icon;
		QTreeWidgetItem *item=nullptr,*item1=nullptr, *item2=nullptr;
		QFont font=this->font();
		bool value=false;

		operations_tw->setUpdatesEnabled(false);
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
			item->setData(0, Qt::UserRole, QVariant(enum_cast(obj_type)));

			if(obj_type==ObjectType::BaseRelationship)
				str_aux+=QString("tv");

			item->setIcon(0,QPixmap(PgModelerUiNs::getIconPath(str_aux)));

			operations_tw->insertTopLevelItem(i,item);
			item->setFont(0,font);
			item->setText(0,trUtf8("Object: %1").arg(BaseObject::getTypeName(obj_type)));

			item2=new QTreeWidgetItem(item);
			item2->setIcon(0,QPixmap(PgModelerUiNs::getIconPath("uid")));
			item2->setFont(0,font);
			item2->setText(0,trUtf8("Name: %1").arg(obj_name));

			if(op_type==Operation::ObjectCreated)
			{
				op_icon=QString("criado");
				op_name=trUtf8("created");
			}
			else if(op_type==Operation::ObjectRemoved)
			{
				op_icon=QString("removido");
				op_name=trUtf8("removed");
			}
			else if(op_type==Operation::ObjectModified)
			{
				op_icon=QString("modificado");
				op_name=trUtf8("modified");
			}
			else if(op_type==Operation::ObjectMoved)
			{
				op_icon=QString("movimentado");
				op_name=trUtf8("moved");
			}

			item1=new QTreeWidgetItem(item);
			item1->setIcon(0,QPixmap(PgModelerUiNs::getIconPath(op_icon)));
			item1->setFont(0,font);
			item1->setText(0,trUtf8("Operation: %1").arg(op_name));

			operations_tw->expandItem(item);

			if(value)
				operations_tw->scrollToItem(item1);
		}

		operations_tw->setUpdatesEnabled(true);
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
		QApplication::setOverrideCursor(Qt::WaitCursor);
		model_wgt->op_list->undoOperation();
		notifyUpdateOnModel();
		model_wgt->scene->clearSelection();
		QApplication::restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();
		this->updateOperationList();

		if(e.getErrorType()==ErrorCode::UndoRedoOperationInvalidObject)
		{
			Messagebox msg_box;
			msg_box.show(e, "", Messagebox::AlertIcon);
		}
		else
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void OperationListWidget::redoOperation(void)
{
	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		model_wgt->op_list->redoOperation();
		notifyUpdateOnModel();
		model_wgt->scene->clearSelection();
		QApplication::restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();

		if(e.getErrorType()==ErrorCode::UndoRedoOperationInvalidObject)
		{
			Messagebox msg_box;
			msg_box.show(e, "", Messagebox::AlertIcon);
		}
		else
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void OperationListWidget::removeOperations(void)
{
	Messagebox msg_box;

	msg_box.show(trUtf8("Operation history exclusion"),
				 trUtf8("Delete the executed operations history is an irreversible action, do you want to continue?"),
				 Messagebox::ConfirmIcon,
				 Messagebox::YesNoButtons);

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

