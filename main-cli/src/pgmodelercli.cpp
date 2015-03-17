/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
const QString PgModelerCLI::INPUT=QString("--input");
const QString PgModelerCLI::OUTPUT=QString("--output");
const QString PgModelerCLI::EXPORT_TO_FILE=QString("--export-to-file");
const QString PgModelerCLI::EXPORT_TO_PNG=QString("--export-to-png");
const QString PgModelerCLI::EXPORT_TO_DBMS=QString("--export-to-dbms");
const QString PgModelerCLI::DROP_DATABASE=QString("--drop-database");
const QString PgModelerCLI::DROP_OBJECTS=QString("--drop-objects");
const QString PgModelerCLI::PGSQL_VER=QString("--pgsql-ver");
const QString PgModelerCLI::HELP=QString("--help");
const QString PgModelerCLI::SHOW_GRID=QString("--show-grid");
const QString PgModelerCLI::SHOW_DELIMITERS=QString("--show-delimiters");
const QString PgModelerCLI::PAGE_BY_PAGE=QString("--page-by-page");
const QString PgModelerCLI::IGNORE_DUPLICATES=QString("--ignore-duplicates");
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
		parseOptions(opts);

		if(!parsed_opts.empty())
		{
			model=new DatabaseModel;
			xmlparser=model->getXMLParser();
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
					connection.setConnectionParam(Connection::PARAM_SERVER_FQDN, parsed_opts[HOST]);
					connection.setConnectionParam(Connection::PARAM_USER, parsed_opts[USER]);
					connection.setConnectionParam(Connection::PARAM_PORT, parsed_opts[PORT]);
					connection.setConnectionParam(Connection::PARAM_PASSWORD, parsed_opts[PASSWD]);
					connection.setConnectionParam(Connection::PARAM_DB_NAME, parsed_opts[INITIAL_DB]);
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
	long_opts[DROP_DATABASE]=false;
  long_opts[DROP_OBJECTS]=false;
	long_opts[PGSQL_VER]=true;
	long_opts[HELP]=false;
	long_opts[SHOW_GRID]=false;
	long_opts[SHOW_DELIMITERS]=false;
  long_opts[PAGE_BY_PAGE]=false;
	long_opts[IGNORE_DUPLICATES]=false;
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

  short_opts[INPUT]=QString("-i");
  short_opts[OUTPUT]=QString("-o");
  short_opts[EXPORT_TO_FILE]=QString("-f");
  short_opts[EXPORT_TO_PNG]=QString("-p");
  short_opts[EXPORT_TO_DBMS]=QString("-d");
  short_opts[DROP_DATABASE]=QString("-T");
  short_opts[DROP_OBJECTS]=QString("-J");
  short_opts[PGSQL_VER]=QString("-v");
  short_opts[HELP]=QString("-h");
  short_opts[SHOW_GRID]=QString("-g");
  short_opts[SHOW_DELIMITERS]=QString("-l");
  short_opts[PAGE_BY_PAGE]=QString("-b");
  short_opts[IGNORE_DUPLICATES]=QString("-I");
  short_opts[CONN_ALIAS]=QString("-c");
  short_opts[HOST]=QString("-H");
  short_opts[PORT]=QString("-P");
  short_opts[USER]=QString("-u");
  short_opts[PASSWD]=QString("-w");
  short_opts[INITIAL_DB]=QString("-D");
  short_opts[SILENT]=QString("-s");
  short_opts[LIST_CONNS]=QString("-L");
  short_opts[SIMULATE]=QString("-S");
  short_opts[FIX_MODEL]=QString("-F");
  short_opts[FIX_TRIES]=QString("-t");
  short_opts[ZOOM_FACTOR]=QString("-z");
  short_opts[USE_TMP_NAMES]=QString("-n");
}

