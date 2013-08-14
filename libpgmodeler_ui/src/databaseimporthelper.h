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
\ingroup libpgmodeler_ui
\class DatabaseImportHelper
\brief Implements the base operations to import existing databases into model (reverse engineering)
*/

#ifndef DATABASE_IMPORT_HELPER_H
#define DATABASE_IMPORT_HELPER_H

#include <QObject>
#include <QThread>
#include "catalog.h"

class DatabaseImportHelper: public QObject {
	private:
		Q_OBJECT

		Catalog catalog;

		Connection connection;

		bool import_canceled;

		vector<unsigned> object_oids;

		map<unsigned, vector<unsigned>> column_oids;

	public:
		DatabaseImportHelper(QObject *parent=0);

		void setConnection(Connection &conn);
		void setCurrentDatabase(const QString &dbname);
		void setObjectsOIDS(vector<unsigned> &obj_oids, map<unsigned, vector<unsigned>> col_oids);

		/*! \brief Returns an attribute map for the specified object type. The parameters "schema" and "table"
				must be used only when retrieving table children objects.
				\note: The database used as reference is the same as the currently connection. So,
				if the user want a different database it must call Connection::switchToDatabase() method
				before assigne the connection to this class. */
		attribs_map getObjects(ObjectType obj_type, const QString schema="", const QString table="");

	signals:
		//! \brief This singal is emitted whenever the export progress changes
		void s_progressUpdated(int progress, QString msg, ObjectType obj_type=BASE_OBJECT);

		//! \brief This signal is emited when the import has finished
		void s_importFinished(void);

		//! \brief This signal is emited when the import has been cancelled
		void s_importCanceled(void);

		//! \brief This signal is emited when the import has encountered a critical error (only in thread mode)
		void s_importAborted(Exception e);

	protected slots:
		void cancelImport(void);

	public slots:
		void importDatabase(void);
		
	friend class DatabaseImportForm;
};

#endif
