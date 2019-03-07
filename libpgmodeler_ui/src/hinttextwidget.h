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

/**
\ingroup libpgmodeler_ui
\class HintTextWidget
\brief Styled "what's this" widget. Since QWhatsThis can't be handled with style sheets this widget comes
	   as a substitute to it.
*/

#ifndef HINT_TEXT_WIDGET_H
#define HINT_TEXT_WIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include "ui_hinttextwidget.h"
#include "exception.h"

class HintTextWidget: public QWidget, public Ui::HintTextWidget {
	private:
		Q_OBJECT

		//! \brief Tool button installed on the parent in order to trigger the hint popup
		QToolButton *hint_tb;

		bool eventFilter(QObject *object, QEvent *event);

		void showEvent(QShowEvent *);

	public:
		static constexpr unsigned SmallIcon=16,
		MediumIcon=24,
		LargeIcon=32;

		HintTextWidget(QWidget *btn_parent, QWidget *parent);

	public slots:
		void setVisible(bool value);
		void setWidgetPosition(void);
		void setText(const QString &text);
		void setIconSize(unsigned icon_sz);
		QString getText(void);
};

#endif
