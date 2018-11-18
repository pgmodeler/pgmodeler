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

QTextStream PgModelerCli::out(stdout);

const QRegExp PgModelerCli::PasswordRegExp=QRegExp("(password)(=)(.)*( )");
const QString PgModelerCli::PasswordPlaceholder=QString("password=******");

const QString PgModelerCli::Input=QString("--input");
const QString PgModelerCli::Output=QString("--output");
const QString PgModelerCli::InputDb=QString("--input-db");
const QString PgModelerCli::ExportToFile=QString("--export-to-file");
const QString PgModelerCli::ExportToPng=QString("--export-to-png");
const QString PgModelerCli::ExportToSvg=QString("--export-to-svg");
const QString PgModelerCli::ExportToDbms=QString("--export-to-dbms");
const QString PgModelerCli::ImportDb=QString("--import-db");
const QString PgModelerCli::Diff=QString("--diff");
const QString PgModelerCli::DropDatabase=QString("--drop-database");
const QString PgModelerCli::DropObjects=QString("--drop-objects");
const QString PgModelerCli::PgSqlVer=QString("--pgsql-ver");
const QString PgModelerCli::Help=QString("--help");
const QString PgModelerCli::ShowGrid=QString("--show-grid");
const QString PgModelerCli::ShowDelimiters=QString("--show-delimiters");
const QString PgModelerCli::PageByPage=QString("--page-by-page");
const QString PgModelerCli::IgnoreDuplicates=QString("--ignore-duplicates");
const QString PgModelerCli::IgnoreErrorCodes=QString("--ignore-error-codes");
const QString PgModelerCli::ConnAlias=QString("--conn-alias");
const QString PgModelerCli::Host=QString("--host");
const QString PgModelerCli::Port=QString("--port");
const QString PgModelerCli::User=QString("--user");
const QString PgModelerCli::Passwd=QString("--passwd");
const QString PgModelerCli::InitialDb=QString("--initial-db");
const QString PgModelerCli::Silent=QString("--silent");
const QString PgModelerCli::ListConns=QString("--list-conns");
const QString PgModelerCli::Simulate=QString("--simulate");
const QString PgModelerCli::FixModel=QString("--fix-model");
const QString PgModelerCli::FixTries=QString("--fix-tries");
const QString PgModelerCli::ZoomFactor=QString("--zoom");
const QString PgModelerCli::UseTmpNames=QString("--use-tmp-names");
const QString PgModelerCli::DbmMimeType=QString("--dbm-mime-type");
const QString PgModelerCli::Install=QString("install");
const QString PgModelerCli::Uninstall=QString("uninstall");
const QString PgModelerCli::IgnoreImportErrors=QString("--ignore-errors");
const QString PgModelerCli::ImportSystemObjs=QString("--import-sys-objs");
const QString PgModelerCli::ImportExtensionObjs=QString("--import-ext-objs");
const QString PgModelerCli::DebugMode=QString("--debug-mode");
const QString PgModelerCli::CompareTo=QString("--compare-to");
const QString PgModelerCli::SaveDiff=QString("--save-diff");
const QString PgModelerCli::ApplyDiff=QString("--apply-diff");
const QString PgModelerCli::NoDiffPreview=QString("--no-diff-preview");
const QString PgModelerCli::DropClusterObjs=QString("--drop-cluster-objs");
const QString PgModelerCli::RevokePermissions=QString("--revoke-perms");
const QString PgModelerCli::DropMissingObjs=QString("--drop-missing");
const QString PgModelerCli::ForceDropColsConstrs=QString("--force-drop-cols");
const QString PgModelerCli::RenameDb=QString("--rename-db");
const QString PgModelerCli::TruncOnColsTypeChange=QString("--trunc-type-change");
const QString PgModelerCli::NoSequenceReuse=QString("--no-sequence-reuse");
const QString PgModelerCli::NoCascadeDropTrunc=QString("--no-cascade");
const QString PgModelerCli::NoForceObjRecreation=QString("--no-force-recreation");
const QString PgModelerCli::NoUnmodObjRecreation=QString("--no-unmod-recreation");

const QString PgModelerCli::TagExpr=QString("<%1");
const QString PgModelerCli::EndTagExpr=QString("</%1");
const QString PgModelerCli::AttributeExpr=QString("(%1)( )*(=)(\")(\\w|\\d|,|\\.|\\&|\\;)+(\")");

