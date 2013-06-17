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
#include "taskprogresswidget.h"
#include "objectdepsrefswidget.h"
#include "configurationform.h"
#include "modelexportform.h"
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
TaskProgressWidget *task_prog_wgt=nullptr;
ObjectDepsRefsWidget *deps_refs_wgt=nullptr;
ConfigurationForm *configuration_form=nullptr;
ModelExportForm *export_form=nullptr;
ObjectRenameWidget *objectrename_wgt=nullptr;
SQLAppendWidget *sqlappend_wgt=nullptr;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	int i, count;
	map<QString, map<QString, QString> >confs;
	map<QString, map<QString, QString> >::iterator itr, itr_end;
	map<QString, QString> attribs;
	QStringList prev_session_files;
	BaseConfigWidget *conf_wgt=nullptr;
	PluginsConfigWidget *plugins_conf_wgt=nullptr;
	ObjectType obj_types[]={
		BASE_RELATIONSHIP,OBJ_RELATIONSHIP, OBJ_TABLE, OBJ_VIEW,
		OBJ_AGGREGATE, OBJ_OPERATOR, OBJ_INDEX, OBJ_CONSTRAINT,
		OBJ_SEQUENCE, OBJ_CONVERSION,
		OBJ_CAST, OBJ_OPFAMILY, OBJ_OPCLASS,
		BASE_RELATIONSHIP, OBJ_TEXTBOX,
		OBJ_DOMAIN, OBJ_TYPE, OBJ_FUNCTION, OBJ_SCHEMA,
		OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_ROLE,
		OBJ_RULE, OBJ_COLUMN, OBJ_TRIGGER, OBJ_INDEX,
		OBJ_CONSTRAINT, OBJ_COLLATION };

	setupUi(this);
	print_dlg=new QPrintDialog(this);

	try
	{
		QDir dir;

		this->setFocusPolicy(Qt::WheelFocus);

		//Check if the temporary dir exists, if not, creates it.
		if(!dir.exists(GlobalAttributes::TEMPORARY_DIR))
			dir.mkdir(GlobalAttributes::TEMPORARY_DIR);

		about_form=new AboutForm;
		configuration_form=new ConfigurationForm(this, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
		export_form=new ModelExportForm(this);

		restoration_form=new ModelRestorationForm(this);
		oper_list_wgt=new OperationListWidget;
		model_objs_wgt=new ModelObjectsWidget;
		overview_wgt=new ModelOverviewWidget;
		model_valid_wgt=new ModelValidationWidget;
		obj_finder_wgt=new ObjectFinderWidget;

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
		task_prog_wgt=new TaskProgressWidget();
		deps_refs_wgt=new ObjectDepsRefsWidget(this);
		objectrename_wgt=new ObjectRenameWidget(this);
		sqlappend_wgt=new SQLAppendWidget(this);
	}
	catch(Exception &e)
	{
		msg_box.show(e);
	}

	count=sizeof(obj_types)/sizeof(ObjectType);
	for(i=0; i < count; i++)
		task_prog_wgt->addIcon(obj_types[i],
																QIcon(QString(":/icones/icones/") +
																			BaseObject::getSchemaName(obj_types[i]) +
																			QString(".png")));

	connect(action_exit,SIGNAL(triggered(bool)),this,SLOT(close()));
	connect(action_new_model,SIGNAL(triggered(bool)),this,SLOT(addModel()));
	connect(action_close_model,SIGNAL(triggered(bool)),this,SLOT(closeModel()));

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
	connect(permission_wgt, SIGNAL(s_objectManipulated(void)), this, SLOT(__updateDockWidgets(void)));

	connect(oper_list_wgt, SIGNAL(s_operationExecuted(void)), overview_wgt, SLOT(updateOverview(void)));
	connect(configuration_form, SIGNAL(finished(int)), this, SLOT(applyConfigurations(void)));
	connect(&model_save_timer, SIGNAL(timeout(void)), this, SLOT(saveAllModels(void)));
	connect(&tmpmodel_save_timer, SIGNAL(timeout(void)), this, SLOT(saveTemporaryModel()));
	connect(action_export, SIGNAL(triggered(bool)), this, SLOT(exportModel(void)));

	window_title=this->windowTitle() + " " + GlobalAttributes::PGMODELER_VERSION;
	this->setWindowTitle(window_title);

	current_model=nullptr;
	models_tbw->setVisible(false);
	model_objs_parent->setVisible(false);
	oper_list_parent->setVisible(false);
	obj_finder_parent->setVisible(false);
	model_valid_parent->setVisible(false);

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

	models_tbw_parent->resize(QSize(models_tbw_parent->maximumWidth(), models_tbw_parent->height()));

	//Forcing the splitter that handles the bottom widgets to resize its children to their minimum size
	QList<int> sizes;
	sizes.push_back(10);
	sizes.push_back(0);
	v_splitter1->setSizes(sizes);

	showRightWidgetsBar();
	showBottomWidgetsBar();

	try
	{
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
	}
	catch(Exception &e)
	{
		msg_box.show(e);
	}

	//Restore temporary models (if exists)
	if(restoration_form->hasTemporaryModels())
	{
		restoration_form->exec();

		if(restoration_form->result()==QDialog::Accepted)
		{
			ModelWidget *model=nullptr;

			try
			{
				QStringList tmp_models=restoration_form->getSelectedModels();

				while(!tmp_models.isEmpty())
				{
					this->addModel(tmp_models.front());

					//Get the model widget generated from file
					model=dynamic_cast<ModelWidget *>(models_tbw->widget(models_tbw->count()-1));

					//Set the model as modified forcing the user to save when the autosave timer ends
					model->modified=true;
					model->filename.clear();
					tmp_models.pop_front();
				}
			}
			catch(Exception &e)
			{
				msg_box.show(e);
			}
		}

		restoration_form->removeTemporaryModels();
	}

	/* Loading the files from the previous session. The session will be restored only
	if pgModeler is on model restore mode or pgModeler is opening a model clicked by user
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
		}
		catch(Exception &e)
		{
			msg_box.show(e);
		}
	}

	updateConnections();
	updateRecentModelsMenu();
	applyConfigurations();
}

void MainWindow::showRightWidgetsBar(void)
{
	right_wgt_bar->setVisible(objects_btn->isChecked() || operations_btn->isChecked());
}

void MainWindow::showBottomWidgetsBar(void)
{
	bottom_wgt_bar->setVisible(validation_btn->isChecked() || find_obj_btn->isChecked());
}

MainWindow::~MainWindow(void)
{
	restoration_form->removeTemporaryModels();
	delete(overview_wgt);
	delete(about_form);
}

void MainWindow::showEvent(QShowEvent *)
{
	//Starts the timer if the interval is set
	if(save_interval > 0)
		model_save_timer.start(save_interval);

	//The temporary model timer is always of 1 minute
	tmpmodel_save_timer.start(60000);

 #ifndef Q_OS_MAC
	GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));
	QTimer::singleShot(1000, conf_wgt, SLOT(updateFileAssociation()));
 #endif
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	GeneralConfigWidget *conf_wgt=nullptr;
	map<QString, map<QString, QString> > confs;
	bool save_conf=false, modified=false;
	int i=0;

	//Checking if there is modified models and ask the user to save them before close the application
	if(models_tbw->count() > 0)
	{
		i=0;
		while(i < models_tbw->count() && !modified)
			modified=dynamic_cast<ModelWidget *>(models_tbw->widget(i++))->isModified();

		if(modified)
		{
			msg_box.show(trUtf8("Save all models"),
											trUtf8("Some models were modified! Do you want to save them before exit pgModeler?"),
											Messagebox::CONFIRM_ICON,Messagebox::YES_NO_BUTTONS);

			/* If the user accepts the message box the close event will be aborted
			causing pgModeler not to be finished */
			if(msg_box.result()==QDialog::Accepted)
				event->ignore();
		}
	}

	if(event->isAccepted())
	{
		this->overview_wgt->close();
		conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));
		confs=conf_wgt->getConfigurationParams();
		conf_wgt->removeConfigurationParams();

		//Case is needed to save the session
		if(!confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_SESSION].isEmpty())
		{
			int i, count;
			ModelWidget *model=nullptr;
			QString param_id;
			map<QString, QString> attribs;

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
			save_conf=true;
		}


		//Saving recent models list
		if(!recent_models.isEmpty())
		{
			int i=0;
			QString param_id;
			map<QString, QString> attribs;

			while(!recent_models.isEmpty())
			{
				param_id=QString("%1%2").arg(ParsersAttributes::RECENT).arg(i++);
				attribs[ParsersAttributes::ID]=param_id;
				attribs[ParsersAttributes::PATH]=recent_models.front();
				conf_wgt->addConfigurationParam(param_id, attribs);
				attribs.clear();
				recent_models.pop_front();
			}
			save_conf=true;
		}

		if(save_conf)
			conf_wgt->saveConfiguration();
	}
}

