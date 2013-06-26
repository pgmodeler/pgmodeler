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

#include "pgmodelercli.h"

QTextStream PgModelerCLI::out(stdout);
QString PgModelerCLI::INPUT="--input";
QString PgModelerCLI::OUTPUT="--output";
QString PgModelerCLI::EXPORT_TO_FILE="--export-to-file";
QString PgModelerCLI::EXPORT_TO_PNG="--export-to-png";
QString PgModelerCLI::EXPORT_TO_DBMS="--export-to-dbms";
QString PgModelerCLI::PGSQL_VER="--pgsql-ver";
QString PgModelerCLI::HELP="--help";
QString PgModelerCLI::SHOW_GRID="--show-grid";
QString PgModelerCLI::SHOW_DELIMITERS="--show-delimiters";
QString PgModelerCLI::IGNORE_DUPLICATES="--ignore-duplicates";
QString PgModelerCLI::CONN_ALIAS="--conn-alias";
QString PgModelerCLI::HOST="--host";
QString PgModelerCLI::PORT="--port";
QString PgModelerCLI::USER="--user";
QString PgModelerCLI::PASSWD="--passwd";
QString PgModelerCLI::INITIAL_DB="--initial-db";
QString PgModelerCLI::SILENT="--silent";
QString PgModelerCLI::LIST_CONNS="--list-conns";

PgModelerCLI::PgModelerCLI(int argc, char **argv) :  QApplication(argc, argv)
{
	try
	{
		QString op, value;
		bool accepts_val=false;
		int eq_pos=-1;
		map<QString, QString> opts;

		model=nullptr;
		scene=nullptr;
    executable_dir=QFileInfo(argv[0]).absolutePath();

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
					eq_pos=op.indexOf("=");

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
						throw Exception(trUtf8("Unrecognized option '%1'.").arg(op), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Raises an error if the value is empty and the option accepts a value
					if(accepts_val && value.isEmpty())
						throw Exception(trUtf8("Value not specified for option '%1'.").arg(op), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					else if(!accepts_val && !value.isEmpty())
						throw Exception(trUtf8("Option '%1' does not accept values.").arg(op), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					opts[op]=value;
				}
			}
		}

		//Validates and executes the options
		parserOptions(opts);

		if(!parsed_opts.empty())
		{
			model=new DatabaseModel;
			silent_mode=(parsed_opts.count(SILENT));

			//If the export is to png image loads additional configurations
			if(parsed_opts.count(EXPORT_TO_PNG))
			{
				connect(model, SIGNAL(s_objectAdded(BaseObject*)), this, SLOT(handleObjectAddition(BaseObject *)));

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
			else if(parsed_opts.count(EXPORT_TO_DBMS))
			{
				//Getting the connection using its alias
				if(parsed_opts.count(CONN_ALIAS))
				{
					if(!connections.count(parsed_opts[CONN_ALIAS]))
						throw Exception(trUtf8("Connection aliased as '%1' was not found on configuration file.").arg(parsed_opts[CONN_ALIAS]),
														ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Make a copy of the named connection
					connection=(*connections[parsed_opts[CONN_ALIAS]]);
				}
				else
				{
					connection.setConnectionParam(DBConnection::PARAM_SERVER_FQDN, parsed_opts[HOST]);
					connection.setConnectionParam(DBConnection::PARAM_USER, parsed_opts[USER]);
					connection.setConnectionParam(DBConnection::PARAM_PORT, parsed_opts[PORT]);
					connection.setConnectionParam(DBConnection::PARAM_PASSWORD, parsed_opts[PASSWD]);
					connection.setConnectionParam(DBConnection::PARAM_DB_NAME, parsed_opts[INITIAL_DB]);
				}
			}

			if(!silent_mode)
				connect(&export_hlp, SIGNAL(s_progressUpdated(int,QString)), this, SLOT(updateProgress(int,QString)));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

PgModelerCLI::~PgModelerCLI(void)
{
	if(scene) delete(scene);
	delete(model);
}

void PgModelerCLI::initializeOptions(void)
{
	long_opts[INPUT]=true;
	long_opts[OUTPUT]=true;
	long_opts[EXPORT_TO_FILE]=false;
	long_opts[EXPORT_TO_PNG]=false;
	long_opts[EXPORT_TO_DBMS]=false;
	long_opts[PGSQL_VER]=true;
	long_opts[HELP]=false;
	long_opts[SHOW_GRID]=false;
	long_opts[SHOW_DELIMITERS]=false;
	long_opts[IGNORE_DUPLICATES]=false;
	long_opts[CONN_ALIAS]=true;
	long_opts[HOST]=true;
	long_opts[PORT]=true;
	long_opts[USER]=true;
	long_opts[PASSWD]=true;
	long_opts[INITIAL_DB]=true;
	long_opts[LIST_CONNS]=false;

	short_opts[INPUT]="-i";
	short_opts[OUTPUT]="-o";
	short_opts[EXPORT_TO_FILE]="-f";
	short_opts[EXPORT_TO_PNG]="-p";
	short_opts[EXPORT_TO_DBMS]="-d";
	short_opts[PGSQL_VER]="-v";
	short_opts[HELP]="-h";
	short_opts[SHOW_GRID]="-g";
	short_opts[SHOW_DELIMITERS]="-l";
	short_opts[IGNORE_DUPLICATES]="-I";
	short_opts[CONN_ALIAS]="-c";
	short_opts[HOST]="-H";
	short_opts[PORT]="-P";
	short_opts[USER]="-u";
	short_opts[PASSWD]="-w";
	short_opts[INITIAL_DB]="-D";
	short_opts[SILENT]="-s";
	short_opts[LIST_CONNS]="-L";
}

bool PgModelerCLI::isOptionRecognized(QString &op, bool &accepts_val)
{
	map<QString, QString>::iterator itr=short_opts.begin();
	bool found=false;

	while(itr!=short_opts.end() && !found)
	{
		found=(op==itr->first || op==itr->second);
		accepts_val=(found && long_opts[itr->first]);
		if(found)	op=itr->first;
		itr++;
	}

	return(found);
}

void PgModelerCLI::showMenu(void)
{
	out << endl;
	out << trUtf8("Usage: pgmodeler-cli [OPTIONS]") << endl;
	out << "pgModeler " << GlobalAttributes::PGMODELER_VERSION << trUtf8(" command line interface.") << endl;
	out << trUtf8("PostgreSQL Database Modeler Project - pgmodeler.com.br") << endl;
	out << trUtf8("Copyright 2006-2013 Raphael A. Silva <rkhaotix@gmail.com>") << endl;
	out << endl;
	out << trUtf8("This tool provides a way to export pgModeler's database models without\n\
the need to load them on graphical interface. All available exporting\n\
modes are described below.") << endl;
	out << endl;
	out << trUtf8("General options: ") << endl;
	out << trUtf8("   %1, %2=[FILE]\t\t Input model file (.dbm).").arg(short_opts[INPUT]).arg(INPUT) << endl;
	out << trUtf8("   %1, %2=[FILE]\t\t Output file. Available only on export to file or png.").arg(short_opts[OUTPUT]).arg(OUTPUT) << endl;
	out << trUtf8("   %1, %2\t\t Export to a sql script file.").arg(short_opts[EXPORT_TO_FILE]).arg(EXPORT_TO_FILE)<< endl;
	out << trUtf8("   %1, %2\t\t Export to a png image.").arg(short_opts[EXPORT_TO_PNG]).arg(EXPORT_TO_PNG) << endl;
	out << trUtf8("   %1, %2\t\t Export directly to a PostgreSQL server.").arg(short_opts[EXPORT_TO_DBMS]).arg(EXPORT_TO_DBMS) << endl;
	out << trUtf8("   %1, %2\t\t List available connections on %3 file.").arg(short_opts[LIST_CONNS]).arg(LIST_CONNS).arg(GlobalAttributes::CONNECTIONS_CONF + GlobalAttributes::CONFIGURATION_EXT) << endl;
	out << trUtf8("   %1, %2\t\t Version of generated SQL code. Only for file or dbms export.").arg(short_opts[PGSQL_VER]).arg(PGSQL_VER) << endl;
	out << trUtf8("   %1, %2\t\t\t Silent execution. Only critical errors are shown during process.").arg(short_opts[SILENT]).arg(SILENT) << endl;
	out << trUtf8("   %1, %2\t\t\t Show this help menu.").arg(short_opts[HELP]).arg(HELP) << endl;
	out << endl;
	out << trUtf8("PNG export options: ") << endl;
	out << trUtf8("   %1, %2\t\t Draws the grid on the exported png image.").arg(short_opts[SHOW_GRID]).arg(SHOW_GRID) << endl;
	out << trUtf8("   %1, %2\t Draws the page delimiters on the exported png image.").arg(short_opts[SHOW_DELIMITERS]).arg(SHOW_DELIMITERS) << endl;
	out << endl;
	out << trUtf8("DBMS export options: ") << endl;
	out << trUtf8("   %1, %2\t Ignores errors related to duplicated objects that eventually exists on server side.").arg(short_opts[IGNORE_DUPLICATES]).arg(IGNORE_DUPLICATES) << endl;
	out << trUtf8("   %1, %2=[ALIAS]\t Connection configuration alias to be used.").arg(short_opts[CONN_ALIAS]).arg(CONN_ALIAS) << endl;
	out << trUtf8("   %1, %2=[HOST]\t\t PostgreSQL host which export will operate.").arg(short_opts[HOST]).arg(HOST) << endl;
	out << trUtf8("   %1, %2=[PORT]\t\t PostgreSQL host listening port.").arg(short_opts[PORT]).arg(PORT) << endl;
	out << trUtf8("   %1, %2=[USER]\t\t PosrgreSQL username.").arg(short_opts[USER]).arg(USER) << endl;
	out << trUtf8("   %1, %2=[PASSWORD]\t PosrgreSQL user password.").arg(short_opts[PASSWD]).arg(PASSWD) << endl;
	out << trUtf8("   %1, %2=[DBNAME]\t Connection's initial database.").arg(short_opts[INITIAL_DB]).arg(INITIAL_DB) << endl;
	out << endl;
}

void PgModelerCLI::parserOptions(map<QString, QString> &opts)
{
	//Loading connections
	if(opts.count(LIST_CONNS) || opts.count(EXPORT_TO_DBMS))
	{
		conn_conf.loadConfiguration();
		conn_conf.getConnections(connections);
	}

	if(opts.empty() || opts.count(HELP))
		showMenu();
	//Listing connections
	else if(opts.count(LIST_CONNS))
	{
		map<QString, DBConnection *>::iterator itr=connections.begin();

		if(connections.empty())
			out << endl <<  trUtf8("There is no connections configured.") << endl << endl;
		else
		{
			out << endl << trUtf8("Available connections (alias : conn. string)") << endl;
			while(itr != connections.end())
			{
				out << itr->first << " : " << itr->second->getConnectionString() << endl;
				itr++;
			}
			out << endl;
		}
	}
	else
	{
		int mode_cnt=0;

		//Checking if multiples export modes were specified
		mode_cnt+=opts.count(EXPORT_TO_FILE);
		mode_cnt+=opts.count(EXPORT_TO_PNG);
		mode_cnt+=opts.count(EXPORT_TO_DBMS);

		if(mode_cnt==0)
			throw Exception(trUtf8("No export mode specified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(mode_cnt > 1)
			throw Exception(trUtf8("Multiple export mode especified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else	if(opts[INPUT].isEmpty())
			throw Exception(trUtf8("No input file specified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else	if(!opts.count(EXPORT_TO_DBMS) && opts[OUTPUT].isEmpty())
			throw Exception(trUtf8("No output file specified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(!opts.count(EXPORT_TO_DBMS) && opts[INPUT]==opts[OUTPUT])
			throw Exception(trUtf8("Input file must be different from output!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(opts.count(EXPORT_TO_DBMS) && !opts.count(CONN_ALIAS) &&
						 (!opts.count(HOST) || !opts.count(USER) || !opts.count(PASSWD) || !opts.count(INITIAL_DB)) )
			throw Exception(trUtf8("Incomplete connection information!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    //Converting input and output files to absolute paths to avoid that they are read/written on the app's working dir
    opts[INPUT]=QFileInfo(opts[INPUT]).absoluteFilePath();
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
      //Switch the app working dir in order to make it find the dependencies paths (./schemas, ./conf, etc)
      QDir::setCurrent(executable_dir);

			if(!silent_mode)
			{
				out << endl << "pgModeler " << GlobalAttributes::PGMODELER_VERSION << trUtf8(" command line interface.") << endl;
				out << trUtf8("Starting model export...") << endl;
				out << trUtf8("Loading input file: ") << parsed_opts[INPUT] << endl;
			}

			//Create the systems objects on model before loading it
			model->createSystemObjects(false);

			//Load the model file
			model->loadModel(parsed_opts[INPUT]);

			//Export to PNG
			if(parsed_opts.count(EXPORT_TO_PNG))
			{
				if(!silent_mode)
					out << trUtf8("Export to PNG image: ") << parsed_opts[OUTPUT] << endl;

				export_hlp.exportToPNG(scene, parsed_opts[OUTPUT],
															 parsed_opts.count(SHOW_GRID) > 0,
															 parsed_opts.count(SHOW_DELIMITERS) > 0);
			}
			//Export to SQL file
			else if(parsed_opts.count(EXPORT_TO_FILE))
			{
				if(!silent_mode)
					out << trUtf8("Export to SQL script file: ") << parsed_opts[OUTPUT] << endl;

				export_hlp.exportToSQL(model, parsed_opts[OUTPUT], parsed_opts[PGSQL_VER]);
			}
			//Export to DBMS
			else
			{
				if(!silent_mode)
					out << trUtf8("Export to DBMS: ") <<  connection.getConnectionString() << endl;

				export_hlp.exportToDBMS(model, connection, parsed_opts[PGSQL_VER], !parsed_opts[IGNORE_DUPLICATES].isEmpty());
			}

			if(!silent_mode)
				out << trUtf8("Export successfully ended!") << endl << endl;
		}

		return(0);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PgModelerCLI::updateProgress(int progress, QString msg)
{
	if(progress > 0)
		out << msg <<  "(" << progress << "%)" << endl;
	else
		out << msg << endl;
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
			case OBJ_TABLE:
				item=new TableView(dynamic_cast<Table *>(graph_obj));
			break;

			case OBJ_VIEW:
				item=new GraphicalView(dynamic_cast<View *>(graph_obj));
			break;

			case OBJ_RELATIONSHIP:
			case BASE_RELATIONSHIP:
				item=new RelationshipView(dynamic_cast<BaseRelationship *>(graph_obj)); break;
			break;

			case OBJ_SCHEMA:
				item=new SchemaView(dynamic_cast<Schema *>(graph_obj)); break;
			break;

			default:
				item=new TextboxView(dynamic_cast<Textbox *>(graph_obj)); break;
			break;
		}

		scene->addItem(item);

		if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}
