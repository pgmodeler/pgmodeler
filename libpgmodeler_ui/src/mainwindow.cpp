/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "mainwindow.h"
#include "textboxwidget.h"
#include "sourcecodewidget.h"
#include "databasewidget.h"
#include "schemawidget.h"
#include "rolewidget.h"
#include "permissionwidget.h"
#include "tablespacewidget.h"
#include "languagewidget.h"
#include "functionwidget.h"
#include "parameterwidget.h"
#include "castwidget.h"
#include "conversionwidget.h"
#include "domainwidget.h"
#include "aggregatewidget.h"
#include "sequencewidget.h"
#include "operatorwidget.h"
#include "operatorfamilywidget.h"
#include "operatorclasswidget.h"
#include "typewidget.h"
#include "viewwidget.h"
#include "columnwidget.h"
#include "constraintwidget.h"
#include "rulewidget.h"
#include "triggerwidget.h"
#include "indexwidget.h"
#include "relationshipwidget.h"
#include "tablewidget.h"
#include "collationwidget.h"
#include "extensionwidget.h"
#include "tagwidget.h"
#include "taskprogresswidget.h"
#include "objectdepsrefswidget.h"
#include "configurationform.h"
#include "objectrenamewidget.h"
#include "sqlappendwidget.h"

//Global forms and widgets
AboutForm *about_form=nullptr;
TextboxWidget *textbox_wgt=nullptr;
SourceCodeWidget *sourcecode_wgt=nullptr;
DatabaseWidget *database_wgt=nullptr;
SchemaWidget *schema_wgt=nullptr;
RoleWidget *role_wgt=nullptr;
PermissionWidget *permission_wgt=nullptr;
TablespaceWidget *tablespace_wgt=nullptr;
LanguageWidget *language_wgt=nullptr;
ParameterWidget *parameter_wgt=nullptr;
FunctionWidget *function_wgt=nullptr;
CastWidget *cast_wgt=nullptr;
ConversionWidget *conversion_wgt=nullptr;
DomainWidget *domain_wgt=nullptr;
AggregateWidget *aggregate_wgt=nullptr;
SequenceWidget *sequence_wgt=nullptr;
OperatorWidget *operator_wgt=nullptr;
OperatorFamilyWidget *opfamily_wgt=nullptr;
OperatorClassWidget *opclass_wgt=nullptr;
TypeWidget *type_wgt=nullptr;
ViewWidget *view_wgt=nullptr;
ColumnWidget *column_wgt=nullptr;
ConstraintWidget *constraint_wgt=nullptr;
RuleWidget *rule_wgt=nullptr;
TriggerWidget *trigger_wgt=nullptr;
IndexWidget *index_wgt=nullptr;
RelationshipWidget *relationship_wgt=nullptr;
TableWidget *table_wgt=nullptr;
CollationWidget *collation_wgt=nullptr;
ExtensionWidget *extension_wgt=nullptr;
TagWidget *tag_wgt=nullptr;
TaskProgressWidget *task_prog_wgt=nullptr;
ObjectDepsRefsWidget *deps_refs_wgt=nullptr;
ConfigurationForm *configuration_form=nullptr;
ObjectRenameWidget *objectrename_wgt=nullptr;
SQLAppendWidget *sqlappend_wgt=nullptr;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	map<QString, attribs_map >confs;
	map<QString, attribs_map >::iterator itr, itr_end;
	attribs_map attribs;
	BaseConfigWidget *conf_wgt=nullptr;
	PluginsConfigWidget *plugins_conf_wgt=nullptr;
  vector<ObjectType> obj_types=BaseObject::getObjectTypes(true);

	setupUi(this);
	print_dlg=new QPrintDialog(this);

  try
  {
    configuration_form=new ConfigurationForm(this, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    configuration_form->loadConfiguration();

    plugins_conf_wgt=dynamic_cast<PluginsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::PLUGINS_CONF_WGT));
    plugins_conf_wgt->installPluginsActions(nullptr, plugins_menu, this, SLOT(executePlugin(void)));
    plugins_menu->setEnabled(!plugins_menu->isEmpty());
    action_plugins->setEnabled(!plugins_menu->isEmpty());
    action_plugins->setMenu(plugins_menu);
    dynamic_cast<QToolButton *>(general_tb->widgetForAction(action_plugins))->setPopupMode(QToolButton::InstantPopup);

    conf_wgt=configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT);
    confs=conf_wgt->getConfigurationParams();

    itr=confs.begin();
    itr_end=confs.end();

    //Configuring the widget visibility according to the configurations
    while(itr!=itr_end)
    {
      attribs=itr->second;
      if(attribs.count(ParsersAttributes::PATH)!=0)
      {
        try
        {
          //Storing the file of a previous session
          if(itr->first.contains(ParsersAttributes::_FILE_) &&
             !attribs[ParsersAttributes::PATH].isEmpty())
            prev_session_files.push_back(attribs[ParsersAttributes::PATH]);

          //Creating the recent models menu
          else if(itr->first.contains(ParsersAttributes::RECENT) &&
                  !attribs[ParsersAttributes::PATH].isEmpty())
            recent_models.push_back(attribs[ParsersAttributes::PATH]);
        }
        catch(Exception &e)
        {
          msg_box.show(e);
        }
      }

      itr++;
    }

    //Enables the action to restore session when there are registered session files
    action_restore_session->setEnabled(!prev_session_files.isEmpty());
  }
  catch(Exception &e)
  {
    msg_box.show(e);
  }

	try
	{
		QDir dir;

		this->setFocusPolicy(Qt::WheelFocus);

		//Check if the temporary dir exists, if not, creates it.
		if(!dir.exists(GlobalAttributes::TEMPORARY_DIR))
			dir.mkdir(GlobalAttributes::TEMPORARY_DIR);

    about_form=new AboutForm(nullptr, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    model_export_form=new ModelExportForm(this, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    model_fix_form=new ModelFixForm(this, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    db_import_form=new DatabaseImportForm(this, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    restoration_form=new ModelRestorationForm(this, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

		oper_list_wgt=new OperationListWidget;
		model_objs_wgt=new ModelObjectsWidget;
		overview_wgt=new ModelOverviewWidget;
		model_valid_wgt=new ModelValidationWidget;
    sql_tool_wgt=new SQLToolWidget;
		obj_finder_wgt=new ObjectFinderWidget;
    update_notifier_wgt=new UpdateNotifierWidget(this);


		permission_wgt=new PermissionWidget(this);
		sourcecode_wgt=new SourceCodeWidget(this);
		textbox_wgt=new TextboxWidget(this);
		database_wgt=new DatabaseWidget(this);
		schema_wgt=new SchemaWidget(this);
		role_wgt=new RoleWidget(this);
		tablespace_wgt=new TablespaceWidget(this);
		language_wgt=new LanguageWidget(this);
		parameter_wgt=new ParameterWidget(this);
		function_wgt=new FunctionWidget(this);
		cast_wgt=new CastWidget(this);
		conversion_wgt=new ConversionWidget(this);
		domain_wgt=new DomainWidget(this);
		aggregate_wgt=new AggregateWidget(this);
		sequence_wgt=new SequenceWidget(this);
		operator_wgt=new OperatorWidget(this);
		opfamily_wgt=new OperatorFamilyWidget(this);
		opclass_wgt=new OperatorClassWidget(this);
		type_wgt=new TypeWidget(this);
		view_wgt=new ViewWidget(this);
		column_wgt=new ColumnWidget(this);
		constraint_wgt=new ConstraintWidget(this);
		rule_wgt=new RuleWidget(this);
		trigger_wgt=new TriggerWidget(this);
		index_wgt=new IndexWidget(this);
		relationship_wgt=new RelationshipWidget(this);
		table_wgt=new TableWidget(this);
		collation_wgt=new CollationWidget(this);
		extension_wgt=new ExtensionWidget(this);
    tag_wgt=new TagWidget(this);
		task_prog_wgt=new TaskProgressWidget();
		deps_refs_wgt=new ObjectDepsRefsWidget(this);
		objectrename_wgt=new ObjectRenameWidget(this);
		sqlappend_wgt=new SQLAppendWidget(this);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

  for(auto obj_tp : obj_types)
    task_prog_wgt->addIcon(obj_tp,
																QIcon(QString(":/icones/icones/") +
                                      BaseObject::getSchemaName(obj_tp) +
																			QString(".png")));

  connect(update_notifier_wgt, SIGNAL(s_visibilityChanged(bool)), action_update_found, SLOT(setChecked(bool)));
  connect(update_notifier_wgt, SIGNAL(s_updateAvailable(bool)), update_tb, SLOT(setVisible(bool)));
  connect(action_update_found,SIGNAL(toggled(bool)),this,SLOT(toggleUpdateNotifier(bool)));
  connect(action_check_update,SIGNAL(triggered()), update_notifier_wgt, SLOT(checkForUpdate()));

	connect(action_restore_session,SIGNAL(triggered(bool)),this,SLOT(restoreLastSession()));
	connect(action_exit,SIGNAL(triggered(bool)),this,SLOT(close()));
	connect(action_new_model,SIGNAL(triggered(bool)),this,SLOT(addModel()));
	connect(action_close_model,SIGNAL(triggered(bool)),this,SLOT(closeModel()));
  connect(action_fix_model,SIGNAL(triggered(bool)),model_fix_form,SLOT(exec()));

	connect(models_tbw,SIGNAL(currentChanged(int)),this,SLOT(setCurrentModel()));
	connect(action_next,SIGNAL(triggered(bool)),this,SLOT(setCurrentModel()));
	connect(action_previous,SIGNAL(triggered(bool)),this,SLOT(setCurrentModel()));
	connect(action_about,SIGNAL(triggered(bool)),about_form,SLOT(show()));
	connect(action_wiki,SIGNAL(triggered(bool)),this,SLOT(openWiki()));

	connect(action_inc_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));
	connect(action_dec_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));
	connect(action_normal_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));

	connect(action_load_model,SIGNAL(triggered(bool)),this,SLOT(loadModel()));
	connect(action_save_model,SIGNAL(triggered(bool)),this,SLOT(saveModel()));
	connect(action_save_as,SIGNAL(triggered(bool)),this,SLOT(saveModel()));
	connect(action_save_all,SIGNAL(triggered(bool)),this,SLOT(saveAllModels()));

	connect(oper_list_wgt, SIGNAL(s_operationExecuted(void)), this, SLOT(__updateDockWidgets(void)));
	connect(oper_list_wgt, SIGNAL(s_operationListUpdated(void)), this, SLOT(__updateToolsState(void)));
	connect(action_undo,SIGNAL(triggered(bool)),oper_list_wgt,SLOT(undoOperation(void)));
	connect(action_redo,SIGNAL(triggered(bool)),oper_list_wgt,SLOT(redoOperation(void)));
	connect(models_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(closeModel(int)));

	connect(action_print, SIGNAL(triggered(bool)), this, SLOT(printModel(void)));
	connect(action_configuration, SIGNAL(triggered(bool)), configuration_form, SLOT(show(void)));

	connect(database_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(database_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(updateModelTabName(void)));
	connect(schema_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(role_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(textbox_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(tablespace_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(language_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(function_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(cast_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(conversion_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(domain_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(aggregate_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(sequence_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(operator_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(opfamily_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(opclass_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(type_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(view_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(column_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(constraint_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(rule_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(trigger_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(index_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(relationship_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(table_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(collation_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(extension_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
  connect(tag_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));
	connect(permission_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));

	connect(oper_list_wgt, SIGNAL(s_operationExecuted(void)), overview_wgt, SLOT(updateOverview(void)));
	connect(configuration_form, SIGNAL(finished(int)), this, SLOT(applyConfigurations(void)));
	connect(&model_save_timer, SIGNAL(timeout(void)), this, SLOT(saveAllModels(void)));

  connect(model_fix_form, SIGNAL(s_modelLoadRequested(QString)), this, SLOT(loadModel(QString)));

	connect(action_export, SIGNAL(triggered(bool)), this, SLOT(exportModel(void)));
	connect(action_import, SIGNAL(triggered(bool)), this, SLOT(importDatabase(void)));

	window_title=this->windowTitle() + " " + GlobalAttributes::PGMODELER_VERSION;
	this->setWindowTitle(window_title);

	current_model=nullptr;
	models_tbw->setVisible(false);
	model_objs_parent->setVisible(false);
	oper_list_parent->setVisible(false);
	obj_finder_parent->setVisible(false);
	model_valid_parent->setVisible(false);
  sql_tool_parent->setVisible(false);
	bg_saving_wgt->setVisible(false);
  update_notifier_wgt->setVisible(false);
  update_tb->setVisible(false);

	QVBoxLayout *vlayout=new QVBoxLayout;
	vlayout->setContentsMargins(0,0,0,0);
	vlayout->addWidget(model_objs_wgt);
	model_objs_parent->setLayout(vlayout);

	vlayout=new QVBoxLayout;
	vlayout->setContentsMargins(0,0,0,0);
	vlayout->addWidget(oper_list_wgt);
	oper_list_parent->setLayout(vlayout);

	QHBoxLayout * hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hlayout->addWidget(model_valid_wgt);
	model_valid_parent->setLayout(hlayout);

	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hlayout->addWidget(obj_finder_wgt);
	obj_finder_parent->setLayout(hlayout);

  vlayout=new QVBoxLayout;
  vlayout->setContentsMargins(0,0,0,0);
  vlayout->addWidget(sql_tool_wgt);
  sql_tool_parent->setLayout(vlayout);

	connect(objects_btn, SIGNAL(toggled(bool)), model_objs_parent, SLOT(setVisible(bool)));
	connect(objects_btn, SIGNAL(toggled(bool)), model_objs_wgt, SLOT(setVisible(bool)));
	connect(objects_btn, SIGNAL(toggled(bool)), this, SLOT(showRightWidgetsBar(void)));
	connect(model_objs_wgt, SIGNAL(s_visibilityChanged(bool)), objects_btn, SLOT(setChecked(bool)));
	connect(model_objs_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showRightWidgetsBar()));

	connect(operations_btn, SIGNAL(toggled(bool)), oper_list_parent, SLOT(setVisible(bool)));
	connect(operations_btn, SIGNAL(toggled(bool)), oper_list_wgt, SLOT(setVisible(bool)));
	connect(operations_btn, SIGNAL(toggled(bool)), this, SLOT(showRightWidgetsBar(void)));
	connect(oper_list_wgt, SIGNAL(s_visibilityChanged(bool)), operations_btn, SLOT(setChecked(bool)));
	connect(oper_list_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showRightWidgetsBar()));

	connect(validation_btn, SIGNAL(toggled(bool)), model_valid_parent, SLOT(setVisible(bool)));
	connect(validation_btn, SIGNAL(toggled(bool)), model_valid_wgt, SLOT(setVisible(bool)));
	connect(validation_btn, SIGNAL(toggled(bool)), this, SLOT(showBottomWidgetsBar(void)));
	connect(model_valid_wgt, SIGNAL(s_visibilityChanged(bool)), validation_btn, SLOT(setChecked(bool)));
	connect(model_valid_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showBottomWidgetsBar()));

	connect(find_obj_btn, SIGNAL(toggled(bool)), obj_finder_parent, SLOT(setVisible(bool)));
	connect(find_obj_btn, SIGNAL(toggled(bool)), obj_finder_wgt, SLOT(setVisible(bool)));
	connect(find_obj_btn, SIGNAL(toggled(bool)), this, SLOT(showBottomWidgetsBar(void)));
	connect(obj_finder_wgt, SIGNAL(s_visibilityChanged(bool)), find_obj_btn, SLOT(setChecked(bool)));
	connect(obj_finder_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showBottomWidgetsBar()));

  connect(sql_tool_btn, SIGNAL(toggled(bool)), sql_tool_parent, SLOT(setVisible(bool)));
  connect(sql_tool_btn, SIGNAL(toggled(bool)), sql_tool_wgt, SLOT(setVisible(bool)));
  connect(sql_tool_wgt, SIGNAL(s_visibilityChanged(bool)), sql_tool_btn, SLOT(setChecked(bool)));

	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), this->main_menu_mb, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), control_tb, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), general_tb, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), models_tbw, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), oper_list_wgt, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), model_objs_wgt, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), obj_finder_wgt, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), models_tbw, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), this, SLOT(stopTimers(bool)));

	connect(&tmpmodel_save_timer, SIGNAL(timeout()), &tmpmodel_thread, SLOT(start()));
	connect(&tmpmodel_thread, SIGNAL(started()), this, SLOT(saveTemporaryModels()));
  connect(&tmpmodel_thread, &QThread::started, [=](){ tmpmodel_thread.setPriority(QThread::LowPriority); });

	models_tbw_parent->resize(QSize(models_tbw_parent->maximumWidth(), models_tbw_parent->height()));

	//Forcing the splitter that handles the bottom widgets to resize its children to their minimum size
  v_splitter1->setSizes({500, 250, 500});

	showRightWidgetsBar();
	showBottomWidgetsBar();


	//Restore temporary models (if exists)
	if(restoration_form->hasTemporaryModels())
	{
		restoration_form->exec();

		if(restoration_form->result()==QDialog::Accepted)
		{
			ModelWidget *model=nullptr;
			QString model_file;
			QStringList tmp_models=restoration_form->getSelectedModels();

			while(!tmp_models.isEmpty())
			{
				try
				{
					model_file=tmp_models.front();
					tmp_models.pop_front();
					this->addModel(model_file);

					//Get the model widget generated from file
					model=dynamic_cast<ModelWidget *>(models_tbw->widget(models_tbw->count()-1));

					//Set the model as modified forcing the user to save when the autosave timer ends
					model->modified=true;
					model->filename.clear();
					restoration_form->removeTemporaryModel(model_file);
				}
				catch(Exception &e)
				{
					//Destroy the temp file if the "keep  models" isn't checked
					if(!restoration_form->keep_models_chk->isChecked())
						restoration_form->removeTemporaryModel(model_file);

					msg_box.show(e);
				}
			}
		}
	}

	//If a previous session was restored save the temp models
	saveTemporaryModels(true);
	updateConnections();
	updateRecentModelsMenu();
	applyConfigurations();

	//Temporary models are saved every two minutes
	tmpmodel_save_timer.setInterval(120000);
}

void MainWindow::showRightWidgetsBar(void)
{
	right_wgt_bar->setVisible(objects_btn->isChecked() || operations_btn->isChecked());
}

void MainWindow::showBottomWidgetsBar(void)
{
	bottom_wgt_bar->setVisible(validation_btn->isChecked() || find_obj_btn->isChecked());
}

void MainWindow::restoreLastSession(void)
{
	/* Loading the files from the previous session. The session will be restored only
	if pgModeler is not on model restore mode or pgModeler is not opening a model clicked by user
	o the file manager */
	if(QApplication::arguments().size() <= 1 &&
		 !prev_session_files.isEmpty() && restoration_form->result()==QDialog::Rejected)
	{
		try
		{
			while(!prev_session_files.isEmpty())
			{
				this->addModel(prev_session_files.front());
				prev_session_files.pop_front();
			}

			saveTemporaryModels(true);
			action_restore_session->setEnabled(false);
		}
		catch(Exception &e)
		{
			msg_box.show(e);
		}
	}
}

void MainWindow::stopTimers(bool value)
{
	if(value)
	{
		tmpmodel_save_timer.stop();
		model_save_timer.stop();
    tmpmodel_thread.quit();
	}
	else
	{
		tmpmodel_save_timer.start();
		model_save_timer.start();
  }
}

MainWindow::~MainWindow(void)
{
	delete(overview_wgt);
	delete(about_form);
}

void MainWindow::showEvent(QShowEvent *)
{
  GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));
  map<QString, attribs_map> confs=conf_wgt->getConfigurationParams();

  #ifndef Q_OS_MAC
    QTimer::singleShot(1000, conf_wgt, SLOT(updateFileAssociation()));
  #endif

  //Enabling update check at startup
  if(confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::CHECK_UPDATE]==ParsersAttributes::_TRUE_)
    QTimer::singleShot(2000, update_notifier_wgt, SLOT(checkForUpdate()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	//pgModeler will not close when the validation thread is still running
	if(model_valid_wgt->validation_thread->isRunning())
		event->ignore();
	else
	{
		GeneralConfigWidget *conf_wgt=nullptr;
		map<QString, attribs_map > confs;
		bool modified=false;
		int i=0;

		//Stops the saving timers as well the temp. model saving thread before close pgmodeler
		model_save_timer.stop();
		tmpmodel_save_timer.stop();
		tmpmodel_thread.quit();

		//Checking if there is modified models and ask the user to save them before close the application
		if(models_tbw->count() > 0)
		{
			i=0;
			while(i < models_tbw->count() && !modified)
				modified=dynamic_cast<ModelWidget *>(models_tbw->widget(i++))->isModified();

			if(modified)
			{
				msg_box.show(trUtf8("Save all models"),
										 trUtf8("Some models were modified! Do you really want to quit pgModeler without save them?"),
										 Messagebox::CONFIRM_ICON,Messagebox::YES_NO_BUTTONS);

				/* If the user rejects the message box the close event will be aborted
			causing pgModeler not to be finished */
				if(msg_box.result()==QDialog::Rejected)
					event->ignore();
			}
		}

		if(event->isAccepted())
		{
			int i, count;
			ModelWidget *model=nullptr;
			QString param_id;
			attribs_map attribs;

			this->overview_wgt->close();
			conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));
			confs=conf_wgt->getConfigurationParams();
			conf_wgt->removeConfigurationParams();

			//Saving the session
			count=models_tbw->count();
			for(i=0; i < count; i++)
			{
				model=dynamic_cast<ModelWidget *>(models_tbw->widget(i));

				if(!model->getFilename().isEmpty())
				{
					param_id=QString("%1%2").arg(ParsersAttributes::_FILE_).arg(i);
					attribs[ParsersAttributes::ID]=param_id;
					attribs[ParsersAttributes::PATH]=model->getFilename();
					conf_wgt->addConfigurationParam(param_id, attribs);
					attribs.clear();
				}
			}

			//Saving recent models list
			if(!recent_models.isEmpty())
			{
				int i=0;
				QString param_id;
				attribs_map attribs;

				while(!recent_models.isEmpty())
				{
					param_id=QString("%1%2").arg(ParsersAttributes::RECENT).arg(i++);
					attribs[ParsersAttributes::ID]=param_id;
					attribs[ParsersAttributes::PATH]=recent_models.front();
					conf_wgt->addConfigurationParam(param_id, attribs);
					attribs.clear();
					recent_models.pop_front();
				}
			}

			conf_wgt->saveConfiguration();
			restoration_form->removeTemporaryModels();

			//Remove import log files
			QDir dir(GlobalAttributes::TEMPORARY_DIR);
			QStringList log_files;

			dir.setNameFilters({"*.log"});
			log_files=dir.entryList(QDir::Files);

			while(!log_files.isEmpty())
			{
				dir.remove(log_files.front());
				log_files.pop_front();
			}
		}
  }
}

void MainWindow::updateConnections(void)
{
	ConnectionsConfigWidget *conn_cfg_wgt=nullptr;
	map<QString, Connection *> connections;

	conn_cfg_wgt=dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT));
	conn_cfg_wgt->getConnections(connections);
	model_valid_wgt->updateConnections(connections);
  sql_tool_wgt->updateConnections(connections);
}

void MainWindow::saveTemporaryModels(bool force)
{
	try
	{
		ModelWidget *model=nullptr;
    int count=models_tbw->count();

    if(count > 0 && (force || this->isActiveWindow()))
		{
			bg_saving_wgt->setVisible(true);
			bg_saving_pb->setValue(0);
			bg_saving_lbl->setText(trUtf8("Saving temp. models"));
			bg_saving_wgt->repaint();

			for(int i=0; i < count; i++)
			{
				model=dynamic_cast<ModelWidget *>(models_tbw->widget(i));
				bg_saving_pb->setValue(((i+1)/static_cast<float>(count)) * 100);

				if(model->isModified())
					model->getDatabaseModel()->saveModel(model->getTempFilename(), SchemaParser::XML_DEFINITION);

				QThread::msleep(200);
			}

			bg_saving_pb->setValue(100);
			bg_saving_wgt->setVisible(false);
		}

		tmpmodel_thread.quit();
	}
	catch(Exception &e)
	{
		tmpmodel_thread.quit();
		msg_box.show(e);
	}
}

void MainWindow::updateRecentModelsMenu(void)
{
	recent_mdls_menu.clear();
	recent_models.removeDuplicates();

	for(int i=0; i < recent_models.size() && i < MAX_RECENT_MODELS; i++)
		recent_mdls_menu.addAction(recent_models[i],this,SLOT(loadRecentModel(void)));

	if(!recent_mdls_menu.isEmpty())
	{
		recent_mdls_menu.addSeparator();
		recent_mdls_menu.addAction(trUtf8("Clear Menu"), this, SLOT(clearRecentModelsMenu(void)));
		action_recent_models->setMenu(&recent_mdls_menu);
		dynamic_cast<QToolButton *>(control_tb->widgetForAction(action_recent_models))->setPopupMode(QToolButton::InstantPopup);
	}

	action_recent_models->setEnabled(!recent_mdls_menu.isEmpty());
}

void MainWindow::loadRecentModel(void)
{
	QAction *act=dynamic_cast<QAction *>(sender());

	if(act)
	{
		addModel(act->text());
		saveTemporaryModels(true);
	}
}

void MainWindow::clearRecentModelsMenu(void)
{
	recent_models.clear();
	updateRecentModelsMenu();
}

void MainWindow::addModel(const QString &filename)
{
	ModelWidget *model_tab=nullptr;
	QString obj_name, tab_name, str_aux;
	Schema *public_sch=nullptr;
	bool start_timers=(models_tbw->count() == 0);

	//Set a name for the tab widget
	str_aux=QString("%1").arg(models_tbw->count());
	obj_name="model_";
	obj_name+=str_aux;
	tab_name=obj_name;

	model_tab=new ModelWidget;
	model_tab->setObjectName(Utf8String::create(obj_name));

	//Add the tab to the tab widget
	obj_name=model_tab->db_model->getName();

	models_tbw->blockSignals(true);
	models_tbw->addTab(model_tab, Utf8String::create(obj_name));
	models_tbw->setCurrentIndex(models_tbw->count()-1);
	models_tbw->blockSignals(false);

	//Creating the system objects (public schema and languages C, SQL and pgpgsql)
	model_tab->db_model->createSystemObjects(filename.isEmpty());
	model_tab->db_model->setInvalidated(false);

	if(!filename.isEmpty())
	{
		try
		{
			model_tab->loadModel(filename);
			models_tbw->setTabToolTip(models_tbw->currentIndex(), filename);
			//Get the "public" schema and set as system object
			public_sch=dynamic_cast<Schema *>(model_tab->db_model->getObject("public", OBJ_SCHEMA));
			if(public_sch)	public_sch->setSystemObject(true);

			models_tbw->setTabText(models_tbw->currentIndex(),
															Utf8String::create(model_tab->db_model->getName()));

      models_tbw->setVisible(true);
      model_tab->restoreLastCanvasPosition();
		}
		catch(Exception &e)
		{
			model_tab->setParent(nullptr);

			//Destroy the temp file generated by allocating a new model widget
			restoration_form->removeTemporaryModel(model_tab->getTempFilename());

			delete(model_tab);
      updateToolsState(true);

			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
	else
		models_tbw->setTabToolTip(models_tbw->currentIndex(), trUtf8("(model not saved yet)"));

	//The model is set to modified when no model file is loaded
	model_tab->setModified(filename.isEmpty());
	setCurrentModel();

	if(start_timers)
	{
		if(model_save_timer.interval() > 0)
			model_save_timer.start();

		tmpmodel_save_timer.start();
	}
}

void MainWindow::addModel(ModelWidget *model_wgt)
{
	if(!model_wgt)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(model_wgt->parent())
		throw Exception(ERR_ASG_WGT_ALREADY_HAS_PARENT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	models_tbw->blockSignals(true);
	models_tbw->addTab(model_wgt, Utf8String::create(model_wgt->getDatabaseModel()->getName()));
	models_tbw->setCurrentIndex(models_tbw->count()-1);
	models_tbw->blockSignals(false);
	setCurrentModel();
}

int MainWindow::getModelCount(void)
{
	return(models_tbw->count());
}

ModelWidget *MainWindow::getModel(int idx)
{
	if(idx < 0 || idx > models_tbw->count())
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(dynamic_cast<ModelWidget *>(models_tbw->widget(idx)));
}

void MainWindow::setCurrentModel(void)
{
	QObject *object=nullptr;
	QList<QAction *> act_list;
	QToolButton *tool_btn=nullptr;

	object=sender();
	models_tbw->setVisible(models_tbw->count() > 0);

	//Removing model specific actions from general toolbar
	act_list=general_tb->actions();
	while(act_list.size() > 5)
	{
		general_tb->removeAction(act_list.back());
		act_list.pop_back();
	}

	edit_menu->clear();
	edit_menu->addAction(action_undo);
	edit_menu->addAction(action_redo);
	edit_menu->addSeparator();

	if(object==action_next || object==action_previous)
	{
		models_tbw->blockSignals(true);

		if(object==action_next)
			models_tbw->setCurrentIndex(models_tbw->currentIndex()+1);
		else if(object==action_previous)
			models_tbw->setCurrentIndex(models_tbw->currentIndex()-1);

		models_tbw->blockSignals(false);
	}

	//Avoids the tree state saving in order to restore the current model tree state
	model_objs_wgt->saveTreeState(false);

	//Restore the tree state
	if(current_model)
		model_objs_wgt->saveTreeState(model_tree_states[current_model]);

	current_model=dynamic_cast<ModelWidget *>(models_tbw->currentWidget());

	if(current_model)
	{
    //this->applyZoom();

		current_model->setFocus(Qt::OtherFocusReason);
		current_model->cancelObjectAddition();

		general_tb->addAction(current_model->action_new_object);
		tool_btn=dynamic_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_new_object));
		tool_btn->setPopupMode(QToolButton::InstantPopup);
    //tool_btn->setShortcut(QKeySequence("N"));

		general_tb->addAction(current_model->action_quick_actions);
		tool_btn=dynamic_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_quick_actions));
		tool_btn->setPopupMode(QToolButton::InstantPopup);
    //tool_btn->setShortcut(QKeySequence("Q"));

		general_tb->addAction(current_model->action_edit);
		general_tb->addAction(current_model->action_source_code);
		general_tb->addAction(current_model->action_select_all);

		edit_menu->addAction(current_model->action_copy);
		edit_menu->addAction(current_model->action_cut);
		edit_menu->addAction(current_model->action_paste);
		edit_menu->addAction(current_model->action_remove);

		if(current_model->getFilename().isEmpty())
			this->setWindowTitle(window_title);
		else
			this->setWindowTitle(window_title + " - " + QDir::toNativeSeparators(current_model->getFilename()));

		connect(current_model, SIGNAL(s_objectsMoved(void)),oper_list_wgt, SLOT(updateOperationList(void)));
		connect(current_model, SIGNAL(s_objectModified(void)),this, SLOT(__updateDockWidgets(void)));
		connect(current_model, SIGNAL(s_objectCreated(void)),this, SLOT(__updateDockWidgets(void)));
		connect(current_model, SIGNAL(s_objectRemoved(void)),this, SLOT(__updateDockWidgets(void)));

		connect(current_model, SIGNAL(s_zoomModified(float)), this, SLOT(updateToolsState(void)));
		connect(current_model, SIGNAL(s_objectModified(void)), this, SLOT(updateModelTabName(void)));


		connect(action_alin_objs_grade, SIGNAL(triggered(bool)), this, SLOT(setGridOptions(void)));
		connect(action_show_grid, SIGNAL(triggered(bool)), this, SLOT(setGridOptions(void)));
		connect(action_show_delimiters, SIGNAL(triggered(bool)), this, SLOT(setGridOptions(void)));

		connect(action_overview, SIGNAL(toggled(bool)), this, SLOT(showOverview(bool)));
		connect(overview_wgt, SIGNAL(s_overviewVisible(bool)), action_overview, SLOT(setChecked(bool)));

		if(action_overview->isChecked())
			overview_wgt->show(current_model);
	}
	else
		this->setWindowTitle(window_title);

	edit_menu->addSeparator();
	edit_menu->addAction(action_configuration);

	updateToolsState();

	oper_list_wgt->setModel(current_model);
	model_objs_wgt->setModel(current_model);
	model_valid_wgt->setModel(current_model);
	obj_finder_wgt->setModel(current_model);

	if(current_model)
		model_objs_wgt->restoreTreeState(model_tree_states[current_model]);

	model_objs_wgt->saveTreeState(true);
}

void MainWindow::setGridOptions(void)
{
	//Configures the global settings for the scene grid
	ObjectsScene::setGridOptions(action_show_grid->isChecked(),
															 action_alin_objs_grade->isChecked(),
															 action_show_delimiters->isChecked());

	if(current_model)
	{
		//Align the object to grid is the option is checked
		if(action_alin_objs_grade->isChecked())
			current_model->scene->alignObjectsToGrid();

		//Redraw the scene to apply the new grid options
		current_model->scene->update();
	}
}

void MainWindow::applyZoom(void)
{
	if(current_model)
	{
		float zoom=current_model->getCurrentZoom();

		if(sender()==action_normal_zoom)
			zoom=1;
		else if(sender()==action_inc_zoom && zoom < ModelWidget::MAXIMUM_ZOOM)
			zoom+=ModelWidget::ZOOM_INCREMENT;
		else if(sender()==action_dec_zoom && zoom > ModelWidget::MINIMUM_ZOOM)
			zoom-=ModelWidget::ZOOM_INCREMENT;

		current_model->applyZoom(zoom);
	}
}

void MainWindow::closeModel(int model_id)
{
	QWidget *tab=nullptr;

	if(model_id >= 0)
		tab=models_tbw->widget(model_id);
	else
		tab=models_tbw->currentWidget();

	if(tab)
	{
		ModelWidget *model=dynamic_cast<ModelWidget *>(tab);

		//Ask the user to save the model if its modified
		if(model->isModified())
		{
			msg_box.show(trUtf8("Save model"),
											trUtf8("The model was modified! Do you really want to close without save it?"),
                      Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);
		}

		if(!model->isModified() ||
			 (model->isModified() && msg_box.result()==QDialog::Accepted))
		{

			model_tree_states.erase(model);

			disconnect(tab, nullptr, oper_list_wgt, nullptr);
			disconnect(tab, nullptr, model_objs_wgt, nullptr);
			disconnect(tab, nullptr, this, nullptr);
			disconnect(action_alin_objs_grade, nullptr, this, nullptr);
			disconnect(action_show_grid, nullptr, this, nullptr);
			disconnect(action_show_delimiters, nullptr, this, nullptr);

			//Remove the temporary file related to the closed model
			QDir arq_tmp;
			arq_tmp.remove(model->getTempFilename());

			if(model_id >= 0)
				models_tbw->removeTab(model_id);
			else
				models_tbw->removeTab(models_tbw->currentIndex());

			delete(model);
		}
	}

	if(models_tbw->count()==0)
	{
		current_model=nullptr;
		model_objs_wgt->setModel(static_cast<DatabaseModel *>(nullptr));
		oper_list_wgt->setModel(static_cast<ModelWidget *>(nullptr));
		updateToolsState(true);
		model_save_timer.stop();
		tmpmodel_save_timer.stop();
	}
	else
	{
		setCurrentModel();
	}
}

void MainWindow::updateModelTabName(void)
{
	if(current_model && current_model->db_model->getName()!=models_tbw->tabText(models_tbw->currentIndex()))
	{
		models_tbw->setTabText(models_tbw->currentIndex(), Utf8String::create(current_model->db_model->getName()));
	}
}

void MainWindow::applyConfigurations(void)
{
	if(!sender() ||
		 (sender()==configuration_form && configuration_form->result()==QDialog::Accepted))
	{
		GeneralConfigWidget *conf_wgt=nullptr;
		int count, i;
		ModelWidget *model=nullptr;

		conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));

		//Disable the auto save if the option is not checked
		if(!conf_wgt->autosave_interv_chk->isChecked())
		{
			//Stop the save timer
			model_save_timer.stop();
			model_save_timer.setInterval(0);
		}
		else
		{
      model_save_timer.setInterval(conf_wgt->autosave_interv_spb->value() * 60000);
			model_save_timer.start();
		}

		//Force the update of all opened models
		count=models_tbw->count();
		for(i=0; i < count; i++)
		{
			model=dynamic_cast<ModelWidget *>(models_tbw->widget(i));
			model->db_model->setObjectsModified();
			model->update();
		}

		updateConnections();
	}
}


void MainWindow::saveAllModels(void)
{
  if(models_tbw->count() > 0 &&
     ((sender()==action_save_all) ||
      (sender()==&model_save_timer &&	this->isActiveWindow())))

	{
		int i, count;

		count=models_tbw->count();
		for(i=0; i < count; i++)
			this->saveModel(dynamic_cast<ModelWidget *>(models_tbw->widget(i)));
	}
}

void MainWindow::saveModel(ModelWidget *model)
{
	try
	{
		if(!model) model=current_model;

		if(model)
		{
      if(model->getDatabaseModel()->isInvalidated())
      {
        msg_box.show(trUtf8("Confirmation"),
                     trUtf8(" <strong>WARNING:</strong> The model <strong>%1</strong> is invalidated and it's extremely recommended that it be validated before save. Ignoring this situation can generate a broken model that will need manual fixes to be loadable again!").arg(model->getDatabaseModel()->getName()),
                     Messagebox::ALERT_ICON, Messagebox::ALL_BUTTONS,
                     trUtf8("Save anyway"), trUtf8("Validate"), "",
                     ":/icones/icones/salvar.png", ":/icones/icones/validation.png");

        //If the user cancel the saving force the stopping of autosave timer to give user the chance to validate the model
        if(msg_box.isCancelled())
        {
          model_save_timer.stop();

          //The autosave timer will be reactivated in 5 minutes
          QTimer::singleShot(300000, &model_save_timer, SLOT(start()));
        }
      }

			if((!model->getDatabaseModel()->isInvalidated() ||
          (model->getDatabaseModel()->isInvalidated() && msg_box.result()==QDialog::Accepted))
					 && (model->isModified() || sender()==action_save_as))
			{
				//If the action that calls the slot were the 'save as' or the model filename isn't set
				if(sender()==action_save_as || model->filename.isEmpty())
				{
					QFileDialog file_dlg;

					file_dlg.setDefaultSuffix("dbm");
					file_dlg.setWindowTitle(trUtf8("Save '%1' as...").arg(model->db_model->getName()));
					file_dlg.setNameFilter(tr("Database model (*.dbm);;All files (*.*)"));
					file_dlg.setFileMode(QFileDialog::AnyFile);
					file_dlg.setAcceptMode(QFileDialog::AcceptSave);
					file_dlg.setModal(true);

          if(file_dlg.exec()==QFileDialog::Accepted && !file_dlg.selectedFiles().isEmpty())
					{
            model->saveModel(file_dlg.selectedFiles().at(0));
            recent_models.push_front(file_dlg.selectedFiles().at(0));
            updateRecentModelsMenu();
            models_tbw->setTabToolTip(models_tbw->indexOf(model), file_dlg.selectedFiles().at(0));
          }
				}
				else
					model->saveModel();

				this->setWindowTitle(window_title + " - " + QDir::toNativeSeparators(model->getFilename()));
				model_valid_wgt->clearOutput();
			}
      //When the user click "Validate" on the message box the validation will be executed
      else if(model->getDatabaseModel()->isInvalidated() &&
              msg_box.result()==QDialog::Rejected && !msg_box.isCancelled())
      {
        validation_btn->setChecked(true);
        model_valid_wgt->validate_btn->click();
      }
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void MainWindow::exportModel(void)
{
	if(current_model)
		model_export_form->show(current_model);
}

void MainWindow::importDatabase(void)
{
	db_import_form->exec();

	if(db_import_form->result()==QDialog::Accepted && db_import_form->getModelWidget())
	 this->addModel(db_import_form->getModelWidget());
}

void MainWindow::printModel(void)
{
	if(current_model)
	{
		QPrinter *printer=nullptr;
		QPrinter::PageSize paper_size, curr_paper_size;
		QPrinter::Orientation orientation, curr_orientation;
		QRectF margins;
		QSizeF custom_size;
		qreal ml,mt,mr,mb, ml1, mt1, mr1, mb1;
		GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));

		print_dlg->setOption(QAbstractPrintDialog::PrintCurrentPage, false);
		print_dlg->setWindowTitle(trUtf8("Database model printing"));

		//Get the page configuration of the scene
		ObjectsScene::getPaperConfiguration(paper_size, orientation, margins, custom_size);

		//Get a reference to the printer
		printer=print_dlg->printer();

		//Sets the printer options based upon the configurations from the scene
    /* if(paper_size!=QPrinter::Custom)
			printer->setPaperSize(paper_size);
    else
      printer->setPaperSize(custom_size, QPrinter::DevicePixel);

		printer->setOrientation(orientation);
    printer->setPageMargins(margins.left(), margins.top(), margins.width(), margins.height(), QPrinter::Millimeter); */
    ObjectsScene::configurePrinter(printer);

		printer->getPageMargins(&mt,&ml,&mb,&mr,QPrinter::Millimeter);

		print_dlg->exec();

		//If the user confirms the printing
		if(print_dlg->result() == QDialog::Accepted)
		{
			//Checking If the user modified the default settings overriding the scene configurations
			printer->getPageMargins(&mt1,&ml1,&mb1,&mr1,QPrinter::Millimeter);
			curr_orientation=print_dlg->printer()->orientation();
			curr_paper_size=print_dlg->printer()->paperSize();

			if(ml!=ml1 || mr!=mr1 || mt!=mt1 || mb!=mb1 ||
				 orientation!=curr_orientation || curr_paper_size!=paper_size)
			{
				msg_box.show(trUtf8("Confirmation"),
												trUtf8("Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click 'No' or 'Cancel' to abort printing."),
												Messagebox::ALERT_ICON, Messagebox::ALL_BUTTONS);
			}

			if(!msg_box.isCancelled())
			{
				if(msg_box.result()==QDialog::Rejected)
				{
					//Reverting the configurations to the scene defaults
        /*	if(paper_size!=QPrinter::Custom)
						printer->setPaperSize(paper_size);
					else
            printer->setPaperSize(custom_size, QPrinter::DevicePixel);

					printer->setOrientation(orientation);
          printer->setPageMargins(margins.left(), margins.top(), margins.width(), margins.height(), QPrinter::Millimeter); */
          ObjectsScene::configurePrinter(printer);
				}

				current_model->printModel(printer, conf_wgt->print_grid_chk->isChecked(), conf_wgt->print_pg_num_chk->isChecked());
			}
		}
	}
}

void MainWindow::loadModel(void)
{
	QFileDialog file_dlg;

	try
	{
		file_dlg.setNameFilter(trUtf8("Database model (*.dbm);;All files (*.*)"));
		file_dlg.setWindowIcon(QPixmap(QString(":/icones/icones/pgsqlModeler48x48.png")));
		file_dlg.setWindowTitle(trUtf8("Load model"));
		file_dlg.setFileMode(QFileDialog::ExistingFiles);
		file_dlg.setAcceptMode(QFileDialog::AcceptOpen);

		if(file_dlg.exec()==QFileDialog::Accepted)
			loadModels(file_dlg.selectedFiles());
	}
	catch(Exception &e)
	{
		msg_box.show(e);
  }
}

void MainWindow::loadModel(const QString &filename)
{
  loadModels({ filename });
}

void MainWindow::loadModels(const QStringList &list)
{
	int i=0;

	try
	{
		for(i=0; i < list.count(); i++)
		{
			addModel(list[i]);
			recent_models.push_front(list[i]);
		}

		updateRecentModelsMenu();
		saveTemporaryModels(true);
	}
	catch(Exception &e)
	{
		msg_box.show(Exception(Exception::getErrorMessage(ERR_MODEL_FILE_NOT_LOADED).arg(list[i]),
                 ERR_MODEL_FILE_NOT_LOADED ,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e),
                 trUtf8("Could not load the database model file `%1'. Check the error stack to see details. You can try to fix it in order to make it loadable again.").arg(list[i]),
                 Messagebox::ERROR_ICON, Messagebox::YES_NO_BUTTONS,
                 trUtf8("Fix model"), trUtf8("Cancel"), "",
                 ":/icones/icones/fixobject.png", ":/icones/icones/msgbox_erro.png");

    if(msg_box.result()==QDialog::Accepted)
    {
      QFileInfo fi(list[i]);
      model_fix_form->input_file_edt->setText(list[i]);
      model_fix_form->output_file_edt->setText(fi.absolutePath() + GlobalAttributes::DIR_SEPARATOR + fi.baseName() + "_fixed." + fi.suffix());
      model_fix_form->exec();
    }
	}
}

void MainWindow::__updateToolsState(void)
{
	updateToolsState(false);
}

void MainWindow::updateToolsState(bool model_closed)
{
	bool enabled=!model_closed;

	action_print->setEnabled(enabled);
	action_save_as->setEnabled(enabled);
	action_save_model->setEnabled(enabled);
	action_save_all->setEnabled(enabled);
	action_export->setEnabled(enabled);
	action_close_model->setEnabled(enabled);
	action_show_grid->setEnabled(enabled);
	action_show_delimiters->setEnabled(enabled);
	action_overview->setEnabled(enabled);

	action_normal_zoom->setEnabled(enabled);
	action_inc_zoom->setEnabled(enabled);
	action_dec_zoom->setEnabled(enabled);
	action_alin_objs_grade->setEnabled(enabled);
	action_undo->setEnabled(enabled);
	action_redo->setEnabled(enabled);

	if(!model_closed && current_model && models_tbw->count() > 0)
	{
		action_previous->setEnabled(models_tbw->currentIndex() > 0 &&
																models_tbw->count() > 1);
		action_next->setEnabled(models_tbw->currentIndex() >= 0 &&
															 models_tbw->currentIndex()!=(models_tbw->count()-1));
		action_undo->setEnabled(current_model->op_list->isUndoAvailable());
		action_redo->setEnabled(current_model->op_list->isRedoAvailable());

		action_inc_zoom->setEnabled(current_model->getCurrentZoom() <= ModelWidget::MAXIMUM_ZOOM - ModelWidget::ZOOM_INCREMENT);
		action_normal_zoom->setEnabled(current_model->getCurrentZoom()!=0);
		action_dec_zoom->setEnabled(current_model->getCurrentZoom() >= ModelWidget::MINIMUM_ZOOM + ModelWidget::ZOOM_INCREMENT);
	}
}

void MainWindow::updateDockWidgets(void)
{
	if(current_model)
	{
		oper_list_wgt->updateOperationList();
		__updateToolsState();
	}
}

void MainWindow::__updateDockWidgets(void)
{
	oper_list_wgt->updateOperationList();
	model_objs_wgt->updateObjectsView();

	/* Any operation executed over the model will reset the validation and
	the finder will execute the search again */
	model_valid_wgt->setModel(current_model);

	if(obj_finder_wgt->result_tbw->rowCount() > 0)
		obj_finder_wgt->findObjects();
}

void MainWindow::executePlugin(void)
{
	QAction *action=dynamic_cast<QAction *>(sender());

	if(action)
	{
		PgModelerPlugin *plugin=reinterpret_cast<PgModelerPlugin *>(action->data().value<void *>());

		if(plugin)
			plugin->executePlugin(current_model);
	}
}

void MainWindow::showOverview(bool show)
{
	if(show && current_model && !overview_wgt->isVisible())
		overview_wgt->show(current_model);
	else if(!show)
		overview_wgt->close();
}

void MainWindow::openWiki(void)
{
	msg_box.show(trUtf8("Open Wiki pages"),
							 trUtf8("This action will open a web browser window! Want to proceed?"),
							 Messagebox::CONFIRM_ICON,Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
		QDesktopServices::openUrl(QUrl(GlobalAttributes::PGMODELER_WIKI));
}

void MainWindow::toggleUpdateNotifier(bool show)
{
  if(show)
  {
    QAction *action=qobject_cast<QAction *>(sender());

    if(!action)
      update_notifier_wgt->move(0,0);
    else
    {
      QWidget *wgt=update_tb->widgetForAction(action);
      QPoint pos=(wgt ? wgt->pos() : QPoint(0,0));

      pos=wgt->mapTo(this, pos);
      pos.setX(pos.x() - 9);
      pos.setY(update_tb->pos().y() + update_tb->height() - 9);
      update_notifier_wgt->move(pos);
    }
  }

  update_notifier_wgt->setVisible(show);
}
