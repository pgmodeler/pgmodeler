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

#include "mainwindow.h"
#include "pgmodeleruins.h"
#include "bugreportform.h"
#include "metadatahandlingform.h"
#include "sqlexecutionwidget.h"

bool MainWindow::confirm_validation=true;
int MainWindow::GeneralActionsCount=0;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	setupUi(this);
	GeneralActionsCount = general_tb->actions().size();

	map<QString, attribs_map >confs;
	map<QString, attribs_map >::iterator itr, itr_end;
	attribs_map attribs;
	PluginsConfigWidget *plugins_conf_wgt=nullptr;
	QGridLayout *grid=nullptr;

	pending_op=NoPendingOp;
	central_wgt=nullptr;

	layers_wgt = new LayersWidget(this);
	layers_wgt->setVisible(false);

	scene_info_wgt = new SceneInfoWidget(this);
	QHBoxLayout *hbox = new QHBoxLayout(scene_info_parent);
	hbox->addWidget(scene_info_wgt);
	hbox->setContentsMargins(4,4,4,4);
	scene_info_parent->setLayout(hbox);

	fix_menu.addAction(action_fix_model);
	fix_menu.addAction(action_handle_metadata);

	QToolButton *tool_btn = qobject_cast<QToolButton *>(general_tb->widgetForAction(action_fix));
	tool_btn->setMenu(&fix_menu);
	tool_btn->setPopupMode(QToolButton::InstantPopup);

	tool_btn = qobject_cast<QToolButton *>(control_tb->widgetForAction(action_arrange_objects));
	tool_btn->setMenu(&arrange_menu);
	tool_btn->setPopupMode(QToolButton::InstantPopup);
	arrange_menu.addAction(tr("Grid"), this, SLOT(arrangeObjects()));
	arrange_menu.addAction(tr("Hierarchical"), this, SLOT(arrangeObjects()));
	arrange_menu.addAction(tr("Scattered"), this, SLOT(arrangeObjects()));

	try
	{
		models_tbw->tabBar()->setVisible(false);
		general_tb->layout()->setContentsMargins(0,0,0,0);

		central_wgt=new WelcomeWidget(views_stw);
		grid=new QGridLayout;
		grid->setContentsMargins(0,0,0,0);
		grid->setSpacing(0);
		grid->addWidget(central_wgt, 0, 0);
		views_stw->widget(WelcomeView)->setLayout(grid);

		action_welcome->setData(WelcomeView);
		action_design->setData(DesignView);
		action_manage->setData(ManageView);

		sql_tool_wgt=new SQLToolWidget;
		grid=new QGridLayout;
		grid->setContentsMargins(0,0,0,0);
		grid->setSpacing(0);
		grid->addWidget(sql_tool_wgt, 0, 0);
		views_stw->widget(ManageView)->setLayout(grid);

		configuration_form=new ConfigurationForm(nullptr, Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
		PgModelerUiNs::resizeDialog(configuration_form);
		configuration_form->loadConfiguration();

		plugins_conf_wgt=dynamic_cast<PluginsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::PluginsConfWgt));
		plugins_conf_wgt->installPluginsActions(plugins_menu, this, SLOT(executePlugin()));
		plugins_conf_wgt->initPlugins(this);
		plugins_menu->setEnabled(!plugins_menu->isEmpty());
		action_plugins->setEnabled(!plugins_menu->isEmpty());
		action_plugins->setMenu(plugins_menu);

		action_more_actions->setMenu(&more_actions_menu);

		confs=GeneralConfigWidget::getConfigurationParams();
		itr=confs.begin();
		itr_end=confs.end();

		//Configuring the widget visibility according to the configurations
		while(itr!=itr_end)
		{
			attribs=itr->second;
			if(attribs.count(Attributes::Path)!=0)
			{
				try
				{
					//Storing the file of a previous session
					if(itr->first.contains(Attributes::File) &&
							!attribs[Attributes::Path].isEmpty())
						prev_session_files.push_back(attribs[Attributes::Path]);

					//Creating the recent models menu
					else if(itr->first.contains(Attributes::Recent) &&
							!attribs[Attributes::Path].isEmpty())
						recent_models.push_back(attribs[Attributes::Path]);
				}
				catch(Exception &e)
				{
					Messagebox msg_box;
					msg_box.show(e);
				}
			}

			itr++;
		}

		//Enables the action to restore session when there are registered session files
		action_restore_session->setEnabled(!prev_session_files.isEmpty());
		central_wgt->last_session_tb->setEnabled(action_restore_session->isEnabled());


	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
	}

	try
	{
		this->setFocusPolicy(Qt::WheelFocus);
		model_nav_wgt=new ModelNavigationWidget(this);

		control_tb->addWidget(model_nav_wgt);
		control_tb->addSeparator();

		control_tb->addAction(action_plugins);
		dynamic_cast<QToolButton *>(control_tb->widgetForAction(action_plugins))->setPopupMode(QToolButton::InstantPopup);

		control_tb->addAction(action_bug_report);
		control_tb->addAction(action_donate);
		control_tb->addAction(action_support);
		control_tb->addAction(action_about);
		control_tb->addAction(action_update_found);

		about_wgt=new AboutWidget(this);
		donate_wgt=new DonateWidget(this);
		restoration_form=new ModelRestorationForm(nullptr, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

#ifdef NO_UPDATE_CHECK
		update_notifier_wgt=nullptr;
#else
		update_notifier_wgt=new UpdateNotifierWidget(this);
		update_notifier_wgt->setVisible(false);
#endif

		oper_list_wgt=new OperationListWidget;
		model_objs_wgt=new ModelObjectsWidget;
		overview_wgt=new ModelOverviewWidget;
		model_valid_wgt=new ModelValidationWidget;
		obj_finder_wgt=new ObjectFinderWidget;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	connect(central_wgt->new_tb, SIGNAL(clicked()), this, SLOT(addModel()));
	connect(central_wgt->open_tb, SIGNAL(clicked()), this, SLOT(loadModel()));
	connect(central_wgt->last_session_tb, SIGNAL(clicked()), this, SLOT(restoreLastSession()));
	connect(central_wgt->support_tb, SIGNAL(clicked()), this, SLOT(openSupport()));

#ifndef NO_UPDATE_CHECK
	connect(update_notifier_wgt, SIGNAL(s_updateAvailable(bool)), action_update_found, SLOT(setVisible(bool)), Qt::QueuedConnection);
	connect(update_notifier_wgt, SIGNAL(s_updateAvailable(bool)), action_update_found, SLOT(setChecked(bool)), Qt::QueuedConnection);
	connect(update_notifier_wgt, SIGNAL(s_visibilityChanged(bool)), action_update_found, SLOT(setChecked(bool)), Qt::QueuedConnection);
	connect(action_update_found,SIGNAL(toggled(bool)),this,SLOT(toggleUpdateNotifier(bool)), Qt::QueuedConnection);
	connect(action_check_update,SIGNAL(triggered()), update_notifier_wgt, SLOT(checkForUpdate()));
#endif

	connect(action_about,SIGNAL(toggled(bool)),this,SLOT(toggleAboutWidget(bool)));
	connect(about_wgt, SIGNAL(s_visibilityChanged(bool)), action_about, SLOT(setChecked(bool)));

	connect(action_donate, SIGNAL(toggled(bool)),this,SLOT(toggleDonateWidget(bool)));
	connect(donate_wgt, SIGNAL(s_visibilityChanged(bool)), action_donate, SLOT(setChecked(bool)));

	connect(action_restore_session,SIGNAL(triggered(bool)),this,SLOT(restoreLastSession()));
	connect(action_exit,SIGNAL(triggered(bool)),this,SLOT(close()));
	connect(action_new_model,SIGNAL(triggered(bool)),this,SLOT(addModel()));
	connect(action_close_model,SIGNAL(triggered(bool)),this,SLOT(closeModel()));
	connect(action_fix_model, SIGNAL(triggered(bool)), this, SLOT(fixModel()));
	connect(action_support,SIGNAL(triggered(bool)),this,SLOT(openSupport()));

	connect(action_inc_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));
	connect(action_dec_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));
	connect(action_normal_zoom,SIGNAL(triggered(bool)),this,SLOT(applyZoom()));

	connect(action_load_model,SIGNAL(triggered(bool)),this,SLOT(loadModel()));
	connect(action_save_model,SIGNAL(triggered(bool)),this,SLOT(saveModel()));
	connect(action_save_as,SIGNAL(triggered(bool)),this,SLOT(saveModel()));
	connect(action_save_all,SIGNAL(triggered(bool)),this,SLOT(saveAllModels()));

	connect(oper_list_wgt, SIGNAL(s_operationExecuted()), this, SLOT(updateDockWidgets()));
	connect(oper_list_wgt, SIGNAL(s_operationListUpdated()), this, SLOT(__updateToolsState()));
	connect(action_undo,SIGNAL(triggered(bool)),oper_list_wgt,SLOT(undoOperation()));
	connect(action_redo,SIGNAL(triggered(bool)),oper_list_wgt,SLOT(redoOperation()));

	connect(model_nav_wgt, SIGNAL(s_modelCloseRequested(int)), this, SLOT(closeModel(int)));
	connect(model_nav_wgt, SIGNAL(s_currentModelChanged(int)), this, SLOT(setCurrentModel()));

	connect(action_print, SIGNAL(triggered(bool)), this, SLOT(printModel()));

	connect(action_configuration, &QAction::triggered, [&](){
	  GeneralConfigWidget::restoreWidgetGeometry(configuration_form);
	  configuration_form->exec();
	  GeneralConfigWidget::saveWidgetGeometry(configuration_form);
	});

	connect(oper_list_wgt, SIGNAL(s_operationExecuted()), overview_wgt, SLOT(updateOverview()));
	connect(layers_wgt, SIGNAL(s_activeLayersChanged()), overview_wgt, SLOT(updateOverview()));

	connect(configuration_form, SIGNAL(finished(int)), this, SLOT(applyConfigurations()));
	connect(configuration_form, SIGNAL(rejected()), this, SLOT(updateConnections()));

	connect(configuration_form, &ConfigurationForm::s_invalidateModelsRequested, [&](){
		//Forcing the models code invalidation if the user changes the escape comments option
		for(int idx = 0; idx < models_tbw->count(); idx++)
			dynamic_cast<ModelWidget *>(models_tbw->widget(idx))->getDatabaseModel()->setCodesInvalidated();
	});

	connect(&model_save_timer, SIGNAL(timeout()), this, SLOT(saveAllModels()));

	connect(action_export, SIGNAL(triggered(bool)), this, SLOT(exportModel()));
	connect(action_import, SIGNAL(triggered(bool)), this, SLOT(importDatabase()));
	connect(action_diff, SIGNAL(triggered(bool)), this, SLOT(diffModelDatabase()));

	connect(action_welcome, SIGNAL(toggled(bool)), this, SLOT(changeCurrentView(bool)));
	connect(action_design, SIGNAL(toggled(bool)), this, SLOT(changeCurrentView(bool)));
	connect(action_manage, SIGNAL(toggled(bool)), this, SLOT(changeCurrentView(bool)));

	connect(action_bug_report, SIGNAL(triggered()), this, SLOT(reportBug()));
	connect(action_handle_metadata, SIGNAL(triggered(bool)), this, SLOT(handleObjectsMetadata()));

	connect(model_valid_wgt, &ModelValidationWidget::s_connectionsUpdateRequest, [&](){ updateConnections(true); });
	connect(sql_tool_wgt, &SQLToolWidget::s_connectionsUpdateRequest, [&](){ updateConnections(true); });

	connect(action_compact_view, SIGNAL(toggled(bool)), this, SLOT(toggleCompactView()));

	window_title=this->windowTitle() + QString(" ") + GlobalAttributes::PgModelerVersion;

