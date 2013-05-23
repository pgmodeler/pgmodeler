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
#include "operatorclass.h"

class Type: public BaseObject {
	private:
		//! \brief Type configuration (BASE | ENUMERATION | COMPOSITE | RANGE)
		unsigned config;

		//! \brief Attributes for composite type
		vector<TypeAttribute> attributes;

		//! \brief Enumerations of enumeration type
		vector<QString> enumerations;

		//! \brief Functions used by the base type
		Function *functions[9];

		//! \brief Type's internal length ( > 0 - Fixed length, 0 - Variable length)
		unsigned internal_len; //! \brief INTERNALLENGTH

		//! \brief Indicates that the type can be passed by value
		bool by_value, // PASSEDBYVALUE

		//! \brief Indicates if type can use collations
		collatable, //COLLATABLE

		//! \brief Used with the category attribute (only for base type) - Default FALSE
		preferred; //PREFERRED

		//! \brief Storage alignmnet (char, smallint (int2), integer (int4) ou double precision)
		PgSQLType alignment, //ALIGNMENT
							element; //ELEMENT

		//! \brief Type's storage
		StorageType storage; //STORAGE

		//! \brief Default value for the type
		QString default_value; //DEFAULT

		//! \brief Type's category (only for base type) - Default 'U'
		CategoryType category; //CATEGORY

		/*! \brief Type which will have some of its attributes copied to the current type
		 (only for base type). If like_type is 'any' means that the
		 current type does not copy attributes of any type */
		PgSQLType like_type,  //LIKE

		//! \brief Subtype used by a range type
		subtype; //SUBTYPE

		//! \brief Character used as value delimiter when the type is used as array
		char delimiter; //DELIMITER

		//! \brief Operator class associated to the subtype (only for range type)
		OperatorClass *subtype_opclass;

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
		static constexpr unsigned BASE_TYPE=10,
															ENUMERATION_TYPE=11,
															COMPOSITE_TYPE=12,
															RANGE_TYPE=13;

		static constexpr unsigned INPUT_FUNC=0,
															OUTPUT_FUNC=1,
															RECV_FUNC=2,
															SEND_FUNC=3,
															TPMOD_IN_FUNC=4,
															TPMOD_OUT_FUNC=5,
															ANALYZE_FUNC=6,
															CANONICAL_FUNC=7,
															SUBTYPE_DIFF_FUNC=8;

		Type(void);
		~Type(void);

		//! \brief Sets the type name
		void setName(const QString &name);

		//! \brief Sets the type schema
		void setSchema(BaseObject *schema);

		/*! \brief Defines the type configuration (BASE | ENUMARATION | COMPOSITE | RANGE).
		Calling this method causes all attribute to be reset, so it may be executed before
		any other attribute of the type be defined. */
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

		//! \brief Sets if the type can use collation (only for base type)
		void setCollatable(bool value);

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

		//! \brief Sets the subtype that will be used by the range (only for range type)
		void setSubtype(PgSQLType subtype);

		//! \brief Sets the subtype operator class (only for range type)
		void setSubtypeOpClass(OperatorClass *opclass);

		PgSQLType getSubtype(void);
		OperatorClass *getSubtypeOpClass(void);
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
		bool isCollatable(void);
		PgSQLType getAlignment(void);
		StorageType getStorage(void);
		QString getDefaultValue(void);
		PgSQLType getElement(void);
		char getDelimiter(void);

		/*! \brief Returns the SQL / XML definition for the type. If the boolean
		 parameter is set the code definition is generated in a
		 reduced form (XML only) */
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form) final;

		//! \brief Returns the SQL / XML definition for the type
		virtual QString getCodeDefinition(unsigned def_type) final;

		//! \brief Makes a copy between two type
		void operator = (Type &tipo);

		friend class DatabaseModel;
};

#endif
