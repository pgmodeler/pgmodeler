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

/**
\ingroup libpgmodeler_ui
\class MainWindow
\brief Implements the pgModeler main window. Reuniting all the resources implemented by the libraries.
*/

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include "ui_mainwindow.h"
#include "modelwidget.h"
#include "aboutform.h"
#include "messagebox.h"
#include "baseform.h"
#include "modelrestorationform.h"
#include "operationlistwidget.h"
#include "modelobjectswidget.h"
#include "pgmodelerplugin.h"
#include "modeloverviewwidget.h"

using namespace std;

class MainWindow: public QMainWindow, public Ui::MainWindow {
	private:
		Q_OBJECT

		//! \brief Timer used for auto saving the model and temporary model.
		QTimer model_save_timer,	tmpmodel_save_timer;

		//! \brief Message box widget used to show error/confirmation messages
		MessageBox msg_box;

		//! \brief Model overview widget
		ModelOverviewWidget *overview_wgt;

		//! \brief Temporary model restoration form
		ModelRestorationForm *restoration_form;

		//! \brief Operation list dock widget
		OperationListWidget *oper_list_wgt;

		//! \brief Model objects dock widget
		ModelObjectsWidget *model_objs_wgt;

		//! \brief Stores the currently focused model
		ModelWidget *current_model;

		//! \brief Stores the model objects tree state for each opened model
		map<ModelWidget *, vector<BaseObject *> > model_tree_states;

		//! \brief Stores the defaul window title
		QString window_title;

		//! \brief Stores the auto save interval (in miliseconds)
		int save_interval;

		//! \brief QMainWindow::closeEvent() overload: Saves the configurations before close the application
		void closeEvent(QCloseEvent *);

		//! \brief QMainWindow::showEvent(): Start the countdown to model autosave
		void showEvent(QShowEvent *);

	public:
		MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~MainWindow(void);

	public slots:
		/*! \brief Creates a new empty model inside the main window. If the parameter 'filename' is specified,
		creates the model loading it from a file */
		void addModel(const QString &filename="");

		//! \brief Closes the currently focused model. If the 'model_id' is specified the model at given index is closed
		void closeModel(int model_id=-1);

		//! \brief Returns the currently loaded model count.
		int getModelCount(void);

		//! \brief Returns the model at given index
		ModelWidget *getModel(int idx);

	private slots:
		//! \brief Atualiza as definições da grade com base nas ações: Exibir Grade, Alin. Grade e Exibir limites
		void setGridOptions(void);

		//! \brief Updates the state (enable/disable) of the buttons of toolbars
		void updateToolsState(bool model_closed=false);
		void __updateToolsState(void);

		//! \brief Updates the operation list and model objects dockwidgets
		void updateDockWidgets(void);
		void __updateDockWidgets(void);

		//! \brief Updates the reference to the current model when changing the tab focus
		void setCurrentModel(void);

		//! \brief Loads a model from a file via file dialog
		void loadModel(void);

		//! \brief Saves the currently focused model. If the parameter 'model' is set, saves the passed model
		void saveModel(ModelWidget *model=NULL);

		//! \brief Save all loaded models
		void saveAllModels(void);

		//! \brief Prints the currently focused model
		void printModel(void);

		//! \brief Executes the export of the currently focused model
		void exportModel(void);

		//! \brief Updates the opened models with new configurations
		void updateModelsConfigurations(void);

		//! \brief Applies the zoom to the currently focused model
		void applyZoom(void);

		//! \brief Toggle the fullscreen mode
		void showFullScreen(bool fullscreen);

		//! \brief Execute the plugin represented by the action that calls the slot
		void executePlugin(void);

		/*! \brief Saves a temporary model file related to the currently edited model. In case of failure (crash)
		pgModeler can restore the previous model */
		void saveTemporaryModel(void);

		//! \brief Toggles the overview widget for the currently opened model
		void showOverview(bool show);

		//! \brief Updates the tab name of the currently opened model if the database name is changed
		void updateModelTabName(void);

		//! \brief Opens the pgModeler Wiki in a web browser window
		void openWiki(void);
};

#endif
