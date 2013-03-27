/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "collation.h"

unsigned Collation::collation_id=40000;

Collation::Collation(void)
{
	object_id=Collation::collation_id++;
	obj_type=OBJ_COLLATION;
	encoding=BaseType::null;
}

void Collation::setLocale(const QString &locale)
{
	setLocalization(LC_CTYPE, locale);
	setLocalization(LC_COLLATE, locale);
	this->locale=locale;
}

void Collation::setLocalization(int lc_id, const QString &lc_name)
{
	if(locale.isEmpty())
	{
		switch(lc_id)
		{
			case LC_CTYPE: localization[0]=	lc_name; break;
			case LC_COLLATE: localization[1]=	lc_name; break;
			default:
				throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			break;
		}
	}
}

void Collation::setEncoding(EncodingType encoding)
{
	this->encoding=encoding;
}

void Collation::setCollation(Collation *collation)
{
	BaseObject::setCollation(collation);

	encoding=BaseType::null;
	locale.clear();
	localization[0]=localization[1]="";
}

QString Collation::getLocale(void)
{
 return(locale);
}

QString Collation::getLocalization(int lc_id)
{
	switch(lc_id)
	{
		case LC_CTYPE: return(localization[0]);	break;
		case LC_COLLATE: return(localization[1]); break;
		default:
			throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}
}

QString Collation::getCodeDefinition(unsigned def_type)
{
	return(getCodeDefinition(def_type, false));
}

QString Collation::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	return(BaseObject::__getCodeDefinition(def_type));
}


