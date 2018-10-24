/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "pgmodelercli.h"

QTextStream PgModelerCLI::out(stdout);

const QRegExp PgModelerCLI::PASSWORD_REGEXP=QRegExp("(password)(=)(.)*( )");
const QString PgModelerCLI::PASSWORD_PLACEHOLDER=QString("password=******");

const QString PgModelerCLI::INPUT=QString("--input");
const QString PgModelerCLI::OUTPUT=QString("--output");
const QString PgModelerCLI::INPUT_DB=QString("--input-db");
const QString PgModelerCLI::EXPORT_TO_FILE=QString("--export-to-file");
const QString PgModelerCLI::EXPORT_TO_PNG=QString("--export-to-png");
const QString PgModelerCLI::EXPORT_TO_SVG=QString("--export-to-svg");
const QString PgModelerCLI::EXPORT_TO_DBMS=QString("--export-to-dbms");
const QString PgModelerCLI::IMPORT_DB=QString("--import-db");
const QString PgModelerCLI::DIFF=QString("--diff");
const QString PgModelerCLI::DROP_DATABASE=QString("--drop-database");
const QString PgModelerCLI::DROP_OBJECTS=QString("--drop-objects");
const QString PgModelerCLI::PGSQL_VER=QString("--pgsql-ver");
const QString PgModelerCLI::HELP=QString("--help");
const QString PgModelerCLI::SHOW_GRID=QString("--show-grid");
const QString PgModelerCLI::SHOW_DELIMITERS=QString("--show-delimiters");
const QString PgModelerCLI::PAGE_BY_PAGE=QString("--page-by-page");
const QString PgModelerCLI::IGNORE_DUPLICATES=QString("--ignore-duplicates");
const QString PgModelerCLI::IGNORE_ERROR_CODES=QString("--ignore-error-codes");
const QString PgModelerCLI::CONN_ALIAS=QString("--conn-alias");
const QString PgModelerCLI::HOST=QString("--host");
const QString PgModelerCLI::PORT=QString("--port");
const QString PgModelerCLI::USER=QString("--user");
const QString PgModelerCLI::PASSWD=QString("--passwd");
const QString PgModelerCLI::INITIAL_DB=QString("--initial-db");
const QString PgModelerCLI::SILENT=QString("--silent");
const QString PgModelerCLI::LIST_CONNS=QString("--list-conns");
const QString PgModelerCLI::SIMULATE=QString("--simulate");
const QString PgModelerCLI::FIX_MODEL=QString("--fix-model");
const QString PgModelerCLI::FIX_TRIES=QString("--fix-tries");
const QString PgModelerCLI::ZOOM_FACTOR=QString("--zoom");
const QString PgModelerCLI::USE_TMP_NAMES=QString("--use-tmp-names");
const QString PgModelerCLI::DBM_MIME_TYPE=QString("--dbm-mime-type");
const QString PgModelerCLI::INSTALL=QString("install");
const QString PgModelerCLI::UNINSTALL=QString("uninstall");
const QString PgModelerCLI::IGNORE_IMPORT_ERRORS=QString("--ignore-errors");
const QString PgModelerCLI::IMPORT_SYSTEM_OBJS=QString("--import-sys-objs");
const QString PgModelerCLI::IMPORT_EXTENSION_OBJS=QString("--import-ext-objs");
const QString PgModelerCLI::DEBUG_MODE=QString("--debug-mode");
const QString PgModelerCLI::COMPARE_TO=QString("--compare-to");
const QString PgModelerCLI::SAVE_DIFF=QString("--save-diff");
const QString PgModelerCLI::APPLY_DIFF=QString("--apply-diff");
const QString PgModelerCLI::NO_DIFF_PREVIEW=QString("--no-diff-preview");
const QString PgModelerCLI::DROP_CLUSTER_OBJS=QString("--drop-cluster-objs");
const QString PgModelerCLI::REVOKE_PERMISSIONS=QString("--revoke-perms");
const QString PgModelerCLI::DROP_MISSING_OBJS=QString("--drop-missing");
const QString PgModelerCLI::FORCE_DROP_COLS_CONSTRS=QString("--force-drop-cols");
const QString PgModelerCLI::RENAME_DB=QString("--rename-db");
const QString PgModelerCLI::TRUNC_ON_COLS_TYPE_CHANGE=QString("--trunc-type-change");
const QString PgModelerCLI::NO_SEQUENCE_REUSE=QString("--no-sequence-reuse");
const QString PgModelerCLI::NO_CASCADE_DROP_TRUNC=QString("--no-cascade");
const QString PgModelerCLI::NO_FORCE_OBJ_RECREATION=QString("--no-force-recreation");
const QString PgModelerCLI::NO_UNMOD_OBJ_RECREATION=QString("--no-unmod-recreation");

const QString PgModelerCLI::TAG_EXPR=QString("<%1");
const QString PgModelerCLI::END_TAG_EXPR=QString("</%1");
const QString PgModelerCLI::ATTRIBUTE_EXPR=QString("(%1)( )*(=)(\")(\\w|\\d|,|\\.|\\&|\\;)+(\")");

PgModelerCLI::PgModelerCLI(int argc, char **argv) :  QApplication(argc, argv)
{
	try
	{
		QString op, value;
		bool accepts_val=false;
		int eq_pos=-1;
		attribs_map opts;

		model=nullptr;
		scene=nullptr;
		xmlparser=nullptr;
		zoom=1;

		initializeOptions();

		if(argc > 1)
		{
			for(int i=1; i < argc; i++)
			{
				op=argv[i];

				//If the retrieved option starts with - it will be treated as a command option
				if(op[0]=='-')
				{
					value.clear();
					eq_pos=op.indexOf('=');

					// if the option has a = attached strip the string, assuming as value the	right part of it
					if(eq_pos >= 0)
					{
						value=op.mid(eq_pos+1);
						op=op.mid(0,eq_pos);
					}
					else if(i < argc-1 && argv[i+1][0]!='-')
					{
						//If the next option does not starts with '-', is considered a value
						value=argv[++i];
					}

					//Raises an error if the option is not recognized
					if(!isOptionRecognized(op, accepts_val))
						throw Exception(trUtf8("Unrecognized option '%1'.").arg(op), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Raises an error if the value is empty and the option accepts a value
					if(accepts_val && value.isEmpty())
						throw Exception(trUtf8("Value not specified for option '%1'.").arg(op), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					else if(!accepts_val && !value.isEmpty())
						throw Exception(trUtf8("Option '%1' does not accept values.").arg(op), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					opts[op]=value;
				}
			}
		}

		//Validates and executes the options
		parseOptions(opts);

		if(!parsed_opts.empty())
		{
			model=new DatabaseModel;
			xmlparser=model->getXMLParser();
			silent_mode=(parsed_opts.count(SILENT));

			//If the export is to png or svg loads additional configurations
			if(parsed_opts.count(EXPORT_TO_PNG) || parsed_opts.count(EXPORT_TO_SVG) || parsed_opts.count(IMPORT_DB))
			{
				connect(model, SIGNAL(s_objectAdded(BaseObject*)), this, SLOT(handleObjectAddition(BaseObject *)));
				connect(model, SIGNAL(s_objectRemoved(BaseObject*)), this, SLOT(handleObjectRemoval(BaseObject *)));

				//Creates a scene to
				scene=new ObjectsScene;
				scene->setParent(this);
				scene->setSceneRect(QRectF(0,0,2000,2000));

				//Load the general configuration including grid and delimiter options
				GeneralConfigWidget conf_wgt;
				conf_wgt.loadConfiguration();

				//Load the objects styles
				BaseObjectView::loadObjectsStyle();
			}

			if(parsed_opts.count(EXPORT_TO_DBMS) || parsed_opts.count(IMPORT_DB) || parsed_opts.count(DIFF))
			{
				configureConnection(false);

				//Replacing the initial db parameter for the input database when reverse engineering
				if((parsed_opts.count(IMPORT_DB) || parsed_opts.count(DIFF)) && !parsed_opts[INPUT_DB].isEmpty())
					connection.setConnectionParam(Connection::ParamDbName, parsed_opts[INPUT_DB]);
			}

			if(parsed_opts.count(DIFF))
			{
				configureConnection(true);

				if(!extra_connection.isConfigured())
					extra_connection = connection;

				extra_connection.setConnectionParam(Connection::ParamDbName, parsed_opts[COMPARE_TO]);
			}

			if(!silent_mode)
			{
				connect(&export_hlp, SIGNAL(s_progressUpdated(int,QString)), this, SLOT(updateProgress(int,QString)));
				connect(&export_hlp, SIGNAL(s_errorIgnored(QString,QString,QString)), this, SLOT(printIgnoredError(QString,QString,QString)));
				connect(&import_hlp, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString)));
				connect(&diff_hlp, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString)));
			}
		}
	}
	catch(Exception &e)
	{
		throw e;
	}
}

PgModelerCLI::~PgModelerCLI(void)
{
	if(scene) delete(scene);
	delete(model);
}

void PgModelerCLI::printMessage(const QString &msg)
{
	if(!silent_mode) out << msg << endl;
}

