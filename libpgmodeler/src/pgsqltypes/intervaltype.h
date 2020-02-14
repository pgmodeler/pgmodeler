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
\class IntervalType
\brief Implements the operations to manipulate time interval types.
**/

#ifndef INTERVAL_TYPE
#define INTERVAL_TYPE

#include "basetype.h"

class IntervalType: public BaseType{
	private:
		static constexpr unsigned Offset=139;
		static constexpr unsigned TypesCount=13;

	public:
		static constexpr unsigned Year=Offset;
		static constexpr unsigned Month=Offset+1;
		static constexpr unsigned Day=Offset+2;
		static constexpr unsigned Hour=Offset+3;
		static constexpr unsigned Minute=Offset+4;
		static constexpr unsigned Second=Offset+5;
		static constexpr unsigned YearToMonth=Offset+6;
		static constexpr unsigned DayToHour=Offset+7;
		static constexpr unsigned DayToMinute=Offset+8;
		static constexpr unsigned DayToSecond=Offset+9;
		static constexpr unsigned HourToMinute=Offset+10;
		static constexpr unsigned HourToSecond=Offset+11;
		static constexpr unsigned MinuteToSecond=Offset+12;

		IntervalType(const QString &type_name);
		IntervalType(unsigned type_id);
		IntervalType();

		static void getTypes(QStringList &type_list);
		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
};

#endif 
