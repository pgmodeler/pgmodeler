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
\class EventTriggerWidget
\brief Implements the operations to create/edit event triggers via form.
*/

#ifndef EVENT_TRIGGER_WIDGET_H
#define EVENT_TRIGGER_WIDGET_H

#include "baseobjectwidget.h"
#include "objectstablewidget.h"
#include "ui_eventtriggerwidget.h"

class EventTriggerWidget: public BaseObjectWidget, public Ui::EventTriggerWidget {
	private:
		Q_OBJECT
		
		ObjectsTableWidget *filter_tab;
		
		ObjectSelectorWidget *function_sel;
		
	public:
		EventTriggerWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, EventTrigger *event_trig);
		
	public slots:
		void applyConfiguration();
		
	private slots:
		void handleTagValue(int row);
};

#endif