void PgModelerCLI::configureConnection(bool extra_conn)
{
	QString chr = (extra_conn ? "1" : "");
	Connection *conn = (extra_conn ? &extra_connection : &connection);

	//Getting the connection using its alias
	if(parsed_opts.count(CONN_ALIAS + chr))
	{
		if(!connections.count(parsed_opts[CONN_ALIAS + chr]))
			throw Exception(trUtf8("Connection aliased as '%1' was not found in the configuration file.").arg(parsed_opts[CONN_ALIAS + chr]),
							Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Make a copy of the named connection
		*conn = (*connections[parsed_opts[CONN_ALIAS + chr]]);
	}
	else
	{
		conn->setConnectionParam(Connection::ParamServerFqdn, parsed_opts[HOST + chr]);
		conn->setConnectionParam(Connection::ParamUser, parsed_opts[USER + chr]);
		conn->setConnectionParam(Connection::ParamPort, parsed_opts[PORT + chr]);
		conn->setConnectionParam(Connection::ParamPassword, parsed_opts[PASSWD + chr]);
		conn->setConnectionParam(Connection::ParamDbName, parsed_opts[INITIAL_DB + chr]);
	}
}

void PgModelerCLI::initializeOptions(void)
{
	long_opts[INPUT]=true;
	long_opts[OUTPUT]=true;
	long_opts[INPUT_DB]=true;
	long_opts[EXPORT_TO_FILE]=false;
	long_opts[EXPORT_TO_PNG]=false;
	long_opts[EXPORT_TO_SVG]=false;
	long_opts[EXPORT_TO_DBMS]=false;
	long_opts[IMPORT_DB]=false;
	long_opts[DIFF]=false;
	long_opts[DROP_DATABASE]=false;
	long_opts[DROP_OBJECTS]=false;
	long_opts[PGSQL_VER]=true;
	long_opts[HELP]=false;
	long_opts[SHOW_GRID]=false;
	long_opts[SHOW_DELIMITERS]=false;
	long_opts[PAGE_BY_PAGE]=false;
	long_opts[IGNORE_DUPLICATES]=false;
	long_opts[IGNORE_ERROR_CODES]=true;
	long_opts[CONN_ALIAS]=true;
	long_opts[HOST]=true;
	long_opts[PORT]=true;
	long_opts[USER]=true;
	long_opts[PASSWD]=true;
	long_opts[INITIAL_DB]=true;
	long_opts[LIST_CONNS]=false;
	long_opts[SIMULATE]=false;
	long_opts[FIX_MODEL]=false;
	long_opts[FIX_TRIES]=true;
	long_opts[ZOOM_FACTOR]=true;
	long_opts[USE_TMP_NAMES]=false;
	long_opts[DBM_MIME_TYPE]=true;
	long_opts[IGNORE_IMPORT_ERRORS]=false;
	long_opts[IMPORT_SYSTEM_OBJS]=false;
	long_opts[IMPORT_EXTENSION_OBJS]=false;
	long_opts[DEBUG_MODE]=false;
	long_opts[COMPARE_TO]=true;
	long_opts[SAVE_DIFF]=false;
	long_opts[APPLY_DIFF]=false;
	long_opts[NO_DIFF_PREVIEW]=false;
	long_opts[DROP_CLUSTER_OBJS]=false;
	long_opts[REVOKE_PERMISSIONS]=false;
	long_opts[DROP_MISSING_OBJS]=false;
	long_opts[FORCE_DROP_COLS_CONSTRS]=false;
	long_opts[RENAME_DB]=false;
	long_opts[TRUNC_ON_COLS_TYPE_CHANGE]=false;
	long_opts[NO_SEQUENCE_REUSE]=false;
	long_opts[NO_CASCADE_DROP_TRUNC]=false;
	long_opts[NO_FORCE_OBJ_RECREATION]=false;
	long_opts[NO_UNMOD_OBJ_RECREATION]=false;

	short_opts[INPUT]=QString("-if");
	short_opts[OUTPUT]=QString("-of");
	short_opts[INPUT_DB]=QString("-id");
	short_opts[EXPORT_TO_FILE]=QString("-ef");
	short_opts[EXPORT_TO_PNG]=QString("-ep");
	short_opts[EXPORT_TO_SVG]=QString("-es");
	short_opts[EXPORT_TO_DBMS]=QString("-ed");
	short_opts[IMPORT_DB]=QString("-im");
	short_opts[DIFF]=QString("-df");
	short_opts[DROP_DATABASE]=QString("-dd");
	short_opts[DROP_OBJECTS]=QString("-do");
	short_opts[PGSQL_VER]=QString("-v");
	short_opts[HELP]=QString("-h");
	short_opts[SHOW_GRID]=QString("-sg");
	short_opts[SHOW_DELIMITERS]=QString("-sl");
	short_opts[PAGE_BY_PAGE]=QString("-pp");
	short_opts[IGNORE_DUPLICATES]=QString("-ir");
	short_opts[IGNORE_ERROR_CODES]=QString("-ic");
	short_opts[CONN_ALIAS]=QString("-ca");
	short_opts[HOST]=QString("-H");
	short_opts[PORT]=QString("-p");
	short_opts[USER]=QString("-u");
	short_opts[PASSWD]=QString("-w");
	short_opts[INITIAL_DB]=QString("-D");
	short_opts[SILENT]=QString("-s");
	short_opts[LIST_CONNS]=QString("-lc");
	short_opts[SIMULATE]=QString("-sm");
	short_opts[FIX_MODEL]=QString("-fm");
	short_opts[FIX_TRIES]=QString("-ft");
	short_opts[ZOOM_FACTOR]=QString("-zf");
	short_opts[USE_TMP_NAMES]=QString("-tn");
	short_opts[DBM_MIME_TYPE]=QString("-mt");
	short_opts[IGNORE_IMPORT_ERRORS]=QString("-ie");
	short_opts[IMPORT_SYSTEM_OBJS]=QString("-is");
	short_opts[IMPORT_EXTENSION_OBJS]=QString("-ix");
	short_opts[DEBUG_MODE]=QString("-d");
	short_opts[COMPARE_TO]=QString("-ct");
	short_opts[SAVE_DIFF]=QString("-sd");
	short_opts[APPLY_DIFF]=QString("-ad");
	short_opts[NO_DIFF_PREVIEW]=QString("-np");
	short_opts[DROP_CLUSTER_OBJS]=QString("-dc");
	short_opts[REVOKE_PERMISSIONS]=QString("-rv");
	short_opts[DROP_MISSING_OBJS]=QString("-dm");
	short_opts[FORCE_DROP_COLS_CONSTRS]=QString("-fd");
	short_opts[RENAME_DB]=QString("-rn");
	short_opts[TRUNC_ON_COLS_TYPE_CHANGE]=QString("-tt");
	short_opts[NO_SEQUENCE_REUSE]=QString("-ns");
	short_opts[NO_CASCADE_DROP_TRUNC]=QString("-nd");
	short_opts[NO_FORCE_OBJ_RECREATION]=QString("-nf");
	short_opts[NO_UNMOD_OBJ_RECREATION]=QString("-nu");
}

bool PgModelerCLI::isOptionRecognized(QString &op, bool &accepts_val)
{
	bool found=false, append_chr = false;

	if(op.endsWith('1'))
	{
		op.chop(1);
		append_chr = true;
	}

	for(auto &itr : short_opts)
	{
		found=(op==itr.first || op==itr.second);
		accepts_val=(found && long_opts[itr.first]);

		if(found)
		{
			op = itr.first;
			break;
		}
	}

	if(append_chr) op += '1';
	return(found);
}

void PgModelerCLI::showMenu(void)
{
	out << endl;
	out << QString("pgModeler ") << GlobalAttributes::PgModelerVersion << trUtf8(" command line interface.") << endl;
	out << trUtf8("PostgreSQL Database Modeler Project - pgmodeler.io") << endl;
	out << trUtf8("Copyright 2006-2018 Raphael A. Silva <raphael@pgmodeler.io>") << endl;
	out << endl;
	out << trUtf8("Usage: pgmodeler-cli [OPTIONS]") << endl << endl;
	out << trUtf8("This CLI tool provides several operations over models and databases without the need to perform them\nin pgModeler's graphical interface. All available options are described below.") << endl;
	out << endl;
	out << trUtf8("General options: ") << endl;
	out << trUtf8("  %1, %2 [FILE]\t\t    Input model file (.dbm). This is mandatory for fix, export operations.").arg(short_opts[INPUT]).arg(INPUT) << endl;
	out << trUtf8("  %1, %2 [DBNAME]\t    Input database name. This is mandatory for import operation.").arg(short_opts[INPUT_DB]).arg(INPUT_DB) << endl;
	out << trUtf8("  %1, %2 [FILE]\t\t    Output file. This is mandatory for fixing model or exporting to file, png or svg.").arg(short_opts[OUTPUT]).arg(OUTPUT) << endl;
	out << trUtf8("  %1, %2\t\t    Try to fix the structure of the input model file in order to make it loadable again.").arg(short_opts[FIX_MODEL]).arg(FIX_MODEL) << endl;
	out << trUtf8("  %1, %2 [NUMBER]\t    Model fix tries. When reaching the maximum count the invalid objects will be discarded.").arg(short_opts[FIX_TRIES]).arg(FIX_TRIES) << endl;
	out << trUtf8("  %1, %2\t\t    Export the input model to a sql script file.").arg(short_opts[EXPORT_TO_FILE]).arg(EXPORT_TO_FILE)<< endl;
	out << trUtf8("  %1, %2\t\t    Export the input model to a png image.").arg(short_opts[EXPORT_TO_PNG]).arg(EXPORT_TO_PNG) << endl;
	out << trUtf8("  %1, %2\t\t    Export the input model to a svg file.").arg(short_opts[EXPORT_TO_SVG]).arg(EXPORT_TO_SVG) << endl;
	out << trUtf8("  %1, %2\t\t    Export the input model directly to a PostgreSQL server.").arg(short_opts[EXPORT_TO_DBMS]).arg(EXPORT_TO_DBMS) << endl;
	out << trUtf8("  %1, %2\t\t    Import a database to an output file.").arg(short_opts[IMPORT_DB]).arg(IMPORT_DB) << endl;
	out << trUtf8("  %1, %2\t\t\t    Compares a model and a database or two databases generating the SQL script to synch the latter in relation to the first.").arg(short_opts[DIFF]).arg(DIFF) << endl;
	out << trUtf8("  %1, %2\t\t    Force the PostgreSQL version of generated SQL code.").arg(short_opts[PGSQL_VER]).arg(PGSQL_VER) << endl;
	out << trUtf8("  %1, %2\t\t\t    Silent execution. Only critical messages and errors are shown during process.").arg(short_opts[SILENT]).arg(SILENT) << endl;
	out << trUtf8("  %1, %2\t\t\t    Show this help menu.").arg(short_opts[HELP]).arg(HELP) << endl;
	out << endl;
	out << trUtf8("Connection options: ") << endl;
	out << trUtf8("  %1, %2\t\t    List available connections in file %3.").arg(short_opts[LIST_CONNS]).arg(LIST_CONNS).arg(GlobalAttributes::ConnectionsConf + GlobalAttributes::ConfigurationExt) << endl;
	out << trUtf8("  %1, %2 [ALIAS]\t    Connection configuration alias to be used.").arg(short_opts[CONN_ALIAS]).arg(CONN_ALIAS) << endl;
	out << trUtf8("  %1, %2 [HOST]\t\t    PostgreSQL host in which a task will operate.").arg(short_opts[HOST]).arg(HOST) << endl;
	out << trUtf8("  %1, %2 [PORT]\t\t    PostgreSQL host listening port.").arg(short_opts[PORT]).arg(PORT) << endl;
	out << trUtf8("  %1, %2 [USER]\t\t    PostgreSQL username.").arg(short_opts[USER]).arg(USER) << endl;
	out << trUtf8("  %1, %2 [PASSWORD]\t    PostgreSQL user password.").arg(short_opts[PASSWD]).arg(PASSWD) << endl;
	out << trUtf8("  %1, %2 [DBNAME]\t    Connection's initial database.").arg(short_opts[INITIAL_DB]).arg(INITIAL_DB) << endl;
	out << endl;
	out << trUtf8("PNG and SVG export options: ") << endl;
	out << trUtf8("  %1, %2\t\t    Draws the grid in the exported image.").arg(short_opts[SHOW_GRID]).arg(SHOW_GRID) << endl;
	out << trUtf8("  %1, %2\t    Draws the page delimiters in the exported image.").arg(short_opts[SHOW_DELIMITERS]).arg(SHOW_DELIMITERS) << endl;
	out << trUtf8("  %1, %2\t\t    Each page will be exported in a separated png image. (Only for PNG images)").arg(short_opts[PAGE_BY_PAGE]).arg(PAGE_BY_PAGE) << endl;
	out << trUtf8("  %1, %2 [FACTOR]\t\t    Applies a zoom (in percent) before export to png image. Accepted zoom interval: %3-%4 (Only for PNG images)").arg(short_opts[ZOOM_FACTOR]).arg(ZOOM_FACTOR).arg(ModelWidget::MINIMUM_ZOOM*100).arg(ModelWidget::MAXIMUM_ZOOM*100) << endl;
	out << endl;
	out << trUtf8("DBMS export options: ") << endl;
	out << trUtf8("  %1, %2\t    Ignores errors related to duplicated objects that eventually exist in the server.").arg(short_opts[IGNORE_DUPLICATES]).arg(IGNORE_DUPLICATES) << endl;
	out << trUtf8("  %1, %2 [CODES] Ignores additional errors by their codes. A comma-separated list of alphanumeric codes should be provided.").arg(short_opts[IGNORE_ERROR_CODES]).arg(IGNORE_ERROR_CODES) << endl;
	out << trUtf8("  %1, %2\t\t    Drop the database before execute a export process.").arg(short_opts[DROP_DATABASE]).arg(DROP_DATABASE) << endl;
	out << trUtf8("  %1, %2\t\t    Runs the DROP commands attached to SQL-enabled objects.").arg(short_opts[DROP_OBJECTS]).arg(DROP_OBJECTS) << endl;
	out << trUtf8("  %1, %2\t\t    Simulates an export process by executing all steps but undoing any modification in the end.").arg(short_opts[SIMULATE]).arg(SIMULATE) << endl;
	out << trUtf8("  %1, %2\t\t    Generates temporary names for database, roles and tablespaces when in simulation mode.").arg(short_opts[USE_TMP_NAMES]).arg(USE_TMP_NAMES) << endl;
	out << endl;
	out << trUtf8("Database import options: ") << endl;
	out << trUtf8("  %1, %2\t\t    Ignore all errors and try to create as many as possible objects.").arg(short_opts[IGNORE_IMPORT_ERRORS]).arg(IGNORE_IMPORT_ERRORS) << endl;
	out << trUtf8("  %1, %2\t    Import system built-in objects. This option causes the model bloating due to the importing of unneeded objects.").arg(short_opts[IMPORT_SYSTEM_OBJS]).arg(IMPORT_SYSTEM_OBJS) << endl;
	out << trUtf8("  %1, %2\t    Import extension objects. This option causes the model bloating due to the importing of unneeded objects.").arg(short_opts[IMPORT_EXTENSION_OBJS]).arg(IMPORT_EXTENSION_OBJS) << endl;
	out << trUtf8("  %1, %2\t\t    Run import in debug mode printing all queries executed in the server.").arg(short_opts[DEBUG_MODE]).arg(DEBUG_MODE) << endl;
	out << endl;
	out << trUtf8("Diff options: ") << endl;
	out << trUtf8("  %1, %2 [DBNAME]\t    The database used in the comparison. All the SQL code generated is applied to it.").arg(short_opts[COMPARE_TO]).arg(COMPARE_TO) << endl;
	out << trUtf8("  %1, %2\t\t    Save the generated diff code to output file.").arg(short_opts[SAVE_DIFF]).arg(SAVE_DIFF) << endl;
	out << trUtf8("  %1, %2\t\t    Apply the generated diff code on the database server.").arg(short_opts[APPLY_DIFF]).arg(APPLY_DIFF) << endl;
	out << trUtf8("  %1, %2\t    Don't preview the generated diff code when applying it to the server.").arg(short_opts[NO_DIFF_PREVIEW]).arg(NO_DIFF_PREVIEW) << endl;
	out << trUtf8("  %1, %2\t    Drop cluster level objects like roles and tablespaces.").arg(short_opts[DROP_CLUSTER_OBJS]).arg(DROP_CLUSTER_OBJS) << endl;
	out << trUtf8("  %1, %2\t\t    Revoke permissions already set on the database. New permissions configured in the input model are still applied.").arg(short_opts[REVOKE_PERMISSIONS]).arg(REVOKE_PERMISSIONS) << endl;
	out << trUtf8("  %1, %2\t\t    Drop missing objects. Generates DROP commands for objects that are present in the input model but not in the compared database.").arg(short_opts[DROP_MISSING_OBJS]).arg(DROP_MISSING_OBJS) << endl;
	out << trUtf8("  %1, %2\t    Force the drop of missing columns and constraints. Causes only columns and constraints to be dropped, other missing objects aren't removed.").arg(short_opts[FORCE_DROP_COLS_CONSTRS]).arg(FORCE_DROP_COLS_CONSTRS) << endl;
	out << trUtf8("  %1, %2\t\t    Rename the destination database when the names of the involved databases are different.").arg(short_opts[RENAME_DB]).arg(RENAME_DB) << endl;
	out << trUtf8("  %1, %2\t\t    Don't drop or truncate objects in cascade mode.").arg(short_opts[NO_CASCADE_DROP_TRUNC]).arg(NO_CASCADE_DROP_TRUNC) << endl;
	out << trUtf8("  %1, %2\t    Truncate tables prior to alter columns. Avoids errors related to type casting when the new type of a column isn't compatible to the old one.").arg(short_opts[TRUNC_ON_COLS_TYPE_CHANGE]).arg(TRUNC_ON_COLS_TYPE_CHANGE) << endl;
	out << trUtf8("  %1, %2\t    Don't reuse sequences on serial columns. Drop the old sequence assigned to a serial column and creates a new one.").arg(short_opts[NO_SEQUENCE_REUSE]).arg(NO_SEQUENCE_REUSE) << endl;
	out << trUtf8("  %1, %2\t    Don't force the recreation of objects. Avoids the usage of a DROP and CREATE commands to create a new version of the objects.").arg(short_opts[NO_FORCE_OBJ_RECREATION]).arg(NO_FORCE_OBJ_RECREATION) << endl;
	out << trUtf8("  %1, %2\t    Don't recreate the unmodifiable objects. These objects are the ones which can't be changed via ALTER command.").arg(short_opts[NO_UNMOD_OBJ_RECREATION]).arg(NO_UNMOD_OBJ_RECREATION) << endl;
	out << endl;

#ifndef Q_OS_MAC
	out << trUtf8("Miscellaneous options: ") << endl;
	out << trUtf8("  %1, %2 [ACTION]\t    Handles the file association to .dbm files. The ACTION can be [%3 | %4].").arg(short_opts[DBM_MIME_TYPE]).arg(DBM_MIME_TYPE).arg(INSTALL).arg(UNINSTALL) << endl;
	out << endl;
#endif

	out << trUtf8("** The diff process allows the usage of the following options related to import and export operations: ") << endl;
	out << "   " << QStringList({ trUtf8("* Export: "), IGNORE_DUPLICATES, IGNORE_ERROR_CODES, "\n  ", trUtf8("* Import: "), IMPORT_SYSTEM_OBJS, IMPORT_EXTENSION_OBJS, IGNORE_IMPORT_ERRORS, DEBUG_MODE }).join(" ") << endl;
	out << endl;
	out << trUtf8("** When running the diff using two databases (%1 and %2) there's the need to specify two connections/aliases. ").arg(INPUT_DB).arg(COMPARE_TO) << endl;
	out << trUtf8("   If only one connection is set it will be used to import the input database as well to retrieve database used in the comparison.") << endl;
	out << trUtf8("   A second connection can be specified by appending a 1 on any connection configuration parameter listed above.") << endl;
	out << endl;
}

void PgModelerCLI::parseOptions(attribs_map &opts)
{
	//Loading connections
	if(opts.count(LIST_CONNS) || opts.count(EXPORT_TO_DBMS) || opts.count(IMPORT_DB) || opts.count(DIFF))
	{
		conn_conf.loadConfiguration();
		conn_conf.getConnections(connections, false);
	}
	//Loading general and relationship settings when exporting to image formats
	else if(opts.count(EXPORT_TO_PNG) || opts.count(EXPORT_TO_SVG))
	{
		general_conf.loadConfiguration();
		rel_conf.loadConfiguration();
	}

	if(opts.empty() || opts.count(HELP))
		showMenu();
	//Listing connections
	else if(opts.count(LIST_CONNS))
	{
		map<QString, Connection *>::iterator itr=connections.begin();

		if(connections.empty())
			out << endl <<  trUtf8("There are no connections configured.") << endl << endl;
		else
		{
			unsigned id=0;

			out << endl << trUtf8("Available connections (alias : connection string)") << endl;
			while(itr != connections.end())
			{
				out << QString("[") << id++ <<  QString("] ") << itr->first << QString(" : ") <<
							 itr->second->getConnectionString().replace(PASSWORD_REGEXP, PASSWORD_PLACEHOLDER) << endl;

				itr++;
			}
			out << endl;
		}
	}
	else
	{
		int mode_cnt=0, other_modes_cnt=0;
		bool fix_model=(opts.count(FIX_MODEL) > 0), upd_mime=(opts.count(DBM_MIME_TYPE) > 0),
				import_db=(opts.count(IMPORT_DB) > 0), diff=(opts.count(DIFF) > 0);

		//Checking if multiples export modes were specified
		mode_cnt+=opts.count(EXPORT_TO_FILE);
		mode_cnt+=opts.count(EXPORT_TO_PNG);
		mode_cnt+=opts.count(EXPORT_TO_SVG);
		mode_cnt+=opts.count(EXPORT_TO_DBMS);

		other_modes_cnt+=opts.count(FIX_MODEL);
		other_modes_cnt+=opts.count(IMPORT_DB);
		other_modes_cnt+=opts.count(DIFF);
		other_modes_cnt+=opts.count(DBM_MIME_TYPE);

		if(opts.count(ZOOM_FACTOR))
			zoom=opts[ZOOM_FACTOR].toDouble()/static_cast<double>(100);

		if(other_modes_cnt==0 && mode_cnt==0)
			throw Exception(trUtf8("No operation mode was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if((mode_cnt > 0 && (fix_model || upd_mime || import_db || diff)) || (mode_cnt==0 && other_modes_cnt > 1))
			throw Exception(trUtf8("Export, fix model, import database, diff and update mime operations can't be used at the same time!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!fix_model && !upd_mime && mode_cnt > 1)
			throw Exception(trUtf8("Multiple export mode was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!upd_mime && !import_db && !diff && opts[INPUT].isEmpty())
			throw Exception(trUtf8("No input file was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(import_db && opts[INPUT_DB].isEmpty())
			throw Exception(trUtf8("No input database was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(!opts.count(EXPORT_TO_DBMS) && !upd_mime && !diff && opts[OUTPUT].isEmpty())
			throw Exception(trUtf8("No output file was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!opts.count(EXPORT_TO_DBMS) && !upd_mime && !import_db &&
			 !opts[INPUT].isEmpty() && !opts[OUTPUT].isEmpty() &&
			 QFileInfo(opts[INPUT]).absoluteFilePath() == QFileInfo(opts[OUTPUT]).absoluteFilePath())
			throw Exception(trUtf8("Input file must be different from output!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(opts.count(EXPORT_TO_DBMS) && !opts.count(CONN_ALIAS) &&
			 (!opts.count(HOST) || !opts.count(USER) || !opts.count(PASSWD) || !opts.count(INITIAL_DB)) )
			throw Exception(trUtf8("Incomplete connection information!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(opts.count(EXPORT_TO_PNG) && (zoom < ModelWidget::MINIMUM_ZOOM || zoom > ModelWidget::MAXIMUM_ZOOM))
			throw Exception(trUtf8("Invalid zoom specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(upd_mime && opts[DBM_MIME_TYPE]!=INSTALL && opts[DBM_MIME_TYPE]!=UNINSTALL)
			throw Exception(trUtf8("Invalid action specified to update mime option!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			
		if(opts.count(DIFF))
		{
			if(opts[INPUT].isEmpty() && opts[INPUT_DB].isEmpty())
				throw Exception(trUtf8("No input file or database was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts[INPUT].isEmpty() && !opts[INPUT_DB].isEmpty())
				throw Exception(trUtf8("The input file and database can't be used at the same time!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts.count(COMPARE_TO))
				throw Exception(trUtf8("No database to be compared was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts.count(SAVE_DIFF) && !opts.count(APPLY_DIFF))
				throw Exception(trUtf8("No diff action (save or apply) was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(opts.count(SAVE_DIFF) && opts[OUTPUT].isEmpty())
				throw Exception(trUtf8("No output file for the diff code was specified!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		
		//Converting input and output files to absolute paths to avoid that they are read/written on the app's working dir
		if(!opts[INPUT].isEmpty())
			opts[INPUT]=QFileInfo(opts[INPUT]).absoluteFilePath();

		if(!opts[OUTPUT].isEmpty())
			opts[OUTPUT]=QFileInfo(opts[OUTPUT]).absoluteFilePath();

		parsed_opts=opts;
	}
}

int PgModelerCLI::exec(void)
{
	try
	{
		if(!parsed_opts.empty())
		{
			printMessage(QString("\npgModeler %1 %2").arg(GlobalAttributes::PgModelerVersion).arg(trUtf8(" command line interface.")));

			if(parsed_opts.count(FIX_MODEL))
				fixModel();
			else if(parsed_opts.count(DBM_MIME_TYPE))
				updateMimeType();
			else if(parsed_opts.count(IMPORT_DB))
				importDatabase();
			else if(parsed_opts.count(DIFF))
				diffModelDatabase();
			else
				exportModel();
		}

		return(0);
	}
	catch(Exception &e)
	{
		throw e;
	}
}

void PgModelerCLI::updateProgress(int progress, QString msg, ObjectType)
{
	if(progress > 0)
		out << QString("[%1%] ").arg(progress > 100 ? 100 : progress) << msg << endl;
	else
		out << msg << endl;
}

void PgModelerCLI::printIgnoredError(QString err_cod, QString err_msg, QString cmd)
{
	out << endl;
	out << trUtf8("** Error code `%1' found and ignored. Proceeding with export.").arg(err_cod) << endl;
	out << trUtf8("** Command: %1").arg(cmd) << endl;
	out << err_msg << endl;
	out << endl;
}

void PgModelerCLI::handleObjectAddition(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	if(graph_obj)
	{
		ObjectType obj_type=graph_obj->getObjectType();
		QGraphicsItem *item=nullptr;

		switch(obj_type)
		{
			case ObjTable:
				item=new TableView(dynamic_cast<Table *>(graph_obj));
			break;

			case ObjView:
				item=new GraphicalView(dynamic_cast<View *>(graph_obj));
			break;

			case ObjRelationship:
			case ObjBaseRelationship:
				item=new RelationshipView(dynamic_cast<BaseRelationship *>(graph_obj)); break;
			break;

			case ObjSchema:
				item=new SchemaView(dynamic_cast<Schema *>(graph_obj)); break;
			break;

			default:
				item=new StyledTextboxView(dynamic_cast<Textbox *>(graph_obj)); break;
			break;
		}

		scene->addItem(item);

		if(obj_type==ObjTable || obj_type==ObjView)
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}


void PgModelerCLI::handleObjectRemoval(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	if(graph_obj)
	{
		scene->removeItem(dynamic_cast<QGraphicsItem *>(graph_obj->getReceiverObject()));

		//Updates the parent schema if the removed object were a table or view
		if(graph_obj->getSchema() &&
				(graph_obj->getObjectType()==ObjTable || graph_obj->getObjectType()==ObjView))
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}


void PgModelerCLI::extractObjectXML(void)
{
	QFile input;
	QString buf, lin, def_xml, end_tag;
	QTextStream ts;
	QRegExp regexp(QString("^(\\<\\?xml)(.)*(\\<%1)( )*").arg(ParsersAttributes::DB_MODEL)),
			default_obj=QRegExp(QString("(default)(\\-)(schema|owner|collation|tablespace)")),

			//[schema].[func_name](...OUT [type]...)
			func_signature=QRegExp(QString("(\")(.)+(\\.)(.)+(\\()(.)*(OUT )(.)+(\\))(\")")),

			//[,]OUT [schema].[type]
			out_param=QRegExp(QString("(,)?(OUT )([a-z]|[0-9]|(\\.)|(\\_)|(\\-)|( )|(\\[)|(\\])|(&quot;))+((\\()([0-9])+(\\)))?"));
	int start=-1, end=-1;
	bool open_tag=false, close_tag=false, is_rel=false, short_tag=false, end_extract_rel;

	printMessage(trUtf8("Extracting objects' XML..."));

	input.setFileName(parsed_opts[INPUT]);
	input.open(QFile::ReadOnly);

	if(!input.isOpen())
		throw Exception(Exception::getErrorMessage(FileDirectoryNotAccessed).arg(parsed_opts[INPUT]),
						FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	buf.append(input.readAll());
	input.close();

	//Check if the file contains a valid header (for .dbm file)
	start=regexp.indexIn(buf);

	if(start < 0)
		throw Exception(trUtf8("Invalid input file! It seems that is not a pgModeler generated model or the file is corrupted!"), Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		//Remove the header entry from buffer
		buf.remove(start, regexp.matchedLength()+1);
		buf.remove(0, buf.indexOf(QString("\n")));
		buf.remove(QString("<\\%1>").arg(ParsersAttributes::DB_MODEL));
		ts.setString(&buf);

		//Extracts the objects xml line by line
		while(!ts.atEnd())
		{
			lin=ts.readLine();

			/*  Special case for empty tags like <language />, they will be converted to
		  <language></language> in order to be correctly extracted further. Currently only language has this
		  behaviour, so additional object may be added in the future. */
			if(lin.contains(QString("<%1").arg(BaseObject::getSchemaName(ObjLanguage))))
			{
				lin=lin.simplified();

				if(lin.contains(QString("/>")))
					lin.replace(QString("/>"), QString("></%1>").arg(BaseObject::getSchemaName(ObjLanguage)));
			}
			/* Special case for function signatures. In previous releases, the function's signature was wrongly
		 including OUT parameters and according to docs they are not part of the signature, so it is needed
		 to remove them if the current line contains a valid signature with parameters. */
			else if(lin.contains(func_signature))
				lin.remove(out_param);


			if(is_rel && (((short_tag && lin.contains(QString("/>"))) ||
						   (lin.contains(QString("[a-z]+")) && !containsRelAttributes(lin)))))
				open_tag=close_tag=true;
			else if(lin.contains(default_obj))
			{
				lin.clear();
			}
			else
			{
				//If the line contains an objects open tag
				//if((lin.startsWith('<') || lin.startsWith(QString("\n<"))) && !open_tag)
				if(lin.contains(QRegExp("^(((\n)|(\t))*(<))")) && !open_tag)
				{
					//Check the flag indicating an open tag
					open_tag=true;

					start=lin.indexOf('<');
					end=lin.indexOf(' ');
					if(end < 0)	end=lin.indexOf('>');

					//Configures the end tag with the same word extracted from open tag
					end_tag=lin.mid(start, end-start+1).trimmed();
					end_tag.replace(QString("<"),QString("</"));

					if(!end_tag.endsWith('>'))
						end_tag+=QString(">");

					/* Checking if the line start a relationship. Relationships are treated
		  a little different because they can be empty <relationship attribs /> or
		  contain open and close tags <relationship attribs></relationship> */
					is_rel=lin.contains(ParsersAttributes::RELATIONSHIP);

					if(is_rel)
					{
						end_extract_rel=short_tag=false;

						while(!end_extract_rel && !ts.atEnd())
						{
							def_xml+=lin + QString("\n");
							lin=lin.trimmed();

							//Checking if the current line is the end of a short-tag relationship
							if(!short_tag && !lin.startsWith('<') && lin.endsWith(QString("/>")))
								short_tag=true;

							end_extract_rel=((!short_tag && lin.contains(end_tag)) || short_tag);

							if(!end_extract_rel)
								lin=ts.readLine();
						}

						close_tag=true;
					}
					else
						close_tag=lin.contains(end_tag);
				}
				else if(open_tag && lin.contains(end_tag))
					close_tag=true;
			}

			if(!is_rel && !lin.isEmpty())
				def_xml+=lin + QString("\n");
			else if(lin.isEmpty())
				def_xml+=QString("\n");

			//If the iteration reached the end of the object's definition
			if(open_tag && close_tag)
			{
				//Pushes the extracted definition to the list (only if not empty)
				if(def_xml!=QString("\n"))
					objs_xml.push_back(def_xml);

				def_xml.clear();
				open_tag=close_tag=is_rel=false;
			}
		}
	}
}

void PgModelerCLI::recreateObjects(void)
{
	QStringList fail_objs, constr, list;
	QString xml_def, aux_def, start_tag="<%1", end_tag="</%1>", aux_tag;
	BaseObject *object=nullptr;
	ObjectType obj_type=ObjBaseObject;
	vector<ObjectType> types={ ObjIndex, ObjTrigger, ObjRule };
	attribs_map attribs;
	bool use_fail_obj=false;
	unsigned tries=0, max_tries=parsed_opts[FIX_TRIES].toUInt();
	int start_pos=-1, end_pos=-1, len=0;

	printMessage(trUtf8("Recreating objects..."));

	if(max_tries==0)
		max_tries=1;

	model->createSystemObjects(false);

	while(!objs_xml.isEmpty())
	{
		//If there are failed objects and the flag is set
		if(use_fail_obj && !fail_objs.isEmpty())
		{
			xml_def=fail_objs.front();
			fail_objs.pop_front();
			use_fail_obj=false;
		}
		else
		{
			xml_def=objs_xml.front();
			objs_xml.pop_front();
			fixObjectAttributes(xml_def);
		}

		try
		{
			xmlparser->restartParser();
			xmlparser->loadXMLBuffer(xml_def);
			obj_type=BaseObject::getObjectType(xmlparser->getElementName());

			xmlparser->getElementAttributes(attribs);

			if(obj_type==ObjDatabase)
				model->configureDatabase(attribs);
			else
			{
				if(obj_type==ObjTable)
				{
					//Before create a table extract it's foreign keys
					list=extractForeignKeys(xml_def);

					/* If fks were extracted insert them on the main constraints list
		  and restarts the XMLParser with the modified buffer */
					if(!list.isEmpty())
					{
						constr.append(list);
						xmlparser->restartParser();
						xmlparser->loadXMLBuffer(xml_def);
					}
				}

				//Discarding fk relationships
				if(obj_type!=ObjRelationship ||
						(obj_type==ObjRelationship && !xml_def.contains(QString("\"%1\"").arg(ParsersAttributes::RELATIONSHIP_FK))))
				{
					object=model->createObject(obj_type);

					if(object)
					{
						if(!dynamic_cast<TableObject *>(object) && obj_type!=ObjRelationship && obj_type!=ObjBaseRelationship)
							model->addObject(object);
					}

					//For each sucessful created object the method will try to create a failed one
					use_fail_obj=(!fail_objs.isEmpty());
				}

				/* Additional step to extract indexes/triggers/rules from within tables/views
		   and putting their xml on the list of object to be created */
				if((obj_type==ObjTable || obj_type==ObjView) &&
						xml_def.contains(QRegExp("(<)(index|trigger|rule)")))
				{
					for(ObjectType type : types)
					{
						do
						{
							//Checking where the object starts and ends
							aux_tag=start_tag.arg(BaseObject::getSchemaName(type));
							start_pos=xml_def.indexOf(aux_tag);
							end_pos=(start_pos >=0 ? xml_def.indexOf(end_tag.arg(BaseObject::getSchemaName(type))) : -1);

							if(start_pos >=0 && end_pos >= 0)
							{
								//Extracts the xml code
								len=(end_pos - start_pos) + end_tag.arg(BaseObject::getSchemaName(type)).length() + 1;
								aux_def=xml_def.mid(start_pos, len);

								//Remove the code from original table's definition
								xml_def.remove(start_pos, len);

								//If the extract object doesn't contains the 'table=' attribute it'll be added.
								if(!aux_def.contains("table="))
								{
									aux_def.replace(aux_tag, QString("%1 table=\"%2\"").arg(aux_tag).arg(object->getName(true)));
									aux_def=SchemaParser::convertCharsToXMLEntities(aux_def);
								}

								objs_xml.push_back(aux_def);
							}
						}
						while(start_pos >= 0);
					}
				}
			}
		}
		catch(Exception &e)
		{
			if(obj_type!=ObjDatabase)
				fail_objs.push_back(xml_def);
			else
				throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		if(objs_xml.isEmpty() && (!fail_objs.isEmpty() || !constr.isEmpty()))
		{
			tries++;

			//If the maximum creation tries reaches the maximum value
			if(tries > max_tries)
			{
				//Outputs the code of the objects that wasn't created
				out << trUtf8("\n** Object(s) that couldn't fixed: ") << endl;
				while(!fail_objs.isEmpty())
				{
					out << fail_objs.front() << endl;
					fail_objs.pop_front();
				}

				break;
			}
			else
			{
				printMessage(trUtf8("WARNING: There are objects that maybe can't be fixed. Trying again... (tries %1/%2)").arg(tries).arg(max_tries));
				model->validateRelationships();
				objs_xml=fail_objs;
				objs_xml.append(constr);
				fail_objs.clear();
				constr.clear();
			}
		}
	}
}

void PgModelerCLI::fixObjectAttributes(QString &obj_xml)
{
	//Placing objects <index>, <rule>, <trigger> outside of <table>
	if(!obj_xml.startsWith(TAG_EXPR.arg(BaseObject::getSchemaName(ObjTablespace))) &&
			obj_xml.startsWith(TAG_EXPR.arg(BaseObject::getSchemaName(ObjTable))))
	{
		int start_idx=-1, end_idx=-1, len=0;
		ObjectType obj_types[3]={ ObjRule, ObjTrigger, ObjIndex  };
		QString  curr_tag, curr_end_tag, def, tab_name, sch_name,
				name_attr=QString("name=\""),
				sch_name_attr=TAG_EXPR.arg(BaseObject::getSchemaName(ObjSchema)) + QString(" ") + name_attr;

		//Extracting the table's name
		start_idx=obj_xml.indexOf(name_attr);
		end_idx=obj_xml.indexOf("\"", start_idx + name_attr.size());
		tab_name=obj_xml.mid(start_idx, end_idx - start_idx).remove(name_attr);

		//Extracting the table's schema name
		start_idx=obj_xml.indexOf(sch_name_attr);
		end_idx=obj_xml.indexOf('"', start_idx + sch_name_attr.size());
		sch_name=obj_xml.mid(start_idx, end_idx - start_idx).remove(sch_name_attr);

		//Configuring the table=[name] attribute to be included on rule objects
		tab_name=QString("table=\"%1.%2\"").arg(sch_name).arg(tab_name);

		for(unsigned idx=0; idx < 3; idx++)
		{
			curr_tag=TAG_EXPR.arg(BaseObject::getSchemaName(obj_types[idx]));
			curr_end_tag=END_TAG_EXPR.arg(BaseObject::getSchemaName(obj_types[idx])) + QString(">");
			start_idx=obj_xml.indexOf(curr_tag);

			while(start_idx >=0)
			{
				end_idx=obj_xml.indexOf(curr_end_tag);
				len=(end_idx - start_idx) + curr_end_tag.size();
				def=obj_xml.mid(start_idx, len) + QString("\n\n");
				obj_xml.remove(start_idx, len);

				//If the object is a rule include the table attribute
				if(def.startsWith(TAG_EXPR.arg(BaseObject::getSchemaName(ObjRule))))
				{
					start_idx=def.indexOf('>');
					def.replace(start_idx, 1, QString(" ") + tab_name + QString(">"));
				}

				start_idx=obj_xml.indexOf(curr_tag);

				if(!def.isEmpty())
					//Puts the object's defintion to the list in order to be evaluated in the main process
					objs_xml.push_back(def);
			}
		}
	}

	//Remove recheck attribute from <element> tags.
	if(obj_xml.contains(TAG_EXPR.arg(ParsersAttributes::ELEMENT)))
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("recheck"))));

	//Remove values greater-op, less-op, sort-op or sort2-op from ref-type attribute from <operator> tags.
	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjOperator))))
	{
		obj_xml.remove(QString("greater-op"));
		obj_xml.remove(QString("less-op"));
		obj_xml.remove(QString("sort-op"));
		obj_xml.remove(QString("sort2-op"));
	}

	//Replacing attribute owner by onwer-col for sequences
	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjSequence))))
		obj_xml.replace(ParsersAttributes::OWNER, ParsersAttributes::OWNER_COLUMN);

	//Remove sysid attribute from <role> tags.
	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjRole))))
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("sysid"))));

	//Replace <parameter> tag by <typeattrib> on <usertype> tags.
	if(obj_xml.contains(TAG_EXPR.arg(QString("usertype"))))
	{
		obj_xml.replace(TAG_EXPR.arg(ParsersAttributes::PARAMETER), TAG_EXPR.arg(ParsersAttributes::TYPE_ATTRIBUTE));
		obj_xml.replace(END_TAG_EXPR.arg(ParsersAttributes::PARAMETER), END_TAG_EXPR.arg(ParsersAttributes::TYPE_ATTRIBUTE));
	}

	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjRelationship))))
	{
		//Remove auto-sufix, src-sufix, dst-sufix, col-indexes, constr-indexes, attrib-indexes from <relationship> tags.
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("auto-sufix"))));
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("src-sufix"))));
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("dst-sufix"))));
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("col-indexes"))));
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("constr-indexes"))));
		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(QString("attrib-indexes"))));

		obj_xml.replace(QString("line-color"), ParsersAttributes::CUSTOM_COLOR);
	}

	//Renaming the tag <condition> to <predicate> on indexes
	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjIndex))))
	{
		obj_xml.replace(TAG_EXPR.arg(ParsersAttributes::CONDITION), TAG_EXPR.arg(ParsersAttributes::PREDICATE));
		obj_xml.replace(END_TAG_EXPR.arg(ParsersAttributes::CONDITION), END_TAG_EXPR.arg(ParsersAttributes::PREDICATE));
	}

	//Renaming the attribute default to default-value on domain
	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjDomain))))
		obj_xml.replace(ParsersAttributes::DEFAULT, ParsersAttributes::DEFAULT_VALUE);

	//Renaming the tag <grant> to <permission>
	if(obj_xml.contains(TAG_EXPR.arg(QString("grant"))))
	{
		obj_xml.replace(TAG_EXPR.arg(QString("grant")), TAG_EXPR.arg(BaseObject::getSchemaName(ObjPermission)));
		obj_xml.replace(END_TAG_EXPR.arg(QString("grant")), END_TAG_EXPR.arg(BaseObject::getSchemaName(ObjPermission)));
	}

	//Replace the constraint attribute and tag expression by constraint tag in <domain>.
	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjDomain))) &&
		 obj_xml.contains(TAG_EXPR.arg(ParsersAttributes::EXPRESSION)))
	{
		int start_idx=-1, end_idx=-1;
		QRegExp regexp = QRegExp(ATTRIBUTE_EXPR.arg(ParsersAttributes::CONSTRAINT));
		QString constr_name;

		regexp.indexIn(obj_xml);
		constr_name = regexp.capturedTexts().at(0);
		constr_name.remove(QString("%1=\"").arg(ParsersAttributes::CONSTRAINT));
		constr_name.remove(constr_name.length() - 1, 1);

		obj_xml.remove(QRegExp(ATTRIBUTE_EXPR.arg(ParsersAttributes::CONSTRAINT)));

		start_idx = obj_xml.indexOf(TAG_EXPR.arg(ParsersAttributes::EXPRESSION));
		obj_xml.insert(start_idx, QString("\n\t<constraint name=\"%1\" type=\"check\">\n\t\t").arg(constr_name));

		end_idx = obj_xml.indexOf(END_TAG_EXPR.arg(ParsersAttributes::EXPRESSION));
		obj_xml.insert(end_idx + END_TAG_EXPR.arg(ParsersAttributes::EXPRESSION).length() + 1, QString("\n\t</constraint>\n"));
	}

	//Fix the references to op. classes and families if needed
	fixOpClassesFamiliesReferences(obj_xml);
}

