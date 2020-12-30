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
\ingroup libpgmodeler/pgsqltypes
\class TemplateBaseType
\brief Implements the basic operations to handle specialized types derived from BaseType.

In order to reuse this class' implementation on derivated classes we need to
use CRTP (Curiosly Recurring Template Pattern) which allows Base classes to access Derived classes
members and attributes. In the particular case of this class we need to create a new static type_names
attribute for each time the class is derived. See the derived classes any class *Type derived from TemplateType
to see the CRTP working.

Interesting readings:
https://stackoverflow.com/questions/12796580/static-variable-for-each-derived-class
https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/

**/

#ifndef TEMPLATE_BASE_TYPE_H
#define TEMPLATE_BASE_TYPE_H

#include "basetype.h"

template<class Class>
class TemplateType: public BaseType {
	protected:
		//NOTE: This static attribute MUST BE INITIALIZED in the derived class
		static QStringList type_names;

	public:
		TemplateType();
		virtual ~TemplateType();

		unsigned setType(unsigned type_id);
		unsigned setType(const QString &type_name);
		static QStringList getTypes();

		unsigned operator = (unsigned type_id);
		unsigned operator = (const QString &type_name);
		QString operator ~();
};

template<class Class>
TemplateType<Class>::TemplateType() { }

template<class Class>
TemplateType<Class>::~TemplateType(){ }

template<class Class>
unsigned TemplateType<Class>::setType(unsigned type_id)
{
	return BaseType::setType(type_id, type_names);
}

template<class Class>
unsigned TemplateType<Class>::setType(const QString &type_name)
{
	return BaseType::setType(type_name, type_names);
}

template<class Class>
QStringList TemplateType<Class>::getTypes()
{
	return BaseType::getTypes(type_names);
}

template<class Class>
unsigned TemplateType<Class>::operator = (unsigned type_id)
{
	return BaseType::setType(type_id, type_names);
}

template<class Class>
unsigned TemplateType<Class>::operator = (const QString &type_name)
{
	return BaseType::setType(type_name, type_names);
}

template<class Class>
QString TemplateType<Class>::operator ~ ()
{
	return BaseType::getTypeName(type_idx, type_names);
}

#endif