#ifdef DEMO_VERSION
	window_title+=tr(" (Demo)");
#endif

	this->setWindowTitle(window_title);

	current_model=nullptr;
	models_tbw->setVisible(false);

	model_objs_parent->setVisible(false);
	oper_list_parent->setVisible(false);
	obj_finder_parent->setVisible(false);
	model_valid_parent->setVisible(false);
	bg_saving_wgt->setVisible(false);
	about_wgt->setVisible(false);
	donate_wgt->setVisible(false);

	models_tbw_parent->lower();
	central_wgt->lower();
	v_splitter1->lower();

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
	connect(objects_btn, SIGNAL(toggled(bool)), this, SLOT(showRightWidgetsBar()));
	connect(model_objs_wgt, SIGNAL(s_visibilityChanged(bool)), objects_btn, SLOT(setChecked(bool)));
	connect(model_objs_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showRightWidgetsBar()));

	connect(operations_btn, SIGNAL(toggled(bool)), oper_list_parent, SLOT(setVisible(bool)));
	connect(operations_btn, SIGNAL(toggled(bool)), oper_list_wgt, SLOT(setVisible(bool)));
	connect(operations_btn, SIGNAL(toggled(bool)), this, SLOT(showRightWidgetsBar()));
	connect(oper_list_wgt, SIGNAL(s_visibilityChanged(bool)), operations_btn, SLOT(setChecked(bool)));
	connect(oper_list_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showRightWidgetsBar()));

	connect(validation_btn, SIGNAL(toggled(bool)), model_valid_parent, SLOT(setVisible(bool)));
	connect(validation_btn, SIGNAL(toggled(bool)), model_valid_wgt, SLOT(setVisible(bool)));
	connect(validation_btn, SIGNAL(toggled(bool)), this, SLOT(showBottomWidgetsBar()));
	connect(model_valid_wgt, SIGNAL(s_visibilityChanged(bool)), validation_btn, SLOT(setChecked(bool)));
	connect(model_valid_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showBottomWidgetsBar()));

	connect(find_obj_btn, SIGNAL(toggled(bool)), obj_finder_parent, SLOT(setVisible(bool)));
	connect(find_obj_btn, SIGNAL(toggled(bool)), obj_finder_wgt, SLOT(setVisible(bool)));
	connect(find_obj_btn, SIGNAL(toggled(bool)), this, SLOT(showBottomWidgetsBar()));
	connect(obj_finder_wgt, SIGNAL(s_visibilityChanged(bool)), find_obj_btn, SLOT(setChecked(bool)));
	connect(obj_finder_wgt, SIGNAL(s_visibilityChanged(bool)), this, SLOT(showBottomWidgetsBar()));

	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), this->main_menu_mb, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), control_tb, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), general_tb, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), models_tbw, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), oper_list_wgt, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), model_objs_wgt, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), obj_finder_wgt, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), models_tbw, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), this, SLOT(stopTimers(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), layers_btn, SLOT(setDisabled(bool)));
	connect(model_valid_wgt, SIGNAL(s_validationInProgress(bool)), layers_wgt, SLOT(close()));

	connect(model_valid_wgt, &ModelValidationWidget::s_validationCanceled, [&](){ pending_op=NoPendingOp; });
	connect(model_valid_wgt, SIGNAL(s_validationFinished(bool)), this, SLOT(executePendingOperation(bool)));
	connect(model_valid_wgt, SIGNAL(s_fixApplied()), this, SLOT(removeOperations()), Qt::QueuedConnection);
	connect(model_valid_wgt, SIGNAL(s_graphicalObjectsUpdated()), model_objs_wgt, SLOT(updateObjectsView()), Qt::QueuedConnection);

	connect(layers_btn, SIGNAL(toggled(bool)), this, SLOT(toggleLayersWidget(bool)));
	connect(layers_wgt, SIGNAL(s_visibilityChanged(bool)), layers_btn, SLOT(setChecked(bool)));

	connect(&tmpmodel_save_timer, SIGNAL(timeout()), this, SLOT(saveTemporaryModels()));

	models_tbw_parent->resize(QSize(models_tbw_parent->maximumWidth(), models_tbw_parent->height()));

	//Forcing the splitter that handles the bottom widgets to resize its children to their minimum size
	v_splitter1->setSizes({500, 250, 500});

	showRightWidgetsBar();
	showBottomWidgetsBar();

	GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GeneralConfWgt));
	confs=conf_wgt->getConfigurationParams();

	//If a previous session was restored save the temp models
	updateConnections();
	updateRecentModelsMenu();
	configureSamplesMenu();
	applyConfigurations();

	QList<QAction *> actions=general_tb->actions();
	QToolButton *btn=nullptr;
	QFont font;

	for(auto &act : actions)
	{
		btn=qobject_cast<QToolButton *>(general_tb->widgetForAction(act));

		if(btn)
		{
			PgModelerUiNs::configureWidgetFont(btn, static_cast<unsigned>(PgModelerUiNs::MediumFontFactor));
			font = btn->font();
			font.setBold(true);
			btn->setFont(font);
			PgModelerUiNs::createDropShadow(btn);
		}
	}

#ifdef Q_OS_MAC
	control_tb->removeAction(action_main_menu);
	action_main_menu->setEnabled(false);
#else
	plugins_menu->menuAction()->setIconVisibleInMenu(false);
	main_menu.addMenu(file_menu);
	main_menu.addMenu(edit_menu);
	main_menu.addMenu(show_menu);
	main_menu.addMenu(plugins_menu);
	main_menu.addMenu(about_menu);
	main_menu.addSeparator();
	main_menu.addAction(action_show_main_menu);
	action_main_menu->setMenu(&main_menu);
	dynamic_cast<QToolButton *>(control_tb->widgetForAction(action_main_menu))->setPopupMode(QToolButton::InstantPopup);

	connect(action_show_main_menu, SIGNAL(triggered()), this, SLOT(showMainMenu()));
	connect(action_hide_main_menu, SIGNAL(triggered()), this, SLOT(showMainMenu()));
#endif

	actions=control_tb->actions();
	actions.append(general_tb->actions());

	for(QAction *act : actions)
	{
		if(!act->shortcut().toString().isEmpty())
			act->setToolTip(act->toolTip() + QString(" (%1)").arg(act->shortcut().toString()));
	}

	try
	{
		SQLExecutionWidget::loadSQLHistory();
	}
	catch(Exception &){}

#ifndef Q_OS_MAC
	//Restoring the canvas grid options
	action_show_grid->setChecked(confs[Attributes::Configuration][Attributes::ShowCanvasGrid]==Attributes::True);
	action_alin_objs_grade->setChecked(confs[Attributes::Configuration][Attributes::AlignObjsToGrid]==Attributes::True);
	action_show_delimiters->setChecked(confs[Attributes::Configuration][Attributes::ShowPageDelimiters]==Attributes::True);
	action_compact_view->setChecked(confs[Attributes::Configuration][Attributes::CompactView]==Attributes::True);

	ObjectsScene::setGridOptions(action_show_grid->isChecked(),
															 action_alin_objs_grade->isChecked(),
															 action_show_delimiters->isChecked());

	//Hiding/showing the main menu bar depending on the retrieved conf
	main_menu_mb->setVisible(confs[Attributes::Configuration][Attributes::ShowMainMenu]==Attributes::True);

	if(main_menu_mb->isVisible())
		file_menu->addAction(action_hide_main_menu);

	action_main_menu->setVisible(!main_menu_mb->isVisible());
