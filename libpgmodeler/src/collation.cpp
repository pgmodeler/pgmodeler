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

#include "collation.h"

Collation::Collation()
{
	obj_type = ObjectType::Collation;
	encoding = BaseType::Null;
	is_deterministic = false;

	attributes[Attributes::LcCtype] = "";
	attributes[Attributes::LcCollate] = "";
	attributes[Attributes::Locale] = "";
	attributes[Attributes::LcCtypeMod] = "";
	attributes[Attributes::LcCollateMod] = "";
	attributes[Attributes::LocaleMod] = "";
	attributes[Attributes::Encoding] = "";
	attributes[Attributes::Provider] = "";
	attributes[Attributes::Deterministic] = "";
}

void Collation::setLocale(const QString &locale)
{
	setLocalization(LcCtype, locale);
	setLocalization(LcCollate, locale);
	this->locale=locale;
}

void Collation::setLocalization(unsigned lc_id, QString lc_name)
{
	if(locale.isEmpty())
	{
		int pos = -1;

		if(lc_id > LcCollate)
			throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(lc_name.contains('@'))
		{
			/* Removes and stores separately the modifier from the localization e.g 'aa_BB.ENC@modifier' will
			 * turn into 'aa_BB' since the modifier is appended on code generation */
			pos = lc_name.indexOf('@');
			modifier[lc_id] = lc_name.mid(pos + 1, lc_name.size());
			lc_name.remove(pos, lc_name.size());
		}

		if(lc_name.contains('.'))
		{
			/* Removes encoding specification from the localization e.g 'aa_BB.ENC' will
			 * turn into 'aa_BB' since the encoding is appended on code generation */
			lc_name.remove(lc_name.indexOf('.'), lc_name.size());
		}

		setCodeInvalidated(localization[lc_id] != lc_name);
		localization[lc_id]=lc_name;
	}
}

void Collation::setModifier(unsigned lc_id, QString mod)
{
	if(lc_id > Locale)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	setCodeInvalidated(modifier[lc_id] != mod);
	modifier[lc_id] = mod;
}

QString Collation::getModifier(unsigned lc_id)
{
	if(lc_id > Locale)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return modifier[lc_id];
}

void Collation::setEncoding(EncodingType encoding)
{
	this->encoding=encoding;
}

void Collation::setCollation(BaseObject *collation)
{
	if(collation==this)
		throw Exception(Exception::getErrorMessage(ErrorCode::ObjectReferencingItself)
						.arg(this->getName(true))
						.arg(this->getTypeName()),
						ErrorCode::ObjectReferencingItself,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObject::setCollation(collation);

	encoding = BaseType::Null;
	locale.clear();
	localization[0] = localization[1]= "";
	provider = ProviderType::Null;
	modifier[0] = modifier[1] = modifier[2] = "";
	is_deterministic = false;
}

QString Collation::getLocale()
{
	return locale;
}

QString Collation::getLocalization(unsigned lc_id)
{
	if(lc_id > LcCollate)
		throw Exception(ErrorCode::RefElementInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return localization[lc_id];
}

EncodingType Collation::getEncoding()
{
	return encoding;
}

void Collation::setProvider(ProviderType type)
{
	setCodeInvalidated(provider != type);
	provider = type;
}

ProviderType Collation::getProvider()
{
	return provider;
}

void Collation::setDeterministic(bool value)
{
	setCodeInvalidated(is_deterministic != value);
	is_deterministic = value;
}

bool Collation::isDeterministic()
{
	return is_deterministic;
}

QString Collation::getCodeDefinition(unsigned def_type)
{
	return getCodeDefinition(def_type, false);
}

QString Collation::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	QString fmt_encoding;
	fmt_encoding = "." + (~encoding).toLower();

	if(!locale.isEmpty())
	{
		attributes[Attributes::Locale]=locale;

		if(def_type==SchemaParser::SqlDefinition && encoding!=BaseType::Null)
			attributes[Attributes::Locale]=locale + fmt_encoding;
	}
	else if(collation)
		attributes[Attributes::Collation]=collation->getName(true);
	else
	{
		QString lc_attribs[2]={ Attributes::LcCtype, Attributes::LcCollate };

		if(localization[LcCtype].isEmpty() && localization[LcCollate].isEmpty())
			throw Exception(ErrorCode::EmptyLCCollationAttributes,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		for(unsigned int i=LcCtype; i <= LcCollate; i++)
		{
			attributes[lc_attribs[i]]=getLocalization(i);

			if(def_type==SchemaParser::SqlDefinition && encoding!=BaseType::Null && !attributes[lc_attribs[i]].isEmpty())
				attributes[lc_attribs[i]] += fmt_encoding;
		}
	}

	attributes[Attributes::Encoding] = ~encoding;
	attributes[Attributes::Provider] = ~provider;
	attributes[Attributes::Deterministic]= is_deterministic ? Attributes::True : Attributes::False;
	attributes[Attributes::LocaleMod] = modifier[Locale];
	attributes[Attributes::LcCollateMod] = modifier[LcCollate];
	attributes[Attributes::LcCtypeMod] = modifier[LcCtype];

	return BaseObject::getCodeDefinition(def_type, reduced_form);
}

QString Collation::getAlterDefinition(BaseObject *object)
{
	try
	{
		attributes[Attributes::AlterCmds]=BaseObject::getAlterDefinition(object);
		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
