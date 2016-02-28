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
	this->setWindowFlags(this->windowFlags() ^ Qt::WindowContextHelpButtonHint);
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

void BaseForm::resizeForm(QWidget *widget)
{
	QVBoxLayout *vbox=new QVBoxLayout;
	QScreen *screen=qApp->screens().at(0);
	QSize min_size=widget->minimumSize();
	int max_h = screen->size().height() * 0.70,
			max_w = screen->size().width() * 0.70,
			inc_w = 0, curr_w =0, curr_h = 0;

	vbox->setContentsMargins(2,2,2,2);

	if(max_w < min_size.width() || max_h < min_size.height())
	{
		QScrollArea *scrollarea=nullptr;
		scrollarea=new QScrollArea(main_frm);
		scrollarea->setFrameShape(QFrame::NoFrame);
		scrollarea->setFrameShadow(QFrame::Plain);
		scrollarea->setWidget(widget);
		scrollarea->setWidgetResizable(true);
		widget->setParent(scrollarea);
		vbox->addWidget(scrollarea);
		inc_w=scrollarea->verticalScrollBar()->width();
	}
	else
	{
		vbox->addWidget(widget);
		widget->setParent(main_frm);
	}

	main_frm->setLayout(vbox);
	QDialog::adjustSize();

	curr_h=this->height(),
	curr_w=this->width();

	if(min_size.height() > 0 &&
					curr_h > min_size.height() && min_size.height() < max_h)
		curr_h = (curr_h + min_size.height())/2;
	else if(min_size.height() >= max_h)
		curr_h = max_h;

	if(min_size.width() > 0 &&
		 curr_w > min_size.width() && min_size.width() < max_w)
		curr_w = (curr_w + min_size.width())/2;
	else if(min_size.width() >= max_w)
		curr_w = max_w;

	curr_w += inc_w +
						((vbox->contentsMargins().left() +
							vbox->contentsMargins().right()) * 2);

	curr_h += apply_ok_btn->height() +
							((buttons_lt->contentsMargins().top() +
								buttons_lt->contentsMargins().bottom()) * 4);

	this->setMinimumSize(curr_w, curr_h);
	this->resize(curr_w, curr_h);
}

void BaseForm::setMainWidget(BaseObjectWidget *widget)
{
	if(!widget)	return;

	if(widget->getHandledObjectType()!=BASE_OBJECT && widget->windowTitle().isEmpty())
		setWindowTitle(trUtf8("%1 properties").arg(BaseObject::getTypeName(widget->getHandledObjectType())));
	else
		setWindowTitle(widget->windowTitle());

	resizeForm(widget);
	setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);

	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(apply_ok_btn, SIGNAL(clicked(bool)), widget, SLOT(applyConfiguration()));
	connect(widget, SIGNAL(s_closeRequested()), this, SLOT(accept()));
}

void BaseForm::setMainWidget(QWidget *widget)
{
	if(!widget)	return;

	setWindowTitle(widget->windowTitle());
	resizeForm(widget);
	setButtonConfiguration(Messagebox::OK_BUTTON);

	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(apply_ok_btn, SIGNAL(clicked(bool)), this, SLOT(accept()));
}
