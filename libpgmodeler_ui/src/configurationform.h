/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class ConfigurationForm
\brief Reunites in a single form all available configuration widgets.
*/

#ifndef CONFIGURATION_FORM_H
#define CONFIGURATION_FORM_H

#include "ui_configurationform.h"
#include "appearanceconfigwidget.h"
#include "generalconfigwidget.h"
#include "connectionsconfigwidget.h"
#include "pluginsconfigwidget.h"
#include "relationshipconfigwidget.h"
#include "snippetsconfigwidget.h"

class LIBPGMODELER_UI_EXPORT ConfigurationForm: public QDialog, public Ui::ConfigurationForm {
	private:
		Q_OBJECT

		GeneralConfigWidget *general_conf;
		AppearanceConfigWidget *appearance_conf;
		ConnectionsConfigWidget *connections_conf;
		RelationshipConfigWidget *relationships_conf;
    SnippetsConfigWidget *snippets_conf;
    PluginsConfigWidget *plugins_conf;

    void hideEvent(QHideEvent *);

	public:
		static const int	GENERAL_CONF_WGT=0;
		static const int	RELATIONSHIPS_CONF_WGT=1;
		static const int	APPEARANCE_CONF_WGT=2;
		static const int	CONNECTIONS_CONF_WGT=3;
		static const int	SNIPPETS_CONF_WGT=4;
		static const int	PLUGINS_CONF_WGT=5;

		ConfigurationForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

		BaseConfigWidget *getConfigurationWidget(unsigned idx);

	public slots:
		void applyConfiguration(void);
		void loadConfiguration(void);
		void reject(void);

	private slots:
		void restoreDefaults(void);
};

#endif
