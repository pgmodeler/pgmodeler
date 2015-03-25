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
\class ObjectsDiffInfo
\brief Stores information about difference between two objects when the models are being compared. Instances of this class
are emitted during the diff process on ModelsDiffHelper class.
*/

#ifndef OBJECTS_DIFF_INFO_H
#define OBJECTS_DIFF_INFO_H

#include "tableobject.h"

class LIBPGMODELER_UI_EXPORT ObjectsDiffInfo {
	private:
		//! \brief Difference type (see constants below)
		unsigned diff_type;

    BaseObject *object, *old_object;

	public:
		static const unsigned CREATE_OBJECT;
		static const unsigned DROP_OBJECT;
		static const unsigned ALTER_OBJECT;
		static const unsigned IGNORE_OBJECT;
		static const unsigned NO_DIFFERENCE;

		ObjectsDiffInfo(void);
    ObjectsDiffInfo(unsigned diff_type, BaseObject *ref_object, BaseObject *old_object);

		unsigned getDiffType(void);
		QString getInfoMessage(void);
		BaseObject *getObject(void);
    BaseObject *getOldObject(void);

    bool operator == (ObjectsDiffInfo &info);

		friend class ModelsDiffHelper;
		friend class QVariant;
};

#endif
