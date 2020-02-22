/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class OperatorFamily
\brief Implements the operations to manipulate operator family on the database.
\note <strong>Creation date:</strong> 08/07/2008
*/

#ifndef OPERATOR_FAMILY_H
#define OPERATOR_FAMILY_H

#include "baseobject.h"
#include "pgsqltypes/indexingtype.h"

class OperatorFamily: public BaseObject {
	private:
		//! \brief Indexing type used by the operator family
		IndexingType indexing_type;

	public:
		OperatorFamily();

		//! \brief Sets the indexing type of the operator family
		void setIndexingType(IndexingType idx_type);

		//! \brief Returns the indexing type of the operator family
		IndexingType getIndexingType();

		//! \brief Returns the SQL / XML code definition for the operator family
		virtual QString getCodeDefinition(unsigned def_type, bool reduced_form) final;
		virtual QString getCodeDefinition(unsigned def_type) final;

		virtual QString getSignature(bool format=true) final;
};

#endif
