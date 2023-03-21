/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class PluginsConfigWidget
\brief Implements the operations to manage plugins.
*/

#ifndef PLUGINS_CONFIG_WIDGET_H
#define PLUGINS_CONFIG_WIDGET_H

#include "baseconfigwidget.h"
#include "ui_pluginsconfigwidget.h"
#include "widgets/objectstablewidget.h"
#include "pgmodelerplugin.h"
#include "widgets/fileselectorwidget.h"

class __libgui PluginsConfigWidget: public BaseConfigWidget, public Ui::PluginsConfigWidget {
	private:
		Q_OBJECT

		//! \brief Loaded plugins
		std::vector<PgModelerPlugin *> plugins;

		//! \brief Table used to show the loaded plugins
		ObjectsTableWidget *plugins_tab;

		FileSelectorWidget *root_dir_sel;

		/* Disabled methods */
		void applyConfiguration(void){}
		void saveConfiguration(void){}
		void restoreDefaults(void){}
		void addConfigurationParam(const QString &, const attribs_map &){}

	public:
		PluginsConfigWidget(QWidget *parent = nullptr);

		virtual ~PluginsConfigWidget();

		//! \brief Since plugins has its own configurations this method load all plugins instead
		void loadConfiguration();

		/*! \brief Installs the plugins's ocnfiguration action in the provided menu.
		 *  Additionally, it returns a list of actions that is meant to be placed in a toolbar. */
		QList<QAction *> installPluginsActions(QMenu *menu);

		//! \brief Performs the initialization of all loaded plugins (see PgModelerPlugin::initPlugin())
		void initPlugins(MainWindow *main_window);

		//! \brief Execute the post initialization of all loaded plugins
		void postInitPlugins();

		//! \brief Returns a list of actions of the loaded plugins related to model actions only
		QList<QAction *> getPluginsModelsActions();

	private slots:
		void showPluginInfo(int idx);
};

#endif
