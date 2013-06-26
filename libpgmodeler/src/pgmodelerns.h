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
\namespace PgModelerNS
\brief This namespace is used to organize all functions or constants used in the libpgmodeler package.
*/

#ifndef PGMODELER_NS_H
#define PGMODELER_NS_H

#include "baseobject.h"
#include "indexelement.h"

namespace PgModelerNS {
	/*! \brief Template function that makes a copy from 'copy_obj' to 'psrc_obj' doing the cast to the
		 correct object type. If the source object (psrc_obj) is not allocated the function allocates the attributes
		 before copying. Both objects must be the same type if both are allocated.
		 -- Brainfuck syntax style! :p -- */
	template <class Classe>
	void copyObject(BaseObject **psrc_obj, Classe *copy_obj);

	/*! \brief This functions is a second way to make a copy between two objects. It simply calls
		 the template function above. */
	void copyObject(BaseObject **psrc_obj, BaseObject *copy_obj, ObjectType obj_type);
}

#endif
