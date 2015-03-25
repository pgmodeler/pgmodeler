/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "objecttablewidget.h"
#include "ui_eventtriggerwidget.h"

class LIBPGMODELER_UI_EXPORT EventTriggerWidget: public BaseObjectWidget, public Ui::EventTriggerWidget {
	private:
		Q_OBJECT

		ObjectTableWidget *filter_tab;

		ObjectSelectorWidget *function_sel;

		void hideEvent(QHideEvent*);

	public:
		EventTriggerWidget(QWidget * parent = 0);
		void setAttributes(DatabaseModel *model, OperationList *op_list, EventTrigger *event_trig);

	public slots:
		void applyConfiguration(void);

	private slots:
		void handleTagValue(int row);
};

#endif
