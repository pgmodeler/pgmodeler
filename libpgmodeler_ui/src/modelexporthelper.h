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
\ingroup libpgmodeler_ui
\class ModelExportHelper
\brief Implements the base operations to export models (forward engineering)
*/

#ifndef MODEL_EXPORT_HELPER_H
#define MODEL_EXPORT_HELPER_H

#include "modelwidget.h"
#include "connection.h"

class LIBPGMODELER_UI_EXPORT ModelExportHelper: public QObject {
	private:
		Q_OBJECT

		//! \brief  Stores the total progress
		int progress,

		//! \brief  Stores the sql generation progress
		sql_gen_progress;

		/*! \brief Indicates if the database was created on the server (only dbms export).
		This attribute is used to drop the database from server */
		bool db_created;

		//! \brief PostgreSQL version used by the exporter (only in thread mode)
		QString pgsql_ver;

		//! \brief Indicates to the exporter to ignore object duplicity (only in thread mode)
		bool ignore_dup,

		//! \brief Indicates to the exporter to run as a simulator[model validation] (only in thread mode)
		simulate,

		//! \brief Indicates to the exporter to drop database before export
		drop_db,

    //! \brief Indicates to the exporter to drop the objects that are enabled and have a DROP command attached (see SQL source for objects)
    drop_objs,

    //! \brief Indicates to the exporter to generate random names for database, roles and tablespaces before export (only in thread mode)
    use_tmp_names,

		//! \brief Indicates if the exporting thread was canceled by the user (only in thread mode)
    export_canceled,

    db_sql_reenabled;

		//! \brief Database model used as reference on export operation (only in thread mode)
		DatabaseModel *db_model;

		//! \brief Database connection used to export data to DBMS (only in thread mode)
		Connection *connection;

    QString sql_buffer, db_name;

    //! brief List of ignored error codes
    QStringList ignored_errors;

    vector<Exception> errors;

		/*! \brief Indicates which role / tablespaces were created on server (only dbms export).
		This attribute is used to drop the created roles / tablespaces from server */
		map<ObjectType, int> created_objs;

		//! \brief Stores the current state of ALTER command generation for table columns/constraints
		map<Table *, bool> alter_cmds_status;

    //! brief Stores the original object names before the call of generateRandomObjectNames()
    map<BaseObject *, QString> orig_obj_names;

		//! \brief Saves the current state of ALTER command generaton for table columns/constraints
		void saveGenAtlerCmdsStatus(DatabaseModel *db_model);

		//! \brief Retores the previous ALTER command generation state for table columns/constraints
		void restoreGenAtlerCmdsStatus(void);

		//! \brief Revert the dbms export process, removing the created database, roles and tablespaces
    void undoDBMSExport(DatabaseModel *db_model, Connection &conn, bool use_tmp_names);

    /*! brief Cause the names of the database, roles and tablespaces to be replaced by a temporary name in order
    to avoid duplicity error when exporting. This feature is only useful when validating the model against a
    server which some of the objects (at cluster level) still exists */
    void generateTempObjectNames(DatabaseModel *db_model);

    //! brief Restore the original name of the database, roles and tablespaces
    void restoreObjectNames(void);

    //! brief Exports the contents of the buffer to a previously opened connection
    void exportBufferToDBMS(const QString &buffer, Connection &conn, bool drop_objs=false);

    //! brief Returns if the error code is one of the treated by the export process as object duplication error
    bool isDuplicationError(const QString &error_code);

  protected:
    /*! \brief Configures the DBMS export params before start the export thread (only in thread mode).
        This form receive a database model as input and the sql code to be exported will be generated from it.
        \note The params drop_db and drop_objs can't be true at the same time. */
    void setExportToDBMSParams(DatabaseModel *db_model, Connection *conn, const QString &pgsql_ver=QString(), bool ignore_dup=false,
                               bool drop_db=false, bool drop_objs=false, bool simulate=false, bool use_tmp_names=false);

    /*! \brief Configures the DBMS export params before start the export thread (only in thread mode).
        This form receive a previously generated sql buffer to be exported the the helper */
    void setExportToDBMSParams(const QString &sql_buffer, Connection *conn, const QString &db_name, bool ignore_dup=false);

	public:
		ModelExportHelper(QObject *parent = 0);

    /*! brief Determines which error codes must be ignored during the export process.
        There must be some caution when ignore some error codes because the export may
        create an incomplete database or even reach unknown behaviors.
        Error catalog is available at: postgresql.org/docs/current/static/errcodes-appendix.html */
    void setIgnoredErrors(const QStringList &err_codes);

		//! \brief Exports the model to a named SQL file. The PostgreSQL version syntax must be specified.
		void exportToSQL(DatabaseModel *db_model, const QString &filename, const QString &pgsql_ver);

		/*! \brief Exports the model to a named PNG image. The boolean parameters controls the grid exhibition
		as well the page delimiters on the output image. The zoom parameter controls the zoom applied to the viewport
		before draw it on the pixmap */
    void exportToPNG(ObjectsScene *scene, const QString &filename, float zoom, bool show_grid, bool show_delim, bool page_by_page);

		/*! \brief Exports the model directly to the DBMS. A valid connection must be specified. The PostgreSQL
		version is optional, since the helper identifies the version from the server. The boolean parameter
    make the helper to ignore object duplicity errors.
    \note The params drop_db and drop_objs can't be true at the same time. */
    void exportToDBMS(DatabaseModel *db_model, Connection conn, const QString &pgsql_ver=QString(), bool ignore_dup=false,
                      bool drop_db=false, bool drop_objs=false, bool simulate=false, bool use_tmp_names=false);

		/*! \brief When the execution of the instance of this class is in another thread instead of main app
		thread puts the parent thread to sleep for [msecs] ms to give time to external operationsto be correctly
		finished before completely quit the thread itself otherwise the method don't do anything. */
		void sleepThread(unsigned msecs);

  signals:
		//! \brief This singal is emitted whenever the export progress changes
		void s_progressUpdated(int progress, QString msg, ObjectType obj_type=BASE_OBJECT, QString cmd=QString());

		//! \brief This signal is emited when the export has finished
		void s_exportFinished(void);

		//! \brief This signal is emited when the export has been cancelled
		void s_exportCanceled(void);

		//! \brief This signal is emited when the export has encountered a critical error (only in thread mode)
		void s_exportAborted(Exception e);

    //! \brief This signal is emited when the export has encountered a ignorable error (only in thread mode)
    void s_errorIgnored(QString err_code, QString err_msg, QString cmd);

	protected slots:
		void exportToDBMS(void);
		void cancelExport(void);

	private slots:
		//! \brief Updates the exporting progress with the internal progress of sql generation of objects
		void updateProgress(int progress, QString object_id, unsigned obj_type);

	friend class ModelValidationHelper;
	friend class ModelExportForm;
  friend class ModelDatabaseDiffForm;
};

#endif
