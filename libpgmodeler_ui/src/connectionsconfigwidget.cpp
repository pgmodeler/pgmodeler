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

#include "connectionsconfigwidget.h"
#include "pgmodeleruins.h"
#include "baseform.h"

vector<Connection *> ConnectionsConfigWidget::connections;
map<QString, attribs_map> ConnectionsConfigWidget::config_params;
const QString ConnectionsConfigWidget::DefaultFor("default-for-%1");

ConnectionsConfigWidget::ConnectionsConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
{
	Ui_ConnectionsConfigWidget::setupUi(this);

	connect(ssl_mode_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableCertificates()));

	connect(new_tb, SIGNAL(clicked(bool)), this, SLOT(newConnection()));
	connect(cancel_tb, SIGNAL(clicked(bool)), this, SLOT(newConnection()));
	connect(duplicate_tb, SIGNAL(clicked(bool)), this, SLOT(duplicateConnection()));

	connect(test_tb, SIGNAL(clicked(bool)), this, SLOT(testConnection()));
	connect(add_tb, SIGNAL(clicked(bool)), this, SLOT(handleConnection()));
	connect(update_tb, SIGNAL(clicked(bool)), this, SLOT(handleConnection()));
	connect(edit_tb, SIGNAL(clicked(bool)), this, SLOT(editConnection()));
	connect(remove_tb, SIGNAL(clicked(bool)), this, SLOT(removeConnection()));

	connect(alias_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest()));
	connect(host_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest()));
	connect(user_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest()));
	connect(passwd_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest()));
	connect(conn_db_edt, SIGNAL(textChanged(QString)), this, SLOT(enableConnectionTest()));

	update_tb->setVisible(false);
	cancel_tb->setVisible(false);
}

ConnectionsConfigWidget::~ConnectionsConfigWidget()
{

}

void ConnectionsConfigWidget::hideEvent(QHideEvent *)
{
	this->newConnection();
}

void ConnectionsConfigWidget::showEvent(QShowEvent *)
{
	updateConnectionsCombo();
	newConnection();
	conn_attribs_tbw->setCurrentIndex(0);
}

void ConnectionsConfigWidget::updateConnectionsCombo()
{
	connections_cmb->clear();

	for(auto &conn : connections)
		connections_cmb->addItem(QIcon(PgModelerUiNs::getIconPath("server")), conn->getConnectionId());
}

void ConnectionsConfigWidget::destroyConnections()
{
	Connection *conn=nullptr;

	while(!connections.empty())
	{
		conn=connections.back();
		connections.pop_back();
		connections_cmb->removeItem(0);
		delete conn;
	}
}

map<QString, attribs_map> ConnectionsConfigWidget::getConfigurationParams()
{
	return config_params;
}

void ConnectionsConfigWidget::loadConfiguration()
{
	try
	{
		vector<QString> key_attribs;
		map<QString, attribs_map >::iterator itr, itr_end;
		Connection *conn=nullptr;

		destroyConnections();
		key_attribs.push_back(Attributes::Alias);
		BaseConfigWidget::loadConfiguration(GlobalAttributes::ConnectionsConf, config_params, key_attribs);

		itr=config_params.begin();
		itr_end=config_params.end();

		while(itr!=itr_end)
		{
			conn=new Connection;

			conn->setConnectionParam(Connection::ParamAlias, itr->second[Attributes::Alias]);
			conn->setConnectionParam(Connection::ParamServerFqdn, itr->second[Connection::ParamServerFqdn]);
			conn->setConnectionParam(Connection::ParamPort, itr->second[Connection::ParamPort]);
			conn->setConnectionParam(Connection::ParamUser, itr->second[Connection::ParamUser]);
			conn->setConnectionParam(Connection::ParamPassword,itr->second[Connection::ParamPassword]);
			conn->setConnectionParam(Connection::ParamDbName, itr->second[Connection::ParamDbName]);
			conn->setConnectionParam(Connection::ParamConnTimeout, itr->second[Attributes::ConnectionTimeout]);
			conn->setConnectionParam(Connection::ParamSslMode, itr->second[Connection::ParamSslMode]);
			conn->setConnectionParam(Connection::ParamSslRootCert, itr->second[Connection::ParamSslRootCert]);
			conn->setConnectionParam(Connection::ParamSslCert, itr->second[Connection::ParamSslCert]);
			conn->setConnectionParam(Connection::ParamSslKey, itr->second[Connection::ParamSslKey]);
			conn->setConnectionParam(Connection::ParamSslCrl, itr->second[Connection::ParamSslCrl]);
			conn->setConnectionParam(Connection::ParamLibGssapi, itr->second[Connection::ParamLibGssapi]);
			conn->setConnectionParam(Connection::ParamKerberosServer, itr->second[Connection::ParamKerberosServer]);
			conn->setConnectionParam(Connection::ParamOthers, itr->second[Connection::ParamOthers]);

			conn->setAutoBrowseDB(itr->second[Attributes::AutoBrowseDb]==Attributes::True);
			conn->setDefaultForOperation(Connection::OpDiff, itr->second[DefaultFor.arg(Attributes::Diff)]==Attributes::True);
			conn->setDefaultForOperation(Connection::OpExport, itr->second[DefaultFor.arg(Attributes::Export)]==Attributes::True);
			conn->setDefaultForOperation(Connection::OpImport, itr->second[DefaultFor.arg(Attributes::Import)]==Attributes::True);
			conn->setDefaultForOperation(Connection::OpValidation, itr->second[DefaultFor.arg(Attributes::Validation)]==Attributes::True);

			connections.push_back(conn);
			itr++;
		}

		edit_tb->setEnabled(!connections.empty());
		remove_tb->setEnabled(!connections.empty());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo());
	}
}