#endif

	restoreDockWidgetsSettings();

	//Positioning the update notifier widget before showing it (if there is an update)
	setFloatingWidgetPos(update_notifier_wgt, action_update_found, control_tb, false);
	action_update_found->setVisible(false);
	QTimer::singleShot(1000, this, SLOT(restoreTemporaryModels()));

	//If there's no previuos geometry registered for the mainwindow display it maximized
	if(!GeneralConfigWidget::restoreWidgetGeometry(this))
	  this->setWindowState(Qt::WindowMaximized);

#ifdef NO_UPDATE_CHECK
#warning "NO UPDATE CHECK: Update checking is disabled."
#else
	//Enabling update check at startup
	if(confs[Attributes::Configuration][Attributes::CheckUpdate]==Attributes::True)
		QTimer::singleShot(10000, update_notifier_wgt, SLOT(checkForUpdate()));
#endif

#ifdef DEMO_VERSION
#warning "DEMO VERSION: demonstration version startup alert."
	QTimer::singleShot(5000, this, SLOT(showDemoVersionWarning()));
#endif
}

MainWindow::~MainWindow()
{
	//This fix the crash on exit at Mac OSX system (but not sure why) (???)
	file_menu->clear();

	delete restoration_form;
	delete overview_wgt;
	delete configuration_form;
}

void MainWindow::restoreTemporaryModels()
{
	QStringList ignored_files;

	/* If we have loaded models prior the execution of the restoration form
	 * (e.g, the user has started pgModeler from a double click on a .dbm file)
	 * we just need to ignore their temp files so the restoration form shows just files
	 * created on a previous session */
	for(int idx = 0; idx < models_tbw->count(); idx++)
		ignored_files += QFileInfo(dynamic_cast<ModelWidget *>(models_tbw->widget(idx))->getTempFilename()).fileName();

	PgModelerUiNs::resizeDialog(restoration_form);
	restoration_form->setIgnoredFiles(ignored_files);

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
					model->setModified(true);
					model->filename.clear();
					restoration_form->removeTemporaryModel(model_file);
				}
				catch(Exception &e)
				{
					//Destroy the temp file if the "keep  models" isn't checked
					if(!restoration_form->keep_models_chk->isChecked())
						restoration_form->removeTemporaryModel(model_file);

					Messagebox msg_box;
					msg_box.show(e);
				}
			}
		}
	}
}

bool MainWindow::isToolButtonsChecked(QHBoxLayout *layout, const QWidgetList &ignored_wgts)
{
	int i = 0;
	bool show = false;
	QToolButton * btn = nullptr;

	//This is currently the only way to enumerate widgets inside a layout.
	//See https://stackoverflow.com/a/27225570/7359123
	while(layout && layout->itemAt(i) && !show)
	{
		btn = dynamic_cast<QToolButton *>(layout->itemAt(i)->widget());
		i++;

		if(ignored_wgts.contains(btn))
			continue;

		if(btn && btn->isChecked())
			return true;
	}

	return false;
}

void MainWindow::showRightWidgetsBar()
{
	right_wgt_bar->setVisible(isToolButtonsChecked(vert_wgts_btns_layout));
}

void MainWindow::showBottomWidgetsBar()
{
	bottom_wgt_bar->setVisible(isToolButtonsChecked(horiz_wgts_btns_layout, {layers_btn}));
}

void MainWindow::restoreLastSession()
{
	/* Loading the files from the previous session. The session will be restored only
	if pgModeler is not on model restore mode or pgModeler is not opening a model clicked by user
	o the file manager */
	if(QApplication::arguments().size() <= 1 &&
			!prev_session_files.isEmpty() && restoration_form->result()==QDialog::Rejected)
	{
		try
		{
			qApp->setOverrideCursor(Qt::WaitCursor);

			while(!prev_session_files.isEmpty())
			{
				this->addModel(prev_session_files.front());
				prev_session_files.pop_front();
			}

			action_restore_session->setEnabled(false);
			central_wgt->last_session_tb->setEnabled(false);
			qApp->restoreOverrideCursor();
		}
		catch(Exception &e)
		{
			qApp->restoreOverrideCursor();
			Messagebox msg_box;
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
	}
	else
	{
        tmpmodel_save_timer.start();

        if(model_save_timer.interval() < InfinityInterval)
            model_save_timer.start();
	}
}

void MainWindow::fixModel(const QString &filename)
{
	ModelFixForm model_fix_form(nullptr, Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

	connect(&model_fix_form, SIGNAL(s_modelLoadRequested(QString)), this, SLOT(loadModel(QString)));

	if(!filename.isEmpty())
	{
		QFileInfo fi(filename);
		model_fix_form.input_file_sel->setSelectedFile(fi.absoluteFilePath());
		model_fix_form.output_file_sel->setSelectedFile(fi.absolutePath() + GlobalAttributes::DirSeparator + fi.baseName() + QString("_fixed.") + fi.suffix());
	}

	PgModelerUiNs::resizeDialog(&model_fix_form);
	GeneralConfigWidget::restoreWidgetGeometry(&model_fix_form);
	model_fix_form.exec();
	GeneralConfigWidget::saveWidgetGeometry(&model_fix_form);

	disconnect(&model_fix_form, nullptr, this, nullptr);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	if(central_wgt)
	{
		central_wgt->move(bg_widget->width()/2 - central_wgt->width()/2 ,
						  bg_widget->height()/2 - central_wgt->height()/2);
	}

	action_about->setChecked(false);
	action_donate->setChecked(false);
	action_update_found->setChecked(false);

	toggleLayersWidget(layers_wgt->isVisible());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	//pgModeler will not close when the validation thread is still running
	if(model_valid_wgt->isValidationRunning())
		event->ignore();
	else
	{
		GeneralConfigWidget *conf_wgt=nullptr;
		map<QString, attribs_map > confs;

		GeneralConfigWidget::saveWidgetGeometry(this);

		//Stops the saving timers as well the temp. model saving thread before close pgmodeler
		model_save_timer.stop();
		tmpmodel_save_timer.stop();
		plugins_menu->clear();

		//If not in demo version there is no confirmation before close the software
#ifndef DEMO_VERSION
		int i=0;
		QStringList model_names;
		ModelWidget *model=nullptr;

		//Checking if there is modified models and ask the user to save them before close the application
		if(models_tbw->count() > 0)
		{
			i=0;
			while(i < models_tbw->count())
			{
				model=dynamic_cast<ModelWidget *>(models_tbw->widget(i++));

				if(model->isModified())
					model_names.push_back(QString("<strong>%1</strong>").arg(model->getDatabaseModel()->getName()));
			}

			if(!model_names.isEmpty())
			{
				Messagebox msg_box;

				msg_box.show(tr("Save modified model(s)"),
							 tr("The following models were modified but not saved: %1. Do you really want to quit pgModeler?").arg(model_names.join(", ")),
							 Messagebox::ConfirmIcon,Messagebox::YesNoButtons);

				/* If the user rejects the message box the close event will be aborted
				causing pgModeler not to be finished */
				if(msg_box.result()==QDialog::Rejected)
					event->ignore();
			}
		}
#endif

		if(event->isAccepted())
		{
			int i, count;
			ModelWidget *model=nullptr;
			QString param_id;
			attribs_map attribs;

			this->overview_wgt->close();
			conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GeneralConfWgt));
			confs=conf_wgt->getConfigurationParams();

			attribs[Attributes::CompactView]=action_compact_view->isChecked() ? Attributes::True : QString();
			attribs[Attributes::ShowMainMenu]=main_menu_mb->isVisible() ? Attributes::True : QString();

			conf_wgt->addConfigurationParam(Attributes::Configuration, attribs);
			attribs.clear();

			count=models_tbw->count();

			//Remove the references to old session
			conf_wgt->removeConfigurationParam(QRegExp(QString("(%1)([0-9])+").arg(Attributes::File)));

			//Saving the session
			for(i=0; i < count; i++)
			{
				model=dynamic_cast<ModelWidget *>(models_tbw->widget(i));

				if(!model->getFilename().isEmpty())
				{
					param_id=QString("%1%2").arg(Attributes::File).arg(i);
					attribs[Attributes::Id]=param_id;
					attribs[Attributes::Path]=model->getFilename();
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
					param_id=QString("%1%2").arg(Attributes::Recent).arg(QString::number(i++).rightJustified(2, '0'));
					attribs[Attributes::Id]=param_id;
					attribs[Attributes::Path]=recent_models.front();
					conf_wgt->addConfigurationParam(param_id, attribs);
					attribs.clear();
					recent_models.pop_front();
				}

				recent_mdls_menu.clear();
			}
			else
				conf_wgt->removeConfigurationParam(QRegExp(QString("(%1)(.)+").arg(Attributes::Recent)));

			//Saving dock widgets settings
			storeDockWidgetsSettings();

			conf_wgt->saveConfiguration();
			restoration_form->removeTemporaryFiles();

			SQLExecutionWidget::saveSQLHistory();
			qApp->quit();
		}
	}
}

