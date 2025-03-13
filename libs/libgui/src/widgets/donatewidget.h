/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2025 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class DonateWidget
\brief Implements a basic dialog to ask for donations shown as a popup widget on main window.
*/

#ifndef DONATE_WIDGET_H
#define DONATE_WIDGET_H

#include "guiglobal.h"
#include "ui_donatewidget.h"
#include <QGraphicsDropShadowEffect>

class __libgui DonateWidget: public QWidget, public Ui::DonateWidget {
	Q_OBJECT

	public:
		DonateWidget(QWidget *parent = nullptr);
		
	signals:
		void s_hideRequested();
};

#endif
