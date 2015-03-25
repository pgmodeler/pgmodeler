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
\class Conversion
\brief Implements the operations to manipulate character encoding conversion on the database
\note <strong>Creation date:</strong> 04/06/2008
*/

#ifndef CONVERSION_H
#define CONVERSION_H

#include "baseobject.h"
#include "function.h"
#include "role.h"

class LIBPGMODELER_EXPORT Conversion: public BaseObject {
	private:
		/*! \brief Encoding types vector:
		 0 -> Source encoding
		 1 -> Destination encoding */
		EncodingType encodings[2];

		//! \brief Function used to perform the conversion between the encodings
		Function *conversion_func;

		//! \brief Indicates whether the conversion is the default for the envolved encodings
		bool is_default;

	public:
		//! \brief Constants used to access the conversion encodings
		static const unsigned SRC_ENCODING=0;
		static const unsigned DST_ENCODING=1;

		Conversion(void);

		//! \brief Defines whether the conversion is the default for the encodings
		void setDefault(bool value);

		//! \brief Sets one of the conversion encodings (using the encoding index constants)
		void setEncoding(unsigned encoding_idx, EncodingType encoding_type);

		//! \brief Sets the conversion function used to convert character between encodings
		void setConversionFunction(Function *conv_func);

		//! \brief Returns the encoding related to the index (using the encoding index constants)
		EncodingType getEncoding(unsigned encoding_idx);

		//! \brief Returns the current used conversion function
		Function *getConversionFunction(void);

		//! \brief Returns if the conversion is the default for the envolved encodings
		bool isDefault(void);

		//! \brief Returns the SQL/XML code definition for the conversion
		virtual QString getCodeDefinition(unsigned def_type) final;
};

#endif