void PgModelerCLI::fixOpClassesFamiliesReferences(QString &obj_xml)
{
	ObjectType ref_obj_type;

	if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjIndex))) ||
			obj_xml.contains(QRegExp(QString("(%1)(.)+(type=)(\")(%2)(\")")
									 .arg(TAG_EXPR.arg(BaseObject::getSchemaName(ObjConstraint)))
									 .arg(ParsersAttributes::EX_CONSTR))))
		ref_obj_type=ObjOpClass;
	else if(obj_xml.contains(TAG_EXPR.arg(BaseObject::getSchemaName(ObjOpClass))))
		ref_obj_type=ObjOpFamily;
	else
		return;

	QString ref_obj_name=BaseObject::getSchemaName(ref_obj_type);
	if(!obj_xml.contains(TAG_EXPR.arg(ref_obj_name)))
		return;

	QString obj_name, aux_obj_name, signature=QString("%1 USING %2");
	QRegExp sign_regexp=QRegExp(ATTRIBUTE_EXPR.arg(QString("signature")));
	QStringList index_types;
	int pos=0;

	obj_xml.replace(TAG_EXPR.arg(ref_obj_name) + QString(" name="),
					TAG_EXPR.arg(ref_obj_name) + QString(" signature="));

	IndexingType::getTypes(index_types);

	do
	{
		pos=sign_regexp.indexIn(obj_xml, pos);

		if(pos >= 0)
		{
			//Extracting the signature attribute
			obj_name=obj_xml.mid(pos, sign_regexp.matchedLength());

			//Removing useless portions signature=" in order to retrive only the object's name
			obj_name.remove(QRegExp("(signature)( )*(=)"));
			obj_name.remove('"');

			//Transforming xml entity for quote into the char
			obj_name.replace(XMLParser::CharQuot, QString("\""));

			for(QString idx_type : index_types)
			{
				//Building a name by appe
				aux_obj_name=signature.arg(obj_name).arg(idx_type);

				if(model->getObjectIndex(aux_obj_name, ref_obj_type) >= 0)
				{
					//Replacing the old signature with the corrected form
					aux_obj_name.replace(QString("\""), XMLParser::CharQuot);
					obj_xml.replace(pos, sign_regexp.matchedLength(), QString("signature=\"%1\"").arg(aux_obj_name));
					break;
				}
			}

			pos+=sign_regexp.matchedLength();
		}
	}
	while(pos >= 0);
}

