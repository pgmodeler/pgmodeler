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

#include "databaseimportform.h"
#include "taskprogresswidget.h"
#include "configurationform.h"

extern ConfigurationForm *configuration_form;

DatabaseImportForm::DatabaseImportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
}

void DatabaseImportForm::updateProgress(int progress, QString msg)
{
 progress_lbl->setText(msg);
 progress_pb->setValue(progress);
 this->repaint();
}

void DatabaseImportForm::importDatabase(void)
{
	try
	{

	}
	catch(Exception &e)
	{

	}
}

void DatabaseImportForm::hideProgress(void)
{
	ln2_frm->setVisible(false);
	progress_lbl->setVisible(false);
	progress_pb->setVisible(false);
	ico_lbl->setVisible(false);
	this->resize(this->minimumSize());
}
