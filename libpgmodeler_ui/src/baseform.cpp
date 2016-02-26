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

void BaseForm::resizeToIdealSize(const QSize &ideal_size, int size_padding)
{
	QDialog::adjustSize();

	QSize size=this->size();
	int curr_h=size.height(),
			curr_w=size.width();

	if(ideal_size.isValid())
	{
		if(curr_h < ideal_size.height())
			curr_h = ideal_size.height();
		else if(curr_h > ideal_size.height() + size_padding)
			curr_h = ((ideal_size.height() * 2) + size_padding)/2;

		if(curr_w < ideal_size.width())
			curr_w = ideal_size.width();
		else if(curr_w > ideal_size.width() + size_padding)
			curr_w = ((ideal_size.width() * 2) + size_padding)/2;
	}

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

	QVBoxLayout *vbox=new QVBoxLayout;
	QScreen *screen=qApp->screens().at(0);
	QSize min_size=widget->minimumSize();
	int max_h = screen->size().height() * 0.67,
			max_w = screen->size().width() * 0.67,
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
		inc_w=scrollarea->verticalScrollBar()->width()/2;
	}
	else
	{
		vbox->addWidget(widget);
		widget->setParent(main_frm);
	}

	main_frm->setLayout(vbox);
	setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);

	QDialog::adjustSize();

	curr_h=this->height(),
	curr_w=this->width();

	if(curr_h > min_size.height() && min_size.height() < max_h)
		curr_h = min_size.height();
	else if(min_size.height() >= max_h)
		curr_h = max_h;

	if(curr_w > min_size.width() && min_size.width() < max_w)
		curr_w = min_size.width();
	else if(min_size.width() >= max_w)
		curr_w = max_w;

	curr_w += vbox->contentsMargins().left() +
						vbox->contentsMargins().right();

	curr_h += apply_ok_btn->height() +
						buttons_lt->contentsMargins().top() +
						buttons_lt->contentsMargins().bottom() +
						vbox->contentsMargins().top() +
						vbox->contentsMargins().bottom();

	this->setMinimumSize(curr_w, curr_h);
	this->resize(curr_w, curr_h);

	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(apply_ok_btn, SIGNAL(clicked(bool)), widget, SLOT(applyConfiguration()));
	connect(widget, SIGNAL(s_closeRequested()), this, SLOT(accept()));
}

void BaseForm::setMainWidget(QWidget *widget)
{
	if(!widget)	return;
	/*generalwidget_wgt->insertWidget(0, widget);
	generalwidget_wgt->setCurrentIndex(0);*/

	QVBoxLayout *hbox=new QVBoxLayout;
	hbox->addWidget(widget);
//	widget->setParent(main_wgt);

	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(apply_ok_btn, SIGNAL(clicked(bool)), this, SLOT(accept()));
}