void PgModelerCLI::fixModel(void)
{
	printMessage(trUtf8("Starting model fixing..."));
	printMessage(trUtf8("Loading input file: %1").arg(parsed_opts[INPUT]));
	printMessage(trUtf8("Fixed model file: %1").arg(parsed_opts[OUTPUT]));

	extractObjectXML();
	recreateObjects();
	model->updateTablesFKRelationships();
	model->saveModel(parsed_opts[OUTPUT], SchemaParser::XmlDefinition);

	printMessage(trUtf8("Model successfully fixed!"));
}

void PgModelerCLI::exportModel(void)
{
	printMessage(trUtf8("Starting model export..."));
	printMessage(trUtf8("Loading input file: %1").arg(parsed_opts[INPUT]));

	//Create the systems objects on model before loading it
	model->createSystemObjects(false);

	//Load the model file
	model->loadModel(parsed_opts[INPUT]);

	//Export to PNG
	if(parsed_opts.count(EXPORT_TO_PNG))
	{
		printMessage(trUtf8("Export to PNG image: %1").arg(parsed_opts[OUTPUT]));

		export_hlp.exportToPNG(scene, parsed_opts[OUTPUT], zoom,
								 parsed_opts.count(SHOW_GRID) > 0,
								 parsed_opts.count(SHOW_DELIMITERS) > 0,
								 parsed_opts.count(PAGE_BY_PAGE) > 0);
	}
	//Export to SVG
	else if(parsed_opts.count(EXPORT_TO_SVG))
	{
		printMessage(trUtf8("Export to SVG file: %1").arg(parsed_opts[OUTPUT]));

		export_hlp.exportToSVG(scene, parsed_opts[OUTPUT],
													 parsed_opts.count(SHOW_GRID) > 0,
													 parsed_opts.count(SHOW_DELIMITERS) > 0);
	}
	//Export to SQL file
	else if(parsed_opts.count(EXPORT_TO_FILE))
	{
		printMessage(trUtf8("Export to SQL script file: %1").arg(parsed_opts[OUTPUT]));

		export_hlp.exportToSQL(model, parsed_opts[OUTPUT], parsed_opts[PGSQL_VER]);
	}
	//Export to DBMS
	else
	{
		printMessage(trUtf8("Export to DBMS: %1").arg(connection.getConnectionString().replace(PASSWORD_REGEXP, PASSWORD_PLACEHOLDER)));

		if(parsed_opts.count(IGNORE_ERROR_CODES))
			export_hlp.setIgnoredErrors(parsed_opts[IGNORE_ERROR_CODES].split(','));

		export_hlp.exportToDBMS(model, connection, parsed_opts[PGSQL_VER],
								parsed_opts.count(IGNORE_DUPLICATES) > 0,
								parsed_opts.count(DROP_DATABASE) > 0,
								parsed_opts.count(DROP_OBJECTS) > 0,
								parsed_opts.count(SIMULATE) > 0,
								parsed_opts.count(USE_TMP_NAMES) > 0);
	}

	printMessage(trUtf8("Export successfully ended!\n"));
}

