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

#include "modeldatabasediffform.h"
#include "configurationform.h"
#include "databaseimportform.h"
#include "pgmodeleruins.h"
#include <QTemporaryFile>

bool ModelDatabaseDiffForm::low_verbosity = false;
map<QString, attribs_map> ModelDatabaseDiffForm::config_params;

ModelDatabaseDiffForm::ModelDatabaseDiffForm(QWidget *parent, Qt::WindowFlags flags) : BaseConfigWidget (parent)
{
	try
	{
		setupUi(this);
		setWindowFlags(flags);

		sqlcode_txt=PgModelerUiNs::createNumberedTextEditor(sqlcode_wgt);
		sqlcode_txt->setReadOnly(true);

		htmlitem_del=new HtmlItemDelegate(this);
		output_trw->setItemDelegateForColumn(0, htmlitem_del);

		file_sel = new FileSelectorWidget(this);
		file_sel->setAllowFilenameInput(true);
		file_sel->setFileMode(QFileDialog::AnyFile);
		file_sel->setAcceptMode(QFileDialog::AcceptSave);
		file_sel->setFileDialogTitle(tr("Save diff as"));
		file_sel->setMimeTypeFilters({"application/sql", "application/octet-stream"});
		file_sel->setDefaultSuffix("sql");
		store_in_file_grid->addWidget(file_sel, 0, 1);

		is_adding_new_preset=false;
		imported_model=loaded_model=source_model=nullptr;
		src_import_helper=import_helper=nullptr;
		diff_helper=nullptr;
		export_helper=nullptr;
		src_import_thread=import_thread=diff_thread=export_thread=nullptr;
		src_import_item=import_item=diff_item=export_item=nullptr;
		export_conn=nullptr;
		process_paused=false;
		diff_progress=curr_step=total_steps=0;

		sqlcode_hl=new SyntaxHighlighter(sqlcode_txt);
		sqlcode_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

		pgsql_ver_cmb->addItems(PgSqlVersions::AllVersions);
		PgModelerUiNs::configureWidgetFont(message_lbl, PgModelerUiNs::MediumFontFactor);

		cancel_preset_edit_tb->setVisible(false);
		preset_name_edt->setVisible(false);

		new_preset_tb->setToolTip(new_preset_tb->toolTip() + QString(" (%1)").arg(new_preset_tb->shortcut().toString()));
		edit_preset_tb->setToolTip(edit_preset_tb->toolTip() + QString(" (%1)").arg(edit_preset_tb->shortcut().toString()));
		save_preset_tb->setToolTip(save_preset_tb->toolTip() + QString(" (%1)").arg(save_preset_tb->shortcut().toString()));
		cancel_preset_edit_tb->setToolTip(cancel_preset_edit_tb->toolTip() + QString(" (%1)").arg(cancel_preset_edit_tb->shortcut().toString()));
		remove_preset_tb->setToolTip(remove_preset_tb->toolTip() + QString(" (%1)").arg(remove_preset_tb->shortcut().toString()));
		default_presets_tb->setToolTip(default_presets_tb->toolTip() + QString(" (%1)").arg(default_presets_tb->shortcut().toString()));

		connect(cancel_btn, &QToolButton::clicked, [&](){ cancelOperation(true); });
		connect(pgsql_ver_chk, SIGNAL(toggled(bool)), pgsql_ver_cmb, SLOT(setEnabled(bool)));
		connect(connections_cmb, SIGNAL(activated(int)), this, SLOT(listDatabases()));
		connect(store_in_file_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
		connect(apply_on_server_rb, SIGNAL(clicked()), this, SLOT(enableDiffMode()));
		connect(file_sel, SIGNAL(s_selectorChanged(bool)), this, SLOT(enableDiffMode()));
		connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableDiffMode()));
		connect(generate_btn, SIGNAL(clicked()), this, SLOT(generateDiff()));
		connect(close_btn, SIGNAL(clicked()), this, SLOT(close()));
		connect(store_in_file_rb, SIGNAL(clicked(bool)), store_in_file_wgt, SLOT(setEnabled(bool)));
		connect(force_recreation_chk, SIGNAL(toggled(bool)), recreate_unmod_chk, SLOT(setEnabled(bool)));
		connect(dont_drop_missing_objs_chk, SIGNAL(toggled(bool)), drop_missing_cols_constr_chk, SLOT(setEnabled(bool)));
		connect(create_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(drop_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(alter_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(ignore_tb, SIGNAL(toggled(bool)), this, SLOT(filterDiffInfos()));
		connect(ignore_error_codes_chk, SIGNAL(toggled(bool)), error_codes_edt, SLOT(setEnabled(bool)));
		connect(src_model_rb, SIGNAL(toggled(bool)), src_model_name_lbl, SLOT(setEnabled(bool)));
		connect(src_connections_cmb, SIGNAL(activated(int)), this, SLOT(listDatabases()));
		connect(src_database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableDiffMode()));
		connect(src_model_rb, SIGNAL(toggled(bool)), this, SLOT(enableDiffMode()));
		connect(open_in_sql_tool_btn, SIGNAL(clicked(bool)), this, SLOT(loadDiffInSQLTool()));
		connect(presets_cmb, SIGNAL(activated(int)), this, SLOT(selectPreset()));

		connect(default_presets_tb, SIGNAL(clicked(bool)), this, SLOT(restoreDefaults()));
		connect(remove_preset_tb, SIGNAL(clicked(bool)), this, SLOT(removePreset()));
		connect(save_preset_tb, SIGNAL(clicked(bool)), this, SLOT(savePreset()));

		connect(src_database_rb, &QRadioButton::toggled, [&](bool toggle){
			src_database_wgt->setEnabled(toggle);
			src_connection_lbl->setEnabled(toggle && src_connections_cmb->count() > 0);
			enableDiffMode();
		});

		connect(new_preset_tb, &QToolButton::clicked, [&](){
			togglePresetConfiguration(true);
		});

		connect(edit_preset_tb, &QToolButton::clicked, [&](){
			togglePresetConfiguration(true, true);
		});

		connect(cancel_preset_edit_tb, &QToolButton::clicked, [&](){
			togglePresetConfiguration(false);
			enablePresetButtons();
		});

		connect(preset_name_edt, &QLineEdit::textChanged, [&](const QString &text){
			save_preset_tb->setEnabled(!text.isEmpty());
		});

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
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

ModelDatabaseDiffForm::~ModelDatabaseDiffForm()
{
	destroyThread(ImportThread);
	destroyThread(DiffThread);
	destroyThread(ExportThread);
	destroyModel();
}

void ModelDatabaseDiffForm::exec()
{
	show();
	loadConfiguration();
	event_loop.exec();
}

void ModelDatabaseDiffForm::setModelWidget(ModelWidget *model_wgt)
{
	if(model_wgt)
	{
		source_model=loaded_model=model_wgt->getDatabaseModel();
		src_model_name_lbl->setText(QString("%1 [%2]").arg(source_model->getName()).arg(QFileInfo(model_wgt->getFilename()).fileName()));
		src_model_name_lbl->setToolTip(model_wgt->getFilename().isEmpty() ? tr("model not saved yet") : model_wgt->getFilename());
	}
	else
	{
		src_model_name_lbl->setText(tr("(none)"));
		src_model_name_lbl->setToolTip("");
		src_database_rb->setChecked(true);
		src_model_rb->setEnabled(false);
	}
}

void ModelDatabaseDiffForm::setLowVerbosity(bool value)
{
	low_verbosity = value;
}

bool ModelDatabaseDiffForm::isThreadsRunning()
{
	return ((import_thread && import_thread->isRunning()) ||
					(src_import_thread && src_import_thread->isRunning()) ||
					(diff_thread && diff_thread->isRunning()) ||
					(export_thread && export_thread->isRunning()));
}

void ModelDatabaseDiffForm::resetForm()
{
	ConnectionsConfigWidget::fillConnectionsComboBox(src_connections_cmb, true);
	src_connections_cmb->setEnabled(src_connections_cmb->count() > 0);
	src_connection_lbl->setEnabled(src_connections_cmb->isEnabled());
	src_database_cmb->setCurrentIndex(0);

	ConnectionsConfigWidget::fillConnectionsComboBox(connections_cmb, true, Connection::OpDiff);
	connections_cmb->setEnabled(connections_cmb->count() > 0);
	connection_lbl->setEnabled(connections_cmb->isEnabled());
	database_cmb->setCurrentIndex(0);

	enableDiffMode();
	settings_tbw->setTabEnabled(1, false);
	settings_tbw->setTabEnabled(2, false);
}

void ModelDatabaseDiffForm::closeEvent(QCloseEvent *event)
{
	//Ignore the close event when the thread is running
	if(isThreadsRunning())
		event->ignore();
	else if(process_paused)
		cancelOperation(true);

	//If no threads are running we quit the event loop so the control can be returned to main thread (application)
	if(!isThreadsRunning())
		event_loop.quit();
}

void ModelDatabaseDiffForm::showEvent(QShowEvent *)
{
	//Doing the form configuration in the first show in order to populate the connections combo
	if(!isThreadsRunning() && connections_cmb->count() == 0)
	{
		resetForm();

		if(connections_cmb->currentIndex() > 0)
			listDatabases();
	}
}

void ModelDatabaseDiffForm::createThread(unsigned thread_id)
{
	if(thread_id==SrcImportThread)
	{
		src_import_thread=new QThread;
		src_import_helper=new DatabaseImportHelper;
		src_import_helper->moveToThread(src_import_thread);

		connect(src_import_thread, SIGNAL(started()), src_import_helper, SLOT(importDatabase()));
		connect(src_import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)), Qt::BlockingQueuedConnection);
		connect(src_import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
		connect(src_import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	}
	else if(thread_id==ImportThread)
	{
		import_thread=new QThread;
		import_helper=new DatabaseImportHelper;
		import_helper->moveToThread(import_thread);

		connect(import_thread, SIGNAL(started()), import_helper, SLOT(importDatabase()));
		connect(import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)), Qt::BlockingQueuedConnection);
		connect(import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
		connect(import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	}
	else if(thread_id==DiffThread)
	{
		diff_thread=new QThread;
		diff_helper=new ModelsDiffHelper;
		diff_helper->moveToThread(diff_thread);

		connect(diff_thread, SIGNAL(started()), diff_helper, SLOT(diffModels()));
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

		connect(export_thread, SIGNAL(started()), export_helper, SLOT(exportToDBMS()));
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
		delete src_import_thread;
		delete src_import_helper;
		src_import_thread=nullptr;
		src_import_helper=nullptr;
	}
	else if(thread_id==ImportThread && import_thread)
	{
		delete import_thread;
		delete import_helper;
		import_thread=nullptr;
		import_helper=nullptr;
	}
	else if(thread_id==DiffThread && diff_thread)
	{
		diff_thread=nullptr;
		diff_helper=nullptr;
		delete diff_thread;
		delete diff_helper;
	}
	else if(export_thread)
	{
		if(export_conn)
		{
			delete export_conn;
			export_conn=nullptr;
		}

		delete export_thread;
		delete export_helper;
		export_thread=nullptr;
		export_helper=nullptr;
	}
}

void ModelDatabaseDiffForm::destroyModel()
{
	if(imported_model)
		delete imported_model;

	if(source_model && source_model != loaded_model && src_database_rb->isChecked())
	{
		delete source_model;
		source_model = nullptr;
	}

	imported_model=nullptr;
}

void ModelDatabaseDiffForm::clearOutput()
{
	output_trw->clear();
	src_import_item=import_item=diff_item=export_item=nullptr;

	step_lbl->setText(tr("Waiting process to start..."));
	step_ico_lbl->setPixmap(QPixmap());
	progress_lbl->setText(tr("Waiting process to start..."));
	progress_ico_lbl->setPixmap(QPixmap());

	step_pb->setValue(0);
	progress_pb->setValue(0);

	create_tb->setText(QString("0"));
	alter_tb->setText(QString("0"));
	drop_tb->setText(QString("0"));
	ignore_tb->setText(QString("0"));
}

void ModelDatabaseDiffForm::listDatabases()
{
	QComboBox *conn_cmb = (sender() == src_connections_cmb ? src_connections_cmb : connections_cmb),
			*db_cmb = (conn_cmb == src_connections_cmb ? src_database_cmb : database_cmb);
	QLabel *db_lbl = (conn_cmb == src_connections_cmb ? src_database_lbl : database_lbl);

	try
	{
		if(conn_cmb->currentIndex()==conn_cmb->count()-1)
		{
			ConnectionsConfigWidget::openConnectionsConfiguration(conn_cmb, true);
			resetForm();
			emit s_connectionsUpdateRequest();
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::enableDiffMode()
{
	store_in_file_wgt->setEnabled(store_in_file_rb->isChecked());

	generate_btn->setEnabled(database_cmb->currentIndex() > 0 &&
													 ((src_database_rb->isChecked() && src_database_cmb->currentIndex() > 0) ||
														(src_model_rb->isChecked() && loaded_model)) &&
													 ((store_in_file_rb->isChecked() && !file_sel->getSelectedFile().isEmpty() && !file_sel->hasWarning()) ||
														(apply_on_server_rb->isChecked())));
}

void ModelDatabaseDiffForm::generateDiff()
{
	// Cancel any pending preset editing before run the diff
	togglePresetConfiguration(false);

	//Destroy previously allocated threads and helper before start over.
	destroyModel();
	destroyThread(SrcImportThread);
	destroyThread(ImportThread);
	destroyThread(DiffThread);
	destroyThread(ExportThread);

	clearOutput();
	curr_step = 1;

	if(low_verbosity)
		PgModelerUiNs::createOutputTreeItem(output_trw, tr("<strong>Low verbosity is set:</strong> only key informations and errors will be displayed."),
																				QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), nullptr, false);

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
	close_btn->setEnabled(false);

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
		step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("import")));

		conn.switchToDatabase(db_cmb->currentText());

		step_lbl->setText(tr("Step %1/%2: Importing database <strong>%3</strong>...")
											.arg(curr_step)
											.arg(total_steps)
											.arg(conn.getConnectionId(true, true)));

		if(thread_id == SrcImportThread)
			src_import_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);
		else
			import_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);

		pgsql_ver=conn.getPgSQLVersion(true);
		catalog.setConnection(conn);

		//The import process will exclude built-in array array types, system and extension objects
		catalog.setQueryFilter(Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes |
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
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::diffModels()
{
	createThread(DiffThread);

	step_lbl->setText(tr("Step %1/%2: Comparing <strong>%3</strong> and <strong>%4</strong>...")
						.arg(curr_step)
						.arg(total_steps)
					  .arg(source_model->getName())
					  .arg(imported_model->getName()));
	step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("diff")));

	if(src_import_item)
		output_trw->collapseItem(src_import_item);

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
		msg_box.show(tr("Confirmation"),
					 tr(" <strong>WARNING:</strong> The generated diff is ready to be exported! Once started this process will cause irreversible changes on the database. Do you really want to proceed?"),
					 Messagebox::AlertIcon, Messagebox::AllButtons,
					 tr("Apply diff"), tr("Preview diff"), QString(),
					 PgModelerUiNs::getIconPath("diff"), PgModelerUiNs::getIconPath("codigosql"));

	if(!confirm || msg_box.result()==QDialog::Accepted)
	{
		export_conn=new Connection;
		*export_conn=*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

		settings_tbw->setCurrentIndex(1);
		apply_on_server_btn->setEnabled(true);

		step_lbl->setText(tr("Step %1/%2: Exporting diff to database <strong>%3@%4</strong>...")
											.arg(curr_step)
											.arg(total_steps)
											.arg(imported_model->getName())
											.arg(export_conn->getConnectionId(true)));
		step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("exportar")));

		output_trw->collapseItem(diff_item);
		diff_progress=step_pb->value();
		export_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);

		export_helper->setExportToDBMSParams(sqlcode_txt->toPlainText(), export_conn,
																				 database_cmb->currentText(), ignore_duplic_chk->isChecked());
		if(ignore_error_codes_chk->isChecked())
			export_helper->setIgnoredErrors(error_codes_edt->text().simplified().split(' '));

		export_thread->start();
		close_btn->setEnabled(false);
	}
	else if(msg_box.isCancelled())
		cancelOperation(true);
	else
	{
		process_paused=true;
		close_btn->setEnabled(true);
		settings_tbw->setCurrentIndex(2);
		apply_on_server_btn->setVisible(true);
		output_trw->collapseItem(diff_item);
		PgModelerUiNs::createOutputTreeItem(output_trw,
											tr("Diff process paused. Waiting user action..."),
											QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), nullptr);
	}
}

