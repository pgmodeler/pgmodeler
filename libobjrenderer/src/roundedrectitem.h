/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

class LIBOBJECTRENDERER_EXPORT RoundedRectItem: public QGraphicsRectItem {
  private:
    //! brief Rect border radius
    float radius;

    //! brief Polygon used to simulate the rounded rectangle with custom rounded corners
    QPolygonF polygon;

    //! brief Stores which corners of the rectangle must be rounded
    unsigned corners;

    //! brief Creates the polygon that represents the rounded rectangle
    void createPolygon(void);

    /*! brief Calculates the points to simulate the rounded border. It must be informed
        the starting point and start/end angles. Based upon those parameters a line
        is configured and rotated from start_angle to end_angle and its final point
        (the rounded border point) is detected. The method returns all the calculated
        points which forms the rounded border. */
    QVector<QPointF> calculatePoints(QPointF start_pnt, float start_angle, float end_angle);

  public:
    //! brief Implies a normal rectangle (no rounded corner)
    static const unsigned NONE_CORNERS=0;

    //! brief The top-left corner will be rounded
    static const unsigned TOPLEFT_CORNER=2;

    //! brief The top-right corner will be rounded
    static const unsigned TOPRIGHT_CORNER=4;

    //! brief The Bottom-left corner will be rounded
    static const unsigned BOTTOMLEFT_CORNER=8;

    //! brief The Bottom-right corner will be rounded
    static const unsigned BOTTOMRIGHT_CORNER=16;

    //! brief All corners will be rounded
    static const unsigned ALL_CORNERS=32;

    explicit RoundedRectItem(QGraphicsItem *parent = 0);

    void setBorderRadius(float radius);
    float getBorderRadius(void);

    void setRoundedCorners(unsigned corners);
    unsigned getRoundedCorners(void);

    //! brief Test if a corners is configured to be rounded
    bool isCornerRounded(unsigned corner);

    void setRect(const QRectF &rect);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};

#endif

