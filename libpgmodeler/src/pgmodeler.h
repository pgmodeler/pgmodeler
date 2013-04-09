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
\namespace PgModeler
\brief This namespace is used to organize all functions or constants used in the libpgmodeler package.
*/

#ifndef PGMODELER_H
#define PGMODELER_H

#include "baseobject.h"
#include "indexelement.h"

namespace PgModeler {
	/*! \brief Template function that makes a copy from 'copy_obj' to 'psrc_obj' doing the cast to the
		 correct object type. If the source object (psrc_obj) is not allocated the function allocates the attributes
		 before copying. Both objects must be the same type if both are allocated.
		 -- Brainfuck syntax style! :p -- */
	template <class Classe>
	void copyObject(BaseObject **psrc_obj, Classe *copy_obj);

	/*! \brief This functions is a second way to make a copy between two objects. It simply calls
		 the template function above. */
	void copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type);

	/*! \brief Inserts an index element (expression type) into the specified element vector.
	This function is used in two other classes (Index and Constraint). */
	void addIndexElement(vector<IndexElement> &elements, const QString &expr, Collation *coll, OperatorClass *op_class,
											 bool use_sorting, bool asc_order, bool nulls_first);

	/*! \brief Inserts an index element (column type) into the specified element vector.
	This function is used in two other classes (Index and Constraint). */
	void addIndexElement(vector<IndexElement> &elements, Column *column, Collation *coll, OperatorClass *op_class,
											 bool use_sorting, bool asc_order, bool nulls_first);

	/*! \brief Inserts a pre configured index element into the specified element vector.
	This function is used in two other classes (Index and Constraint). */
	void addIndexElement(vector<IndexElement> &elements, IndexElement elem);

	/*! \brief Returns if the element exists in the specified list returning its index, a negative
	index means that the element does not exists. */
	int isIndexElementExists(vector<IndexElement> &elements, IndexElement elem);
}

#endif
