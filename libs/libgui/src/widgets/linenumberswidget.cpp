/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include <QTextBlock>
#include "exception.h"

QColor LineNumbersWidget::font_color { Qt::lightGray };
QColor LineNumbersWidget::bg_color { Qt::black };

LineNumbersWidget::LineNumbersWidget(QPlainTextEdit * parent) : QWidget(parent)
{
	if(!parent)
		throw Exception(ErrorCode::AsgNotAllocattedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	parent_edt = qobject_cast<QPlainTextEdit *>(parent);
	first_line = line_count=start_sel_pos=0;
	dy = block_height = 0;
	has_selection = false;
	start_sel_line = -1;

	connect(parent_edt, &QPlainTextEdit::selectionChanged, this, qOverload<>(&LineNumbersWidget::update));
	connect(parent_edt, &QPlainTextEdit::cursorPositionChanged, this, qOverload<>(&LineNumbersWidget::update));
}

void LineNumbersWidget::drawLineNumbers(int first_line, int line_count, int dy, int blk_height)
{
	this->first_line=first_line;
	this->line_count=line_count;
	this->dy = dy;
	this->block_height = blk_height;
	this->update();
}

void LineNumbersWidget::setColors(const QColor &font_color, const QColor &bg_color)
{
	LineNumbersWidget::font_color=font_color;
	LineNumbersWidget::bg_color=bg_color;
}

void LineNumbersWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int y = dy,	last_line=first_line + line_count;
	QFont font = painter.font();
	QTextCursor cursor = parent_edt->textCursor();

	//Repaint the widget to clear previous drawn numbers
	painter.fillRect(event->rect(), bg_color);
	painter.setPen(font_color);

	QTextCursor aux_cur;
	QTextBlock block;
	int blk_num = 0, prev_blk_num = -1, padding = (line_count == 1 ? -3 : 1);
	QString lin_str;

	//Draw line numbers
	for(int lin = first_line; lin < last_line; lin++)
	{
		aux_cur = parent_edt->cursorForPosition(QPoint(0, y));
		block = aux_cur.block();

		blk_num = block.blockNumber();

		if(blk_num != prev_blk_num)
		{
			lin_str = QString::number(blk_num + 1);
			prev_blk_num = blk_num;
		}
		else
			lin_str = "↪";

		if(cursor.blockNumber() == aux_cur.blockNumber() ||
			 (cursor.hasSelection() &&
				aux_cur.position() >= cursor.selectionStart() &&
				aux_cur.position() <= cursor.selectionEnd()))
			font.setBold(true);
		else
			font.setBold(false);

		painter.setFont(font);

		if(font.bold())
		{
			painter.setBrush(bg_color.darker(150));
			painter.setPen(Qt::transparent);
			painter.drawRect(QRect(-1, y - 1, this->width() + 1, block_height + padding));
			painter.setPen(font_color.lighter(180));
		}
		else
			painter.setPen(font_color);

		painter.drawText(0, y, this->width(), block_height, Qt::AlignHCenter, lin_str);
		y += block_height;
	}
}

void LineNumbersWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton && !has_selection)
	{
		QTextCursor evnt_cursor = parent_edt->cursorForPosition(QPoint(0, event->pos().y()));

		has_selection = true;
		evnt_cursor.movePosition(QTextCursor::EndOfLine);
		evnt_cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
		parent_edt->setTextCursor(evnt_cursor);
		start_sel_line = evnt_cursor.blockNumber();
		start_sel_pos = evnt_cursor.position();
	}
}

void LineNumbersWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton && has_selection)
	{
		QTextCursor evnt_cursor = parent_edt->cursorForPosition(QPoint(0, event->pos().y())),
				curr_cursor = parent_edt->textCursor();

		//If the user wants to select the lines below the first
		if(evnt_cursor.blockNumber() > start_sel_line)
		{
			curr_cursor.setPosition(start_sel_pos);
			evnt_cursor.movePosition(QTextCursor::EndOfLine);
			curr_cursor.setPosition(evnt_cursor.position(), QTextCursor::KeepAnchor);
			parent_edt->setTextCursor(curr_cursor);
		}
		//If the user wants to select the lines above the first
		else if(evnt_cursor.blockNumber() < start_sel_line)
		{
			curr_cursor.setPosition(start_sel_pos);
			curr_cursor.movePosition(QTextCursor::EndOfLine);
			evnt_cursor.movePosition(QTextCursor::StartOfLine);
			curr_cursor.setPosition(evnt_cursor.position(), QTextCursor::KeepAnchor);
			parent_edt->setTextCursor(curr_cursor);
		}
		else
		{
			evnt_cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
			parent_edt->setTextCursor(evnt_cursor);
		}

		this->update();
	}
}

void LineNumbersWidget::mouseReleaseEvent(QMouseEvent *)
{
	has_selection = false;
}

QColor LineNumbersWidget::getBackgroundColor()
{
	return bg_color;
}

QColor LineNumbersWidget::getFontColor()
{
	return font_color;
}