void PgModelerCLI::importDatabase(void)
{
	printMessage(trUtf8("Starting database import..."));
	printMessage(trUtf8("Input database: %1").arg(connection.getConnectionId(true, true)));

	ModelWidget *model_wgt = new ModelWidget;

	importDatabase(model_wgt->getDatabaseModel(), connection);
	model_wgt->rearrangeSchemasInGrid();

	printMessage(trUtf8("Saving the imported database to file..."));

	model_wgt->getDatabaseModel()->saveModel(parsed_opts[OUTPUT], SchemaParser::XmlDefinition);

	printMessage(trUtf8("Import successfully ended!\n"));

	delete(model_wgt);
}

void PgModelerCLI::importDatabase(DatabaseModel *model, Connection conn)
{
	try
	{
		map<ObjectType, vector<unsigned>> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;
		Catalog catalog;
		QString db_oid;

		catalog.setConnection(conn);

		//For diff we don't need the oids of all system objects
		catalog.setFilter(Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes |
											Catalog::ExclExtensionObjs | Catalog::ExclSystemObjs);

		catalog.getObjectsOIDs(obj_oids, col_oids, {{ParsersAttributes::FILTER_TABLE_TYPES, ParsersAttributes::_TRUE_}});

		db_oid = catalog.getObjectOID(conn.getConnectionParam(Connection::ParamDbName), ObjDatabase);
		obj_oids[ObjDatabase].push_back(db_oid.toUInt());

		catalog.closeConnection();

		import_hlp.setConnection(conn);
		import_hlp.setImportOptions(parsed_opts.count(IMPORT_SYSTEM_OBJS) > 0,
																parsed_opts.count(IMPORT_EXTENSION_OBJS) > 0,
																true,
																parsed_opts.count(IGNORE_IMPORT_ERRORS) > 0,
																parsed_opts.count(DEBUG_MODE) > 0,
																!parsed_opts.count(DIFF), !parsed_opts.count(DIFF));

		model->createSystemObjects(true);
		import_hlp.setSelectedOIDs(model, obj_oids, col_oids);
		import_hlp.importDatabase();
		import_hlp.closeConnection();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PgModelerCLI::diffModelDatabase(void)
{
	DatabaseModel *model_aux = new DatabaseModel();
	QString dbname;

	printMessage(trUtf8("Starting diff process..."));

	if(!parsed_opts[INPUT].isEmpty())
		printMessage(trUtf8("Input model: %1").arg(parsed_opts[INPUT]));
	else
		printMessage(trUtf8("Input database: %1").arg(connection.getConnectionId(true, true)));

	dbname = extra_connection.getConnectionId(true, true);
	printMessage(trUtf8("Compare to: %1").arg(dbname));

	if(!parsed_opts[INPUT].isEmpty())
	{
		printMessage(trUtf8("Loading input model..."));
		model->createSystemObjects(false);
		model->loadModel(parsed_opts[INPUT]);
	}
	else
	{
		printMessage(trUtf8("Importing the database `%1'...").arg(connection.getConnectionId(true, true)));
		importDatabase(model, connection);
	}

	printMessage(trUtf8("Importing the database `%1'...").arg(dbname));
	importDatabase(model_aux, extra_connection);

	diff_hlp.setModels(model, model_aux);
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_KEEP_CLUSTER_OBJS, !parsed_opts.count(DROP_CLUSTER_OBJS));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_CASCADE_MODE, !parsed_opts.count(NO_CASCADE_DROP_TRUNC));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_TRUCANTE_TABLES, parsed_opts.count(TRUNC_ON_COLS_TYPE_CHANGE));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_FORCE_RECREATION, !parsed_opts.count(NO_FORCE_OBJ_RECREATION));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_RECREATE_UNCHANGEBLE, !parsed_opts.count(NO_UNMOD_OBJ_RECREATION));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_KEEP_OBJ_PERMS, !parsed_opts.count(REVOKE_PERMISSIONS));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_REUSE_SEQUENCES, !parsed_opts.count(NO_SEQUENCE_REUSE));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_PRESERVE_DB_NAME, !parsed_opts.count(RENAME_DB));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_DONT_DROP_MISSING_OBJS, !parsed_opts.count(DROP_MISSING_OBJS));
	diff_hlp.setDiffOption(ModelsDiffHelper::OPT_DROP_MISSING_COLS_CONSTR, !parsed_opts.count(FORCE_DROP_COLS_CONSTRS));

	if(!parsed_opts[PGSQL_VER].isEmpty())
		diff_hlp.setPgSQLVersion(parsed_opts[PGSQL_VER]);
	else
	{
		extra_connection.connect();
		diff_hlp.setPgSQLVersion(extra_connection.getPgSQLVersion(true));
		extra_connection.close();
	}

	printMessage(trUtf8("Comparing the generated models..."));
	diff_hlp.diffModels();

	if(diff_hlp.getDiffDefinition().isEmpty())
		printMessage(trUtf8("No differences were detected."));
	else
	{
		if(parsed_opts.count(SAVE_DIFF))
		{
			QFile output;

			printMessage(trUtf8("Saving diff to file `%1'").arg(parsed_opts[OUTPUT]));
			output.setFileName(parsed_opts[OUTPUT]);

			if(!output.open(QFile::WriteOnly))
				throw Exception(Exception::getErrorMessage(FileDirectoryNotWritten).arg(parsed_opts[OUTPUT]),
												FileDirectoryNotWritten, __PRETTY_FUNCTION__,__FILE__,__LINE__);
			output.write(diff_hlp.getDiffDefinition().toUtf8());
			output.close();
		}
		else
		{
			bool apply_diff = true;

			if(!parsed_opts.count(NO_DIFF_PREVIEW))
			{
				QString res, buff, line;
				QTextStream in(stdin), preview;

				buff += "\n** Press ENTER to scroll the preview **\n";
				buff += "\n### DIFF PREVIEW ###\n\n";
				buff += diff_hlp.getDiffDefinition();
				buff += "\n### END OF PREVIEW  ###\n\n";

				preview.setString(&buff, QIODevice::ReadOnly);

				while(!preview.atEnd())
				{
					line = preview.readLine();
					res.append(line + '\n');

					if(res.count(QChar('\n')) >= 30 || preview.atEnd())
					{
						out << res;
						out.flush();
						res.clear();

						if(!preview.atEnd())
							in.readLine();
					}
				}

				out << endl;
				out << trUtf8("** WARNING: You are about to apply the generated diff code to the server. Data can be lost in the process!") << endl;

				do
				{
					out << trUtf8("** Proceed with the diff applying? (yes/no) > ");
					out.flush();

					in.skipWhiteSpace();
					res = in.readLine();
				}
				while(res.toLower() != trUtf8("yes") && res.toLower() != trUtf8("no"));

				if(res.toLower() == trUtf8("no"))
				{
					apply_diff = false;
					printMessage(trUtf8("Diff code not applied to the server."));
				}
			}

			if(apply_diff)
			{
				printMessage(trUtf8("Applying diff to the database `%1'...").arg(dbname));
				export_hlp.setExportToDBMSParams(diff_hlp.getDiffDefinition(),
												 &extra_connection,
												 parsed_opts[COMPARE_TO], parsed_opts.count(IGNORE_DUPLICATES));

				if(parsed_opts.count(IGNORE_ERROR_CODES))
					export_hlp.setIgnoredErrors(parsed_opts[IGNORE_ERROR_CODES].split(','));

				export_hlp.exportToDBMS();
			}
		}
	}

	printMessage(trUtf8("Diff successfully ended!\n"));
}

