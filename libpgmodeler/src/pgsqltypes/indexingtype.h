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
\ingroup libpgmodeler/pgsqltypes
\class IndexingType
\brief Implements the operations to manipulate index access methods (indexing types).
**/

#ifndef INDEXING_TYPE
#define INDEXING_TYPE

#include "templatetype.h"

class IndexingType: public TemplateType<IndexingType>{
	public:
		static constexpr unsigned Btree = 1,
		Gist = 2,
		Hash = 3,
		Gin = 4,
		Spgist = 5,
		Brin = 6;

		IndexingType(const QString &type_name);
		IndexingType(unsigned type_id);
		IndexingType();
};

#endif 
