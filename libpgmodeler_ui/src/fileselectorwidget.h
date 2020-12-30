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
\class FileSelectorWidget
\brief Implements a simple file/directory picker with customizable behavior.
*/

#ifndef FILE_SELECTOR_WIDGET_H
#define FILE_SELECTOR_WIDGET_H

#include <QtWidgets>
#include "ui_fileselectorwidget.h"

class FileSelectorWidget: public QWidget, public Ui::FileSelectorWidget {
	private:
		Q_OBJECT

		QString default_ext;

		QFileDialog file_dlg;

		QLabel *warn_ico_lbl;

		bool allow_filename_input, read_only;

		void showWarning();

	protected:
		bool eventFilter(QObject *obj, QEvent *evnt) override;
		void resizeEvent(QResizeEvent *event) override;

	public:
		FileSelectorWidget(QWidget * parent = nullptr);

		//! \brief Indicate if the selector allows manual filename input
		void setAllowFilenameInput(bool allow_fl_input);

		/*! \brief Configures the file selection mode. This one just configures the internal
		 * QFileDialog file mode (see QFileDialog::setFileMode). Multiple file selection trough
		 * QFileDialog::ExistingFiles is not allowed and will be overriden by QFileDialog::ExistingFile if provided. */
		void setFileMode(QFileDialog::FileMode file_mode);

		/*! \brief Indicate if the accept mode that the selector uses, this is used to configure
		 * the internal QFileDialog instance (see QFileDialog::setAcceptMode) */
		void setAcceptMode(QFileDialog::AcceptMode accept_mode);

		//! \brief Configures the name filters of the internal QFileDialog (see QFileDialog::setNameFilters)
		void setNameFilters(const QStringList &filters);

		//! \brief Configures the window title of the file dialog instance
		void setFileDialogTitle(const QString &title);

		//! \brief Configures the currently selected file in the selector
		void setSelectedFile(const QString &file);

		//! \brief Configures the mime filters of the internal QFileDialog (see QFileDialog::setMimeTypeFilters)
		void setMimeTypeFilters(const QStringList &filters);

		//! \brief Defined the default suffix (extension) of the selected file if not provided
		void setDefaultSuffix(const QString &suffix);

		//! \brief Returns if the selector is in warning state (the warning icon is visible due to any inconsistency)
		bool hasWarning();

		//! \brief Returns the selected file/directory
		QString getSelectedFile();

		//! \brief Sets a custom warning message to the selector
		void setCustomWarning(const QString &warn_msg);

		//! \brief Clears the custom warning message
		void clearCustomWarning(void);

		/*! \brief Configures the selector as readonly. This will cause
		 * the clear input button to be hidden, the input field to be set as readonly and
		 * the action of the file browse button will be to open the file/directory externally */
		void setReadOnly(bool value);

		void setToolTip(const QString &tooltip);

	public slots:
		void clearSelector();
		void validateSelectedFile();
		void openFileDialog();
		void openFileExternally();

	signals:
		//! \brief Signal emitted when the user selects an file/directory
		void s_fileSelected(QString);

		//! \brief Signal emitted when the user clears the selector
		void s_selectorCleared();

		/*! \brief Signal emitted when the user clears the selector or selects a file/directory
		 * The boolean param indicates if there's an file/directory selected or not */
		void s_selectorChanged(bool);
};

#endif