PgModelerCli::PgModelerCli(int argc, char **argv) :  QApplication(argc, argv)
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
						throw Exception(trUtf8("Unrecognized option '%1'.").arg(op), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Raises an error if the value is empty and the option accepts a value
					if(accepts_val && value.isEmpty())
						throw Exception(trUtf8("Value not specified for option '%1'.").arg(op), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					else if(!accepts_val && !value.isEmpty())
						throw Exception(trUtf8("Option '%1' does not accept values.").arg(op), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
			silent_mode=(parsed_opts.count(Silent));

			//If the export is to png or svg loads additional configurations
			if(parsed_opts.count(ExportToPng) || parsed_opts.count(ExportToSvg) || parsed_opts.count(ImportDb))
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

			if(parsed_opts.count(ExportToDbms) || parsed_opts.count(ImportDb) || parsed_opts.count(Diff))
			{
				configureConnection(false);

				//Replacing the initial db parameter for the input database when reverse engineering
				if((parsed_opts.count(ImportDb) || parsed_opts.count(Diff)) && !parsed_opts[InputDb].isEmpty())
					connection.setConnectionParam(Connection::ParamDbName, parsed_opts[InputDb]);
			}

			if(parsed_opts.count(Diff))
			{
				configureConnection(true);

				if(!extra_connection.isConfigured())
					extra_connection = connection;

				extra_connection.setConnectionParam(Connection::ParamDbName, parsed_opts[CompareTo]);
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

PgModelerCli::~PgModelerCli(void)
{
	if(scene) delete(scene);
	delete(model);
}

void PgModelerCli::printMessage(const QString &msg)
{
	if(!silent_mode) out << msg << endl;
}

void PgModelerCli::configureConnection(bool extra_conn)
{
	QString chr = (extra_conn ? "1" : "");
	Connection *conn = (extra_conn ? &extra_connection : &connection);

	//Getting the connection using its alias
	if(parsed_opts.count(ConnAlias + chr))
	{
		if(!connections.count(parsed_opts[ConnAlias + chr]))
			throw Exception(trUtf8("Connection aliased as '%1' was not found in the configuration file.").arg(parsed_opts[ConnAlias + chr]),
							ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		//Make a copy of the named connection
		*conn = (*connections[parsed_opts[ConnAlias + chr]]);
	}
	else
	{
		conn->setConnectionParam(Connection::ParamServerFqdn, parsed_opts[Host + chr]);
		conn->setConnectionParam(Connection::ParamUser, parsed_opts[User + chr]);
		conn->setConnectionParam(Connection::ParamPort, parsed_opts[Port + chr]);
		conn->setConnectionParam(Connection::ParamPassword, parsed_opts[Passwd + chr]);
		conn->setConnectionParam(Connection::ParamDbName, parsed_opts[InitialDb + chr]);
	}
}

void PgModelerCli::initializeOptions(void)
{
	long_opts[Input]=true;
	long_opts[Output]=true;
	long_opts[InputDb]=true;
	long_opts[ExportToFile]=false;
	long_opts[ExportToPng]=false;
	long_opts[ExportToSvg]=false;
	long_opts[ExportToDbms]=false;
	long_opts[ImportDb]=false;
	long_opts[Diff]=false;
	long_opts[DropDatabase]=false;
	long_opts[DropObjects]=false;
	long_opts[PgSqlVer]=true;
	long_opts[Help]=false;
	long_opts[ShowGrid]=false;
	long_opts[ShowDelimiters]=false;
	long_opts[PageByPage]=false;
	long_opts[IgnoreDuplicates]=false;
	long_opts[IgnoreErrorCodes]=true;
	long_opts[ConnAlias]=true;
	long_opts[Host]=true;
	long_opts[Port]=true;
	long_opts[User]=true;
	long_opts[Passwd]=true;
	long_opts[InitialDb]=true;
	long_opts[ListConns]=false;
	long_opts[Simulate]=false;
	long_opts[FixModel]=false;
	long_opts[FixTries]=true;
	long_opts[ZoomFactor]=true;
	long_opts[UseTmpNames]=false;
	long_opts[DbmMimeType]=true;
	long_opts[IgnoreImportErrors]=false;
	long_opts[ImportSystemObjs]=false;
	long_opts[ImportExtensionObjs]=false;
	long_opts[DebugMode]=false;
	long_opts[CompareTo]=true;
	long_opts[SaveDiff]=false;
	long_opts[ApplyDiff]=false;
	long_opts[NoDiffPreview]=false;
	long_opts[DropClusterObjs]=false;
	long_opts[RevokePermissions]=false;
	long_opts[DropMissingObjs]=false;
	long_opts[ForceDropColsConstrs]=false;
	long_opts[RenameDb]=false;
	long_opts[TruncOnColsTypeChange]=false;
	long_opts[NoSequenceReuse]=false;
	long_opts[NoCascadeDropTrunc]=false;
	long_opts[NoForceObjRecreation]=false;
	long_opts[NoUnmodObjRecreation]=false;

	short_opts[Input]=QString("-if");
	short_opts[Output]=QString("-of");
	short_opts[InputDb]=QString("-id");
	short_opts[ExportToFile]=QString("-ef");
	short_opts[ExportToPng]=QString("-ep");
	short_opts[ExportToSvg]=QString("-es");
	short_opts[ExportToDbms]=QString("-ed");
	short_opts[ImportDb]=QString("-im");
	short_opts[Diff]=QString("-df");
	short_opts[DropDatabase]=QString("-dd");
	short_opts[DropObjects]=QString("-do");
	short_opts[PgSqlVer]=QString("-v");
	short_opts[Help]=QString("-h");
	short_opts[ShowGrid]=QString("-sg");
	short_opts[ShowDelimiters]=QString("-sl");
	short_opts[PageByPage]=QString("-pp");
	short_opts[IgnoreDuplicates]=QString("-ir");
	short_opts[IgnoreErrorCodes]=QString("-ic");
	short_opts[ConnAlias]=QString("-ca");
	short_opts[Host]=QString("-H");
	short_opts[Port]=QString("-p");
	short_opts[User]=QString("-u");
	short_opts[Passwd]=QString("-w");
	short_opts[InitialDb]=QString("-D");
	short_opts[Silent]=QString("-s");
	short_opts[ListConns]=QString("-lc");
	short_opts[Simulate]=QString("-sm");
	short_opts[FixModel]=QString("-fm");
	short_opts[FixTries]=QString("-ft");
	short_opts[ZoomFactor]=QString("-zf");
	short_opts[UseTmpNames]=QString("-tn");
	short_opts[DbmMimeType]=QString("-mt");
	short_opts[IgnoreImportErrors]=QString("-ie");
	short_opts[ImportSystemObjs]=QString("-is");
	short_opts[ImportExtensionObjs]=QString("-ix");
	short_opts[DebugMode]=QString("-d");
	short_opts[CompareTo]=QString("-ct");
	short_opts[SaveDiff]=QString("-sd");
	short_opts[ApplyDiff]=QString("-ad");
	short_opts[NoDiffPreview]=QString("-np");
	short_opts[DropClusterObjs]=QString("-dc");
	short_opts[RevokePermissions]=QString("-rv");
	short_opts[DropMissingObjs]=QString("-dm");
	short_opts[ForceDropColsConstrs]=QString("-fd");
	short_opts[RenameDb]=QString("-rn");
	short_opts[TruncOnColsTypeChange]=QString("-tt");
	short_opts[NoSequenceReuse]=QString("-ns");
	short_opts[NoCascadeDropTrunc]=QString("-nd");
	short_opts[NoForceObjRecreation]=QString("-nf");
	short_opts[NoUnmodObjRecreation]=QString("-nu");
}

bool PgModelerCli::isOptionRecognized(QString &op, bool &accepts_val)
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

void PgModelerCli::showMenu(void)
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
	out << trUtf8("  %1, %2 [FILE]\t\t    Input model file (.dbm). This is mandatory for fix, export operations.").arg(short_opts[Input]).arg(Input) << endl;
	out << trUtf8("  %1, %2 [DBNAME]\t    Input database name. This is mandatory for import operation.").arg(short_opts[InputDb]).arg(InputDb) << endl;
	out << trUtf8("  %1, %2 [FILE]\t\t    Output file. This is mandatory for fixing model or exporting to file, png or svg.").arg(short_opts[Output]).arg(Output) << endl;
	out << trUtf8("  %1, %2\t\t    Try to fix the structure of the input model file in order to make it loadable again.").arg(short_opts[FixModel]).arg(FixModel) << endl;
	out << trUtf8("  %1, %2 [NUMBER]\t    Model fix tries. When reaching the maximum count the invalid objects will be discarded.").arg(short_opts[FixTries]).arg(FixTries) << endl;
	out << trUtf8("  %1, %2\t\t    Export the input model to a sql script file.").arg(short_opts[ExportToFile]).arg(ExportToFile)<< endl;
	out << trUtf8("  %1, %2\t\t    Export the input model to a png image.").arg(short_opts[ExportToPng]).arg(ExportToPng) << endl;
	out << trUtf8("  %1, %2\t\t    Export the input model to a svg file.").arg(short_opts[ExportToSvg]).arg(ExportToSvg) << endl;
	out << trUtf8("  %1, %2\t\t    Export the input model directly to a PostgreSQL server.").arg(short_opts[ExportToDbms]).arg(ExportToDbms) << endl;
	out << trUtf8("  %1, %2\t\t    Import a database to an output file.").arg(short_opts[ImportDb]).arg(ImportDb) << endl;
	out << trUtf8("  %1, %2\t\t\t    Compares a model and a database or two databases generating the SQL script to synch the latter in relation to the first.").arg(short_opts[Diff]).arg(Diff) << endl;
	out << trUtf8("  %1, %2\t\t    Force the PostgreSQL version of generated SQL code.").arg(short_opts[PgSqlVer]).arg(PgSqlVer) << endl;
	out << trUtf8("  %1, %2\t\t\t    Silent execution. Only critical messages and errors are shown during process.").arg(short_opts[Silent]).arg(Silent) << endl;
	out << trUtf8("  %1, %2\t\t\t    Show this help menu.").arg(short_opts[Help]).arg(Help) << endl;
	out << endl;
	out << trUtf8("Connection options: ") << endl;
	out << trUtf8("  %1, %2\t\t    List available connections in file %3.").arg(short_opts[ListConns]).arg(ListConns).arg(GlobalAttributes::ConnectionsConf + GlobalAttributes::ConfigurationExt) << endl;
	out << trUtf8("  %1, %2 [ALIAS]\t    Connection configuration alias to be used.").arg(short_opts[ConnAlias]).arg(ConnAlias) << endl;
	out << trUtf8("  %1, %2 [HOST]\t\t    PostgreSQL host in which a task will operate.").arg(short_opts[Host]).arg(Host) << endl;
	out << trUtf8("  %1, %2 [PORT]\t\t    PostgreSQL host listening port.").arg(short_opts[Port]).arg(Port) << endl;
	out << trUtf8("  %1, %2 [USER]\t\t    PostgreSQL username.").arg(short_opts[User]).arg(User) << endl;
	out << trUtf8("  %1, %2 [PASSWORD]\t    PostgreSQL user password.").arg(short_opts[Passwd]).arg(Passwd) << endl;
	out << trUtf8("  %1, %2 [DBNAME]\t    Connection's initial database.").arg(short_opts[InitialDb]).arg(InitialDb) << endl;
	out << endl;
	out << trUtf8("PNG and SVG export options: ") << endl;
	out << trUtf8("  %1, %2\t\t    Draws the grid in the exported image.").arg(short_opts[ShowGrid]).arg(ShowGrid) << endl;
	out << trUtf8("  %1, %2\t    Draws the page delimiters in the exported image.").arg(short_opts[ShowDelimiters]).arg(ShowDelimiters) << endl;
	out << trUtf8("  %1, %2\t\t    Each page will be exported in a separated png image. (Only for PNG images)").arg(short_opts[PageByPage]).arg(PageByPage) << endl;
	out << trUtf8("  %1, %2 [FACTOR]\t\t    Applies a zoom (in percent) before export to png image. Accepted zoom interval: %3-%4 (Only for PNG images)").arg(short_opts[ZoomFactor]).arg(ZoomFactor).arg(ModelWidget::MinimumZoom*100).arg(ModelWidget::MaximumZoom*100) << endl;
	out << endl;
	out << trUtf8("DBMS export options: ") << endl;
	out << trUtf8("  %1, %2\t    Ignores errors related to duplicated objects that eventually exist in the server.").arg(short_opts[IgnoreDuplicates]).arg(IgnoreDuplicates) << endl;
	out << trUtf8("  %1, %2 [CODES] Ignores additional errors by their codes. A comma-separated list of alphanumeric codes should be provided.").arg(short_opts[IgnoreErrorCodes]).arg(IgnoreErrorCodes) << endl;
	out << trUtf8("  %1, %2\t\t    Drop the database before execute a export process.").arg(short_opts[DropDatabase]).arg(DropDatabase) << endl;
	out << trUtf8("  %1, %2\t\t    Runs the DROP commands attached to SQL-enabled objects.").arg(short_opts[DropObjects]).arg(DropObjects) << endl;
	out << trUtf8("  %1, %2\t\t    Simulates an export process by executing all steps but undoing any modification in the end.").arg(short_opts[Simulate]).arg(Simulate) << endl;
	out << trUtf8("  %1, %2\t\t    Generates temporary names for database, roles and tablespaces when in simulation mode.").arg(short_opts[UseTmpNames]).arg(UseTmpNames) << endl;
	out << endl;
	out << trUtf8("Database import options: ") << endl;
	out << trUtf8("  %1, %2\t\t    Ignore all errors and try to create as many as possible objects.").arg(short_opts[IgnoreImportErrors]).arg(IgnoreImportErrors) << endl;
	out << trUtf8("  %1, %2\t    Import system built-in objects. This option causes the model bloating due to the importing of unneeded objects.").arg(short_opts[ImportSystemObjs]).arg(ImportSystemObjs) << endl;
	out << trUtf8("  %1, %2\t    Import extension objects. This option causes the model bloating due to the importing of unneeded objects.").arg(short_opts[ImportExtensionObjs]).arg(ImportExtensionObjs) << endl;
	out << trUtf8("  %1, %2\t\t    Run import in debug mode printing all queries executed in the server.").arg(short_opts[DebugMode]).arg(DebugMode) << endl;
	out << endl;
	out << trUtf8("Diff options: ") << endl;
	out << trUtf8("  %1, %2 [DBNAME]\t    The database used in the comparison. All the SQL code generated is applied to it.").arg(short_opts[CompareTo]).arg(CompareTo) << endl;
	out << trUtf8("  %1, %2\t\t    Save the generated diff code to output file.").arg(short_opts[SaveDiff]).arg(SaveDiff) << endl;
	out << trUtf8("  %1, %2\t\t    Apply the generated diff code on the database server.").arg(short_opts[ApplyDiff]).arg(ApplyDiff) << endl;
	out << trUtf8("  %1, %2\t    Don't preview the generated diff code when applying it to the server.").arg(short_opts[NoDiffPreview]).arg(NoDiffPreview) << endl;
	out << trUtf8("  %1, %2\t    Drop cluster level objects like roles and tablespaces.").arg(short_opts[DropClusterObjs]).arg(DropClusterObjs) << endl;
	out << trUtf8("  %1, %2\t\t    Revoke permissions already set on the database. New permissions configured in the input model are still applied.").arg(short_opts[RevokePermissions]).arg(RevokePermissions) << endl;
	out << trUtf8("  %1, %2\t\t    Drop missing objects. Generates DROP commands for objects that are present in the input model but not in the compared database.").arg(short_opts[DropMissingObjs]).arg(DropMissingObjs) << endl;
	out << trUtf8("  %1, %2\t    Force the drop of missing columns and constraints. Causes only columns and constraints to be dropped, other missing objects aren't removed.").arg(short_opts[ForceDropColsConstrs]).arg(ForceDropColsConstrs) << endl;
	out << trUtf8("  %1, %2\t\t    Rename the destination database when the names of the involved databases are different.").arg(short_opts[RenameDb]).arg(RenameDb) << endl;
	out << trUtf8("  %1, %2\t\t    Don't drop or truncate objects in cascade mode.").arg(short_opts[NoCascadeDropTrunc]).arg(NoCascadeDropTrunc) << endl;
	out << trUtf8("  %1, %2\t    Truncate tables prior to alter columns. Avoids errors related to type casting when the new type of a column isn't compatible to the old one.").arg(short_opts[TruncOnColsTypeChange]).arg(TruncOnColsTypeChange) << endl;
	out << trUtf8("  %1, %2\t    Don't reuse sequences on serial columns. Drop the old sequence assigned to a serial column and creates a new one.").arg(short_opts[NoSequenceReuse]).arg(NoSequenceReuse) << endl;
	out << trUtf8("  %1, %2\t    Don't force the recreation of objects. Avoids the usage of a DROP and CREATE commands to create a new version of the objects.").arg(short_opts[NoForceObjRecreation]).arg(NoForceObjRecreation) << endl;
	out << trUtf8("  %1, %2\t    Don't recreate the unmodifiable objects. These objects are the ones which can't be changed via ALTER command.").arg(short_opts[NoUnmodObjRecreation]).arg(NoUnmodObjRecreation) << endl;
	out << endl;

#ifndef Q_OS_MAC
	out << trUtf8("Miscellaneous options: ") << endl;
	out << trUtf8("  %1, %2 [ACTION]\t    Handles the file association to .dbm files. The ACTION can be [%3 | %4].").arg(short_opts[DbmMimeType]).arg(DbmMimeType).arg(Install).arg(Uninstall) << endl;
	out << endl;
#endif

	out << trUtf8("** The diff process allows the usage of the following options related to import and export operations: ") << endl;
	out << "   " << QStringList({ trUtf8("* Export: "), IgnoreDuplicates, IgnoreErrorCodes, "\n  ", trUtf8("* Import: "), ImportSystemObjs, ImportExtensionObjs, IgnoreImportErrors, DebugMode }).join(" ") << endl;
	out << endl;
	out << trUtf8("** When running the diff using two databases (%1 and %2) there's the need to specify two connections/aliases. ").arg(InputDb).arg(CompareTo) << endl;
	out << trUtf8("   If only one connection is set it will be used to import the input database as well to retrieve database used in the comparison.") << endl;
	out << trUtf8("   A second connection can be specified by appending a 1 on any connection configuration parameter listed above.") << endl;
	out << endl;
}

void PgModelerCli::parseOptions(attribs_map &opts)
{
	//Loading connections
	if(opts.count(ListConns) || opts.count(ExportToDbms) || opts.count(ImportDb) || opts.count(Diff))
	{
		conn_conf.loadConfiguration();
		conn_conf.getConnections(connections, false);
	}
	//Loading general and relationship settings when exporting to image formats
	else if(opts.count(ExportToPng) || opts.count(ExportToSvg))
	{
		general_conf.loadConfiguration();
		rel_conf.loadConfiguration();
	}

	if(opts.empty() || opts.count(Help))
		showMenu();
	//Listing connections
	else if(opts.count(ListConns))
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
							 itr->second->getConnectionString().replace(PasswordRegExp, PasswordPlaceholder) << endl;

				itr++;
			}
			out << endl;
		}
	}
	else
	{
		int mode_cnt=0, other_modes_cnt=0;
		bool fix_model=(opts.count(FixModel) > 0), upd_mime=(opts.count(DbmMimeType) > 0),
				import_db=(opts.count(ImportDb) > 0), diff=(opts.count(Diff) > 0);

		//Checking if multiples export modes were specified
		mode_cnt+=opts.count(ExportToFile);
		mode_cnt+=opts.count(ExportToPng);
		mode_cnt+=opts.count(ExportToSvg);
		mode_cnt+=opts.count(ExportToDbms);

		other_modes_cnt+=opts.count(FixModel);
		other_modes_cnt+=opts.count(ImportDb);
		other_modes_cnt+=opts.count(Diff);
		other_modes_cnt+=opts.count(DbmMimeType);

		if(opts.count(ZoomFactor))
			zoom=opts[ZoomFactor].toDouble()/static_cast<double>(100);

		if(other_modes_cnt==0 && mode_cnt==0)
			throw Exception(trUtf8("No operation mode was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if((mode_cnt > 0 && (fix_model || upd_mime || import_db || diff)) || (mode_cnt==0 && other_modes_cnt > 1))
			throw Exception(trUtf8("Export, fix model, import database, diff and update mime operations can't be used at the same time!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!fix_model && !upd_mime && mode_cnt > 1)
			throw Exception(trUtf8("Multiple export mode was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!upd_mime && !import_db && !diff && opts[Input].isEmpty())
			throw Exception(trUtf8("No input file was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(import_db && opts[InputDb].isEmpty())
			throw Exception(trUtf8("No input database was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(!opts.count(ExportToDbms) && !upd_mime && !diff && opts[Output].isEmpty())
			throw Exception(trUtf8("No output file was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!opts.count(ExportToDbms) && !upd_mime && !import_db &&
			 !opts[Input].isEmpty() && !opts[Output].isEmpty() &&
			 QFileInfo(opts[Input]).absoluteFilePath() == QFileInfo(opts[Output]).absoluteFilePath())
			throw Exception(trUtf8("Input file must be different from output!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(opts.count(ExportToDbms) && !opts.count(ConnAlias) &&
			 (!opts.count(Host) || !opts.count(User) || !opts.count(Passwd) || !opts.count(InitialDb)) )
			throw Exception(trUtf8("Incomplete connection information!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(opts.count(ExportToPng) && (zoom < ModelWidget::MinimumZoom || zoom > ModelWidget::MaximumZoom))
			throw Exception(trUtf8("Invalid zoom specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(upd_mime && opts[DbmMimeType]!=Install && opts[DbmMimeType]!=Uninstall)
			throw Exception(trUtf8("Invalid action specified to update mime option!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			
		if(opts.count(Diff))
		{
			if(opts[Input].isEmpty() && opts[InputDb].isEmpty())
				throw Exception(trUtf8("No input file or database was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts[Input].isEmpty() && !opts[InputDb].isEmpty())
				throw Exception(trUtf8("The input file and database can't be used at the same time!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts.count(CompareTo))
				throw Exception(trUtf8("No database to be compared was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts.count(SaveDiff) && !opts.count(ApplyDiff))
				throw Exception(trUtf8("No diff action (save or apply) was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(opts.count(SaveDiff) && opts[Output].isEmpty())
				throw Exception(trUtf8("No output file for the diff code was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}
		
		//Converting input and output files to absolute paths to avoid that they are read/written on the app's working dir
		if(!opts[Input].isEmpty())
			opts[Input]=QFileInfo(opts[Input]).absoluteFilePath();

		if(!opts[Output].isEmpty())
			opts[Output]=QFileInfo(opts[Output]).absoluteFilePath();

		parsed_opts=opts;
	}
}

int PgModelerCli::exec(void)
{
	try
	{
		if(!parsed_opts.empty())
		{
			printMessage(QString("\npgModeler %1 %2").arg(GlobalAttributes::PgModelerVersion).arg(trUtf8(" command line interface.")));

			if(parsed_opts.count(FixModel))
				fixModel();
			else if(parsed_opts.count(DbmMimeType))
				updateMimeType();
			else if(parsed_opts.count(ImportDb))
				importDatabase();
			else if(parsed_opts.count(Diff))
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

void PgModelerCli::updateProgress(int progress, QString msg, ObjectType)
{
	if(progress > 0)
		out << QString("[%1%] ").arg(progress > 100 ? 100 : progress) << msg << endl;
	else
		out << msg << endl;
}

void PgModelerCli::printIgnoredError(QString err_cod, QString err_msg, QString cmd)
{
	out << endl;
	out << trUtf8("** Error code `%1' found and ignored. Proceeding with export.").arg(err_cod) << endl;
	out << trUtf8("** Command: %1").arg(cmd) << endl;
	out << err_msg << endl;
	out << endl;
}

void PgModelerCli::handleObjectAddition(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	if(graph_obj)
	{
		ObjectType obj_type=graph_obj->getObjectType();
		QGraphicsItem *item=nullptr;

		switch(obj_type)
		{
			case ObjectType::Table:
				item=new TableView(dynamic_cast<Table *>(graph_obj));
			break;

			case ObjectType::View:
				item=new GraphicalView(dynamic_cast<View *>(graph_obj));
			break;

			case ObjectType::Relationship:
			case ObjectType::BaseRelationship:
				item=new RelationshipView(dynamic_cast<BaseRelationship *>(graph_obj)); break;
			break;

			case ObjectType::Schema:
				item=new SchemaView(dynamic_cast<Schema *>(graph_obj)); break;
			break;

			default:
				item=new StyledTextboxView(dynamic_cast<Textbox *>(graph_obj)); break;
			break;
		}

		scene->addItem(item);

		if(obj_type==ObjectType::Table || obj_type==ObjectType::View)
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}


void PgModelerCli::handleObjectRemoval(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	if(graph_obj)
	{
		scene->removeItem(dynamic_cast<QGraphicsItem *>(graph_obj->getReceiverObject()));

		//Updates the parent schema if the removed object were a table or view
		if(graph_obj->getSchema() &&
				(graph_obj->getObjectType()==ObjectType::Table || graph_obj->getObjectType()==ObjectType::View))
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}


void PgModelerCli::extractObjectXML(void)
{
	QFile input;
	QString buf, lin, def_xml, end_tag;
	QTextStream ts;
	QRegExp regexp(QString("^(\\<\\?xml)(.)*(\\<%1)( )*").arg(Attributes::DbModel)),
			default_obj=QRegExp(QString("(default)(\\-)(schema|owner|collation|tablespace)")),

			//[schema].[func_name](...OUT [type]...)
			func_signature=QRegExp(QString("(\")(.)+(\\.)(.)+(\\()(.)*(OUT )(.)+(\\))(\")")),

			//[,]OUT [schema].[type]
			out_param=QRegExp(QString("(,)?(OUT )([a-z]|[0-9]|(\\.)|(\\_)|(\\-)|( )|(\\[)|(\\])|(&quot;))+((\\()([0-9])+(\\)))?"));
	int start=-1, end=-1;
	bool open_tag=false, close_tag=false, is_rel=false, short_tag=false, end_extract_rel;

	printMessage(trUtf8("Extracting objects' XML..."));

	input.setFileName(parsed_opts[Input]);
	input.open(QFile::ReadOnly);

	if(!input.isOpen())
		throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(parsed_opts[Input]),
										ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	buf.append(input.readAll());
	input.close();

	//Check if the file contains a valid header (for .dbm file)
	start=regexp.indexIn(buf);

	if(start < 0)
		throw Exception(trUtf8("Invalid input file! It seems that is not a pgModeler generated model or the file is corrupted!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		//Remove the header entry from buffer
		buf.remove(start, regexp.matchedLength()+1);
		buf.remove(0, buf.indexOf(QString("\n")));
		buf.remove(QString("<\\%1>").arg(Attributes::DbModel));
		ts.setString(&buf);

		//Extracts the objects xml line by line
		while(!ts.atEnd())
		{
			lin=ts.readLine();

			/*  Special case for empty tags like <language />, they will be converted to
		  <language></language> in order to be correctly extracted further. Currently only language has this
		  behaviour, so additional object may be added in the future. */
			if(lin.contains(QString("<%1").arg(BaseObject::getSchemaName(ObjectType::Language))))
			{
				lin=lin.simplified();

				if(lin.contains(QString("/>")))
					lin.replace(QString("/>"), QString("></%1>").arg(BaseObject::getSchemaName(ObjectType::Language)));
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
					is_rel=lin.contains(Attributes::Relationship);

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

void PgModelerCli::recreateObjects(void)
{
	QStringList fail_objs, constr, list;
	QString xml_def, aux_def, start_tag="<%1", end_tag="</%1>", aux_tag;
	BaseObject *object=nullptr;
	ObjectType obj_type=ObjectType::BaseObject;
	vector<ObjectType> types={ ObjectType::Index, ObjectType::Trigger, ObjectType::Rule };
	attribs_map attribs;
	bool use_fail_obj=false;
	unsigned tries=0, max_tries=parsed_opts[FixTries].toUInt();
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

			if(obj_type==ObjectType::Database)
				model->configureDatabase(attribs);
			else
			{
				if(obj_type==ObjectType::Table)
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
				if(obj_type!=ObjectType::Relationship ||
						(obj_type==ObjectType::Relationship && !xml_def.contains(QString("\"%1\"").arg(Attributes::RelationshipFk))))
				{
					object=model->createObject(obj_type);

					if(object)
					{
						if(!dynamic_cast<TableObject *>(object) && obj_type!=ObjectType::Relationship && obj_type!=ObjectType::BaseRelationship)
							model->addObject(object);
					}

					//For each sucessful created object the method will try to create a failed one
					use_fail_obj=(!fail_objs.isEmpty());
				}

				/* Additional step to extract indexes/triggers/rules from within tables/views
		   and putting their xml on the list of object to be created */
				if((obj_type==ObjectType::Table || obj_type==ObjectType::View) &&
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
			if(obj_type!=ObjectType::Database)
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

void PgModelerCli::fixObjectAttributes(QString &obj_xml)
{
	//Placing objects <index>, <rule>, <trigger> outside of <table>
	if(!obj_xml.startsWith(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Tablespace))) &&
			obj_xml.startsWith(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Table))))
	{
		int start_idx=-1, end_idx=-1, len=0;
		ObjectType obj_types[3]={ ObjectType::Rule, ObjectType::Trigger, ObjectType::Index  };
		QString  curr_tag, curr_end_tag, def, tab_name, sch_name,
				name_attr=QString("name=\""),
				sch_name_attr=TagExpr.arg(BaseObject::getSchemaName(ObjectType::Schema)) + QString(" ") + name_attr;

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
			curr_tag=TagExpr.arg(BaseObject::getSchemaName(obj_types[idx]));
			curr_end_tag=EndTagExpr.arg(BaseObject::getSchemaName(obj_types[idx])) + QString(">");
			start_idx=obj_xml.indexOf(curr_tag);

			while(start_idx >=0)
			{
				end_idx=obj_xml.indexOf(curr_end_tag);
				len=(end_idx - start_idx) + curr_end_tag.size();
				def=obj_xml.mid(start_idx, len) + QString("\n\n");
				obj_xml.remove(start_idx, len);

				//If the object is a rule include the table attribute
				if(def.startsWith(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Rule))))
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
	if(obj_xml.contains(TagExpr.arg(Attributes::Element)))
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("recheck"))));

	//Remove values greater-op, less-op, sort-op or sort2-op from ref-type attribute from <operator> tags.
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Operator))))
	{
		obj_xml.remove(QString("greater-op"));
		obj_xml.remove(QString("less-op"));
		obj_xml.remove(QString("sort-op"));
		obj_xml.remove(QString("sort2-op"));
	}

	//Replacing attribute owner by onwer-col for sequences
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Sequence))))
		obj_xml.replace(Attributes::Owner, Attributes::OwnerColumn);

	//Remove sysid attribute from <role> tags.
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Role))))
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("sysid"))));

	//Replace <parameter> tag by <typeattrib> on <usertype> tags.
	if(obj_xml.contains(TagExpr.arg(QString("usertype"))))
	{
		obj_xml.replace(TagExpr.arg(Attributes::Parameter), TagExpr.arg(Attributes::TypeAttribute));
		obj_xml.replace(EndTagExpr.arg(Attributes::Parameter), EndTagExpr.arg(Attributes::TypeAttribute));
	}

	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Relationship))))
	{
		//Remove auto-sufix, src-sufix, dst-sufix, col-indexes, constr-indexes, attrib-indexes from <relationship> tags.
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("auto-sufix"))));
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("src-sufix"))));
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("dst-sufix"))));
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("col-indexes"))));
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("constr-indexes"))));
		obj_xml.remove(QRegExp(AttributeExpr.arg(QString("attrib-indexes"))));

		obj_xml.replace(QString("line-color"), Attributes::CustomColor);
	}

	//Renaming the tag <condition> to <predicate> on indexes
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Index))))
	{
		obj_xml.replace(TagExpr.arg(Attributes::Condition), TagExpr.arg(Attributes::Predicate));
		obj_xml.replace(EndTagExpr.arg(Attributes::Condition), EndTagExpr.arg(Attributes::Predicate));
	}

	//Renaming the attribute default to default-value on domain
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Domain))))
		obj_xml.replace(Attributes::Default, Attributes::DefaultValue);

	//Renaming the tag <grant> to <permission>
	if(obj_xml.contains(TagExpr.arg(QString("grant"))))
	{
		obj_xml.replace(TagExpr.arg(QString("grant")), TagExpr.arg(BaseObject::getSchemaName(ObjectType::Permission)));
		obj_xml.replace(EndTagExpr.arg(QString("grant")), EndTagExpr.arg(BaseObject::getSchemaName(ObjectType::Permission)));
	}

	//Replace the constraint attribute and tag expression by constraint tag in <domain>.
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Domain))) &&
		 obj_xml.contains(TagExpr.arg(Attributes::Expression)))
	{
		int start_idx=-1, end_idx=-1;
		QRegExp regexp = QRegExp(AttributeExpr.arg(Attributes::Constraint));
		QString constr_name;

		regexp.indexIn(obj_xml);
		constr_name = regexp.capturedTexts().at(0);
		constr_name.remove(QString("%1=\"").arg(Attributes::Constraint));
		constr_name.remove(constr_name.length() - 1, 1);

		obj_xml.remove(QRegExp(AttributeExpr.arg(Attributes::Constraint)));

		start_idx = obj_xml.indexOf(TagExpr.arg(Attributes::Expression));
		obj_xml.insert(start_idx, QString("\n\t<constraint name=\"%1\" type=\"check\">\n\t\t").arg(constr_name));

		end_idx = obj_xml.indexOf(EndTagExpr.arg(Attributes::Expression));
		obj_xml.insert(end_idx + EndTagExpr.arg(Attributes::Expression).length() + 1, QString("\n\t</constraint>\n"));
	}

	//Remove the deprecated attribute hide-ext-attribs from <table> and <views>
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Table))) ||
		 obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::View))))
	{
		obj_xml.replace(QRegExp(AttributeExpr.arg(Attributes::HideExtAttribs)), QString());
	}

	//Fix the references to op. classes and families if needed
	fixOpClassesFamiliesReferences(obj_xml);
}

