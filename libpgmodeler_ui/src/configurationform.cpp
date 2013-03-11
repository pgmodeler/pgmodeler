#include "configurationform.h"

ConfigurationForm::ConfigurationForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	QGridLayout *layout=NULL;

	setupUi(this);

	general_conf=new GeneralConfigWidget(this);
	appearance_conf=new AppearanceConfigWidget(this);
	connections_conf=new ConnectionsConfigWidget(this);
	plugins_conf=new PluginsConfigWidget(this);

	layout=new QGridLayout;
	layout->setContentsMargins(2,2,2,2);
	layout->addWidget(general_conf);
	confs_stw->widget(GENERAL_CONF_WGT)->setLayout(layout);

	layout=new QGridLayout;
	layout->setContentsMargins(2,2,2,2);
	layout->addWidget(appearance_conf);
	confs_stw->widget(APPEARANCE_CONF_WGT)->setLayout(layout);

	layout=new QGridLayout;
	layout->setContentsMargins(2,2,2,2);
	layout->addWidget(connections_conf);
	confs_stw->widget(CONNECTIONS_CONF_WGT)->setLayout(layout);

	layout=new QGridLayout;
	layout->setContentsMargins(2,2,2,2);
	layout->addWidget(plugins_conf);
	confs_stw->widget(PLUGINS_CONF_WGT)->setLayout(layout);

	connect(icons_lst, SIGNAL(currentRowChanged(int)), confs_stw, SLOT(setCurrentIndex(int)));
	connect(cancel_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
	connect(apply_btn, SIGNAL(clicked(void)), this, SLOT(applyConfiguration(void)));
	connect(defaults_btn, SIGNAL(clicked(void)), this, SLOT(restoreDefaults(void)));
}

void ConfigurationForm::close(void)
{
	try
	{
		if(sender()==cancel_btn)
		{
			appearance_conf->loadConfiguration();
			connections_conf->loadConfiguration();
		}
	}
	catch(Exception &e)
	{}

	QDialog::close();
}

void ConfigurationForm::applyConfiguration(void)
{
	general_conf->saveConfiguration();
	general_conf->applyConfiguration();
	appearance_conf->saveConfiguration();
	connections_conf->saveConfiguration();
	this->close();
}

void ConfigurationForm::loadConfiguration(void)
{
	try
	{
		general_conf->loadConfiguration();
		appearance_conf->loadConfiguration();
		connections_conf->loadConfiguration();
		plugins_conf->loadPlugins();
	}
	catch(Exception &e)
	{
		if(e.getErrorType()==ERR_PLUGINS_NOT_LOADED)
			msg_box.show(e);
		else
			throw Exception(ERR_CONFIG_NOT_LOADED,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConfigurationForm::restoreDefaults(void)
{
	msg_box.show(trUtf8("Confirmation"),
									trUtf8("Any modification made until now in the current section will be lost! Do you really want to restore default settings?"),
									MessageBox::CONFIRM_ICON,
									MessageBox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		switch(confs_stw->currentIndex())
		{
			case GENERAL_CONF_WGT:
				dynamic_cast<GeneralConfigWidget *>(this->getConfigurationWidget(ConfigurationForm::GENERAL_CONF_WGT))->restoreDefaults();
			break;

			case APPEARANCE_CONF_WGT:
				dynamic_cast<AppearanceConfigWidget *>(this->getConfigurationWidget(ConfigurationForm::APPEARANCE_CONF_WGT))->restoreDefaults();
			break;

			case CONNECTIONS_CONF_WGT:
				dynamic_cast<ConnectionsConfigWidget *>(this->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->restoreDefaults();
			break;

			default:
			break;
		}
	}
}

BaseConfigWidget *ConfigurationForm::getConfigurationWidget(unsigned idx)
{
	if(idx >= static_cast<unsigned>(confs_stw->count()))
		return(NULL);
	else
	{
		switch(idx)
		{
			case GENERAL_CONF_WGT: return(dynamic_cast<BaseConfigWidget *>(general_conf)); break;
			case APPEARANCE_CONF_WGT: return(dynamic_cast<BaseConfigWidget *>(appearance_conf)); break;
			case CONNECTIONS_CONF_WGT: return(dynamic_cast<BaseConfigWidget *>(connections_conf)); break;
			default:
			return(dynamic_cast<BaseConfigWidget *>(plugins_conf));
			break;
		}
	}
}

