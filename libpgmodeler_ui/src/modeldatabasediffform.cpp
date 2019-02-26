/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "pgmodeleruins.h"

ModelDatabaseDiffForm::ModelDatabaseDiffForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	try
	{
		setupUi(this);
		sqlcode_txt=PgModelerUiNs::createNumberedTextEditor(sqlcode_wgt);
		sqlcode_txt->setReadOnly(true);

		htmlitem_del=new HtmlItemDelegate(this);
		output_trw->setItemDelegateForColumn(0, htmlitem_del);

		imported_model=loaded_model=source_model=nullptr;
		src_import_helper=import_helper=nullptr;
		diff_helper=nullptr;
		export_helper=nullptr;
		src_import_thread=import_thread=diff_thread=export_thread=nullptr;
		src_import_item=import_item=diff_item=export_item=nullptr;
		export_conn=nullptr;
		process_paused=false;
		diff_progress=curr_step=total_steps=0;

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

		recreate_unmod_ht=new HintTextWidget(recreate_unmod_hint, this);
		recreate_unmod_ht->setText("Recreates only objects that can't be changed through ALTER commands according to pgModeler implementation not the PostgreSQL one.\
								   Currently, those objects are:<br/><br/>aggregate, cast, constraint, collation, conversion, language, operator, operator class, operator family, rule, trigger and view.");

		cascade_mode_ht=new HintTextWidget(drop_cascade_hint, this);
		cascade_mode_ht->setText(cascade_mode_chk->statusTip());

		pgsql_ver_ht=new HintTextWidget(pgsql_ver_hint, this);
		pgsql_ver_ht->setText(pgsql_ver_chk->statusTip());

		keep_obj_perms_ht=new HintTextWidget(keep_obj_perms_hint, this);
		keep_obj_perms_ht->setText(keep_obj_perms_chk->statusTip());

		ignore_duplic_ht=new HintTextWidget(ignore_duplic_hint, this);
		ignore_duplic_ht->setText(ignore_duplic_chk->statusTip());

		reuse_sequences_ht=new HintTextWidget(reuse_sequences_hint, this);
		reuse_sequences_ht->setText(reuse_sequences_chk->statusTip());

		preserve_db_name_ht=new HintTextWidget(preserve_db_name_hint, this);
		preserve_db_name_ht->setText(preserve_db_name_chk->statusTip());

		dont_drop_missing_objs_ht=new HintTextWidget(dont_drop_missing_objs_hint, this);
		dont_drop_missing_objs_ht->setText(dont_drop_missing_objs_chk->statusTip());

		drop_missing_cols_constr_ht=new HintTextWidget(drop_missing_cols_constr_hint, this);
		drop_missing_cols_constr_ht->setText(drop_missing_cols_constr_chk->statusTip());

		ignore_error_codes_ht=new HintTextWidget(ignore_extra_errors_hint, this);
		ignore_error_codes_ht->setText(ignore_error_codes_chk->statusTip());

		sqlcode_hl=new SyntaxHighlighter(sqlcode_txt);
		sqlcode_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

		pgsql_ver_cmb->addItems(PgSqlVersions::AllVersions);

		PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MediumFontFactor);

		connect(cancel_btn, &QToolButton::clicked, [&](){ cancelOperation(true); });
		connect(pgsql_ver_chk, SIGNAL(toggled(bool)), pgsql_ver_cmb, SLOT(setEnabled(bool)));
		connect(connections_cmb, SIGNAL(activated(int)), this, SLOT(listDatabases()));
		connect(store_in_file_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
		connect(apply_on_server_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
		connect(file_edt, SIGNAL(textChanged(QString)), this, SLOT(enableDiffMode()));
		connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableDiffMode()));
		connect(generate_btn, SIGNAL(clicked()), this, SLOT(generateDiff()));
		connect(close_btn, SIGNAL(clicked()), this, SLOT(close()));
		connect(store_in_file_rb, SIGNAL(clicked(bool)), store_in_file_wgt, SLOT(setEnabled(bool)));
		connect(select_file_tb, SIGNAL(clicked()), this, SLOT(selectOutputFile()));
		connect(file_edt, SIGNAL(textChanged(QString)), this, SLOT(enableDiffMode()));
		connect(force_recreation_chk, SIGNAL(toggled(bool)), recreate_unmod_chk, SLOT(setEnabled(bool)));
		connect(dont_drop_missing_objs_chk, SIGNAL(toggled(bool)), drop_missing_cols_constr_chk, SLOT(setEnabled(bool)));
		connect(create_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(drop_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(alter_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(ignore_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(ignore_error_codes_chk, SIGNAL(toggled(bool)), error_codes_edt, SLOT(setEnabled(bool)));
		connect(src_database_rb, SIGNAL(toggled(bool)), src_database_wgt, SLOT(setEnabled(bool)));
		connect(src_model_rb, SIGNAL(toggled(bool)), src_model_name_lbl, SLOT(setEnabled(bool)));
		connect(src_connections_cmb, SIGNAL(activated(int)), this, SLOT(listDatabases()));
		connect(src_database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableDiffMode()));
		connect(src_model_rb, SIGNAL(toggled(bool)), this, SLOT(enableDiffMode()));
		connect(src_database_rb, SIGNAL(toggled(bool)), this, SLOT(enableDiffMode()));

#ifdef DEMO_VERSION
	#warning "DEMO VERSION: forcing ignore errors in diff due to the object count limit."
	ignore_errors_chk->setChecked(true);
	ignore_errors_chk->setEnabled(false);

	ignore_error_codes_chk->setChecked(false);
	ignore_error_codes_chk->setEnabled(false);

	apply_on_server_rb->setChecked(false);
	apply_on_server_rb->setEnabled(false);
#endif
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

ModelDatabaseDiffForm::~ModelDatabaseDiffForm(void)
{
	destroyThread(ImportThread);
	destroyThread(DiffThread);
	destroyThread(ExportThread);
	destroyModel();
}

void ModelDatabaseDiffForm::setModelWidget(ModelWidget *model_wgt)
{
	if(model_wgt)
	{
		source_model=loaded_model=model_wgt->getDatabaseModel();
		src_model_name_lbl->setText(QString("%1 [%2]").arg(source_model->getName()).arg(QFileInfo(model_wgt->getFilename()).fileName()));
		src_model_name_lbl->setToolTip(model_wgt->getFilename().isEmpty() ? trUtf8("model not saved yet") : model_wgt->getFilename());
	}
	else
	{
		src_model_name_lbl->setText(trUtf8("(none)"));
		src_model_name_lbl->setToolTip("");
		src_database_rb->setChecked(true);
		src_model_rb->setEnabled(false);
	}
}

void ModelDatabaseDiffForm::resetForm(void)
{
	ConnectionsConfigWidget::fillConnectionsComboBox(src_connections_cmb, true);
	src_connections_cmb->setEnabled(src_connections_cmb->count() > 0);
	src_connection_lbl->setEnabled(src_connections_cmb->isEnabled());

	ConnectionsConfigWidget::fillConnectionsComboBox(connections_cmb, true, Connection::OpDiff);
	connections_cmb->setEnabled(connections_cmb->count() > 0);
	connection_lbl->setEnabled(connections_cmb->isEnabled());

	enableDiffMode();
	settings_tbw->setTabEnabled(1, false);
	settings_tbw->setTabEnabled(2, false);
}

void ModelDatabaseDiffForm::closeEvent(QCloseEvent *event)
{
	//Ignore the close event when the thread is running
	if(process_paused ||
			(import_thread && import_thread->isRunning()) ||
			(diff_thread && diff_thread->isRunning()) ||
			(export_thread && export_thread->isRunning()))
		event->ignore();
}

void ModelDatabaseDiffForm::showEvent(QShowEvent *)
{
	resetForm();

	if(connections_cmb->currentIndex() > 0)
		listDatabases();
}

void ModelDatabaseDiffForm::createThread(unsigned thread_id)
{
	if(thread_id==SrcImportThread)
	{
		src_import_thread=new QThread;
		src_import_helper=new DatabaseImportHelper;
		src_import_helper->moveToThread(src_import_thread);

		output_trw->setUniformRowHeights(true);

		connect(src_import_thread, SIGNAL(started(void)), src_import_helper, SLOT(importDatabase()));
		connect(src_import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)), Qt::BlockingQueuedConnection);
		connect(src_import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
		connect(src_import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	}
	else if(thread_id==ImportThread)
	{
		import_thread=new QThread;
		import_helper=new DatabaseImportHelper;
		import_helper->moveToThread(import_thread);

		output_trw->setUniformRowHeights(true);

		connect(import_thread, SIGNAL(started(void)), import_helper, SLOT(importDatabase()));
		connect(import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)), Qt::BlockingQueuedConnection);
		connect(import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
		connect(import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	}
	else if(thread_id==DiffThread)
	{
		diff_thread=new QThread;
		diff_helper=new ModelsDiffHelper;
		diff_helper->moveToThread(diff_thread);

		connect(diff_thread, SIGNAL(started(void)), diff_helper, SLOT(diffModels()));
		connect(diff_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)));
		connect(diff_helper, SIGNAL(s_diffFinished()), this, SLOT(handleDiffFinished()));
		connect(diff_helper, SIGNAL(s_diffAborted(Exception)), this, SLOT(captureThreadError(Exception)));
		connect(diff_helper, SIGNAL(s_objectsDiffInfoGenerated(ObjectsDiffInfo)), this, SLOT(updateDiffInfo(ObjectsDiffInfo)), Qt::BlockingQueuedConnection);
	}
	else
	{
		export_thread=new QThread;
		export_helper=new ModelExportHelper;
		export_helper->setIgnoredErrors({ QString("0A000") });
		export_helper->moveToThread(export_thread);

		connect(apply_on_server_btn, &QPushButton::clicked,
			[&](){
						apply_on_server_btn->setEnabled(false);
						if(!export_thread->isRunning())
							exportDiff(false);
			});

		connect(export_thread, SIGNAL(started(void)), export_helper, SLOT(exportToDBMS()));

		connect(export_thread, &QThread::finished, [&](){
			output_trw->setUniformRowHeights(false);
		});

		connect(export_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType,QString)), this, SLOT(updateProgress(int,QString,ObjectType,QString)), Qt::BlockingQueuedConnection);
		connect(export_helper, SIGNAL(s_errorIgnored(QString,QString, QString)), this, SLOT(handleErrorIgnored(QString,QString,QString)));
		connect(export_helper, SIGNAL(s_exportFinished()), this, SLOT(handleExportFinished()));
		connect(export_helper, SIGNAL(s_exportAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	}
}

void ModelDatabaseDiffForm::destroyThread(unsigned thread_id)
{
	if(thread_id==SrcImportThread && src_import_thread)
	{
		delete(src_import_thread);
		delete(src_import_helper);
		src_import_thread=nullptr;
		src_import_helper=nullptr;
	}
	else if(thread_id==ImportThread && import_thread)
	{
		delete(import_thread);
		delete(import_helper);
		import_thread=nullptr;
		import_helper=nullptr;
	}
	else if(thread_id==DiffThread && diff_thread)
	{
		diff_thread=nullptr;
		diff_helper=nullptr;
		delete(diff_thread);
		delete(diff_helper);
	}
	else if(export_thread)
	{
		if(export_conn)
		{
			delete(export_conn);
			export_conn=nullptr;
		}

		delete(export_thread);
		delete(export_helper);
		export_thread=nullptr;
		export_helper=nullptr;
	}
}

void ModelDatabaseDiffForm::destroyModel(void)
{
	if(imported_model)
		delete(imported_model);

	if(source_model && source_model != loaded_model && src_database_rb->isChecked())
	{
		delete(source_model);
		source_model = nullptr;
	}

	imported_model=nullptr;
}

void ModelDatabaseDiffForm::clearOutput(void)
{
	output_trw->clear();
	src_import_item=import_item=diff_item=export_item=nullptr;

	step_lbl->setText(trUtf8("Waiting process to start..."));
	step_ico_lbl->setPixmap(QPixmap());
	progress_lbl->setText(trUtf8("Waiting process to start..."));
	progress_ico_lbl->setPixmap(QPixmap());

	step_pb->setValue(0);
	progress_pb->setValue(0);

	create_tb->setText(QString("0"));
	alter_tb->setText(QString("0"));
	drop_tb->setText(QString("0"));
	ignore_tb->setText(QString("0"));
}

void ModelDatabaseDiffForm::listDatabases(void)
{
	QComboBox *conn_cmb = (sender() == src_connections_cmb ? src_connections_cmb : connections_cmb),
			*db_cmb = (conn_cmb == src_connections_cmb ? src_database_cmb : database_cmb);
	QLabel *db_lbl = (conn_cmb == src_connections_cmb ? src_database_lbl : database_lbl);

	try
	{
		if(conn_cmb->currentIndex()==conn_cmb->count()-1 &&
			 ConnectionsConfigWidget::openConnectionsConfiguration(conn_cmb, true))
		{
			emit s_connectionsUpdateRequest();
			resetForm();
		}

		Connection *conn=reinterpret_cast<Connection *>(conn_cmb->itemData(conn_cmb->currentIndex()).value<void *>());

		if(conn)
		{
			DatabaseImportHelper imp_helper;
			imp_helper.setConnection(*conn);
			DatabaseImportForm::listDatabases(imp_helper, db_cmb);
		}
		else
			db_cmb->clear();

		db_cmb->setEnabled(db_cmb->count() > 0);
		db_lbl->setEnabled(db_cmb->isEnabled());
	}
	catch(Exception &e)
	{
		db_cmb->clear();
		db_cmb->setEnabled(false);
		db_lbl->setEnabled(false);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::enableDiffMode(void)
{
	store_in_file_wgt->setEnabled(store_in_file_rb->isChecked());

	generate_btn->setEnabled(database_cmb->currentIndex() > 0 &&
													 ((src_database_rb->isChecked() && src_database_cmb->currentIndex() > 0) ||
														(src_model_rb->isChecked() && loaded_model)) &&
													 ((store_in_file_rb->isChecked() && !file_edt->text().isEmpty()) ||
														(apply_on_server_rb->isChecked())));
}

void ModelDatabaseDiffForm::generateDiff(void)
{
	//Destroy previously allocated threads and helper before start over.
	destroyModel();
	destroyThread(SrcImportThread);
	destroyThread(ImportThread);
	destroyThread(DiffThread);
	destroyThread(ExportThread);

	clearOutput();
	curr_step = 1;

	if(src_model_rb->isChecked())
	{
		source_model = loaded_model;
		total_steps=3;
	}
	else
		total_steps=4;

	importDatabase(src_database_rb->isChecked() ? SrcImportThread : ImportThread);

	buttons_wgt->setEnabled(false);
	cancel_btn->setEnabled(true);
	generate_btn->setEnabled(false);

	settings_tbw->setTabEnabled(0, false);
	settings_tbw->setTabEnabled(1, true);
	settings_tbw->setTabEnabled(2, false);
	settings_tbw->setCurrentIndex(1);
}

void ModelDatabaseDiffForm::importDatabase(unsigned thread_id)
{
	try
	{
		if(thread_id != SrcImportThread && thread_id != ImportThread)
			throw Exception(ErrorCode::AllocationObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		createThread(thread_id);

		QThread *thread = (thread_id == SrcImportThread ? src_import_thread : import_thread);
		DatabaseImportHelper *import_hlp = (thread_id == SrcImportThread ? src_import_helper : import_helper);
		QComboBox *conn_cmb = (thread_id == SrcImportThread ? src_connections_cmb : connections_cmb),
				*db_cmb = (thread_id == SrcImportThread ? src_database_cmb : database_cmb);
		Connection conn=(*reinterpret_cast<Connection *>(conn_cmb->itemData(conn_cmb->currentIndex()).value<void *>())), conn1;
		map<ObjectType, vector<unsigned>> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;
		Catalog catalog;
		DatabaseModel *db_model = nullptr;

		conn1=conn;
		step_lbl->setText(trUtf8("Step %1/%2: Importing database <strong>%3</strong>...")
											.arg(curr_step)
											.arg(total_steps)
											.arg(db_cmb->currentText()));

		step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("import")));

		if(thread_id == SrcImportThread)
			src_import_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);
		else
			import_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);

		conn.switchToDatabase(db_cmb->currentText());
		pgsql_ver=conn.getPgSQLVersion(true);

		catalog.setConnection(conn);

		//The import process will exclude built-in array array types, system and extension objects
		catalog.setFilter(Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes |
						  Catalog::ExclExtensionObjs | Catalog::ExclSystemObjs);
		catalog.getObjectsOIDs(obj_oids, col_oids, {{Attributes::FilterTableTypes, Attributes::True}});
		obj_oids[ObjectType::Database].push_back(db_cmb->currentData().value<unsigned>());

		if(thread_id == SrcImportThread)
		{
			source_model=new DatabaseModel;
			source_model->createSystemObjects(true);
			db_model = source_model;
		}
		else
		{
			imported_model=new DatabaseModel;
			imported_model->createSystemObjects(true);
			db_model = imported_model;
		}

		import_hlp->setConnection(conn1);
		import_hlp->setSelectedOIDs(db_model, obj_oids, col_oids);
		import_hlp->setCurrentDatabase(db_cmb->currentText());
		import_hlp->setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(), true,
										ignore_errors_chk->isChecked(), false, false, false);
		thread->start();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::diffModels(void)
{
	createThread(DiffThread);

	step_lbl->setText(trUtf8("Step %1/%2: Comparing <strong>%3</strong> and <strong>%4</strong>...")
						.arg(curr_step)
						.arg(total_steps)
					  .arg(source_model->getName())
					  .arg(imported_model->getName()));
	step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("diff")));

	output_trw->collapseItem(import_item);
	diff_progress=step_pb->value();

	diff_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);

	diff_helper->setDiffOption(ModelsDiffHelper::OptKeepClusterObjs, keep_cluster_objs_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptCascadeMode, cascade_mode_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptTruncateTables, trunc_tables_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptForceRecreation, force_recreation_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptRecreateUnchangeble, recreate_unmod_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptKeepObjectPerms, keep_obj_perms_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptReuseSequences, reuse_sequences_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptPreserveDbName, preserve_db_name_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptDontDropMissingObjs, dont_drop_missing_objs_chk->isChecked());
	diff_helper->setDiffOption(ModelsDiffHelper::OptDropMissingColsConstr, drop_missing_cols_constr_chk->isChecked());

	diff_helper->setModels(source_model, imported_model);

	if(pgsql_ver_chk->isChecked())
		diff_helper->setPgSQLVersion(pgsql_ver_cmb->currentText());
	else
		diff_helper->setPgSQLVersion(pgsql_ver);

	diff_thread->start();
}

