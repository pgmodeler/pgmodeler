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

#include "foreignobject.h"

const QString ForeignObject::OptionsSeparator = QString(",");
const QString ForeignObject::OptionValueSeparator = QString("=");

ForeignObject::ForeignObject(void) : BaseObject()
{
	attributes[Attributes::Options] = QString();
}

void ForeignObject::setOption(const QString &opt, const QString &value)
{
	if(opt.isEmpty())
		throw Exception(ErrorCode::AsgOptionInvalidName,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	options[opt] = value;
}

void ForeignObject::setOptions(const attribs_map &options)
{
	for(auto &itr : options)
	{
		if(itr.first.isEmpty())
			throw Exception(ErrorCode::AsgOptionInvalidName,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	this->options = options;
}

void ForeignObject::removeOption(const QString &opt)
{
	options.erase(opt);
}

void ForeignObject::removeOptions(void)
{
	options.clear();
}

attribs_map ForeignObject::getOptions(void)
{
	return(options);
}

void ForeignObject::setOptionsAttribute(unsigned def_type)
{
	QStringList fmt_options;

	for(auto &itr : options)
		fmt_options += def_type == SchemaParser::SqlDefinition ?
																QString("%1 '%2'").arg(itr.first).arg(itr.second) :
																QString("%1%2%3").arg(itr.first).arg(OptionValueSeparator).arg(itr.second);

	attributes[Attributes::Options] = fmt_options.join(OptionsSeparator);
}
