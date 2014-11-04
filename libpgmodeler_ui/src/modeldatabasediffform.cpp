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
  try
  {
    setupUi(this);

    import_helper=nullptr;
    diff_helper=nullptr;
    imported_model=nullptr;
    import_thread=diff_thread=nullptr;
    import_item=diff_item=export_item=nullptr;

    apply_on_server_ht=new HintTextWidget(apply_on_server_hint, this);
    apply_on_server_ht->setText(apply_on_server_rb->statusTip());

    store_in_file_ht=new HintTextWidget(store_in_file_hint, this);
    store_in_file_ht->setText(store_in_file_rb->statusTip());

    import_sys_objs_ht=new HintTextWidget(import_sys_objs_hint, this);
    import_sys_objs_ht->setText(import_sys_objs_chk->statusTip());

    import_ext_objs_ht=new HintTextWidget(import_ext_objs_hint, this);
    import_ext_objs_ht->setText(import_ext_objs_chk->statusTip());

    keep_cluster_objs_ht=new HintTextWidget(keep_cluster_objs_hint, this);
    keep_cluster_objs_ht->setText(keep_cluster_objs_chk->statusTip());

    trunc_tables_ht=new HintTextWidget(trunc_tables_hint, this);
    trunc_tables_ht->setText(trunc_tables_chk->statusTip());

    ignore_errors_ht=new HintTextWidget(ignore_errors_hint, this);
    ignore_errors_ht->setText(ignore_errors_chk->statusTip());

    force_recreation_ht=new HintTextWidget(force_recreation_hint, this);
    force_recreation_ht->setText(force_recreation_chk->statusTip());

    drop_cascade_ht=new HintTextWidget(drop_cascade_hint, this);
    drop_cascade_ht->setText(drop_cascade_chk->statusTip());

    sqlcode_hl=new SyntaxHighlighter(sqlcode_txt, false);

    connect(connect_tb, SIGNAL(clicked()), this, SLOT(listDatabases()));
    connect(store_in_file_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
    connect(apply_on_server_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
    connect(file_edt, SIGNAL(textChanged(QString)), this, SLOT(enableDiffMode()));
    connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableDiffMode()));
    connect(generate_btn, SIGNAL(clicked()), this, SLOT(generateDiff()));
    connect(close_btn, SIGNAL(clicked()), this, SLOT(close()));
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}

ModelDatabaseDiffForm::~ModelDatabaseDiffForm(void)
{
	destroyThreads();
}

void ModelDatabaseDiffForm::setDatabaseModel(DatabaseModel *model)
{
	source_model=model;
}

void ModelDatabaseDiffForm::showEvent(QShowEvent *)
{
	dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->fillConnectionsComboBox(connections_cmb);
	connections_cmb->setEnabled(connections_cmb->count() > 0);
	connection_lbl->setEnabled(connections_cmb->isEnabled());
	connect_tb->setEnabled(connections_cmb->isEnabled());
	enableDiffMode();
	settings_tbw->setTabEnabled(1, false);
}

void ModelDatabaseDiffForm::closeEvent(QCloseEvent *event)
{
	//Ignore the close event when the thread is running
	if((import_thread && import_thread->isRunning()) ||
		 (diff_thread && diff_thread->isRunning()))
		event->ignore();
}

void ModelDatabaseDiffForm::createThreads(void)
{
	import_thread=new QThread;
	import_helper=new DatabaseImportHelper;
	import_helper->moveToThread(import_thread);

	diff_thread=new QThread;
	diff_helper=new ModelsDiffHelper;
	diff_helper->moveToThread(diff_thread);

	connect(cancel_btn, &QToolButton::clicked, [=](){ import_helper->cancelImport(); diff_helper->cancelDiff(); });
	connect(import_thread, SIGNAL(started(void)), import_helper, SLOT(importDatabase()));
	connect(diff_thread, SIGNAL(started(void)), diff_helper, SLOT(diffModels()));

	connect(import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
	connect(import_helper, SIGNAL(s_importCanceled()), this, SLOT(handleOperationCanceled()));
	connect(import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	connect(import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)));

	connect(diff_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)));
	connect(diff_helper, SIGNAL(s_diffFinished()), this, SLOT(handleOperationFinished()));
	connect(diff_helper, SIGNAL(s_diffCanceled()), this, SLOT(handleOperationCanceled()));
	connect(diff_helper, SIGNAL(s_diffAborted(Exception)), this, SLOT(captureThreadError(Exception)));

	connect(diff_helper, SIGNAL(s_objectsDiffInfoGenerated(ObjectsDiffInfo)), this, SLOT(updateDiffInfo(ObjectsDiffInfo)));
}

