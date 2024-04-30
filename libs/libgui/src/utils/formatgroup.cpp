/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "formatgroup.h"

FormatGroup::FormatGroup()
{
	allow_completion = false;
}

FormatGroup::FormatGroup(const QString &_name, const QTextCharFormat &_format, bool _allow_compl, bool _persistent)
{
	name = _name;
	format = _format;
	allow_completion = _allow_compl;
	persistent = _persistent;
}

void FormatGroup::addExpression(const ExprElement &expr)
{
	if(expr.isValid())
		expr_elements.append(expr);
}

QString FormatGroup::getName() const
{
	return name;
}

QTextCharFormat FormatGroup::getFormat() const
{
	return format;
}

bool FormatGroup::isPersistent() const
{
	return persistent;
}

bool FormatGroup::isMultiline() const
{
	bool has_initial = false,
			has_final = false;

	for(auto &expr : expr_elements)
	{
		if(expr.isInitial())
			has_initial = true;

		if(expr.isFinal())
			has_final = true;

		if(has_initial && has_final)
			return true;
	}

	return false;
}

bool FormatGroup::isCompletionAllowed() const
{
	return allow_completion;
}

const QList<ExprElement> &FormatGroup::getExpressions()
{
	return expr_elements;
}
