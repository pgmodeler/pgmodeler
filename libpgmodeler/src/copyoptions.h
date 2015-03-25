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
\class CopyOptions
\brief Auxiliary class that helps to control LIKE (Copy tables) options
*/

#ifndef COPY_OPTIONS_H
#define COPY_OPTIONS_H

#include "baseobject.h"
#include <QString>

class LIBPGMODELER_EXPORT CopyOptions {
	private:
		unsigned copy_mode, copy_op_ids;

	public:
		static const unsigned DEFAULTS=1;
		static const unsigned CONSTRAINTS=2;
		static const unsigned INDEXES=4;
		static const unsigned STORAGE=8;
		static const unsigned COMMENTS=16;
		static const unsigned ALL=31;
		static const unsigned INCLUDING=64;
		static const unsigned EXCLUDING=128;

		CopyOptions(void);
		CopyOptions(unsigned copy_mode, unsigned copy_op_ids);

		unsigned getCopyMode(void);
		unsigned getCopyOptionsIds(void);

		bool isOptionSet(unsigned op);
		bool isIncluding(void);
		bool isExcluding(void);

		QString getSQLDefinition(void);
		bool operator != (CopyOptions &cp);
};

#endif
