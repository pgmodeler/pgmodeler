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

#include "pluginsconfigwidget.h"

PluginsConfigWidget::PluginsConfigWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	QGridLayout *grid=new QGridLayout(loaded_plugins_gb);
	QDir dir=QDir(GlobalAttributes::PLUGINS_DIR);

	root_dir_edt->setText(dir.absolutePath());

	plugins_tab=new ObjectTableWidget(ObjectTableWidget::EDIT_BUTTON, false, this);
	plugins_tab->setColumnCount(3);
	plugins_tab->setHeaderLabel(trUtf8("Plugin"),0);
	plugins_tab->setHeaderIcon(QPixmap(":/icones/icones/plugins.png"),0);
	plugins_tab->setHeaderLabel(trUtf8("Version"),1);
	plugins_tab->setHeaderLabel(trUtf8("Library"),2);

	connect(plugins_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showPluginInfo(int)));
	connect(open_fm_tb, SIGNAL(clicked(void)), this, SLOT(openRootPluginDiretory(void)));

	grid->setContentsMargins(2,2,2,2);
	grid->addWidget(plugins_tab,0,0,1,1);
	loaded_plugins_gb->setLayout(grid);
}

PluginsConfigWidget::~PluginsConfigWidget(void)
{
	while(!plugins.empty())
	{
		delete(plugins.back());
		plugins.pop_back();
	}
}

void PluginsConfigWidget::openRootPluginDiretory(void)
{
	QDesktopServices::openUrl(QUrl("file:///" + root_dir_edt->text()));
}

void PluginsConfigWidget::showPluginInfo(int idx)
{
	plugins[idx]->showPluginInfo();
}

void PluginsConfigWidget::loadPlugins(void)
{
	vector<Exception> errors;
	QString lib, plugin_name,
			dir_plugins=GlobalAttributes::PLUGINS_DIR +
									GlobalAttributes::DIR_SEPARATOR;
	QPluginLoader plugin_loader;
	QStringList dir_list;
	PgModelerPlugin *plugin=NULL;
	QAction *plugin_action=NULL;
	QPixmap icon;
	QFileInfo fi;

	//The plugin loader must resolve all symbols otherwise return an error if some symbol is missing on library
	plugin_loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);

	if(GlobalAttributes::PLUGINS_DIR.isEmpty())
		dir_plugins="." + GlobalAttributes::DIR_SEPARATOR;

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
				GlobalAttributes::DIR_SEPARATOR  +
				plugin_name + QString(".dll");
#else
#ifdef Q_OS_MAC
		lib=dir_plugins + plugin_name +
				GlobalAttributes::DIR_SEPARATOR  +
				QString("lib") + plugin_name + QString(".dylib");
#else
		lib=dir_plugins + plugin_name +
				GlobalAttributes::DIR_SEPARATOR  +
				QString("lib") + plugin_name + QString(".so");
#endif
#endif

		//Try to load the library
		plugin_loader.setFileName(lib);
		if(plugin_loader.load())
		{
			fi.setFile(lib);

			//Inserts the loaded plugin on the vector
			plugin=qobject_cast<PgModelerPlugin *>(plugin_loader.instance());
			plugins.push_back(plugin);

			//Configures the action related to plugin
			plugin_action=new QAction(this);
			plugin_action->setText(plugin->getPluginTitle());
			plugin_action->setData(QVariant::fromValue<void *>(reinterpret_cast<void *>(plugin)));
			plugin_action->setShortcut(plugin->getPluginShortcut());

			icon.load(dir_plugins + plugin_name +
								GlobalAttributes::DIR_SEPARATOR  +
								plugin_name + QString(".png"));
			plugin_action->setIcon(icon);

			plugins_actions.push_back(plugin_action);
			plugins_tab->addRow();
			plugins_tab->setCellText(plugin->getPluginTitle(), plugins_tab->getRowCount()-1, 0);
			plugins_tab->setCellText(plugin->getPluginVersion(), plugins_tab->getRowCount()-1, 1);
			plugins_tab->setCellText(fi.fileName(), plugins_tab->getRowCount()-1, 2);
		}
		else
		{
			errors.push_back(Exception(Exception::getErrorMessage(ERR_PLUGIN_NOT_LOADED)
																 .arg(Utf8String::create(dir_list.front()))
																 .arg(Utf8String::create(lib))
																 .arg(plugin_loader.errorString()),
																 ERR_PLUGIN_NOT_LOADED, __PRETTY_FUNCTION__,__FILE__,__LINE__));
		}
		dir_list.pop_front();
		plugins_tab->clearSelection();
	}

	if(!errors.empty())
		throw Exception(ERR_PLUGINS_NOT_LOADED,__PRETTY_FUNCTION__,__FILE__,__LINE__, errors);
}

void PluginsConfigWidget::installPluginsActions(QToolBar *toolbar, QMenu *menu, QObject *recv, const char *slot)
{
	if((toolbar || menu) && slot)
	{
		vector<QAction *>::iterator itr=plugins_actions.begin();

		while(itr!=plugins_actions.end())
		{
			if(toolbar)
				toolbar->addAction(*itr);

			if(menu)
				menu->addAction(*itr);

			connect(*itr, SIGNAL(triggered(void)), recv, slot);
			itr++;
		}
	}
}
