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

#include "pluginsconfigwidget.h"

PluginsConfigWidget::PluginsConfigWidget(QWidget *parent) : BaseConfigWidget(parent)
{
	setupUi(this);

	QGridLayout *grid=new QGridLayout(loaded_plugins_gb);
	QDir dir=QDir(GlobalAttributes::getPluginsDir());

	root_dir_sel = new FileSelectorWidget(this);
	root_dir_sel->setToolTip(tr("pgModeler plugins directory"));
	root_dir_sel->setReadOnly(true);
	root_dir_sel->setFileMode(QFileDialog::Directory);
	root_dir_sel->setSelectedFile(GlobalAttributes::getPluginsDir());
	plugins_layout->insertWidget(1, root_dir_sel);

	plugins_tab=new ObjectsTableWidget(ObjectsTableWidget::EditButton, false, this);
	plugins_tab->setColumnCount(3);
	plugins_tab->setHeaderLabel(tr("Plugin"),0);
	plugins_tab->setHeaderIcon(QPixmap(GuiUtilsNs::getIconPath("plugins")),0);
	plugins_tab->setHeaderLabel(tr("Version"),1);
	plugins_tab->setHeaderLabel(tr("Library"),2);

	connect(plugins_tab, &ObjectsTableWidget::s_rowEdited, this, &PluginsConfigWidget::showPluginInfo);

	grid->setContentsMargins(GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin);
	grid->addWidget(plugins_tab,0,0,1,1);
	loaded_plugins_gb->setLayout(grid);
}

PluginsConfigWidget::~PluginsConfigWidget()
{
	while(!plugins.empty())
	{
		delete plugins.back();
		plugins.pop_back();
	}
}

void PluginsConfigWidget::showPluginInfo(int idx)
{
	plugins[idx]->showPluginInfo();
}

void PluginsConfigWidget::loadConfiguration()
{
	std::vector<Exception> errors;
	QString lib, plugin_name,
			dir_plugins=GlobalAttributes::getPluginsDir() +
						GlobalAttributes::DirSeparator;
	QPluginLoader plugin_loader;
	QStringList dir_list;
	PgModelerPlugin *plugin=nullptr;
	QAction *plugin_action=nullptr;
	QPixmap icon;
	QFileInfo fi;

	//The plugin loader must resolve all symbols otherwise return an error if some symbol is missing on library
	plugin_loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);

	/* Configures an QDir instance to list only directories on the plugins/ subdir.
		If the user does not put the plugin in it's directory the file is ignored  */
	dir_list=QDir(dir_plugins, "*", QDir::Name, QDir::AllDirs | QDir::NoDotAndDotDot).entryList();

	while(!dir_list.isEmpty())
	{
		plugin_name=dir_list.front();

		/* Configures the basic path to the library on the form:

		 [PLUGINS ROOT DIR]/[PLUGIN NAME]/lib[PLUGIN NAME].[EXTENSION] */
#ifdef Q_OS_WIN
		lib=dir_plugins + plugin_name +
            GlobalAttributes::DirSeparator  +
			plugin_name + ".dll";
#else
#ifdef Q_OS_MAC
		lib=dir_plugins + plugin_name +
            GlobalAttributes::DirSeparator  +
			"lib" + plugin_name + ".dylib";
#else
		lib=dir_plugins + plugin_name +
			GlobalAttributes::DirSeparator  +
			"lib" + plugin_name + ".so";
#endif
#endif

		//Try to load the library
		plugin_loader.setFileName(lib);

		if(plugin_loader.load())
		{
			fi.setFile(lib);

			//Inserts the loaded plugin on the vector
			plugin = qobject_cast<PgModelerPlugin *>(plugin_loader.instance());
			plugin->setLibraryName(QFileInfo(lib).fileName());
			plugins.push_back(plugin);

			if(plugin->hasMenuAction())
			{
				//Configures the action related to plugin
				plugin_action=new QAction(this);
				plugin_action->setText(plugin->getPluginTitle());
				plugin_action->setData(QVariant::fromValue<void *>(reinterpret_cast<void *>(plugin)));
				plugin_action->setShortcut(plugin->getPluginShortcut());

				icon.load(dir_plugins + plugin_name +
									GlobalAttributes::DirSeparator +
									GlobalAttributes::ResourcesDir +
									GlobalAttributes::DirSeparator +
									plugin_name + ".png");

				plugin_action->setIcon(icon);
				plugins_actions[plugin] = plugin_action;
			}

			plugins_tab->addRow();
			plugins_tab->setCellText(plugin->getPluginTitle(), plugins_tab->getRowCount()-1, 0);
			plugins_tab->setCellText(plugin->getPluginVersion(), plugins_tab->getRowCount()-1, 1);
			plugins_tab->setCellText(fi.fileName(), plugins_tab->getRowCount()-1, 2);
		}
		else
		{
			errors.push_back(Exception(Exception::getErrorMessage(ErrorCode::PluginNotLoaded)
																 .arg(dir_list.front())
																 .arg(lib)
																 .arg(plugin_loader.errorString()),
																 ErrorCode::PluginNotLoaded, __PRETTY_FUNCTION__,__FILE__,__LINE__));
		}

		dir_list.pop_front();
		plugins_tab->clearSelection();
	}

	if(!errors.empty())
		throw Exception(ErrorCode::PluginsNotLoaded,__PRETTY_FUNCTION__,__FILE__,__LINE__, errors);
}

void PluginsConfigWidget::initPlugins(MainWindow *main_window)
{
	std::vector<PgModelerPlugin *> inv_plugins;
	std::vector<Exception> errors;

	for(auto &plugin : plugins)
	{
		try
		{
			plugin->initPlugin(main_window);
			__plugins_actions.append(plugin->getPluginActions());
		}
		catch(Exception &e)
		{
			inv_plugins.push_back(plugin);
			errors.push_back(e);
		}
	}

	// Erasing the plugins/actions related to the ones that failed to initialize
	while(!inv_plugins.empty())
	{
		plugins.erase(std::find(plugins.begin(), plugins.end(), inv_plugins.back()));

		if(plugins_actions.count(inv_plugins.back()))
		{
			delete plugins_actions[inv_plugins.back()];
			plugins_actions.erase(inv_plugins.back());
		}

		delete inv_plugins.back();
		inv_plugins.pop_back();
	}

	if(!errors.empty())
	{
		Messagebox msgbox;
		msgbox.show(Exception(tr("One or more plug-ins could not be initialized due to minimum pgModeler version requirements! The failed ones were unloaded. Please, check the error stack for more details."),
													ErrorCode::Custom, __PRETTY_FUNCTION__, __FILE__, __LINE__, errors));
	}
}

QList<PluginActions> PluginsConfigWidget::getPluginsActions()
{
	return __plugins_actions;
}
