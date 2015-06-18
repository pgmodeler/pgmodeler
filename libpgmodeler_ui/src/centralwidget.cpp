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

#include "centralwidget.h"
#include "QGraphicsDropShadowEffect"
#include "QTextStream"

CentralWidget::CentralWidget(QWidget *parent): QWidget(parent)
{
	setupUi(this);

	QList<QToolButton *> btns= { new_tb, open_tb, recent_tb, last_session_tb, sample_tb };
	QGraphicsDropShadowEffect *shadow=nullptr;

  for(auto &btn : btns)
	{
		shadow=new QGraphicsDropShadowEffect(this);
		shadow->setXOffset(3);
		shadow->setYOffset(3);
		shadow->setBlurRadius(10);
		btn->setGraphicsEffect(shadow);
	}
}
