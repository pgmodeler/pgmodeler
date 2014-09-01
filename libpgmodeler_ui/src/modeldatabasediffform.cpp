/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "modeldatabasediffform.h"
#include "configurationform.h"
#include "databaseimportform.h"

extern ConfigurationForm *configuration_form;

ModelDatabaseDiffForm::ModelDatabaseDiffForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	import_thread=new QThread(this);
	import_helper.moveToThread(import_thread);
	dbmodel=nullptr;

	connect(connect_tb, SIGNAL(clicked()), this, SLOT(listDatabases()));
	connect(store_in_file_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
	connect(apply_on_server_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
	connect(file_edt, SIGNAL(textChanged(QString)), this, SLOT(enableDiffMode()));
	connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableDiffMode()));

	connect(import_thread, SIGNAL(started(void)), &import_helper, SLOT(importDatabase(void)));
	connect(import_thread, &QThread::started, [=](){ import_thread->setPriority(QThread::HighPriority); });

	connect(&import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
	connect(&import_helper, SIGNAL(s_importCanceled(void)), this, SLOT(handleImportCanceled(void)));
	connect(&import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	connect(&import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)));

	connect(generate_btn, SIGNAL(clicked()), this, SLOT(generateDiff()));
	connect(cancel_btn, &QToolButton::clicked, [=](){ import_helper.cancelImport(); });
	connect(close_btn, SIGNAL(clicked()), this, SLOT(close()));
}

void ModelDatabaseDiffForm::showEvent(QShowEvent *)
{
	dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->fillConnectionsComboBox(connections_cmb);
	hideProgress(true);
	enableDiffMode();
}

void ModelDatabaseDiffForm::closeEvent(QCloseEvent *event)
{
	//Ignore the close event when the thread is running
	if(import_thread->isRunning())
		event->ignore();
}

void ModelDatabaseDiffForm::destroyModel(void)
{
	if(dbmodel)
		delete(dbmodel);

	dbmodel=nullptr;
}

void ModelDatabaseDiffForm::hideProgress(bool hide)
{
	if((hide && progress_parent_wgt->isVisible()) ||
		 (!hide && !progress_parent_wgt->isVisible()))
	{
		progress_parent_wgt->setHidden(hide);

		if(hide)
			this->setMinimumHeight(this->minimumHeight() - progress_parent_wgt->height());
		else
			this->setMinimumHeight(this->minimumHeight() + progress_parent_wgt->height());

		this->resize(this->minimumSize());
	}

	connections_cmb->setEnabled(connections_cmb->count() > 0);
	connection_lbl->setEnabled(connections_cmb->isEnabled());
	connect_tb->setEnabled(connections_cmb->isEnabled());
}

void ModelDatabaseDiffForm::listDatabases(void)
{
	try
	{
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

		import_helper.closeConnection();
		import_helper.setConnection(*conn);
		DatabaseImportForm::listDatabases(import_helper, true, database_cmb);

		database_cmb->setEnabled(database_cmb->count() > 0);
		database_lbl->setEnabled(database_cmb->isEnabled());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::enableDiffMode(void)
{
	store_in_file_wgt->setEnabled(store_in_file_rb->isChecked());
	apply_on_server_wgt->setEnabled(apply_on_server_rb->isChecked());

	generate_btn->setEnabled(database_cmb->currentIndex() > 0 &&
													 ((store_in_file_rb->isChecked() && !file_edt->text().isEmpty()) ||
														(apply_on_server_rb->isChecked())));
}

void ModelDatabaseDiffForm::generateDiff(void)
{
	step_lbl->setText(trUtf8("Waiting process to start..."));
	progress_lbl->setText(trUtf8("Waiting process to start..."));
	step_pb->setValue(0);
	progress_pb->setValue(0);

	importDatabase();
	cancel_btn->setEnabled(true);
	hideProgress(false);
}

void ModelDatabaseDiffForm::importDatabase(void)
{
	try
	{
		Connection conn=(*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()));
		map<ObjectType, vector<unsigned>> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;
		Catalog catalog;

		step_lbl->setText(trUtf8("Importing database <strong>%1</strong>...").arg(database_cmb->currentText()));
		step_ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/import.png")));

		conn.switchToDatabase(database_cmb->currentText());
		catalog.setConnection(conn);
		catalog.setFilter(Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES |
											Catalog::EXCL_EXTENSION_OBJS | Catalog::EXCL_SYSTEM_OBJS);
		catalog.getObjectsOIDs(obj_oids, col_oids);

		dbmodel=new ModelWidget;
		dbmodel->getDatabaseModel()->createSystemObjects(true);

		import_helper.setSelectedOIDs(dbmodel->getDatabaseModel(), obj_oids, col_oids);
		import_helper.setCurrentDatabase(database_cmb->currentText());
		import_helper.setImportOptions(import_sys_objs_chk->isChecked(), false, true, ignore_errors_chk->isChecked(), false, false);

		import_thread->start();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::cancelOperation(void)
{
	destroyModel();
	import_thread->quit();
	cancel_btn->setEnabled(false);
}

void ModelDatabaseDiffForm::captureThreadError(Exception e)
{
	cancelOperation();
	step_lbl->setText(trUtf8("Process aborted due to errors!"));
	ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
	throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void ModelDatabaseDiffForm::handleImportCanceled(void)
{
	cancelOperation();
}

void ModelDatabaseDiffForm::handleImportFinished(Exception e)
{
	if(!e.getErrorMessage().isEmpty())
	{
		Messagebox msgbox;
		msgbox.show(e, e.getErrorMessage(), Messagebox::ALERT_ICON);
	}

	step_lbl->setText(trUtf8("Comparing the model and database!"));
	step_ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/sync.png")));
	import_thread->quit();
}

void ModelDatabaseDiffForm::updateProgress(int progress, QString msg, ObjectType obj_type)
{
	if(progress > 90 && import_thread->isRunning())
		step_pb->setValue(step_pb->value() + 5);

	msg.replace(msg.indexOf('`'), 1 ,"<strong>");
	msg.replace(msg.indexOf('\''), 1,"</strong>");
	msg.replace(msg.indexOf('`'), 1 ,"<em>");
	msg.replace(msg.indexOf('\''), 1,"</em>");
	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=BASE_OBJECT)
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")));
	else
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));

	this->repaint();
}
