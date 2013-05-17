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
\ingroup libpgmodeler_ui
\class BaseForm
\brief A basic form implementation, contains only an apply and a cancel button.
*/

#ifndef BASE_FORM_H
#define BASE_FORM_H

#include <QtWidgets>
#include "ui_baseform.h"
#include "exception.h"
#include "messagebox.h"

class BaseForm: public QDialog, public Ui::BaseForm {
	private:
		Q_OBJECT

	public:
		BaseForm(QWidget * parent = 0, Qt::WindowFlags f = 0);
		void setButtonConfiguration(unsigned button_conf=Messagebox::OK_CANCEL_BUTTONS);
};

#endif
