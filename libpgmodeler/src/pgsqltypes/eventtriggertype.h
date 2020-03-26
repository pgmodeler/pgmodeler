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
\ingroup libpgmodeler/pgsqltypes
\class EventTriggerType
\brief Implements the operations to manipulate event trigger types/modes.
**/

#ifndef EVENT_TRIGGER_TYPE
#define EVENT_TRIGGER_TYPE

#include "templatetype.h"

class EventTriggerType: public TemplateType<EventTriggerType>{
	public:
		static constexpr unsigned DdlCommandStart = 1,
		DdlCommandEnd = 2,
		SqlDrop = 3,
		TableRewrite = 4;

		EventTriggerType(const QString &type_name);
		EventTriggerType(unsigned type_id);
		EventTriggerType();
};

#endif 