void ModelDatabaseDiffForm::filterDiffInfos()
{
	QToolButton *btn=dynamic_cast<QToolButton *>(sender());
	map<QToolButton *, unsigned> diff_types={ {create_tb, ObjectsDiffInfo::CreateObject},
											  {drop_tb, ObjectsDiffInfo::DropObject},
											  {alter_tb, ObjectsDiffInfo::AlterObject},
											  {ignore_tb, ObjectsDiffInfo::IgnoreObject}};

	for(int i=0; i < diff_item->childCount(); i++)
	{
		if(diff_item->child(i)->data(0, Qt::UserRole).toUInt()==diff_types[btn])
			diff_item->child(i)->setHidden(!btn->isChecked());
	}
}

void ModelDatabaseDiffForm::loadDiffInSQLTool()
{
	QString database = database_cmb->currentText(), filename;
	QFile out_tmp_file;
	Connection conn=(*reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>()));
	QByteArray buffer;
	QTemporaryFile tmp_sql_file;

	cancelOperation(true);

	if(store_in_file_rb->isChecked())
			filename = file_sel->getSelectedFile();
	else
	{
			tmp_sql_file.setFileTemplate(GlobalAttributes::getTemporaryFilePath(QString("diff_%1_XXXXXX.sql").arg(database)));

			tmp_sql_file.open();
			filename = tmp_sql_file.fileName();
			tmp_sql_file.close();

			out_tmp_file.setFileName(filename);
			out_tmp_file.open(QFile::WriteOnly);

			if(!out_tmp_file.isOpen())
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(out_tmp_file.fileName()),
												ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			buffer.append(sqlcode_txt->toPlainText());
			out_tmp_file.write(buffer);
			out_tmp_file.close();
	}

	emit s_loadDiffInSQLTool(conn.getConnectionId(), database, filename);
	close();
}

