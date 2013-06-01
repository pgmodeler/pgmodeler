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
\ingroup libpgmodeler
\class Textbox
\brief This class is used to represent informative textboxes over the
model also used to represent relationship labels.
\note <strong>Creation date:</strong> 05/09/2007
*/

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "basegraphicobject.h"
#include <QColor>

class Textbox: public BaseGraphicObject{
	private:
		//! \brief Stores the status of text attributes (bold / italic / underline)
		bool text_attributes[3];

		//! \brief Color used to draw the text
		QColor text_color;

	public:
		//! \brief Constants used to configure the text attributes
		static constexpr unsigned ITALIC_TXT=0,
															BOLD_TXT=1,
															UNDERLINE_TXT=2;

		/*! \brief To define the content of the textboxes
		 the method setComment() must be used and the getComment()
		 method used to get the current text */
		Textbox(void);

		//! \brief Sets the attributes of the text
		void setTextAttribute(unsigned attrib, bool value);

		//! \brief Sets the color used to draw the text of textbox
		void setTextColor(const QColor &color);

		/*! \brief Since textboxes doesn't has SQL code definition this method will return a empty
		definition whenever the user try to generate a SQL for this object. */
		virtual QString getCodeDefinition(unsigned def_type) final;

		//! \brief Returns the current state of the passed text attribute
		bool getTextAttribute(unsigned attrib);

		QColor getTextColor(void);

		//! \brief Copies the attributes between textboxes
		void operator = (Textbox &txtbox);
};

#endif
