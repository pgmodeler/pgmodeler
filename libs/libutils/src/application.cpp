/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "application.h"
#include "exception.h"
#include <QTranslator>

Application::Application(int &argc, char **argv) : QApplication(argc,argv)
{
	/* Checking if the user specified another widget style using the -style param
	 * If no custom style is specified we force the usage of Fusion (the default for Qt and pgModeler) */
	if(!arguments().contains("-style"))
		setStyle(GlobalAttributes::DefaultQtStyle);
}

bool Application::loadTranslation(const QString &lang_id, const QString &directory)
{
	if(lang_id.isEmpty())
		return false;

	QTranslator *translator = new QTranslator(this);

	/* Tries to load the ui translation according to the system's locale,
	 * and in case of success install it in the application */
	if(!translator->load(lang_id, directory) ||
		 !installTranslator(translator))
	{
		delete(translator);
		return false;
	}

	return true;
}

void Application::createUserConfiguration(bool missing_only)
{
	QDir config_dir(GlobalAttributes::getConfigurationsPath()),
			old_cfg_dir(GlobalAttributes::getConfigurationsPath().replace(GlobalAttributes::PgModelerAppName, "pgmodeler"));

	/* First, we check if there are pgModeler 0.9.x config files in the user's local storage.
	 * If that's the case, we copy some files that are compatible with pgModeler 1.x+ */
	if(!config_dir.exists() && old_cfg_dir.exists())
	{
		QStringList old_files = old_cfg_dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
		config_dir.mkpath(config_dir.path());

		for(auto &file : old_files)
		{
			if(file.contains("-style") ||
				 file.contains("-highlight") ||
				 file.contains("pgmodeler") ||
				 file.contains(GlobalAttributes::RelationshipsConf))
				continue;

			QFile::copy(old_cfg_dir.absoluteFilePath(file), config_dir.absoluteFilePath(file));
		}
	}

	try
	{
		//If the directory not exists
		if(!config_dir.exists() || missing_only ||
			 // If the overwrite flag is not set we'll copy the files only if the directory is empty
			(!missing_only &&
			 config_dir.entryList({QString("*%1").arg(GlobalAttributes::ConfigurationExt)},
														QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).isEmpty()))
		{
			copyFilesRecursively(GlobalAttributes::getTmplConfigurationPath(), GlobalAttributes::getConfigurationsPath(), missing_only);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(ErrorCode::InitialUserConfigNotCreated).arg(GlobalAttributes::getConfigurationsPath(), GlobalAttributes::getTmplConfigurationPath()), ErrorCode::InitialUserConfigNotCreated,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Application::copyFilesRecursively(const QString &src_path, const QString &dst_path, bool missing_only)
{
	QFileInfo src_file(src_path);

	if(!src_file.exists())
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(src_path),
						__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(src_file.isDir())
	{
		QString new_src_path, new_dst_path;
		QStringList filenames;
		QDir dst_dir(dst_path),
				src_dir(src_path);

		if(!dst_dir.exists() && !dst_dir.mkpath(dst_path))
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(dst_path),
							__PRETTY_FUNCTION__,__FILE__,__LINE__);

		filenames = src_dir.entryList({QString("*%1").arg(GlobalAttributes::ConfigurationExt)}, QDir::Files | QDir::NoDotAndDotDot);

		for(auto &filename : filenames)
		{
			new_src_path = src_path + src_dir.separator() + filename;
			new_dst_path = dst_path + dst_dir.separator() + filename;

			// Ignoring ui style confs
			if(filename.startsWith("ui-") || filename.startsWith("icons-") ||
				 (missing_only && QFileInfo::exists(new_dst_path)))
				continue;

			copyFilesRecursively(new_src_path, new_dst_path, missing_only);
		}
	}
	else
	{
		if(!QFile::copy(src_path, dst_path))
		{
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(dst_path),
											__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		else
		{
			// Set write permissions when copying file with read-only permissions
			QFile file(dst_path);

			if(!file.permissions().testFlag(QFile::WriteOwner))
				file.setPermissions(file.permissions() | QFileDevice::WriteOwner);
		}
	}
}