void PgModelerCLI::updateMimeType(void)
{
#ifndef Q_OS_MAC
		printMessage(trUtf8("Starting mime update..."));

		handleMimeDatabase(parsed_opts[DBM_MIME_TYPE]==UNINSTALL);

		printMessage(trUtf8("Mime database successfully updated!\n"));
#endif
}

QStringList PgModelerCLI::extractForeignKeys(QString &obj_xml)
{
	QStringList constr_lst;
	int start=0, end=0, pos=0, count=0;
	QString start_tag=QString("<%1").arg(ParsersAttributes::CONSTRAINT),
			end_tag=QString("</%1").arg(ParsersAttributes::CONSTRAINT),
			constr;

	do
	{
		start=obj_xml.indexOf(start_tag, pos);
		end=obj_xml.indexOf(end_tag, start);

		if(start > 0 && end > 0)
		{
			count=(end - start) + end_tag.size() + 1;
			constr=obj_xml.mid(start, count);

			if(constr.contains(ParsersAttributes::FK_CONSTR))
			{
				obj_xml.remove(start, count);
				constr_lst.push_back(constr);
				pos=0;
			}
			else
				pos=end;
		}
		else
			break;
	}
	while(pos >= 0 && pos < obj_xml.size());

	return(constr_lst);
}

