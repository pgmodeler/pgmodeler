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

ExprElement::ExprElement(const QRegularExpression &regexp, bool initial, bool final)
{
	this->regexp = regexp;
	this->initial = initial;
	this->final = final;
}

void ExprElement::clear()
{
	regexp = QRegularExpression("");
	initial = final = false;
}

const QRegularExpression *ExprElement::getRegExp() const
{
	return &regexp;
}

bool ExprElement::isValid() const
{
	return regexp.isValid();
}

bool ExprElement::isInitial() const
{
	return initial;
}

bool ExprElement::isFinal() const
{
	return final;
}
