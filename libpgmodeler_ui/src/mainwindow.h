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

/**
\ingroup libpgmodeler_ui
\class MainWindow
\brief Implements the pgModeler main window. Reuniting all the resources implemented by the libraries.
*/

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets>
#include <QPrintDialog>
#include "ui_mainwindow.h"
#include "modelwidget.h"
#include "aboutwidget.h"
#include "messagebox.h"
#include "baseform.h"
#include "modelrestorationform.h"
#include "operationlistwidget.h"
#include "modelobjectswidget.h"
#include "pgmodelerplugin.h"
#include "modeloverviewwidget.h"
#include "modelvalidationwidget.h"
#include "objectfinderwidget.h"
#include "modelexportform.h"
#include "databaseimportform.h"
#include "modeldatabasediffform.h"
#include "sqltoolwidget.h"
#include "modelfixform.h"
#include "updatenotifierwidget.h"
#include "modelnavigationwidget.h"
#include "welcomewidget.h"
#include "configurationform.h"
#include "donatewidget.h"
#include "sceneinfowidget.h"
#include "layerswidget.h"

class MainWindow: public QMainWindow, public Ui::MainWindow {
	private:
		Q_OBJECT

		static int GeneralActionsCount;

		static constexpr int WelcomeView=0,
		DesignView=1,
		ManageView=2,
		InfinityInterval = INT_MAX;

		static bool confirm_validation;

		//! \brief Constants used to mark a pending operation to be executed after validate model
		static constexpr unsigned NoPendingOp=0,
		PendingSaveOp=1,
		PendingSaveAsOp=2,
		PendingExportOp=3,
		PendingDiffOp=4;

		unsigned pending_op;

		//! \brief Timer used for auto saving the model and temporary model.
		QTimer model_save_timer,	tmpmodel_save_timer;

		AboutWidget *about_wgt;

		DonateWidget *donate_wgt;

		SceneInfoWidget *scene_info_wgt;

		//! \brief Layers management widget
		LayersWidget *layers_wgt;

		/*! \brief Widget positioned on the center of main window that contains some basic operations like
		create new model, open a file, restore session */
		WelcomeWidget *central_wgt;

		//! \brief Model overview widget
		ModelOverviewWidget *overview_wgt;

		//! \brief Widget used to navigate through the opened models.
		ModelNavigationWidget *model_nav_wgt;

		//! \brief Model validation widget
		ModelValidationWidget *model_valid_wgt;

		//! \brief SQL tool widget widget
		SQLToolWidget *sql_tool_wgt;

		//! \brief Operation list dock widget
		OperationListWidget *oper_list_wgt;

		//! \brief Model objects dock widget
		ModelObjectsWidget *model_objs_wgt;

		//! \brief Temporary model restoration form
		ModelRestorationForm *restoration_form;

		//! \brief Object finder used as dock widget
		ObjectFinderWidget *obj_finder_wgt;

		//! \brief Update notifier popup widget
		UpdateNotifierWidget *update_notifier_wgt;

		//! \brief Configuration form
		ConfigurationForm *configuration_form;

		//! \brief Stores the currently focused model
		ModelWidget *current_model;

		//! \brief Stores the model objects tree state for each opened model
		map<ModelWidget *, vector<BaseObject *> > model_tree_states;

		//! \brief Stores the defaul window title
		QString window_title;

		//! \brief Stores the recent models file names
		QStringList recent_models,

		//! \brief Stores the last session file names
		prev_session_files;

		//! \brief Stores the actions related to recent models
		QMenu recent_mdls_menu,

		main_menu,

		sample_mdls_menu,

		arrange_menu,

		more_actions_menu,

		fix_menu;

		//! \brief QMainWindow::closeEvent() overload: Saves the configurations before close the application
		void closeEvent(QCloseEvent *event);

		void resizeEvent(QResizeEvent *);

		//! \brief Set the postion of a floating widget based upon an action at a tool bar
		void setFloatingWidgetPos(QWidget *widget, QAction *act, QToolBar *toolbar, bool map_to_window);

		void configureSamplesMenu();

		/*! \brief Stores the current checkboxes states of the main dock widgets on the set of configuration params
				in order to save them on the main configuration file */
		void storeDockWidgetsSettings();

		//! \brief Restore the dock widget configurations from the parameters loaded from main configuration file
		void restoreDockWidgetsSettings();

		//! \brief Shows a error dialog informing that the model demands a fix after the error ocurred when loading the filename.
		void showFixMessage(Exception &e, const QString &filename);

		/*! \brief This method determines if the provided layout has togglable buttons and one of them are checked.
		 * This is an auxiliary method used to determine if widget bars (bottom or right) can be displayed based upon
		 * the current button toggle state. */
		bool isToolButtonsChecked(QHBoxLayout *layout, const QWidgetList &ignored_wgts = QWidgetList());

