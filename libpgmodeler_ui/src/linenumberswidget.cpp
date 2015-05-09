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

LineNumbersWidget::LineNumbersWidget(QWidget * parent) : QWidget(parent)
{
  first_line=line_count=0;
}

LineNumbersWidget::~LineNumbersWidget(void)
{

}

void LineNumbersWidget::drawLineNumbers(unsigned first_line, unsigned line_count)
{
  //Validate line numbers!!!
  this->first_line=first_line;
  this->line_count=line_count;
  this->update();
}

void LineNumbersWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  int y=fontMetrics().height(), dy=fontMetrics().height(), x=0;
  unsigned last_line=first_line + line_count;
  QString str_number;

  painter.fillRect(event->rect(), Qt::lightGray);

  for(unsigned lin=first_line; lin < last_line; lin++)
  {
    str_number=QString::number(lin);
    x=(this->width()/2) - (fontMetrics().width(str_number)/2);

    painter.drawText(x, y, str_number);
    y+=dy;
  }
}
