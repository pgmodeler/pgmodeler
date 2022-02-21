/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libcore
\class SimpleColumn
\brief This simple struct simulate view columns.
*/

#ifndef SIMPLE_COLUMN_H
#define SIMPLE_COLUMN_H

#include <QString>
#include <qmetatype.h>

/*! \brief This simple struct acts like a rudimentary column used by Reference and Views to represent columns
 * Note that, as intended, the struct expects that all received values are validated since they are immutable */
struct SimpleColumn {
		QString name, type, alias;

		SimpleColumn(){}

		SimpleColumn(const SimpleColumn &col) : SimpleColumn(col.name, col.type, col.alias) {}

		SimpleColumn(const QString &_name, const QString &_type, const QString &_alias)
		{
			name = _name;
			type = _type;
			alias = _alias;
		}

		bool operator == (const SimpleColumn &col) const
		{
			return name == col.name && type == col.type && alias == col.alias;
		}

		SimpleColumn & operator = (const SimpleColumn &col)
		{
			this->name = col.name;
			this->type = col.type;
			this->alias = col.alias;
			return *this;
		}

		bool isValid() const
		{
			return !name.isEmpty() && !type.isEmpty();
		}
};

// Declaring SimpleColumn as a metatype in order to use it in QVariant
Q_DECLARE_METATYPE(SimpleColumn)

#endif
