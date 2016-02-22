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

#include "messagebox.h"
#include "pgmodeleruins.h"

Messagebox::Messagebox(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	cancelled=false;
	connect(yes_ok_btn,SIGNAL(clicked()),this,SLOT(handleYesOkClick()));
	connect(no_btn,SIGNAL(clicked()),this,SLOT(handleNoCancelClick()));
	connect(cancel_btn,SIGNAL(clicked()),this,SLOT(handleNoCancelClick()));
	connect(show_errors_tb,SIGNAL(clicked()),this,SLOT(showExceptionList()));
	connect(show_errors_tb,SIGNAL(toggled(bool)),show_raw_info_tb,SLOT(setVisible(bool)));
	connect(show_raw_info_tb,SIGNAL(toggled(bool)),this,SLOT(showExceptionList(void)));
	show_errors_tb->setVisible(false);
	show_raw_info_tb->setVisible(false);
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

		if(show_raw_info_tb->isChecked())
			objs_group_wgt->setCurrentIndex(2);
		else
			objs_group_wgt->setCurrentIndex(1);
	}
	else if(!show_errors_tb->isVisible() && show_raw_info_tb->isChecked())
	{
		objs_group_wgt->setCurrentIndex(2);
	}
	else
	{
		show_errors_tb->setIcon(QPixmap(QString(":/icones/icones/refazer.png")));
		objs_group_wgt->setCurrentIndex(0);
	}
}

void Messagebox::show(Exception e, const QString &msg, unsigned icon_type, unsigned buttons, const QString &yes_lbl, const QString &no_lbl, const QString &cancel_lbl,
					  const QString &yes_ico, const QString &no_ico, const QString &cancel_ico)
{
	vector<Exception> list;
	vector<Exception>::reverse_iterator itr,itr_end;
	QTreeWidgetItem *item=nullptr,*item1=nullptr,*item2=nullptr;
	QLabel *label=nullptr;
	int idx=0;
	Exception *ex=nullptr;
	QString str_aux, title;
	QFont font=this->font();

	show_raw_info_tb->blockSignals(true);
	show_raw_info_tb->setChecked(false);
	show_raw_info_tb->blockSignals(false);

	raw_info_txt->setPlainText(e.getExceptionsText());

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
		item1->setText(0,ex->getFile() + QString(" (") + ex->getLine() + QString(")"));

		item2=new QTreeWidgetItem(item);
		item2->setIcon(0,QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
		item2->setText(0,Exception::getErrorCode(ex->getErrorType()) +
					   QString(" (") + QString("%1").arg(ex->getErrorType()) + QString(")"));

		item1=new QTreeWidgetItem(item);
		item1->setIcon(0,QPixmap(QString(":/icones/icones/msgbox_erro.png")));
		label=new QLabel;
		label->setWordWrap(true);
		label->setFont(font);
		label->setStyleSheet(QString("color: #ff0000;"));
		exceptions_trw->setItemWidget(item1, 0, label);
		label->setText(ex->getErrorMessage());

		if(!ex->getExtraInfo().isEmpty())
		{
			item1=new QTreeWidgetItem(item);
			item1->setIcon(0,QPixmap(QString(":/icones/icones/msgbox_info.png")));
			label=new QLabel;
			label->setWordWrap(true);
			label->setFont(font);
			label->setStyleSheet(QString("color: #000080;"));
			label->setTextInteractionFlags(Qt::TextSelectableByMouse);
			exceptions_trw->setItemWidget(item1, 0, label);
			label->setText(ex->getExtraInfo());
		}

		itr++;
		idx++;
	}

	if(msg.isEmpty())
		str_aux=PgModelerUiNS::formatMessage(e.getErrorMessage());
	else
		str_aux=PgModelerUiNS::formatMessage(msg);

	this->show(title, str_aux, icon_type, buttons, yes_lbl, no_lbl, cancel_lbl, yes_ico, no_ico, cancel_ico);
}

