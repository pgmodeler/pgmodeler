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
\class ObjectsFilterWidget
\brief Implements the operations to generated filter patterns for reverse engineering process.
*/

#ifndef OBJECTS_FILTER_WIDGET_H
#define OBJECTS_FILTER_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QMenu>
#include "baseobject.h"
#include "ui_objectsfilterwidget.h"

class ObjectsFilterWidget : public QWidget, Ui::ObjectsFilterWidget {
	private:
		Q_OBJECT

		vector<ObjectType> extra_obj_types;

		QMenu tab_objs_menu, options_menu;

		QAction *action_only_matching, *action_match_signature, *action_forced_filter;

		//! \brief Creates a combobox of the objects' types accepted in the filter
		QComboBox *createObjectsCombo();

	public:
		explicit ObjectsFilterWidget(QWidget *parent = nullptr);

		//! \brief Returns a list of filters in the format accepted by the Catalog class (object_type:pattern:mode)
		QStringList getObjectFilters();

		//! \brief Returns a list of table children objects to be filtered forcebly
		QStringList getForceObjectsFilter();

		bool isOnlyMatching();
		bool isMatchSignature();
		bool hasFiltersConfigured();

		/*! \brief When value is true the widget disable some features that aren't used
		 * by the object filtering in database models. Additionally, when the model filtering is enabled
		 * extra object types can be provided so they can be enabled in the filters construction */
		void setModelFilteringMode(bool value, const vector<ObjectType> &extra_types = {});

		//! \brief Populates the widget using a list of preconfigured filters
		void addFilters(const QStringList &filters);

	private slots:
		void addFilter();
		void removeFilter();

	public slots:
		void clearFilters();

	signals:
		void s_filterApplyingRequested();
		void s_filtersRemoved();
};

#endif
