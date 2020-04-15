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
\class AppearanceConfigWidget
\brief Implements the operations to manage graphical objects appearance configuration.
*/

#ifndef APPEARANCE_CONFIG_WIDGET_H
#define APPEARANCE_CONFIG_WIDGET_H

#include "ui_appearanceconfigwidget.h"
#include "baseconfigwidget.h"
#include "colorpickerwidget.h"
#include "objectsscene.h"
#include "databasemodel.h"
#include <algorithm>

class AppearanceConfigWidget: public BaseConfigWidget, public Ui::AppearanceConfigWidget  {
	private:
		Q_OBJECT
		
		static map<QString, attribs_map> config_params;
		
		//! \brief Auxiliary class that stores the formating data of each element
		class AppearanceConfigItem {
			public:
				QString conf_id;
				QTextCharFormat font_fmt;
				QColor colors[3];
				bool obj_conf;
		};
		
		RoundedRectItem *placeholder;
		
		ColorPickerWidget *color_picker;
		
		//! \brief Color picker dialog
		QColorDialog color_dlg;
		
		//! \brief Viewport used to show the example model
		QGraphicsView *viewp;
		
		//! \brief Object scene used to store the graphical objects
		ObjectsScene *scene;
		
		//! \brief Database model used to store the example base objects
		DatabaseModel *model;
		
		//! \brief Stores the element configuration items
		vector<AppearanceConfigItem> conf_items;
		
		//! \brief Loads the example model from file (conf/exampledb.dbm)
		void loadExampleModel();
		
		//! \brief Updates the color configuration for the placeholder item
		void updatePlaceholderItem();
		
	public:
		AppearanceConfigWidget(QWidget * parent = nullptr);
		virtual ~AppearanceConfigWidget();
		
		void saveConfiguration();
		void loadConfiguration();
		static map<QString, attribs_map> getConfigurationParams();
		
	private slots:
		void enableConfigElement();
		void applyFontStyle();
		void applyElementColor(unsigned color_idx, QColor color);
		
		/*! \brief Disabled method */
		void applyConfiguration(void){}
		
	public slots:
		void restoreDefaults();
};

#endif