bool PgModelerCLI::isOptionRecognized(QString &op, bool &accepts_val)
{
	attribs_map::iterator itr=short_opts.begin();
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
  out << QString("pgModeler ") << GlobalAttributes::PGMODELER_VERSION << trUtf8(" command line interface.") << endl;
	out << trUtf8("PostgreSQL Database Modeler Project - pgmodeler.com.br") << endl;
  out << trUtf8("Copyright 2006-2015 Raphael A. Silva <raphael@pgmodeler.com.br>") << endl;
	out << endl;
	out << trUtf8("This CLI tool provides the operations to export pgModeler's database models without\n\
the need to load them on graphical interface as well to fix model files to the most recent\n\
accepted structure. All available options are described below.") << endl;
	out << endl;
	out << trUtf8("General options: ") << endl;
	out << trUtf8("   %1, %2=[FILE]\t\t Input model file (.dbm). Mandatory use when fixing a model or exporting it.").arg(short_opts[INPUT]).arg(INPUT) << endl;
	out << trUtf8("   %1, %2=[FILE]\t\t Output file. Mandatory use when fixing model or export to file or png.").arg(short_opts[OUTPUT]).arg(OUTPUT) << endl;
  out << trUtf8("   %1, %2\t\t Try to fix the structure of the input model file in order to make it loadable again.").arg(short_opts[FIX_MODEL]).arg(FIX_MODEL) << endl;
	out << trUtf8("   %1, %2\t\t Model fix tries. When reaching the maximum count the invalid objects will be discard.").arg(short_opts[FIX_TRIES]).arg(FIX_TRIES) << endl;
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
  out << trUtf8("   %1, %2\t\t Each page will be exported on a separated png image.").arg(short_opts[PAGE_BY_PAGE]).arg(PAGE_BY_PAGE) << endl;
  out << trUtf8("   %1, %2=[FACTOR]\t\t Applies a zoom (in percent) before export to png image. Accepted zoom interval: %3-%4").arg(short_opts[ZOOM_FACTOR]).arg(ZOOM_FACTOR).arg(ModelWidget::MINIMUM_ZOOM*100).arg(ModelWidget::MAXIMUM_ZOOM*100) << endl;
	out << endl;
	out << trUtf8("DBMS export options: ") << endl;
	out << trUtf8("   %1, %2\t Ignores errors related to duplicated objects that eventually exists on server side.").arg(short_opts[IGNORE_DUPLICATES]).arg(IGNORE_DUPLICATES) << endl;
	out << trUtf8("   %1, %2\t\t Drop the database before execute a export process.").arg(short_opts[DROP_DATABASE]).arg(DROP_DATABASE) << endl;
  out << trUtf8("   %1, %2\t\t Runs the DROP commands attached to SQL-enabled objects.").arg(short_opts[DROP_OBJECTS]).arg(DROP_OBJECTS) << endl;
	out << trUtf8("   %1, %2\t\t Simulates a export process. Actually executes all steps but undoing any modification.").arg(short_opts[SIMULATE]).arg(SIMULATE) << endl;
  out << trUtf8("   %1, %2\t\t Generates temporary names for database, roles and tablespaces when in simulation mode.").arg(short_opts[USE_TMP_NAMES]).arg(USE_TMP_NAMES) << endl;
  out << trUtf8("   %1, %2=[ALIAS]\t Connection configuration alias to be used.").arg(short_opts[CONN_ALIAS]).arg(CONN_ALIAS) << endl;
	out << trUtf8("   %1, %2=[HOST]\t\t PostgreSQL host which export will operate.").arg(short_opts[HOST]).arg(HOST) << endl;
	out << trUtf8("   %1, %2=[PORT]\t\t PostgreSQL host listening port.").arg(short_opts[PORT]).arg(PORT) << endl;
	out << trUtf8("   %1, %2=[USER]\t\t PosrgreSQL username.").arg(short_opts[USER]).arg(USER) << endl;
	out << trUtf8("   %1, %2=[PASSWORD]\t PosrgreSQL user password.").arg(short_opts[PASSWD]).arg(PASSWD) << endl;
	out << trUtf8("   %1, %2=[DBNAME]\t Connection's initial database.").arg(short_opts[INITIAL_DB]).arg(INITIAL_DB) << endl;
	out << endl;
}