void ModelDatabaseDiffForm::exportDiff(bool confirm)
{
	createThread(ExportThread);

	Messagebox msg_box;

	if(confirm)
		msg_box.show(trUtf8("Confirmation"),
					 trUtf8(" <strong>WARNING:</strong> The generated diff is ready to be exported! Once started this process will cause irreversible changes on the database. Do you really want to proceed?"),
					 Messagebox::AlertIcon, Messagebox::AllButtons,
					 trUtf8("Apply diff"), trUtf8("Preview diff"), QString(),
					 PgModelerUiNs::getIconPath("diff"), PgModelerUiNs::getIconPath("codigosql"));

	if(!confirm || msg_box.result()==QDialog::Accepted)
	{
		settings_tbw->setCurrentIndex(1);
		apply_on_server_btn->setEnabled(true);

		step_lbl->setText(trUtf8("Step %1/%2: Exporting diff to database <strong>%3</strong>...")
											.arg(curr_step)
											.arg(total_steps)
											.arg(imported_model->getName()));
		step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("exportar")));

		output_trw->collapseItem(diff_item);
		diff_progress=step_pb->value();
		export_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);

		export_conn=new Connection;
		*export_conn=*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		export_helper->setExportToDBMSParams(sqlcode_txt->toPlainText(), export_conn,
																				 database_cmb->currentText(), ignore_duplic_chk->isChecked());
		if(ignore_error_codes_chk->isChecked())
			export_helper->setIgnoredErrors(error_codes_edt->text().simplified().split(' '));

		export_thread->start();
	}
	else if(msg_box.isCancelled())
		cancelOperation(true);
	else
	{
		process_paused=true;
		settings_tbw->setCurrentIndex(2);
		apply_on_server_btn->setVisible(true);
		output_trw->collapseItem(diff_item);
		PgModelerUiNs::createOutputTreeItem(output_trw,
											trUtf8("Diff process paused. Waiting user action..."),
											QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), nullptr);
	}
}

