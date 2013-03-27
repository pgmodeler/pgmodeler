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
	base_encoding=BaseType::null;
	loc_encoding[0]=loc_encoding[1]=BaseType::null;
}

void Collation::setlocale(const QString &locale, EncodingType encoding)
{
	setlocalization(LC_CTYPE, locale, encoding);
	setlocalization(LC_COLLATE, locale, encoding);
	this->locale=locale;
}

void Collation::setlocalization(int lc_id, const QString &lc_name, EncodingType encoding)
{
	if(locale.isEmpty())
	{
		unsigned idx;

		switch(lc_id)
		{
			case LC_CTYPE: idx=0;	break;
			case LC_COLLATE: idx=1; break;
			default:
				throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			break;
		}

		localization[idx]=lc_name;
		loc_encoding[idx]=encoding;
	}
}

void Collation::setCollation(Collation *collation)
{
	BaseObject::setCollation(collation);
	base_encoding=BaseType::null;

	locale.clear();
	for(unsigned i=0; i < 2; i++)
	{
		loc_encoding[i]=BaseType::null;
		localization[i].clear();
	}
}

void Collation::getLocale(QString &locale, EncodingType &encoding)
{
 locale=this->locale;
 encoding=this->base_encoding;
}

void Collation::getLocalization(int lc_id, QString &lc_name, EncodingType &encoding)
{
	unsigned idx;

	switch(lc_id)
	{
		case LC_CTYPE: idx=0;	break;
		case LC_COLLATE: idx=1; break;
		default:
			throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		break;
	}

	lc_name=localization[idx];
	encoding=loc_encoding[idx];
}

QString Collation::getCodeDefinition(unsigned def_type)
{
	return(getCodeDefinition(def_type, false));
}

QString Collation::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	return(BaseObject::__getCodeDefinition(def_type));
}