void Messagebox::show(const QString &msg, unsigned icon_type, unsigned buttons)
{
	this->show(QString(), msg,  icon_type, buttons);
}

void Messagebox::show(const QString &title, const QString &msg, unsigned icon_type, unsigned buttons, const QString &yes_lbl, const QString &no_lbl,
					  const QString &cancel_lbl, const QString &yes_ico, const QString &no_ico, const QString &cancel_ico)
{
	QString icon_name, aux_title=title;

	if(!yes_lbl.isEmpty())
		yes_ok_btn->setText(yes_lbl);
	else
		yes_ok_btn->setText(buttons==OK_BUTTON ? trUtf8("&Ok") : trUtf8("&Yes"));

	yes_ok_btn->setIcon(!yes_ico.isEmpty() ? QIcon(yes_ico) : QPixmap(QString(":/icones/icones/confirmar.png")));

	no_btn->setText(!no_lbl.isEmpty() ? no_lbl : trUtf8("&No"));
	no_btn->setIcon(!no_ico.isEmpty() ? QIcon(no_ico) : QPixmap(QString(":/icones/icones/fechar1.png")));

	cancel_btn->setText(!cancel_lbl.isEmpty() ? cancel_lbl : trUtf8("&Cancel"));
	cancel_btn->setIcon(!cancel_ico.isEmpty() ? QIcon(cancel_ico) : QPixmap(QString(":/icones/icones/cancelar.png")));

	no_btn->setVisible(buttons==YES_NO_BUTTONS || buttons==ALL_BUTTONS);
	cancel_btn->setVisible(buttons==OK_CANCEL_BUTTONS || buttons==ALL_BUTTONS);

	if(title.isEmpty())
	{
		switch(icon_type)
		{
			case ERROR_ICON:
				aux_title=trUtf8("Error");
			break;

			case ALERT_ICON:
				aux_title=trUtf8("Alert");
			break;

			case INFO_ICON:
				aux_title=trUtf8("Information");
			break;

			case CONFIRM_ICON:
				aux_title=trUtf8("Confirmation");
			break;
		}
	}

	switch(icon_type)
	{
		case ERROR_ICON:
			icon_name=QString("msgbox_erro.png");
		break;

		case INFO_ICON:
			icon_name=QString("msgbox_info.png");
		break;

		case ALERT_ICON:
			icon_name=QString("msgbox_alerta.png");
		break;

		case CONFIRM_ICON:
			icon_name=QString("msgbox_quest.png");
		break;

		default:
			icon_name=QString();
		break;
	}

	cancelled=false;
	icon_lbl->setVisible(!icon_name.isEmpty());

	if(!icon_name.isEmpty())
		icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + icon_name));

	msg_lbl->setText(msg);

	this->setWindowTitle(QString("pgModeler - ") + aux_title);
	this->objs_group_wgt->setCurrentIndex(0);
	this->show_errors_tb->setChecked(false);
	this->show_errors_tb->setVisible((exceptions_trw->topLevelItemCount() > 0));
	showExceptionList();

	this->resize(this->minimumWidth(), this->minimumHeight());

	QFontMetrics fm(msg_lbl->font());
	QString aux_msg=msg;
	aux_msg.replace(QRegExp(QString("(<)(br)(/)?(>)"), Qt::CaseInsensitive), QString("\n"));
	QSize size=QSize(msg_lbl->width(), fm.height() * (aux_msg.count('\n') + 1));
	int max_h=msg_lbl->minimumHeight() * 3;

	//Resizing the message box if the text height is greater than the default size
	if(size.height() > msg_lbl->minimumHeight() && size.height() < max_h)
		this->setMinimumHeight((size.height() + (size.height() * 0.25))  + show_raw_info_tb->height() + name_lbl->height() + 30);
	else if(size.height() >= max_h)
		this->setMinimumHeight(max_h);

	this->resize(this->minimumWidth(), this->minimumHeight());

	QDialog::exec();
}
