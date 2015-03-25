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
\ingroup libpgmodeler
\class Cast
\brief Implements the operations to manipulate data type cast on the database
\note <strong>Creation date:</strong> 03/06/2008
*/

#ifndef CAST_H
#define CAST_H

#include "baseobject.h"
#include "function.h"

class LIBPGMODELER_EXPORT Cast: public BaseObject {
	private:
		/*! \brief Data types used on the cast:
		0 -> Source data type
		1 -> Destination data type */
		PgSQLType types[2];

		//! \brief Conversion type (ASSIGNMENT or IMPLICIT)
		unsigned cast_type;

		//! \brief Function used to do the type cast
		Function *cast_function;

		/*! \brief Indicates that the type cast is inout, this means that
		 the function used to cast the types will be the 'output'
		 function of the source data type */
		bool is_in_out;

	public:
		//! \brief Constants used to access the data types envolved in the cast
		static const unsigned SRC_TYPE=0;
		static const unsigned DST_TYPE=1;
		//! \brief Constants used to define the cast type
		static const unsigned ASSIGNMENT=10;
		static const unsigned IMPLICIT=20;
		static const unsigned EXPLICIT=30;
		Cast(void);

		/*! \brief Defines one of the data types envolved on the cast
		 (using constants SRC_TYPE | DST_TYPE) */
		void setDataType(unsigned type_idx, PgSQLType type);

		//! \brief Defines the cast type (using constants ASSINGMENT | IMPLICIT)
		void setCastType(unsigned cast_type);

		//! \brief Defines the function used to do the type cast
		void setCastFunction(Function *cast_func);

		/*! \brief Defines if the conversion is inout. If so the conversion function
		 assigned to the cast is  ignored since the PostgreSQL will use
		 the output function of the source type. */
		void setInOut(bool value);

		//! \brief Returns one of the cast envolved data types
		PgSQLType getDataType(unsigned type_idx);

		//! \brief Returns the cast type
		unsigned getCastType(void);

		//! \brief Returns the cast function
		Function *getCastFunction(void);

		//! \brief Returs whether the cast is inout or not
		bool isInOut(void);

		//! \brief Returns the SQL/XML code definition for the cast
		virtual QString getCodeDefinition(unsigned def_type) final;

    virtual QString getSignature(bool=false) final;
};

#endif
