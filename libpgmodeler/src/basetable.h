/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\class BaseTable
\brief Base class used to define table like objects (views and tables)
\note <strong>Creation date:</strong> 09/04/2008
*/

#ifndef BASE_TABLE_H
#define BASE_TABLE_H

#include "basegraphicobject.h"

class BaseTable: public BaseGraphicObject {
	public:
		BaseTable(void);
		virtual QString getCodeDefinition(unsigned tipo_def)=0;

		//! \brief Adds an object to the table. It can be inserted at a specified index 'obj_idx'.
		virtual void addObject(BaseObject *obj, int obj_idx=-1)=0;

		//! \brief Gets a object from table through its index and type
		virtual BaseObject *getObject(unsigned obj_idx, ObjectType obj_type)=0;

		//! \brief Gets a object from table through its name and type
		virtual BaseObject *getObject(const QString &name, ObjectType obj_type)=0;

		//! \brief Removes a object from table through its index and type
		virtual void removeObject(unsigned obj_idx, ObjectType obj_type)=0;

		//! \brief Removes a object from table through its name and type
		virtual void removeObject(const QString &name, ObjectType obj_type)=0;

		//! \brief Removes the specified object from table
		virtual void removeObject(BaseObject *obj)=0;

		/*! \brief Gets the the count for the specified object type. The boolean parameter indicates
		 that objects added by relationship must be counted */
		virtual unsigned getObjectCount(ObjectType obj_type, bool inc_added_by_rel=true)=0;

		//! \brief Gets the object index using its name and type
		virtual int getObjectIndex(const QString &name, ObjectType obj_type)=0;

		//! \brief Returns the index for the specified table object
		virtual int getObjectIndex(BaseObject *obj)=0;

		friend class RelacionamentoBase;
		friend class DatabaseModel;
};

#endif
