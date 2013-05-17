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

#include "xml2object.h"
#include "exception.h"
#include "messagebox.h"

Xml2Object::Xml2Object(void)
{
	QTranslator *translator=new QTranslator;

	translator->load(QString("xml2object.") + QLocale::system().name(),
									 GlobalAttributes::PLUGINS_DIR +
									 GlobalAttributes::DIR_SEPARATOR +
									 QString("xml2object") +
									 GlobalAttributes::DIR_SEPARATOR +
									 QString("lang"));

	QCoreApplication::installTranslator(translator);

	configurePluginInfo(getPluginTitle(),
											getPluginVersion(),
											getPluginAuthor(),
											getPluginDescription(),

											GlobalAttributes::PLUGINS_DIR +
											GlobalAttributes::DIR_SEPARATOR +
											QString("xml2object") +
											GlobalAttributes::DIR_SEPARATOR + "xml2object.png");
}

QString Xml2Object::getPluginTitle(void)
{
	return(trUtf8("Xml2Object"));
}

QString Xml2Object::getPluginVersion(void)
{
	return("0.1");
}

QString Xml2Object::getPluginAuthor(void)
{
	return("Raphael A. Silva");
}

QString Xml2Object::getPluginDescription(void)
{
	return(trUtf8("This plugin permits the creation of objects from XML code and inserting them on the currently opened model."));
}

void Xml2Object::showPluginInfo(void)
{
	plugin_info_frm->show();
}

void Xml2Object::executePlugin(ModelWidget *model)
{
	if(!model)
		throw Exception(trUtf8("This plugin must be executed with at least one model opened!"),ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	xml2obj_wgt.show(model->getDatabaseModel(), model->getOperationList());
}

QKeySequence Xml2Object::getPluginShortcut(void)
{
	return(QKeySequence("Ctrl+K"));
}
