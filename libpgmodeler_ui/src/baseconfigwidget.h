/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Class: BaseConfigWidget
# Description: Implements base operations to load/save XML based configuration files.
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
#ifndef BASE_CONF_WIDGET_H
#define BASE_CONF_WIDGET_H

#include "exception.h"
#include "xmlparser.h"
#include "parsersattributes.h"
#include <algorithm>

class BaseConfigWidget {
 protected:
  /** @details Stores the configuration params, the main key is the xml element name
     and the value is a map where the key is the attribute name and the value
     is the current attribute value */
  map<QString, map<QString, QString> > config_params;

  /** @details Saves the configuration on file. The conf_id param indicates the type of
     configuration to be saved. (see GlobalAttributes::*_CONF) */
  void saveConfiguration(const QString &conf_id);

  /** @details Loads a configuration from file. The vector key_attribs is used to specify the xml element name
     considered as a key on the configuration map */
  void loadConfiguration(const QString &conf_id, const vector<QString> &key_attribs=vector<QString>());

  /// @details Get a configuratoin key from the xml parser
  void getConfigurationParams(const vector<QString> &key_attribs);

  /// @details Restore the configuration specified by conf_in loading them from the original file (conf/defaults)
  void restoreDefaults(const QString &conf_id);

 public:
  BaseConfigWidget(void){}

  /// @details Adds a configuration param to the configuration map. Replaces the values if the param already exists.
  void addConfigurationParam(const QString &param, const map<QString, QString> &attribs);

  /// @details Gets the parameters loaded from file
  map<QString, map<QString, QString> > getConfigurationParams(void);

  /// @details Removes the values for the specified configuration parameter
  void removeConfigurationParam(const QString &param);

  /// @details Removes all the configuration params
  void removeConfigurationParams(void);

  virtual void applyConfiguration(void)=0;
};

#endif
