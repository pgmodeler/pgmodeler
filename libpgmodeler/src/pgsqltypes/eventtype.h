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
\ingroup libpgmodeler/pgsqltypes
\class EventType
\brief Implements the operations to manipulate trigger events.
**/

#ifndef EVENT_TYPE
#define EVENT_TYPE

#include "basetype.h"

class EventType: public BaseType{
	private:
		static constexpr unsigned Offset=11;
		static constexpr unsigned TypesCount=5;

	public:
		static constexpr unsigned OnSelect=Offset;
		static constexpr unsigned OnInsert=Offset+1;
		static constexpr unsigned OnDelete=Offset+2;
		static constexpr unsigned OnUpdate=Offset+3;
		static constexpr unsigned OnTruncate=Offset+4;

		EventType(const QString &type_name);
		EventType(unsigned type_id);
		EventType();

		static void getTypes(QStringList &type_list);

		virtual QString operator ~ ();
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);

		/*! \brief These two operators where created to permit the use the
		 class EventType on STL containers (specially maps) */
		bool operator < (EventType type) const;
		bool operator < (unsigned type_id) const;
};

#endif 
