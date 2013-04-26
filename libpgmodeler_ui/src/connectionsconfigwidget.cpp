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

#include "connectionsconfigwidget.h"

ConnectionsConfigWidget::ConnectionsConfigWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

	connect(ssl_mode_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableCertificates(void)));

	connect(new_tb, SIGNAL(clicked(bool)), this, SLOT(newConnection(void)));
	connect(cancel_tb, SIGNAL(clicked(bool)), this, SLOT(newConnection(void)));

	connect(test_tb, SIGNAL(clicked(bool)), this, SLOT(testConnection(void)));
	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(handleConnection(void)));
	connect(update_tb, SIGNAL(clicked(bool)), this, SLOT(handleConnection(void)));
	connect(edit_tb, SIGNAL(clicked(bool)), this, SLOT(editConnection(void)));
	connect(remove_tb, SIGNAL(clicked(bool)), this, SLOT(removeConnection(void)));

	connect(alias_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest(void)));
	connect(host_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest(void)));
	connect(user_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest(void)));
	connect(passwd_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest(void)));
	connect(conn_db_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest(void)));

	update_tb->setVisible(false);
	cancel_tb->setVisible(false);
}

ConnectionsConfigWidget::~ConnectionsConfigWidget(void)
{
	//Destroy all the loaded connections
	while(connections_cmb->count() > 0)
		this->removeConnection();
}

void ConnectionsConfigWidget::loadConfiguration(void)
{
	vector<QString> key_attribs;
	map<QString, map<QString, QString> >::iterator itr, itr_end;
	DBConnection *conn=NULL;

	key_attribs.push_back(ParsersAttributes::ALIAS);
	BaseConfigWidget::loadConfiguration(GlobalAttributes::CONNECTIONS_CONF, key_attribs);

	itr=config_params.begin();
	itr_end=config_params.end();

	while(itr!=itr_end)
	{
		conn=new DBConnection;

		conn->setConnectionParam(DBConnection::PARAM_SERVER_FQDN, itr->second[DBConnection::PARAM_SERVER_FQDN]);
		conn->setConnectionParam(DBConnection::PARAM_PORT, itr->second[DBConnection::PARAM_PORT]);
		conn->setConnectionParam(DBConnection::PARAM_USER, itr->second[DBConnection::PARAM_USER]);
		conn->setConnectionParam(DBConnection::PARAM_PASSWORD,itr->second[DBConnection::PARAM_PASSWORD]);
		conn->setConnectionParam(DBConnection::PARAM_DB_NAME, itr->second[DBConnection::PARAM_DB_NAME]);
		conn->setConnectionParam(DBConnection::PARAM_CONN_TIMEOUT, itr->second[DBConnection::PARAM_CONN_TIMEOUT]);
		conn->setConnectionParam(DBConnection::PARAM_SSL_MODE, itr->second[DBConnection::PARAM_SSL_MODE]);
		conn->setConnectionParam(DBConnection::PARAM_SSL_ROOT_CERT, itr->second[DBConnection::PARAM_SSL_ROOT_CERT]);
		conn->setConnectionParam(DBConnection::PARAM_SSL_CERT, itr->second[DBConnection::PARAM_SSL_CERT]);
		conn->setConnectionParam(DBConnection::PARAM_SSL_KEY, itr->second[DBConnection::PARAM_SSL_KEY]);
		conn->setConnectionParam(DBConnection::PARAM_SSL_CRL, itr->second[DBConnection::PARAM_SSL_CRL]);
		conn->setConnectionParam(DBConnection::PARAM_LIB_GSSAPI, itr->second[DBConnection::PARAM_LIB_GSSAPI]);
		conn->setConnectionParam(DBConnection::PARAM_KERBEROS_SERVER, itr->second[DBConnection::PARAM_KERBEROS_SERVER]);
		conn->setConnectionParam(DBConnection::PARAM_OPTIONS, itr->second[DBConnection::PARAM_OPTIONS]);

		connections_cmb->addItem(Utf8String::create(itr->second[ParsersAttributes::ALIAS]),
				QVariant::fromValue<void *>(reinterpret_cast<void *>(conn)));

		itr++;
	}

	edit_tb->setEnabled(connections_cmb->count() > 0);
	remove_tb->setEnabled(connections_cmb->count() > 0);
}

