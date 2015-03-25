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
\class Tablespace
\brief Implements the operations to manipulate tablespace on the database.
\note <strong>Creation date:</strong> 05/06/2008
*/

#ifndef TABLESPACE_H
#define TABLESPACE_H

#include "baseobject.h"

class LIBPGMODELER_EXPORT Tablespace: public BaseObject{
	private:
		static unsigned tabspace_id;

		//! \brief Directory where the tablespace resides
		QString directory;

	public:
		Tablespace(void);

		void setName(const QString &name);

		//! \brief Sets the directory where tablespace resides
		void setDirectory(const QString &dir);

		//! \brief Returns the directory where the tablespace resides
		QString getDirectory(void);

		//! \brief Returns the SQL / XML code for the tablespace
		virtual QString getCodeDefinition(unsigned def_type) final;
};

#endif
