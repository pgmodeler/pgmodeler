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
\class DatabaseModelsDiffHelper
\brief Implements the operations to compare and generate a diff between two database models.
*/

#ifndef MODELS_DIFF_HELPER_H
#define MODELS_DIFF_HELPER_H

#include <QObject>
#include "databasemodel.h"
#include "objectsdiffinfo.h"

class LIBPGMODELER_UI_EXPORT ModelsDiffHelper: public QObject {
	private:
		Q_OBJECT

    //! brief Stores the SQL code that represents the diff between model and database
    QString diff_def,

    //! brief PostgreSQL version used to generate the diff
    pgsql_version;

    //! brief Indicates if the diff was cancelled by user
    bool diff_canceled,

    /*! brief Indicates if cluster level objects like tablespaces or roles must be kept intact on
        database in case of drop */
    keep_cluster_objs,

    //! brief Indicates if any DROP/TRUNCATE generated must be in cascade mode
    cascade_mode,

    //! brief Forces the recreation of any object maked as ALTER in the output
    force_recreation,

    //! brief Recreates only objects that can't be modified using ALTER commands
    recreate_unchangeble,

    //! brief Generate a TRUNCATE command for every table which columns was modified in their data types
    trucante_tables,

    //! brief Indicates if permissions must be preserved on database
    keep_obj_perms,

    /*! brief Indicates that existing sequences must be reused in serial columns. Since serial columns are converted
        into integer and a new sequence created and assigned as nextval(sequence) default value for those columns,
        if reuse is enabled, new sequences will not be created instead the ones which name matches the column's default
        value will be reused */
    reuse_sequences;

    //! brief Stores the count of objects to be dropped, changed or created
    unsigned diffs_counter[4];

    //! brief Reference model from which all changes are generated
    DatabaseModel *source_model,

    //! brief Model which is compared to the source one
    *imported_model;

    //! brief Stores all generated diff information during the process
		vector<ObjectsDiffInfo> diff_infos;

    //! brief Stores all temporary objects created during the diff process
    vector<BaseObject *> tmp_objects;

    /*! note The parameter diff_type in any methods below is one of the values in
        ObjectsDiffInfo::CREATE_OBJECT|ALTER_OBJECT|DROP_OBJECT */

    //! brief Compares two tables storing the diff between them in the diff_infos vector.
		void diffTables(Table *src_table, Table *imp_table, unsigned diff_type);

    //! brief Compares the two models storing the diff between them in the diff_infos vector.
		void diffModels(unsigned diff_type);

    /*! brief Compares the specified table object against the ones on the source model or imported
        model depending on the diff_type parameter. */
		void diffTableObject(TableObject *tab_obj, unsigned diff_type);

    //! brief Creates a diff info instance storing in o diff_infos vector
    void generateDiffInfo(unsigned diff_type, BaseObject *object, BaseObject *old_object=nullptr);

    /*! brief Processes the generated diff infos resulting in a SQL buffer with the needed commands
        to synchronize both model and database */
    void processDiffInfos(void);

    /*! brief Generates the proper DROP and CREATE for the specified object and its references. This method
        is used when the force_recreation is true and the object in the parameter is marked with an ALTER_OBJECT */
    void recreateObject(BaseObject *object, vector<BaseObject *> &drop_objs, vector<BaseObject *> &create_objs);

    /*! brief Returns if a diff information exists for the object. The exact_match parameter is used to force the
        comparison of all values on the paramenter against the diff infos. When false the exact_match parameter
        considers one of parameters object or old_object to be used (if not null) */
    bool isDiffInfoExists(unsigned diff_type, BaseObject *object, BaseObject *old_object, bool exact_match = true);

    /*! brief Generate the proper code definition for the table's child objects. If drop_cmd is true a DROP command
        will be generated otherwise a CREATE is generated. */
    QString getCodeDefinition(BaseObject *object, bool drop_cmd);

    //! brief Destroy the temporary objects and clears the diff info list
    void destroyTempObjects(void);

    BaseObject *getRelNNTable(const QString &obj_name, DatabaseModel *model);

  public:
		ModelsDiffHelper(void);
    ~ModelsDiffHelper(void);

    /*! brief Configures the models to be compared. It is assumed that src_model is the reference model
        from which all changes must be collected and applied to the database. The imp_model is the
        database model that represents the current arrange of the database. */
    void setModels(DatabaseModel *src_model, DatabaseModel *imp_model);

    //! brief Configures the set of diff options before start the process
    void setDiffOptions(bool keep_cluster_objs, bool cascade_mode, bool truncate_tables,
                        bool force_recreation, bool recreate_unchangeble, bool keep_obj_perms,
                        bool reuse_sequences);

    //! brief Configures the PostgreSQL version used in the diff generation
    void setPgSQLVersion(const QString pgsql_ver);

    //! brief Returns the count of diff infos of the specified diff_type
		unsigned getDiffTypeCount(unsigned diff_type);

    //! brief Reset all the diff info counters in order to restart the diff process
		void resetDiffCounter(void);

    //! brief Returns the diff containing all the SQL commands needed to synchronize the model and database
    QString getDiffDefinition(void);

  public slots:
		void diffModels(void);
		void cancelDiff(void);

	signals:
		//! \brief This singal is emitted whenever the diff progress changes
		void s_progressUpdated(int progress, QString msg, ObjectType obj_type=BASE_OBJECT);	

		//! \brief This signal is emited when the diff has finished
		void s_diffFinished(void);

		//! \brief This signal is emited when the diff has been canceled
		void s_diffCanceled(void);

		//! \brief This signal is emited when the diffhas encountered a critical error (only in thread mode)
		void s_diffAborted(Exception e);

		//! \brief This signal is emitted when a diff info is generated
		void s_objectsDiffInfoGenerated(ObjectsDiffInfo diff_info);
};

#endif
