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

#include "linenumberswidget.h"
#include <QPainter>
#include <QPaintEvent>

QColor LineNumbersWidget::font_color=Qt::lightGray;
QColor LineNumbersWidget::bg_color=Qt::black;

LineNumbersWidget::LineNumbersWidget(QPlainTextEdit * parent) : QWidget(parent)
{
  first_line=line_count=0;
  dy=0;
}

void LineNumbersWidget::drawLineNumbers(unsigned first_line, unsigned line_count, int dy)
{
  bool update=(first_line!=this->first_line || line_count != this->line_count);

  if(update)
  {
    this->first_line=first_line;
    this->line_count=line_count;
    this->dy=dy;
    this->update();
  }
}

void LineNumbersWidget::setColors(const QColor &font_color, const QColor &bg_color)
{
  LineNumbersWidget::font_color=font_color;
  LineNumbersWidget::bg_color=bg_color;
}

void LineNumbersWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  int y=dy, height=fontMetrics().height();
  unsigned last_line=first_line + line_count;

  painter.fillRect(event->rect(), bg_color);
  painter.setPen(font_color);

  for(unsigned lin=first_line; lin < last_line; lin++)
  {
    painter.drawText(0, y, this->width(), fontMetrics().height(),
                     Qt::AlignHCenter, QString::number(lin));
    y+=height;
  }
}