void ConnectionsConfigWidget::enableCertificates()
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

void ConnectionsConfigWidget::enableConnectionTest()
{
	test_tb->setEnabled(!alias_edt->text().isEmpty() &&
						!host_edt->text().isEmpty() &&
						!user_edt->text().isEmpty() &&
						!conn_db_edt->text().isEmpty());
	add_tb->setEnabled(test_tb->isEnabled());
	update_tb->setEnabled(test_tb->isEnabled());

	if(!isConfigurationChanged())
		setConfigurationChanged(true);
}

void ConnectionsConfigWidget::newConnection()
{
	conn_db_edt->clear();
	alias_edt->clear();
	user_edt->clear();
	host_edt->clear();
	port_sbp->setValue(5432);
	passwd_edt->clear();
	other_params_edt->clear();

	auto_browse_chk->setChecked(false);
	diff_chk->setChecked(false);
	export_chk->setChecked(false);
	import_chk->setChecked(false);
	validation_chk->setChecked(false);

	ssl_mode_cmb->setCurrentIndex(0);
	client_cert_edt->setText(QString("~/.postgresql/postgresql.crt"));
	root_cert_edt->setText(QString("~/.postgresql/root.crt"));
	crl_edt->setText(QString("~/.postgresql/root.crl"));
	client_key_edt->setText(QString("~/.postgresql/postgresql.key"));

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
	duplicate_tb->setEnabled(connections_cmb->count() > 0);
}

