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
\class Type
\brief Implements the operations to manipulate user defined types on database.
\note <strong>Creation date:</strong> 05/06/2008
*/

#ifndef TYPE_H
#define TYPE_H

#include "baseobject.h"
#include "schema.h"
#include "function.h"
#include "role.h"
#include "typeattribute.h"

class Type: public BaseObject {
	private:
		static unsigned type_id;

		//! \brief Type configuration (BASE | ENUMERATION | COMPOSITE)
		unsigned config;

		//! \brief Attributes for composite type
		vector<TypeAttribute> attributes;

		//! \brief Enumerations of enumeration type
		vector<QString> enumerations;

		//! \brief Functions used by the base type
		Function *functions[7];

		//! \brief Type's internal length ( > 0 - Fixed length, 0 - Variable length)
		unsigned internal_len; //! \brief INTERNALLENGTH

		//! \brief Indicates that the type can be passed by value
		bool by_value; //! \brief PASSEDBYVALUE

		//! \brief Storage alignmnet (char, smallint (int2), integer (int4) ou double precision)
		PgSQLType alignment, //! \brief ALIGNMENT
							element; //! \brief ELEMENT

		//! \brief Type's storage
		StorageType storage; //! \brief STORAGE

		//! \brief Default value for the type
		QString default_value; //! \brief DEFAULT

		//! \brief Type's category (only for base type) - Default 'U'
		CategoryType category; //! \brief CATEGORY

		//! \brief Used with the category attribute (only for base type) - Default FALSE
		bool preferred; //! \brief PREFERRED

		/*! \brief Type which will have some of its attributes copied to the current type
		 (only for base type). If like_type is 'any' means that the
		 current type does not copy attributes of any type */
		PgSQLType like_type; //! \brief LIKE

		//! \brief Character used as value delimiter when the type is used as array
		char delimiter; //! \brief DELIMITER

		//! \brief Checks if the named attribute exists
		bool isAttributeExists(const QString &attrib_name);

		//! \brief Checks if the named enumeration exists
		bool isEnumerationExists(const QString &enum_name);

		//! \brief Formats the elements string used by the SchemaParser
		void setElementsAttribute(unsigned def_type);

		//! \brief Formats the enumeration string used by the SchemaParser
		void setEnumerationsAttribute(unsigned def_type);

		/*! \brief Performs the conversion of parameters and return types of functions.
		 If the parameter 'inverse_conv' is selected, the method
		 makes the conversion from 'type_name' to 'any' otherwise
		 makes the conversion from 'any' to 'type_name'. This method is used
		 when generating the SQL codes for user-defined type */
		void convertFunctionParameters(bool inverse_conv=false);

	public:
		static const unsigned BASE_TYPE=10,
		ENUMERATION_TYPE=20,
		COMPOSITE_TYPE=30;

		static const unsigned INPUT_FUNC=0,
		OUTPUT_FUNC=1,
		RECV_FUNC=2,
		SEND_FUNC=3,
		TPMOD_IN_FUNC=4,
		TPMOD_OUT_FUNC=5,
		ANALYZE_FUNC=6;

		Type(void);
		~Type(void);

		//! \brief Sets the type name
		void setName(const QString &name);

		//! \brief Sets the type schema
		void setSchema(BaseObject *schema);

		//! \brief Defines the type configuration (BASE | ENUMARATION | COMPOSITE)
		void setConfiguration(unsigned conf);

		//! \brief Adds an attribute to the type (only for composite type)
		void addAttribute(TypeAttribute attrib);

		//! \brief Removes an attribute from the type (only for composite type)
		void removeAttribute(unsigned attrib_idx);

		//! \brief Removes all attributes from the type (only for composite type)
		void removeAttributes(void);

		//! \brief Adds an enumeration to the type (only for enumeration type)
		void addEnumeration(const QString &enum_name);

		//! \brief Removes an enumeration from the type (only for enumeration type)
		void removeEnumeration(unsigned enum_idx);

		//! \brief Removes all enumerations from the type (only for enumeration type)
		void removeEnumerations(void);

		//! \brief Sets on function to the type (only for base type)
		void setFunction(unsigned func_id, Function *func);

		//! \brief Sets the type internal length (only for base type)
		void setInternalLength(unsigned length);

		//! \brief Sets if the type can be passed by value (only for base type)
		void setByValue(bool value);

		//! \brief Sets the alignment for the type (only for base type)
		void setAlignment(PgSQLType type);

		//! \brief Sets the storage type (only for base type)
		void setStorage(StorageType strg);

		//! \brief Sets the default value for the type (only for base type)
		void setDefaultValue(const QString &value);

		//! \brief Sets the element for the type (only for base type)
		void setElement(PgSQLType elem);

		//! \brief Sets the delimiter for the type (only for base type)
		void setDelimiter(char delim);

		//! \brief Sets the category for the type (only for base type)
		void setCategory(CategoryType categ);

		//! \brief Sets if the type is preferred (only for base type)
		void setPreferred(bool value);

		//! \brief Sets the type that will be used as template (only for base type)
		void setLikeType(PgSQLType like_type);

		TypeAttribute getAttribute(unsigned attrib_idx);
		unsigned getAttributeCount(void);
		QString getEnumeration(unsigned idx_enum);
		unsigned getEnumerationCount(void);
		unsigned getConfiguration(void);
		CategoryType getCategory(void);
		bool isPreferred(void);
		PgSQLType getLikeType(void);
		Function *getFunction(unsigned func_id);
		unsigned getInternalLength(void);
		bool isByValue(void);
		PgSQLType getAlignment(void);
		StorageType getStorage(void);
		QString getDefaultValue(void);
		PgSQLType getElement(void);
		char getDelimiter(void);

		/*! \brief Returns the SQL / XML definition for the type. If the boolean
		 parameter is set the code definition is generated in a
		 reduced form (XML only) */
		QString getCodeDefinition(unsigned def_type, bool reduced_form);

		//! \brief Returns the SQL / XML definition for the type
		QString getCodeDefinition(unsigned def_type);

		//! \brief Makes a copy between two type
		void operator = (Type &tipo);

		friend class DatabaseModel;
};

#endif