void PgModelerCLI::parseOptions(attribs_map &opts)
{
  QString orig_work_dir=QDir::current().absolutePath();

  //Changing the current working dir to the executable's directory in
  QDir::setCurrent(this->applicationDirPath());

	//Loading connections
	if(opts.count(LIST_CONNS) || opts.count(EXPORT_TO_DBMS))
	{
		conn_conf.loadConfiguration();
		conn_conf.getConnections(connections, false);
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

			out << endl << trUtf8("Available connections (alias : conn. string)") << endl;
			while(itr != connections.end())
			{
        out << QString("[") << id++ <<  QString("] ") << itr->first << QString(" : ") << itr->second->getConnectionString() << endl;
				itr++;
			}
			out << endl;
		}
	}
	else
	{
		int mode_cnt=0;
    bool fix_model=(opts.count(FIX_MODEL) > 0);
    QFileInfo input_fi(opts[INPUT]), output_fi(opts[OUTPUT]);

		//Checking if multiples export modes were specified
		mode_cnt+=opts.count(EXPORT_TO_FILE);
		mode_cnt+=opts.count(EXPORT_TO_PNG);
		mode_cnt+=opts.count(EXPORT_TO_DBMS);

		if(opts.count(ZOOM_FACTOR))
			zoom=opts[ZOOM_FACTOR].toFloat()/static_cast<float>(100);

    if(!fix_model && mode_cnt==0)
			throw Exception(trUtf8("No export mode specified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    else if(!fix_model && mode_cnt > 1)
			throw Exception(trUtf8("Multiple export mode especified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else	if(opts[INPUT].isEmpty())
			throw Exception(trUtf8("No input file specified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else	if(!opts.count(EXPORT_TO_DBMS) && opts[OUTPUT].isEmpty())
			throw Exception(trUtf8("No output file specified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    else if(!opts.count(EXPORT_TO_DBMS) && input_fi.absoluteFilePath()==output_fi.absoluteFilePath())
			throw Exception(trUtf8("Input file must be different from output!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(opts.count(EXPORT_TO_DBMS) && !opts.count(CONN_ALIAS) &&
						 (!opts.count(HOST) || !opts.count(USER) || !opts.count(PASSWD) || !opts.count(INITIAL_DB)) )
			throw Exception(trUtf8("Incomplete connection information!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(opts.count(EXPORT_TO_PNG) && (zoom < ModelWidget::MINIMUM_ZOOM || zoom > ModelWidget::MAXIMUM_ZOOM))
			throw Exception(trUtf8("Invalid zoom specified!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    //Converting input and output files to absolute paths to avoid that they are read/written on the app's working dir
    QDir::setCurrent(orig_work_dir);
    opts[INPUT]=input_fi.absoluteFilePath();
    opts[OUTPUT]=output_fi.absoluteFilePath();

    //Changing the current working dir to the executable's directory in
    QDir::setCurrent(this->applicationDirPath());

		parsed_opts=opts;
	}
}

int PgModelerCLI::exec(void)
{
	try
	{
		if(!parsed_opts.empty())
    {
			if(!silent_mode)
			{
        out << endl << QString("pgModeler ") << GlobalAttributes::PGMODELER_VERSION << trUtf8(" command line interface.") << endl;

				if(parsed_opts.count(FIX_MODEL))
					out << trUtf8("Starting model fixing...") << endl;
				else
					out << trUtf8("Starting model export...") << endl;

				out << trUtf8("Loading input file: ") << parsed_opts[INPUT] << endl;
			}

			if(parsed_opts.count(FIX_MODEL))
			{
        if(!silent_mode)
          out << trUtf8("Fixed model file: ") << parsed_opts[OUTPUT] << endl;

				extractObjectXML();
				recreateObjects();
				model->updateTablesFKRelationships();
				model->saveModel(parsed_opts[OUTPUT], SchemaParser::XML_DEFINITION);

				if(!silent_mode)
					out << trUtf8("Model successfully fixed!") << endl << endl;
			}
			else
			{
				//Create the systems objects on model before loading it
				model->createSystemObjects(false);

				//Load the model file
				model->loadModel(parsed_opts[INPUT]);

				//Export to PNG
				if(parsed_opts.count(EXPORT_TO_PNG))
				{
					if(!silent_mode)
						out << trUtf8("Export to PNG image: ") << parsed_opts[OUTPUT] << endl;

					export_hlp.exportToPNG(scene, parsed_opts[OUTPUT], zoom,
																 parsed_opts.count(SHOW_GRID) > 0,
                                 parsed_opts.count(SHOW_DELIMITERS) > 0,
                                 parsed_opts.count(PAGE_BY_PAGE) > 0);
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

          export_hlp.exportToDBMS(model, connection, parsed_opts[PGSQL_VER],
                                  parsed_opts.count(IGNORE_DUPLICATES) > 0,
                                  parsed_opts.count(DROP_DATABASE) > 0,
                                  parsed_opts.count(DROP_OBJECTS) > 0,
                                  parsed_opts.count(SIMULATE) > 0,
                                  parsed_opts.count(USE_TMP_NAMES) > 0);
				}

				if(!silent_mode)
					out << trUtf8("Export successfully ended!") << endl << endl;
			}
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
    out << msg <<  QString("(") << progress << QString("%)") << endl;
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
        item=new StyledTextboxView(dynamic_cast<Textbox *>(graph_obj)); break;
			break;
		}

		scene->addItem(item);

		if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}
}

void PgModelerCLI::extractObjectXML(void)
{
	QFile input;
  QString buf, lin, def_xml, end_tag;
	QTextStream ts;
  QRegExp regexp(QString("^(\\<\\?xml)(.)*(\\<%1)( )*").arg(ParsersAttributes::DB_MODEL)),
      default_obj=QRegExp(QString("(default)(\\-)(schema|owner|collation|tablespace)"));
  int start=-1, end=-1;
  bool open_tag=false, close_tag=false, is_rel=false, short_tag=false, end_extract_rel;

	if(!silent_mode)
		out << trUtf8("Extracting objects' XML...") << endl;

	input.setFileName(parsed_opts[INPUT]);
	input.open(QFile::ReadOnly);

	if(!input.isOpen())
		throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(parsed_opts[INPUT]),
										ERR_FILE_DIR_NOT_ACCESSED,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	buf.append(input.readAll());
	input.close();

	//Check if the file contains a valid header (for .dbm file)
	start=regexp.indexIn(buf);

	if(start < 0)
		throw Exception(trUtf8("Invalid input file! It seems that is not a pgModeler generated model or the file is corrupted!"), ERR_CUSTOM,__PRETTY_FUNCTION__,__FILE__,__LINE__);
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
          <tag></tag> in order to be correctly extracted further. Currently only language has this
          behaviour, so additional object may be added in the future. */
      if(lin.contains(QString("<%1").arg(BaseObject::getSchemaName(OBJ_LANGUAGE))))
      {
        lin=lin.simplified();

        if(lin.contains(QString("/>")))
          lin.replace(QString("/>"), QString("></%1>").arg(BaseObject::getSchemaName(OBJ_LANGUAGE)));
      }

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
  ObjectType obj_type=BASE_OBJECT;
  vector<ObjectType> types={ OBJ_INDEX, OBJ_TRIGGER, OBJ_RULE };
	attribs_map attribs;
	bool use_fail_obj=false;
	unsigned tries=0, max_tries=parsed_opts[FIX_TRIES].toUInt();
  int start_pos=-1, end_pos=-1, len=0;

	if(!silent_mode)
		out << trUtf8("Recreating objects...") << endl;

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

			if(obj_type==OBJ_DATABASE)
				model->configureDatabase(attribs);
			else
			{
				if(obj_type==OBJ_TABLE)
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
				if(obj_type!=OBJ_RELATIONSHIP ||
					 (obj_type==OBJ_RELATIONSHIP && !xml_def.contains(QString("\"%1\"").arg(ParsersAttributes::RELATIONSHIP_FK))))
				{
					object=model->createObject(obj_type);

					if(object)
					{
						if(!dynamic_cast<TableObject *>(object) && obj_type!=OBJ_RELATIONSHIP && obj_type!=BASE_RELATIONSHIP)
							model->addObject(object);
					}

					//For each sucessful created object the method will try to create a failed one
					use_fail_obj=(!fail_objs.isEmpty());
				}

        /* Additional step to extract indexes/triggers/rules from within tables/views
           and putting their xml on the list of object to be created */
        if((obj_type==OBJ_TABLE || obj_type==OBJ_VIEW) &&
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
			if(obj_type!=OBJ_DATABASE)
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
				if(!silent_mode)
					out << trUtf8("WARNING: There are objects that maybe can't be fixed. Trying again... (tries %1/%2)").arg(tries).arg(max_tries) << endl;

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
  QString tag=QString("<%1"), end_tag=QString("</%1"),
      att_regexp=QString("(%1)( )*(=)(\")(\\w|\\d|,|\\.|\\&\\;)+(\")");

	//Placing objects <index>, <rule>, <trigger> outside of <table>
	if(!obj_xml.startsWith(tag.arg(BaseObject::getSchemaName(OBJ_TABLESPACE))) &&
		 obj_xml.startsWith(tag.arg(BaseObject::getSchemaName(OBJ_TABLE))))
	{
		int start_idx=-1, end_idx=-1, len=0;
		ObjectType obj_types[3]={ OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX  };
		QString  curr_tag, curr_end_tag, def, tab_name, sch_name,
        name_attr=QString("name=\""),
        sch_name_attr=tag.arg(BaseObject::getSchemaName(OBJ_SCHEMA)) + QString(" ") + name_attr;

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
			curr_tag=tag.arg(BaseObject::getSchemaName(obj_types[idx]));
      curr_end_tag=end_tag.arg(BaseObject::getSchemaName(obj_types[idx])) + QString(">");
			start_idx=obj_xml.indexOf(curr_tag);

			while(start_idx >=0)
			{
				end_idx=obj_xml.indexOf(curr_end_tag);
				len=(end_idx - start_idx) + curr_end_tag.size();
        def=obj_xml.mid(start_idx, len) + QString("\n\n");
				obj_xml.remove(start_idx, len);

				//If the object is a rule include the table attribute
				if(def.startsWith(tag.arg(BaseObject::getSchemaName(OBJ_RULE))))
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
	if(obj_xml.contains(tag.arg(ParsersAttributes::ELEMENT)))
    obj_xml.remove(QRegExp(att_regexp.arg(QString("recheck"))));

	//Remove values greater-op, less-op, sort-op or sort2-op from ref-type attribute from <operator> tags.
	if(obj_xml.contains(tag.arg(BaseObject::getSchemaName(OBJ_OPERATOR))))
	{
    obj_xml.remove(QString("greater-op"));
    obj_xml.remove(QString("less-op"));
    obj_xml.remove(QString("sort-op"));
    obj_xml.remove(QString("sort2-op"));
	}

	//Replacing attribute owner by onwer-col for sequences
	if(obj_xml.contains(tag.arg(BaseObject::getSchemaName(OBJ_SEQUENCE))))
		obj_xml.replace(ParsersAttributes::OWNER, ParsersAttributes::OWNER_COLUMN);

	//Remove sysid attribute from <role> tags.
	if(obj_xml.contains(tag.arg(BaseObject::getSchemaName(OBJ_ROLE))))
    obj_xml.remove(QRegExp(att_regexp.arg(QString("sysid"))));

	//Replace <parameter> tag by <typeattrib> on <usertype> tags.
  if(obj_xml.contains(tag.arg(QString("usertype"))))
	{
		obj_xml.replace(tag.arg(ParsersAttributes::PARAMETER), tag.arg(ParsersAttributes::TYPE_ATTRIBUTE));
		obj_xml.replace(end_tag.arg(ParsersAttributes::PARAMETER), end_tag.arg(ParsersAttributes::TYPE_ATTRIBUTE));
	}

	if(obj_xml.contains(tag.arg(BaseObject::getSchemaName(OBJ_RELATIONSHIP))))
	{
		//Remove auto-sufix, src-sufix, dst-sufix, col-indexes, constr-indexes, attrib-indexes from <relationship> tags.
    obj_xml.remove(QRegExp(att_regexp.arg(QString("auto-sufix"))));
    obj_xml.remove(QRegExp(att_regexp.arg(QString("src-sufix"))));
    obj_xml.remove(QRegExp(att_regexp.arg(QString("dst-sufix"))));
    obj_xml.remove(QRegExp(att_regexp.arg(QString("col-indexes"))));
    obj_xml.remove(QRegExp(att_regexp.arg(QString("constr-indexes"))));
    obj_xml.remove(QRegExp(att_regexp.arg(QString("attrib-indexes"))));

    obj_xml.replace(QString("line-color"), ParsersAttributes::CUSTOM_COLOR);
	}

  //Renaming the tag <condition> to <predicate> on indexes
  if(obj_xml.contains(tag.arg(BaseObject::getSchemaName(OBJ_INDEX))))
  {
    obj_xml.replace(tag.arg(ParsersAttributes::CONDITION), tag.arg(ParsersAttributes::PREDICATE));
    obj_xml.replace(end_tag.arg(ParsersAttributes::CONDITION), end_tag.arg(ParsersAttributes::PREDICATE));
  }

	//Renaming the attribute default to default-value on domain
	if(obj_xml.contains(tag.arg(BaseObject::getSchemaName(OBJ_DOMAIN))))
		obj_xml.replace(ParsersAttributes::DEFAULT, ParsersAttributes::DEFAULT_VALUE);

	//Renaming the tag <grant> to <permission>
  if(obj_xml.contains(tag.arg(QString("grant"))))
	{
    obj_xml.replace(tag.arg(QString("grant")), tag.arg(BaseObject::getSchemaName(OBJ_PERMISSION)));
    obj_xml.replace(end_tag.arg(QString("grant")), end_tag.arg(BaseObject::getSchemaName(OBJ_PERMISSION)));
	}
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

void PgModelerCLI::updateFileAssociation(bool remove)
{
 SchemaParser schparser;
 Messagebox msg_box;
 QString title=trUtf8("File association missing"),
         msg=trUtf8("It seems that .dbm files aren't associated with pgModeler. Do you want to do it now?");

 #ifdef Q_OS_LINUX
  attribs_map attribs;
  QString str_aux,

       //Configures the path to the application logo
       exec_icon=QDir(GlobalAttributes::CONFIGURATIONS_DIR +
                      GlobalAttributes::DIR_SEPARATOR + QString("pgmodeler_logo.png")).absolutePath(),

       //Configures the path to the document logo
       dbm_icon=QDir(GlobalAttributes::CONFIGURATIONS_DIR +
                     GlobalAttributes::DIR_SEPARATOR + QString("pgmodeler_dbm.png")).absolutePath(),

       //Path to directory that register mime types
       mime_db_dir=QDir::homePath() + QString("/.local/share/mime"),

       //Path to the file that associates apps to mimetypes
       mimeapps=QDir::homePath() + QString("/.local/share/applications/mimeapps.list"),

       base_conf_dir=GlobalAttributes::CONFIGURATIONS_DIR + GlobalAttributes::DIR_SEPARATOR +
                     GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR,

       //Files generated after update file association (application-dbm.xml and pgModeler.desktop)
       files[] = { QDir::homePath() + QString("/.local/share/applications/pgModeler.desktop"),
                   mime_db_dir + QString("/packages/application-dbm.xml") },

       schemas[] = { base_conf_dir + QString("desktop") + GlobalAttributes::SCHEMA_EXT,
                     base_conf_dir + QString("application-dbm") + GlobalAttributes::SCHEMA_EXT };
  QByteArray buf, buf_aux;
  QFile out;

  //Check if the necessary file exists. If not asks the user to update file association
  if(!QFileInfo(files[0]).exists() || !QFileInfo(files[1]).exists())
    msg_box.show(title, msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

  if(msg_box.result()==QDialog::Accepted)
  {
    //file_associated=true;
    attribs[ParsersAttributes::ROOT_DIR]=QApplication::applicationDirPath();
    attribs[ParsersAttributes::ICON]=exec_icon;

    try
    {
      for(unsigned i=0; i < 2; i++)
      {
        schparser.loadFile(schemas[i]);
        buf.append(schparser.getCodeDefinition(attribs));
        QDir(QString(".")).mkpath(QFileInfo(files[i]).absolutePath());

        out.setFileName(files[i]);
        out.open(QFile::WriteOnly);

        if(!out.isOpen())
          throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_WRITTEN).arg(files[i]),
                          ERR_FILE_DIR_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

        out.write(buf.data(), buf.size());
        out.close();
        buf.clear();
        attribs[ParsersAttributes::ICON]=dbm_icon;
      }

      out.setFileName(mimeapps);

      //If the file mimeapps.list doesn't exists (generally in Ubuntu) creates a new one
      if(!QFileInfo(mimeapps).exists())
      {
        out.open(QFile::WriteOnly);
        out.write(QByteArray("[Added Associations]\napplication/dbm=pgModeler.desktop;\n"));
        out.close();
      }
      else
      {

        out.open(QFile::ReadOnly);

        if(!out.isOpen())
          throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_WRITTEN).arg(mimeapps),
                          ERR_FILE_DIR_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
            if(str_aux.contains(QString("[Added Associations]")))
              str_aux.append(QString("\napplication/dbm=pgModeler.desktop;\n"));
            else
              str_aux+=QString("\n");

            buf_aux.append(str_aux);
          }
        }

        //Write a new copy of the mimeapps.list file
        out.open(QFile::Truncate | QFile::WriteOnly);
        out.write(buf_aux.data(), buf_aux.size());
        out.close();
      }

      //Update the mime database
      QProcess::execute(QString("update-mime-database"), QStringList { mime_db_dir });
    }
    catch(Exception &e)
    {
      msg_box.show(e);
    }
  }
 #else
    #ifdef Q_OS_WIN
     //Checking if the .dbm registry key exists
     QSettings dbm_ext(QString("HKEY_CURRENT_USER\\Software\\Classes\\.dbm"), QSettings::NativeFormat);
     QString exe_path=QDir::toNativeSeparators(QApplication::applicationDirPath() + QString("\\pgmodeler.exe"));

     //If there is no value assigned to .dbm/Default key shows the update extension confirmation message
     if(dbm_ext.value(QString("Default")).toString().isEmpty())
       msg_box.show(title, msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Accepted)
      {
        //Write the default value for .dbm registry key
        dbm_ext.setValue(QString("Default"), QString("dbm_auto_file"));
        dbm_ext.sync();

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
          for(int i=0; i < itr->second.size(); i+=2)
            s.setValue(itr->second[i], itr->second[i+1]);

          s.sync();
          itr++;
        }
     }
    #endif
#endif
}
