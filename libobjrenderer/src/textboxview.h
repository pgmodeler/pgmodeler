/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\class TextboxView
\brief Represents the textbox in a graphical way on the object scene
*/

#ifndef TEXTBOX_VIEW_H
#define TEXTBOX_VIEW_H

#include "textbox.h"
#include "baseobjectview.h"

class TextboxView: public BaseObjectView {
	private:
		Q_OBJECT

		//! @details Graphical item that represent the box
		QGraphicsPolygonItem *box;

		//! @details Graphical item that represent the text
		QGraphicsSimpleTextItem *text;

		//! @details Indicates the the font / color styles will be overriden (need to call setColorStyle, setFontStyle)
		bool override_style;

	public:
		TextboxView(Textbox *txtbox, bool override_style=false);

		/*! @details Sets the fill and border color for the text box. This method has effect only when
		 the style can be overriden (via constructor) */
		void setColorStyle(const QBrush &fill_style, const QPen &border_style);

		/*! @details Sets the font style for the text box. This method has effect only when
		 the style can be overriden (via constructor) */
		void setFontStyle(const QTextCharFormat &fmt);

		~TextboxView(void);

	protected slots:
		void configureObject(void);
};

#endif