void ModelDatabaseDiffForm::filterDiffInfos(void)
{
	QToolButton *btn=dynamic_cast<QToolButton *>(sender());
	map<QToolButton *, unsigned> diff_types={ {create_tb, ObjectsDiffInfo::CreateObject},
											  {drop_tb, ObjectsDiffInfo::DropObject},
											  {alter_tb, ObjectsDiffInfo::AlterObject},
											  {ignore_tb, ObjectsDiffInfo::IgnoreObject}};

	for(int i=0; i < diff_item->childCount(); i++)
	{
		if(diff_item->child(i)->data(0, Qt::UserRole).toUInt()==diff_types[btn])
			output_trw->setItemHidden(diff_item->child(i), !btn->isChecked());
	}
}

void ModelDatabaseDiffForm::resetButtons(void)
{
	buttons_wgt->setEnabled(true);
	cancel_btn->setEnabled(false);
	settings_tbw->setTabEnabled(0, true);
	apply_on_server_btn->setVisible(false);
	enableDiffMode();
}

void ModelDatabaseDiffForm::saveDiffToFile(void)
{
	if(!sqlcode_txt->toPlainText().isEmpty())
	{
		QFile output;

		step_lbl->setText(trUtf8("Saving diff to file <strong>%1</strong>").arg(file_edt->text()));
		step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("salvar")));
		import_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);
		step_pb->setValue(90);
		progress_pb->setValue(100);

		output.setFileName(file_edt->text());

		if(!output.open(QFile::WriteOnly))
			captureThreadError(Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(file_edt->text()),
																	 ErrorCode::FileDirectoryNotWritten, __PRETTY_FUNCTION__,__FILE__,__LINE__));

		output.write(sqlcode_txt->toPlainText().toUtf8());
		output.close();
	}

	finishDiff();
}

