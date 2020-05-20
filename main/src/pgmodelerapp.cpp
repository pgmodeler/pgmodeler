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
#include "pgmodelerapp.h"
#include "globalattributes.h"
#include "messagebox.h"
#include "attributes.h"

PgModelerApp::PgModelerApp(int &argc, char **argv) : Application(argc,argv)
{
	QTranslator *main_translator=nullptr, *plugin_translator=nullptr;
	QFile ui_style(GlobalAttributes::getTmplConfigurationFilePath("",
																																GlobalAttributes::UiStyleConf +
																																GlobalAttributes::ConfigurationExt));
	QString plugin_name, plug_lang_dir, plug_lang_file;
	QStringList dir_list;
	QDir dir;

	try
	{
		//Creating the initial user's configuration
		createUserConfiguration();
	}
	catch(Exception &e)
	{
		Messagebox msgbox;
		msgbox.show(e);
	}

	//Changing the current working dir to the executable's directory in
	QDir::setCurrent(this->applicationDirPath());

	//Adding paths which executable will find plugins and it's dependecies
	this->addLibraryPath(this->applicationDirPath());

	//If pgModeler bundles plugins, add the root plugins path to lib search paths
	if(dir.exists(GlobalAttributes::getPluginsDir()))
		this->addLibraryPath(GlobalAttributes::getPluginsDir());

	//Check if the temporary dir exists, if not, creates it.
	if(!dir.exists(GlobalAttributes::getTemporaryDir()))
	{
		if(!dir.mkdir(GlobalAttributes::getTemporaryDir()))
		{
			Messagebox msg;
			msg.show(Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(GlobalAttributes::getTemporaryDir()),
												 ErrorCode::FileDirectoryNotWritten, __PRETTY_FUNCTION__,__FILE__,__LINE__));
		}
	}

	//Trying to identify if the user defined a custom UI language in the pgmodeler.conf file
	QString conf_file =	GlobalAttributes::getConfigurationFilePath(GlobalAttributes::GeneralConf);
	QFile input;
	QString lang_id = QLocale::system().name();

	input.setFileName(conf_file);

	if(input.open(QFile::ReadOnly))
	{
		QString buf = QString(input.readAll());
		QRegExp regexp = QRegExp(QString("(%1)(.*)(=)(\\\")(.)+(\\\")(\\\n)").arg(Attributes::UiLanguage));
		int idx =	regexp.indexIn(QString(buf));

		//Extract the value of the ui-language attribute in the conf file
		lang_id = buf.mid(idx, regexp.matchedLength());
		lang_id.remove(Attributes::UiLanguage);
		lang_id.remove(QChar('"')).remove(QChar('=')).remove(QChar('\n'));
	}

	//Tries to load the main ui translation according to the system's locale
	main_translator=new QTranslator(this);
	main_translator->load(lang_id, GlobalAttributes::getLanguagesDir());
	this->installTranslator(main_translator);

	//Trying to load plugins translations
	dir_list=QDir(GlobalAttributes::getPluginsDir() +
								GlobalAttributes::DirSeparator,
								QString("*"), QDir::Name, QDir::AllDirs | QDir::NoDotAndDotDot).entryList();

	while(!dir_list.isEmpty())
	{
		plugin_name=dir_list.front();
		dir_list.pop_front();

		//Configure the path to "lang" subdir at current plugin directory
		plug_lang_dir=GlobalAttributes::getPluginsDir() +
					  GlobalAttributes::DirSeparator + plugin_name +
					  GlobalAttributes::DirSeparator + QString("lang") +
					  GlobalAttributes::DirSeparator;

		plug_lang_file=plugin_name + QString(".") + lang_id;

		//Check if the .qm file exists for the current plugin. If so create and install a translator
		if(QFileInfo(plug_lang_dir + plug_lang_file + QString(".qm")).exists())
		{
			plugin_translator=new QTranslator(this);
			plugin_translator->load(plug_lang_file, plug_lang_dir);
			this->installTranslator(plugin_translator);
		}
	}

	//Loading app style sheet
	ui_style.open(QFile::ReadOnly);

	//Raises an error if ui style is not found
	if(!ui_style.isOpen())
	{
		Messagebox msg;
		msg.show(Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(ui_style.fileName()),
											 ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__));
	}
	else
		this->setStyleSheet(ui_style.readAll());
}

bool PgModelerApp::notify(QObject *receiver, QEvent *event)
{
	try
	{
		return QApplication::notify(receiver,event);
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
		return false;
	}
	catch(...)
	{
		Messagebox msg_box;
		msg_box.show(tr("Unknown exception caught!"), Messagebox::ErrorIcon);
		return false;
	}
}
