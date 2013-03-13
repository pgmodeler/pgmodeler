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
	if(button_conf==MessageBox::OK_CANCEL_BUTTONS)
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

