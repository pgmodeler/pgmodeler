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
\class OperatorClass
\brief This class is used to generate the XML/SQL code related to PostgreSQL
operator classes. This type of object indicates to RDBMS how a data type
can be used in a particular indexing method.
\note <strong>Creation date:</strong> 08/07/2008
*/

#ifndef OPERATOR_CLASS_H
#define OPERATOR_CLASS_H

#include "baseobject.h"                                                                                                                                                                                            
#include "operatorclasselement.h"                                                                                                                                                                                  
#include "operatorfamily.h"

class LIBPGMODELER_EXPORT OperatorClass: public BaseObject {
	private:
		//! \brief Data type witch the operator class acts on
		PgSQLType data_type;

		//! \brief Operator family that the operator class is part
		OperatorFamily *family;

		//! \brief Indexing type (method) adopted by the operator class
		IndexingType indexing_type;

		/*! \brief Indicates if the operator class is the default for the
		 specified data type */
		bool is_default;

		//! \brief Operator class attributes list
		vector<OperatorClassElement> elements;

		/*! \brief Formats the elements string used by the SchemaParser on the
		 SQL/XML code definition for operator classes */
		void setElementsAttribute(unsigned def_type);

	public:
		OperatorClass(void);
		~OperatorClass(void);

		//! \brief Sets the data type that the operator class works on
		void setDataType(PgSQLType data_type);

		//! \brief Sets the operator family that the operator class is part
		void setFamily(OperatorFamily *family);

		//! \brief Set the indexing type used by the operator class
		void setIndexingType(IndexingType index_type);

		//! \brief Sets if the opertor class is default for the specified data type
		void setDefault(bool value);

		//! \brief Methods used to manipulate the elements FUNCTION, OPERATOR and STORAGE
		void addElement(OperatorClassElement elem);
		void removeElement(unsigned elem_idx);
		OperatorClassElement getElement(unsigned elem_idx);
		bool isElementExists(OperatorClassElement elem);
		void removeElements(void);

		//! \brief Methods used to obtain the class attributes
		PgSQLType getDataType(void);
		OperatorFamily *getFamily(void);
		IndexingType getIndexingType(void);
		bool isDefault(void);
		unsigned getElementCount(void);

		//! \brief Returns the SQL/XML code definition for operator class
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form) final;
    virtual QString getCodeDefinition(unsigned def_type) final;

    virtual QString getSignature(bool format=true) final;
};

#endif
