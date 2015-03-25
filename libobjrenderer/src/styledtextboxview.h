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

/**
\ingroup libobjrenderer
\class StyledTextboxView
\brief This class is only a styled representation of the TextboxView class
*/

#ifndef STYLED_TEXTBOX_VIEW_H
#define STYLED_TEXTBOX_VIEW_H

#include "textboxview.h"

class LIBOBJECTRENDERER_EXPORT StyledTextboxView: public TextboxView {
  private:
    Q_OBJECT

    //! brief Fold indicator appended at bottom-right corner of the object
    QGraphicsPolygonItem *fold;

  public:
    StyledTextboxView(Textbox *txtbox, bool override_style=false);
    ~StyledTextboxView(void);

  protected slots:
    void configureObject(void);
};

#endif