void PgModelerCli::fixOpClassesFamiliesReferences(QString &obj_xml)
{
	ObjectType ref_obj_type;

	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Index))) ||
			obj_xml.contains(QRegExp(QString("(%1)(.)+(type=)(\")(%2)(\")")
									 .arg(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Constraint)))
									 .arg(Attributes::ExConstr))))
		ref_obj_type=ObjectType::OpClass;
	else if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::OpClass))))
		ref_obj_type=ObjectType::OpFamily;
	else
		return;

	QString ref_obj_name=BaseObject::getSchemaName(ref_obj_type);
	if(!obj_xml.contains(TagExpr.arg(ref_obj_name)))
		return;

	QString obj_name, aux_obj_name, signature=QString("%1 USING %2");
	QRegExp sign_regexp=QRegExp(AttributeExpr.arg(QString("signature")));
	QStringList index_types;
	int pos=0;

	obj_xml.replace(TagExpr.arg(ref_obj_name) + QString(" name="),
					TagExpr.arg(ref_obj_name) + QString(" signature="));

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
			obj_name.replace(XmlParser::CharQuot, QString("\""));

			for(QString idx_type : index_types)
			{
				//Building a name by appe
				aux_obj_name=signature.arg(obj_name).arg(idx_type);

				if(model->getObjectIndex(aux_obj_name, ref_obj_type) >= 0)
				{
					//Replacing the old signature with the corrected form
					aux_obj_name.replace(QString("\""), XmlParser::CharQuot);
					obj_xml.replace(pos, sign_regexp.matchedLength(), QString("signature=\"%1\"").arg(aux_obj_name));
					break;
				}
			}

			pos+=sign_regexp.matchedLength();
		}
	}
	while(pos >= 0);
}

