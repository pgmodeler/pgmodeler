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

#include "baseform.h"

BaseForm::BaseForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	/* Windows System workaround: permitting the windows subject to maximize shows
	the maximize button as well the editing dialogs stay on top of all other dialogs */
#ifdef Q_OS_WIN
	this->setWindowFlags(this->windowFlags() |
						 Qt::Dialog |
						 Qt::WindowMaximizeButtonHint);

	this->generalwidget_wgt->setFrameShape(QFrame::WinPanel);
#endif
}

void BaseForm::setButtonConfiguration(unsigned button_conf)
{
	if(button_conf==Messagebox::OK_CANCEL_BUTTONS)
	{
		apply_ok_btn->setText(trUtf8("&Apply"));
		cancel_btn->setVisible(true);
	}
	else
	{
		apply_ok_btn->setText(trUtf8("&Ok"));
		cancel_btn->setVisible(false);
	}
}

void BaseForm::adjustSize(const QSize &ideal_size, int size_padding)
{
	QDialog::adjustSize();

	QSize size=this->size();
	int curr_h=size.height(),
			curr_w=size.width();

	if(curr_h < ideal_size.height())
		curr_h = ideal_size.height();
	else if(curr_h > ideal_size.height() + size_padding)
		curr_h = ((ideal_size.height() * 2) + size_padding)/2;

	if(curr_w < ideal_size.width())
		curr_w = ideal_size.width();
	else if(curr_w > ideal_size.width() + size_padding)
		 curr_w = ((ideal_size.width() * 2) + size_padding)/2;

	this->setMinimumSize(curr_w, curr_h);
	this->resize(curr_w, curr_h);
}

void BaseForm::setMainWidget(BaseObjectWidget *widget)
{
	if(!widget)	return;

	setWindowTitle(trUtf8("%1 properties").arg(BaseObject::getTypeName(widget->getHandledObjectType())));
	generalwidget_wgt->insertWidget(0, widget);
  generalwidget_wgt->setCurrentWidget(widget);

	setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);
  this->adjustSize(widget->getIdealSize(), widget->getSizePadding());

	connect(apply_ok_btn, SIGNAL(clicked(bool)), widget, SLOT(applyConfiguration()));
	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(reject()));
  connect(apply_ok_btn, SIGNAL(clicked(bool)), this, SLOT(accept()));
}

void BaseForm::setMainWidget(QWidget *widget)
{
	if(!widget)	return;
	generalwidget_wgt->insertWidget(0, widget);
	generalwidget_wgt->setCurrentIndex(0);
}
