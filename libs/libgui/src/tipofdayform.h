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

#ifndef TIPOFDAY_WIDGET_H
#define TIPOFDAY_WIDGET_H

#include <QWidget>
#include "ui_tipofdayform.h"
#include <QNetworkAccessManager>

class TipOfDayForm : public QWidget, public Ui::TipOfDayForm {
	Q_OBJECT

	private:
		QNetworkAccessManager *net_manager;

		bool load_tip;

	protected:
		void showEvent(QShowEvent *event) override;

	public:
		explicit TipOfDayForm(QWidget *parent = nullptr);
		~TipOfDayForm() = default;

	private slots:
		void loadIndex(QNetworkReply *reply);
		void loadTipOfDay(QNetworkReply *reply);
		void handleItemSelected(QTreeWidgetItem *item);
};

#endif