void MainWindow::updateConnections(bool force)
{
	ConnectionsConfigWidget *conn_cfg_wgt=
			dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::ConnectionsConfWgt));

	if(force || (!force && (conn_cfg_wgt->isConfigurationChanged() ||
							model_valid_wgt->connections_cmb->count()==0 ||
							sql_tool_wgt->connections_cmb->count()==0)))
	{
		if(sender()!=sql_tool_wgt)
		{
			ConnectionsConfigWidget::fillConnectionsComboBox(sql_tool_wgt->connections_cmb, true);
			sql_tool_wgt->clearDatabases();
		}

		if(sender()!=model_valid_wgt)
			ConnectionsConfigWidget::fillConnectionsComboBox(model_valid_wgt->connections_cmb, true, Connection::OpValidation);
	}
}

void MainWindow::saveTemporaryModels()
{
#ifdef DEMO_VERSION
#warning "DEMO VERSION: temporary model saving disabled."
#else
	try
	{
		ModelWidget *model=nullptr;
		int count=models_tbw->count();

		if(count > 0)
		{
			QApplication::setOverrideCursor(Qt::WaitCursor);
			scene_info_parent->setVisible(false);
			bg_saving_wgt->setVisible(true);
			bg_saving_pb->setValue(0);
			bg_saving_wgt->repaint();

			for(int i=0; i < count; i++)
			{
				model=dynamic_cast<ModelWidget *>(models_tbw->widget(i));
				bg_saving_pb->setValue(((i+1)/static_cast<double>(count)) * 100);

				if(model->isModified())
					model->getDatabaseModel()->saveModel(model->getTempFilename(), SchemaParser::XmlDefinition);
			}

			bg_saving_pb->setValue(100);
			bg_saving_wgt->setVisible(false);
			scene_info_parent->setVisible(true);
			QApplication::restoreOverrideCursor();
		}

		tmpmodel_save_timer.start();
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();
		Messagebox msg_box;
		msg_box.show(e);
		tmpmodel_save_timer.start();
	}
#endif
}

void MainWindow::updateRecentModelsMenu()
{
	QAction *act=nullptr;
	recent_mdls_menu.clear();
	recent_models.removeDuplicates();

	for(int i=0; i < recent_models.size() && i < GeneralConfigWidget::MaxRecentModels; i++)
	{
		act=recent_mdls_menu.addAction(QFileInfo(recent_models[i]).fileName(),this,SLOT(loadModelFromAction()));
		act->setToolTip(recent_models[i]);
		act->setData(recent_models[i]);
	}

	if(!recent_mdls_menu.isEmpty())
	{
		recent_mdls_menu.addSeparator();
		recent_mdls_menu.addAction(tr("Clear Menu"), this, SLOT(clearRecentModelsMenu()));
		action_recent_models->setMenu(&recent_mdls_menu);
		dynamic_cast<QToolButton *>(control_tb->widgetForAction(action_recent_models))->setPopupMode(QToolButton::InstantPopup);
	}

	action_recent_models->setEnabled(!recent_mdls_menu.isEmpty());
	central_wgt->recent_tb->setEnabled(action_recent_models->isEnabled());
	central_wgt->recent_tb->setMenu(recent_mdls_menu.isEmpty() ? nullptr : &recent_mdls_menu);
}

void MainWindow::loadModelFromAction()
{
	QAction *act=dynamic_cast<QAction *>(sender());

	if(act)
	{
		QString filename=act->data().toString();

		try
		{
			qApp->setOverrideCursor(Qt::WaitCursor);
			addModel(filename);
			recent_models.push_back(act->data().toString());
			updateRecentModelsMenu();
			qApp->restoreOverrideCursor();
		}
		catch(Exception &e)
		{
			qApp->restoreOverrideCursor();
			if(QFileInfo(filename).exists())
				showFixMessage(e, filename);
			else
				throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
		}
	}
}

void MainWindow::clearRecentModelsMenu()
{
	recent_models.clear();
	updateRecentModelsMenu();
}

void MainWindow::addModel(const QString &filename)
{
#ifdef DEMO_VERSION
#warning "DEMO VERSION: database model creation limit."
	if(models_tbw->count()==1)
		throw Exception(tr("The demonstration version can create only `one' instance of database model!"),
										ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
#endif

	try
	{
		ModelWidget *model_tab=nullptr;
		QString obj_name, tab_name, str_aux;
		Schema *public_sch=nullptr;
		bool start_timers=(models_tbw->count() == 0);

		//Set a name for the tab widget
		str_aux=QString("%1").arg(models_tbw->count());
		obj_name=QString("model_");
		obj_name+=str_aux;
		tab_name=obj_name;

		model_tab=new ModelWidget;
		model_tab->setObjectName(obj_name);

		//Add the tab to the tab widget
		obj_name=model_tab->db_model->getName();

		models_tbw->blockSignals(true);
		models_tbw->setUpdatesEnabled(false);
		models_tbw->addTab(model_tab, obj_name);
		models_tbw->setCurrentIndex(models_tbw->count()-1);
		models_tbw->blockSignals(false);
		models_tbw->currentWidget()->layout()->setContentsMargins(3,3,0,3);

		//Creating the system objects (public schema and languages C, SQL and pgpgsql)
		model_tab->db_model->createSystemObjects(filename.isEmpty());

		if(!filename.isEmpty())
		{
			try
			{
				model_tab->loadModel(filename);
				models_tbw->setTabToolTip(models_tbw->currentIndex(), filename);

				//Get the "public" schema and set as system object
				public_sch=dynamic_cast<Schema *>(model_tab->db_model->getObject(QString("public"), ObjectType::Schema));
				if(public_sch)	public_sch->setSystemObject(true);

				model_tab->db_model->setInvalidated(false);
				model_tab->restoreLastCanvasPosition();

				//Making a copy of the loaded database model file as the first version of the temp. model
				QFile::copy(filename, model_tab->getTempFilename());
			}
			catch(Exception &e)
			{
				models_tbw->setUpdatesEnabled(true);
				central_wgt->update();
				models_tbw->removeTab(models_tbw->indexOf(model_tab));
				model_tab->setParent(nullptr);

				//Destroy the temp file generated by allocating a new model widget
				restoration_form->removeTemporaryModel(model_tab->getTempFilename());

				updateToolsState(true);

				throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}
		}

		model_nav_wgt->addModel(model_tab);
		models_tbw->setUpdatesEnabled(true);
		models_tbw->setVisible(true);
		setCurrentModel();

		if(start_timers)
		{
			if(model_save_timer.interval() > 0)
				model_save_timer.start();

			tmpmodel_save_timer.start();
		}

		model_tab->setModified(false);
		action_save_model->setEnabled(false);

		if(action_alin_objs_grade->isChecked())
			current_model->scene->alignObjectsToGrid();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void MainWindow::addModel(ModelWidget *model_wgt)
{
	try
	{
		if(!model_wgt)
			throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(model_wgt->parent())
			throw Exception(ErrorCode::AsgWidgetAlreadyHasParent,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		model_nav_wgt->addModel(model_wgt);

		models_tbw->blockSignals(true);
		models_tbw->addTab(model_wgt, model_wgt->getDatabaseModel()->getName());
		models_tbw->setCurrentIndex(models_tbw->count()-1);
		models_tbw->blockSignals(false);
		setCurrentModel();
		models_tbw->currentWidget()->layout()->setContentsMargins(3,3,0,3);

		if(action_alin_objs_grade->isChecked())
			current_model->scene->alignObjectsToGrid();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

int MainWindow::getModelCount()
{
	return models_tbw->count();
}

ModelWidget *MainWindow::getModel(int idx)
{
	if(idx < 0 || idx > models_tbw->count())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return dynamic_cast<ModelWidget *>(models_tbw->widget(idx));
}

void MainWindow::showMainMenu()
{
	action_main_menu->setVisible(sender()!=action_show_main_menu);
	main_menu_mb->setVisible(sender()==action_show_main_menu);

	if(sender()!=action_show_main_menu)
		file_menu->removeAction(action_hide_main_menu);
	else
		file_menu->addAction(action_hide_main_menu);
}

void MainWindow::setCurrentModel()
{
	layers_wgt->setVisible(false);
	models_tbw->setVisible(models_tbw->count() > 0);
	action_design->setEnabled(models_tbw->count() > 0);

	if(models_tbw->count() > 0)
		action_design->setChecked(true);
	else
		action_welcome->setChecked(true);

	removeModelActions();

	edit_menu->clear();
	edit_menu->addAction(action_undo);
	edit_menu->addAction(action_redo);
	edit_menu->addSeparator();

	//Avoids the tree state saving in order to restore the current model tree state
	model_objs_wgt->saveTreeState(false);

	//Restore the tree state
	if(current_model)
		model_objs_wgt->saveTreeState(model_tree_states[current_model]);

	models_tbw->setCurrentIndex(model_nav_wgt->getCurrentIndex());
	current_model=dynamic_cast<ModelWidget *>(models_tbw->currentWidget());
	action_arrange_objects->setEnabled(current_model != nullptr);

	if(current_model)
	{
		QToolButton *tool_btn=nullptr;
		QList<QToolButton *> btns;
		QFont font;

		layers_wgt->setModel(current_model);
		current_model->setFocus(Qt::OtherFocusReason);
		current_model->cancelObjectAddition();
		general_tb->addSeparator();

		general_tb->addAction(current_model->action_new_object);
		tool_btn=qobject_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_new_object));
		tool_btn->setPopupMode(QToolButton::InstantPopup);
		btns.push_back(tool_btn);

		general_tb->addAction(current_model->action_quick_actions);
		tool_btn=qobject_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_quick_actions));
		tool_btn->setPopupMode(QToolButton::InstantPopup);
		btns.push_back(tool_btn);

		general_tb->addAction(current_model->action_edit);
		tool_btn=qobject_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_edit));
		btns.push_back(tool_btn);

		general_tb->addAction(current_model->action_source_code);
		tool_btn=qobject_cast<QToolButton *>(general_tb->widgetForAction(current_model->action_source_code));
		btns.push_back(tool_btn);

		configureMoreActionsMenu();
		general_tb->addAction(action_more_actions);

		tool_btn = qobject_cast<QToolButton *>(general_tb->widgetForAction(action_more_actions));
		tool_btn->setPopupMode(QToolButton::InstantPopup);
		btns.push_back(tool_btn);

		for(QToolButton *btn : btns)
		{
			PgModelerUiNs::configureWidgetFont(btn, static_cast<unsigned>(PgModelerUiNs::MediumFontFactor));
			font = btn->font();
			font.setBold(true);
			btn->setFont(font);
			PgModelerUiNs::createDropShadow(btn);
		}

		edit_menu->addAction(current_model->action_copy);
		edit_menu->addAction(current_model->action_cut);
		edit_menu->addAction(current_model->action_duplicate);
		edit_menu->addAction(current_model->action_paste);
		edit_menu->addAction(current_model->action_remove);
		edit_menu->addAction(current_model->action_cascade_del);

		if(current_model->getFilename().isEmpty())
			this->setWindowTitle(window_title);
		else
			this->setWindowTitle(window_title + QString(" - ") + QDir::toNativeSeparators(current_model->getFilename()));

		connect(current_model, SIGNAL(s_modelModified(bool)), model_nav_wgt, SLOT(setCurrentModelModified(bool)), Qt::UniqueConnection);

		connect(current_model, SIGNAL(s_manipulationCanceled()),oper_list_wgt, SLOT(updateOperationList()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_objectsMoved()),oper_list_wgt, SLOT(updateOperationList()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_objectModified()),this, SLOT(updateDockWidgets()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_objectCreated()),this, SLOT(updateDockWidgets()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_objectRemoved()),this, SLOT(updateDockWidgets()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_objectManipulated()),this, SLOT(updateDockWidgets()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_objectManipulated()), this, SLOT(updateModelTabName()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_zoomModified(double)), this, SLOT(updateToolsState()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_objectModified()), this, SLOT(updateModelTabName()), Qt::UniqueConnection);

		connect(current_model, SIGNAL(s_sceneInteracted(BaseObjectView*)), this, SLOT(configureMoreActionsMenu()), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_sceneInteracted(BaseObjectView*)), scene_info_wgt, SLOT(updateSelectedObject(BaseObjectView*)), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_sceneInteracted(int,QRectF)), scene_info_wgt, SLOT(updateSelectedObjects(int,QRectF)), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_sceneInteracted(QPointF)), scene_info_wgt, SLOT(updateMousePosition(QPointF)), Qt::UniqueConnection);
		connect(current_model, SIGNAL(s_zoomModified(double)), scene_info_wgt, SLOT(updateSceneZoom(double)), Qt::UniqueConnection);

		connect(action_alin_objs_grade, SIGNAL(triggered(bool)), this, SLOT(setGridOptions()), Qt::UniqueConnection);
		connect(action_show_grid, SIGNAL(triggered(bool)), this, SLOT(setGridOptions()), Qt::UniqueConnection);
		connect(action_show_delimiters, SIGNAL(triggered(bool)), this, SLOT(setGridOptions()), Qt::UniqueConnection);

		connect(action_overview, SIGNAL(toggled(bool)), this, SLOT(showOverview(bool)), Qt::UniqueConnection);
		connect(overview_wgt, SIGNAL(s_overviewVisible(bool)), action_overview, SLOT(setChecked(bool)), Qt::UniqueConnection);

		if(action_overview->isChecked())
			overview_wgt->show(current_model);

		scene_info_wgt->updateMousePosition(QPointF(0,0));
		scene_info_wgt->updateSceneZoom(current_model->getCurrentZoom());
		current_model->emitSceneInteracted();
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

	emit s_currentModelChanged(current_model);
}

