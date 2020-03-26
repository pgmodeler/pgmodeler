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

#include "messagebox.h"
#include "pgmodeleruins.h"
#include "baseobjectview.h"

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
	connect(show_raw_info_tb,SIGNAL(toggled(bool)),this,SLOT(showExceptionList()));

	show_raw_info_tb->setVisible(false);
	error_show_btns_wgt->setVisible(false);
	custom_option_chk->setVisible(false);
}

void Messagebox::handleYesOkClick()
{
	exceptions_trw->clear();
	accept();
}

void Messagebox::handleNoCancelClick()
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

bool Messagebox::isCancelled()
{
	return cancelled;
}

void Messagebox::setCustomOptionText(const QString &text)
{
	custom_option_chk->setVisible(!text.isEmpty());
	custom_option_chk->setText(text);
}

bool Messagebox::isCustomOptionChecked()
{
	return custom_option_chk->isChecked();
}

void Messagebox::showExceptionList()
{
	if(show_errors_tb->isChecked())
	{
		show_errors_tb->setIcon(QPixmap(PgModelerUiNs::getIconPath("desfazer")));

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
		show_errors_tb->setIcon(QPixmap(PgModelerUiNs::getIconPath("refazer")));
		objs_group_wgt->setCurrentIndex(0);
	}
}

void Messagebox::show(Exception e, const QString &msg, unsigned icon_type, unsigned buttons, const QString &yes_lbl, const QString &no_lbl, const QString &cancel_lbl,
					  const QString &yes_ico, const QString &no_ico, const QString &cancel_ico)
{
	QString str_aux, title;

	show_raw_info_tb->blockSignals(true);
	show_raw_info_tb->setChecked(false);
	show_raw_info_tb->blockSignals(false);

	raw_info_txt->setPlainText(e.getExceptionsText());
	PgModelerUiNs::createExceptionsTree(exceptions_trw, e, nullptr);
	exceptions_trw->expandAll();
	exceptions_trw->scrollToTop();

	if(msg.isEmpty())
		str_aux=PgModelerUiNs::formatMessage(e.getErrorMessage());
	else
		str_aux=PgModelerUiNs::formatMessage(msg);

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
		yes_ok_btn->setText(buttons==OkButton ? tr("&Ok") : tr("&Yes"));

	yes_ok_btn->setIcon(!yes_ico.isEmpty() ? QIcon(yes_ico) : QPixmap(PgModelerUiNs::getIconPath("confirmar")));

	no_btn->setText(!no_lbl.isEmpty() ? no_lbl : tr("&No"));
	no_btn->setIcon(!no_ico.isEmpty() ? QIcon(no_ico) : QPixmap(PgModelerUiNs::getIconPath("fechar1")));

	cancel_btn->setText(!cancel_lbl.isEmpty() ? cancel_lbl : tr("&Cancel"));
	cancel_btn->setIcon(!cancel_ico.isEmpty() ? QIcon(cancel_ico) : QPixmap(PgModelerUiNs::getIconPath("cancelar")));

	no_btn->setVisible(buttons==YesNoButtons || buttons==AllButtons);
	cancel_btn->setVisible(buttons==OkCancelButtons || buttons==AllButtons);

	if(title.isEmpty())
	{
		switch(icon_type)
		{
			case ErrorIcon:
				aux_title=tr("Error");
			break;

			case AlertIcon:
				aux_title=tr("Alert");
			break;

			case InfoIcon:
				aux_title=tr("Information");
			break;

			case ConfirmIcon:
				aux_title=tr("Confirmation");
			break;
		}
	}

	switch(icon_type)
	{
		case ErrorIcon:
			icon_name=QString("msgbox_erro");
		break;

		case InfoIcon:
			icon_name=QString("msgbox_info");
		break;

		case AlertIcon:
			icon_name=QString("msgbox_alerta");
		break;

		case ConfirmIcon:
			icon_name=QString("msgbox_quest");
		break;

		default:
			icon_name=QString();
		break;
	}

	cancelled=false;
	icon_lbl->setVisible(!icon_name.isEmpty());

	if(!icon_name.isEmpty())
		icon_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath(icon_name)));

	msg_lbl->setText(msg);

	this->setWindowTitle(aux_title);
	this->objs_group_wgt->setCurrentIndex(0);
	this->show_errors_tb->setChecked(false);
	error_show_btns_wgt->setVisible((exceptions_trw->topLevelItemCount() > 0));
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

	double factor = BaseObjectView::getScreenDpiFactor();
	this->resize(this->minimumWidth() * factor, this->minimumHeight() * factor);

	QDialog::exec();
}
