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
\ingroup libutils
\class PgModelerPlugin
\brief Implements the basic operations to create third party plugins based upon shared libraries.
*/

#ifndef PGMODELER_PLUGIN_H
#define PGMODELER_PLUGIN_H

#include "utilsglobal.h"
#include <QString>

class __libutils PgModelerPlugin {
	private:
		QString libname,

		plugin_name;

		//! \brief Defines the name of the library from where the plugin is being loaded
		void setLibraryName(const QString &lib);

		//! \brief Defines the name of plugin itself. In practical terms, it's the plugin's root folder name
		void setPluginName(const QString &name);

	public:
		PgModelerPlugin();

		virtual ~PgModelerPlugin();

		//! \brief Returns the name of the library of the plugin
		QString getLibraryName() const;

		//! \brief Returns the name of the plugin
		QString getPluginName() const;

		/*! \brief This method returns a full path to a file using a plugin's name as the base folder.
		 *
		 * If both subdir and filename are empty, only the base path is returned [root-path]/[plugin-name]
		 * If subdir is empty and filename not then a path in the format [root-path]/[plugin-name]/filename is returned.
		 * If both subdir and filename are set then a path in the format [root-path]/[plugin-name]/subdir/filename is returned.
		 *
		 * NOTE: always validate the path returned by this method since it's not restricted to the
		 * pgModeler installation root! */
		static QString getPluginFilePath(const QString &plugin_name, const QString &root_path, const QString &subdir, const QString &filename);

		/*! \brief This method returns a path of a template file (asset) that resided in a plugin directory which in turn
		 * is located in the plugin's root directory in a pgModeler installation.
		 *
		 * For example, say we have a pgModeler installation in /opt/pgmodeler, and the plugins directory is located at /opt/pgmodeler/plugins.
		 * Now considering that plugin_name is "foo", we'll get the following results depending on subdir and filename values:
		 *
		 *  > subidir = "" and filename = "" --> /opt/pgmodeler/plugins/foo
		 *  > subidir = "bar" and filename = "" --> /opt/pgmodeler/plugins/foo/bar
		 *  > subidir = "bar" and filename = "test.conf" --> /opt/pgmodeler/plugins/foo/bar/test.conf
		 *  > subidir = "" and filename = "test.conf" --> /opt/pgmodeler/plugins/foo/test.conf
		 */
		static QString getPluginTmplFilePath(const QString &plugin_name, const QString &subdir, const QString &filename);

		//! \brief This is the non-static version of getPluginFilePath and uses the this->plugin_name as the base folder
		QString getPluginFilePath(const QString &root_path, const QString &subdir, const QString &filename) const;

		/*! \brief This method mimics the behavior of GlobalAttributes::getConfigFilePath
		 * returning the full path to a file inside a subdirectory in the plugin's configuration directory
		 * inside the pgModeler's user settings directory.
		 *
		 * If both subdir and filename are empty, only the full path to the plugin's configuration directory is returned.
		 * If subdir is empty and filename not then a path in the format [plugin-conf]/filename is returned.
		 * If both subdir and filename are set then a path in the format [plugin-conf]/subdir/filename is returned. */
		QString getConfigFilePath(const QString &subdir, const QString &filename) const;

		/*! \brief This method mimics the behavior of GlobalAttributes::getTmplConfigFilePath
		 * returning the full path to a file inside a subdirectory in the plugin's template confs directory.
		 *
		 * If both subdir and filename are empty, only the full path to the plugin's template confs directory is returned.
		 * If subdir is empty and filename not then a path in the format [plugin-tmpl-conf]/filename is returned.
		 * If both subdir and filename are set then a path in the format [plugin-tmpl-conf]/subdir/filename is returned. */
		QString getTmplConfigFilePath(const QString &subdir, const QString &filename) const;

		friend class PluginsConfigWidget;
		friend class PgModelerCliApp;
};

#endif
