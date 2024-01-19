/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class PgModelerPlugin
\brief Implements the basic operations to create third party plugins based upon shared libraries.
*/

#ifndef PGMODELER_PLUGIN_H
#define PGMODELER_PLUGIN_H

#include "widgets/modelwidget.h"
#include "baseform.h"

#if defined(PRIVATE_PLUGINS_SYMBOLS)
	#include "privpluginsns.h"
#endif

/*	The plug-ins in pgModeler must be within the "plugins" folder in their own
		directory and must have the following basic structure:

		 [PGMODELER_PLUGINS_DIR]/
					|
					+ - pluginA/
							 |
							 + ---- (lib)*(pluginA.)(so|dylib|dll) (library)
							 |
							 + ---- res/
											 |
											 +--- pluginA.qrc (resource file)
														|
														+--- /pluginA (context)
																 |
																 +--- pluginA.png (icon)

		> Library: it's the shared object that represents the plugin. The prefix (lib) and suffix (so|dylib|dll) are plataform dependent.
		> Resource file: it's the file where all icons used by the plug-in are registered.
		> Context: it's is the default namespace where the plug-ins are organized logically inside the .qrc file.
		> Icon: it is a PNG image that represents an icon of the plug-in

		> Plug-ins can have an optional "lang" subdir to store the UI translations for them. The translation files must be named
			in the form [plugin name].[lang code].qm, for instance, Brazilian Portuguese translation for "sampleplugin"
			would be: "sampleplugin.pt_BR.qm".

	Note: Plug-ins can have additional subdirectories but any reference to them must be made programatically by the plug-in author. */

// Making the MainWindow class of pgModeler be known by the plugin interface
class MainWindow;
class PluginsConfigWidget;

class __libgui PgModelerPlugin {
	private:
		QString libname,

		plugin_name;

		QLabel	*icon_lbl,
		*title_lbl,
		*author_lbl,
		*version_lbl,
		*description_lbl;

		//! \brief Defines the name of the library from where the plugin is being loaded
		void setLibraryName(const QString &lib);

		//! \brief Defines the name of plugin itself. In practical terms, it's the plugin's root folder name
		void setPluginName(const QString &name);

		/*! \brief This method returns a full path to a file using the plugin's name as the root folder.
		 * If both subdir and filename are empty, only the root path is returned [root-path]/[plugin-name]
		 * If subdir is empty and filename not then a path in the format [root-path]/[plugin-name]/filename is returned.
		 * If both subdir and filename are set then a path in the format [root-path]/[plugin-name]/subdir/filename is returned. */
		QString getPluginFilePath(const QString &root_path, const QString &subdir, const QString &filename) const;

	protected:
		BaseForm *plugin_info_frm;

		MainWindow *main_window;

		/*! \brief This method is executed right before the main window is created and can be used to perform
		 * plugin's initializations like UI modications and other miscellaneous initialization that can't be done
		 * in the plug-in constructor. Additionally, a main window instance must be passed to the plugin in order to facilitate
		 * customizations on the UI. The default implementation is to do nothing else than only expose the main window to the plugin. */
		virtual void initPlugin(MainWindow *main_window);

		/*! \brief Performs operations after the main window is completely loaded/initialized.
		 *  This is useful, for example, to perform signal/slot connections when all components of the
		 *  main window are properly initilized. */
		virtual void postInitPlugin();

		//! \brief Sets all the plugin's attributes at once.
		void configurePluginInfo(const QString &title, const QString &version, const QString &author, const QString &description);

	public:
		enum ActionId {
			//! \brief References the action that will be put in the toolbar at main window
			ToolbarAction,

			//! \brief References the action that will be put in the model's context menu
			ModelAction,

			//! \brief References the action that will be put in the main window menu reserved for plugins settings
			ConfigAction,

			//! \brief References a general purpose action that must be handled by the plugin developer
			OtherAction
		};

		PgModelerPlugin();

		virtual ~PgModelerPlugin();

		//! \brief Returns the plugin's title, this same text is used as action's text on plugins toolbar.
		virtual QString getPluginTitle(void) const = 0;

		//! \brief Returns the plugin's author
		virtual QString getPluginAuthor(void) const = 0;

		//! \brief Returns the plugin's version
		virtual QString getPluginVersion(void) const = 0;

		//! \brief Returns the plugin's complete description
		virtual QString getPluginDescription(void) const = 0;

		//! \brief Shows the plugin's information dialog
		virtual void showPluginInfo(void) const;

		//! \brief Returns the action identified by act_id
		virtual QAction *getAction(ActionId act_id) const = 0;

		//! \brief Returns the tool button inserted in database explorer instances
		virtual QToolButton *getToolButton() const = 0;

		//! \brief Returns the name of the library of the plugin
		QString getLibraryName() const;

		//! \brief Returns the name of the plugin
		QString getPluginName() const;

		//! \brief Returns the path to a plugin icon in the plugin's qrc file
		QString getPluginIcon(const QString &icon_name) const;

		/*! \brief This method mimics the behavior of GlobalAttributes::getConfigurationFilePath
		 * returning the full path to a file inside a subdirectory in the plugin's root directory.
		 * If both subdir and filename are empty, only the full path to the plugin's root directory is returned.
		 * If subdir is empty and filename not then a path in the format [plugin-root]/filename is returned.
		 * If both subdir and filename are set then a path in the format [plugin-root]/subdir/filename is returned. */
		QString getPluginFilePath(const QString &subdir, const QString &filename) const;

		/*! \brief This method mimics the behavior of GlobalAttributes::getTmplConfigurationFilePath
		 * returning the full path to a file inside a subdirectory in the plugin's template confs directory.
		 * If both subdir and filename are empty, only the full path to the plugin's template confs directory is returned.
		 * If subdir is empty and filename not then a path in the format [plugin-tmpl-conf]/filename is returned.
		 * If both subdir and filename are set then a path in the format [plugin-tmpl-conf]/subdir/filename is returned. */
		QString getTmplPluginFilePath(const QString &subdir, const QString &filename) const;

		friend class PluginsConfigWidget;
};

/* Declares the class PgModelerPlugin as interface, this means that the class is a base
	 for plugin implementation. All plugin must inherit this class and use the Q_INTERFACE
	 directive in its declaration  */
Q_DECLARE_INTERFACE(PgModelerPlugin,"io.pgmodeler.PgModelerPlugin")

#endif
