/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libpgmodeler_ui
\class LineNumbersWidget
\brief Implements a simple line number widget that expose the current highlighted line in a text editor
*/

#ifndef LINE_NUMBERS_WIDGET_H
#define LINE_NUMBERS_WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>

class LineNumbersWidget : public QWidget {
	private:
		Q_OBJECT

		QPlainTextEdit *parent_edt;

		bool has_selection;

		//! \brief The first line number that must be drawn
		unsigned first_line,

		//! \brief The amount of lines to be drawn
		line_count;

		//! \brief The y axis increment to start drawn the line number
		int dy;

		int	start_sel_line, start_sel_pos;

		//! \brief Font color for drawn line numbers
		static QColor font_color,

		//! \brief Widget's background color
		bg_color;

	protected:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *);

	public:
		explicit LineNumbersWidget(QPlainTextEdit *parent);

		/*! \brief Draw the lines starting from 'first_line' and stoping at fisrt_line + line_count -1.
		The dy param. defines an initial Y translation before drawn lines */
		void drawLineNumbers(unsigned first_line, unsigned line_count, int dy);

		//! \brief Configures the lines for both font and widget's background
		static void setColors(const QColor &font_color, const QColor &bg_color);

		static QColor getBackgroundColor();
};

#endif
