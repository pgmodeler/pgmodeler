/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "custommenustyle.h"

int CustomMenuStyle::icon_pixel_metric = 25;

CustomMenuStyle::CustomMenuStyle()
{

}

void CustomMenuStyle::setIconPixelMetric(int icon_pm)
{
	icon_pixel_metric = icon_pm;
}

int CustomMenuStyle::pixelMetric(PixelMetric metric, const QStyleOption * option, const QWidget * widget) const
{
	if (metric == QStyle::PM_SmallIconSize && icon_pixel_metric > 0)
		return icon_pixel_metric;

	return QProxyStyle::pixelMetric(metric, option, widget);
}
