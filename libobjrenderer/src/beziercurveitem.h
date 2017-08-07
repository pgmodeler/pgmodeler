/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

/**
\ingroup libobjrenderer
\class BezierCurveItem
\brief Represents a simple bezier curve item (used by relationships)
*/

#ifndef BEZIER_CURVE_ITEM_H
#define BEZIER_CURVE_ITEM_H

#include <QGraphicsPathItem>

class BezierCurveItem: public QGraphicsPathItem {
	private:
		QPainterPath stroke;

		bool simple_curve;

	protected:
		void setPath(const QPainterPath &path);

	public:
		BezierCurveItem(QGraphicsItem *parent = 0);

		void setLine(const QLineF &line, bool simple_curve, unsigned rel_type);

		bool isSimpleCurve(void);

		virtual bool contains(const QPointF &pnt) const;

		virtual bool collidesWithPath(const QPainterPath &path, Qt::ItemSelectionMode) const;
};

#endif