void PgModelerCli::fixModel(void)
{
	printMessage(trUtf8("Starting model fixing..."));
	printMessage(trUtf8("Loading input file: %1").arg(parsed_opts[Input]));
	printMessage(trUtf8("Fixed model file: %1").arg(parsed_opts[Output]));

	extractObjectXML();
	recreateObjects();
	model->updateTablesFKRelationships();
	model->saveModel(parsed_opts[Output], SchemaParser::XmlDefinition);

	printMessage(trUtf8("Model successfully fixed!"));
}

void PgModelerCli::exportModel(void)
{
	printMessage(trUtf8("Starting model export..."));
	printMessage(trUtf8("Loading input file: %1").arg(parsed_opts[Input]));

	//Create the systems objects on model before loading it
	model->createSystemObjects(false);

	//Load the model file
	model->loadModel(parsed_opts[Input]);

	//Export to PNG
	if(parsed_opts.count(ExportToPng))
	{
		printMessage(trUtf8("Export to PNG image: %1").arg(parsed_opts[Output]));

		export_hlp.exportToPNG(scene, parsed_opts[Output], zoom,
								 parsed_opts.count(ShowGrid) > 0,
								 parsed_opts.count(ShowDelimiters) > 0,
								 parsed_opts.count(PageByPage) > 0);
	}
	//Export to SVG
	else if(parsed_opts.count(ExportToSvg))
	{
		printMessage(trUtf8("Export to SVG file: %1").arg(parsed_opts[Output]));

		export_hlp.exportToSVG(scene, parsed_opts[Output],
													 parsed_opts.count(ShowGrid) > 0,
													 parsed_opts.count(ShowDelimiters) > 0);
	}
	//Export to SQL file
	else if(parsed_opts.count(ExportToFile))
	{
		printMessage(trUtf8("Export to SQL script file: %1").arg(parsed_opts[Output]));

		export_hlp.exportToSQL(model, parsed_opts[Output], parsed_opts[PgSqlVer]);
	}
	//Export to DBMS
	else
	{
		printMessage(trUtf8("Export to DBMS: %1").arg(connection.getConnectionString().replace(PasswordRegExp, PasswordPlaceholder)));

		if(parsed_opts.count(IgnoreErrorCodes))
			export_hlp.setIgnoredErrors(parsed_opts[IgnoreErrorCodes].split(','));

		export_hlp.exportToDBMS(model, connection, parsed_opts[PgSqlVer],
								parsed_opts.count(IgnoreDuplicates) > 0,
								parsed_opts.count(DropDatabase) > 0,
								parsed_opts.count(DropObjects) > 0,
								parsed_opts.count(Simulate) > 0,
								parsed_opts.count(UseTmpNames) > 0);
	}

	printMessage(trUtf8("Export successfully ended!\n"));
}