bool PgModelerCLI::containsRelAttributes(const QString &str)
{
	bool found=false;
	static vector<QString> attribs={ ParsersAttributes::RELATIONSHIP,
									 ParsersAttributes::TYPE, ParsersAttributes::SRC_REQUIRED, ParsersAttributes::DST_REQUIRED,
									 ParsersAttributes::SRC_TABLE, ParsersAttributes::DST_TABLE,	ParsersAttributes::POINTS,
									 ParsersAttributes::COLUMNS,	ParsersAttributes::COLUMN, ParsersAttributes::CONSTRAINT,
									 ParsersAttributes::LABEL, ParsersAttributes::LINE, ParsersAttributes::POSITION,
									 ParsersAttributes::IDENTIFIER, ParsersAttributes::DEFERRABLE, ParsersAttributes::DEFER_TYPE,
									 ParsersAttributes::TABLE_NAME, ParsersAttributes::SPECIAL_PK_COLS, ParsersAttributes::TABLE,
									 ParsersAttributes::ANCESTOR_TABLE, ParsersAttributes::COPY_OPTIONS, ParsersAttributes::COPY_MODE,
									 ParsersAttributes::SRC_COL_PATTERN, ParsersAttributes::DST_COL_PATTERN, ParsersAttributes::PK_PATTERN,
									 ParsersAttributes::UQ_PATTERN, ParsersAttributes::SRC_FK_PATTERN, ParsersAttributes::DST_FK_PATTERN };

	for(unsigned i=0; i < attribs.size() && !found; i++)
		found=str.contains(attribs[i]);

	return(found);
}

