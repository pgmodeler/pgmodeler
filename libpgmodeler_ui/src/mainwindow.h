/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "centralwidget.h"
#include "configurationform.h"

class MainWindow: public QMainWindow, public Ui::MainWindow {
	private:
		Q_OBJECT

		//! \brief Maximum number of files listed on recent models menu
		const static int MAX_RECENT_MODELS=10;

    const static int GENERAL_ACTIONS_COUNT=9;

    const static int WELCOME_VIEW=0,
    DESIGN_VIEW=1,
    MANAGE_VIEW=2;

    static bool confirm_validation;

    //! \brief Constants used to mark a pending operation to be executed after validate model
    const static unsigned NO_PENDING_OPER=0,
    PENDING_SAVE_OPER=1,
    PENDING_SAVE_AS_OPER=2,
    PENDING_EXPORT_OPER=3,
    PENDING_DIFF_OPER=4;

    unsigned pending_op;

    AboutWidget *about_wgt;

		/*! brief Widget positioned on the center of main window that contains some basic operations like
		create new model, open a file, restore session */
		CentralWidget *central_wgt;

		//! brief Widget used to navigate through the opened models.
		ModelNavigationWidget *model_nav_wgt;

		//! \brief Thread that controls temporary model file savings
		QThread tmpmodel_thread;

		//! \brief Timer used for auto saving the model and temporary model.
		QTimer model_save_timer,	tmpmodel_save_timer;

		//! \brief Model overview widget
		ModelOverviewWidget *overview_wgt;

		//! \brief Model validation widget
		ModelValidationWidget *model_valid_wgt;

    //! \brief SQL tool widget widget
    SQLToolWidget *sql_tool_wgt;

		//! \brief Temporary model restoration form
		ModelRestorationForm *restoration_form;

		//! \brief Operation list dock widget
		OperationListWidget *oper_list_wgt;

		//! \brief Model objects dock widget
		ModelObjectsWidget *model_objs_wgt;

		//! \brief Object finder used as dock widget
		ObjectFinderWidget *obj_finder_wgt;

    //! brief Update notifier popup widget
    UpdateNotifierWidget *update_notifier_wgt;

    //! brief Configuration form
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

		sample_mdls_menu;

		//! \brief QMainWindow::closeEvent() overload: Saves the configurations before close the application
		void closeEvent(QCloseEvent *event);

		//! \brief QMainWindow::showEvent(): Start the countdown to model autosave
		void showEvent(QShowEvent *);

		void resizeEvent(QResizeEvent *);

		//! brief Set the postion of a floating widget based upon an action at a tool bar
		void setFloatingWidgetPos(QWidget *widget, QAction *act, QToolBar *toolbar, bool map_to_window);

		//! brief Creates drop shadown on a tool button that represents an QAction
		QGraphicsDropShadowEffect *createDropShadow(QToolButton *btn);

		void configureSamplesMenu(void);

		/*! brief Stores the current checkboxes states of the main dock widgets on the set of configuration params
				in order to save them on the main configuration file */
		void storeDockWidgetsSettings(void);

		//! brief Restore the dock widget configurations from the parameters loaded from main configuration file
		void restoreDockWidgetsSettings(void);

	public:
		MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~MainWindow(void);

		//! \brief Loads a set of models from string list
		void loadModels(const QStringList &list);

    //! brief Indicates if model must be validated before save, diff or export
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
		int getModelCount(void);

		//! \brief Returns the model at given index
		ModelWidget *getModel(int idx);

	private slots:
		void showMainMenu(void);

		//! \brief Atualiza as definições da grade com base nas ações: Exibir Grade, Alin. Grade e Exibir limites
		void setGridOptions(void);

		//! \brief Updates the state (enable/disable) of the buttons of toolbars
		void updateToolsState(bool model_closed=false);
		void __updateToolsState(void);

		//! \brief Updates the operation list and model objects dockwidgets
		void updateDockWidgets(void);

		//! \brief Updates the reference to the current model when changing the tab focus
		void setCurrentModel(void);

		//! \brief Loads a model from a file via file dialog
		void loadModel(void);

    //! \brief Loads a model from a specified filename
    void loadModel(const QString &filename);

		//! \brief Saves the currently focused model. If the parameter 'model' is set, saves the passed model
		void saveModel(ModelWidget *model=nullptr);

		//! \brief Save all loaded models
		void saveAllModels(void);

		//! \brief Prints the currently focused model
		void printModel(void);

		//! \brief Executes the export of the currently focused model
		void exportModel(void);

		//! \brief Executes the reverse engineering
		void importDatabase(void);

		//! \brief Executes the model <> database comparison
		void diffModelDatabase(void);

		//! \brief Updates the opened models with new configurations
		void applyConfigurations(void);

		//! \brief Applies the zoom to the currently focused model
		void applyZoom(void);

		//! \brief Execute the plugin represented by the action that calls the slot
		void executePlugin(void);

		//! \brief Toggles the overview widget for the currently opened model
		void showOverview(bool show);

		//! \brief Updates the tab name of the currently opened model if the database name is changed
		void updateModelTabName(void);

		//! \brief Loads a recent model. The filename is get from the action that triggered the slot
		void loadModelFromAction(void);

		//! \brief Clears the recent models menu/list
		void clearRecentModelsMenu(void);

		//! \brief Update the recent models menu entries
		void updateRecentModelsMenu(void);

		//! \brief Updates the connections list of the validator widget
		void updateConnections(void);

		//! \brief Save the temp files for all opened models
    void saveTemporaryModels(void);

		//! \brief Opens the pgModeler Wiki in a web browser window
		void openWiki(void);

		/*! \brief Stop the saving timers. This is used when validating the model
		in order to avoid the saving while the validation is working */
		void stopTimers(bool value);

    //! \brief Executes one of the pending operations (save, export, diff) after validate the model
    void executePendingOperation(bool valid_error);

		void fixModel(const QString &filename=QString());
    void showRightWidgetsBar(void);
    void showBottomWidgetsBar(void);
    void restoreLastSession(void);
    void toggleUpdateNotifier(bool show);
		void toggleAboutWidget(bool show);
		void removeModelActions(void);
		void showDemoVersionWarning(void);
		void quitDemoVersion(void);
    void changeCurrentView(bool checked);
    void reportBug(void);
};

#endif