void PgModelerCli::importDatabase(void)
{
	printMessage(trUtf8("Starting database import..."));
	printMessage(trUtf8("Input database: %1").arg(connection.getConnectionId(true, true)));

	ModelWidget *model_wgt = new ModelWidget;

	importDatabase(model_wgt->getDatabaseModel(), connection);
	model_wgt->rearrangeSchemasInGrid();

	printMessage(trUtf8("Saving the imported database to file..."));

	model_wgt->getDatabaseModel()->saveModel(parsed_opts[Output], SchemaParser::XmlDefinition);

	printMessage(trUtf8("Import successfully ended!\n"));

	delete(model_wgt);
}

void PgModelerCli::importDatabase(DatabaseModel *model, Connection conn)
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

		catalog.getObjectsOIDs(obj_oids, col_oids, {{Attributes::FilterTableTypes, Attributes::True}});

		db_oid = catalog.getObjectOID(conn.getConnectionParam(Connection::ParamDbName), ObjectType::Database);
		obj_oids[ObjectType::Database].push_back(db_oid.toUInt());

		catalog.closeConnection();

		import_hlp.setConnection(conn);
		import_hlp.setImportOptions(parsed_opts.count(ImportSystemObjs) > 0,
																parsed_opts.count(ImportExtensionObjs) > 0,
																true,
																parsed_opts.count(IgnoreImportErrors) > 0,
																parsed_opts.count(DebugMode) > 0,
																!parsed_opts.count(Diff), !parsed_opts.count(Diff));

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

