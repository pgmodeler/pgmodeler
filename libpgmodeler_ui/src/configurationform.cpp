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

#include "configurationform.h"

ConfigurationForm::ConfigurationForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	general_conf=new GeneralConfigWidget(this);
	appearance_conf=new AppearanceConfigWidget(this);
	connections_conf=new ConnectionsConfigWidget(this);
	relationships_conf=new RelationshipConfigWidget(this);
	snippets_conf=new SnippetsConfigWidget(this);
	plugins_conf=new PluginsConfigWidget(this);

	QWidgetList wgt_list={ general_conf, relationships_conf,
												 appearance_conf, connections_conf,
												 snippets_conf, plugins_conf};

	for(int i=GeneralConfWgt; i <= PluginsConfWgt; i++)
		confs_stw->addWidget(wgt_list[i]);

	connect(icons_lst, SIGNAL(currentRowChanged(int)), confs_stw, SLOT(setCurrentIndex(int)));
	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(apply_btn, SIGNAL(clicked()), this, SLOT(applyConfiguration()));
	connect(defaults_btn, SIGNAL(clicked()), this, SLOT(restoreDefaults()));

	icons_lst->setCurrentRow(GeneralConfWgt);
	setMinimumSize(890, 740);
}

ConfigurationForm::~ConfigurationForm()
{
	connections_conf->destroyConnections();
}

void ConfigurationForm::hideEvent(QHideEvent *)
{
	icons_lst->setCurrentRow(GeneralConfWgt);
}

void ConfigurationForm::showEvent(QShowEvent *)
{
	snippets_conf->snippet_txt->updateLineNumbers();
}

void ConfigurationForm::reject()
{
	try
	{
		if(sender()==cancel_btn)
		{
			QWidgetList wgt_list={ appearance_conf, connections_conf, snippets_conf };
			BaseConfigWidget *conf_wgt=nullptr;

			for(QWidget *wgt : wgt_list)
			{
				conf_wgt=qobject_cast<BaseConfigWidget *>(wgt);

				if(conf_wgt->isConfigurationChanged())
					conf_wgt->loadConfiguration();
			}
		}
	}
	catch(Exception &)
	{}

	QDialog::reject();
}

void ConfigurationForm::applyConfiguration()
{
	BaseConfigWidget *conf_wgt=nullptr;
	bool curr_escape_comments = BaseObject::isEscapeComments();

	for(int i=GeneralConfWgt; i <= SnippetsConfWgt; i++)
	{
		conf_wgt=qobject_cast<BaseConfigWidget *>(confs_stw->widget(i));

		if(conf_wgt->isConfigurationChanged())
			conf_wgt->saveConfiguration();
	}

	general_conf->applyConfiguration();
	relationships_conf->applyConfiguration();

	if(curr_escape_comments != BaseObject::isEscapeComments())
		emit s_invalidateModelsRequested();

	QDialog::accept();
}

void ConfigurationForm::loadConfiguration()
{
	BaseConfigWidget *config_wgt = nullptr;

	for(int i=GeneralConfWgt; i <= PluginsConfWgt; i++)
	{
		try
		{
			config_wgt = qobject_cast<BaseConfigWidget *>(confs_stw->widget(i));
			config_wgt->loadConfiguration();
		}
		catch(Exception &e)
		{
			Messagebox msg_box;

			if(e.getErrorCode()==ErrorCode::PluginsNotLoaded)
			{
				msg_box.show(e);
			}
			else
			{
				Exception ex = Exception(Exception::getErrorMessage(ErrorCode::ConfigurationNotLoaded).arg(e.getExtraInfo()),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
				msg_box.show(ex, QString("%1 %2").arg(ex.getErrorMessage()).arg(tr("In some cases restore the default settings related to it may solve the problem. Would like to do that?")),
										 Messagebox::AlertIcon, Messagebox::YesNoButtons, tr("Restore"), QString(), QString(), PgModelerUiNs::getIconPath("atualizar"));

				if(msg_box.result() == QDialog::Accepted)
					config_wgt->restoreDefaults();
			}
		}
	}
}

void ConfigurationForm::restoreDefaults()
{
	Messagebox msg_box;
	msg_box.show(tr("Any modification made until now in the current section will be lost! Do you really want to restore default settings?"),
				 Messagebox::ConfirmIcon,
				 Messagebox::YesNoButtons);

	if(msg_box.result()==QDialog::Accepted)
		qobject_cast<BaseConfigWidget *>(confs_stw->currentWidget())->restoreDefaults();
}

BaseConfigWidget *ConfigurationForm::getConfigurationWidget(unsigned idx)
{
	if(idx >= static_cast<unsigned>(confs_stw->count()))
		return nullptr;
	else
		return qobject_cast<BaseConfigWidget *>(confs_stw->widget(static_cast<unsigned>(idx)));
}

