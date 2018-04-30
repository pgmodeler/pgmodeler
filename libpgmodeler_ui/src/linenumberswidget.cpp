/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "exception.h"
#include <QTextBlock>

QColor LineNumbersWidget::font_color=Qt::lightGray;
QColor LineNumbersWidget::bg_color=Qt::black;

LineNumbersWidget::LineNumbersWidget(QPlainTextEdit * parent) : QWidget(parent)
{
	if(!parent)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	parent_edt = qobject_cast<QPlainTextEdit *>(parent);
	first_line=line_count=0;
	dy=0;
	has_selection = false;
	first_sel_line = -1;
	last_sel_line = -1;

	connect(parent_edt, SIGNAL(selectionChanged()), this, SLOT(updateSelectedLineNumbers()));
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
	int y = dy, height = 0;
	unsigned last_line=first_line + line_count;
	QFont font = painter.font();
	unsigned fs_line = std::min(first_sel_line, last_sel_line),
			ls_line = std::max(first_sel_line, last_sel_line);

	//Repaint the widget to clear previous drawn numbers
	painter.fillRect(event->rect(), bg_color);
	painter.setPen(font_color);

	//Draw line numbers
	for(unsigned lin=first_line; lin < last_line; lin++)
	{
		font.setBold(lin-1 >= fs_line && lin-1 <= ls_line);
		height =  QFontMetrics(font).height();
		painter.setFont(font);
		painter.drawText(0, y, this->width(), height, Qt::AlignHCenter, QString::number(lin));
		y+=height;
	}
}

void LineNumbersWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton && !has_selection)
	{
		QTextCursor cursor = parent_edt->cursorForPosition(QPoint(0, event->pos().y()));

		parent_edt->blockSignals(true);
		cursor.select(QTextCursor::LineUnderCursor);
		parent_edt->setTextCursor(cursor);
		has_selection = true;
		first_sel_line = last_sel_line = cursor.blockNumber();
		this->update();
	}
}

void LineNumbersWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton && has_selection)
	{
		QTextCursor cursor = parent_edt->cursorForPosition(QPoint(0, event->pos().y())),
				curr_cursor = parent_edt->textCursor();

		last_sel_line = cursor.blockNumber();

		//If the current selected line is the same as the first one (when the first click ocurred)
		if(first_sel_line == cursor.blockNumber())
		{
			//Forcing the selection of the whole line
			cursor.movePosition(QTextCursor::StartOfLine);
			cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
			parent_edt->setTextCursor(cursor);
		}
		//If the user wants selects lines below the first
		else if(first_sel_line < cursor.blockNumber())
		{
			cursor.movePosition(QTextCursor::EndOfLine);
			curr_cursor.setPosition(cursor.position(), QTextCursor::KeepAnchor);
			parent_edt->setTextCursor(curr_cursor);
		}
		//If the user wants selects lines above the first
		else
		{
			int pos = curr_cursor.position();

			//We first move the cursor related to the mouse position to the start of the line
			cursor.movePosition(QTextCursor::StartOfLine);
			//Then we move it to the position of the current cursor (where the click occured)
			cursor.setPosition(pos, QTextCursor::KeepAnchor);
			parent_edt->setTextCursor(cursor);
		}

		this->update();
	}
}

void LineNumbersWidget::mouseReleaseEvent(QMouseEvent *)
{
	has_selection = false;
	parent_edt->blockSignals(false);
}

QColor LineNumbersWidget::getBackgroundColor(void)
{
	return(LineNumbersWidget::bg_color);
}

void LineNumbersWidget::updateSelectedLineNumbers(void)
{
	QTextCursor cursor = parent_edt->textCursor();

	if(cursor.hasSelection())
	{
		QTextCursor start = cursor,
				end = cursor;

		start.setPosition(cursor.selectionStart());
		first_sel_line = start.blockNumber();
		end.setPosition(cursor.selectionEnd());
		last_sel_line = end.blockNumber();
	}
	else
		first_sel_line = last_sel_line = -1;

	this->update();
}