void MainWindow::setGridOptions()
{
	GeneralConfigWidget *conf_wgt = dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GeneralConfWgt));
	map<QString, attribs_map> attribs = conf_wgt->getConfigurationParams();

	//Configures the global settings for the scene grid
	ObjectsScene::setGridOptions(action_show_grid->isChecked(),
								 action_alin_objs_grade->isChecked(),
								 action_show_delimiters->isChecked());

	attribs[Attributes::Configuration][Attributes::AlignObjsToGrid] = (action_alin_objs_grade->isChecked() ? Attributes::True : Attributes::False);
	attribs[Attributes::Configuration][Attributes::ShowCanvasGrid] = (action_show_grid->isChecked() ? Attributes::True : Attributes::False);
	attribs[Attributes::Configuration][Attributes::ShowPageDelimiters] = (action_show_delimiters->isChecked() ? Attributes::True : Attributes::False);

	if(current_model)
	{
		//Align the object to grid is the option is checked
		if(action_alin_objs_grade->isChecked())
		{
			current_model->scene->alignObjectsToGrid();

			//Forcing the relationship updating to fit the new position of the tables
			current_model->getDatabaseModel()->setObjectsModified({ ObjectType::Relationship, ObjectType::BaseRelationship });
		}

		//Redraw the scene to apply the new grid options
		current_model->scene->update();
	}

	conf_wgt->addConfigurationParam(Attributes::Configuration, attribs[Attributes::Configuration]);
}

void MainWindow::applyZoom()
{
	if(current_model)
	{
		double zoom=current_model->getCurrentZoom();

		if(sender()==action_normal_zoom)
			zoom=1;
		else if(sender()==action_inc_zoom && zoom < ModelWidget::MaximumZoom)
			zoom+=ModelWidget::ZoomIncrement;
		else if(sender()==action_dec_zoom && zoom > ModelWidget::MinimumZoom)
			zoom-=ModelWidget::ZoomIncrement;

		current_model->applyZoom(zoom);
	}
}

void MainWindow::removeModelActions()
{
	QList<QAction *> act_list;
	act_list=general_tb->actions();

	while(act_list.size() > GeneralActionsCount)
	{
		general_tb->removeAction(act_list.back());
		act_list.pop_back();
	}
}

void MainWindow::closeModel(int model_id)
{
	QWidget *tab=nullptr;

	overview_wgt->close();

	if(model_id >= 0)
		tab=models_tbw->widget(model_id);
	else
		tab=models_tbw->currentWidget();

	if(tab)
	{
		ModelWidget *model=dynamic_cast<ModelWidget *>(tab);
		Messagebox msg_box;

#ifdef DEMO_VERSION
		msg_box.setResult(QDialog::Accepted);
#else
		//Ask the user to save the model if its modified
		if(model->isModified())
		{
			msg_box.show(tr("Save model"),
						 tr("The model <strong>%1</strong> was modified! Do you really want to close without save it?").arg(model->getDatabaseModel()->getName()),
						 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);
		}
#endif

		if(!model->isModified() ||
				(model->isModified() && msg_box.result()==QDialog::Accepted))
		{
			model_nav_wgt->removeModel(model_id);
			model_tree_states.erase(model);

			disconnect(model, nullptr, nullptr, nullptr);
			disconnect(action_alin_objs_grade, nullptr, this, nullptr);
			disconnect(action_show_grid, nullptr, this, nullptr);
			disconnect(action_show_delimiters, nullptr, this, nullptr);

			//Remove the temporary file related to the closed model
			QDir arq_tmp;
			arq_tmp.remove(model->getTempFilename());

			//Removing model specific actions from general toolbar
			removeModelActions();

			if(model_id >= 0)
				models_tbw->removeTab(model_id);
			else
				models_tbw->removeTab(models_tbw->currentIndex());
		}
	}

	if(models_tbw->count()==0)
	{
		current_model=nullptr;
		setCurrentModel();
		model_save_timer.stop();
		tmpmodel_save_timer.stop();
		models_tbw->setVisible(false);
	}
	else
	{
		setCurrentModel();
	}
}

void MainWindow::updateModelTabName()
{
	if(current_model && current_model->db_model->getName()!=models_tbw->tabText(models_tbw->currentIndex()))
		model_nav_wgt->updateModelText(models_tbw->currentIndex(), current_model->db_model->getName(), current_model->getFilename());
}

