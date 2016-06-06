/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "aboutwidget.h"
#include "pgmodeleruins.h"

AboutWidget::AboutWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	QGraphicsDropShadowEffect * drop_shadow=new QGraphicsDropShadowEffect(this);
	drop_shadow->setOffset(5,5);
	drop_shadow->setBlurRadius(30);
	this->setGraphicsEffect(drop_shadow);

	pgmodeler_ver_lbl->setText(QString("v%1").arg(GlobalAttributes::PGMODELER_VERSION));
	code_name_lbl->setText(QString(" « %1 »").arg(GlobalAttributes::PGMODELER_VER_CODENAME));
	build_num_lbl->setText(GlobalAttributes::PGMODELER_BUILD_NUMBER);

	for(int row=0; row < contributors_tab->rowCount(); row++)
		contributors_tab->item(row, 2)->setToolTip(contributors_tab->item(row, 2)->text().replace(QString("; "), QString(";\n")));

	contributors_tab->sortByColumn(0, Qt::AscendingOrder);
	contributors_tab->resizeColumnsToContents();

	connect(hide_tb, &QToolButton::clicked, this,
			[=](){
		this->close();
		emit s_visibilityChanged(false);
	});

	PgModelerUiNS::configureWidgetFont(title_lbl, PgModelerUiNS::HUGE_FONT_FACTOR);
	PgModelerUiNS::configureWidgetFont(slogan_lbl, PgModelerUiNS::BIG_FONT_FACTOR);
	PgModelerUiNS::configureWidgetFont(pgmodeler_ver_lbl, PgModelerUiNS::HUGE_FONT_FACTOR);
	PgModelerUiNS::configureWidgetFont(code_name_lbl, PgModelerUiNS::BIG_FONT_FACTOR);
	PgModelerUiNS::configureWidgetFont(build_lbl, PgModelerUiNS::MEDIUM_FONT_FACTOR);
	PgModelerUiNS::configureWidgetFont(build_num_lbl, PgModelerUiNS::MEDIUM_FONT_FACTOR);
	this->adjustSize();
}