void ModelDatabaseDiffForm::destroyThreads(void)
{
	if(import_thread && import_thread->isRunning())
	{
		import_thread->quit();
		import_thread->wait();
		delete(import_thread);
		import_thread=nullptr;
	}

	if(diff_thread && diff_thread->isRunning())
	{
		diff_thread->quit();
		diff_thread->wait();
		delete(diff_thread);
		diff_thread=nullptr;
	}

	delete(import_helper);
	delete(diff_helper);
	import_helper=nullptr;
	diff_helper=nullptr;
}

void ModelDatabaseDiffForm::destroyModel(void)
{
	if(imported_model)
		delete(imported_model);

	imported_model=nullptr;
}

void ModelDatabaseDiffForm::clearOutput(void)
{
	output_trw->clear();
	import_item=diff_item=export_item=nullptr;

	step_lbl->setText(trUtf8("Waiting process to start..."));
	step_ico_lbl->setPixmap(QPixmap());
	progress_lbl->setText(trUtf8("Waiting process to start..."));
	progress_ico_lbl->setPixmap(QPixmap());

	step_pb->setValue(0);
	progress_pb->setValue(0);

	create_cnt_lbl->setText("0");
	alter_cnt_lbl->setText("0");
	drop_cnt_lbl->setText("0");
}

QTreeWidgetItem *ModelDatabaseDiffForm::createOutputItem(const QString &text, const QPixmap &ico, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *item=nullptr;
	QLabel *label=new QLabel;

	item=new QTreeWidgetItem(parent);
	item->setIcon(0, ico);
	label->setText(text);

	if(!parent)
		output_trw->insertTopLevelItem(output_trw->topLevelItemCount(), item);

	output_trw->setItemWidget(item, 0, label);
	item->setExpanded(true);
	output_trw->scrollToItem(item);

	return(item);
}

