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

#include "textblockinfo.h"

TextBlockInfo::TextBlockInfo(void)
{
	reset();
}

void TextBlockInfo::reset(void)
{
	group.clear();
	is_multi_expr = false;
	is_closed = false;
	closed_once = false;
	allow_completion = false;
}

void TextBlockInfo::setGroup(const QString &grp)
{
	group = grp;
}

void TextBlockInfo::setClosed(bool value)
{
	if(!closed_once && value)
		closed_once = true;

	is_closed = value;
}

void TextBlockInfo::setMultiExpr(bool value)
{
	is_multi_expr = value;
}

void TextBlockInfo::setAllowCompletion(bool value)
{
	allow_completion = value;
}

QString TextBlockInfo::getGroup()
{
	return group;
}

bool TextBlockInfo::isMultiExpr()
{
	return is_multi_expr;
}

bool TextBlockInfo::isClosedOnce()
{
	return closed_once;
}

bool TextBlockInfo::isClosed()
{
	return is_closed;
}

bool TextBlockInfo::isCompletionAllowed()
{
	return allow_completion;
}
