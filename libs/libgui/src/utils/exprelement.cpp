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

#include "exprelement.h"

ExprElement::ExprElement()
{
	clear();
}

ExprElement::ExprElement(const QString &_pattern, bool _initial, bool _final, bool _exact, bool _case_sensitive)
{
	pattern = _pattern;
	initial = _initial;
	final = _final;
	exact = _exact;
	case_sensitive = _case_sensitive;

	// An expression can't be both initial and final
	if(initial && final)
		initial = final = false;
}

void ExprElement::clear()
{
	pattern.clear();
	initial = final = false;
	exact = case_sensitive = false;
}

QString ExprElement::getPattern() const
{
	return pattern;
}

bool ExprElement::isValid() const
{
	return !pattern.isEmpty();
}

bool ExprElement::isInitial() const
{
	return initial;
}

bool ExprElement::isFinal() const
{
	return final;
}

bool ExprElement::isExact() const
{
	return exact;
}

bool ExprElement::isCaseSensitive() const
{
	return case_sensitive;
}
