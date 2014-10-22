/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class RoundedRectItem
\brief Represents a rounded rectangle item
*/

#ifndef ROUNDED_RECT_ITEM_H
#define ROUNDED_RECT_ITEM_H

#include <QGraphicsRectItem>
#include <QPainter>

class RoundedRectItem: public QGraphicsRectItem {
  private:
    float radius;

    QPolygonF polygon;

    unsigned corners;

    void createPolygon(void);

    QVector<QPointF> calculatePoints(QPointF start_pnt, float start_angle, float end_angle);

  public:
    //! brief Implies a normal rectangle (no rounded corner)
    static const unsigned NONE_CORNERS=0,

    //! brief The top-left corner will be rounded
    TOPLEFT_CORNER=2,

    //! brief The top-right corner will be rounded
    TOPRIGHT_CORNER=4,

    //! brief The Bottom-left corner will be rounded
    BOTTOMLEFT_CORNER=8,

    //! brief The Bottom-right corner will be rounded
    BOTTOMRIGHT_CORNER=16,

    //! brief All corners will be rounded
    ALL_CORNERS=32;

    explicit RoundedRectItem(QGraphicsItem *parent = 0);

    void setBorderRadius(float radius);
    float getBorderRadius(void);

    void setRoundedCorners(unsigned corners);
    unsigned getRoundedCorners(void);
    bool isCornerRounded(unsigned corner);

    void setRect(const QRectF &rect);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};

#endif

