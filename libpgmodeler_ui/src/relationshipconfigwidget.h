/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class RelationshipConfigWidget
\brief Implements the operations to manage global settings for relationship objects.
*/

#ifndef RELATIONSHIP_CONFIG_WIDGET_H
#define RELATIONSHIP_CONFIG_WIDGET_H

#include "ui_relationshipconfigwidget.h"
#include "baseconfigwidget.h"
#include "pgsqltypes.h"
#include "syntaxhighlighter.h"
#include "relationshipview.h"
#include "hinttextwidget.h"

class RelationshipConfigWidget:  public BaseConfigWidget, public Ui::RelationshipConfigWidget {
	private:
		Q_OBJECT

		static map<QString, attribs_map> config_params;

		map<QString, attribs_map> patterns;

		HintTextWidget *fk_to_pk_ht, *center_pnts_ht, *tab_edges_ht, *crows_foot_ht;

		void hideEvent(QHideEvent *);

	public:
		RelationshipConfigWidget(QWidget * parent = nullptr);

		void saveConfiguration(void);
		void loadConfiguration(void);
		static map<QString, attribs_map> getConfigurationParams(void);

	public slots:
		void applyConfiguration(void);
		void restoreDefaults(void);

	private slots:
		void fillNamePatterns(void);
		void updatePattern(void);
		void enableConnModePreview(void);

};

#endif