void ConnectionsConfigWidget::enableCertificates(void)
{
	client_cert_lbl->setEnabled(ssl_mode_cmb->currentIndex()!=0);
	client_cert_edt->setEnabled(ssl_mode_cmb->currentIndex()!=0);
	root_cert_lbl->setEnabled(ssl_mode_cmb->currentIndex()!=0);
	root_cert_edt->setEnabled(ssl_mode_cmb->currentIndex()!=0);
	crl_edt->setEnabled(ssl_mode_cmb->currentIndex()!=0);
	crl_lbl->setEnabled(ssl_mode_cmb->currentIndex()!=0);
	client_key_lbl->setEnabled(ssl_mode_cmb->currentIndex()!=0);
	client_key_edt->setEnabled(ssl_mode_cmb->currentIndex()!=0);
}

void ConnectionsConfigWidget::enableConnectionTest(void)
{
	test_tb->setEnabled(!alias_edt->text().isEmpty() &&
											!host_edt->text().isEmpty() &&
											!user_edt->text().isEmpty() &&
											!passwd_edt->text().isEmpty() &&
											!conn_db_edt->text().isEmpty());
	add_tb->setEnabled(test_tb->isEnabled());
	update_tb->setEnabled(test_tb->isEnabled());
}

void ConnectionsConfigWidget::newConnection(void)
{
	conn_db_edt->clear();
	alias_edt->clear();
	user_edt->clear();
	host_edt->clear();
	port_sbp->setValue(5432);
	passwd_edt->clear();
	options_edt->clear();

	ssl_mode_cmb->setCurrentIndex(0);
	client_cert_edt->setText("~/.postgresql/postgresql.crt");
	root_cert_edt->setText("~/.postgresql/root.crt");
	crl_edt->setText("~/.postgresql/root.crl");
	client_key_edt->setText("~/.postgresql/postgresql.key");

	gssapi_auth_chk->setChecked(false);
	krb_server_edt->clear();

	timeout_sbp->setValue(2);
	add_tb->setVisible(true);
	update_tb->setVisible(false);
	connections_cmb->setEnabled(true);

	new_tb->setVisible(true);
	cancel_tb->setVisible(false);

	edit_tb->setEnabled(connections_cmb->count() > 0);
	remove_tb->setEnabled(connections_cmb->count() > 0);
}

