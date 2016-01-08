/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "globalattributes.h"
#include <QCoreApplication>
#include <QTextStream>

namespace GlobalAttributes {
  const QString
  PGMODELER_APP_NAME=QString("pgmodeler"),
  PGMODELER_URI=QString("pgmodeler.com.br"),
  PGMODELER_REVERSE_URI=QString("br.com.pgmodeler"),
  PGMODELER_VERSION=QString("0.8.2-beta"),
  PGMODELER_VER_CODENAME=QString("Faithful Elephant"),
  PGMODELER_BUILD_NUMBER=QString(BUILDNUM),
  PGMODELER_SITE=QString("http://www.pgmodeler.com.br"),
  PGMODELER_WIKI=QString("%1/wiki").arg(PGMODELER_SITE),
  PGMODELER_SRC_URL=QString("https://github.com/pgmodeler/pgmodeler/releases"),
  PGMODELER_PURCHASE_URL=QString("%1/purchase.php").arg(PGMODELER_SITE),
  PGMODELER_RECOVER_URL=QString("%1/recover.php").arg(PGMODELER_SITE),
  PGMODELER_DONATE_URL=QString("%1/donate.html").arg(PGMODELER_SITE),
  PGMODELER_UPD_CHECK_URL=QString("%1/checkupdate.php?current_ver=").arg(PGMODELER_SITE),

  BUG_REPORT_EMAIL=QString("bug@pgmodeler.com.br"),
  BUG_REPORT_FILE=QString("pgmodeler%1.bug"),
  STACKTRACE_FILE=QString(".stacktrace"),

  DIR_SEPARATOR=QString("/"),
  DEFAULT_CONFS_DIR=QString("defaults"),
  SCHEMAS_DIR=QString("schemas"),
  SQL_SCHEMA_DIR=QString("sql"),
  XML_SCHEMA_DIR=QString("xml"),
  ALTER_SCHEMA_DIR=QString("alter"),
  SCHEMA_EXT=QString(".sch"),
  OBJECT_DTD_DIR=QString("dtd"),
  OBJECT_DTD_EXT=QString(".dtd"),
  ROOT_DTD=QString("dbmodel"),
  CONFIGURATION_EXT=QString(".conf"),
  HIGHLIGHT_FILE_SUF=QString("-highlight"),

  CODE_HIGHLIGHT_CONF=QString("source-code-highlight"),
  OBJECTS_STYLE_CONF=QString("objects-style"),
  GENERAL_CONF=QString("pgmodeler"),
  CONNECTIONS_CONF=QString("connections"),
  RELATIONSHIPS_CONF=QString("relationships"),
  SNIPPETS_CONF=QString("snippets"),

  SQL_HIGHLIGHT_CONF=QString("sql-highlight"),
  XML_HIGHLIGHT_CONF=QString("xml-highlight"),
  PATTERN_HIGHLIGHT_CONF=QString("pattern-highlight"),

  EXAMPLE_MODEL=QString("example.dbm"),
  UI_STYLE_CONF=QString("ui-style"),

  DEFAULT_QT_STYLE=QString("Fusion"),
  UI_STYLE_OPT=QString("-style"),

  SCHEMAS_ROOT_DIR=getPathFromEnv(QString("PGMODELER_SCHEMAS_DIR"), QString(SCHEMASDIR), QString("./schemas")),
  LANGUAGES_DIR=getPathFromEnv(QString("PGMODELER_LANG_DIR"), QString(LANGDIR), QString("./lang")),
  SAMPLES_DIR=getPathFromEnv(QString("PGMODELER_SAMPLES_DIR"), QString(SAMPLESDIR), QString("./samples")),
  TMPL_CONFIGURATIONS_DIR=getPathFromEnv(QString("PGMODELER_TMPL_CONF_DIR"), QString(CONFDIR), QString("./conf")),

  //Currently, plugins folder is auto-created when missing so it can't be resolved by getPathFromEnv()
  PLUGINS_DIR=getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace('\\','/') : QString(PLUGINSDIR),

