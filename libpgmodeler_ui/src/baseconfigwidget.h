/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\brief Implements base operations to load/save XML based configuration files.
*/

#ifndef BASE_CONF_WIDGET_H
#define BASE_CONF_WIDGET_H

#include "exception.h"
#include "xmlparser.h"
#include "parsersattributes.h"
#include <algorithm>

class BaseConfigWidget {
	protected:
		XMLParser xmlparser;

		SchemaParser schparser;

		/*! \brief Stores the configuration params, the main key is the xml element name
		 and the value is a map where the key is the attribute name and the value
		 is the current attribute value */
		map<QString, attribs_map > config_params;

		/*! \brief Saves the configuration on file. The conf_id param indicates the type of
		 configuration to be saved. (see GlobalAttributes::*_CONF) */
		void saveConfiguration(const QString &conf_id);

		/*! \brief Loads a configuration from file. The vector key_attribs is used to specify the xml element name
		 considered as a key on the configuration map */
		void loadConfiguration(const QString &conf_id, const vector<QString> &key_attribs=vector<QString>());

		//! \brief Get a configuratoin key from the xml parser
		void getConfigurationParams(const vector<QString> &key_attribs);

		//! \brief Restore the configuration specified by conf_in loading them from the original file (conf/defaults)
		void restoreDefaults(const QString &conf_id);

	public:
		BaseConfigWidget(void){}

		//! \brief Adds a configuration param to the configuration map. Replaces the values if the param already exists.
		void addConfigurationParam(const QString &param, const attribs_map &attribs);

		//! \brief Gets the parameters loaded from file
		map<QString, attribs_map > getConfigurationParams(void);

		//! \brief Removes the values for the specified configuration parameter
		void removeConfigurationParam(const QString &param);

		//! \brief Removes all the configuration params
		void removeConfigurationParams(void);

		virtual void applyConfiguration(void)=0;
};

#endif
