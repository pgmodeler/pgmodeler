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
\class EventType
\brief Implements the operations to manipulate trigger events.
**/

#ifndef EVENT_TYPE
#define EVENT_TYPE

#include "templatetype.h"

class EventType: public TemplateType<EventType>{
	public:
		static constexpr unsigned OnSelect = 1,
		OnInsert = 2,
		OnDelete = 3,
		OnUpdate = 4,
		OnTruncate = 5;

		EventType(const QString &type_name);
		EventType(unsigned type_id);
		EventType();

		/*! \brief These two operators were created to permit the use of the
		 class EventType on STL containers (specially maps) */
		bool operator < (EventType type) const;
		bool operator < (unsigned type_id) const;
};

#endif 