void MainWindow::updateConnections(void)
{
	ConnectionsConfigWidget *conn_cfg_wgt=nullptr;
	map<QString, DBConnection *> connections;

	conn_cfg_wgt=dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT));
	conn_cfg_wgt->getConnections(connections);
	model_valid_wgt->updateConnections(connections);
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
		addModel(act->text());
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
	QLayout *layout=nullptr;

	//Set a name for the tab widget
	str_aux=QString("%1").arg(models_tbw->count());
	obj_name="model_";
	obj_name+=str_aux;
	tab_name=obj_name;

	model_tab = new ModelWidget(models_tbw);
	model_tab->setObjectName(Utf8String::create(obj_name));

	//Add the tab to the tab widget
	obj_name=model_tab->db_model->getName();
	models_tbw->addTab(model_tab, Utf8String::create(obj_name));
	models_tbw->setCurrentIndex(models_tbw->count()-1);
	layout=models_tbw->currentWidget()->layout();
	layout->setContentsMargins(4,4,4,4);

	//Creating the system objects (public schema and languages C, SQL and pgpgsql)
	model_tab->db_model->createSystemObjects(filename.isEmpty());

	if(!filename.isEmpty())
	{
		try
		{
			model_tab->loadModel(filename);

			//Get the "public" schema and set as system object
			public_sch=dynamic_cast<Schema *>(model_tab->db_model->getObject("public", OBJ_SCHEMA));
			if(public_sch)	public_sch->setSystemObject(true);

			models_tbw->setTabText(models_tbw->currentIndex(),
															Utf8String::create(model_tab->db_model->getName()));
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}

	//The model is set to modified when no model file is loaded
	model_tab->setModified(filename.isEmpty());
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
	while(act_list.size() > 4)
	{
		general_tb->removeAction(act_list.back());
		act_list.pop_back();
	}

	edit_menu->clear();
	edit_menu->addAction(action_undo);
	edit_menu->addAction(action_redo);
	edit_menu->addSeparator();

	//Update the navigation buttons
	if(object==action_next)
		models_tbw->setCurrentIndex(models_tbw->currentIndex()+1);
	else if(object==action_previous)
		models_tbw->setCurrentIndex(models_tbw->currentIndex()-1);

	//Avoids the tree state saving in order to restore the current model tree state
	model_objs_wgt->saveTreeState(false);

	//Restore the tree state
	if(current_model)
		model_objs_wgt->saveTreeState(model_tree_states[current_model]);

	current_model=dynamic_cast<ModelWidget *>(models_tbw->currentWidget());
	if(current_model)
	{
		this->applyZoom();

		current_model->setFocus(Qt::OtherFocusReason);
		current_model->cancelObjectAddition();

		general_tb->addAction(current_model->action_new_object);
		tool_btn=dynamic_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_new_object));
		tool_btn->setPopupMode(QToolButton::InstantPopup);
		tool_btn->setShortcut(QKeySequence("N"));

		general_tb->addAction(current_model->action_quick_actions);
		tool_btn=dynamic_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_quick_actions));
		tool_btn->setPopupMode(QToolButton::InstantPopup);
		tool_btn->setShortcut(QKeySequence("Q"));

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
	this->saveTemporaryModel(true);
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
		float zoom=current_model->currentZoom();

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

		//Ask the user to save the model if its modified
		if(model->isModified())
		{
			msg_box.show(trUtf8("Save model"),
											trUtf8("The model were modified! Do you want to save it before close?"),
											Messagebox::CONFIRM_ICON,Messagebox::YES_NO_BUTTONS);

			if(msg_box.result()==QDialog::Accepted)
				this->saveModel(model);
		}

		if(model_id >= 0)
			models_tbw->removeTab(model_id);
		else
			models_tbw->removeTab(models_tbw->currentIndex());

		delete(model);
	}

	if(models_tbw->count()==0)
	{
		current_model=nullptr;
		model_objs_wgt->setModel(static_cast<DatabaseModel *>(nullptr));
		oper_list_wgt->setModel(static_cast<ModelWidget *>(nullptr));
		updateToolsState(true);
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

		conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));

		//Disable the auto save if the option is not checked
		if(!conf_wgt->autosave_interv_chk->isChecked())
		{
			//Stop the save timer
			save_interval=0;
			model_save_timer.stop();
		}
		else
		{
			save_interval=conf_wgt->autosave_interv_spb->value() * 60000;
			model_save_timer.start(save_interval);
		}

		//Force the update of all opened models
		count=models_tbw->count();
		for(i=0; i < count; i++)
			dynamic_cast<ModelWidget *>(models_tbw->widget(i))->db_model->setObjectsModified();

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
			bool sql_val_checked=model_valid_wgt->sql_validation_chk->isChecked();

			if(model->isModified())
			{
				model_valid_wgt->sql_validation_chk->setChecked(false);
				model_valid_wgt->validate_btn->click();
				model_valid_wgt->sql_validation_chk->setChecked(sql_val_checked);
			}

			/* The model is saved only when is modified and is not invalidated or the user
			confirmed the of invalidated model */
			if(!model->getDatabaseModel()->isInvalidated() && (model->isModified() || sender()==action_save_as))
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

					if(file_dlg.exec()==QFileDialog::Accepted)
					{
						if(!file_dlg.selectedFiles().isEmpty())
							model->saveModel(file_dlg.selectedFiles().at(0));
					}

					recent_models.push_front(file_dlg.selectedFiles().at(0));
					updateRecentModelsMenu();
				}
				else
					model->saveModel();

				this->setWindowTitle(window_title + " - " + QDir::toNativeSeparators(model->getFilename()));
				model_valid_wgt->clearOutput();
			}
			//Aborts the model saving and show a warning message when it is invalid
			else if(model->getDatabaseModel()->isInvalidated())
			{
				validation_btn->setChecked(true);
				msg_box.show(trUtf8("Model not saved"),
										 trUtf8("Could not save the model '%1' due to some validation errors! Please, use the validation tool to fix any problem and try again.")
										 .arg(model->getDatabaseModel()->getName()),
										 Messagebox::ALERT_ICON, Messagebox::OK_BUTTON);
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
		export_form->show(current_model);
}

