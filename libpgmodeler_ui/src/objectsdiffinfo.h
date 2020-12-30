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
\ingroup libpgmodeler_ui
\class ObjectsDiffInfo
\brief Stores information about difference between two objects when the models are being compared. Instances of this class
are emitted during the diff process on ModelsDiffHelper class.
*/

#ifndef OBJECTS_DIFF_INFO_H
#define OBJECTS_DIFF_INFO_H

#include "tableobject.h"

class ObjectsDiffInfo {
	private:
		//! \brief Difference type (see constants below)
		unsigned diff_type;

		BaseObject *object, *old_object;

	public:
		static constexpr unsigned CreateObject=0,
		DropObject=1,
		AlterObject=2,
		IgnoreObject=3,
		NoDifference=4;

		ObjectsDiffInfo();
		ObjectsDiffInfo(unsigned diff_type, BaseObject *ref_object, BaseObject *old_object);

		unsigned getDiffType();
		QString getInfoMessage();
		QString getDiffTypeString();
		BaseObject *getObject();
		BaseObject *getOldObject();

		bool operator == (ObjectsDiffInfo &info);

		friend class ModelsDiffHelper;
		friend class QVariant;
};

#endif
