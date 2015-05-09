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

#include "numberedtexteditor.h"
#include <QTextBlock>
#include <QTextStream>
#include <QScrollBar>

NumberedTextEditor::NumberedTextEditor(QWidget * parent) : QPlainTextEdit(parent)
{
  line_number_wgt=new LineNumbersWidget(this);

  setWordWrapMode(QTextOption::NoWrap);
  updateLineNumbersWidth();
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumbers(QRect,int)));
}

NumberedTextEditor::~NumberedTextEditor(void)
{

}

void NumberedTextEditor::setFont(const QFont &font)
{
  QWidget::setFont(font);
  line_number_wgt->setFont(font);
}

void NumberedTextEditor::updateLineNumbers(QRect rect, int dy)
{
  QTextBlock block = firstVisibleBlock();
  int block_number = block.blockNumber(),
      top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top()),
      bottom = top +  static_cast<int>(blockBoundingRect(block).height());

  unsigned first_line=0, line_count=0;

  updateLineNumbersWidth();

  while(block.isValid())
  {
    if(block.isVisible())
    {
      line_count++;
      if(first_line==0)
        first_line=static_cast<unsigned>(block_number + 1);
    }

    block = block.next();
    top = bottom;
    bottom = top + static_cast<int>(blockBoundingRect(block).height());
    ++block_number;
  }

  line_number_wgt->drawLineNumbers(first_line, line_count);
}

void NumberedTextEditor::updateLineNumbersWidth(void)
{
  QRect rect=contentsRect();

  setViewportMargins(getLineNumbersWidth(), 0, 0, 0);
  line_number_wgt->setGeometry(QRect(rect.left(), rect.top(),
                                    getLineNumbersWidth(), rect.height()));
}

int NumberedTextEditor::getLineNumbersWidth(void)
{
  int digits = 1, max = qMax(1, document()->lineCount());

  while(max >= 10)
  {
    max /= 10;
    ++digits;
  }

  return(10 + fontMetrics().width(QChar('9')) * digits);
}
