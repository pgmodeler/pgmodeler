/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "hinttextwidget.h"
#include "colorpickerwidget.h"
#include "numberedtexteditor.h"

class GeneralConfigWidget: public BaseConfigWidget, public Ui::GeneralConfigWidget {
	private:
		Q_OBJECT

		QWidgetList child_wgts;

		NumberedTextEditor *font_preview_txt;

		static map<QString, attribs_map> config_params;

		static const unsigned UNIT_MILIMETERS=0,
		UNIT_POINT=1,
		UNIT_INCHS=2,
		UNIT_CENTIMETERS=3;

		HintTextWidget *simp_obj_creation_ht, *confirm_validation_ht, *corner_move_ht,
		*save_last_pos_ht, *invert_rangesel_ht, *disable_smooth_ht,
		*hide_ext_attribs_ht, *hide_table_tags_ht, *hide_rel_name_ht,
		*code_completion_ht, *use_placeholders_ht, *min_obj_opacity_ht;

		ColorPickerWidget *line_numbers_cp, *line_numbers_bg_cp, *line_highlight_cp;

		void hideEvent(QHideEvent *);

	public:
		GeneralConfigWidget(QWidget * parent=0);

		void saveConfiguration(void);
		void loadConfiguration(void);
		static void addConfigurationParam(const QString &param, const attribs_map &attribs);
		static void removeConfigurationParam(const QRegExp &param_reg);
		static map<QString, attribs_map> getConfigurationParams(void);

		/*! \brief Returns a single value of a configuration param in the specified section id.
		Section id can be <configuration>, <dock-widget>, <file[n]> or <recent[n]> */
		static QString getConfigurationParam(const QString &section_id, const QString &param_name);

	public slots:
		void applyConfiguration(void);
		void restoreDefaults(void);
		void selectPaperSize(void);
		void selectSourceEditor(void);

	private slots:
		void convertMarginUnity(void);
		void updateFontPreview(void);
};

#endif