void ModelDatabaseDiffForm::resetButtons()
{
	buttons_wgt->setEnabled(true);
	cancel_btn->setEnabled(false);
	settings_tbw->setTabEnabled(0, true);
	apply_on_server_btn->setVisible(false);
	enableDiffMode();
}

void ModelDatabaseDiffForm::saveDiffToFile()
{
	if(!sqlcode_txt->toPlainText().isEmpty())
	{
		QFile output;

		step_lbl->setText(tr("Saving diff to file <strong>%1</strong>").arg(file_sel->getSelectedFile()));
		step_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("salvar")));
		import_item=PgModelerUiNs::createOutputTreeItem(output_trw, step_lbl->text(), *step_ico_lbl->pixmap(), nullptr);
		step_pb->setValue(90);
		progress_pb->setValue(100);

		output.setFileName(file_sel->getSelectedFile());

		if(!output.open(QFile::WriteOnly))
			captureThreadError(Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(file_sel->getSelectedFile()),
																	 ErrorCode::FileDirectoryNotWritten, __PRETTY_FUNCTION__,__FILE__,__LINE__));

		output.write(sqlcode_txt->toPlainText().toUtf8());
		output.close();
	}

	finishDiff();
}

void ModelDatabaseDiffForm::finishDiff()
{
	cancelOperation(false);

	step_lbl->setText(tr("Diff process sucessfully ended!"));
	progress_lbl->setText(tr("No operations left."));

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
		step_lbl->setText(tr("Operation cancelled by the user."));
		progress_lbl->setText(tr("No operations left."));

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
	close_btn->setEnabled(true);
}

