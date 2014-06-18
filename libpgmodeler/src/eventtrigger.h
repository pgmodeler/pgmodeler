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
\ingroup libpgmodeler
\class EventTrigger
\brief Implements the operations to manipulate event triggers on the database.
*/

#ifndef EVENT_TRIGGER_H
#define EVENT_TRIGGER_H

#include "baseobject.h"
#include "function.h"

class EventTrigger: public BaseObject {
	private:

		//! \brief Function that is excuted when the trigger is activated
		Function *function;

		EventTriggerMode mode;

		map<QString, QStringList> filter;

	public:
		EventTrigger(void);

		void setMode(EventTriggerMode mode);
		void setFunction(Function *func);
		void setFilter(const QString &variable, const QStringList &values);

		EventTriggerMode getMode(void);
		Function *getFunction(void);
		QStringList getFilter(const QString &variable);

		QString getCodeDefinition(unsigned def_type);
};

#endif
