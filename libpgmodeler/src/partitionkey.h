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
\brief Implements the operations to manipulate partition key elements.
*/

#ifndef PARTITION_KEY_H
#define PARTITION_KEY_H

#include "element.h"

class PartitionKey: public Element {
	private:
		//! \brief Collation referenced by the element
		Collation *collation;

	public:
		PartitionKey();

		//! \brief Defines the collation referenced by the element
		void setCollation(Collation *collation);

		//! \brief Get the collation referenced by the element
		Collation *getCollation();

		//! \brief Returns the SQL / XML code definition for the partition key
		virtual QString getCodeDefinition(unsigned def_type) final;
};

#endif
