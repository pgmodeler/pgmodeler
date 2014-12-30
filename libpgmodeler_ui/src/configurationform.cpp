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

#include "configurationform.h"

ConfigurationForm::ConfigurationForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	general_conf=new GeneralConfigWidget(this);
	appearance_conf=new AppearanceConfigWidget(this);
	connections_conf=new ConnectionsConfigWidget(this);
	plugins_conf=new PluginsConfigWidget(this);
	relationships_conf=new RelationshipConfigWidget(this);
  snippets_conf=new SnippetsConfigWidget(this);

  QWidgetList wgt_list={ general_conf, relationships_conf,
                         appearance_conf, connections_conf,
                         snippets_conf, plugins_conf};

  for(int i=GENERAL_CONF_WGT; i <= PLUGINS_CONF_WGT; i++)
    confs_stw->addWidget(wgt_list[i]);

	connect(icons_lst, SIGNAL(currentRowChanged(int)), confs_stw, SLOT(setCurrentIndex(int)));
	connect(cancel_btn, SIGNAL(clicked(void)), this, SLOT(reject(void)));
	connect(apply_btn, SIGNAL(clicked(void)), this, SLOT(applyConfiguration(void)));
	connect(defaults_btn, SIGNAL(clicked(void)), this, SLOT(restoreDefaults(void)));
}

void ConfigurationForm::reject(void)
{
	try
	{
		if(sender()==cancel_btn)
    {
      appearance_conf->loadConfiguration();
      connections_conf->loadConfiguration();
      snippets_conf->loadConfiguration();
		}
	}
	catch(Exception &)
	{}

	QDialog::reject();
}

void ConfigurationForm::applyConfiguration(void)
{
  for(int i=GENERAL_CONF_WGT; i <= SNIPPETS_CONF_WGT; i++)
   qobject_cast<BaseConfigWidget *>(confs_stw->widget(i))->saveConfiguration();

  general_conf->applyConfiguration();
  relationships_conf->applyConfiguration();

	QDialog::accept();
}

void ConfigurationForm::loadConfiguration(void)
{
	try
	{
    for(int i=GENERAL_CONF_WGT; i <= PLUGINS_CONF_WGT; i++)
      qobject_cast<BaseConfigWidget *>(confs_stw->widget(i))->loadConfiguration();
	}
	catch(Exception &e)
	{
		if(e.getErrorType()==ERR_PLUGINS_NOT_LOADED)
		{
			Messagebox msg_box;
			msg_box.show(e);
		}
		else
			throw Exception(ERR_CONFIG_NOT_LOADED,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConfigurationForm::restoreDefaults(void)
{
	Messagebox msg_box;
	msg_box.show(trUtf8("Confirmation"),
									trUtf8("Any modification made until now in the current section will be lost! Do you really want to restore default settings?"),
									Messagebox::CONFIRM_ICON,
									Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
    qobject_cast<BaseConfigWidget *>(confs_stw->currentWidget())->restoreDefaults();
}

BaseConfigWidget *ConfigurationForm::getConfigurationWidget(unsigned idx)
{
	if(idx >= static_cast<unsigned>(confs_stw->count()))
		return(nullptr);
	else
    return(qobject_cast<BaseConfigWidget *>(confs_stw->widget(static_cast<unsigned>(idx))));
}

