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

#include "dummy.h"
#include "exception.h"
#include "messagebox.h"

Dummy::Dummy(void)
{
	QTranslator *translator=new QTranslator;

	translator->load(QString("dummy.") + QLocale::system().name(),
									 GlobalAttributes::PLUGINS_DIR +
									 GlobalAttributes::DIR_SEPARATOR +
									 QString("dummy") +
									 GlobalAttributes::DIR_SEPARATOR +
									 QString("lang"));

	QCoreApplication::installTranslator(translator);

	configurePluginInfo(getPluginTitle(),
											getPluginVersion(),
											getPluginAuthor(),
											getPluginDescription(),

											GlobalAttributes::PLUGINS_DIR +
											GlobalAttributes::DIR_SEPARATOR +
											QString("dummy") +
											GlobalAttributes::DIR_SEPARATOR + "dummy.png");
}

QString Dummy::getPluginTitle(void)
{
	return(trUtf8("Dummy"));
}


QString Dummy::getPluginVersion(void)
{
	return("0.1");
}

QString Dummy::getPluginAuthor(void)
{
	return("Raphael A. Silva");
}

QString Dummy::getPluginDescription(void)
{
	return(trUtf8("A dummy plugin only to test the pgModeler plugin structure."));
}

void Dummy::showPluginInfo(void)
{
	plugin_info_frm->show();
}

void Dummy::executePlugin(ModelWidget *)
{
	Messagebox msgbox;
	msgbox.show(trUtf8("Plugin successfully loaded!"),
							trUtf8("Plugin successfully loaded! Check the <a href='http://www.pgmodeler.com.br/wiki/doku.php?id=plugins'>plugins wiki page</a> to know how to create your own plugins."),
							Messagebox::INFO_ICON);
}

QKeySequence Dummy::getPluginShortcut(void)
{
	return(QKeySequence("Ctrl+J"));
}
