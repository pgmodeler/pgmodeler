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

class ConfigurationForm: public QDialog, public Ui::ConfigurationForm {
	private:
		Q_OBJECT

		Messagebox msg_box;
		GeneralConfigWidget *general_conf;
		AppearanceConfigWidget *appearance_conf;
		ConnectionsConfigWidget *connections_conf;
		PluginsConfigWidget *plugins_conf;


	public:
		static const int	GENERAL_CONF_WGT=0,
											APPEARANCE_CONF_WGT=1,
											CONNECTIONS_CONF_WGT=2,
											PLUGINS_CONF_WGT=3;

		ConfigurationForm(QWidget * parent = 0, Qt::WindowFlags f = 0);

		BaseConfigWidget *getConfigurationWidget(unsigned idx);

	public slots:
		void applyConfiguration(void);
		void loadConfiguration(void);
		void close(void);

	private slots:
		void restoreDefaults(void);
};

#endif
