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

/**
\ingroup libpgmodeler_ui
\class MessageBox
\brief Implements the message box to show errors / alerts / infos to the user.
*/

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "ui_messagebox.h"
#include "exception.h"

class Messagebox: public QDialog, public Ui::Messagebox {
	private:
		Q_OBJECT

		bool cancelled;

	public:
		//! \brief Constants used to define the message icon
		static constexpr unsigned NoIcon=10,
		ErrorIcon=11,
		InfoIcon=12,
		AlertIcon=13,
		ConfirmIcon=14;

		//! \brief Constants used to configure the visible buttons
		static constexpr unsigned YesNoButtons=0,
		OkCancelButtons=1,
		OkButton=2,
		AllButtons=3;

		Messagebox(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::Widget);

		/*! \brief Shows the message box defining the icons and available buttons.
	User can specify custom button labels as well custom icons. The icons can be a path to a local file
	or a Qt  resource icon ':/path/icon' and will be enabled only specifing custom labels for the respective button. */
		void show(const QString &title, const QString &msg, unsigned icon_type=NoIcon, unsigned buttons=OkButton,
				  const QString &yes_lbl=QString(), const QString &no_lbl=QString(), const QString &cancel_lbl=QString(),
				  const QString &yes_ico=QString(), const QString &no_ico=QString(), const QString &cancel_ico=QString());

		//! \brief Shows the message box using an excpetion as message
		void show(Exception e, const QString &msg=QString(), unsigned icon_type=ErrorIcon, unsigned buttons=OkButton,
				  const QString &yes_lbl=QString(), const QString &no_lbl=QString(), const QString &cancel_lbl=QString(),
				  const QString &yes_ico=QString(), const QString &no_ico=QString(), const QString &cancel_ico=QString());

		//! \brief Shows a simple message box with the title automatically defined by the icon type
		void show(const QString &msg, unsigned icon_type=NoIcon, unsigned buttons=OkButton);

		bool isCancelled();

		void setCustomOptionText(const QString &text);
		bool isCustomOptionChecked();

	private slots:
		void handleYesOkClick();
		void handleNoCancelClick();
		void showExceptionList();
};

#endif
