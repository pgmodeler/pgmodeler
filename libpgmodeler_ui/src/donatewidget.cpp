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

#include "donatewidget.h"
#include "globalattributes.h"
#include <QDesktopServices>
#include <QUrl>
#include "pgmodeleruins.h"

DonateWidget::DonateWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);

	QGraphicsDropShadowEffect * drop_shadow=new QGraphicsDropShadowEffect(this);
	drop_shadow->setOffset(5,5);
	drop_shadow->setBlurRadius(30);
	this->setGraphicsEffect(drop_shadow);

	connect(hide_tb, &QToolButton::clicked,
			[&](){
		this->close();
		emit s_visibilityChanged(false);
	});

	connect(donate_tb, &QToolButton::clicked,
			[&](){
		QDesktopServices::openUrl(QUrl(GlobalAttributes::PgModelerDonateURL));
		this->close();
		emit s_visibilityChanged(false);
	});

	PgModelerUiNs::configureWidgetFont(title_lbl, PgModelerUiNs::BigFontFactor);
	this->adjustSize();
}
