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
\class ConnectionsConfigWidget
\brief Implements the operations to manage database connections.
*/

#ifndef CONNECTIONS_CONFIG_WIDGET_H
#define CONNECTIONS_CONFIG_WIDGET_H

#include "ui_connectionsconfigwidget.h"
#include "baseconfigwidget.h"
#include "connection.h"
#include "messagebox.h"

class ConnectionsConfigWidget: public BaseConfigWidget, public Ui::ConnectionsConfigWidget {
	private:
		Q_OBJECT
		
		static const QString DefaultFor;
		
		//! \brief Stores the connections created by the user
		static vector<Connection *> connections;
		
		/*! \brief Stores the connections attributes. This map is used to write the connections.conf file
		as well to create the connections stored by the 'connections' vector */
		static map<QString, attribs_map> config_params;
		
		//! \brief Configures the passed connection setting it's attributes using the values from the form
		void configureConnection(Connection *conn);

		void hideEvent(QHideEvent *);
		void showEvent(QShowEvent *);
		void updateConnectionsCombo();
		
	public:
		ConnectionsConfigWidget(QWidget * parent = nullptr);
		virtual ~ConnectionsConfigWidget();
		
		void saveConfiguration();
		void loadConfiguration();
		
		static map<QString, attribs_map> getConfigurationParams();
		
		//! \brief Fills the passed map with all the loaded connections.
		static void getConnections(map<QString, Connection *> &conns, bool inc_hosts=true);

		//! \brief Return a connection with the provided ID. If no connection is found the method returns nullptr
		static Connection *getConnection(const QString &conn_id);
		
		//! \brief Fills the passed combobox with all the loaded connections
		static void fillConnectionsComboBox(QComboBox *combo, bool incl_placeholder, unsigned check_def_for=Connection::OpNone);
		
		//! \brief Opens a local instance of connection config dialog to permit user configures connections on-the-fly
		static bool openConnectionsConfiguration(QComboBox *combo, bool incl_placeholder);

		//! \brief Returns the first connection found which is defined as the default for the specified operation
		static Connection *getDefaultConnection(unsigned operation);
		
	protected:
		void destroyConnections();
		
	public slots:
		void restoreDefaults();
		
	private slots:
		void newConnection();
		void duplicateConnection();
		void handleConnection();
		void editConnection();
		void testConnection();
		void removeConnection();
		void enableCertificates();
		void enableConnectionTest();
		void applyConfiguration(void){}
		
		friend class ConfigurationForm;
};

#endif
