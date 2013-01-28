/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Textbox
# Description: This class is used to represent informative textboxes over the
#              model also used to represent relationship labels.
# Creation date: 05/09/2007
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
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "basegraphicobject.h"
#include <QColor>

class Textbox: public BaseGraphicObject{
 private:
  /// Stores the status of text attributes (bold / italic / underline)
  bool text_attributes[3];

  /// Color used to draw the text
  QColor text_color;

  /** Since textboxes doesn't has SQL code definition the base class method that
     generates SQL or XML code is hidden. In replacement the method
     getCodeDefinition(void) must be used in order to get the XML code definition
     of this type of object */
  QString getCodeDefinition(unsigned){ return(""); }

 public:
  /// Constants used to configure the text attributes
  static const unsigned ITALIC_TXT=0,
                        BOLD_TXT=1,
                        UNDERLINE_TXT=2;

  /** To define the content of the textboxes
     the method setComment() must be used and the getComment()
     method used to get the current text */
  Textbox(void);

  /// Sets the attributes of the text
  void setTextAttribute(unsigned attrib, bool value);

  /// Sets the color used to draw the text of textbox
  void setTextColor(const QColor &color);

  /// Returns the XML code definition of the textbox
  QString getCodeDefinition(void);

  /// Returns the current state of the passed text attribute
  bool getTextAttribute(unsigned attrib);

  QColor getTextColor(void);

  /// Copies the attributes between textboxes
  void operator = (Textbox &txtbox);
};

#endif
