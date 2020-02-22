/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class BaseConfigWidget
\brief Implements base operations to load/save XML based configuration files. This class reunites basic methods to perform
	   the load and save operation. Each subclass must satisfy the contract (implement abstract methods) and have their own
	   static configuration parameter set as following: map<QStrig, attribs_map>
*/

#ifndef BASE_CONF_WIDGET_H
#define BASE_CONF_WIDGET_H

#include "exception.h"
#include "xmlparser.h"
#include "attributes.h"
#include <algorithm>
#include <QWidget>

class BaseConfigWidget: public QWidget {
	private:
		Q_OBJECT
		
		bool config_changed;
		
	protected:
		XmlParser xmlparser;
		SchemaParser schparser;
		
		/*! \brief Saves the configuration params on file. The conf_id param indicates the type of
			configuration to be saved. (see GlobalAttributes::*_CONF) and config_params the map containing the
			configuration values */
		void saveConfiguration(const QString &conf_id, map<QString, attribs_map> &config_params);
		
		/*! \brief Loads a configuration from file. The vector key_attribs is used to specify the xml element name
		 considered as a key on the configuration map */
		void loadConfiguration(const QString &conf_id, map<QString, attribs_map> &config_params, const vector<QString> &key_attribs=vector<QString>());
		
		//! \brief Get a configuratoin key from the xml parser
		void getConfigurationParams(map<QString, attribs_map> &config_params, const vector<QString> &key_attribs);
		
		/*! \brief Restore the configuration specified by conf_in loading them from the original file (conf/defaults)
		 * The silent parameter indicates that the restoration should not emit a message box informing the restoration sucess */
		void restoreDefaults(const QString &conf_id, bool silent);
		
		//! \brief Adds a parameter to the specified configuration parameters set
		static void addConfigurationParam(map<QString, attribs_map> &config_params, const QString &param, const attribs_map &attribs);
		
		void showEvent(QShowEvent *);
		
	public:
		BaseConfigWidget(QWidget *parent = nullptr);
		~BaseConfigWidget(void){}
		
		bool isConfigurationChanged();
		
		//! \brief Applies the configuration to object
		virtual void applyConfiguration(void)=0;
		
		//! \brief Loads a set of configurations from a file
		virtual void loadConfiguration(void)=0;
		
		//! \brief Saves the current settings to a file
		virtual void saveConfiguration(void)=0;
		
		//! \brief Destroy the current configuration file and makes a copy of the default one located at conf/defaults
		virtual void restoreDefaults(void)=0;
		
	public slots:
		void setConfigurationChanged(bool changed=true);
};

#endif