void ModelDatabaseDiffForm::finishDiff(void)
{
	cancelOperation(false);

	step_lbl->setText(trUtf8("Diff process sucessfully ended!"));
	progress_lbl->setText(trUtf8("No operations left."));

	step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));
	progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));

	import_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);
	step_pb->setValue(100);
	progress_pb->setValue(100);
}

void ModelDatabaseDiffForm::cancelOperation(bool cancel_by_user)
{
	if(cancel_by_user)
	{
		step_lbl->setText(trUtf8("Operation cancelled by the user."));
		progress_lbl->setText(trUtf8("No operations left."));

		step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")));
		progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")));

		PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);
	}

	if(src_import_helper && src_import_thread->isRunning())
	{
		src_import_helper->cancelImport();
		src_import_thread->quit();
	}

	if(import_helper && import_thread->isRunning())
	{
		import_helper->cancelImport();
		import_thread->quit();
	}

	if(diff_helper && diff_thread->isRunning())
	{
		diff_helper->cancelDiff();
		diff_thread->quit();
	}

	if(export_helper && export_thread->isRunning())
	{
		export_helper->cancelExport();
		export_thread->quit();
	}

	resetButtons();
	process_paused=false;
}

void ModelDatabaseDiffForm::captureThreadError(Exception e)
{
	QTreeWidgetItem *item=nullptr;

	cancelOperation(false);
	progress_lbl->setText(trUtf8("Process aborted due to errors!"));
	progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_erro")));

	item=PgModelerUiNs::createOutputTreeItem(output_trw, PgModelerUiNs::formatMessage(e.getErrorMessage()), *progress_ico_lbl->pixmap(), nullptr, false, true);
	PgModelerUiNs::createExceptionsTree(output_trw, e, item);

	throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void ModelDatabaseDiffForm::handleImportFinished(Exception e)
{
	if(!e.getErrorMessage().isEmpty())
	{
		Messagebox msgbox;
		msgbox.show(e, e.getErrorMessage(), Messagebox::AlertIcon);
	}

	curr_step++;

	if(src_import_thread && src_import_thread->isRunning())
	{
		src_import_thread->quit();
		src_import_item->setExpanded(false);
		importDatabase(ImportThread);
	}
	else
	{
		import_thread->quit();
		diffModels();
	}
}

void ModelDatabaseDiffForm::handleDiffFinished(void)
{
	curr_step++;
	sqlcode_txt->setPlainText(diff_helper->getDiffDefinition());

#ifdef DEMO_VERSION
#warning "DEMO VERSION: SQL code preview truncated."
	if(!sqlcode_txt->toPlainText().isEmpty())
	{
		QString code=sqlcode_txt->toPlainText();
		code=code.mid(0, code.size()/2);
		code+=trUtf8("\n\n-- SQL code purposely truncated at this point in demo version!");
		sqlcode_txt->setPlainText(code);
	}
#endif

	settings_tbw->setTabEnabled(2, true);
	diff_thread->quit();

	if(store_in_file_rb->isChecked())
		saveDiffToFile();
	else if(!sqlcode_txt->toPlainText().isEmpty())
		exportDiff();
	else
		finishDiff();

	if(sqlcode_txt->toPlainText().isEmpty())
		sqlcode_txt->setPlainText(trUtf8("-- No differences were detected between model and database. --"));
}

void ModelDatabaseDiffForm::handleExportFinished(void)
{
	export_thread->quit();
	export_thread->wait();
	listDatabases();
	finishDiff();
}

void ModelDatabaseDiffForm::handleErrorIgnored(QString err_code, QString err_msg, QString cmd)
{
	QTreeWidgetItem *item=nullptr;

	item=PgModelerUiNs::createOutputTreeItem(output_trw, trUtf8("Error code <strong>%1</strong> found and ignored. Proceeding with export.").arg(err_code),
											 QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")),
											 export_item, false);

	PgModelerUiNs::createOutputTreeItem(output_trw, PgModelerUiNs::formatMessage(err_msg),
										QPixmap(QString("msgbox_alerta")),
										item, false);

	PgModelerUiNs::createOutputTreeItem(output_trw, cmd,
										QPixmap(),
										item, false);
}

void ModelDatabaseDiffForm::updateProgress(int progress, QString msg, ObjectType obj_type, QString cmd)
{
	int progress_aux = 0;

	msg=PgModelerUiNs::formatMessage(msg);


	if(src_import_thread && src_import_thread->isRunning())
	{
		progress_aux = progress/5;

		PgModelerUiNs::createOutputTreeItem(output_trw, msg,
											QPixmap(PgModelerUiNs::getIconPath(obj_type)),
											src_import_item);
	}
	else if(import_thread && import_thread->isRunning())
	{
		if(src_model_rb->isChecked())
			progress_aux = progress/4;
		else
			progress_aux = 20 + (progress/5);

		PgModelerUiNs::createOutputTreeItem(output_trw, msg,
											QPixmap(PgModelerUiNs::getIconPath(obj_type)),
											import_item);
	}
	else if(diff_thread && diff_thread->isRunning())
	{
		if((progress == 0 || progress == 100) && obj_type==ObjectType::BaseObject)
		{
			PgModelerUiNs::createOutputTreeItem(output_trw, msg,
												QPixmap(PgModelerUiNs::getIconPath("msgbox_info")),
												diff_item);
		}

		progress_aux = diff_progress + (progress/3);
	}
	else if(export_thread && export_thread->isRunning())
	{
		QTreeWidgetItem *item=nullptr;
		QPixmap ico;

		progress_aux = diff_progress + (progress/3);

		if(obj_type==ObjectType::BaseObject)
			ico=QPixmap(PgModelerUiNs::getIconPath("codigosql"));
		else
			ico=QPixmap(PgModelerUiNs::getIconPath(obj_type));

		item=PgModelerUiNs::createOutputTreeItem(output_trw, msg, ico, export_item, false);

		if(!cmd.isEmpty())
			PgModelerUiNs::createOutputTreeItem(output_trw, cmd, QPixmap(), item, false);
	}

	if(progress_aux > step_pb->value())
		step_pb->setValue(progress_aux);

	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=ObjectType::BaseObject)
		progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath(obj_type)));
	else
		progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));

	this->repaint();
}