void ModelDatabaseDiffForm::captureThreadError(Exception e)
{
	QTreeWidgetItem *item=nullptr;

	cancelOperation(false);
	progress_lbl->setText(tr("Process aborted due to errors!"));
	progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_erro")));

	item=PgModelerUiNs::createOutputTreeItem(output_trw, PgModelerUiNs::formatMessage(e.getErrorMessage()), *progress_ico_lbl->pixmap(), nullptr, false, true);
	PgModelerUiNs::createExceptionsTree(output_trw, e, item);

	throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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

void ModelDatabaseDiffForm::handleDiffFinished()
{
	curr_step++;
	sqlcode_txt->setPlainText(diff_helper->getDiffDefinition());

#ifdef DEMO_VERSION
#warning "DEMO VERSION: SQL code preview truncated."
	if(!sqlcode_txt->toPlainText().isEmpty())
	{
		QString code=sqlcode_txt->toPlainText();
		code=code.mid(0, code.size()/2);
		code+=tr("\n\n-- SQL code purposely truncated at this point in demo version!");
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
		sqlcode_txt->setPlainText(tr("-- No differences were detected between model and database. --"));
}

void ModelDatabaseDiffForm::handleExportFinished()
{
	export_thread->quit();
	export_thread->wait();
	listDatabases();
	finishDiff();
}

void ModelDatabaseDiffForm::handleErrorIgnored(QString err_code, QString err_msg, QString cmd)
{
	QTreeWidgetItem *item=nullptr;

	item=PgModelerUiNs::createOutputTreeItem(output_trw, tr("Error code <strong>%1</strong> found and ignored. Proceeding with export.").arg(err_code),
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

		if(!low_verbosity)
		{
			PgModelerUiNs::createOutputTreeItem(output_trw, msg,
												QPixmap(PgModelerUiNs::getIconPath(obj_type)),
												src_import_item);
		}
	}
	else if(import_thread && import_thread->isRunning())
	{
		if(src_model_rb->isChecked())
			progress_aux = progress/4;
		else
			progress_aux = 20 + (progress/5);

		if(!low_verbosity)
		{
			PgModelerUiNs::createOutputTreeItem(output_trw, msg,
												QPixmap(PgModelerUiNs::getIconPath(obj_type)),
												import_item);
		}
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

		if(!low_verbosity)
		{
			if(obj_type==ObjectType::BaseObject)
				ico=QPixmap(PgModelerUiNs::getIconPath("codigosql"));
			else
				ico=QPixmap(PgModelerUiNs::getIconPath(obj_type));

			item=PgModelerUiNs::createOutputTreeItem(output_trw, msg, ico, export_item, false);

			if(!cmd.isEmpty())
				PgModelerUiNs::createOutputTreeItem(output_trw, cmd, QPixmap(), item, false);
		}
	}

	if(progress_aux > step_pb->value())
		step_pb->setValue(progress_aux);

	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=ObjectType::BaseObject)
		progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath(obj_type)));
	else
		progress_ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));
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

	if(!low_verbosity)
	{
		item=PgModelerUiNs::createOutputTreeItem(output_trw,
												 PgModelerUiNs::formatMessage(diff_info.getInfoMessage()),
												 QPixmap(PgModelerUiNs::getIconPath(diff_info.getObject()->getSchemaName())), diff_item);
		item->setData(0, Qt::UserRole, diff_info.getDiffType());
	}

	if(diff_helper)
		btn->setText(QString::number(diff_helper->getDiffTypeCount(diff_type)));

	if(item)
		item->setHidden(!btn->isChecked());
}