void PgModelerCLI::handleMimeDatabase(bool uninstall)
{
	SchemaParser schparser;
	QString msg_file_associated=trUtf8("Database model files (.dbm) are already associated to pgModeler!"),
			msg_no_association=trUtf8("There is no file association related to pgModeler and .dbm files!");

	printMessage(trUtf8("Mime database operation: %1").arg(uninstall ? QString("uninstall") : QString("install")));

#ifdef Q_OS_LINUX
	attribs_map attribs;
	QString str_aux,

			//Configures the path to the application logo
			exec_icon=QDir(GlobalAttributes::TmplConfigurationDir +
						   GlobalAttributes::DirSeparator + QString("pgmodeler_logo.png")).absolutePath(),

			//Configures the path to the document logo
			dbm_icon=QDir(GlobalAttributes::TmplConfigurationDir +
						  GlobalAttributes::DirSeparator + QString("pgmodeler_dbm.png")).absolutePath(),

			//Path to directory that register mime types
			mime_db_dir=QDir::homePath() + QString("/.local/share/mime"),

			//Path to the file that associates apps to mimetypes
			mimeapps=QDir::homePath() + QString("/.local/share/applications/mimeapps.list"),

			base_conf_dir=GlobalAttributes::TmplConfigurationDir + GlobalAttributes::DirSeparator +
						  GlobalAttributes::SchemasDir + GlobalAttributes::DirSeparator,

			//Files generated after update file association (application-dbm.xml and pgModeler.desktop)
			files[] = { QDir::homePath() + QString("/.local/share/applications/pgModeler.desktop"),
						mime_db_dir + QString("/packages/application-dbm.xml") },

			schemas[] = { base_conf_dir + QString("desktop") + GlobalAttributes::SchemaExt,
						  base_conf_dir + QString("application-dbm") + GlobalAttributes::SchemaExt };
	QByteArray buf, buf_aux;
	QFile out;

	//When installing, check if the necessary file exists. If exists, raises an error and abort.
	if(!uninstall && (QFileInfo(files[0]).exists() || QFileInfo(files[1]).exists()))
	{
		throw Exception(msg_file_associated, Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(uninstall && (!QFileInfo(files[0]).exists() && !QFileInfo(files[1]).exists()))
	{
		throw Exception(msg_no_association, Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(!uninstall)
	{
		QString startup_script=QString("%1/start-pgmodeler.sh")
							   .arg(QFileInfo(GlobalAttributes::PgModelerAppPath).absolutePath());

		attribs[ParsersAttributes::WORKING_DIR]=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
		attribs[ParsersAttributes::APPLICATION]=(QFileInfo(startup_script).exists() ?
													 startup_script : GlobalAttributes::PgModelerAppPath);
		attribs[ParsersAttributes::ICON]=exec_icon;
	}

	try
	{
		for(unsigned i=0; i < 2; i++)
		{
			if(uninstall)
			{
				if(!QFile(files[i]).remove())
					throw Exception(trUtf8("Can't erase the file %1! Check if the current user has permissions to delete it and if the file exists.").arg(files[i]),
									Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
			else
			{
				schparser.loadFile(schemas[i]);
				buf.append(schparser.getCodeDefinition(attribs));
				QDir(QString(".")).mkpath(QFileInfo(files[i]).absolutePath());

				out.setFileName(files[i]);
				out.open(QFile::WriteOnly);

				if(!out.isOpen())
					throw Exception(Exception::getErrorMessage(FileDirectoryNotWritten).arg(files[i]),
									FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				out.write(buf.data(), buf.size());
				out.close();
				buf.clear();
				attribs[ParsersAttributes::ICON]=dbm_icon;
			}
		}

		out.setFileName(mimeapps);

		//If the file mimeapps.list doesn't exists (generally in Ubuntu) creates a new one
		if(!uninstall && !QFileInfo(mimeapps).exists())
		{
			out.open(QFile::WriteOnly);
			out.write(QByteArray("[Added Associations]\napplication/dbm=pgModeler.desktop;\n"));
			out.write(QByteArray("\n[Default Applications]\napplication/dbm=pgModeler.desktop;\n"));
			out.close();
		}
		else
		{
			out.open(QFile::ReadOnly);

			if(!out.isOpen())
				throw Exception(Exception::getErrorMessage(FileDirectoryNotWritten).arg(mimeapps),
								FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			//Opens the mimeapps.list to add a entry linking pgModeler to .dbm files
			buf=out.readAll();
			out.close();

			QTextStream ts(&buf);
			while(!ts.atEnd())
			{
				//Remove any reference to application/dbm mime from file
				str_aux=ts.readLine();
				str_aux.replace(QRegExp(QString("application/dbm*"),Qt::CaseSensitive,QRegExp::Wildcard),QString());

				if(!str_aux.isEmpty())
				{
					//Updates the application/dbm mime association
					if(!uninstall && (str_aux.contains(QString("[Added Associations]")) ||
									  str_aux.contains(QString("[Default Applications]"))))
						str_aux.append(QString("\napplication/dbm=pgModeler.desktop;\n"));
					else
						str_aux+=QString("\n");

					if(str_aux.startsWith("[") && !str_aux.contains("Added Associations"))
						str_aux=QString("\n") + str_aux;

					buf_aux.append(str_aux);
				}
			}

			//Write a new copy of the mimeapps.list file
			out.open(QFile::Truncate | QFile::WriteOnly);
			out.write(buf_aux.data(), buf_aux.size());
			out.close();
		}

		//Update the mime database
		printMessage(trUtf8("Running update-mime-database command..."));

		QProcess::execute(QString("update-mime-database"), QStringList { mime_db_dir });
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
#else
#ifdef Q_OS_WIN

	//Checking if the .dbm registry key exists
	QSettings dbm_ext(QString("HKEY_CURRENT_USER\\Software\\Classes\\.dbm"), QSettings::NativeFormat);
	QString exe_path=QDir::toNativeSeparators(GlobalAttributes::PGMODELER_APP_PATH);

	//If there is no value assigned to .dbm/Default key and the user wants to uninstall file association, raises an error
	if(uninstall && dbm_ext.value(QString("Default")).toString().isEmpty())
		throw Exception(msg_no_association, ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!uninstall && !dbm_ext.value(QString("Default")).toString().isEmpty())
		throw Exception(msg_file_associated, ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		if(!uninstall)
			//Write the default value for .dbm registry key
			dbm_ext.setValue(QString("Default"), QString("dbm_auto_file"));
		else
			dbm_ext.remove(QString());

		dbm_ext.sync();
	}

	//Other registry keys values
	map<QString, QStringList> confs = {
		{ QString("\\HKEY_CURRENT_USER\\Software\\Classes\\dbm_auto_file"), { QString("FriendlyTypeName") , QString("pgModeler Database Model") } },
		{ QString("\\HKEY_CURRENT_USER\\Software\\Classes\\dbm_auto_file\\DefaultIcon"), { QString("Default") , QString("%1,1").arg(exe_path) } },
		{ QString("\\HKEY_CURRENT_USER\\Software\\Classes\\dbm_auto_file\\shell\\open\\command"), { QString("Default") , QString("\"%1\" \"%2\"").arg(exe_path).arg("%1") } },
		{ QString("\\HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.dbm"), { QString("OpenWithList/a"), QString("pgmodeler.exe"), QString("OpenWithList/MRUList"), QString("a")} }
	};

	map<QString, QStringList>::iterator itr;
	itr=confs.begin();

	//Iterates over the configuration map writing the other keys on registry
	while(itr!=confs.end())
	{
		QSettings s(itr->first, QSettings::NativeFormat);

		if(uninstall)
			s.remove(QString());
		else
		{
			for(int i=0; i < itr->second.size(); i+=2)
				s.setValue(itr->second[i], itr->second[i+1]);
		}

		s.sync();
		itr++;
	}

#endif
#endif
}
