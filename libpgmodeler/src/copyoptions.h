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
		static constexpr unsigned Defaults=1,
		Constraints=2,
		Indexes=4,
		Storage=8,
		Comments=16,
		Identity=32,
		Statistics=64,
		All=127,
		Including=256,
		Excluding=512;

		CopyOptions();
		CopyOptions(unsigned copy_mode, unsigned copy_op_ids);

		unsigned getCopyMode();
		unsigned getCopyOptionsIds();

		bool isOptionSet(unsigned op);
		bool isIncluding();
		bool isExcluding();

		QString getSQLDefinition();
		bool operator != (CopyOptions &cp);
};

#endif
