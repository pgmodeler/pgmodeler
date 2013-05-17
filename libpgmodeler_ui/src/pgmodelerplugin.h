/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
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
\class PgModelerPlugin
\brief Implements the basic operations to create third party plugins based upon shared libraries.
*/

#ifndef PGMODELER_PLUGIN_H
#define PGMODELER_PLUGIN_H

#include "modelwidget.h"

/*	The plugins in pgModeler must be within the "plugins" folder in its own
		directory and must have the following basic structure:


		 [PGMODELER_PLUGINS_DIR]/
					|
					+ - pluginA/
							 |
							 + ---- (lib)*(pluginA.)(so|dylib|dll) (library)
							 |
							 + ---- pluginA.png (icon)

		> Library: it is the shared object that represents the plugin. The prefix (lib) and suffix (so|dylib|dll) are plataform dependent.
		> Icon: it is a PNG image that represents the plugin on the plugins toolbar.

		Note: Plugins can have subdirectories but any reference to them must be made programatically by the plugin author. */

class PgModelerPlugin {
	protected:
		BaseForm *plugin_info_frm;

	private:
		QLabel	*icon_lbl,
						*title_lbl,
						*author_lbl,
						*version_lbl,
						*description_lbl;

	public:
		PgModelerPlugin(void);

		virtual ~PgModelerPlugin(void);

		//! \brief Executes the plugins having a ModelWidget as input parameter.
		virtual void executePlugin(ModelWidget *modelo)=0;

		//! \brief Returns the plugin's title, this same text is used as action's text on plugins toolbar.
		virtual QString getPluginTitle(void)=0;

		//! \brief Returns the plugin's author
		virtual QString getPluginAuthor(void)=0;

		//! \brief Returns the plugin's version
		virtual QString getPluginVersion(void)=0;

		//! \brief Returns the plugin's complete description
		virtual QString getPluginDescription(void)=0;

		//! \brief Shows the plugin's information dialog
		virtual void showPluginInfo(void) = 0;

		//! \brief Returns the plugin's action shortcut
		virtual QKeySequence getPluginShortcut(void) = 0;

		//! \brief Sets the plugin's all attributes at once.
		void configurePluginInfo(const QString &title, const QString &version, const QString &author,
														 const QString &description, const QString &ico_filename);
};

/* Declares the class PgModelerPlugin as interface, this means that the class is a base
	 for plugin implementation. All plugin must inherit this class and use the Q_INTERFACE
	 directive in its declaration  */
Q_DECLARE_INTERFACE(PgModelerPlugin,"br.com.pgmodeler.PgModelerPlugin")

#endif
