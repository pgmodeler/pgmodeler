/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class GenericSQL
\brief This class is used to represent generic sql commands as database objects
*/

#ifndef GENERIC_SQL_H
#define GENERIC_SQL_H

#include "baseobject.h"
#include <unordered_map>

class GenericSQL: public BaseObject{
	protected:
		struct ObjectRefConfig {
			QString ref_name;
			BaseObject *object;
			bool use_signature, format_name;
			ObjectRefConfig(const QString &_ref_name, BaseObject *_object, bool _use_signature, bool _format_name) :
				ref_name(_ref_name), object(_object), use_signature(_use_signature), format_name(_format_name) {}
		};

		vector<ObjectRefConfig> getObjectsReferences(void);

	private:
		QString definition;
		vector<ObjectRefConfig> objects_refs;

		int getObjectRefNameIndex(const QString &ref_name);
		void validateObjectReference(ObjectRefConfig ref, bool ignore_duplic);

	public:
		GenericSQL(void);

		void setDefinition(const QString &def);
		QString getDefinition(void);

		void addObjectReference(BaseObject *object, const QString &ref_name, bool use_signature, bool format_name);
		void updateObjectReference(const QString &ref_name, BaseObject *object, const QString &new_ref_name, bool use_signature, bool format_name);
		void removeObjectReference(const QString &ref_name);
		void removeObjectReferences(void);

		bool isObjectReferenced(BaseObject *object);

		/*! \brief Returns whether the object references columns or constraints added
		 * by relationship to their parent tables. This method is used as auxiliary
		 * to control which generic SQL object references objects added by the
		 * relationship in order to avoid referece breaking due constants
		 * connections and disconnections of relationships */
		bool isReferRelationshipAddedObject(void);

		vector<BaseObject *> getReferencedObjects(void);

		virtual QString getCodeDefinition(unsigned def_type);

		friend class GenericSQLWidget;
};

#endif
