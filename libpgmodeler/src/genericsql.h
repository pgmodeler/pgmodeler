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
	private:		

		struct RefConfig {
			QString ref_name;
			BaseObject *object;
			bool use_signature, format_name;
			RefConfig(const QString &_ref_name, BaseObject *_object, bool _use_signature, bool _format_name) :
				ref_name(_ref_name), object(_object), use_signature(_use_signature), format_name(_format_name) {}
		};

		QString definition;

		vector<RefConfig> objects_refs;

		bool isObjectRefNameExists(const QString &ref_name);

	public:
		GenericSQL(void);

		void setDefinition(const QString &def);
		QString getDefinition(void);

		void setParseable(bool value);
		bool isParseable(void);

		void addObjectReference(const QString &ref_name, BaseObject *object, bool use_signature, bool format_name);
		map<QString, BaseObject *> getObjectReferences(void);
		void removeObjectReferences(void);
		bool isObjectReferenced(BaseObject *object);

		virtual QString getCodeDefinition(unsigned def_type);
};

#endif
