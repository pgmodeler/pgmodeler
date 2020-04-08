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
\class GeneralConfigWidget
\brief Implements the operations to manage general configurations.
*/

#ifndef GENERAL_CONFIG_WIDGET_H
#define GENERAL_CONFIG_WIDGET_H

#include "ui_generalconfigwidget.h"
#include "baseconfigwidget.h"
#include "messagebox.h"
#include "colorpickerwidget.h"
#include "numberedtexteditor.h"
#include "fileselectorwidget.h"

class GeneralConfigWidget: public BaseConfigWidget, public Ui::GeneralConfigWidget {
	private:
		Q_OBJECT

		/* This simple struct is used to store the widgets geometry and maximized state
		 * in order to save this info to configuration file being possible to restore
		 * it when reloading the application */
		struct WidgetState
		{
		  QRect geometry;
		  bool maximized;
		  WidgetState() { maximized = false; }
		};

		QWidgetList child_wgts;

		NumberedTextEditor *font_preview_txt;

		FileSelectorWidget *confs_dir_sel,
		*source_editor_sel;

		static map<QString, WidgetState> widgets_geom;

		static map<QString, attribs_map> config_params;

		static constexpr unsigned UnitMilimeters=0,
		UnitPoint=1,
		UnitInches=2,
		UnitCentimeters=3;

		ColorPickerWidget *line_numbers_cp, *line_numbers_bg_cp, *line_highlight_cp;

	public:
		//! \brief Maximum number of files listed as recent models
		static constexpr int MaxRecentModels=15;

		GeneralConfigWidget(QWidget * parent = nullptr);

		void saveConfiguration();
		void loadConfiguration();
		static void addConfigurationParam(const QString &param, const attribs_map &attribs);
		static void removeConfigurationParam(const QRegExp &param_reg);
		static map<QString, attribs_map> getConfigurationParams();

		/*! \brief Returns a single value of a configuration param in the specified section id.
		Section id can be <configuration>, <dock-widget>, <file[n]> or <recent[n]> */
		static QString getConfigurationParam(const QString &section_id, const QString &param_name);

		static void saveWidgetGeometry(QWidget *widget, const QString &custom_wgt_name = QString());
		static bool restoreWidgetGeometry(QWidget *widget, const QString &custom_wgt_name = QString());

	public slots:
		void applyConfiguration();
		void restoreDefaults();
		void selectPaperSize();

	private slots:
		void convertMarginUnity();
		void updateFontPreview();
		void resetDialogsSizes();
};

#endif
