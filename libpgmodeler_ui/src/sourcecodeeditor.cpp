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

#include "sourcecodeeditor.h"
#include <QTextBlock>

SourceCodeEditor::SourceCodeEditor(QWidget * parent) : QTextEdit(parent)
{
  line_number_wgt=new LineNumberWidget(this);

  setWordWrapMode(QTextOption::NoWrap);
  updateLineNumbersWidth();
  connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumbers()));
}

SourceCodeEditor::~SourceCodeEditor(void)
{

}

void SourceCodeEditor::updateLineNumbers(void)
{
  QTextCursor cursor = textCursor();

  cursor.movePosition(QTextCursor::StartOfBlock);
  qDebug(QString::number(cursor.blockNumber()).toStdString().c_str());
  qDebug(QString::number(cursor.block().firstLineNumber()).toStdString().c_str());

  /*cursor = textCursor();
  cursor.movePosition(QTextCursor::EndOfBlock);
  qDebug(QString::number(cursor.blockNumber()).toStdString().c_str());*/

}

void SourceCodeEditor::updateLineNumbersWidth(void)
{
  QRect rect=contentsRect();

  setViewportMargins(getLineNumbersWidth(), 0, 0, 0);
  line_number_wgt->setGeometry(QRect(rect.left(), rect.top(),
                                    getLineNumbersWidth(), rect.height()));
}

int SourceCodeEditor::getLineNumbersWidth(void)
{
  int digits = 1, max = qMax(1, document()->lineCount());

  while(max >= 10)
  {
    max /= 10;
    ++digits;
  }

  return(5 + fontMetrics().width(QLatin1Char('9')) * digits);
}

void SourceCodeEditor::resizeEvent(QResizeEvent *event)
{
  QTextEdit::resizeEvent(event);
  updateLineNumbersWidth();
}
