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

#include "dummyplugin.h"
#include "exception.h"
#include "messagebox.h"

DummyPlugin::DummyPlugin(void)
{
	QTranslator *translator=new QTranslator;

	translator->load(QString("dummyplugin.") + QLocale::system().name(),
									 GlobalAttributes::PLUGINS_DIR +
									 GlobalAttributes::DIR_SEPARATOR +
									 QString("dummyplugin") +
									 GlobalAttributes::DIR_SEPARATOR +
									 QString("lang"));

	QCoreApplication::installTranslator(translator);

	configurePluginInfo(getPluginTitle(),
											getPluginVersion(),
											getPluginAuthor(),
											getPluginDescription(),

											GlobalAttributes::PLUGINS_DIR +
											GlobalAttributes::DIR_SEPARATOR +
											QString("dummyplugin") +
											GlobalAttributes::DIR_SEPARATOR + "dummyplugin.png");
}

QString DummyPlugin::getPluginTitle(void)
{
	return(trUtf8("Dummy Plugin"));
}


QString DummyPlugin::getPluginVersion(void)
{
	return("0.1");
}

QString DummyPlugin::getPluginAuthor(void)
{
	return("Raphael A. Silva");
}

QString DummyPlugin::getPluginDescription(void)
{
	return(trUtf8("A dummy plugin only to test the pgModeler plugin structure."));
}

void DummyPlugin::showPluginInfo(void)
{
	plugin_info_frm->show();
}

void DummyPlugin::executePlugin(ModelWidget *)
{
	MessageBox msgbox;
	msgbox.show(trUtf8("Plugin successfully loaded!"),
							trUtf8("Plugin successfully loaded! Check the file PLUGINS.md to know how to create your own plugins."),
							MessageBox::INFO_ICON);
}

Q_EXPORT_PLUGIN2(dummyplugin, DummyPlugin)