void ConnectionsConfigWidget::duplicateConnection()
{
	Connection *conn=nullptr, *new_conn=nullptr;

	try
	{
		conn=connections.at(connections_cmb->currentIndex());
		new_conn=new Connection;
		(*new_conn)=(*conn);
		connections.push_back(new_conn);

		new_conn->setConnectionParam(Connection::ParamAlias, QString("cp_%1").arg(conn->getConnectionParam(Connection::ParamAlias)));
		connections_cmb->addItem(QIcon(QString(":icones/icones/server.png")), new_conn->getConnectionId());
		connections_cmb->setCurrentIndex(connections_cmb->count()-1);
		setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		if(new_conn)
			delete new_conn;

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::handleConnection()
{
	Connection *conn=nullptr;

	try
	{
		if(!update_tb->isVisible())
		{
			conn=new Connection;
			this->configureConnection(conn);
			connections_cmb->addItem(QIcon(QString(":icones/icones/server.png")), conn->getConnectionId());
			connections.push_back(conn);
		}
		else
		{
			conn=connections.at(connections_cmb->currentIndex());
			this->configureConnection(conn);
			connections_cmb->setItemText(connections_cmb->currentIndex(), conn->getConnectionId());
		}

		this->newConnection();
		edit_tb->setEnabled(connections_cmb->count() > 0);
		remove_tb->setEnabled(connections_cmb->count() > 0);
		setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		if(add_tb->isVisible())
			delete conn;

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::removeConnection()
{
	if(connections_cmb->currentIndex() >= 0)
	{
		Connection *conn=nullptr;
		conn=connections.at(connections_cmb->currentIndex());
		connections.erase(connections.begin() + connections_cmb->currentIndex());
		connections_cmb->removeItem(connections_cmb->currentIndex());
		delete conn;
		this->newConnection();
		setConfigurationChanged(true);
	}
}

void ConnectionsConfigWidget::editConnection()
{
	if(connections_cmb->count() > 0)
	{
		Connection *conn=nullptr;

		conn=connections.at(connections_cmb->currentIndex());
		alias_edt->setText(conn->getConnectionParam(Connection::ParamAlias));
		auto_browse_chk->setChecked(conn->isAutoBrowseDB());

		diff_chk->setChecked(conn->isDefaultForOperation(Connection::OpDiff));
		export_chk->setChecked(conn->isDefaultForOperation(Connection::OpExport));
		import_chk->setChecked(conn->isDefaultForOperation(Connection::OpImport));
		validation_chk->setChecked(conn->isDefaultForOperation(Connection::OpValidation));

		if(!conn->getConnectionParam(Connection::ParamServerFqdn).isEmpty())
			host_edt->setText(conn->getConnectionParam(Connection::ParamServerFqdn));
		else
			host_edt->setText(conn->getConnectionParam(Connection::ParamServerIp));

		conn_db_edt->setText(conn->getConnectionParam(Connection::ParamDbName));
		user_edt->setText(conn->getConnectionParam(Connection::ParamUser));
		passwd_edt->setText(conn->getConnectionParam(Connection::ParamPassword));
		port_sbp->setValue(conn->getConnectionParam(Connection::ParamPort).toInt());
		timeout_sbp->setValue(conn->getConnectionParam(Connection::ParamConnTimeout).toInt());

		krb_server_edt->setText(conn->getConnectionParam(Connection::ParamKerberosServer));
		gssapi_auth_chk->setChecked(conn->getConnectionParam(Connection::ParamLibGssapi)==QString("gssapi"));
		other_params_edt->setText(conn->getConnectionParam(Connection::ParamOthers));

		if(conn->getConnectionParam(Connection::ParamSslMode)==Connection::SslDisable)
			ssl_mode_cmb->setCurrentIndex(0);
		else if(conn->getConnectionParam(Connection::ParamSslMode)==Connection::SslAllow)
			ssl_mode_cmb->setCurrentIndex(1);
		else if(conn->getConnectionParam(Connection::ParamSslMode)==Connection::SslRequire)
			ssl_mode_cmb->setCurrentIndex(2);
		else if(conn->getConnectionParam(Connection::ParamSslMode)==Connection::SslCaVerify)
			ssl_mode_cmb->setCurrentIndex(3);
		else
			ssl_mode_cmb->setCurrentIndex(4);

		if(ssl_mode_cmb->currentIndex() > 0)
		{
			client_cert_edt->setText(conn->getConnectionParam(Connection::ParamSslCert));
			root_cert_edt->setText(conn->getConnectionParam(Connection::ParamSslRootCert));
			client_key_edt->setText(conn->getConnectionParam(Connection::ParamSslKey));
			crl_edt->setText(conn->getConnectionParam(Connection::ParamSslCrl));
		}

		update_tb->setVisible(true);
		add_tb->setVisible(false);
		connections_cmb->setEnabled(false);

		new_tb->setVisible(false);
		duplicate_tb->setEnabled(false);
		cancel_tb->setVisible(true);
		edit_tb->setEnabled(false);
	}
}

void ConnectionsConfigWidget::configureConnection(Connection *conn)
{
	if(conn)
	{
		conn->setAutoBrowseDB(auto_browse_chk->isChecked());
		conn->setConnectionParam(Connection::ParamAlias, alias_edt->text());
		conn->setConnectionParam(Connection::ParamServerIp, QString());
		conn->setConnectionParam(Connection::ParamServerFqdn, host_edt->text());
		conn->setConnectionParam(Connection::ParamPort, QString("%1").arg(port_sbp->value()));
		conn->setConnectionParam(Connection::ParamUser, user_edt->text());
		conn->setConnectionParam(Connection::ParamPassword, passwd_edt->text());
		conn->setConnectionParam(Connection::ParamDbName, conn_db_edt->text());
		conn->setConnectionParam(Connection::ParamConnTimeout, QString("%1").arg(timeout_sbp->value()));

		conn->setDefaultForOperation(Connection::OpDiff, diff_chk->isChecked());
		conn->setDefaultForOperation(Connection::OpExport, export_chk->isChecked());
		conn->setDefaultForOperation(Connection::OpImport, import_chk->isChecked());
		conn->setDefaultForOperation(Connection::OpValidation, validation_chk->isChecked());

		switch(ssl_mode_cmb->currentIndex())
		{
			case 1:
				conn->setConnectionParam(Connection::ParamSslMode, Connection::SslAllow);
			break;
			case 2:
				conn->setConnectionParam(Connection::ParamSslMode, Connection::SslRequire);
			break;
			case 3:
				conn->setConnectionParam(Connection::ParamSslMode, Connection::SslCaVerify);
			break;
			case 4:
				conn->setConnectionParam(Connection::ParamSslMode, Connection::SslFullVerify);
			break;
			default:
			case 0:
				conn->setConnectionParam(Connection::ParamSslMode, Connection::SslDisable);
			break;
		}

		if(ssl_mode_cmb->currentIndex()!=0)
		{
			conn->setConnectionParam(Connection::ParamSslRootCert, root_cert_edt->text());
			conn->setConnectionParam(Connection::ParamSslCert, client_cert_edt->text());
			conn->setConnectionParam(Connection::ParamSslKey, client_key_edt->text());
			conn->setConnectionParam(Connection::ParamSslCrl, crl_edt->text());
		}

		if(gssapi_auth_chk->isChecked())
			conn->setConnectionParam(Connection::ParamLibGssapi, QString("gssapi"));

		if(!krb_server_edt->text().isEmpty())
			conn->setConnectionParam(Connection::ParamKerberosServer, krb_server_edt->text());

		if(!other_params_edt->text().isEmpty())
			conn->setConnectionParam(Connection::ParamOthers, other_params_edt->text());
	}
}

void ConnectionsConfigWidget::testConnection()
{
	Connection conn;
	Messagebox msg_box;
	attribs_map srv_info;

	try
	{
		this->configureConnection(&conn);
		conn.connect();
		srv_info=conn.getServerInfo();
		msg_box.show(tr("Success"),
					 PgModelerUiNs::formatMessage(tr("Connection successfully established!\n\nServer details:\n\nPID: `%1'\nProtocol: `%2'\nVersion: `%3'"))
					 .arg(srv_info[Connection::ServerPid])
				.arg(srv_info[Connection::ServerProtocol])
				.arg(srv_info[Connection::ServerVersion]), Messagebox::InfoIcon);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::restoreDefaults()
{
	try
	{
		//Restore the default connection config file
		BaseConfigWidget::restoreDefaults(GlobalAttributes::ConnectionsConf, false);

		//Remove all connections
		while(connections_cmb->count() > 0)
			this->removeConnection();

		//Reloads the configuration
		this->loadConfiguration();

		updateConnectionsCombo();
		this->setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::saveConfiguration()
{
	try
	{
		attribs_map attribs;

		/* If add or update buttons are enabled when saving the configs indicates
	   that user forgot to click on these buttons and register the connection,
	   so in order to do not lost the data pgModeler will ask to save the connection. */
		if(add_tb->isEnabled() || update_tb->isEnabled())
		{
			Messagebox msg_box;

			msg_box.show(tr("There is a connection being created or edited! Do you want to save it?"),
									 Messagebox::AlertIcon, Messagebox::YesNoButtons);

			if(msg_box.result()==QDialog::Accepted)
				handleConnection();
		}

		config_params[GlobalAttributes::ConnectionsConf].clear();

		/* Workaround: When there is no connection, to prevent saving an empty file, is necessary to
		 fill the attribute CONNECTIONS with white spaces */
		if(connections.empty())
			config_params[GlobalAttributes::ConnectionsConf][Attributes::Connections]=QString("  ");
		else
		{
			for(Connection *conn : connections)
			{
				attribs=conn->getConnectionParams();

				if(attribs[Connection::ParamServerFqdn].isEmpty())
					attribs[Connection::ParamServerFqdn]=attribs[Connection::ParamServerIp];

				attribs[Attributes::Alias]=attribs[Connection::ParamAlias];
				attribs[Attributes::AutoBrowseDb]=(conn->isAutoBrowseDB() ? Attributes::True : QString());
				attribs[Attributes::ConnectionTimeout]=attribs[Connection::ParamConnTimeout];

				attribs[DefaultFor.arg(Attributes::Export)]=(conn->isDefaultForOperation(Connection::OpExport) ? Attributes::True : QString());
				attribs[DefaultFor.arg(Attributes::Import)]=(conn->isDefaultForOperation(Connection::OpImport) ? Attributes::True : QString());
				attribs[DefaultFor.arg(Attributes::Diff)]=(conn->isDefaultForOperation(Connection::OpDiff) ? Attributes::True : QString());
				attribs[DefaultFor.arg(Attributes::Validation)]=(conn->isDefaultForOperation(Connection::OpValidation) ? Attributes::True : QString());

				schparser.ignoreUnkownAttributes(true);
				config_params[GlobalAttributes::ConnectionsConf][Attributes::Connections]+=
						schparser.getCodeDefinition(GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir,
																																											 GlobalAttributes::ConnectionsConf +
																																											 GlobalAttributes::SchemaExt), attribs);

				schparser.ignoreUnkownAttributes(false);
			}
		}

		schparser.ignoreUnkownAttributes(true);
		BaseConfigWidget::saveConfiguration(GlobalAttributes::ConnectionsConf, config_params);
		schparser.ignoreUnkownAttributes(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConnectionsConfigWidget::getConnections(map<QString, Connection *> &conns, bool inc_hosts)
{
	QString alias;

	conns.clear();
	for(Connection *conn : connections)
	{
		alias=conn->getConnectionId();

		if(!inc_hosts)
			alias.remove(QRegExp(QString(" \\((.)*\\)")));

		conns[alias]=conn;
	}
}

Connection *ConnectionsConfigWidget::getConnection(const QString &conn_id)
{
	for(Connection *conn : connections)
	{
		if(conn->getConnectionId() == conn_id)
			return conn;
	}

	return nullptr;
}

void ConnectionsConfigWidget::fillConnectionsComboBox(QComboBox *combo, bool incl_placeholder, unsigned check_def_for)
{
	map<QString, Connection *> connections;
	Connection *def_conn=nullptr;

	if(!combo)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	getConnections(connections);

	combo->blockSignals(true);
	combo->clear();

	if(incl_placeholder)
	{
		if(!connections.empty())
			combo->addItem(tr("Found %1 connection(s)").arg(connections.size()));
		else
			combo->addItem(tr("No connections found"));
	}

	for(auto &itr : connections)
	{
		combo->addItem(QIcon(PgModelerUiNs::getIconPath("server")), itr.first, QVariant::fromValue<void *>(itr.second));

		if(!def_conn && itr.second->isDefaultForOperation(check_def_for))
			def_conn=itr.second;
	}

	if(incl_placeholder)
		combo->addItem(QIcon(QString(":icones/icones/conexaobd.png")), tr("Edit connections"));

	if(def_conn)
		combo->setCurrentText(def_conn->getConnectionId());

	combo->blockSignals(false);
}

bool ConnectionsConfigWidget::openConnectionsConfiguration(QComboBox *combo, bool incl_placeholder)
{
	if(combo)
	{
		BaseForm parent_form;
		ConnectionsConfigWidget conn_cfg_wgt;
		bool conn_saved = false;

		parent_form.setWindowTitle(tr("Edit database connections"));
		parent_form.setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

		connect(parent_form.cancel_btn, SIGNAL(clicked(bool)), &parent_form, SLOT(reject()));
		connect(parent_form.apply_ok_btn, SIGNAL(clicked(bool)),  &parent_form, SLOT(accept()));

		try
		{
			conn_cfg_wgt.loadConfiguration();
			conn_cfg_wgt.frame->setFrameShape(QFrame::NoFrame);
			conn_cfg_wgt.frame->layout()->setContentsMargins(2,2,2,2);

			parent_form.setMainWidget(&conn_cfg_wgt);
			parent_form.setButtonConfiguration(Messagebox::OkCancelButtons);
			parent_form.exec();

			if(parent_form.result()==QDialog::Accepted)
			{
				conn_cfg_wgt.saveConfiguration();
				conn_saved=true;
			}

			conn_cfg_wgt.fillConnectionsComboBox(combo, incl_placeholder);
		}
		catch(Exception &e)
		{
			combo->setCurrentIndex(0);
			throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
		}

		return conn_saved;
	}

	return false;
}

Connection *ConnectionsConfigWidget::getDefaultConnection(unsigned operation)
{
	Connection *conn=nullptr;

	for(Connection *aux_conn : connections)
	{
		if(aux_conn->isDefaultForOperation(operation))
		{
			conn=aux_conn;
			break;
		}
	}

	return conn;
}