void PgModelerCli::diffModelDatabase(void)
{
	DatabaseModel *model_aux = new DatabaseModel();
	QString dbname;

	printMessage(trUtf8("Starting diff process..."));

	if(!parsed_opts[Input].isEmpty())
		printMessage(trUtf8("Input model: %1").arg(parsed_opts[Input]));
	else
		printMessage(trUtf8("Input database: %1").arg(connection.getConnectionId(true, true)));

	dbname = extra_connection.getConnectionId(true, true);
	printMessage(trUtf8("Compare to: %1").arg(dbname));

	if(!parsed_opts[Input].isEmpty())
	{
		printMessage(trUtf8("Loading input model..."));
		model->createSystemObjects(false);
		model->loadModel(parsed_opts[Input]);
	}
	else
	{
		printMessage(trUtf8("Importing the database `%1'...").arg(connection.getConnectionId(true, true)));
		importDatabase(model, connection);
	}

	printMessage(trUtf8("Importing the database `%1'...").arg(dbname));
	importDatabase(model_aux, extra_connection);

	diff_hlp.setModels(model, model_aux);
	diff_hlp.setDiffOption(ModelsDiffHelper::OptKeepClusterObjs, !parsed_opts.count(DropClusterObjs));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptCascadeMode, !parsed_opts.count(NoCascadeDropTrunc));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptTruncateTables, parsed_opts.count(TruncOnColsTypeChange));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptForceRecreation, !parsed_opts.count(NoForceObjRecreation));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptRecreateUnchangeble, !parsed_opts.count(NoUnmodObjRecreation));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptKeepObjectPerms, !parsed_opts.count(RevokePermissions));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptReuseSequences, !parsed_opts.count(NoSequenceReuse));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptPreserveDbName, !parsed_opts.count(RenameDb));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptDontDropMissingObjs, !parsed_opts.count(DropMissingObjs));
	diff_hlp.setDiffOption(ModelsDiffHelper::OptDropMissingColsConstr, !parsed_opts.count(ForceDropColsConstrs));

	if(!parsed_opts[PgSqlVer].isEmpty())
		diff_hlp.setPgSQLVersion(parsed_opts[PgSqlVer]);
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
		if(parsed_opts.count(SaveDiff))
		{
			QFile output;

			printMessage(trUtf8("Saving diff to file `%1'").arg(parsed_opts[Output]));
			output.setFileName(parsed_opts[Output]);

			if(!output.open(QFile::WriteOnly))
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(parsed_opts[Output]),
												ErrorCode::FileDirectoryNotWritten, __PRETTY_FUNCTION__,__FILE__,__LINE__);
			output.write(diff_hlp.getDiffDefinition().toUtf8());
			output.close();
		}
		else
		{
			bool apply_diff = true;

			if(!parsed_opts.count(NoDiffPreview))
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
												 parsed_opts[CompareTo], parsed_opts.count(IgnoreDuplicates));

				if(parsed_opts.count(IgnoreErrorCodes))
					export_hlp.setIgnoredErrors(parsed_opts[IgnoreErrorCodes].split(','));

				export_hlp.exportToDBMS();
			}
		}
	}

	printMessage(trUtf8("Diff successfully ended!\n"));
}