void ModelDatabaseDiffForm::listDatabases(void)
{
	try
	{
		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		DatabaseImportHelper imp_helper;

		imp_helper.setConnection(*conn);
		DatabaseImportForm::listDatabases(imp_helper, true, database_cmb);

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

  generate_btn->setEnabled(database_cmb->currentIndex() > 0 &&
													 ((store_in_file_rb->isChecked() && !file_edt->text().isEmpty()) ||
														(apply_on_server_rb->isChecked())));
}

void ModelDatabaseDiffForm::generateDiff(void)
{
	clearOutput();
	createThreads();
	importDatabase();

	cancel_btn->setEnabled(true);
	generate_btn->setEnabled(false);

  alert_frm->setVisible(!force_recreation_chk->isChecked());
	settings_tbw->setTabEnabled(0, false);  
	settings_tbw->setTabEnabled(1, true);
  settings_tbw->setTabEnabled(2, false);
	settings_tbw->setCurrentIndex(1);
}

void ModelDatabaseDiffForm::importDatabase(void)
{
	try
	{
		Connection conn=(*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>())), conn1;
		map<ObjectType, vector<unsigned>> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;
		Catalog catalog;

		conn1=conn;
		step_lbl->setText(trUtf8("Importing database <strong>%1</strong>...").arg(database_cmb->currentText()));
		step_ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/import.png")));

		import_item=createOutputItem(step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);

		conn.switchToDatabase(database_cmb->currentText());
		catalog.setConnection(conn);
		catalog.setFilter(Catalog::LIST_ALL_OBJS | Catalog::EXCL_BUILTIN_ARRAY_TYPES |
											Catalog::EXCL_EXTENSION_OBJS | Catalog::EXCL_SYSTEM_OBJS);
		catalog.getObjectsOIDs(obj_oids, col_oids, {{ParsersAttributes::FILTER_TABLE_TYPES, "1"}});
		obj_oids[OBJ_DATABASE].push_back(database_cmb->currentData().value<unsigned>());

		imported_model=new DatabaseModel;
		imported_model->createSystemObjects(true);	

		import_helper->setConnection(conn1);
		import_helper->setSelectedOIDs(imported_model, obj_oids, col_oids);
		import_helper->setCurrentDatabase(database_cmb->currentText());
		import_helper->setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(), true, ignore_errors_chk->isChecked(), false, false);

		import_thread->start();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::diffModels(void)
{
	step_lbl->setText(trUtf8("Comparing the model <strong>%1</strong> and database <strong>%2</strong>...")
										.arg(source_model->getName())
										.arg(imported_model->getName()));
  step_ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/diff.png")));

	output_trw->collapseItem(import_item);
	diff_progress=step_pb->value();

	diff_item=createOutputItem(step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);
  diff_helper->setDiffOptions(keep_cluster_objs_chk->isChecked(), drop_cascade_chk->isChecked(),
                              force_recreation_chk->isChecked(), trunc_tables_chk->isChecked());
  diff_helper->setModels(source_model, imported_model);
	diff_thread->start();
}

void ModelDatabaseDiffForm::resetButtons(void)
{
	cancel_btn->setEnabled(false);
	generate_btn->setEnabled(true);
	settings_tbw->setTabEnabled(0, true);
}

void ModelDatabaseDiffForm::cancelOperation(void)
{
	destroyModel();
	destroyThreads();
	resetButtons();
}

void ModelDatabaseDiffForm::captureThreadError(Exception e)
{
	cancelOperation();
	progress_lbl->setText(trUtf8("Process aborted due to errors!"));
	progress_ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
	createOutputItem(progress_lbl->text(), *progress_ico_lbl->pixmap(), nullptr);

	throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void ModelDatabaseDiffForm::handleOperationCanceled(void)
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

	handleOperationFinished();
}

void ModelDatabaseDiffForm::handleOperationFinished(void)
{
	if(import_thread->isRunning())
	{
		import_thread->quit();
		import_thread->wait();
		step_pb->setValue(30);
		diffModels();
	}
	else if(diff_thread->isRunning())
	{
		diff_thread->quit();
		diff_thread->wait();
		resetButtons();

		destroyThreads();
	}
}

QString ModelDatabaseDiffForm::formatMessage(const QString &msg)
{
	QString fmt_msg=msg;

	fmt_msg.replace(fmt_msg.indexOf('`'), 1 ,"<strong>");
	fmt_msg.replace(fmt_msg.indexOf('\''), 1,"</strong>");
	fmt_msg.replace(fmt_msg.indexOf('`'), 1 ,"<em>");
	fmt_msg.replace(fmt_msg.indexOf('\''), 1,"</em>");

	return(fmt_msg);
}

void ModelDatabaseDiffForm::updateProgress(int progress, QString msg, ObjectType obj_type)
{
	QTreeWidgetItem *parent=nullptr;

	if(import_thread->isRunning())
	{
		if(progress > 90)
			step_pb->setValue(step_pb->value() + 5);

		parent=import_item;
	}
	else if(diff_thread->isRunning())
	{
		step_pb->setValue(diff_progress + (progress/2));
		parent=diff_item;
	}

	msg=formatMessage(msg);
	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=BASE_OBJECT)
		progress_ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")));
	else
		progress_ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));

	this->repaint();
}

void ModelDatabaseDiffForm::updateDiffInfo(ObjectsDiffInfo diff_info)
{
	map<unsigned, QLabel *> cnt_labels={ {ObjectsDiffInfo::CREATE_OBJECT, create_cnt_lbl},
																			 {ObjectsDiffInfo::DROP_OBJECT,   drop_cnt_lbl},
																			 {ObjectsDiffInfo::ALTER_OBJECT,  alter_cnt_lbl} };

	unsigned diff_type=diff_info.getDiffType();
	QLabel *lbl=cnt_labels[diff_type];

	createOutputItem(formatMessage(diff_info.getInfoMessage()),
									 QPixmap(QString(":/icones/icones/%1.png").arg(diff_info.getObject()->getSchemaName())) , diff_item);
	lbl->setText(QString::number(diff_helper->getDiffTypeCount(diff_type)));
}
