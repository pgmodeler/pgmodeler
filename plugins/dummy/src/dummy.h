/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgModeler)
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
\ingroup dummy
\class Dummy
\brief Example plugin for pgModeler (does not execute any complex operation)
*/

#ifndef DUMMY_H
#define DUMMY_H

#include "pgmodelerplugin.h"

class Dummy: public QObject, public PgModelerPlugin {
	private:
		Q_OBJECT

		Q_PLUGIN_METADATA(IID "br.com.pgmodeler.PgModelerPlugin" FILE "dummy.json")

		//! \brief Declares the interface which is used to implement the plugin
		Q_INTERFACES(PgModelerPlugin)

	public:
		Dummy(void);

		QString getPluginTitle(void);
		QString getPluginVersion(void);
		QString getPluginAuthor(void);
		QString getPluginDescription(void);
		QKeySequence getPluginShortcut(void);
		void executePlugin(ModelWidget *);

	public slots:
		void showPluginInfo(void);
};

#endif
