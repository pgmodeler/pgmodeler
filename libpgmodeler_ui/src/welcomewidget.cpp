/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "welcomewidget.h"
#include "pgmodeleruins.h"

WelcomeWidget::WelcomeWidget(QWidget *parent): QWidget(parent)
{
	setupUi(this);

	QList<QToolButton *> btns= { new_tb, open_tb, recent_tb, last_session_tb, sample_tb, support_tb };

	for(auto &btn : btns)
	{
		PgModelerUiNs::createDropShadow(btn, 3, 3, 10);

#ifdef Q_OS_LINUX
	PgModelerUiNs::__configureWidgetFont(btn, 1.30);
#else
		PgModelerUiNs::__configureWidgetFont(btn, 1.50);
#endif
	}
}