void MainWindow::printModel(void)
{
	if(current_model)
	{
		QPrinter *printer=nullptr;
		QPrinter::PageSize paper_size, curr_paper_size;
		QPrinter::Orientation orientation, curr_orientation;
		QRectF margins;
		qreal ml,mt,mr,mb, ml1, mt1, mr1, mb1;
		GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT));

		print_dlg->setOption(QAbstractPrintDialog::PrintCurrentPage, false);
		print_dlg->setWindowTitle(trUtf8("Database model printing"));

		//Get the page configuration of the scene
		ObjectsScene::getPageConfiguration(paper_size, orientation, margins);

		//Get a reference to the printer
		printer=print_dlg->printer();

		//Sets the printer options based upon the configurations from the scene
		printer->setPaperSize(paper_size);
		printer->setOrientation(orientation);
		printer->setPageMargins(margins.left(), margins.top(), margins.right(), margins.bottom(), QPrinter::Millimeter);
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
					printer->setPaperSize(paper_size);
					printer->setOrientation(orientation);
					printer->setPageMargins(margins.left(), margins.top(), margins.right(), margins.bottom(), QPrinter::Millimeter);
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

void MainWindow::loadModels(const QStringList &list)
{
	try
	{
		for(int i=0; i < list.count(); i++)
		{
			addModel(list[i]);
			recent_models.push_front(list[i]);
		}

		updateRecentModelsMenu();
	}
	catch(Exception &e)
	{
		closeModel(models_tbw->currentIndex());
		msg_box.show(e);
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

		action_inc_zoom->setEnabled(current_model->currentZoom() <= ModelWidget::MAXIMUM_ZOOM - ModelWidget::ZOOM_INCREMENT);
		action_normal_zoom->setEnabled(current_model->currentZoom()!=0);
		action_dec_zoom->setEnabled(current_model->currentZoom() >= ModelWidget::MINIMUM_ZOOM + ModelWidget::ZOOM_INCREMENT);
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

void MainWindow::saveTemporaryModel(bool force)
{
	if(current_model && (this->isActiveWindow() || force))
		current_model->db_model->saveModel(current_model->getTempFilename(), SchemaParser::XML_DEFINITION);
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
