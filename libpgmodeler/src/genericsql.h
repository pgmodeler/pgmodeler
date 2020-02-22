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
\class GenericSQL
\brief This class is used to represent generic sql commands as database objects
*/

#ifndef GENERIC_SQL_H
#define GENERIC_SQL_H

#include "baseobject.h"
#include <unordered_map>

class GenericSQL: public BaseObject{
	protected:

		//! \brief This is a internal structure used to hold object references configuration
		struct ObjectRefConfig {
			QString ref_name; // Name of the reference (in SQL it be used between {} in order to be parsed)
			BaseObject *object; // The object being referenced
			bool use_signature,  // Indicates that the signature of the object should be used instead of the name
			format_name; // Indicates that the name of the object need to be automatically quoted or the schema name appended
			ObjectRefConfig(const QString &_ref_name, BaseObject *_object, bool _use_signature, bool _format_name) :
				ref_name(_ref_name), object(_object), use_signature(_use_signature), format_name(_format_name) {}
		};

		//! \brief Returns a copy of the objects references list
		vector<ObjectRefConfig> getObjectsReferences();

	private:
		//! \brief The SQL definition of the generic object
		QString definition;

		//! \brief The list of references to other object in the model
		vector<ObjectRefConfig> objects_refs;

		/*! \brief Returns the index of a object reference searching by its name.
		 * A negative return value indicates the reference doens't exist */
		int getObjectRefNameIndex(const QString &ref_name);

		/*! \brief Check if the provided object reference is correclty configured.
		 * The method will raise exceptions if any validation rule is broken.
		 * The parameter ignore_duplic makes the method ignore duplicated references names */
		void validateObjectReference(ObjectRefConfig ref, bool ignore_duplic);

	public:
		GenericSQL();

		void setDefinition(const QString &def);
		QString getDefinition();

		void addObjectReference(BaseObject *object, const QString &ref_name, bool use_signature, bool format_name);
		void updateObjectReference(const QString &ref_name, BaseObject *object, const QString &new_ref_name, bool use_signature, bool format_name);
		void removeObjectReference(const QString &ref_name);
		void removeObjectReferences();

		//! \brief Returns true when the provided object is being referenced by the generic SQL object
		bool isObjectReferenced(BaseObject *object);

		/*! \brief Returns whether the object references columns or constraints added
		 * by relationship to their parent tables. This method is used as auxiliary
		 * to control which generic SQL object references objects added by the
		 * relationship in order to avoid referece breaking due constants
		 * connections and disconnections of relationships */
		bool isReferRelationshipAddedObject();

		/*! \brief Returns a list of objectes being referenced by the generic object.
		 * For performance reasons this method doesn't eliminate duplicated values in
		 * the retunring list*/
		vector<BaseObject *> getReferencedObjects();

		virtual QString getCodeDefinition(unsigned def_type);

		friend class GenericSQLWidget;
};

#endif