void MainWindow::applyConfigurations()
{
  if(!sender() ||
			(sender()==configuration_form && configuration_form->result()==QDialog::Accepted))
	{
		GeneralConfigWidget *conf_wgt=nullptr;
		int count, i;
		ModelWidget *model=nullptr;

		conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GeneralConfWgt));

		//Disable the auto save if the option is not checked
		if(!conf_wgt->autosave_interv_chk->isChecked())
		{
			//Stop the save timer
            model_save_timer.setInterval(InfinityInterval);
            model_save_timer.stop();
		}
		else
		{
			model_save_timer.setInterval(conf_wgt->autosave_interv_spb->value() * 60000);
			model_save_timer.start();
		}

		//Temporary models are saved every five minutes
        tmpmodel_save_timer.setInterval(model_save_timer.interval() < InfinityInterval ? model_save_timer.interval()/2 : 300000);
		tmpmodel_save_timer.start();

		QApplication::setOverrideCursor(Qt::WaitCursor);

		//Force the update of all opened models
		count=models_tbw->count();
		for(i=0; i < count; i++)
		{
			model=dynamic_cast<ModelWidget *>(models_tbw->widget(i));
			model->updateObjectsOpacity();
			model->db_model->setObjectsModified();
		}

		if(current_model)
			current_model->update();

		updateConnections();
		sql_tool_wgt->configureSnippets();

		QApplication::restoreOverrideCursor();
	}

	sql_tool_wgt->updateTabs();
}


void MainWindow::saveAllModels()
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
#ifdef DEMO_VERSION
#warning "DEMO VERSION: model saving disabled."
	Messagebox msg_box;
	msg_box.show(tr("Warning"),
				 tr("You're running a demonstration version! The model saving feature is available only in the full version!"),
				 Messagebox::AlertIcon, Messagebox::OkButton);
#else
	try
	{
		if(!model) model=current_model;

		if(model)
		{
			Messagebox msg_box;
			DatabaseModel *db_model=model->getDatabaseModel();

			if(confirm_validation && db_model->isInvalidated())
			{
				msg_box.show(tr("Confirmation"),
							 tr(" <strong>WARNING:</strong> The model <strong>%1</strong> is invalidated! It's recommended to validate it before save in order to create a consistent model otherwise the generated file will be broken demanding manual fixes to be loadable again!").arg(db_model->getName()),
							 Messagebox::AlertIcon, Messagebox::AllButtons,
							 tr("Validate"), tr("Save anyway"), QString(),
								PgModelerUiNs::getIconPath("validation"), PgModelerUiNs::getIconPath("salvar"));

				//If the user cancel the saving force the stopping of autosave timer to give user the chance to validate the model
				if(msg_box.isCancelled())
				{
					model_save_timer.stop();

					//The autosave timer will be reactivated in 5 minutes
					QTimer::singleShot(300000, &model_save_timer, SLOT(start()));
				}
				else if(msg_box.result()==QDialog::Accepted)
				{
					validation_btn->setChecked(true);
					this->pending_op=(sender()==action_save_as ? PendingSaveAsOp : PendingSaveOp);
					action_design->setChecked(true);
					model_valid_wgt->validateModel();
				}
			}

			stopTimers(true);

			if((!confirm_validation ||
				(!db_model->isInvalidated() ||
				 (confirm_validation && db_model->isInvalidated() && !msg_box.isCancelled() && msg_box.result()==QDialog::Rejected)))
					&& (model->isModified() || sender()==action_save_as))
			{
				//If the action that calls the slot were the 'save as' or the model filename isn't set
				if(sender()==action_save_as || model->filename.isEmpty() || pending_op==PendingSaveAsOp)
				{
					QFileDialog file_dlg;

					file_dlg.setDefaultSuffix(QString("dbm"));
					file_dlg.setWindowTitle(tr("Save '%1' as...").arg(model->db_model->getName()));
					file_dlg.setNameFilter(tr("Database model (*.dbm);;All files (*.*)"));
					file_dlg.setFileMode(QFileDialog::AnyFile);
					file_dlg.setAcceptMode(QFileDialog::AcceptSave);
					file_dlg.setModal(true);

					if(file_dlg.exec()==QFileDialog::Accepted && !file_dlg.selectedFiles().isEmpty())
					{
						model->saveModel(file_dlg.selectedFiles().at(0));
						recent_models.push_front(file_dlg.selectedFiles().at(0));
						updateRecentModelsMenu();
						model_nav_wgt->updateModelText(models_tbw->indexOf(model), model->getDatabaseModel()->getName(), file_dlg.selectedFiles().at(0));
					}
				}
				else
					model->saveModel();

				this->setWindowTitle(window_title + QString(" - ") + QDir::toNativeSeparators(model->getFilename()));
				model_valid_wgt->clearOutput();
			}

			stopTimers(false);
			action_save_model->setEnabled(model->isModified());
		}
	}
	catch(Exception &e)
	{
		stopTimers(false);
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
#endif
}