	public:
		MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::Widget);
		virtual ~MainWindow();

		//! \brief Loads a set of models from string list
		void loadModels(const QStringList &list);

		//! \brief Indicates if model must be validated before save, diff or export
		static void setConfirmValidation(bool value);

	public slots:
		/*! \brief Creates a new empty model inside the main window. If the parameter 'filename' is specified,
		creates the model loading it from a file */
		void addModel(const QString &filename=QString());

		/*! \brief Creates a new model inside the main window using the specified model widget. The method will raise
		an error is the widget isn't allocated or already has a parent */
		void addModel(ModelWidget *model_wgt);

		//! \brief Closes the currently focused model. If the 'model_id' is specified the model at given index is closed
		void closeModel(int model_id=-1);

		//! \brief Returns the currently loaded model count.
		int getModelCount();

		//! \brief Returns the model at given index
		ModelWidget *getModel(int idx);

		//! \brief Switches the currently opened view (Design, Manage, Welcome)
		void switchView(int view);

		/*! \brief This is a convenience method to make able the addition of execution tabs in SQL tool without
		 *  expose the SQL Tool widget itself (useful for plugin developers) */
		void addExecTabInSQLTool(const QString &sql_cmd);

		/*! \brief This is a convenience method that returns a true value when there're databases listed in the SQL tool widget without
		 *  expose the SQL Tool widget itself (useful for plugin developers) */
		bool hasDbsListedInSQLTool();

	private slots:
		void showMainMenu();

		//! \brief Atualiza as definições da grade com base nas ações: Exibir Grade, Alin. Grade e Exibir limites
		void setGridOptions();

		//! \brief Updates the state (enable/disable) of the buttons of toolbars
		void updateToolsState(bool model_closed=false);
		void __updateToolsState();

		//! \brief Updates the operation list and model objects dockwidgets
		void updateDockWidgets();

		//! \brief Updates the reference to the current model when changing the tab focus
		void setCurrentModel();

		//! \brief Loads a model from a file via file dialog
		void loadModel();

		//! \brief Loads a model from a specified filename
		void loadModel(const QString &filename);

		//! \brief Saves the currently focused model. If the parameter 'model' is set, saves the passed model
		void saveModel(ModelWidget *model=nullptr);

		//! \brief Save all loaded models
		void saveAllModels();

		//! \brief Prints the currently focused model
		void printModel();

		//! \brief Executes the export of the currently focused model
		void exportModel();

		//! \brief Executes the reverse engineering
		void importDatabase();

		//! \brief Executes the model <> database comparison
		void diffModelDatabase();

		//! \brief Updates the opened models with new configurations
		void applyConfigurations();

		//! \brief Applies the zoom to the currently focused model
		void applyZoom();

		//! \brief Execute the plugin represented by the action that calls the slot
		void executePlugin();

		//! \brief Toggles the overview widget for the currently opened model
		void showOverview(bool show);

		//! \brief Updates the tab name of the currently opened model if the database name is changed
		void updateModelTabName();

		//! \brief Loads a recent model. The filename is get from the action that triggered the slot
		void loadModelFromAction();

		//! \brief Clears the recent models menu/list
		void clearRecentModelsMenu();

		//! \brief Update the recent models menu entries
		void updateRecentModelsMenu();

		//! \brief Updates the connections list of the validator widget
		void updateConnections(bool force = false);

		//! \brief Save the temp files for all opened models
		void saveTemporaryModels();

		//! \brief Opens the pgModeler Wiki in a web browser window
		void openSupport();

		/*! \brief Stop the saving timers. This is used when validating the model
		in order to avoid the saving while the validation is working */
		void stopTimers(bool value);

		//! \brief Executes one of the pending operations (save, export, diff) after validate the model
		void executePendingOperation(bool valid_error);

		//! \brief Configures the "More" actions in the general toolbar by usinge the current_model's popup menu
		void configureMoreActionsMenu();

		void fixModel(const QString &filename=QString());
		void showRightWidgetsBar();
		void showBottomWidgetsBar();
		void restoreLastSession();
		void toggleUpdateNotifier(bool show);
		void toggleAboutWidget(bool show);
		void toggleDonateWidget(bool show);
		void removeModelActions();
		void showDemoVersionWarning();
		void changeCurrentView(bool checked);
		void reportBug();
		void removeOperations();
		void handleObjectsMetadata();
		void restoreTemporaryModels();
		void arrangeObjects();
		void toggleCompactView();
		void toggleLayersWidget(bool show);

	signals:
		void s_currentModelChanged(ModelWidget *model_wgt);
};

#endif
