/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2021 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class TextPolygonItem
\brief This specialization of QGraphicsPathItem allows draws texts on custom positions in a path item mainly to denote layer boxes in a canvas.
*/

#ifndef LAYER_PATH_ITEM_H
#define LAYER_PATH_ITEM_H

#include <QGraphicsPathItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class LayerPathItem : public QGraphicsPathItem {
	private:
		QList<QRectF> path_brects;

		Qt::Alignment text_align;

		QString text;

		QFont font;

	public:
		static constexpr double LayerPadding = 10.0;

		LayerPathItem(QGraphicsItem *parent = nullptr);

		void setPath(const QPainterPath &path);

		void setTextAlignment(Qt::Alignment align);

		void setText(const QString &txt);

		void setFont(const QFont &fnt);

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif
