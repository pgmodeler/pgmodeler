/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "pgmodelercliapp.h"
#include "utilsns.h"
#include "settings/appearanceconfigwidget.h"

QTextStream PgModelerCliApp::out(stdout);

const QRegularExpression PgModelerCliApp::PasswordRegExp=QRegularExpression("(password)(=)(.)*( )");
const QString PgModelerCliApp::PasswordPlaceholder("password=******");

const QString PgModelerCliApp::AllChildren("all");
const QString PgModelerCliApp::Input("--input");
const QString PgModelerCliApp::Output("--output");
const QString PgModelerCliApp::InputDb("--input-db");
const QString PgModelerCliApp::ExportToFile("--export-to-file");
const QString PgModelerCliApp::ExportToPng("--export-to-png");
const QString PgModelerCliApp::ExportToSvg("--export-to-svg");
const QString PgModelerCliApp::ExportToDbms("--export-to-dbms");
const QString PgModelerCliApp::ExportToDict("--export-to-dict");
const QString PgModelerCliApp::ImportDb("--import-db");
const QString PgModelerCliApp::NoIndex("--no-index");
const QString PgModelerCliApp::Split("--split");
const QString PgModelerCliApp::Diff("--diff");
const QString PgModelerCliApp::DropDatabase("--drop-database");
const QString PgModelerCliApp::DropObjects("--drop-objects");
const QString PgModelerCliApp::PgSqlVer("--pgsql-ver");
const QString PgModelerCliApp::Help("--help");
const QString PgModelerCliApp::ShowGrid("--show-grid");
const QString PgModelerCliApp::ShowDelimiters("--show-delimiters");
const QString PgModelerCliApp::PageByPage("--page-by-page");
const QString PgModelerCliApp::IgnoreDuplicates("--ignore-duplicates");
const QString PgModelerCliApp::IgnoreErrorCodes("--ignore-error-codes");
const QString PgModelerCliApp::ConnAlias("--conn-alias");
const QString PgModelerCliApp::Host("--host");
const QString PgModelerCliApp::Port("--port");
const QString PgModelerCliApp::User("--user");
const QString PgModelerCliApp::Passwd("--passwd");
const QString PgModelerCliApp::InitialDb("--initial-db");
const QString PgModelerCliApp::Silent("--silent");
const QString PgModelerCliApp::ListConns("--list-conns");
const QString PgModelerCliApp::Simulate("--simulate");
const QString PgModelerCliApp::FixModel("--fix-model");
const QString PgModelerCliApp::FixTries("--fix-tries");
const QString PgModelerCliApp::ZoomFactor("--zoom");
const QString PgModelerCliApp::UseTmpNames("--use-tmp-names");
const QString PgModelerCliApp::DbmMimeType("--dbm-mime-type");
const QString PgModelerCliApp::Install("install");
const QString PgModelerCliApp::Uninstall("uninstall");
const QString PgModelerCliApp::SystemWide("--system-wide");
const QString PgModelerCliApp::IgnoreImportErrors("--ignore-errors");
const QString PgModelerCliApp::ImportSystemObjs("--import-sys-objs");
const QString PgModelerCliApp::ImportExtensionObjs("--import-ext-objs");
const QString PgModelerCliApp::DebugMode("--debug-mode");
const QString PgModelerCliApp::FilterObjects("--filter-objects");
const QString PgModelerCliApp::MatchByName("--match-by-name");
const QString PgModelerCliApp::ForceChildren("--force-children");
const QString PgModelerCliApp::OnlyMatching("--only-matching");
const QString PgModelerCliApp::PartialDiff("--partial");
const QString PgModelerCliApp::Force("--force");
const QString PgModelerCliApp::StartDate("--start-date");
const QString PgModelerCliApp::EndDate("--end-date");
const QString PgModelerCliApp::CompareTo("--compare-to");
const QString PgModelerCliApp::SaveDiff("--save");
const QString PgModelerCliApp::ApplyDiff("--apply");
const QString PgModelerCliApp::NoDiffPreview("--no-preview");
const QString PgModelerCliApp::DropClusterObjs("--drop-cluster-objs");
const QString PgModelerCliApp::RevokePermissions("--revoke-perms");
const QString PgModelerCliApp::DropMissingObjs("--drop-missing");
const QString PgModelerCliApp::ForceDropColsConstrs("--force-drop-cols");
const QString PgModelerCliApp::RenameDb("--rename-db");
const QString PgModelerCliApp::NoSequenceReuse("--no-sequence-reuse");
const QString PgModelerCliApp::NoCascadeDrop("--no-cascade");
const QString PgModelerCliApp::ForceRecreateObjs("--force-recreate-objs");
const QString PgModelerCliApp::OnlyUnmodifiable("--only-unmodifiable");
const QString PgModelerCliApp::CreateConfigs("--create-configs");
const QString PgModelerCliApp::MissingOnly("--missing-only");

const QString PgModelerCliApp::TagExpr("<%1");
const QString PgModelerCliApp::EndTagExpr("</%1");
const QString PgModelerCliApp::AttributeExpr("(%1)( )*(=)(\")(\\w|\\d|,|\\.|\\&|\\;|\\)|\\(|\\-| )+(\")");

const QString PgModelerCliApp::MsgFileAssociated(tr("Database model files (*.dbm) are already associated to pgModeler! Try use the option `%1' to install the file association anyway.").arg(Force));
const QString PgModelerCliApp::MsgNoFileAssociation(tr("There is no file association related to pgModeler and *.dbm files! Try use the option `%1' to uninstall the file association anyway.").arg(Force));

attribs_map PgModelerCliApp::short_opts = {
	{ Input, "-if" },		{ Output, "-of" },	{ InputDb, "-id" },
	{ ExportToFile, "-ef" },	{ ExportToPng, "-ep" },	{ ExportToSvg, "-es" },
	{ ExportToDbms, "-ed" },	{ ExportToDict, "-ec" },	{ ImportDb, "-im" },
	{ Diff, "-df" },	{ DropDatabase, "-dd" },	{ DropObjects, "-do" },
	{ PgSqlVer, "-v" },	{ Help, "-h" },	{ ShowGrid, "-sg" },
	{ ShowDelimiters, "-sl" },	{ PageByPage, "-pp" },	{ IgnoreDuplicates, "-ir" },
	{ IgnoreErrorCodes, "-ic" },	{ ConnAlias, "-ca" },	{ Host, "-H" },
	{ Port, "-p" },	{ User, "-u" },	{ Passwd, "-w" },
	{ InitialDb, "-D" },	{ Silent, "-s" },	{ ListConns, "-lc" },
	{ Simulate, "-sm" },	{ FixModel, "-fm" },	{ FixTries, "-ft" },
	{ ZoomFactor, "-zf" },	{ UseTmpNames, "-tn" },	{ DbmMimeType, "-mt" },
	{ IgnoreImportErrors, "-ie" },	{ ImportSystemObjs, "-is" },	{ ImportExtensionObjs, "-ix" },
	{ FilterObjects, "-fo" },	{ MatchByName, "-mn" },	{ ForceChildren, "-fc" },
	{ OnlyMatching, "-om" },	{ DebugMode, "-d" },	{ PartialDiff, "-pd" },
	{ StartDate, "-st" },	{ EndDate, "-et" },	{ CompareTo, "-ct" },
	{ SaveDiff, "-sd" },	{ ApplyDiff, "-ad" },	{ NoDiffPreview, "-np" },
	{ DropClusterObjs, "-dc" },	{ RevokePermissions, "-rv" },	{ DropMissingObjs, "-dm" },
	{ ForceDropColsConstrs, "-fd" },	{ RenameDb, "-rn" },
	{ NoSequenceReuse, "-ns" },	{ NoCascadeDrop, "-nd" },	{ ForceRecreateObjs, "-nf" },
	{ OnlyUnmodifiable, "-nu" },	{ NoIndex, "-ni" },	{ Split, "-sp" },
	{ SystemWide, "-sw" },	{ CreateConfigs, "-cc" }, { Force, "-ff" }, { MissingOnly, "-mo" }
};

std::map<QString, bool> PgModelerCliApp::long_opts = {
	{ Input, true }, { Output, true }, { InputDb, true },
	{ ExportToFile, false },	{ ExportToPng, false },	{ ExportToSvg, false },
	{ ExportToDbms, false },	{ ImportDb, false },	{ Diff, false },
	{ DropDatabase, false },	{ DropObjects, false },	{ PgSqlVer, true },
	{ Help, false },	{ ShowGrid, false },	{ ShowDelimiters, false },
	{ PageByPage, false },	{ IgnoreDuplicates, false },	{ IgnoreErrorCodes, true },
	{ ConnAlias, true },	{ Host, true },	{ Port, true },
	{ User, true },	{ Passwd, true },	{ InitialDb, true },
	{ ListConns, false },	{ Simulate, false },	{ FixModel, false },
	{ FixTries, true },	{ ZoomFactor, true },	{ UseTmpNames, false },
	{ DbmMimeType, true },	{ IgnoreImportErrors, false },	{ ImportSystemObjs, false },
	{ ImportExtensionObjs, false },	{ FilterObjects, true },	{ ForceChildren, true },
	{ OnlyMatching, false },	{ MatchByName, false },	{ DebugMode, false },
	{ PartialDiff, false },	{ StartDate, true },	{ EndDate, true },
	{ CompareTo, true },	{ SaveDiff, false },	{ ApplyDiff, false },
	{ NoDiffPreview, false },	{ DropClusterObjs, false },	{ RevokePermissions, false },
	{ DropMissingObjs, false },	{ ForceDropColsConstrs, false },	{ RenameDb, false },
	{ NoSequenceReuse, false },	{ NoCascadeDrop, false },
	{ ForceRecreateObjs, false },	{ OnlyUnmodifiable, false },	{ ExportToDict, false },
	{ NoIndex, false },	{ Split, false },	{ SystemWide, false },
	{ CreateConfigs, false }, { Force, false }, { MissingOnly, false }
};

std::map<QString, QStringList> PgModelerCliApp::accepted_opts = {
	{{ Attributes::Connection }, { ConnAlias, Host, Port, User, Passwd, InitialDb }},
	{{ ExportToFile }, { Input, Output, PgSqlVer, Split }},
	{{ ExportToPng },  { Input, Output, ShowGrid, ShowDelimiters, PageByPage, ZoomFactor }},
	{{ ExportToSvg },  { Input, Output, ShowGrid, ShowDelimiters }},
	{{ ExportToDict }, { Input, Output, Split, NoIndex }},

	{{ ExportToDbms }, { Input, PgSqlVer, IgnoreDuplicates, IgnoreErrorCodes,
											 DropDatabase, DropObjects, Simulate, UseTmpNames }},

	{{ ImportDb }, { InputDb, Output, IgnoreImportErrors, ImportSystemObjs, ImportExtensionObjs,
									 FilterObjects, OnlyMatching, MatchByName, ForceChildren, DebugMode, ConnAlias,
									 Host, Port, User, Passwd, InitialDb }},

	{{ Diff }, { Input, PgSqlVer, IgnoreDuplicates, IgnoreErrorCodes, CompareTo, PartialDiff, Force,
							 StartDate, EndDate, SaveDiff, ApplyDiff, NoDiffPreview, DropClusterObjs, RevokePermissions,
							 DropMissingObjs, ForceDropColsConstrs, RenameDb, NoCascadeDrop,
							 NoSequenceReuse, ForceRecreateObjs, OnlyUnmodifiable }},

	{{ DbmMimeType }, { SystemWide, Force }},
	{{ FixModel },	{ Input, Output, FixTries }},
	{{ ListConns }, {}},
	{{ CreateConfigs }, {MissingOnly, Force}}
};

