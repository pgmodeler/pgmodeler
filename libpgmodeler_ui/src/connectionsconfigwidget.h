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
\class ConnectionsConfigWidget
\brief Implements the operations to manage database connections.
*/

#ifndef CONNECTIONS_CONFIG_WIDGET_H
#define CONNECTIONS_CONFIG_WIDGET_H

#include "ui_connectionsconfigwidget.h"
#include "baseconfigwidget.h"
#include "dbconnection.h"
#include "messagebox.h"

class ConnectionsConfigWidget: public QWidget, public Ui::ConnectionsConfigWidget, public BaseConfigWidget {
	private:
		Q_OBJECT

		//! \brief Configures the passed connection setting it's attributes using the values from the form
		void configureConnection(DBConnection *conn);

		void hideEvent(QHideEvent *);

	public:
		ConnectionsConfigWidget(QWidget * parent=0);
		~ConnectionsConfigWidget(void);

		void saveConfiguration(void);
		void loadConfiguration(void);

		//! \brief Fills the passed map with all the loaded connections.
		void getConnections(map<QString, DBConnection *> &conns);

	public slots:
		void restoreDefaults(void);

	private slots:
		void newConnection(void);
		void handleConnection(void);
		void editConnection(void);
		void testConnection(void);
		void removeConnection(void);
		void enableCertificates(void);
		void enableConnectionTest(void);
		void applyConfiguration(void){}
};

#endif
