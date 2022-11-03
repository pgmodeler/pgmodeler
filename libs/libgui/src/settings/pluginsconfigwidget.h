/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

class PluginsConfigWidget: public BaseConfigWidget, public Ui::PluginsConfigWidget {
	private:
		Q_OBJECT

		//! \brief Loaded plugins
		std::vector<PgModelerPlugin *> plugins;

		//! \brief Stores the actions assigned for each plugin
		std::vector<QAction *> plugins_actions;

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

		/*! \brief Install the created actions on menu. Additionally the user must specify the
		 receiver object and slot executed when the actions are activated. The parameters recv and slot
			must object the same log as the QObject::connect() where recv is the recever object and slot is the
			method called (in format &Class::method) when the action sends the triggered signal. */
		template <class Class, typename Slot>
		void installPluginsActions(QMenu *menu, const Class *recv, Slot slot);

		//! \brief Performs the initialization of all loaded plugins (see PgModelerPlugin::initPlugin())
		void initPlugins(MainWindow *main_window);

	private slots:
		void showPluginInfo(int idx);
};

template <class Class, typename Slot>
void PluginsConfigWidget::installPluginsActions(QMenu *menu, const Class *recv, Slot slot)
{
	if(!menu || !slot)
		return;

	for(auto &act : plugins_actions)
	{
		if(menu)
			menu->addAction(act);

		connect(act, &QAction::triggered, recv, slot);
	}
}

#endif
