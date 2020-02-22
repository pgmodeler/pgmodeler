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

#include "baseconfigwidget.h"
#include "messagebox.h"

BaseConfigWidget::BaseConfigWidget(QWidget *parent) : QWidget(parent)
{
	config_changed=false;
}

void BaseConfigWidget::addConfigurationParam(map<QString, attribs_map> &config_params, const QString &param, const attribs_map &attribs)
{
	if(!param.isEmpty() && !attribs.empty())
		config_params[param]=attribs;
}

void BaseConfigWidget::showEvent(QShowEvent *)
{
	config_changed=false;
}

void BaseConfigWidget::setConfigurationChanged(bool changed)
{
	config_changed=changed;
}

bool BaseConfigWidget::isConfigurationChanged()
{
	return config_changed;
}

void BaseConfigWidget::saveConfiguration(const QString &conf_id, map<QString, attribs_map> &config_params)
{
	QByteArray buf;

	//Configures the schema filename for the configuration
	QString	sch_filename=GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir,
																																			conf_id + GlobalAttributes::SchemaExt),

			//Cofnigures the filename for the configuration file
			cfg_filename = GlobalAttributes::getConfigurationFilePath(conf_id);

	QFile output(cfg_filename);
	attribs_map attribs;
	map<QString, attribs_map >::iterator itr, itr_end;

	try
	{
		itr=config_params.begin();
		itr_end=config_params.end();

		while(itr!=itr_end)
		{
			attribs.insert((itr->second).begin(), (itr->second).end());
			itr++;
		}

		//Generates the configuration from the schema file
		schparser.ignoreEmptyAttributes(true);
		buf.append(XmlParser::convertCharsToXMLEntities(schparser.getCodeDefinition(sch_filename, attribs)));
		output.open(QFile::WriteOnly);

		if(!output.isOpen())
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(cfg_filename),
											ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Writes the generated configuration to the output file
		output.write(buf.data(), buf.size());
		output.close();
		config_params.erase(conf_id);
	}
	catch(Exception &e)
	{
		if(output.isOpen()) output.close();
		throw Exception(Exception::getErrorMessage(ErrorCode::FileNotWrittenInvalidDefinition).arg(cfg_filename),
										ErrorCode::FileNotWrittenInvalidDefinition,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void BaseConfigWidget::restoreDefaults(const QString &conf_id, bool silent)
{
	QString current_file, default_file;

	//Build the path to the current configuration (conf/[conf_id].conf
	current_file=GlobalAttributes::getConfigurationFilePath(conf_id);

	//Build the path to the default configuration file (conf/defaults/[conf_id].conf
	default_file=GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::DefaultConfsDir,
																															conf_id +
																															GlobalAttributes::ConfigurationExt);
	//Raises an error if the default file doesn't exists
	if(!QFile::exists(default_file))
		throw Exception(Exception::getErrorMessage(ErrorCode::DefaultConfigNotRestored).arg(default_file),
										ErrorCode::DefaultConfigNotRestored,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		bool bkp_saved = false;
		QFileInfo fi(current_file);
		QDir dir;
		QString bkp_dir = fi.absolutePath() + GlobalAttributes::DirSeparator + GlobalAttributes::ConfsBackupsDir,
				bkp_filename = bkp_dir + GlobalAttributes::DirSeparator +
											 QString("%1.bkp_%2").arg(fi.fileName()).arg(QDateTime::currentDateTime().toString("yyyyMMd_hhmmss"));

		dir.mkpath(bkp_dir);
		bkp_saved = QFile::rename(current_file, bkp_filename);
		QFile::copy(default_file, current_file);

		if(bkp_saved && !silent)
		{
			Messagebox msg_box;
			msg_box.show(tr("A backup of the previous settings was saved into <strong>%1</strong>!").arg(bkp_filename), Messagebox::InfoIcon);
		}
	}
}

void BaseConfigWidget::loadConfiguration(const QString &conf_id, map<QString, attribs_map> &config_params, const vector<QString> &key_attribs)
{
	QString filename;

	try
	{
		filename = GlobalAttributes::getConfigurationFilePath(conf_id);

		config_params.clear();
		xmlparser.restartParser();
		/* xmlparser.setDTDFile(GlobalAttributes::getTmplConfigurationDir() +
							 GlobalAttributes::DirSeparator +
							 GlobalAttributes::ObjectDTDDir +
							 GlobalAttributes::DirSeparator +
							 conf_id +
							 GlobalAttributes::ObjectDTDExt,
							 conf_id); */

		xmlparser.setDTDFile(GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::ObjectDTDDir,
																																				conf_id + GlobalAttributes::ObjectDTDExt),
												 conf_id);

		xmlparser.loadXMLFile(filename);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					this->getConfigurationParams(config_params, key_attribs);

					if(xmlparser.hasElement(XmlParser::ChildElement, XML_ELEMENT_NODE))
					{
						xmlparser.savePosition();
						xmlparser.accessElement(XmlParser::ChildElement);

						if(xmlparser.getElementType()!=XML_TEXT_NODE)
						{
							do
							{
								this->getConfigurationParams(config_params, key_attribs);
							}
							while(xmlparser.accessElement(XmlParser::NextElement));
						}

						xmlparser.restorePosition();
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, filename);
	}
}

void BaseConfigWidget::getConfigurationParams(map<QString, attribs_map> &config_params, const vector<QString> &key_attribs)
{
	attribs_map aux_attribs;
	attribs_map::iterator itr, itr_end;
	QString key;

	xmlparser.getElementAttributes(aux_attribs);

	itr=aux_attribs.begin();
	itr_end=aux_attribs.end();

	while(itr!=itr_end && key.isEmpty())
	{
		if(key.isEmpty() && std::find(key_attribs.begin(), key_attribs.end(), itr->first)!=key_attribs.end())
			key=itr->second;

		itr++;
	}

	if(key.isEmpty())
		key=xmlparser.getElementName();

	//Extract the contents of the child element and create a special element on map called "_contents_"
	if(xmlparser.hasElement(XmlParser::ChildElement, XML_TEXT_NODE))
	{
		xmlparser.savePosition();
		xmlparser.accessElement(XmlParser::ChildElement);
		aux_attribs[Attributes::Contents]=xmlparser.getElementContent();
		xmlparser.restorePosition();
	}

	if(!aux_attribs.empty())
		config_params[key]=aux_attribs;
}

