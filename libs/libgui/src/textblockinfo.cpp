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

bool TextBlockInfo::isValid()
{
	return !group.isEmpty();
}