void PgModelerCli::updateMimeType(void)
{
#ifndef Q_OS_MAC
		printMessage(trUtf8("Starting mime update..."));

		handleMimeDatabase(parsed_opts[DbmMimeType]==Uninstall);

		printMessage(trUtf8("Mime database successfully updated!\n"));
#endif
}

QStringList PgModelerCli::extractForeignKeys(QString &obj_xml)
{
	QStringList constr_lst;
	int start=0, end=0, pos=0, count=0;
	QString start_tag=QString("<%1").arg(Attributes::Constraint),
			end_tag=QString("</%1").arg(Attributes::Constraint),
			constr;

	do
	{
		start=obj_xml.indexOf(start_tag, pos);
		end=obj_xml.indexOf(end_tag, start);

		if(start > 0 && end > 0)
		{
			count=(end - start) + end_tag.size() + 1;
			constr=obj_xml.mid(start, count);

			if(constr.contains(Attributes::FkConstr))
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

bool PgModelerCli::containsRelAttributes(const QString &str)
{
	bool found=false;
	static vector<QString> attribs={ Attributes::Relationship,
									 Attributes::Type, Attributes::SrcRequired, Attributes::DstRequired,
									 Attributes::SrcTable, Attributes::DstTable,	Attributes::Points,
									 Attributes::Columns,	Attributes::Column, Attributes::Constraint,
									 Attributes::Label, Attributes::Line, Attributes::Position,
									 Attributes::Identifier, Attributes::Deferrable, Attributes::DeferType,
									 Attributes::TableName, Attributes::SpecialPkCols, Attributes::Table,
									 Attributes::AncestorTable, Attributes::CopyOptions, Attributes::CopyMode,
									 Attributes::SrcColPattern, Attributes::DstColPattern, Attributes::PkPattern,
									 Attributes::UqPattern, Attributes::SrcFkPattern, Attributes::DstFkPattern };

	for(unsigned i=0; i < attribs.size() && !found; i++)
		found=str.contains(attribs[i]);

	return(found);
}

void PgModelerCli::handleMimeDatabase(bool uninstall)
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
		throw Exception(msg_file_associated, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(uninstall && (!QFileInfo(files[0]).exists() && !QFileInfo(files[1]).exists()))
	{
		throw Exception(msg_no_association, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	else if(!uninstall)
	{
		QString startup_script=QString("%1/start-pgmodeler.sh")
							   .arg(QFileInfo(GlobalAttributes::PgModelerAppPath).absolutePath());

		attribs[Attributes::WorkingDir]=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
		attribs[Attributes::Application]=(QFileInfo(startup_script).exists() ?
													 startup_script : GlobalAttributes::PgModelerAppPath);
		attribs[Attributes::Icon]=exec_icon;
	}

	try
	{
		for(unsigned i=0; i < 2; i++)
		{
			if(uninstall)
			{
				if(!QFile(files[i]).remove())
					throw Exception(trUtf8("Can't erase the file %1! Check if the current user has permissions to delete it and if the file exists.").arg(files[i]),
													ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
			else
			{
				schparser.loadFile(schemas[i]);
				buf.append(schparser.getCodeDefinition(attribs));
				QDir(QString(".")).mkpath(QFileInfo(files[i]).absolutePath());

				out.setFileName(files[i]);
				out.open(QFile::WriteOnly);

				if(!out.isOpen())
					throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(files[i]),
													ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				out.write(buf.data(), buf.size());
				out.close();
				buf.clear();
				attribs[Attributes::Icon]=dbm_icon;
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
				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(mimeapps),
												ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
    QString exe_path=QDir::toNativeSeparators(GlobalAttributes::PgModelerAppPath);

	//If there is no value assigned to .dbm/Default key and the user wants to uninstall file association, raises an error
	if(uninstall && dbm_ext.value(QString("Default")).toString().isEmpty())
        throw Exception(msg_no_association, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!uninstall && !dbm_ext.value(QString("Default")).toString().isEmpty())
        throw Exception(msg_file_associated, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