void ConnectionsConfigWidget::handleConnection(void)
{
	DBConnection *conn=NULL;
	QString alias;
	unsigned i=1;

	try
	{
		alias=alias_edt->text();
		while(connections_cmb->findText(alias)>=0)
			alias=QString("%1(%2)").arg(alias_edt->text()).arg(i++);

		if(!update_tb->isVisible())
		{
			conn=new DBConnection;
			this->configurarConexao(conn);
			connections_cmb->addItem(alias, QVariant::fromValue<void *>(reinterpret_cast<void *>(conn)));
		}
		else
		{
			conn=reinterpret_cast<DBConnection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
			this->configurarConexao(conn);
			connections_cmb->setItemText(connections_cmb->currentIndex(), alias);
		}

		this->newConnection();
		edit_tb->setEnabled(connections_cmb->count() > 0);
		remove_tb->setEnabled(connections_cmb->count() > 0);
	}
	catch(Exception &e)
	{
		if(add_tb->isVisible())
			delete(conn);

		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::removeConnection(void)
{
	if(connections_cmb->currentIndex() >= 0)
	{
		DBConnection *conexao=NULL;

		conexao=reinterpret_cast<DBConnection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		connections_cmb->removeItem(connections_cmb->currentIndex());
		delete(conexao);
		this->newConnection();
	}
}

void ConnectionsConfigWidget::editConnection(void)
{
	if(connections_cmb->count() > 0)
	{
		DBConnection *conexao=NULL;

		conexao=reinterpret_cast<DBConnection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
		alias_edt->setText(connections_cmb->currentText());

		if(!conexao->getConnectionParam(DBConnection::PARAM_SERVER_FQDN).isEmpty())
			host_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_SERVER_FQDN));
		else
			host_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_SERVER_IP));

		conn_db_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_DB_NAME));
		user_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_USER));
		passwd_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_PASSWORD));
		port_sbp->setValue(conexao->getConnectionParam(DBConnection::PARAM_PORT).toInt());
		timeout_sbp->setValue(conexao->getConnectionParam(DBConnection::PARAM_CONN_TIMEOUT).toInt());

		krb_server_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_KERBEROS_SERVER));
		gssapi_auth_chk->setChecked(conexao->getConnectionParam(DBConnection::PARAM_LIB_GSSAPI)=="gssapi");
		options_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_OPTIONS));

		if(conexao->getConnectionParam(DBConnection::PARAM_SSL_MODE)==DBConnection::SSL_DESABLE)
			ssl_mode_cmb->setCurrentIndex(0);
		else if(conexao->getConnectionParam(DBConnection::PARAM_SSL_MODE)==DBConnection::SSL_ALLOW)
			ssl_mode_cmb->setCurrentIndex(1);
		else if(conexao->getConnectionParam(DBConnection::PARAM_SSL_MODE)==DBConnection::SSL_REQUIRE)
			ssl_mode_cmb->setCurrentIndex(2);
		else if(conexao->getConnectionParam(DBConnection::PARAM_SSL_MODE)==DBConnection::SSL_CA_VERIF)
			ssl_mode_cmb->setCurrentIndex(3);
		else
			ssl_mode_cmb->setCurrentIndex(4);

		if(ssl_mode_cmb->currentIndex() > 0)
		{
			client_cert_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_SSL_CERT));
			root_cert_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_SSL_ROOT_CERT));
			client_key_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_SSL_KEY));
			crl_edt->setText(conexao->getConnectionParam(DBConnection::PARAM_SSL_CRL));
		}

		update_tb->setVisible(true);
		add_tb->setVisible(false);
		connections_cmb->setEnabled(false);

		new_tb->setVisible(false);
		cancel_tb->setVisible(true);
		edit_tb->setEnabled(false);
	}
}

void ConnectionsConfigWidget::configurarConexao(DBConnection *conn)
{
	if(conn)
	{
		conn->setConnectionParam(DBConnection::PARAM_SERVER_FQDN, host_edt->text());
		conn->setConnectionParam(DBConnection::PARAM_PORT, QString("%1").arg(port_sbp->value()));
		conn->setConnectionParam(DBConnection::PARAM_USER, user_edt->text());
		conn->setConnectionParam(DBConnection::PARAM_PASSWORD, passwd_edt->text());
		conn->setConnectionParam(DBConnection::PARAM_DB_NAME, conn_db_edt->text());
		conn->setConnectionParam(DBConnection::PARAM_CONN_TIMEOUT, QString("%1").arg(timeout_sbp->value()));

		switch(ssl_mode_cmb->currentIndex())
		{
			case 1:
				conn->setConnectionParam(DBConnection::PARAM_SSL_MODE, DBConnection::SSL_ALLOW);
			break;
			case 2:
				conn->setConnectionParam(DBConnection::PARAM_SSL_MODE, DBConnection::SSL_REQUIRE);
			break;
			case 3:
				conn->setConnectionParam(DBConnection::PARAM_SSL_MODE, DBConnection::SSL_CA_VERIF);
			break;
			case 4:
				conn->setConnectionParam(DBConnection::PARAM_SSL_MODE, DBConnection::SSL_FULL_VERIF);
			break;
			default:
			case 0:
				conn->setConnectionParam(DBConnection::PARAM_SSL_MODE, DBConnection::SSL_DESABLE);
			break;
		}

		if(ssl_mode_cmb->currentIndex()!=0)
		{
			conn->setConnectionParam(DBConnection::PARAM_SSL_ROOT_CERT, root_cert_edt->text());
			conn->setConnectionParam(DBConnection::PARAM_SSL_CERT, client_cert_edt->text());
			conn->setConnectionParam(DBConnection::PARAM_SSL_KEY, client_key_edt->text());
			conn->setConnectionParam(DBConnection::PARAM_SSL_CRL, crl_edt->text());
		}

		if(gssapi_auth_chk->isChecked())
			conn->setConnectionParam(DBConnection::PARAM_LIB_GSSAPI, "gssapi");

		if(!krb_server_edt->text().isEmpty())
			conn->setConnectionParam(DBConnection::PARAM_KERBEROS_SERVER, krb_server_edt->text());

		if(!options_edt->text().isEmpty())
			conn->setConnectionParam(DBConnection::PARAM_OPTIONS, options_edt->text());
	}
}

