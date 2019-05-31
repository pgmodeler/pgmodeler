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

QString ForeignObject::getAlterDefinition(BaseObject *object)
{
	try
	{
		attribs_map attribs;
		ForeignObject *fobj = dynamic_cast<ForeignObject *>(object);
		QStringList opts;

		if(!fobj)
			throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		// Comparing options (to be modified or added)
		for(auto &opt : fobj->options)
		{
			if(this->options.count(opt.first) == 0)
				opts.push_back(QString("ADD %1 '%2'").arg(opt.first).arg(opt.second));
			else if(this->options[opt.first] != opt.second)
				opts.push_back(QString("SET %1 '%3'").arg(opt.first).arg(opt.second));
		}

		// Comparing options (to be removed)
		for(auto &opt : this->options)
		{
			if(fobj->options.count(opt.first) == 0)
				opts.push_back(QString("DROP %1").arg(opt.first));
		}

		attribs[Attributes::Options] = opts.join(OptionsSeparator);
		copyAttributes(attribs);

		return(BaseObject::getAlterDefinition(object));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
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
