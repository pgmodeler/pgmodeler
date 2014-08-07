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

#include "collation.h"

Collation::Collation(void)
{
	obj_type=OBJ_COLLATION;
	encoding=BaseType::null;

	attributes[ParsersAttributes::_LC_CTYPE_]="";
	attributes[ParsersAttributes::_LC_COLLATE_]="";
	attributes[ParsersAttributes::LOCALE]="";
	attributes[ParsersAttributes::ENCODING]="";
}

void Collation::setLocale(const QString &locale)
{
	setLocalization(_LC_CTYPE, locale);
	setLocalization(_LC_COLLATE, locale);
	this->locale=locale;
}

void Collation::setLocalization(unsigned lc_id, QString lc_name)
{
	if(locale.isEmpty())
	{	
		if(lc_id > _LC_COLLATE)
			throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* Removes encoding specification from localization e.g 'aa_BB.ENC' will
		 turn into 'aa_BB' since the encoding is appended on code generation */
		lc_name.remove(lc_name.indexOf('.'), lc_name.size());

		setCodeInvalidated(localization[lc_id] != lc_name);
		localization[lc_id]=lc_name;
	}
}

void Collation::setEncoding(EncodingType encoding)
{
	this->encoding=encoding;
}

void Collation::setCollation(BaseObject *collation)
{
	if(collation==this)
		throw Exception(Exception::getErrorMessage(ERR_OBJECT_REFERENCING_ITSELF)
										.arg(Utf8String::create(this->getName(true)))
										.arg(Utf8String::create(this->getTypeName())),
										ERR_OBJECT_REFERENCING_ITSELF,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObject::setCollation(collation);

	encoding=BaseType::null;
	locale.clear();
	localization[0]=localization[1]="";
}

QString Collation::getLocale(void)
{
 return(locale);
}

QString Collation::getLocalization(unsigned lc_id)
{
	if(lc_id > _LC_COLLATE)
		throw Exception(ERR_REF_ELEM_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(localization[lc_id]);
}

EncodingType Collation::getEncoding(void)
{
	return(encoding);
}

QString Collation::getCodeDefinition(unsigned def_type)
{
	return(getCodeDefinition(def_type, false));
}

QString Collation::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return(code_def);

	if(!locale.isEmpty())
	{
		attributes[ParsersAttributes::LOCALE]=locale;

		if(def_type==SchemaParser::SQL_DEFINITION && encoding!=BaseType::null)
			attributes[ParsersAttributes::LOCALE]=locale + "." + (~encoding).toLower();
	}
	else if(collation)
		attributes[ParsersAttributes::COLLATION]=collation->getName(true);
	else
	{
		QString lc_attribs[2]={ ParsersAttributes::_LC_CTYPE_, ParsersAttributes::_LC_COLLATE_ };

		if(localization[_LC_CTYPE].isEmpty() && localization[_LC_COLLATE].isEmpty())
			throw Exception(ERR_EMPTY_LOCAL_ATTRIB_COLLATION,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		for(unsigned int i=_LC_CTYPE; i <= _LC_COLLATE; i++)
		{
			attributes[lc_attribs[i]]=getLocalization(i);

			if(def_type==SchemaParser::SQL_DEFINITION && encoding!=BaseType::null &&
				 !attributes[lc_attribs[i]].isEmpty())
				attributes[lc_attribs[i]]+="." + (~encoding).toLower();
		}
	}

	attributes[ParsersAttributes::ENCODING]=~encoding;
	return(BaseObject::getCodeDefinition(def_type, reduced_form));
}
