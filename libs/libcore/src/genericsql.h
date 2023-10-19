/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libcore
\class GenericSQL
\brief This class is used to represent generic sql commands as database objects
*/

#ifndef GENERIC_SQL_H
#define GENERIC_SQL_H

#include "baseobject.h"
#include <unordered_map>

class __libcore GenericSQL: public BaseObject{
	public:

		//! \brief This is a internal structure used to hold object references configuration
		struct ObjectReference {
			// Name of the reference (in SQL it be used between {} in order to be parsed)
			QString ref_name;

			// The object being referenced
			BaseObject *object;

			// Indicates that the signature of the object should be used instead of the name
			bool use_signature,

			// Indicates that the name of the object need to be automatically quoted or the schema name appended
			format_name;

			// An optional alias for the reference (used only in View code definition)
			QString	ref_alias;

			ObjectReference()
			{
				object = nullptr;
				use_signature = format_name = false;
			}

			ObjectReference(const QString &_ref_name, BaseObject *_object, bool _use_signature,
											bool _format_name, const QString &_ref_alias = "") :
				ref_name(_ref_name), object(_object), use_signature(_use_signature),
				format_name(_format_name), ref_alias(_ref_alias) {}

			bool isValid() const
			{
				return object && !ref_name.isEmpty();
			}
		};

	private:
		//! \brief The SQL definition of the generic object
		QString definition;

		/*! \brief Indicate whether the SQL comment description (-- object: ... type: ...)
		 * of the generic object must be displayed in the SQL code generation */
		bool hide_description;

		//! \brief The list of references to other object in the model
		std::vector<ObjectReference> objects_refs;

		/*! \brief Returns the index of a object reference searching by its name.
		 * A negative return value indicates the reference doens't exist */
		int getObjectRefNameIndex(const QString &ref_name);

		/*! \brief Check if the provided object reference is correclty configured.
		 * The method will raise exceptions if any validation rule is broken.
		 * The parameter ignore_duplic makes the method ignore duplicated references names */
		void validateObjectReference(ObjectReference ref, bool ignore_duplic);

	public:
		GenericSQL();

		virtual ~GenericSQL(){}

		void setDefinition(const QString &def);

		QString getDefinition();

		void setHideDescription(bool value);

		[[deprecated("Use addObjectReference(ObjectRerence) instead.")]]
		void addObjectReference(BaseObject *object, const QString &ref_name, bool use_signature,
														bool format_name, const QString &ref_alias = "");

		void addObjectReference(const GenericSQL::ObjectReference &ref);

		void addObjectReferences(const std::vector<GenericSQL::ObjectReference> &refs);

		[[deprecated]]
		void updateObjectReference(const QString &ref_name, BaseObject *object, const QString &new_ref_name, bool use_signature,
															 bool format_name, const QString &ref_alias = "");

		[[deprecated]]
		void updateObjectReference(const QString &ref_name, const GenericSQL::ObjectReference &new_ref);

		void removeObjectReference(const QString &ref_name);
		void removeObjectReferences();

		[[deprecated]]
		//! \brief Returns true when the provided object is being referenced by the generic SQL object
		bool isObjectReferenced(BaseObject *object);

		/*! \brief Returns whether the object references columns or constraints added
		 * by relationship to their parent tables. This method is used as auxiliary
		 * to control which generic SQL object references objects added by the
		 * relationship in order to avoid referece breaking due constants
		 * connections and disconnections of relationships */
		bool isReferRelationshipAddedObject();

		//! \brief Returns a copy of the objects references list
		std::vector<ObjectReference> getObjectsReferences();

		void validateObjectReferences(bool ignore_duplic);

		virtual QString getSourceCode(SchemaParser::CodeType def_type) override;

		virtual void updateDependencies() override;

		friend class GenericSQLWidget;
};

/* Registering the GenericSQL::ObjectRefConfig struct as a Qt MetaType in order to make
 * it liable to be sent through signal parameters as well as to be
 * to be used by QVariant */
Q_DECLARE_METATYPE(GenericSQL::ObjectReference)

#endif
