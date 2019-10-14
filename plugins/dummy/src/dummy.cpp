/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "mainwindow.h"

Dummy::Dummy(void)
{
	configurePluginInfo(getPluginTitle(),
						getPluginVersion(),
						getPluginAuthor(),
						getPluginDescription(),

						GlobalAttributes::PluginsDir +
						GlobalAttributes::DirSeparator +
						QString("dummy") +
						GlobalAttributes::DirSeparator + QString("dummy.png"));
}

QString Dummy::getPluginTitle(void)
{
	return(trUtf8("Dummy"));
}


QString Dummy::getPluginVersion(void)
{
	return(QString("0.1"));
}

QString Dummy::getPluginAuthor(void)
{
	return(QString("Raphael A. Silva"));
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
							Messagebox::InfoIcon);
}

void Dummy::setMainWindow(QMainWindow *main_window)
{
	this->main_window = main_window;

	/* The sample code below shows how to interact with the exposed main window by adding
	 * a tool button to the bottom widgets bar.
	 *
	 * You can use this basic approach to add more UI elements to it.
	 * To do so, open the design file mainwindow.ui to see the name of the widgets you
	 * need to handle */

	 //MainWindow *mw = dynamic_cast<MainWindow *>(main_window);
	 //QToolButton *tb = new QToolButton(mw->tool_btns_bar_wgt);
	 //mw->tool_btns_bar_wgt->layout()->addWidget(tb);
	 //tb->setText("This is a test button");
	 //connect(tb, &QToolButton::clicked, [&](){
	 //	 Messagebox msgbox;
	 //	 msgbox.show(trUtf8("Alert"), trUtf8("This is a message box triggered by the click signal emitted by the test button!"), Messagebox::AlertIcon);
	 //});
}

QKeySequence Dummy::getPluginShortcut(void)
{
	return(QKeySequence(QString("Ctrl+J")));
}
