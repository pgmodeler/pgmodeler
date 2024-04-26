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

TextBlockInfo::TextBlockInfo()
{
	reset();
}

void TextBlockInfo::reset()
{
	frag_infos.clear();
	group.clear();
	multi_expr = false;
	closed = false;
	closed_once = false;
	allow_completion = false;
	entire_line = false;
}

void TextBlockInfo::addFragmentInfo(const FragmentInfo &f_info)
{
	if(!f_info.isValid())
		return;

	frag_infos.append(f_info);
}

FragmentInfo TextBlockInfo::getFragmentInfo(int start, int length,	bool only_persist)
{
	for(auto &f_info : frag_infos)
	{
		if((only_persist && !f_info.persistent) ||
			 (!only_persist && f_info.persistent))
			continue;

		if(start >= f_info.fmt_start &&
			 start <= (f_info.fmt_start + f_info.fmt_length))
			return f_info;
	}

	return FragmentInfo();
}

void TextBlockInfo::setGroup(const QString &grp)
{
	group = grp;
}

void TextBlockInfo::setClosed(bool value)
{
	if(!closed_once && value)
		closed_once = true;

	closed = value;
}

void TextBlockInfo::setMultiExpr(bool value)
{
	multi_expr = value;

	if(value && entire_line)
		entire_line = false;
}

void TextBlockInfo::setAllowCompletion(bool value)
{
	allow_completion = value;
}

void TextBlockInfo::setEntireLine(bool value)
{
	if(value && multi_expr)
		multi_expr = false;

	entire_line = value;
}

QString TextBlockInfo::getGroup()
{
	return group;
}

bool TextBlockInfo::isMultiExpr()
{
	return multi_expr;
}

bool TextBlockInfo::isClosedOnce()
{
	return closed_once;
}

bool TextBlockInfo::isClosed()
{
	return closed;
}

bool TextBlockInfo::isCompletionAllowed()
{
	return allow_completion;
}

bool TextBlockInfo::isEntireLine()
{
	return entire_line;
}