void ModelDatabaseDiffForm::loadConfiguration()
{
	try
	{
		BaseConfigWidget::loadConfiguration(GlobalAttributes::DiffPresetsConf, config_params, { Attributes::Name });
		applyConfiguration();
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e, QString("%1 %2").arg(e.getErrorMessage()).arg(tr("In some cases restore the default settings related to it may solve the problem. Would like to do that?")),
								 Messagebox::AlertIcon, Messagebox::YesNoButtons, tr("Restore"), QString(), QString(), PgModelerUiNs::getIconPath("atualizar"));

		if(msg_box.result() == QDialog::Accepted)
			restoreDefaults();
	}
}

void ModelDatabaseDiffForm::saveConfiguration()
{
	try
	{
		attribs_map attribs;
		QString preset_sch;
		QString presets;

		preset_sch=GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir,
																															Attributes::Preset +
																															GlobalAttributes::SchemaExt);

		for(auto &conf : config_params)
		{
			schparser.ignoreUnkownAttributes(true);
			schparser.ignoreEmptyAttributes(true);
			presets += schparser.getCodeDefinition(preset_sch, conf.second);
			schparser.ignoreUnkownAttributes(false);
			schparser.ignoreEmptyAttributes(false);
		}

		config_params[GlobalAttributes::DiffPresetsConf][Attributes::Preset] = presets;
		BaseConfigWidget::saveConfiguration(GlobalAttributes::DiffPresetsConf, config_params);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelDatabaseDiffForm::applyConfiguration()
{
	presets_cmb->clear();
	presets_cmb->blockSignals(true);

	for(auto &conf : config_params)
		presets_cmb->addItem(conf.first);

	presets_cmb->blockSignals(false);
	enablePresetButtons();
	selectPreset();
}

void ModelDatabaseDiffForm::restoreDefaults()
{
	try
	{
		Messagebox msg_box;
		msg_box.show(tr("Do you really want to restore the default settings?"),
								 Messagebox::ConfirmIcon,	Messagebox::YesNoButtons);

		if(msg_box.result()==QDialog::Accepted)
		{
			BaseConfigWidget::restoreDefaults(GlobalAttributes::DiffPresetsConf, false);
			BaseConfigWidget::loadConfiguration(GlobalAttributes::DiffPresetsConf, config_params, { Attributes::Name });
			applyConfiguration();
		}
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
	}
}

void ModelDatabaseDiffForm::selectPreset()
{
	attribs_map conf = config_params[presets_cmb->currentText()];
	QStringList db_name;

	src_model_rb->setChecked(src_model_rb->isEnabled() && conf[Attributes::CurrentModel] == Attributes::True);

	src_database_rb->setChecked(!conf[Attributes::InputDatabase].isEmpty());
	src_connections_cmb->setCurrentIndex(0);
	src_connections_cmb->activated(0);
	db_name = conf[Attributes::InputDatabase].split('@');

	if(db_name.size() > 1)
	{
		int idx = src_connections_cmb->findText(db_name[1], Qt::MatchStartsWith);

		if(idx >= 0)
		{
			src_connections_cmb->setCurrentIndex(idx);
			src_connections_cmb->activated(idx);
			src_database_cmb->setCurrentText(db_name[0]);
		}
	}

	// Selecting the database to compare
	connections_cmb->setCurrentIndex(0);
	connections_cmb->activated(0);
	db_name = conf[Attributes::CompareToDatabase].split('@');

	if(db_name.size() > 1)
	{
		int idx = connections_cmb->findText(db_name[1], Qt::MatchStartsWith);

		if(idx > 0)
		{
			connections_cmb->setCurrentIndex(idx);
			connections_cmb->activated(idx);
			database_cmb->setCurrentText(db_name[0]);
		}
	}

	pgsql_ver_chk->setChecked(!conf[Attributes::Version].isEmpty());
	if(pgsql_ver_chk->isChecked())
		pgsql_ver_cmb->setCurrentText(conf[Attributes::Version]);

	store_in_file_rb->setChecked(conf[Attributes::StoreInFile] == Attributes::True);
	apply_on_server_rb->setChecked(conf[Attributes::ApplyOnServer] == Attributes::True);
	enableDiffMode();

	keep_cluster_objs_chk->setChecked(conf[Attributes::KeepClusterObjs] == Attributes::True);
	keep_obj_perms_chk->setChecked(conf[Attributes::KeepObjsPerms] == Attributes::True);
	dont_drop_missing_objs_chk->setChecked(conf[Attributes::DontDropMissingObjs] == Attributes::True);
	drop_missing_cols_constr_chk->setChecked(conf[Attributes::DontDropMissingObjs] == Attributes::True &&
																					 conf[Attributes::DropMissingColsConstrs] == Attributes::True);
	preserve_db_name_chk->setChecked(conf[Attributes::PreserveDbName] == Attributes::True);
	cascade_mode_chk->setChecked(conf[Attributes::DropTruncCascade] == Attributes::True);
	trunc_tables_chk->setChecked(conf[Attributes::TruncColsBeforeAlter] == Attributes::True);
	reuse_sequences_chk->setChecked(conf[Attributes::ReuseSequences] == Attributes::True);
	force_recreation_chk->setChecked(conf[Attributes::ForceObjsRecreation] == Attributes::True);
	recreate_unmod_chk->setChecked(conf[Attributes::ForceObjsRecreation] == Attributes::True &&
																 conf[Attributes::RecreateUnmodObjs] == Attributes::True);

	import_sys_objs_chk->setChecked(conf[Attributes::ImportSysObjs] == Attributes::True);
	import_ext_objs_chk->setChecked(conf[Attributes::ImportExtObjs] == Attributes::True);
	ignore_duplic_chk->setChecked(conf[Attributes::IgnoreDuplicErrors] == Attributes::True);
	ignore_errors_chk->setChecked(conf[Attributes::IgnoreImportErrors] == Attributes::True);
	ignore_error_codes_chk->setChecked(!conf[Attributes::IgnoreErrorCodes].isEmpty());
	error_codes_edt->setText(conf[Attributes::IgnoreErrorCodes]);
}

void ModelDatabaseDiffForm::togglePresetConfiguration(bool toggle, bool is_edit)
{
	is_adding_new_preset = toggle && !is_edit;
	presets_cmb->setVisible(!toggle);
	preset_name_edt->setVisible(toggle);
	default_presets_tb->setVisible(!toggle);
	cancel_preset_edit_tb->setVisible(toggle);
	new_preset_tb->setVisible(!toggle);
	edit_preset_tb->setVisible(!toggle);
	remove_preset_tb->setVisible(!toggle);
	preset_name_edt->clear();
	save_preset_tb->setEnabled(toggle && (is_edit && presets_cmb->count() > 0));

	if(is_edit)
		preset_name_edt->setText(presets_cmb->currentText());

	if(toggle)
		preset_name_edt->setFocus();
}

void ModelDatabaseDiffForm::enablePresetButtons()
{
	presets_cmb->setEnabled(presets_cmb->count() > 0);
	edit_preset_tb->setEnabled(presets_cmb->isEnabled());
	remove_preset_tb->setEnabled(presets_cmb->isEnabled());
	save_preset_tb->setEnabled(presets_cmb->isEnabled());
}

void ModelDatabaseDiffForm::removePreset()
{
	Messagebox msg_box;

	msg_box.show(tr("Are you sure do you want to remove the selected diff preset?"), Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

	if(msg_box.result() == QDialog::Accepted)
	{
		config_params.erase(presets_cmb->currentText());
		applyConfiguration();
		saveConfiguration();
	}
}

void ModelDatabaseDiffForm::savePreset()
{
	QString name, fmt_name;
	attribs_map conf;
	int idx = 0;

	if(!is_adding_new_preset)
	{
		fmt_name = name = preset_name_edt->text().isEmpty() ? presets_cmb->currentText() : preset_name_edt->text();
		config_params.erase(presets_cmb->currentText());
		presets_cmb->removeItem(presets_cmb->currentIndex());
	}
	else
		fmt_name = name = preset_name_edt->text();

	// Checking the preset name duplication and performing a basic desambiguation if necessary
	while(presets_cmb->findText(fmt_name, Qt::MatchExactly) >= 0)
		fmt_name = name + QString::number(++idx);

	conf[Attributes::Name] = fmt_name;
	conf[Attributes::CurrentModel] = src_model_rb->isChecked() ? Attributes::True : QString();

	if(src_database_rb->isChecked())
	{
		conf[Attributes::InputDatabase] = QString("%1@%2")
																			.arg(src_database_cmb->currentIndex() > 0 ? src_database_cmb->currentText() : QString("-"))
																			.arg(src_connections_cmb->currentIndex() > 0 ? src_connections_cmb->currentText() : QString("-"));
	}
	else
		conf[Attributes::InputDatabase] = QString();

	conf[Attributes::CompareToDatabase] = QString("%1@%2")
																				.arg(database_cmb->currentIndex() > 0 ? database_cmb->currentText() : QString("-"))
																				.arg(connections_cmb->currentIndex() > 0 ? connections_cmb->currentText() : QString("-"));
	conf[Attributes::Version] = pgsql_ver_chk->isChecked() ? pgsql_ver_cmb->currentText() : QString();
	conf[Attributes::StoreInFile] = store_in_file_rb->isChecked() ? Attributes::True : QString();
	conf[Attributes::ApplyOnServer] = apply_on_server_rb->isChecked() ? Attributes::True : QString();
	conf[Attributes::KeepClusterObjs] = keep_cluster_objs_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::KeepObjsPerms] = keep_obj_perms_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::DontDropMissingObjs] = dont_drop_missing_objs_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::DropMissingColsConstrs] = drop_missing_cols_constr_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::PreserveDbName] = preserve_db_name_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::DropTruncCascade] = cascade_mode_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::TruncColsBeforeAlter] = trunc_tables_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::ReuseSequences] = reuse_sequences_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::ForceObjsRecreation] = force_recreation_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::RecreateUnmodObjs] = recreate_unmod_chk->isChecked() ? Attributes::True : Attributes::False;

	conf[Attributes::ImportSysObjs] = import_sys_objs_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::ImportExtObjs] = import_ext_objs_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::IgnoreDuplicErrors] = ignore_duplic_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::IgnoreImportErrors] = ignore_errors_chk->isChecked() ? Attributes::True : Attributes::False;
	conf[Attributes::IgnoreErrorCodes] = error_codes_edt->text();

	config_params[fmt_name] = conf;

	saveConfiguration();
	togglePresetConfiguration(false);
	applyConfiguration();

	presets_cmb->setCurrentText(fmt_name);
	selectPreset();
}