void MainWindow::exportModel()
{
	ModelExportForm model_export_form(nullptr, Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	Messagebox msg_box;
	DatabaseModel *db_model=current_model->getDatabaseModel();

	action_design->setChecked(true);

	if(confirm_validation && db_model->isInvalidated())
	{
		msg_box.show(tr("Confirmation"),
					 tr(" <strong>WARNING:</strong> The model <strong>%1</strong> is invalidated! Before run the export process it's recommended to validate in order to correctly create the objects on database server!").arg(db_model->getName()),
					 Messagebox::AlertIcon, Messagebox::AllButtons,
					 tr("Validate"), tr("Export anyway"), QString(),
					 PgModelerUiNs::getIconPath("validation"), PgModelerUiNs::getIconPath("exportar"));

		if(msg_box.result()==QDialog::Accepted)
		{
			validation_btn->setChecked(true);
			this->pending_op=PendingExportOp;
			model_valid_wgt->validateModel();
		}
	}

	if(!confirm_validation ||
			(!db_model->isInvalidated() || (confirm_validation && !msg_box.isCancelled() && msg_box.result()==QDialog::Rejected)))
	{
		stopTimers(true);
		connect(&model_export_form, &ModelExportForm::s_connectionsUpdateRequest, [&](){ updateConnections(true); });

		PgModelerUiNs::resizeDialog(&model_export_form);
		GeneralConfigWidget::restoreWidgetGeometry(&model_export_form);
		model_export_form.exec(current_model);
		GeneralConfigWidget::saveWidgetGeometry(&model_export_form);

		stopTimers(false);
	}
}

void MainWindow::importDatabase()
{
	DatabaseImportForm db_import_form(nullptr, Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

	stopTimers(true);

	connect(&db_import_form, &DatabaseImportForm::s_connectionsUpdateRequest, [&](){ updateConnections(true); });
	db_import_form.setModelWidget(current_model);
	PgModelerUiNs::resizeDialog(&db_import_form);
	GeneralConfigWidget::restoreWidgetGeometry(&db_import_form);
	db_import_form.exec();
	GeneralConfigWidget::saveWidgetGeometry(&db_import_form);
	stopTimers(false);

	if(db_import_form.result()==QDialog::Accepted && db_import_form.getModelWidget())
		this->addModel(db_import_form.getModelWidget());
	else if(current_model)
		updateDockWidgets();
}

void MainWindow::diffModelDatabase()
{
	ModelDatabaseDiffForm modeldb_diff_frm(nullptr, Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	Messagebox msg_box;
	DatabaseModel *db_model=(current_model ? current_model->getDatabaseModel() : nullptr);

	if(current_model)
		action_design->setChecked(true);

	if(confirm_validation && db_model && db_model->isInvalidated())
	{
		msg_box.show(tr("Confirmation"),
					 tr(" <strong>WARNING:</strong> The model <strong>%1</strong> is invalidated! Before run the diff process it's recommended to validate in order to correctly analyze and generate the difference between the model and a database!").arg(db_model->getName()),
					 Messagebox::AlertIcon, Messagebox::AllButtons,
					 tr("Validate"), tr("Diff anyway"), QString(),
					 PgModelerUiNs::getIconPath("validation"), PgModelerUiNs::getIconPath("diff"));

		if(msg_box.result()==QDialog::Accepted)
		{
			validation_btn->setChecked(true);
			this->pending_op=PendingDiffOp;
			model_valid_wgt->validateModel();
		}
	}

	if(!confirm_validation || !db_model ||
			((db_model && !db_model->isInvalidated()) || (confirm_validation && !msg_box.isCancelled() && msg_box.result()==QDialog::Rejected)))
	{
		modeldb_diff_frm.setModelWidget(current_model);

		stopTimers(true);
		connect(&modeldb_diff_frm, &ModelDatabaseDiffForm::s_connectionsUpdateRequest, [&](){ updateConnections(true); });
		connect(&modeldb_diff_frm, &ModelDatabaseDiffForm::s_loadDiffInSQLTool, [&](QString conn_id, QString database, QString filename){
			action_manage->toggle();
			sql_tool_wgt->addSQLExecutionTab(conn_id, database, filename);
		});

		//PgModelerUiNs::resizeDialog(&modeldb_diff_frm);
		GeneralConfigWidget::restoreWidgetGeometry(&modeldb_diff_frm);
		modeldb_diff_frm.exec();
		GeneralConfigWidget::saveWidgetGeometry(&modeldb_diff_frm);
		stopTimers(false);
	}
}

void MainWindow::printModel()
{
	if(current_model)
	{
		QPrintDialog print_dlg;
		QPrinter *printer=nullptr;
		QPrinter::PageSize paper_size, curr_paper_size;
		QPrinter::Orientation orientation, curr_orientation;
		QRectF margins;
		QSizeF custom_size;
		qreal ml,mt,mr,mb, ml1, mt1, mr1, mb1;
		GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GeneralConfWgt));

		print_dlg.setOption(QAbstractPrintDialog::PrintCurrentPage, false);
		print_dlg.setWindowTitle(tr("Database model printing"));

		//Get the page configuration of the scene
		ObjectsScene::getPaperConfiguration(paper_size, orientation, margins, custom_size);

		//Get a reference to the printer
		printer=print_dlg.printer();

		//Sets the printer options based upon the configurations from the scene
		ObjectsScene::configurePrinter(printer);

		printer->getPageMargins(&mt,&ml,&mb,&mr,QPrinter::Millimeter);

		print_dlg.exec();

		//If the user confirms the printing
		if(print_dlg.result() == QDialog::Accepted)
		{
			Messagebox msg_box;

			//Checking If the user modified the default settings overriding the scene configurations
			printer->getPageMargins(&mt1,&ml1,&mb1,&mr1,QPrinter::Millimeter);
			curr_orientation=print_dlg.printer()->orientation();
			curr_paper_size=print_dlg.printer()->paperSize();

			if(ml!=ml1 || mr!=mr1 || mt!=mt1 || mb!=mb1 ||
					orientation!=curr_orientation || curr_paper_size!=paper_size)
			{
				msg_box.show(tr("Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click 'No' or 'Cancel' to abort printing."),
							 Messagebox::AlertIcon, Messagebox::AllButtons);
			}

			if(!msg_box.isCancelled())
			{
				if(msg_box.result()==QDialog::Rejected)
					ObjectsScene::configurePrinter(printer);

				current_model->printModel(printer, conf_wgt->print_grid_chk->isChecked(), conf_wgt->print_pg_num_chk->isChecked());
			}
		}
	}
}

void MainWindow::loadModel()
{
	QFileDialog file_dlg;

	try
	{
		file_dlg.setNameFilter(tr("Database model (*.dbm);;All files (*.*)"));
		file_dlg.setWindowIcon(QPixmap(PgModelerUiNs::getIconPath("pgsqlModeler48x48")));
		file_dlg.setWindowTitle(tr("Load model"));
		file_dlg.setFileMode(QFileDialog::ExistingFiles);
		file_dlg.setAcceptMode(QFileDialog::AcceptOpen);

		if(file_dlg.exec()==QFileDialog::Accepted)
			loadModels(file_dlg.selectedFiles());
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
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
		qApp->setOverrideCursor(Qt::WaitCursor);

		for(i=0; i < list.count(); i++)
		{
			addModel(list[i]);
			recent_models.push_front(list[i]);
		}

		updateRecentModelsMenu();		
		qApp->restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		qApp->restoreOverrideCursor();
		showFixMessage(e, list[i]);
	}
}

void MainWindow::showFixMessage(Exception &e, const QString &filename)
{
	Messagebox msg_box;

	msg_box.show(Exception(Exception::getErrorMessage(ErrorCode::ModelFileNotLoaded).arg(filename),
												 ErrorCode::ModelFileNotLoaded ,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e),
							 tr("Could not load the database model file `%1'. Check the error stack to see details. You can try to fix it in order to make it loadable again.").arg(filename),
							 Messagebox::ErrorIcon, Messagebox::YesNoButtons,
							 tr("Fix model"), tr("Cancel"), QString(),
							 PgModelerUiNs::getIconPath("fixobject"), PgModelerUiNs::getIconPath("msgbox_erro"));

	if(msg_box.result()==QDialog::Accepted)
		fixModel(filename);
}

void MainWindow::setConfirmValidation(bool value)
{
	confirm_validation=value;
}

void MainWindow::__updateToolsState()
{
	updateToolsState(false);
}

void MainWindow::updateToolsState(bool model_closed)
{
	bool enabled=(!model_closed && current_model);

	action_print->setEnabled(enabled);
	action_save_as->setEnabled(enabled);
	action_save_model->setEnabled(!model_closed && current_model && current_model->isModified());
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
	action_compact_view->setEnabled(enabled);

	action_handle_metadata->setEnabled(enabled);

	if(!model_closed && current_model && models_tbw->count() > 0)
	{
		action_undo->setEnabled(current_model->op_list->isUndoAvailable());
		action_redo->setEnabled(current_model->op_list->isRedoAvailable());

		action_inc_zoom->setEnabled(current_model->getCurrentZoom() <= (ModelWidget::MaximumZoom - ModelWidget::ZoomIncrement));
		action_normal_zoom->setEnabled(current_model->getCurrentZoom()!=0);
		action_dec_zoom->setEnabled(current_model->getCurrentZoom() >= ModelWidget::MinimumZoom);
	}
}

void MainWindow::updateDockWidgets()
{
	oper_list_wgt->updateOperationList();
	model_objs_wgt->updateObjectsView();

	/* Any operation executed over the model will reset the validation and
	the finder will execute the search again */
	model_valid_wgt->setModel(current_model);

	if(current_model && obj_finder_wgt->result_tbw->rowCount() > 0)
	  obj_finder_wgt->findObjects();
}

void MainWindow::executePlugin()
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

void MainWindow::openSupport()
{
	QDesktopServices::openUrl(QUrl(GlobalAttributes::PgModelerSupport));
}

void MainWindow::toggleUpdateNotifier(bool show)
{
#ifndef NO_UPDATE_CHECK
	if(show)
	{
		setFloatingWidgetPos(update_notifier_wgt,  qobject_cast<QAction *>(sender()), control_tb, false);
		action_about->setChecked(false);
		action_donate->setChecked(false);
	}

	update_notifier_wgt->setVisible(show);
#endif
}

void MainWindow::toggleAboutWidget(bool show)
{
	if(show)
	{
		setFloatingWidgetPos(about_wgt, qobject_cast<QAction *>(sender()), control_tb, false);
		action_update_found->setChecked(false);
		action_donate->setChecked(false);
	}

	about_wgt->setVisible(show);
}

void MainWindow::toggleDonateWidget(bool show)
{
	if(show)
	{
		setFloatingWidgetPos(donate_wgt, qobject_cast<QAction *>(sender()), control_tb, false);
		action_about->setChecked(false);
		action_update_found->setChecked(false);
	}

	donate_wgt->setVisible(show);
}

void MainWindow::setFloatingWidgetPos(QWidget *widget, QAction *act, QToolBar *toolbar, bool map_to_window)
{
	if(widget && act && toolbar)
	{
		QWidget *wgt=toolbar->widgetForAction(act);
		QPoint pos_orig=(wgt ? wgt->pos() : QPoint(0,0)), pos;

		if(map_to_window) pos=wgt->mapTo(this, pos);
		pos.setX(pos_orig.x() - 10);
		pos.setY(toolbar->pos().y() + toolbar->height() - 10);

		if((pos.x() + widget->width()) > this->width())
			pos.setX(pos_orig.x() - (widget->width() - 40));

		widget->move(pos);
	}
}

void MainWindow::configureSamplesMenu()
{
	QDir dir(GlobalAttributes::getSamplesDir());
	QStringList files=dir.entryList({QString("*.dbm")});
	QAction *act=nullptr;
	QString path;

	while(!files.isEmpty())
	{
		act=sample_mdls_menu.addAction(files.front(),this,SLOT(loadModelFromAction()));
		path=QFileInfo(GlobalAttributes::getSamplesDir() + GlobalAttributes::DirSeparator + files.front()).absoluteFilePath();
		act->setToolTip(path);
		act->setData(path);
		files.pop_front();
	}

	if(sample_mdls_menu.isEmpty())
	{
		act=sample_mdls_menu.addAction(tr("(no samples found)"));
		act->setEnabled(false);
	}

	central_wgt->sample_tb->setMenu(&sample_mdls_menu);
}

void MainWindow::storeDockWidgetsSettings()
{
	GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GeneralConfWgt));
	attribs_map params;

	params[Attributes::Validator]=Attributes::True;
	params[Attributes::SqlValidation]=(model_valid_wgt->sql_validation_chk->isChecked() ? Attributes::True : QString());
	params[Attributes::UseUniqueNames]=(model_valid_wgt->use_tmp_names_chk->isChecked() ? Attributes::True : QString());
	params[Attributes::Version]=model_valid_wgt->version_cmb->currentText();
	conf_wgt->addConfigurationParam(Attributes::Validator, params);
	params.clear();

	params[Attributes::ObjectFinder]=Attributes::True;
	params[Attributes::SelectObjects]=(obj_finder_wgt->select_btn->isChecked() ? Attributes::True : QString());
	params[Attributes::FadeInObjects]=(obj_finder_wgt->fade_btn->isChecked() ? Attributes::True : QString());
	params[Attributes::RegularExp]=(obj_finder_wgt->regexp_chk->isChecked() ? Attributes::True : QString());
	params[Attributes::CaseSensitive]=(obj_finder_wgt->case_sensitive_chk->isChecked() ? Attributes::True : QString());
	params[Attributes::ExactMatch]=(obj_finder_wgt->exact_match_chk->isChecked() ? Attributes::True : QString());
	conf_wgt->addConfigurationParam(Attributes::ObjectFinder, params);
	params.clear();

	params[Attributes::SqlTool]=Attributes::True;
	params[Attributes::ShowAttributesGrid]=(sql_tool_wgt->attributes_tb->isChecked() ? Attributes::True : QString());
	params[Attributes::ShowSourcePane]=(sql_tool_wgt->source_pane_tb->isChecked() ? Attributes::True : QString());
	conf_wgt->addConfigurationParam(Attributes::SqlTool, params);
	params.clear();
}

