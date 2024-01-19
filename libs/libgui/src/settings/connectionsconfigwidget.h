/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\ingroup libgui
\class ConnectionsConfigWidget
\brief Implements the operations to manage database connections.
*/

#ifndef CONNECTIONS_CONFIG_WIDGET_H
#define CONNECTIONS_CONFIG_WIDGET_H

#include "ui_connectionsconfigwidget.h"
#include "baseconfigwidget.h"
#include "connection.h"

class __libgui ConnectionsConfigWidget: public BaseConfigWidget, public Ui::ConnectionsConfigWidget {
	private:
		Q_OBJECT
		
		static const QString DefaultFor;
		
		//! \brief Stores the connections created by the user
		static std::vector<Connection *> connections;
		
		/*! \brief Stores the connections attributes. This map is used to write the connections.conf file
		as well to create the connections stored by the 'connections' vector */
		static std::map<QString, attribs_map> config_params;
		
		//! \brief Configures the passed connection setting it's attributes using the values from the form
		void configureConnection(Connection *conn, bool is_update);

		virtual void hideEvent(QHideEvent *) override;
		virtual void showEvent(QShowEvent *) override;
		void updateConnectionsCombo();
		
	public:
		ConnectionsConfigWidget(QWidget * parent = nullptr);
		virtual ~ConnectionsConfigWidget();
		
		virtual void saveConfiguration() override;

		virtual void loadConfiguration() override;
		
		static std::map<QString, attribs_map> getConfigurationParams();
		
		//! \brief Fills the passed map with all the loaded connections.
		static void getConnections(std::map<QString, Connection *> &conns, bool inc_hosts=true);

		//! \brief Return a connection with the provided ID. If no connection is found the method returns nullptr
		static Connection *getConnection(const QString &conn_id);
		
		//! \brief Fills the passed combobox with all the loaded connections
		static void fillConnectionsComboBox(QComboBox *combo, bool incl_placeholder, Connection::ConnOperation check_def_for=Connection::OpNone);
		
		/*! \brief Opens a local instance of connection config dialog to permit user configures connections on-the-fly
		 *  Returns true when the connection were changed somehow, either by restoring the defaults, adding new connections
		 *  or removing current ones */
		static bool openConnectionsConfiguration(QComboBox *combo, bool incl_placeholder);

		//! \brief Returns the first connection found which is defined as the default for the specified operation
		static Connection *getDefaultConnection(Connection::ConnOperation operation);
		
	protected:
		void destroyConnections();
		
	public slots:
		virtual void restoreDefaults() override;
		
	private slots:
		void newConnection();
		void duplicateConnection();
		void handleConnection();
		void editConnection();
		void testConnection();
		void removeConnection();
		void enableCertificates();
		void enableConnectionTest();
		virtual void applyConfiguration(void) override {}
		
		friend class ConfigurationForm;
};

#endif
