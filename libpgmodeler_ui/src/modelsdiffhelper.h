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
\class DatabaseModelsDiffHelper
\brief Implements the operations to compare and generate a diff between two database models.
*/

#ifndef MODELS_DIFF_HELPER_H
#define MODELS_DIFF_HELPER_H

#include <QObject>
#include "databasemodel.h"
#include "objectsdiffinfo.h"

class ModelsDiffHelper: public QObject {
	private:
		Q_OBJECT

		bool diff_canceled, keep_cluster_objs;

		unsigned diffs_counter[3];

		DatabaseModel *source_model, *imported_model;

		vector<ObjectsDiffInfo> diff_infos;

		void diffTables(Table *src_table, Table *imp_table, unsigned diff_type);
		void diffModels(unsigned diff_type);
		void diffTableObject(TableObject *tab_obj, unsigned diff_type);
    void generateDiffInfo(unsigned diff_type, BaseObject *src_object, BaseObject *new_object=nullptr);
    void processDiffInfos(void);

	public:
		ModelsDiffHelper(void);

		void setModels(DatabaseModel *src_model, DatabaseModel *imp_model, bool keep_cluster_objs);
		unsigned getDiffTypeCount(unsigned diff_type);
		void resetDiffCounter(void);

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