void MainWindow::restoreDockWidgetsSettings()
{
	GeneralConfigWidget *conf_wgt=dynamic_cast<GeneralConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::GeneralConfWgt));
	map<QString, attribs_map> confs=conf_wgt->getConfigurationParams();

	if(confs.count(Attributes::Validator))
	{
		model_valid_wgt->sql_validation_chk->setChecked(confs[Attributes::Validator][Attributes::SqlValidation]==Attributes::True);
		model_valid_wgt->use_tmp_names_chk->setChecked(confs[Attributes::Validator][Attributes::UseUniqueNames]==Attributes::True);
		model_valid_wgt->version_cmb->setCurrentText(confs[Attributes::Validator][Attributes::Version]);
	}

	if(confs.count(Attributes::ObjectFinder))
	{
		obj_finder_wgt->select_btn->setChecked(confs[Attributes::ObjectFinder][Attributes::SelectObjects]==Attributes::True);
		obj_finder_wgt->fade_btn->setChecked(confs[Attributes::ObjectFinder][Attributes::FadeInObjects]==Attributes::True);
		obj_finder_wgt->regexp_chk->setChecked(confs[Attributes::ObjectFinder][Attributes::RegularExp]==Attributes::True);
		obj_finder_wgt->case_sensitive_chk->setChecked(confs[Attributes::ObjectFinder][Attributes::CaseSensitive]==Attributes::True);
		obj_finder_wgt->exact_match_chk->setChecked(confs[Attributes::ObjectFinder][Attributes::ExactMatch]==Attributes::True);
	}

	if(confs.count(Attributes::SqlTool))
	{
		sql_tool_wgt->attributes_tb->setChecked(confs[Attributes::SqlTool][Attributes::ShowAttributesGrid]==Attributes::True);
		sql_tool_wgt->source_pane_tb->setChecked(confs[Attributes::SqlTool][Attributes::ShowSourcePane]==Attributes::True);
	}
}

void MainWindow::showDemoVersionWarning()
{
#ifdef DEMO_VERSION
	Messagebox msg_box;
	msg_box.show(tr("Warning"),
				 tr("You're running a demonstration version! Note that you'll be able to create only <strong>%1</strong> instances \
						of each type of object and some key features will be disabled or limited!<br/><br/>You can purchase a full binary copy or get the source code at <a href='https://pgmodeler.io'>https://pgmodeler.io</a>.\
						<strong>NOTE:</strong> pgModeler is an open source software, but purchasing binary copies or providing some donations will support the project and keep the development alive and at full speed!<br/><br/>\
						<strong>HINT:</strong> in order to test all features it's recommended to use the <strong>demo.dbm</strong> model located in </strong>Sample models</strong> at <strong>Welcome</strong> view.<br/><br/><br/><br/>").arg(GlobalAttributes::MaxObjectCount),
						Messagebox::AlertIcon, Messagebox::OkButton);
#endif
}

void MainWindow::executePendingOperation(bool valid_error)
{
	if(!valid_error && pending_op!=NoPendingOp)
	{
		static const QString op_names[]={ QString(), QT_TR_NOOP("save"), QT_TR_NOOP("save"),
																			QT_TR_NOOP("export"), QT_TR_NOOP("diff") };

		PgModelerUiNs::createOutputTreeItem(model_valid_wgt->output_trw,
											tr("Executing pending <strong>%1</strong> operation...").arg(op_names[pending_op]));

		if(pending_op==PendingSaveOp || pending_op==PendingSaveAsOp)
			saveModel();
		else if(pending_op==PendingExportOp)
			exportModel();
		else if(pending_op==PendingDiffOp)
			diffModelDatabase();

		pending_op=NoPendingOp;
	}
}

void MainWindow::changeCurrentView(bool checked)
{
	QAction *curr_act=qobject_cast<QAction *>(sender());

	layers_wgt->setVisible(false);

	if(checked)
	{
		bool enable=(curr_act==action_design);
		QList<QAction *> actions;

		action_welcome->blockSignals(true);
		action_manage->blockSignals(true);
		action_design->blockSignals(true);

		action_welcome->setChecked(false);
		action_manage->setChecked(false);
		action_design->setChecked(false);

		curr_act->setChecked(true);
		views_stw->setCurrentIndex(curr_act->data().toInt());

		action_welcome->blockSignals(false);
		action_manage->blockSignals(false);
		action_design->blockSignals(false);

		actions=general_tb->actions();
		for(int i=GeneralActionsCount; i < actions.count(); i++)
		{
			actions[i]->setEnabled(enable);

			if(actions[i]->menu())
			{
				for(auto action : actions[i]->menu()->actions())
					action->setEnabled(enable);
			}
		}

		if(!enable)
			overview_wgt->close();

		actions=edit_menu->actions();
		actions.removeOne(action_configuration);
		for(auto &act : actions)
			act->setEnabled(enable);

		actions=show_menu->actions();
		for(auto &act : actions)
			act->setEnabled(enable);

		model_nav_wgt->setEnabled(enable);
		action_print->setEnabled(enable);
		action_close_model->setEnabled(enable);
	}
	else
	{
		curr_act->blockSignals(true);
		curr_act->setChecked(true);
		curr_act->blockSignals(false);
	}
}

void MainWindow::reportBug()
{
	BugReportForm bugrep_frm;
	PgModelerUiNs::resizeDialog(&bugrep_frm);
	GeneralConfigWidget::restoreWidgetGeometry(&bugrep_frm);
	bugrep_frm.exec();
	GeneralConfigWidget::saveWidgetGeometry(&bugrep_frm);
}

void MainWindow::removeOperations()
{
	//Clears the operation list everytime a fix is applied to the model
	if(current_model && current_model->op_list->getCurrentSize()!=0)
	{
		current_model->op_list->removeOperations();
		oper_list_wgt->updateOperationList();
	}
}

void MainWindow::handleObjectsMetadata()
{
	MetadataHandlingForm objs_meta_frm(nullptr, Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	objs_meta_frm.setModelWidget(current_model);
	objs_meta_frm.setModelWidgets(model_nav_wgt->getModelWidgets());
	connect(&objs_meta_frm, SIGNAL(s_metadataHandled()), model_objs_wgt, SLOT(updateObjectsView()));

	PgModelerUiNs::resizeDialog(&objs_meta_frm);
	GeneralConfigWidget::restoreWidgetGeometry(&objs_meta_frm);
	objs_meta_frm.exec();
	GeneralConfigWidget::saveWidgetGeometry(&objs_meta_frm);
}

void MainWindow::arrangeObjects()
{
	if(!current_model)
		return;

	Messagebox msgbox;
	msgbox.show(tr("Rearrange objects over the canvas is an irreversible operation! Would like to proceed?"), Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

	if(msgbox.result() == QDialog::Accepted)
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);

		if(sender() == arrange_menu.actions().at(0))
			current_model->rearrangeSchemasInGrid();
		else if(sender() == arrange_menu.actions().at(1))
			current_model->rearrangeTablesHierarchically();
		else
			current_model->rearrangeTablesInSchemas();

		QApplication::restoreOverrideCursor();
	}
}

void MainWindow::toggleCompactView()
{
	ModelWidget *model_wgt = nullptr;

	BaseObjectView::setCompactViewEnabled(action_compact_view->isChecked());
	QApplication::setOverrideCursor(Qt::WaitCursor);

	for(int idx = 0; idx < models_tbw->count(); idx++)
	{
		model_wgt = dynamic_cast<ModelWidget *>(models_tbw->widget(idx));

		if(action_compact_view->isChecked())
			model_wgt->setAllCollapseMode(CollapseMode::ExtAttribsCollapsed);
		else
			model_wgt->setAllCollapseMode(CollapseMode::NotCollapsed);

		model_wgt->getDatabaseModel()->setObjectsModified({ ObjectType::Table, ObjectType::ForeignTable,
																												ObjectType::View, ObjectType::Relationship,
																												ObjectType::BaseRelationship, ObjectType::Schema});
	}

	if(current_model)
		current_model->update();

	QApplication::restoreOverrideCursor();
}

void MainWindow::toggleLayersWidget(bool show)
{
	QPoint tb_pos = mapTo(this, tool_btns_bar_wgt->pos()),
			btn_pos = mapTo(this, layers_btn->pos());

	layers_wgt->move(btn_pos.x() + general_tb->width(),
									 tb_pos.y() - (layers_wgt->height() - layers_btn->height() - v_splitter1->handleWidth()) + 1);

	layers_wgt->setVisible(show);
}

void MainWindow::configureMoreActionsMenu()
{
	if(!current_model)
		return;

	QList<QAction *> actions = current_model->popup_menu.actions();

	more_actions_menu.clear();
	actions.removeOne(current_model->action_new_object);
	actions.removeOne(current_model->action_quick_actions);
	actions.removeOne(current_model->action_edit);
	actions.removeOne(current_model->action_source_code);
	more_actions_menu.addActions(actions);
}

void MainWindow::switchView(int view)
{
	switch(view)
	{
	case(ManageView):
		action_manage->toggle();
		break;
	case(DesignView):
		action_design->toggle();
		break;
	case(WelcomeView):
		action_welcome->toggle();
	}
}

void MainWindow::addExecTabInSQLTool(const QString &sql_cmd)
{
	try
	{
		if(sql_tool_wgt->hasDatabasesBrowsed())
			sql_tool_wgt->addSQLExecutionTab(sql_cmd);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

bool MainWindow::hasDbsListedInSQLTool()
{
	return sql_tool_wgt->hasDatabasesBrowsed();
}
