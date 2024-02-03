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
\ingroup pgmodeler-cli
\class PgModelerCliPlugin
\brief Implements the basic operations to create third party plugins based upon shared libraries for the CLI application.
*/

#ifndef PGMODELER_CLI_PLUGIN_H
#define PGMODELER_CLI_PLUGIN_H

#include "cliglobal.h"
#include <QObject>
#include <QString>
#include "attribsmap.h"

class PgModelerCliApp;

class __libcli PgModelerCliPlugin {
	private:
		QString libname,

		plugin_name;

		//! \brief Defines the name of the library from where the plugin is being loaded
		void setLibraryName(const QString &lib);

		//! \brief Defines the name of plugin itself. In practical terms, it's the plugin's root folder name
		void setPluginName(const QString &name);

	protected:
		PgModelerCliApp *cli_app;

		virtual void initPlugin(PgModelerCliApp *app);

	public:
		enum OperationId {
			ExportToFile,
			ExportToPng,
			ExportToSvg,
			ExportToDbms,
			ExportToDict,
			ImportDb,
			Diff,
			FixModel,
			CreateConfigs,
			CustomCliOp
		};

		PgModelerCliPlugin();

		virtual ~PgModelerCliPlugin();

		//! \brief Returns the plugin's title, this same text is used as action's text on plugins toolbar.
		virtual QString getPluginTitle() const = 0;

		//! \brief Returns the plugin's author
		virtual QString getPluginAuthor() const = 0;

		//! \brief Returns the plugin's version
		virtual QString getPluginVersion() const = 0;

		//! \brief Returns the plugin's complete description
		virtual QString getPluginDescription() const = 0;

		virtual attribs_map getShortOptions() const = 0;

		virtual std::map<QString, bool> getLongOptions() const = 0;

		virtual attribs_map getOptsDescription() const = 0;

		virtual OperationId getOperationId() const = 0;

		virtual void execBeforeOperation() = 0;
		virtual void execAfterOperation() = 0;
		virtual void execCustomOperation() = 0;

		//! \brief Returns the name of the library of the plugin
		QString getLibraryName() const;

		//! \brief Returns the name of the plugin
		QString getPluginName() const;

		friend class PgModelerCliApp;
};

/* Declares the class PgModelerCliPlugin as interface, this means that the class is a base
	 for plugin implementation. All plugin must inherit this class and use the Q_INTERFACE
	 directive in its declaration  */
Q_DECLARE_INTERFACE(PgModelerCliPlugin,"io.pgmodeler.PgModelerCliPlugin")

#endif
