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

#include "messagebox.h"

Messagebox::Messagebox(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	cancelled=false;
	connect(yes_ok_btn,SIGNAL(clicked()),this,SLOT(handleYesOkClick()));
	connect(no_btn,SIGNAL(clicked()),this,SLOT(handleNoCancelClick()));
	connect(cancel_btn,SIGNAL(clicked()),this,SLOT(handleNoCancelClick()));
	connect(show_errors_tb,SIGNAL(clicked()),this,SLOT(showExceptionList()));
	connect(show_errors_tb,SIGNAL(toggled(bool)),show_errors_txt_tb,SLOT(setVisible(bool)));
	connect(show_errors_txt_tb,SIGNAL(toggled(bool)),this,SLOT(showExceptionList(void)));
	show_errors_tb->setVisible(false);
	show_errors_txt_tb->setVisible(false);
}

void Messagebox::handleYesOkClick(void)
{
	exceptions_trw->clear();
	accept();
}

void Messagebox::handleNoCancelClick(void)
{
	exceptions_trw->clear();

	if((sender()==no_btn && !cancel_btn->isVisible()) ||
		 (sender()==cancel_btn && !no_btn->isVisible()))
		reject();
	else if(sender()==no_btn && cancel_btn->isVisible())
		reject();
	else if(sender()==cancel_btn && no_btn->isVisible())
	{
		cancelled=true;
		reject();
	}
}

bool Messagebox::isCancelled(void)
{
	return(cancelled);
}

void Messagebox::showExceptionList(void)
{
	if(show_errors_tb->isChecked())
	{
		show_errors_tb->setIcon(QPixmap(QString(":/icones/icones/desfazer.png")));

		if(show_errors_txt_tb->isChecked())
			objs_group_wgt->setCurrentIndex(2);
		else
			objs_group_wgt->setCurrentIndex(1);
	}
	else
	{
		show_errors_tb->setIcon(QPixmap(QString(":/icones/icones/refazer.png")));
		objs_group_wgt->setCurrentIndex(0);
	}
}

void Messagebox::show(Exception e, const QString &msg, unsigned icon_type)
{
	vector<Exception> list;
	vector<Exception>::reverse_iterator itr,itr_end;
	QTreeWidgetItem *item=nullptr,*item1=nullptr,*item2=nullptr;
	QLabel *label=nullptr;
	int idx=0;
	Exception *ex=nullptr;
	QString str_aux, title;
	QFont font=this->font();

	show_errors_txt_tb->blockSignals(true);
	show_errors_txt_tb->setChecked(false);
	show_errors_txt_tb->blockSignals(false);

	exceptions_txt->setPlainText(e.getExceptionsText());

	e.getExceptionsList(list);

	itr=list.rbegin();
	itr_end=list.rend();

	while(itr!=itr_end)
	{
		ex=&(*itr);

		item=new QTreeWidgetItem;
		str_aux=QString("[%1] - %2")
						.arg(idx)
						.arg(ex->getMethod());
		item->setIcon(0,QPixmap(QString(":/icones/icones/funcao.png")));
		exceptions_trw->insertTopLevelItem(0,item);
		label=new QLabel;
		label->setFont(font);
		label->setWordWrap(true);
		label->setText(str_aux);
		exceptions_trw->setItemWidget(item, 0, label);

		item1=new QTreeWidgetItem(item);
		item1->setIcon(0,QPixmap(QString(":/icones/icones/codigofonte.png")));
		item1->setText(0,ex->getFile() + " (" + ex->getLine() + ")");

		item2=new QTreeWidgetItem(item);
		item2->setIcon(0,QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
		item2->setText(0,Exception::getErrorCode(ex->getErrorType()) +
									 " (" + QString("%1").arg(ex->getErrorType()) + ")");

		item1=new QTreeWidgetItem(item);
		item1->setIcon(0,QPixmap(":/icones/icones/msgbox_erro.png"));
		label=new QLabel;
		label->setWordWrap(true);
		label->setFont(font);
		label->setStyleSheet("color: #ff0000;");
		exceptions_trw->setItemWidget(item1, 0, label);
		label->setText(ex->getErrorMessage());

		if(!ex->getExtraInfo().isEmpty())
		{
			item1=new QTreeWidgetItem(item);
			item1->setIcon(0,QPixmap(QString(":/icones/icones/msgbox_info.png")));
			label=new QLabel;
			label->setWordWrap(true);
			label->setFont(font);
			label->setStyleSheet("color: #000080;");
			exceptions_trw->setItemWidget(item1, 0, label);
			label->setText(ex->getExtraInfo());
		}

		itr++;
		idx++;
	}

	switch(icon_type)
	{
		case ERROR_ICON:
			title=trUtf8("Error");
		break;

		case ALERT_ICON:
			title=trUtf8("Alert");
		break;

		case INFO_ICON:
			title=trUtf8("Information");
		break;

		default:
			title="";
		break;
	}

	if(msg.isEmpty())
		str_aux=e.getErrorMessage();
	else
		str_aux=msg;

	this->show(title,str_aux,icon_type,OK_BUTTON);
}

void Messagebox::show(const QString &title, const QString &msg, unsigned icon_type, unsigned buttons)
{
	QString icon_name;

	switch(buttons)
	{
		case YES_NO_BUTTONS:
			no_btn->setText(trUtf8("&No"));
			yes_ok_btn->setText(trUtf8("&Yes"));
		break;

		case OK_CANCEL_BUTTONS:
			cancel_btn->setText(trUtf8("&Cancel"));
			yes_ok_btn->setText(trUtf8("&Ok"));
		break;

		case ALL_BUTTONS:
			cancel_btn->setText(trUtf8("&Cancel"));
			no_btn->setText(trUtf8("&No"));
			yes_ok_btn->setText(trUtf8("&Yes"));
		break;

		default:
		case OK_BUTTON:
			yes_ok_btn->setText(trUtf8("&Ok"));
		break;
	}

	no_btn->setVisible(buttons==YES_NO_BUTTONS || buttons==ALL_BUTTONS);
	cancel_btn->setVisible(buttons==OK_CANCEL_BUTTONS || buttons==ALL_BUTTONS);

	switch(icon_type)
	{
		case ERROR_ICON:
			icon_name="msgbox_erro.png";
		break;

		case INFO_ICON:
			icon_name="msgbox_info.png";
		break;

		case ALERT_ICON:
			icon_name="msgbox_alerta.png";
		break;

		case CONFIRM_ICON:
			icon_name="msgbox_quest.png";
		break;

		default:
			icon_name="";
		break;
	}

	cancelled=false;

	icon_lbl->setVisible(!icon_name.isEmpty());
	if(icon_name!="")
		icon_lbl->setPixmap(QPixmap((":/icones/icones/" + icon_name)));

	msg_lbl->setText(msg);

	this->resize(this->minimumWidth(),this->minimumHeight());
	this->setWindowTitle("pgModeler - " + title);
	this->objs_group_wgt->setCurrentIndex(0);
	this->show_errors_tb->setChecked(false);
	this->show_errors_tb->setVisible((exceptions_trw->topLevelItemCount() > 0));
	showExceptionList();

	QDialog::exec();
}