PgModelerCliApp::PgModelerCliApp(int argc, char **argv) : Application(argc, argv)
{
	try
	{
		QString op, value, orig_op;
		bool accepts_val=false;
		attribs_map opts;
		QStringList args = arguments();

		model=nullptr;
		scene=nullptr;
		xmlparser=nullptr;
		zoom=1;

		export_hlp = nullptr;
		import_hlp = nullptr;
		diff_hlp = nullptr;
		conn_conf = nullptr;
		rel_conf = nullptr;
		general_conf = nullptr;

		// We extract the options values only if the help option is not present
		if(args.size() > 1 && !args.contains(Help) && !args.contains(short_opts[Help]))
		{	
			for(int i=1; i < argc; i++)
			{
				op = orig_op = argv[i];

				//If the retrieved option starts with - it will be treated as a command option
				if(op[0]=='-')
				{
					value.clear();

					if(i < argc-1 && argv[i+1][0]!='-')
					{
						//If the next option does not starts with '-', is considered a value
						value=argv[++i];
					}

					//Raises an error if the option is not recognized
					if(!isOptionRecognized(op, accepts_val))
						throw Exception(tr("Unrecognized option `%1'.").arg(orig_op), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Raises an error if the value is empty and the option accepts a value
					if(accepts_val && value.isEmpty())
						throw Exception(tr("Value not specified for option `%1'.").arg(orig_op), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					else if(!accepts_val && !value.isEmpty())
						throw Exception(tr("Option `%1' does not accept values.").arg(orig_op), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					/* If we find a filter object parameter we append its parameter index so
					 * its value is not replaced by the next filter parameter found */
					if(op == FilterObjects)
						opts[QString("%1%2").arg(op).arg(i)] = value;

					opts[op] = value;
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
				connect(model, SIGNAL(s_objectAdded(BaseObject *)), this, SLOT(handleObjectAddition(BaseObject *)));
				connect(model, SIGNAL(s_objectRemoved(BaseObject *)), this, SLOT(handleObjectRemoval(BaseObject *)));

				//Load the appearance settings including grid and delimiter options
				AppearanceConfigWidget appearance_wgt;
				appearance_wgt.loadConfiguration();

				scene=new ObjectsScene;
				scene->setParent(this);
				scene->setSceneRect(QRectF(0,0,2000,2000));
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

			if(!silent_mode && export_hlp && import_hlp && diff_hlp)
			{
				connect(export_hlp, SIGNAL(s_progressUpdated(int,QString)), this, SLOT(updateProgress(int,QString)));
				connect(export_hlp, SIGNAL(s_errorIgnored(QString,QString,QString)), this, SLOT(printIgnoredError(QString,QString,QString)));
				connect(import_hlp, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString)));
				connect(diff_hlp, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString)));
			}
		}
	}
	catch(Exception &e)
	{
		throw e;
	}
}

PgModelerCliApp::~PgModelerCliApp()
{
	if(scene)
		delete scene;

	delete model;

	if(export_hlp)
		delete export_hlp;

	if(import_hlp)
		delete import_hlp;

	if(diff_hlp)
		delete diff_hlp;

	if(conn_conf)
		delete conn_conf;

	if(rel_conf)
		delete rel_conf;

	if(general_conf)
		delete general_conf;
}

void PgModelerCliApp::printText(const QString &txt)
{
	out << txt << Qt::endl;
}

void PgModelerCliApp::printMessage(const QString &txt)
{
	if(!silent_mode)
		printText(txt);
}

void PgModelerCliApp::configureConnection(bool extra_conn)
{
	QString chr = (extra_conn ? "1" : "");
	Connection *conn = (extra_conn ? &extra_connection : &connection);

	//Getting the connection using its alias
	if(parsed_opts.count(ConnAlias + chr))
	{
		if(!connections.count(parsed_opts[ConnAlias + chr]))
			throw Exception(tr("Connection aliased as '%1' was not found in the configuration file.").arg(parsed_opts[ConnAlias + chr]),
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

bool PgModelerCliApp::isOptionRecognized(QString &op, bool &accepts_val)
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
	return found;
}

void PgModelerCliApp::showVersionInfo()
{
	printMessage();
	printMessage(tr("pgModeler command line interface.") );
	printMessage(tr("Version ") + GlobalAttributes::PgModelerVersion + QString(" - %1 Qt %2").arg(GlobalAttributes::PgModelerBuildNumber).arg(QT_VERSION_STR));
	printMessage(tr("PostgreSQL Database Modeler Project - pgmodeler.io") );
	printMessage(tr("Copyright 2006-%1 Raphael Araújo e Silva <raphael@pgmodeler.io>").arg(QDate::currentDate().year()));
	printMessage();
}

void PgModelerCliApp::showMenu()
{
	showVersionInfo();

	printText(tr("Usage: pgmodeler-cli [OPTIONS]"));
	printText(tr("This CLI tool provides several operations over models and databases without the need to perform them\nin pgModeler's graphical interface. All available options are described below."));
	printText();

	printText(tr("Operation mode options: "));
	printText(tr("  %1, %2\t\t    Export the input model to sql script file(s).").arg(short_opts[ExportToFile]).arg(ExportToFile));
	printText(tr("  %1, %2\t\t    Export the input model to a png image.").arg(short_opts[ExportToPng]).arg(ExportToPng) );
	printText(tr("  %1, %2\t\t    Export the input model to a svg file.").arg(short_opts[ExportToSvg]).arg(ExportToSvg) );
	printText(tr("  %1, %2\t\t    Export the input model to a data directory in HTML format.").arg(short_opts[ExportToDict]).arg(ExportToDict));
	printText(tr("  %1, %2\t\t    Export the input model directly to a PostgreSQL server.").arg(short_opts[ExportToDbms]).arg(ExportToDbms));
	printText(tr("  %1, %2\t\t    List available connections in file %3.").arg(short_opts[ListConns]).arg(ListConns).arg(GlobalAttributes::ConnectionsConf + GlobalAttributes::ConfigurationExt));
	printText(tr("  %1, %2\t\t    Import a database to an output file.").arg(short_opts[ImportDb]).arg(ImportDb));
	printText(tr("  %1, %2\t\t\t    Compares a model and a database or two databases generating the SQL script to sync the latter in relation to the first.").arg(short_opts[Diff]).arg(Diff));
	printText(tr("  %1, %2\t\t    Try to fix the structure of the input model file in order to make it loadable again.").arg(short_opts[FixModel]).arg(FixModel));
	printText(tr("  %1, %2\t\t    Create the pgModeler's configuration folder and files in the user's local storage.").arg(short_opts[CreateConfigs]).arg(CreateConfigs));
#ifndef Q_OS_MAC
	printText(tr("  %1, %2 [ACTION]\t    Handles the file association to .dbm files. The ACTION can be [%3 | %4].").arg(short_opts[DbmMimeType]).arg(DbmMimeType).arg(Install).arg(Uninstall));
#endif
	printText(tr("  %1, %2\t\t\t    Show this help menu.").arg(short_opts[Help]).arg(Help));
	printText();

	printText(tr("General options: "));
	printText(tr("  %1, %2 [FILE]\t\t    Input model file (.dbm). This is mandatory for export and fix operations.").arg(short_opts[Input]).arg(Input));
	printText(tr("  %1, %2 [DBNAME]\t    Input database name. This is mandatory for import operation.").arg(short_opts[InputDb]).arg(InputDb));
	printText(tr("  %1, %2 [FILE|DIRECTORY]    Output file or directory. This is mandatory for fixing model or exporting to file, png or svg.").arg(short_opts[Output]).arg(Output));
	printText(tr("  %1, %2\t\t    Force the PostgreSQL syntax to the specified version when generating SQL code. The version string must be in form major.minor.").arg(short_opts[PgSqlVer]).arg(PgSqlVer));
	printText(tr("  %1, %2\t\t\t    Silent execution. Only critical messages and errors are shown during process.").arg(short_opts[Silent]).arg(Silent));
	printText();

	printText(tr("SQL file export options: "));
	printText(tr("  %1, %2\t\t\t    The SQL file is generated per object. The files will be named in such a way to reflect the correct creation order of the objects.").arg(short_opts[Split]).arg(Split));
	printText();

	printText(tr("PNG and SVG export options: "));
	printText(tr("  %1, %2\t\t    Draws the grid in the exported image.").arg(short_opts[ShowGrid]).arg(ShowGrid));
	printText(tr("  %1, %2\t    Draws the page delimiters in the exported image.").arg(short_opts[ShowDelimiters]).arg(ShowDelimiters));
	printText(tr("  %1, %2\t\t    Each page will be exported in a separated png image. (Only for PNG images)").arg(short_opts[PageByPage]).arg(PageByPage));
	printText(tr("  %1, %2 [FACTOR]\t\t    Applies a zoom (in percent) before export to png image. Accepted zoom interval: %3-%4 (Only for PNG images)").arg(short_opts[ZoomFactor]).arg(ZoomFactor).arg(ModelWidget::MinimumZoom*100).arg(ModelWidget::MaximumZoom*100));
	printText();

	printText(tr("Data dictionary export options: "));
	printText(tr("  %1, %2\t\t\t    The data dictionaries are generated in separated files inside the selected output directory.").arg(short_opts[Split]).arg(Split));
	printText(tr("  %1, %2\t\t    Avoids the generation of the index that is used to help navigating through the data dictionary.").arg(short_opts[NoIndex]).arg(NoIndex));
	printText();

	printText(tr("Connection options: "));
	printText(tr("  %1, %2 [ALIAS]\t    Connection configuration alias to be used.").arg(short_opts[ConnAlias]).arg(ConnAlias));
	printText(tr("  %1, %2 [HOST]\t\t    PostgreSQL host in which a task will operate.").arg(short_opts[Host]).arg(Host));
	printText(tr("  %1, %2 [PORT]\t\t    PostgreSQL host listening port.").arg(short_opts[Port]).arg(Port));
	printText(tr("  %1, %2 [USER]\t\t    PostgreSQL username.").arg(short_opts[User]).arg(User));
	printText(tr("  %1, %2 [PASSWORD]\t    PostgreSQL user password.").arg(short_opts[Passwd]).arg(Passwd));
	printText(tr("  %1, %2 [DBNAME]\t    Connection's initial database.").arg(short_opts[InitialDb]).arg(InitialDb));
	printText();

	printText(tr("DBMS export options: "));
	printText(tr("  %1, %2\t    Ignores errors related to duplicated objects that eventually exist in the server.").arg(short_opts[IgnoreDuplicates]).arg(IgnoreDuplicates));
	printText(tr("  %1, %2 [CODES] Ignores additional errors by their codes. A comma-separated list of alphanumeric codes should be provided.").arg(short_opts[IgnoreErrorCodes]).arg(IgnoreErrorCodes));
	printText(tr("  %1, %2\t\t    Drop the database before execute a export process.").arg(short_opts[DropDatabase]).arg(DropDatabase));
	printText(tr("  %1, %2\t\t    Runs the DROP commands attached to SQL-enabled objects.").arg(short_opts[DropObjects]).arg(DropObjects));
	printText(tr("  %1, %2\t\t    Simulates an export process by executing all steps but undoing any modification in the end.").arg(short_opts[Simulate]).arg(Simulate));
	printText(tr("  %1, %2\t\t    Generates temporary names for database, roles and tablespaces when in simulation mode.").arg(short_opts[UseTmpNames]).arg(UseTmpNames));
	printText();

	printText(tr("Database import options: "));
	printText(tr("  %1, %2\t\t    Ignore all errors and try to create as many as possible objects.").arg(short_opts[IgnoreImportErrors]).arg(IgnoreImportErrors));
	printText(tr("  %1, %2\t    Import system built-in objects. This option causes the model bloating due to the importing of unneeded objects.").arg(short_opts[ImportSystemObjs]).arg(ImportSystemObjs));
	printText(tr("  %1, %2\t    Import extension objects. This option causes the model bloating due to the importing of unneeded objects.").arg(short_opts[ImportExtensionObjs]).arg(ImportExtensionObjs));
	printText(tr("  %1, %2 [FILTER]    Causes the import process to import only those objects matching the filter(s). The FILTER should be in the form type:pattern:mode.").arg(short_opts[FilterObjects]).arg(FilterObjects));
	printText(tr("  %1, %2\t\t    Causes only objects matching the provided filter(s) to be imported. Those not matching filter(s) are discarded.").arg(short_opts[OnlyMatching]).arg(OnlyMatching));
	printText(tr("  %1, %2\t\t    Causes the objects matching to be performed over their names instead of their signature ([schema].[name]).").arg(short_opts[MatchByName]).arg(MatchByName));
	printText(tr("  %1, %2 [OBJECTS]   Forces the importing of children objects related to tables/views/foreign tables matched by the filter(s). The OBJECTS is a comma separated list types.").arg(short_opts[ForceChildren]).arg(ForceChildren));
	printText(tr("  %1, %2\t\t    Run import in debug mode printing all queries executed in the server.").arg(short_opts[DebugMode]).arg(DebugMode));
	printText();

	printText(tr("Diff options: "));
	printText(tr("  %1, %2 [DBNAME]\t    The database used in the comparison. All the SQL code generated is applied to it.").arg(short_opts[CompareTo]).arg(CompareTo));
	printText(tr("  %1, %2\t\t    Toggles the partial diff operation. A set of objects filters should be provided using the import option %3.").arg(short_opts[PartialDiff]).arg(PartialDiff).arg(FilterObjects));
	printText(tr("  %1, %2\t\t\t    Forces a full diff if the provided filters were not able to retrieve objects for a partial diff operation.").arg(short_opts[Force]).arg(Force));
	printText(tr("  %1, %2\t\t    Matches all database model objects in which modification date starts in the specified date. (Only for partial diff)").arg(short_opts[StartDate]).arg(StartDate));
	printText(tr("  %1, %2\t\t    Matches all database model objects in which modification date ends in the specified date. (Only for partial diff)").arg(short_opts[EndDate]).arg(EndDate));
	printText(tr("  %1, %2\t\t\t    Save the generated diff code to output file.").arg(short_opts[SaveDiff]).arg(SaveDiff));
	printText(tr("  %1, %2\t\t\t    Apply the generated diff code on the database server.").arg(short_opts[ApplyDiff]).arg(ApplyDiff));
	printText(tr("  %1, %2\t\t    Don't preview the generated diff code when applying it to the server.").arg(short_opts[NoDiffPreview]).arg(NoDiffPreview));
	printText(tr("  %1, %2\t    Drop cluster level objects like roles and tablespaces.").arg(short_opts[DropClusterObjs]).arg(DropClusterObjs));
	printText(tr("  %1, %2\t\t    Revoke permissions already set on the database. New permissions configured in the input model are still applied.").arg(short_opts[RevokePermissions]).arg(RevokePermissions));
	printText(tr("  %1, %2\t\t    Drop missing objects. Generates DROP commands for objects that are present in the input model but not in the compared database.").arg(short_opts[DropMissingObjs]).arg(DropMissingObjs));
	printText(tr("  %1, %2\t    Force the drop of missing columns and constraints. Causes only columns and constraints to be dropped, other missing objects aren't removed.").arg(short_opts[ForceDropColsConstrs]).arg(ForceDropColsConstrs));
	printText(tr("  %1, %2\t\t    Rename the destination database when the names of the involved databases are different.").arg(short_opts[RenameDb]).arg(RenameDb));
	printText(tr("  %1, %2\t\t    Don't drop objects in cascade mode.").arg(short_opts[NoCascadeDrop]).arg(NoCascadeDrop));
	printText(tr("  %1, %2\t    Don't reuse sequences on serial columns. Drop the old sequence assigned to a serial column and creates a new one.").arg(short_opts[NoSequenceReuse]).arg(NoSequenceReuse));
	printText(tr("  %1, %2\t    Force the recreating of objects. Instead of an ALTER command a DROP and CREATE commands are used to create a new version of the objects.").arg(short_opts[ForceRecreateObjs]).arg(ForceRecreateObjs));
	printText(tr("  %1, %2\t    Recreate only the unmodifiable objects. These objects are the ones which can't be changed via ALTER command.").arg(short_opts[OnlyUnmodifiable]).arg(OnlyUnmodifiable));
	printText();

	printText(tr("Model fix options: ") );
	printText(tr("  %1, %2 [NUMBER]\t    Model fix tries. When reaching the maximum count the invalid objects will be discarded.").arg(short_opts[FixTries]).arg(FixTries));
	printText();

#ifndef Q_OS_MAC
	printText(tr("File association options: "));
	printText(tr("  %1, %2\t\t    The file association to .dbm files will be applied in a system wide level instead of to the current user.").arg(short_opts[SystemWide]).arg(SystemWide));
	printText(tr("  %1, %2 \t\t\t    Forces the mime type install or uninstall. ").arg(short_opts[Force]).arg(Force));
	printText();
#endif

	printText(tr("Config files creation options: "));
	printText(tr("  %1, %2 \t\t    Copy only missing configuration files to the user's local storage.").arg(short_opts[MissingOnly]).arg(MissingOnly));
	printText(tr("  %1, %2 \t\t\t    Forces the recreation of all configuration files. This option implies the backup of the current settings.").arg(short_opts[Force]).arg(Force));
	printText();

	printText();
	printText(tr("** The FILTER value in %1 option has the form type:pattern:mode. ").arg(FilterObjects));
	printText(tr("   * The `type' is the type of object to be filtered and accepts the following values (invalid types ignored): "));

	QStringList list;
	QString child_list;

	for(auto &type : BaseObject::getChildObjectTypes(ObjectType::Table))
	{
		if(type == ObjectType::Column)
			continue;

		list.append(BaseObject::getSchemaName(type));
	}

	list.sort();
	child_list = list.join(", ");

	QStringList fmt_types, lines, type_list = Catalog::getFilterableObjectNames();
	int i = 0;

	for(auto &type : type_list)
	{
		fmt_types.append(type);
		i++;
		if(i % 8 == 0 || i == type_list.size() - 1)
		{
			lines.append(QString("     > ") + fmt_types.join(", "));
			fmt_types.clear();
		}
	}
	printText(lines.join('\n'));

	printText();
	printText(tr("   * The `pattern' is the text pattern which is matched against the objects names."));
	printText();
	printText(tr("   * The `mode' is the way the pattern is matched. This one accepts two values: "));
	printText(tr("     > `%1' causes the pattern to be used as a wildcard string while matching objects names.").arg(UtilsNs::FilterWildcard));
	printText(tr("     > `%1' causes the pattern to be treated as a POSIX regular expression while matching objects names.").arg(UtilsNs::FilterRegExp));
	printText();
	printText(tr("   * The option %1 has effect only when used with %2 and will avoid discarding children of matched tables.").arg(ForceChildren).arg(OnlyMatching));
	printText(tr("     Other tables eventually imported which are dependencies of the matched objects will have their children discarded."));
	printText(tr("     The comma separated list of table children objects accepts the values:"));
	printText(tr("     > %1").arg(child_list) );
	printText(tr("     > Use the special keyword `%1' to force all children objects.").arg(AllChildren) );
	printText();
	printText(tr("   * NOTES: all comparisons during filtering process are case insensitive."));
	printText(tr("     Using the filtering options may cause the importing of additional objects due to the automatic dependency resolution."));
	printText();
	printText(tr("** The diff process allows the usage all options related to the import operation."));
	printText(tr("   It also accepts the following export operation options: `%1', `%2'").arg(IgnoreDuplicates).arg(IgnoreErrorCodes));
	printText();
	printText(tr("** The partial diff operation will always force the options %1 and %2 = %3 for more reliable results.").arg(OnlyMatching).arg(ForceChildren).arg(AllChildren));
	printText(tr("   * The options %1 and %2 accepts the ISO8601 date/time format: yyyy-MM-dd hh:mm:ss").arg(StartDate).arg(EndDate));
	printText();
	printText(tr("** When running the diff using two databases (%1 and %2) there's the option to specify two separated connections/aliases.").arg(InputDb).arg(CompareTo));
	printText(tr("   If only one connection is set then it will be used to import the input database as well to retrieve the database used in the comparison."));
	printText(tr("   A second connection can be specified by appending a 1 to any connection configuration parameter listed above."));
	printText(tr("   This causes the connection to be associated to %1 exclusively.").arg(CompareTo));
	printText();
}

void PgModelerCliApp::listConnections()
{
	std::map<QString, Connection *>::iterator itr=connections.begin();

	if(connections.empty())
		printText(tr("There are no connections configured."));
	else
	{
		unsigned id=0;

		printText(tr("Available connections (alias : connection string)"));

		while(itr != connections.end())
		{
			printText(QString("[") + QString::number(id++) + QString("] ") + itr->first + QString(" : ") +
								itr->second->getConnectionString().replace(PasswordRegExp, PasswordPlaceholder));
			itr++;
		}

		printText();
	}
}

void PgModelerCliApp::parseOptions(attribs_map &opts)
{
	//Loading connections
	if(opts.count(ListConns) || opts.count(ExportToDbms) || opts.count(ImportDb) || opts.count(Diff))
	{
		conn_conf = new ConnectionsConfigWidget;
		conn_conf->loadConfiguration();
		conn_conf->getConnections(connections, false);
	}
	//Loading general and relationship settings when exporting to image formats
	else if(opts.count(ExportToPng) || opts.count(ExportToSvg))
	{
		general_conf = new GeneralConfigWidget;
		rel_conf = new RelationshipConfigWidget;

		general_conf->loadConfiguration();
		rel_conf->loadConfiguration();
	}

	//Creating the export/import/diff helpers when one of the operations are specified
	if(opts.count(ExportToDbms) || opts.count(ExportToFile) ||
		 opts.count(ExportToPng) || opts.count(ExportToSvg) ||
		 opts.count(ExportToDict) || opts.count(ImportDb) ||
		 opts.count(Diff))
	{
		export_hlp = new ModelExportHelper;
		import_hlp = new DatabaseImportHelper;
		diff_hlp = new ModelsDiffHelper;
	}

	if(opts.empty() || opts.count(Help))
		showMenu();
	else
	{
		QString curr_op_mode;
		int exp_mode_cnt = 0, other_modes_cnt = 0;
		bool fix_model = (opts.count(FixModel) > 0),
				upd_mime = (opts.count(DbmMimeType) > 0),
				import_db = (opts.count(ImportDb) > 0),
				diff = (opts.count(Diff) > 0),
				create_configs= (opts.count(CreateConfigs) > 0),
				list_conns = (opts.count(ListConns) > 0),
				export_dbms = (opts.count(ExportToDbms) > 0);

		for(auto &itr : accepted_opts)
		{
			if(itr.first == Attributes::Connection)
				continue;

			if(opts.count(itr.first))
			{
				curr_op_mode = itr.first;

				if(itr.first == ExportToFile || itr.first == ExportToPng ||
					 itr.first == ExportToSvg || itr.first == ExportToDbms ||
					 itr.first == ExportToDict)
					exp_mode_cnt++;
				else
					other_modes_cnt++;
			}
		}

		if(opts.count(ZoomFactor))
			zoom=opts[ZoomFactor].toDouble()/static_cast<double>(100);

		if(other_modes_cnt==0 && exp_mode_cnt==0)
			throw Exception(tr("No operation mode was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if((exp_mode_cnt > 0 && (fix_model || upd_mime || import_db || diff || create_configs || list_conns)) || (exp_mode_cnt==0 && other_modes_cnt > 1))
			throw Exception(tr("Multiple operation modes were specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!fix_model && !upd_mime && exp_mode_cnt > 1)
			throw Exception(tr("Multiple export mode was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!list_conns && !upd_mime && !import_db && !diff && !create_configs && !opts.count(Input))
			throw Exception(tr("No input file was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(import_db && !opts.count(InputDb))
			throw Exception(tr("No input database was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(!opts.count(ExportToDbms) && !upd_mime && !list_conns && !diff && !create_configs && !opts.count(Output))
			throw Exception(tr("No output file was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(!opts.count(ExportToDbms) && !upd_mime && !import_db && !list_conns && !create_configs &&
			 opts.count(Input) && opts.count(Output) &&
			 QFileInfo(opts[Input]).absoluteFilePath() == QFileInfo(opts[Output]).absoluteFilePath())
			throw Exception(tr("Input file must be different from output!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(opts.count(ExportToDbms) && !opts.count(ConnAlias) &&
			 (!opts.count(Host) || !opts.count(User) || !opts.count(Passwd) || !opts.count(InitialDb)) )
			throw Exception(tr("Incomplete connection information!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(opts.count(ExportToPng) && (zoom < ModelWidget::MinimumZoom || zoom > ModelWidget::MaximumZoom))
			throw Exception(tr("Invalid zoom specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		
		if(upd_mime && opts[DbmMimeType]!=Install && opts[DbmMimeType]!=Uninstall)
			throw Exception(tr("Invalid action specified to update mime option!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(create_configs && opts.count(Force) && opts.count(MissingOnly))
			throw Exception(tr("The options `%1' and `%2' can't be used together when handling configuration files!").arg(Force).arg(MissingOnly), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(diff)
		{
			if(!opts.count(Input) && !opts.count(InputDb))
				throw Exception(tr("No input file or database was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(opts.count(Input) && opts.count(InputDb))
				throw Exception(tr("The input file and database can't be used at the same time!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts.count(CompareTo))
				throw Exception(tr("No database to be compared was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(!opts.count(SaveDiff) && !opts.count(ApplyDiff))
				throw Exception(tr("No diff action (save or apply) was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(opts.count(SaveDiff) && opts[Output].isEmpty())
				throw Exception(tr("No output file for the diff code was specified!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(opts.count(PartialDiff) && !opts[Input].count() && (opts.count(StartDate) || opts.count(EndDate)))
				throw Exception(tr("Date filters are allowed only on partial diff using an input model!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(opts.count(PartialDiff) && opts.count(FilterObjects) && (opts.count(StartDate) || opts.count(EndDate)))
				throw Exception(tr("Date filters and object filters can't be used together!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			if(opts.count(PartialDiff) && !opts.count(FilterObjects) && !opts.count(StartDate) && !opts.count(EndDate))
				throw Exception(tr("Partial diff enabled but no object filter was provided!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			// For partial diff we force the --only-matching option and --force-children = all
			if(opts.count(PartialDiff))
			{
				opts[ForceChildren] = AllChildren;
				opts[OnlyMatching] = "";
			}

			// Validating the date formats in the provided start/end dates
			QDateTime *dates[2] = { &start_date, &end_date };
			QStringList dt_params = { StartDate, EndDate };

			for(int idx = 0; idx < 2; idx++)
			{
				if(opts.count(dt_params[idx]))
				{
					*dates[idx] = QDateTime::fromString(opts[dt_params[idx]], Qt::ISODate);

					if(!dates[idx]->isValid())
						throw Exception(tr("Invalid date format `%1' in option `%2'!").arg(opts[dt_params[idx]]).arg(dt_params[idx]), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			/* If any of the dates are correctly parsed we need to force
			 * the signature name matching, the forced filtering of all table children objects
			 * and the only-matching option, so the objects can be correctly retrieved from
			 * the destination database */
			if(start_date.isValid() || end_date.isValid())
			{
				parsed_opts.erase(MatchByName);
				parsed_opts[ForceChildren] = AllChildren;
				parsed_opts[OnlyMatching] = "";
			}
		}
		
		//Converting input and output files to absolute paths to avoid that they are read/written on the app's working dir
		if(opts.count(Input))
			opts[Input]=QFileInfo(opts[Input]).absoluteFilePath();

		if(opts.count(Output))
			opts[Output]=QFileInfo(opts[Output]).absoluteFilePath();

		/* Special treatment for filter parameters:
		 * Since it can be specified several filter parameter we need to join
		 * everything in a single string list so it can be passed to the import helper correctly */
		if(opts.count(FilterObjects))
		{
			opts.erase(FilterObjects);

			for(auto &op : opts)
			{
				if(op.first.contains(FilterObjects))
					obj_filters.append(op.second);
			}
		}

		/* Performing a final validation on the parsed options which consists
		 * in check if all provided options are compatible with the operation mode selected */
		QStringList acc_opts = accepted_opts[curr_op_mode];
		QString long_opt;

		// Diff, import and export (to DBMS) share the same connection options
		if(diff || import_db || export_dbms)
			acc_opts.append(accepted_opts[Attributes::Connection]);

		// Diff also accepts all import parameters
		if(diff)
			acc_opts.append(accepted_opts[ImportDb]);

		for(auto &itr : opts)
		{
			long_opt = itr.first;

			if(long_opt == curr_op_mode || long_opt == Silent)
				continue;

			/* Before validate the option we need to remove any appended number to the option name
			 * This happens for options related to objects filters and connections */
			long_opt.remove(QRegularExpression("[0-9]+$"));

			if(!acc_opts.contains(long_opt))
				throw Exception(tr("The option `%1' is not accepted by the operation mode `%2'!").arg(long_opt).arg(curr_op_mode),
												ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		parsed_opts = opts;
	}
}

int PgModelerCliApp::exec()
{
	try
	{
		if(!parsed_opts.empty())
		{
			showVersionInfo();

			if(parsed_opts.count(ListConns))
				 listConnections();
			else if(parsed_opts.count(FixModel))
				fixModel();
			else if(parsed_opts.count(DbmMimeType))
				updateMimeType();
			else if(parsed_opts.count(CreateConfigs))
				createConfigurations();
			else if(parsed_opts.count(ImportDb))
				importDatabase();
			else if(parsed_opts.count(Diff))
				diffModelDatabase();
			else
				exportModel();
		}

		return 0;
	}
	catch(Exception &e)
	{
		throw e;
	}
}

void PgModelerCliApp::updateProgress(int progress, QString msg, ObjectType)
{
	if(progress > 0)
		printMessage(QString("[%1%] ").arg(progress > 100 ? 100 : progress) + msg);
	else
		printMessage(msg);
}

void PgModelerCliApp::printIgnoredError(QString err_cod, QString err_msg, QString cmd)
{
	printText();
	printText(tr("** Error code `%1' found and ignored. Proceeding with export.").arg(err_cod));
	printText(tr("** Command: %1").arg(cmd));
	printText(err_msg);
	printText();
}

void PgModelerCliApp::handleObjectAddition(BaseObject *object)
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

			case ObjectType::ForeignTable:
				item=new TableView(dynamic_cast<ForeignTable *>(graph_obj));
			break;

			case ObjectType::View:
				item=new GraphicalView(dynamic_cast<View *>(graph_obj));
			break;

			case ObjectType::Relationship:
			case ObjectType::BaseRelationship:
				item=new RelationshipView(dynamic_cast<BaseRelationship *>(graph_obj));
			break;

			case ObjectType::Schema:
				item=new SchemaView(dynamic_cast<Schema *>(graph_obj));
			break;

			default:
				item=new StyledTextboxView(dynamic_cast<Textbox *>(graph_obj));
			break;
		}

		scene->addItem(item);

		if(BaseTable::isBaseTable(obj_type))
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}


void PgModelerCliApp::handleObjectRemoval(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	if(graph_obj)
	{
		scene->removeItem(dynamic_cast<QGraphicsItem *>(graph_obj->getOverlyingObject()));

		//Updates the parent schema if the removed object were a table or view
		if(graph_obj->getSchema() && BaseTable::isBaseTable(graph_obj->getObjectType()))
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}


void PgModelerCliApp::extractObjectXML()
{
	QString buf, lin, def_xml, end_tag, pgmodeler_ver;
	QTextStream ts;
	QRegularExpression regexp(QString("^(<\\?)(xml)(.)+(<%1)").arg(Attributes::DbModel),
														QRegularExpression::DotMatchesEverythingOption),

			//[schema].[func_name](...OUT [type]...)
			func_signature=QRegularExpression("(\")(.)+(\\.)(.)+(\\()(.)*(OUT )(.)+(\\))(\")"),

			//[,]OUT [schema].[type]
			out_param=QRegularExpression("(,)?(OUT )([a-z]|[0-9]|(\\.)|(\\_)|(\\-)|( )|(\\[)|(\\])|(&quot;))+((\\()([0-9])+(\\)))?");
	int start=-1, end=-1;
	bool open_tag=false, close_tag=false, is_rel=false, short_tag=false, end_extract_rel=false, is_change_log=false;
	QRegularExpressionMatch match, header_match;

	printMessage(tr("Extracting objects' XML..."));

	buf.append(UtilsNs::loadFile(parsed_opts[Input]));

	// Extracting pgModeler version from input model
	QRegularExpression ver_expr(AttributeExpr.arg(Attributes::PgModelerVersion));

	match = ver_expr.match(buf);
	start = match.capturedStart();
	model_version = buf.mid(start, match.capturedLength());
	model_version.remove(Attributes::PgModelerVersion);
	model_version.remove(QRegularExpression("(\\\"|\\=| )+"));

	if(!model_version.contains(QRegularExpression("(\\d\\.\\d\\.\\d)((\\-)(alpha|beta)(\\d))?")))
	{
		printMessage(tr("** WARNING: Could't determine the pgModeler version in which the input model was created!"));
		printMessage(tr("            Some fix actions that depend on the model version will not be applied!"));
		model_version.clear();
	}

	//Check if the file contains a valid header (for .dbm file)
	header_match = regexp.match(buf);
	start = header_match.capturedStart();

	if(start < 0)
		throw Exception(tr("Invalid input file! It seems that is not a pgModeler generated model or the file is corrupted!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		//Extracting layers informations from the tag <dbmodel>
		QRegularExpression dbm_regexp = QRegularExpression(TagExpr.arg(Attributes::DbModel)),
				db_end_regexp =  QRegularExpression(EndTagExpr.arg(Attributes::Database));
		int attr_start =-1, attr_end = -1, dbm_start = -1;
		QString aux_buf,
				layers, active_layers, attr_expr = "(%1)( )*(=)(\")";
		QList<unsigned> act_layers_ids;

		match = dbm_regexp.match(buf);
		dbm_start = match.capturedStart();
		aux_buf = buf.mid(dbm_start, buf.indexOf(db_end_regexp) - dbm_start);

		//Layers names
		attr_start = aux_buf.indexOf(Attributes::Layers);
		attr_end = aux_buf.indexOf(Attributes::ActiveLayers);
		layers = aux_buf.mid(attr_start, attr_end - attr_start);
		layers.remove(QRegularExpression(attr_expr.arg(Attributes::Layers)));
		layers.remove('"');
		model->setLayers(layers.trimmed().split(',', Qt::SkipEmptyParts));

		//Active layers
		attr_start = attr_end;
		attr_end = aux_buf.indexOf(Attributes::LayerNameColors, attr_start);
		active_layers = aux_buf.mid(attr_start, attr_end - attr_start);
		active_layers.remove(QRegularExpression(attr_expr.arg(Attributes::ActiveLayers)));
		active_layers.remove('"');
		active_layers.replace(',', ';');

		for(auto &id : active_layers.trimmed().split(';', Qt::SkipEmptyParts))
			act_layers_ids.push_back(id.toUInt());

		model->setActiveLayers(act_layers_ids);
		buf.remove(start, header_match.capturedLength() + 1);

		//Checking if the header ends on a role declaration
		QRegularExpression role_regexp = QRegularExpression(QString("(<%1)(.)*(<\\/%1>)").arg(Attributes::Role),
																												QRegularExpression::DotMatchesEverythingOption);
		end = buf.indexOf(role_regexp);

		// If we found role declarations we clear the header until there
		if(end >= 0)
			buf.remove(0, end);
		else
			// Instead, we clear the header until the starting of database declaration
			buf.remove(0, buf.indexOf(QString("<%1").arg(Attributes::Database)));

		buf.remove(QString("<\\%1>").arg(Attributes::DbModel));
		ts.setString(&buf);

		//Extracts the objects xml line by line
		while(!ts.atEnd())
		{
			lin=ts.readLine();

			/* Collecting changelog entries if present and storing in a separated buffer
			 * so it can be restored in the fixed model during objects' reconstruction */
			if(!is_change_log && lin.contains(TagExpr.arg(Attributes::Changelog)))
				is_change_log = true;

			if(is_change_log)
			{
				changelog.append(lin);

				if(lin.contains(EndTagExpr.arg(Attributes::Changelog)))
					is_change_log = false;
				else
					continue;
			}

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
			else
			{
				//If the line contains an objects open tag
				if(lin.contains(QRegularExpression("^(((\n)|(\t))*(<))")) && !open_tag)
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

void PgModelerCliApp::recreateObjects()
{
	QStringList fail_objs, constr, list;
	QString xml_def, aux_def, start_tag = "<%1", end_tag = "</%1>", aux_tag, type_tag = "<type name=\"%1\"";
	BaseObject *object=nullptr;
	ObjectType obj_type=ObjectType::BaseObject;
	std::vector<ObjectType> types={ ObjectType::Index, ObjectType::Trigger, ObjectType::Rule };
	attribs_map attribs, fmt_ext_names;
	bool use_fail_obj=false;
	unsigned tries=0, max_tries=parsed_opts[FixTries].toUInt();
	int start_pos=-1, end_pos=-1, len=0, obj_id = 0, obj_cnt = objs_xml.count();

	printMessage(tr("Recreating objects..."));

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

		/* Replacing the tags [<type name="extension_type"] by [<type name="schema.extension_type"]
		 * in order to avoid reference breaking when loading the model in pgModeler 0.9.4-alpha1 or above. */
		for(auto &ext_name : fmt_ext_names)
			xml_def.replace(type_tag.arg(ext_name.first), type_tag.arg(ext_name.second));

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
					 * and restarts the XMLParser with the modified buffer */
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

						printMessage(QString("[%1%] %2")
												 .arg(static_cast<int>((++obj_id/static_cast<double>(obj_cnt)) * 100))
												 .arg(tr("Object recreated: `%1' (%2)").arg(object->getName(true), object->getTypeName())));

						/* Special case for extensions:
						 * Before pgModeler 0.9.4-alpha1 the types handled by extension (for example hstore, ltree, etc) were
						 * registered in the PgSqlType as user-defined data type without their schemas names prepended. This
						 * was causing lot of troubles importing databases in which extension data types were being used. The
						 * solution was to adjust the extension type names in such a way to prepend schema names. So here we
						 * store the schema-qualified extension name in a special map where the key is the name of the extension
						 * without the schema name, this way search the tags [<type name="extension"] and replace by [<type name="schema.extension"] */
						if(object->getObjectType() == ObjectType::Extension && dynamic_cast<Extension *>(object)->handlesType())
							fmt_ext_names[object->getName()] = object->getName(true, true);
					}

					//For each sucessful created object the method will try to create a failed one
					use_fail_obj=(!fail_objs.isEmpty());
				}

				/* Additional step to extract indexes/triggers/rules from within tables/views
				 * and putting their xml on the list of object to be created */
				if(BaseTable::isBaseTable(obj_type) && xml_def.contains(QRegularExpression("(<)(index|trigger|rule)")))
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
									aux_def=XmlParser::convertCharsToXMLEntities(aux_def);
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
			{
				fail_objs.push_back(xml_def);
				printText(tr("** WARNING: Failed to recreate an object!"));
			}
			else
				throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}

		if(objs_xml.isEmpty() && (!fail_objs.isEmpty() || !constr.isEmpty()))
		{
			tries++;

			//If the maximum creation tries reaches the maximum value
			if(tries > max_tries)
			{
				//Outputs the code of the objects that wasn't created
				printText();
				printText(tr("** Object(s) that couldn't fixed: "));

				while(!fail_objs.isEmpty())
				{
					printText(fail_objs.front());
					fail_objs.pop_front();
				}

				break;
			}
			else
			{
				printMessage(tr("** WARNING: There are objects that maybe can't be fixed. Trying again... (tries %1/%2)").arg(tries).arg(max_tries));
				model->validateRelationships();
				objs_xml=fail_objs;
				objs_xml.append(constr);
				fail_objs.clear();
				constr.clear();
			}
		}
	}

	// Fixing the roles memberships.
	Role *role = nullptr, *mem_role = nullptr;
	bool member_fixed = false;

	for(auto &rl : member_roles)
	{
		role = model->getRole(rl.first);

		if(!role)
		{
			printMessage(tr("** WARNING: Could not find the role `%1'! Ignoring it...").arg(rl.first));
			continue;
		}

		for(auto &name : rl.second)
		{
			mem_role = model->getRole(name);

			if(!mem_role)
			{
				printMessage(tr("** WARNING: Could not find the role `%1' of `%2`! Igorning it...").arg(name, rl.first));
				continue;
			}

			role->addRole(Role::MemberRole, mem_role);
			member_fixed = true;
		}
	}

	if(member_fixed)
	{
		printMessage(tr("** WARNING: Roles memberships were fixed but their creation order is not guaranteed!"));
		printMessage(tr("            It may be necessary to run the fix tool again but now on the file `%1'.").arg(parsed_opts[Output]));
	}

	// Reconstructing the persisted change log if present
	if(!changelog.isEmpty())
	{
		model->setPersistedChangelog(true);
		xmlparser->loadXMLBuffer(changelog);

		if(xmlparser->accessElement(XmlParser::ChildElement))
		{
			do
			{
				xmlparser->getElementAttributes(attribs);
				model->addChangelogEntry(attribs[Attributes::Signature], attribs[Attributes::Type],
																 attribs[Attributes::Action], attribs[Attributes::Date]);
			}
			while(xmlparser->accessElement(XmlParser::NextElement));
		}
	}
}

void PgModelerCliApp::fixObjectAttributes(QString &obj_xml)
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
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("recheck"))));

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
		obj_xml.replace(QRegularExpression(QString("(%1)( )*(=)(\")").arg(Attributes::Owner)), QString("%1 = \"").arg(Attributes::OwnerColumn));

	/* Remove sysid attribute from <role> tags and storing the referenced roles (ref-roles)
	 * for later re-assignment. */
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Role))))
	{
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("sysid"))));

		QRegularExpression ref_roles_expr = QRegularExpression(QString("(<%1)(.)+(%2)( )*(=)(\")(%3)(\")(.)+(>)").arg(Attributes::Roles, Attributes::RoleType, Attributes::Refer));
		QRegularExpressionMatch match;
		int pos = -1;

		match = ref_roles_expr.match(obj_xml);
		pos = match.capturedStart();

		if(pos >= 0)
		{
			QString buf = obj_xml.mid(pos, match.capturedLength()),
					name_attr = "name=\"", role_name;
			int start_idx = 0, end_idx = 0;

			// Extracting the role name
			start_idx = obj_xml.indexOf(name_attr);
			end_idx = obj_xml.indexOf("\"", start_idx + name_attr.size());
			role_name = obj_xml.mid(start_idx, end_idx - start_idx).remove(name_attr);

			// Removing the element <roles ... role-type="refer"/>
			obj_xml.remove(pos, match.capturedLength());

			// Retrieve the name of the ref-roles
			buf.remove(QRegularExpression("^(.)+(names=\")"));
			buf.remove(buf.indexOf("\""), buf.size());

			/* Storing the association between the current role and the ref-roles
			 * in a map for further processing */
			for(auto &rl_name : buf.split(',', Qt::SkipEmptyParts))
				member_roles[rl_name].append(role_name);
		}
	}

	//Replace <parameter> tag by <typeattrib> on <usertype> tags.
	if(obj_xml.contains(TagExpr.arg(QString("usertype"))))
	{
		obj_xml.replace(TagExpr.arg(Attributes::Parameter), TagExpr.arg(Attributes::TypeAttribute));
		obj_xml.replace(EndTagExpr.arg(Attributes::Parameter), EndTagExpr.arg(Attributes::TypeAttribute));

		// Fixing the enumeration type labels
		if(!model_version.isEmpty() && model_version <= "0.9.4-beta1")
		{
			/* Until pgModeler 0.9.3, enum labels separators where commas.
			 * In pgModeler 0.9.4, enum labels separators where UtilsNs::DataSeparator */
			QString sep = (model_version == "0.9.4-beta1" ? UtilsNs::DataSeparator : ","),
					values, labels;
			QRegularExpression enum_start_expr("(" + TagExpr.arg(Attributes::EnumType) + ")( )*(values)( )*(=)( )*(\\\")"),
					enum_end_expr("(\\\")( )*(\\/>)"),
					enum_tag_expr("(" + TagExpr.arg(Attributes::EnumType) + ")(.)+(/>)");
			int start = -1, end = -1;
			QRegularExpressionMatch match;

			match = enum_start_expr.match(obj_xml);
			start = match.capturedStart() + match.capturedLength();

			match = enum_end_expr.match(obj_xml, start);
			end = match.capturedStart();
			values = obj_xml.mid(start, end - start);

			if(!values.isEmpty())
			{
				// Converting each value extract into a separated <enumeration> tag
				for(auto &label : values.split(sep, Qt::SkipEmptyParts))
					labels.append(QString("\t<%1 label=\"%2\"/>\n").arg(Attributes::EnumType, label));

				obj_xml.replace(enum_tag_expr, labels);
			}
		}
	}

	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Relationship))))
	{
		//Remove auto-sufix, src-sufix, dst-sufix, col-indexes, constr-indexes, attrib-indexes from <relationship> tags.
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("auto-sufix"))));
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("src-sufix"))));
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("dst-sufix"))));
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("col-indexes"))));
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("constr-indexes"))));
		obj_xml.remove(QRegularExpression(AttributeExpr.arg(QString("attrib-indexes"))));

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
		QRegularExpression regexp = QRegularExpression(AttributeExpr.arg(Attributes::Constraint));
		QString constr_name;
		QRegularExpressionMatch match;

		match = regexp.match(obj_xml);
		constr_name = match.capturedTexts().at(0);
		constr_name.remove(QString("%1=\"").arg(Attributes::Constraint));
		constr_name.remove(constr_name.length() - 1, 1);

		obj_xml.remove(QRegularExpression(AttributeExpr.arg(Attributes::Constraint)));

		start_idx = obj_xml.indexOf(TagExpr.arg(Attributes::Expression));
		obj_xml.insert(start_idx, QString("\n\t<constraint name=\"%1\" type=\"check\">\n\t\t").arg(constr_name));

		end_idx = obj_xml.indexOf(EndTagExpr.arg(Attributes::Expression));
		obj_xml.insert(end_idx + EndTagExpr.arg(Attributes::Expression).length() + 1, QString("\n\t</constraint>\n"));
	}

	//Replace the deprecated attribute hide-ext-attribs="false|true" from <table> and <views> by collapse-mode="0|1"
	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Table))) ||
		 obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::View))))
	{
		obj_xml.replace(QString("%1=\"false\"").arg(Attributes::HideExtAttribs), QString("%1=\"0\"").arg(Attributes::CollapseMode));
		obj_xml.replace(QString("%1=\"true\"").arg(Attributes::HideExtAttribs), QString("%1=\"1\"").arg(Attributes::CollapseMode));
	}

	//Remove the usage of IN keyword in functions' signatures since it is the default if absent
	QRegularExpression regexp = QRegularExpression(AttributeExpr.arg(Attributes::Signature));
	QRegularExpressionMatch match;
	int sig_idx = -1,	len = 0;
	QString signature, in_keyw = "IN ";

	match = regexp.match(obj_xml);
	sig_idx = match.capturedStart();

	while(sig_idx >= 0)
	{
		signature = obj_xml.mid(sig_idx,  match.capturedLength());
		len = signature.length();

		if(!signature.contains(in_keyw))
		{
			match = regexp.match(obj_xml, sig_idx + len);
			sig_idx = match.capturedStart();
			continue;
		}

		signature.remove(in_keyw);
		obj_xml.remove(sig_idx, len);
		obj_xml.insert(sig_idx, signature);

		match = regexp.match(obj_xml, sig_idx + len);
		sig_idx = match.capturedStart();
	}

	//Rename the attribute layer to layers
	if(obj_xml.contains(QRegularExpression("(layer)( )*(=)")))
		obj_xml.replace("layer", Attributes::Layers);

	//Fix the references to op. classes and families if needed
	fixOpClassesFamiliesReferences(obj_xml);
}

void PgModelerCliApp::fixOpClassesFamiliesReferences(QString &obj_xml)
{
	ObjectType ref_obj_type;

	if(obj_xml.contains(TagExpr.arg(BaseObject::getSchemaName(ObjectType::Index))) ||
			obj_xml.contains(QRegularExpression(QString("(%1)(.)+(type=)(\")(%2)(\")")
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

	QString obj_name, aux_obj_name, signature=("%1 USING %2");
	QRegularExpression sign_regexp=QRegularExpression(AttributeExpr.arg("signature"));
	QRegularExpressionMatch match;
	QStringList index_types;
	int pos=0;

	obj_xml.replace(TagExpr.arg(ref_obj_name) + " name=",
					TagExpr.arg(ref_obj_name) +" signature=");

	index_types = IndexingType::getTypes();

	do
	{
		match = sign_regexp.match(obj_xml, pos);
		pos = match.capturedStart();

		if(pos >= 0)
		{
			//Extracting the signature attribute
			obj_name=obj_xml.mid(pos, match.capturedLength());

			//Removing useless portions signature=" in order to retrive only the object's name
			obj_name.remove(QRegularExpression("(signature)( )*(=)"));
			obj_name.remove('"');

			//Transforming xml entity for quote into the char
			obj_name.replace(XmlParser::CharQuot, QString("\""));

			for(auto &idx_type : index_types)
			{
				aux_obj_name=signature.arg(obj_name).arg(idx_type);

				if(model->getObjectIndex(aux_obj_name, ref_obj_type) >= 0)
				{
					//Replacing the old signature with the corrected form
					aux_obj_name.replace(QString("\""), XmlParser::CharQuot);
					obj_xml.replace(pos, match.capturedLength(), QString("signature=\"%1\"").arg(aux_obj_name));
					break;
				}
			}

			pos += match.capturedLength();
		}
	}
	while(pos >= 0);
}

void PgModelerCliApp::fixModel()
{
	printMessage(tr("Starting model fixing..."));
	printMessage(tr("Loading input file: %1").arg(parsed_opts[Input]));
	printMessage(tr("Fixed model file: %1").arg(parsed_opts[Output]));

	extractObjectXML();
	recreateObjects();

	printMessage(tr("Updating relationships..."));
	model->updateTablesFKRelationships();

	printMessage(tr("Saving fixed output model..."));
	model->saveModel(parsed_opts[Output], SchemaParser::XmlDefinition);

	printMessage(tr("Model successfully fixed!"));
}

void PgModelerCliApp::loadModel()
{
	//Create the systems objects on model before loading it
	model->createSystemObjects(false);

	//Load the model file
	model->loadModel(parsed_opts[Input]);

	/* The scene object is created only when some options are used
	 * so we need to check it if is not null to avoid segfaults */
	if(scene)
	{
		scene->blockSignals(true);

		scene->addLayers(model->getLayers(), false);
		scene->setActiveLayers(model->getActiveLayers());
		scene->setLayerColors(ObjectsScene::LayerNameColor, model->getLayerNameColors());
		scene->setLayerColors(ObjectsScene::LayerRectColor, model->getLayerRectColors());
		scene->setLayerNamesVisible(model->isLayerNamesVisible());
		scene->setLayerRectsVisible(model->isLayerRectsVisible());

		if(model->isLayerRectsVisible())
			model->setObjectsModified({ ObjectType::Schema });

		scene->blockSignals(false);
	}
}

void PgModelerCliApp::exportModel()
{
	printMessage(tr("Starting model export..."));
	printMessage(tr("Loading input file: %1").arg(parsed_opts[Input]));

	loadModel();

	//Export to PNG
	if(parsed_opts.count(ExportToPng))
	{
		printMessage(tr("Export to PNG image: %1").arg(parsed_opts[Output]));

		export_hlp->exportToPNG(scene, parsed_opts[Output], zoom,
								 parsed_opts.count(ShowGrid) > 0,
								 parsed_opts.count(ShowDelimiters) > 0,
								 parsed_opts.count(PageByPage) > 0);
	}
	//Export to SVG
	else if(parsed_opts.count(ExportToSvg))
	{
		printMessage(tr("Export to SVG file: %1").arg(parsed_opts[Output]));

		export_hlp->exportToSVG(scene, parsed_opts[Output],
													 parsed_opts.count(ShowGrid) > 0,
													 parsed_opts.count(ShowDelimiters) > 0);
	}
	//Export to SQL file
	else if(parsed_opts.count(ExportToFile))
	{
		printMessage(tr("Export to SQL script file: %1").arg(parsed_opts[Output]));
		export_hlp->exportToSQL(model, parsed_opts[Output], parsed_opts[PgSqlVer], parsed_opts.count(Split) > 0);
	}
	//Export data dictionary
	else if(parsed_opts.count(ExportToDict))
	{
		printMessage(tr("Export to data dictionary: %1").arg(parsed_opts[Output]));
		export_hlp->exportToDataDict(model, parsed_opts[Output],
																 parsed_opts.count(NoIndex) == 0,
																 parsed_opts.count(Split) > 0);
	}
	//Export to DBMS
	else
	{
		printMessage(tr("Export to DBMS: %1").arg(connection.getConnectionString().replace(PasswordRegExp, PasswordPlaceholder)));

		if(parsed_opts.count(IgnoreErrorCodes))
			export_hlp->setIgnoredErrors(parsed_opts[IgnoreErrorCodes].split(','));

		export_hlp->exportToDBMS(model, connection, parsed_opts[PgSqlVer],
								parsed_opts.count(IgnoreDuplicates) > 0,
								parsed_opts.count(DropDatabase) > 0,
								parsed_opts.count(DropObjects) > 0,
								parsed_opts.count(Simulate) > 0,
								parsed_opts.count(UseTmpNames) > 0);
	}

	printMessage(tr("Export successfully ended!\n"));
}

void PgModelerCliApp::importDatabase()
{
	printMessage(tr("Starting database import..."));
	printMessage(tr("Input database: %1").arg(connection.getConnectionId(true, true)));

	ModelWidget *model_wgt = new ModelWidget;

	importDatabase(model_wgt->getDatabaseModel(), connection);
	model_wgt->rearrangeSchemasInGrid();

	printMessage(tr("Saving the imported database to file..."));

	model_wgt->getDatabaseModel()->saveModel(parsed_opts[Output], SchemaParser::XmlDefinition);

	printMessage(tr("Import successfully ended!\n"));

	delete model_wgt;
}

void PgModelerCliApp::importDatabase(DatabaseModel *model, Connection conn)
{
	try
	{
		std::map<ObjectType, std::vector<unsigned>> obj_oids;
		std::map<unsigned, std::vector<unsigned>> col_oids;
		Catalog catalog;
		QString db_oid;
		QStringList force_tab_objs;
		bool imp_sys_objs = (parsed_opts.count(ImportSystemObjs) > 0),
				imp_ext_objs = (parsed_opts.count(ImportExtensionObjs) > 0);

		if(parsed_opts[ForceChildren] == AllChildren)
		{
			for(auto &type : BaseObject::getChildObjectTypes(ObjectType::Table))
			{
				if(type == ObjectType::Column)
					continue;

				force_tab_objs.append(BaseObject::getSchemaName(type));
			}
		}
		else
			force_tab_objs = parsed_opts[ForceChildren].split(',', Qt::SkipEmptyParts);

		catalog.setConnection(conn);

		/* catalog.setQueryFilter(Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes |
													 (!imp_ext_objs ? Catalog::ExclExtensionObjs : 0) |
													 (!imp_sys_objs ? Catalog::ExclSystemObjs : 0)); */

		catalog.setQueryFilter(Catalog::ListAllObjects | Catalog::ExclBuiltinArrayTypes |
													 Catalog::ExclExtensionObjs | Catalog::ExclSystemObjs);

		catalog.setObjectFilters(obj_filters, parsed_opts.count(OnlyMatching) > 0,
														 parsed_opts.count(MatchByName) == 0, force_tab_objs);

		catalog.getObjectsOIDs(obj_oids, col_oids, {{Attributes::FilterTableTypes, Attributes::True}});

		db_oid = catalog.getObjectOID(conn.getConnectionParam(Connection::ParamDbName), ObjectType::Database);
		obj_oids[ObjectType::Database].push_back(db_oid.toUInt());
		catalog.closeConnection();

		import_hlp->setConnection(conn);
		import_hlp->setImportOptions(imp_sys_objs,
																 imp_ext_objs,
																 true,
																 parsed_opts.count(IgnoreImportErrors) > 0,
																 parsed_opts.count(DebugMode) > 0,
																 !parsed_opts.count(Diff), !parsed_opts.count(Diff));

		model->createSystemObjects(true);
		import_hlp->setSelectedOIDs(model, obj_oids, col_oids);
		import_hlp->importDatabase();
		import_hlp->closeConnection();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PgModelerCliApp::diffModelDatabase()
{
	DatabaseModel *model_aux = new DatabaseModel();
	QString dbname;
	std::vector<BaseObject *> filtered_objs;

	printMessage(tr("Starting diff process..."));

	if(!parsed_opts[Input].isEmpty())
		printMessage(tr("Input model: %1").arg(parsed_opts[Input]));
	else
		printMessage(tr("Input database: %1").arg(connection.getConnectionId(true, true)));

	dbname = extra_connection.getConnectionId(true, true);
	printMessage(tr("Compare to: %1").arg(dbname));

	if(!parsed_opts[Input].isEmpty())
	{
		printMessage(tr("Loading input model..."));
		loadModel();

		if(parsed_opts.count(PartialDiff))
		{
			QString search_attr = parsed_opts.count(MatchByName) ? Attributes::Name : Attributes::Signature;

			// Filtering by modification date always forces the signature matching
			if(start_date.isValid() || end_date.isValid())
				obj_filters.append(model->getFiltersFromChangelog(start_date, end_date));

			filtered_objs = model->findObjects(obj_filters, search_attr);

			/* We need to finish the diff if no object was found based on the filters
			 * this will avoid the diff between an empty database model and a full database model
			 * which may produce unexpected results like try to recreate all objects from the database
			 * model that contains objects */
			if(filtered_objs.empty())
			{
				printMessage(tr("No object was retrieved using the provided filter(s)."));

				if(!parsed_opts.count(Force))
				{
					printMessage(tr("Use the option `%1' to force a full diff in this case.").arg(Force));
					printMessage(tr("The diff process will not continue!\n"));
					return;
				}
				else
					printMessage(tr("Switching to full diff..."));
			}
			else
			{
				/* Special case: when performing a partial diff between a model and a database
				 * and in the set of filtered model objects we have one or more many-to-many, inheritance or partitioning
				 * relationships we need to inject filters to force the retrieval of the all involved tables in those relationships
				 * from the destination database,this way we avoid the diff try to create everytime all tables
				 * in the those relationships. */
				obj_filters.append(ModelsDiffHelper::getRelationshipFilters(filtered_objs, search_attr == Attributes::Signature));
			}
		}
	}
	else
	{
		printMessage(tr("Importing the database `%1'...").arg(connection.getConnectionId(true, true)));
		importDatabase(model, connection);
	}

	printMessage(tr("Importing the database `%1'...").arg(dbname));
	importDatabase(model_aux, extra_connection);

	diff_hlp->setModels(model, model_aux);
	diff_hlp->setFilteredObjects(filtered_objs);
	diff_hlp->setDiffOption(ModelsDiffHelper::OptKeepClusterObjs, !parsed_opts.count(DropClusterObjs));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptCascadeMode, !parsed_opts.count(NoCascadeDrop));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptForceRecreation, parsed_opts.count(ForceRecreateObjs));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptRecreateUnmodifiable, parsed_opts.count(OnlyUnmodifiable));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptKeepObjectPerms, !parsed_opts.count(RevokePermissions));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptReuseSequences, !parsed_opts.count(NoSequenceReuse));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptPreserveDbName, !parsed_opts.count(RenameDb));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptDontDropMissingObjs, !parsed_opts.count(DropMissingObjs));
	diff_hlp->setDiffOption(ModelsDiffHelper::OptDropMissingColsConstr, !parsed_opts.count(ForceDropColsConstrs));

	if(!parsed_opts[PgSqlVer].isEmpty())
		diff_hlp->setPgSQLVersion(parsed_opts[PgSqlVer]);
	else
	{
		extra_connection.connect();
		diff_hlp->setPgSQLVersion(extra_connection.getPgSQLVersion(true));
		extra_connection.close();
	}

	printMessage(tr("Comparing the generated models..."));
	diff_hlp->diffModels();

	if(diff_hlp->getDiffDefinition().isEmpty())
		printMessage(tr("No differences were detected."));
	else
	{
		if(parsed_opts.count(SaveDiff))
		{
			printMessage(tr("Saving diff to file `%1'").arg(parsed_opts[Output]));
			UtilsNs::saveFile(parsed_opts[Output], diff_hlp->getDiffDefinition().toUtf8());
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
				buff += diff_hlp->getDiffDefinition();
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

				out << Qt::endl;
				out << tr("** WARNING: You are about to apply the generated diff code to the server. Data can be lost in the process!") << Qt::endl;

				do
				{
					out << tr("** Proceed with the diff applying? (yes/no) > ");
					out.flush();

					in.skipWhiteSpace();
					res = in.readLine();
				}
				while(res.toLower() != tr("yes") && res.toLower() != tr("no"));

				if(res.toLower() == tr("no"))
				{
					apply_diff = false;
					printMessage(tr("Diff code not applied to the server."));
				}
			}

			if(apply_diff)
			{
				printMessage(tr("Applying diff to the database `%1'...").arg(dbname));
				export_hlp->setExportToDBMSParams(diff_hlp->getDiffDefinition(),
												 &extra_connection,
												 parsed_opts[CompareTo], parsed_opts.count(IgnoreDuplicates));

				if(parsed_opts.count(IgnoreErrorCodes))
					export_hlp->setIgnoredErrors(parsed_opts[IgnoreErrorCodes].split(','));

				export_hlp->exportToDBMS();
			}
		}
	}

	printMessage(tr("Diff successfully ended!\n"));
}

void PgModelerCliApp::updateMimeType()
{
#ifndef Q_OS_MAC
	try
	{
		printMessage(tr("Starting mime update..."));

		handleMimeDatabase(parsed_opts[DbmMimeType]==Uninstall, parsed_opts.count(SystemWide) != 0, parsed_opts.count(Force) != 0);

		printMessage(tr("Mime database successfully updated!\n"));
	}
	catch (Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
#endif
}

QStringList PgModelerCliApp::extractForeignKeys(QString &obj_xml)
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

	return constr_lst;
}

bool PgModelerCliApp::containsRelAttributes(const QString &str)
{
	bool found=false;
	static std::vector<QString> attribs={ Attributes::Relationship,
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

	return found;
}

void PgModelerCliApp::handleMimeDatabase(bool uninstall, bool system_wide, bool force)
{
	printMessage(tr("Mime database operation: %1").arg(uninstall ? QString("uninstall") : QString("install")));

	#ifdef Q_OS_LINUX
		handleLinuxMimeDatabase(uninstall, system_wide, force);
	#else
		#ifdef Q_OS_WIN
			handleWindowsMimeDatabase(uninstall, system_wide, force);
		#endif
	#endif
}

void PgModelerCliApp::handleLinuxMimeDatabase(bool uninstall, bool system_wide, bool force)
{
	SchemaParser schparser;
	attribs_map attribs;
	QString str_aux,

			share_path = !system_wide ? QDir::homePath() + QString("/.local/share") : QString("/usr/share"),

			//Configures the path to the application logo
			exec_icon=GlobalAttributes::getTmplConfigurationFilePath("", "pgmodeler_logo.png"),

			//Configures the path to the document logo
			dbm_icon=GlobalAttributes::getTmplConfigurationFilePath("", "pgmodeler_dbm.png"),

			sch_icon=GlobalAttributes::getTmplConfigurationFilePath("", "pgmodeler_sch.png"),

			//Path to directory that register mime types
			mime_db_dir=QString("%1/mime").arg(share_path),

			//Path to the file that associates apps to mimetypes
			mimeapps=QString("%1/applications/mimeapps.list").arg(share_path);

		//Files generated after update file association (application-dbm.xml and pgModeler.desktop)
		QStringList	files = { QString("%1/applications/pgModeler.desktop").arg(share_path),
													QString("%1/applications/pgModelerSchEditor.desktop").arg(share_path),
													mime_db_dir + QString("/packages/application-dbm.xml"),
													mime_db_dir + QString("/packages/application-sch.xml")},

			schemas = { GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir, QString("desktop") + GlobalAttributes::SchemaExt),
									GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir, QString("desktop-sch") + GlobalAttributes::SchemaExt),
									GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir, QString("application-dbm") + GlobalAttributes::SchemaExt),
									GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir, QString("application-sch") + GlobalAttributes::SchemaExt) },

			icons = { exec_icon, sch_icon, dbm_icon, sch_icon };

	QByteArray buf, buf_aux;
	QFile out;

	for(unsigned i=0; i < 4; i++)
	{
		//When installing, check if the necessary file exists. If exists, raises an error and abort.
		if(!uninstall && QFileInfo(files[i]).exists() && !force)
			throw Exception(MsgFileAssociated, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(uninstall && !QFileInfo(files[i]).exists() && !force)
			throw Exception(MsgNoFileAssociation, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	if(!uninstall && !system_wide)
		attribs[Attributes::WorkingDir]=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	else
		attribs[Attributes::WorkingDir]="";

	try
	{
		for(unsigned i=0; i < 4; i++)
		{
			if(uninstall)
			{
				if(!QFile(files[i]).remove() && !force)
				{
					throw Exception(tr("Can't erase the file %1! Check if the current user has permissions to delete it and if the file exists.").arg(files[i]),
													ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}
			else
			{
				attribs[Attributes::Application]=(i == 0 ? GlobalAttributes::getPgModelerAppPath() : GlobalAttributes::getPgModelerSchemaEditorPath());
				attribs[Attributes::Icon] = icons[i];

				schparser.loadFile(schemas[i]);
				schparser.ignoreEmptyAttributes(true);
				buf.append(schparser.getCodeDefinition(attribs).toUtf8());
				QDir(QString(".")).mkpath(QFileInfo(files[i]).absolutePath());

				UtilsNs::saveFile(files[i], buf);
				buf.clear();
			}
		}

		out.setFileName(mimeapps);

		//If the file mimeapps.list doesn't exists (generally in Ubuntu) creates a new one
		if(!uninstall && !QFileInfo(mimeapps).exists())
		{
			out.open(QFile::WriteOnly);
			out.write(QByteArray("[Added Associations]\napplication/dbm=pgModeler.desktop;\n"));
			out.write(QByteArray("\n[Default Applications]\napplication/dbm=pgModeler.desktop;\n"));
			out.write(QByteArray("\n[Added Associations]\napplication/sch=pgModelerStxEditor.desktop;\n"));
			out.write(QByteArray("\n[Default Applications]\napplication/sch=pgModelerStxEditor.desktop;\n"));
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
				str_aux.replace(QRegularExpression(QRegularExpression::wildcardToRegularExpression("application/dbm*")),"");

				if(!str_aux.isEmpty())
				{
					//Updates the application/dbm mime association
					if(!uninstall && (str_aux.contains("[Added Associations]") ||
										str_aux.contains("[Default Applications]")))
						str_aux.append("\napplication/dbm=pgModeler.desktop;\n");
					else
						str_aux+="\n";

					if(str_aux.startsWith("[") && !str_aux.contains("Added Associations"))
						str_aux="\n" + str_aux;

					buf_aux.append(str_aux.toUtf8());
				}
			}

			//Write a new copy of the mimeapps.list file
			out.open(QFile::Truncate | QFile::WriteOnly);
			out.write(buf_aux.data(), buf_aux.size());
			out.close();
		}

		//Update the mime database
		printMessage(tr("Running update-mime-database command..."));

		QProcess::execute("update-mime-database", QStringList { mime_db_dir });
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void PgModelerCliApp::handleWindowsMimeDatabase(bool uninstall, bool system_wide, bool force)
{
	SchemaParser schparser;
	QString base_reg_key = system_wide ? QString("HKEY_LOCAL_MACHINE\\SOFTWARE") : QString("HKEY_CURRENT_USER\\Software");

	//Checking if the .dbm registry key exists
		QSettings dbm_ext(QString("%1\\Classes\\.dbm").arg(base_reg_key), QSettings::NativeFormat),
				sch_ext(QString("%1\\Classes\\.sch").arg(base_reg_key), QSettings::NativeFormat);
	QString exe_path=QDir::toNativeSeparators(GlobalAttributes::getPgModelerAppPath()),
			sc_exe_path=QDir::toNativeSeparators(GlobalAttributes::getPgModelerSchemaEditorPath());

	//If there is no value assigned to (.dbm | .sch)/Default key and the user wants to uninstall file association, raises an error
	if(uninstall && !force &&
		 (dbm_ext.value(QString("Default")).toString().isEmpty() ||
			sch_ext.value(QString("Default")).toString().isEmpty()))
		throw Exception(MsgNoFileAssociation, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!uninstall && !force &&
		 (!dbm_ext.value(QString("Default")).toString().isEmpty() ||
			!sch_ext.value(QString("Default")).toString().isEmpty()))
		throw Exception(MsgFileAssociated, ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(!uninstall)
	{
		//Write the default value for .dbm registry key
		dbm_ext.setValue(QString("Default"), QString("dbm_auto_file"));
		sch_ext.setValue(QString("Default"), QString("sch_auto_file"));
	}
	else
	{
		dbm_ext.remove("");
		sch_ext.remove("");
	}

	dbm_ext.sync();
	sch_ext.sync();

	//Other registry keys values
	std::map<QString, QStringList> confs = {
				{ QString("\\%1\\Classes\\dbm_auto_file").arg(base_reg_key), { QString("FriendlyTypeName") , QString("pgModeler Database Model") } },
				{ QString("\\%1\\Classes\\dbm_auto_file\\DefaultIcon").arg(base_reg_key), { QString("Default") , QString("%1,1").arg(exe_path) } },
				{ QString("\\%1\\Classes\\dbm_auto_file\\shell\\open\\command").arg(base_reg_key), { QString("Default") , QString("\"%1\" \"%2\"").arg(exe_path).arg("%1") } },
				{ QString("\\%1\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.dbm").arg(base_reg_key), { QString("OpenWithList/a"), QString("pgmodeler.exe"), QString("OpenWithList/MRUList"), QString("a")} },
				{ QString("\\%1\\Classes\\sch_auto_file").arg(base_reg_key), { QString("FriendlyTypeName") , QString("pgModeler Schema File") } },
				{ QString("\\%1\\Classes\\sch_auto_file\\DefaultIcon").arg(base_reg_key), { QString("Default") , QString("%1,1").arg(sc_exe_path) } },
				{ QString("\\%1\\Classes\\sch_auto_file\\shell\\open\\command").arg(base_reg_key), { QString("Default") , QString("\"%1\" \"%2\"").arg(sc_exe_path).arg("%1") } },
				{ QString("\\%1\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.sch").arg(base_reg_key), { QString("OpenWithList/a"), QString("pgmodeler-sc.exe"), QString("OpenWithList/MRUList"), QString("a")} }
	};

	std::map<QString, QStringList>::iterator itr;
	itr=confs.begin();

	//Iterates over the configuration map writing the other keys on registry
	while(itr!=confs.end())
	{
		QSettings s(itr->first, QSettings::NativeFormat);

		if(uninstall)
			s.remove("");
		else
		{
			for(int i=0; i < itr->second.size(); i+=2)
				s.setValue(itr->second[i], itr->second[i+1]);
		}

		s.sync();
		itr++;
	}
}

void PgModelerCliApp::createConfigurations()
{
	QString conf_dir = GlobalAttributes::getConfigurationsDir();

	printMessage(tr("Creating configuration files..."));
	printMessage(tr("Destination path: %1").arg(conf_dir));

	bool missing_only = parsed_opts.count(MissingOnly) > 0,
			force = parsed_opts.count(Force) > 0;

	if(!missing_only && !force && QDir(GlobalAttributes::getConfigurationsDir()).exists())
		throw Exception(tr("Configuration files already exist!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		if(force)
		{
			QDir dir;
			QString bkp_conf_dir = conf_dir + QDateTime::currentDateTime().toString("_yyyyMMd_hhmmss");

			printMessage(tr("Configuration files already exist! Creating a backup..."));

			if(!dir.rename(conf_dir, bkp_conf_dir))
				throw Exception(tr("Failed to create a backup of the configuration files!"), ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		createUserConfiguration(missing_only);
		printMessage(tr("Configuration files successfully created!\n"));
	}
	catch (Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}