void ModelDatabaseDiffForm::updateDiffInfo(ObjectsDiffInfo diff_info)
{
	map<unsigned, QToolButton *> buttons={ {ObjectsDiffInfo::CreateObject, create_tb},
										   {ObjectsDiffInfo::DropObject,   drop_tb},
										   {ObjectsDiffInfo::AlterObject,  alter_tb},
										   {ObjectsDiffInfo::IgnoreObject, ignore_tb} };

	unsigned diff_type=diff_info.getDiffType();
	QToolButton *btn=buttons[diff_type];
	QTreeWidgetItem *item=nullptr;

	item=PgModelerUiNs::createOutputTreeItem(output_trw,
											 PgModelerUiNs::formatMessage(diff_info.getInfoMessage()),
											 QPixmap(PgModelerUiNs::getIconPath(diff_info.getObject()->getSchemaName())), diff_item);
	item->setData(0, Qt::UserRole, diff_info.getDiffType());

	if(diff_helper)
		btn->setText(QString::number(diff_helper->getDiffTypeCount(diff_type)));

	output_trw->setItemHidden(item, !btn->isChecked());
}

void ModelDatabaseDiffForm::selectOutputFile(void)
{
	QFileDialog file_dlg;

	file_dlg.setWindowTitle(trUtf8("Save diff as..."));
	file_dlg.setFileMode(QFileDialog::AnyFile);
	file_dlg.setAcceptMode(QFileDialog::AcceptSave);
	file_dlg.setModal(true);
	file_dlg.setNameFilter(trUtf8("SQL code (*.sql);;All files (*.*)"));

	if(source_model)
		file_dlg.selectFile(source_model->getName() + QString("-diff.sql"));

	if(file_dlg.exec()==QFileDialog::Accepted)
	{
		QString file;

		if(!file_dlg.selectedFiles().isEmpty())
			file = file_dlg.selectedFiles().at(0);

		file_edt->setText(file);
	}
}