void ConnectionsConfigWidget::testConnection(void)
{
	DBConnection conn;
	MessageBox msg_box;

	try
	{
		this->configurarConexao(&conn);
		conn.connect();
		msg_box.show(trUtf8("Success"), trUtf8("Connection successfuly stablished!"), MessageBox::INFO_ICON);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::restoreDefaults(void)
{
	try
	{
		//Restore the default connection config file
		BaseConfigWidget::restoreDefaults(GlobalAttributes::CONNECTIONS_CONF);

		//Remove all connections
		while(connections_cmb->count() > 0)
			this->removeConnection();

		//Reloads the configuration
		this->loadConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::saveConfiguration(void)
{
	try
	{
		int i, count;
		DBConnection *conn=NULL;
		map<QString, QString> attribs;

		config_params[GlobalAttributes::CONNECTIONS_CONF].clear();
		count=connections_cmb->count();

		/* Workaround: When there is no connection, to prevent saving an empty file, is necessary to
		 fill the attribute CONNECTIONS with white spaces */
		if(count==0)
			config_params[GlobalAttributes::CONNECTIONS_CONF][ParsersAttributes::CONNECTIONS]="  ";
		else
		{
			for(i=0; i < count; i++)
			{
				conn=reinterpret_cast<DBConnection *>(connections_cmb->itemData(i).value<void *>());
				attribs=conn->getConnectionParams();

				if(attribs.count(DBConnection::PARAM_SERVER_FQDN)==0)
					attribs[DBConnection::PARAM_SERVER_FQDN]=attribs[DBConnection::PARAM_SERVER_IP];

				attribs[ParsersAttributes::ALIAS]=connections_cmb->itemText(i);

				SchemaParser::setIgnoreUnkownAttributes(true);
				config_params[GlobalAttributes::CONNECTIONS_CONF][ParsersAttributes::CONNECTIONS]+=
						SchemaParser::getCodeDefinition(GlobalAttributes::CONFIGURATIONS_DIR +
																						GlobalAttributes::DIR_SEPARATOR +
																						GlobalAttributes::SCHEMAS_DIR +
																						GlobalAttributes::DIR_SEPARATOR +
																						GlobalAttributes::CONNECTIONS_CONF +
																						GlobalAttributes::SCHEMA_EXT,
																						attribs);
				SchemaParser::setIgnoreUnkownAttributes(false);
			}
		}

		BaseConfigWidget::saveConfiguration(GlobalAttributes::CONNECTIONS_CONF);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::getConnections(map<QString, DBConnection *> &conns, bool inc_hostname)
{
	int i, count;
	QString host;
	DBConnection *conn=NULL;

	conns.clear();
	count=connections_cmb->count();

	for(i=0; i < count; i++)
	{
		conn=reinterpret_cast<DBConnection *>(connections_cmb->itemData(i).value<void *>());

		if(inc_hostname)
		{
			host=conn->getConnectionParam(DBConnection::PARAM_SERVER_FQDN);

			if(host.isEmpty())
				host=conn->getConnectionParam(DBConnection::PARAM_SERVER_IP);

			if(!host.isEmpty())
				host=QString(" (%1)").arg(host);
		}

		conns[connections_cmb->itemText(i) + host]=conn;
	}
}