  #if defined(Q_OS_MAC)
    CONFIGURATIONS_DIR=getPathFromEnv(QString("PGMODELER_CONF_DIR"),
                                      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PGMODELER_REVERSE_URI)),

    TEMPORARY_DIR=getPathFromEnv(QString("PGMODELER_TMP_DIR"),
                                 QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)  + QString("/%1/tmp").arg(PGMODELER_REVERSE_URI)),

  #elif defined(Q_OS_LINUX)
    CONFIGURATIONS_DIR=getPathFromEnv(QString("PGMODELER_CONF_DIR"),
                                      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PGMODELER_APP_NAME)),

    TEMPORARY_DIR=getPathFromEnv(QString("PGMODELER_TMP_DIR"),
                                 QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1/tmp").arg(PGMODELER_APP_NAME)),
  #else
    CONFIGURATIONS_DIR=getPathFromEnv(QString("PGMODELER_CONF_DIR"),
                                      QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +  QString("/%1").arg(PGMODELER_APP_NAME)),

    TEMPORARY_DIR=getPathFromEnv(QString("PGMODELER_TMP_DIR"),
                                 QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1/tmp").arg(PGMODELER_APP_NAME)),
  #endif

  SQL_HIGHLIGHT_CONF_PATH=CONFIGURATIONS_DIR + DIR_SEPARATOR + SQL_HIGHLIGHT_CONF + CONFIGURATION_EXT,
  XML_HIGHLIGHT_CONF_PATH=CONFIGURATIONS_DIR + DIR_SEPARATOR + XML_HIGHLIGHT_CONF + CONFIGURATION_EXT,

  #if defined(Q_OS_UNIX)
    #if defined(Q_OS_MAC)
      //For MacOSX the crash handler path is fixed (inside bundle)
      PGMODELER_CHANDLER_PATH=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"),
                                             QString(BINDIR) + QString("/pgmodeler-ch"),
                                             QString("./pgmodeler-ch")),
    #else
      PGMODELER_CHANDLER_PATH=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"),
                                             QString(PRIVATEBINDIR) + QString("/pgmodeler-ch"),
                                             QString("./pgmodeler-ch")),
    #endif

    PGMODELER_CLI_PATH=getPathFromEnv(QString("PGMODELER_CLI_PATH"),
                                      QString(BINDIR) + QString("/pgmodeler-cli"),
                                      QString("./pgmodeler-cli")),

    PGMODELER_APP_PATH=getPathFromEnv(QString("PGMODELER_APP_PATH"),
                                      QString(BINDIR) + QString("/pgmodeler"),
                                      QString("./pgmodeler"));


  #else
    PGMODELER_CHANDLER_PATH=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"),
                                           QString(PRIVATEBINDIR) + QString("\\pgmodeler-ch.exe"),
                                           QString(".\\pgmodeler-ch.exe")),

    PGMODELER_CLI_PATH=getPathFromEnv(QString("PGMODELER_CLI_PATH"),
                                      QString(PRIVATEBINDIR) + QString("\\pgmodeler-cli.exe"),
                                      QString(".\\pgmodeler-cli.exe")),

    PGMODELER_APP_PATH=getPathFromEnv(QString("PGMODELER_APP_PATH"),
                                      QString(BINDIR) + QString("\\pgmodeler.exe"),
                                      QString(".\\pgmodeler.exe"));

  #endif

  #ifdef DEMO_VERSION
   //Maximum object creation counter for demo version
   const unsigned MAX_OBJECT_COUNT=8;
  #endif


  QString getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val)
  {
     QFileInfo fi;
     QStringList paths={ QDir::toNativeSeparators(getenv(varname.toStdString().c_str())),
                         QDir::toNativeSeparators(default_val) };

     for(int i=0; i < 2; i++)
     {
       fi.setFile(paths[i]);
       if(fi.exists() || (i==1 && fallback_val.isEmpty()))
         return(paths[i].replace('\\','/'));
     }

     fi.setFile(fallback_val);
     return(fi.absoluteFilePath());
  }
}
