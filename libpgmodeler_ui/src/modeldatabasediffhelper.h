/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\ingroup libpgmodeler_ui
\class ModelDatabaseDiffHelper
\brief Implements the operations to compare and generate a diff between model and database.
*/

#ifndef MODEL_DATABASE_DIFF_HELPER_H
#define MODEL_DATABASE_DIFF_HELPER_H

#include <QObject>

class ModelDatabaseDiffHelper: public QObject {
	private:
		Q_OBJECT

	public:
		ModelDatabaseDiffHelper(void);

	public slots:

};

#endif
