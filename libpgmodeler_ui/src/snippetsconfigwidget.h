/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class SnippetsConfigWidget
\brief Implements the operations to manage sql snippets configuration.
*/

#ifndef SNIPPETS_CONFIG_WIDGET_H
#define SNIPPETS_CONFIG_WIDGET_H

#include "ui_snippetsconfigwidget.h"
#include "baseconfigwidget.h"

class SnippetsConfigWidget: public QWidget, public Ui::SnippetsConfigWidget, public BaseConfigWidget {
	private:
		Q_OBJECT

	public:
    SnippetsConfigWidget(QWidget * parent=0);
    ~SnippetsConfigWidget(void);

		//! \brief Saves the configuration to file
		void saveConfiguration(void);

		//! \brief Loads the configuration from file
		void loadConfiguration(void);

	private slots:
		/*! \brief Disabled method */
		void applyConfiguration(void){}

	public slots:
		void restoreDefaults(void);
};

#endif
