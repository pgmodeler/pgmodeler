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
\class CopyOptions
\brief Auxiliary class that helps to control LIKE (Copy tables) options
*/

#ifndef COPY_OPTIONS_H
#define COPY_OPTIONS_H

#include "baseobject.h"
#include <QString>

class CopyOptions {
	private:
		unsigned copy_mode, copy_op_ids;

	public:
		static const unsigned DEFAULTS=1,
													CONSTRAINTS=2,
													INDEXES=4,
													STORAGE=8,
													COMMENTS=16,
													ALL=31,
													INCLUDING=64,
													EXCLUDING=128;

		CopyOptions(void);
		CopyOptions(unsigned copy_mode, unsigned copy_op_ids);

		unsigned getCopyMode(void);
		unsigned getCopyOptionsIds(void);

		bool isOptionSet(unsigned op);
		bool isIncluding(void);
		bool isExcluding(void);

		QString getSQLDefinition(void);
};

#endif
