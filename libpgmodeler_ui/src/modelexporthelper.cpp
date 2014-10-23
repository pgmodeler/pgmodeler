#include "modelexporthelper.h"

ModelExportHelper::ModelExportHelper(QObject *parent) : QObject(parent)
{
	sql_gen_progress=progress=0;
  db_created=ignore_dup=drop_db=export_canceled=false;
  simulate=use_tmp_names=db_sql_reenabled=false;
	created_objs[OBJ_ROLE]=created_objs[OBJ_TABLESPACE]=-1;
	db_model=nullptr;
	connection=nullptr;
}

void ModelExportHelper::exportToSQL(DatabaseModel *db_model, const QString &filename, const QString &pgsql_ver)
{
	if(!db_model)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)));

	try
	{
		progress=sql_gen_progress=0;
		BaseObject::setPgSQLVersion(pgsql_ver);
		emit s_progressUpdated(progress,
													 trUtf8("PostgreSQL %1 version code generation.").arg(BaseObject::getPgSQLVersion()),
													 OBJ_DATABASE);
		progress=1;
		db_model->saveModel(filename, SchemaParser::SQL_DEFINITION);

		emit s_exportFinished();
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	disconnect(db_model, nullptr, this, nullptr);
}

void ModelExportHelper::exportToPNG(ObjectsScene *scene, const QString &filename, float zoom, bool show_grid, bool show_delim, bool page_by_page)
{
	if(!scene)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		QPixmap pix;
		bool shw_grd, shw_dlm, align_objs;
		QGraphicsView viewp(scene);
		QRect retv;
		QPolygon pol;
    vector<QRectF> pages;
    unsigned v_cnt=0, h_cnt=0, page_idx=1;
    QString tmpl_filename, file;

		//Clear the object scene selection to avoid drawing the selectoin rectangle of the objects
		scene->clearSelection();

		//Make a backup of the current scene options
		ObjectsScene::getGridOptions(shw_grd, align_objs, shw_dlm);

		//Sets the options passed by the user
		ObjectsScene::setGridOptions(show_grid, false, show_delim);

    if(page_by_page)
    {
      QPrinter::Orientation orient;
      QRectF margins;
      QSizeF custom_sz, page_sz;
      QPrinter::PaperSize paper_sz;
      QPrinter prt;
      QFileInfo fi(filename);

      ObjectsScene::getPaperConfiguration(paper_sz, orient, margins, custom_sz);

      if(paper_sz==QPrinter::Custom)
        page_sz=custom_sz;
      else
      {
        prt.setPaperSize(paper_sz);
        prt.setOrientation(orient);
        page_sz=prt.paperSize(QPrinter::Point);
      }

      //Calculates the page count to be exported
      pages=scene->getPagesForPrinting(page_sz, margins.size(), h_cnt, v_cnt);

      //Configures the template filename for pages pixmaps
      tmpl_filename=fi.absolutePath() + GlobalAttributes::DIR_SEPARATOR + fi.baseName() + "_p%1." + fi.completeSuffix();
    }
    else
    {
      pages.push_back(scene->itemsBoundingRect(true));
      file=filename;
    }

		//Updates the scene to apply the change on grid and delimiter
		scene->update();

		//Configures the viewport alignment to top-left coordinates.
		viewp.setAlignment(Qt::AlignLeft | Qt::AlignTop);

		//Apply the zoom factor on the viewport
    viewp.resetTransform();
    viewp.centerOn(0,0);
    viewp.scale(zoom, zoom);

    QPainter painter;
    vector<QRectF>::iterator itr=pages.begin(), itr_end=pages.end();

    while(itr!=itr_end)
    {
      //Convert the objects bounding rect to viewport coordinates to correctly draw them onto pixmap
      pol=viewp.mapFromScene(*itr);
      itr++;

      //Configure the viewport area to be copied
      retv.setTopLeft(pol.at(0));
      retv.setTopRight(pol.at(1));
      retv.setBottomRight(pol.at(2));
      retv.setBottomLeft(pol.at(3));

      //Creates the output pixmap
      pix=QPixmap(retv.size());
      pix.fill();

      //Setting optimizations on the painter
      painter.begin(&pix);
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.setRenderHint(QPainter::TextAntialiasing, true);
      painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

      emit s_progressUpdated((page_idx/static_cast<float>(pages.size())) * 100,
                             trUtf8("Rendering objects onto the output pixmap."), BASE_OBJECT);

      //Render the entire viewport onto the pixmap
      viewp.render(&painter, QRectF(QPointF(0,0), pix.size()), retv);
      painter.end();

      if(page_by_page)
        file=tmpl_filename.arg(page_idx++);

      //If the pixmap is not saved raises an error
      if(!pix.save(file))
      {
        //Restoring the scene settings before throw error
        ObjectsScene::setGridOptions(shw_grd, align_objs, shw_dlm);
        scene->update();

        throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(Utf8String::create(file)),
                        ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
    }

    //Restoring the scene settings
    ObjectsScene::setGridOptions(shw_grd, align_objs, shw_dlm);
    scene->update();

		emit s_exportFinished();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelExportHelper::sleepThread(unsigned msecs)
{
	if(this->thread()!=qApp->thread())
		QThread::msleep(msecs);
}

void ModelExportHelper::exportToDBMS(DatabaseModel *db_model, Connection conn, const QString &pgsql_ver, bool ignore_dup, bool drop_db, bool simulate, bool use_tmp_names)
{
	int type_id;
	QString  version, sql_buf, sql_cmd, lin;
	Connection new_db_conn;
	unsigned i, count;
	ObjectType types[]={OBJ_ROLE, OBJ_TABLESPACE};
	BaseObject *object=nullptr;
	vector<Exception> errors;
	QTextStream ts;
	bool ddl_tk_found=false;

	/* Error codes treated in this method
			42P04 	duplicate_database
			42723 	duplicate_function
			42P06 	duplicate_schema
			42P07 	duplicate_table
			42710 	duplicate_object
			42701   duplicate_column
			42P16   invalid_table_definition

		 Reference:
			http://www.postgresql.org/docs/current/static/errcodes-appendix.html*/
	QString error_codes[]={"42P04", "42723", "42P06", "42P07", "42710", "42701", "42P16"};
	vector<QString> err_codes_vect(error_codes, error_codes + sizeof(error_codes) / sizeof(QString));

	try
	{
		if(!db_model)
			throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* If the export is called using ignore duplications or drop database and simulation mode at same time
		an error is raised because the simulate mode (mainly used as SQL validation) cannot
		undo column addition (this can be changed in the future) */
		if(simulate && (ignore_dup || drop_db))
			throw Exception(ERR_MIX_INCOMP_EXPORT_OPTS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)));

		export_canceled=false;
		db_created=false;
		progress=sql_gen_progress=0;
		created_objs[OBJ_ROLE]=created_objs[OBJ_TABLESPACE]=-1;

		//Retrive the DBMS version in order to generate the correct code
    conn.connect();
    version=(conn.getPgSQLVersion()).mid(0,3);

    emit s_progressUpdated(progress, trUtf8("Starting export to DBMS."));

		//Overriding the DBMS version case the version is informed on parameter
		if(!pgsql_ver.isEmpty())
		{
			BaseObject::setPgSQLVersion(pgsql_ver);
      emit s_progressUpdated(progress, trUtf8("PostgreSQL version detection overrided. Using version %1.").arg(pgsql_ver));
		}
		else
		{
			BaseObject::setPgSQLVersion(version);
      emit s_progressUpdated(progress, trUtf8("PostgreSQL %1 server detected.").arg(version));
		}

    if(simulate && use_tmp_names)
    {
      emit s_progressUpdated(progress, trUtf8("Generating temporary names for database, roles and tablespaces."));
      generateTempObjectNames(db_model);
    }
    else if(use_tmp_names)
      throw Exception(ERR_INV_USE_TMPNAMES_EXPORT_OPT,__PRETTY_FUNCTION__,__FILE__,__LINE__);


    if(simulate && db_model->isSQLDisabled())
    {
      db_model->setSQLDisabled(false);
      db_sql_reenabled=true;
      emit s_progressUpdated(progress, trUtf8("Enabling the SQL code for database `%1'' to avoid errors.").arg(db_model->getName()));
    }


		if(ignore_dup)
		{
      emit s_progressUpdated(progress, trUtf8("Ignoring object duplication errors."));

			//Save the current status for ALTER command generation for table columns/constraints
			saveGenAtlerCmdsStatus(db_model);
		}

		if(drop_db)
		{
      emit s_progressUpdated(progress, trUtf8("Trying to drop database `%1'.").arg(db_model->getName()));
			conn.executeDDLCommand(QString("DROP DATABASE IF EXISTS %1;").arg(db_model->getName(true)));
		}

		if(simulate)
      emit s_progressUpdated(progress, trUtf8("Simulation mode activated."));

		//Creates the roles and tablespaces separately from the other objects
		for(type_id=0; type_id < 2 && !export_canceled; type_id++)
		{
			count=db_model->getObjectCount(types[type_id]);

			for(i=0; i < count && !export_canceled; i++)
			{
				object=db_model->getObject(i, types[type_id]);
				progress=((10 * (type_id+1)) + ((i/static_cast<float>(count)) * 10));

				try
				{
					if(!object->isSQLDisabled())
					{
						//Emits a signal indicating that the object is being exported
						emit s_progressUpdated(progress,
                                   trUtf8("Creating object `%1' (%2).").arg(Utf8String::create(object->getName())).arg(object->getTypeName()),
																	 object->getObjectType());

						sql_cmd=object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
						conn.executeDDLCommand(sql_cmd);
					}
				}
				catch(Exception &e)
				{
					/* Raises an error if the object is duplicated and the ignore duplicity is not set or the error
					returned by the server is other than object duplicity */
					if(!ignore_dup ||
						 (ignore_dup &&
							std::find(err_codes_vect.begin(), err_codes_vect.end(), e.getExtraInfo())==err_codes_vect.end()))
						throw Exception(e.getErrorMessage(),
														e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, sql_cmd);
					else
						//If the object is duplicated store the error on a vector
						errors.push_back(e);
				}

				created_objs[types[type_id]]++;
			}
		}

		try
		{
			if(!db_model->isSQLDisabled() && !export_canceled)
			{
				//Creating the database on the DBMS
				emit s_progressUpdated(progress,
                               trUtf8("Creating database `%1'.").arg(Utf8String::create(db_model->getName())),
															 OBJ_DATABASE);
				sql_cmd=db_model->__getCodeDefinition(SchemaParser::SQL_DEFINITION);
				conn.executeDDLCommand(sql_cmd);
				db_created=true;
			}
		}
		catch(Exception &e)
		{
			/* Raises an error if the object is duplicated and the ignore duplicity is not set or the error
			returned by the server is other than object duplicity */
			if(!ignore_dup ||
				 (ignore_dup &&
					std::find(err_codes_vect.begin(), err_codes_vect.end(), e.getExtraInfo())==err_codes_vect.end()))
				throw Exception(e.getErrorMessage(),
												e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, sql_cmd);
			else
				errors.push_back(e);
		}

		if(!export_canceled)
		{
			//Connects to the new created database to create the other objects
			progress=20;
			new_db_conn=conn;
			new_db_conn.setConnectionParam(Connection::PARAM_DB_NAME, db_model->getName());
			emit s_progressUpdated(progress,
                             trUtf8("Connecting to database `%1'.").arg(Utf8String::create(db_model->getName())));

			new_db_conn.connect();
			progress=30;
			//Creating the other object types
      emit s_progressUpdated(progress, trUtf8("Creating objects on database `%1'.").arg(Utf8String::create(db_model->getName())));

			//Generates the sql from entire model
			sql_buf=db_model->getCodeDefinition(SchemaParser::SQL_DEFINITION, false);

			/* Extract each SQL command from the buffer and execute them separately. This is done
			 to permit the user, in case of error, identify what object is wrongly configured. */
			ts.setString(&sql_buf);
			unsigned aux_prog=0, curr_size=0, buf_size=sql_buf.size();

			progress+=(sql_gen_progress/progress);
			sql_cmd.clear();

			while(!ts.atEnd() && !export_canceled)
			{
				try
				{
					//Cleanup single line comments
					lin=ts.readLine();
					curr_size+=lin.size();
					aux_prog=progress + ((curr_size/static_cast<float>(buf_size)) * 70);

					ddl_tk_found=(lin.indexOf(ParsersAttributes::DDL_END_TOKEN) >= 0);
					lin.remove(QRegExp("^(--)+(.)+$"));

					//If the line isn't empty after cleanup it will be included on sql command
					if(!lin.isEmpty())
						sql_cmd += lin + "\n";

					//If the ddl end token is found
					if(ddl_tk_found || (!sql_cmd.isEmpty() && ts.atEnd()))
					{
						//Regexp used to extract the object being created
						QRegExp obj_reg("(CREATE)(.)+(\n)"),
										fk_reg("^(ALTER TABLE)(.)+(ADD CONSTRAINT)( )*");
						int pos=fk_reg.indexIn(sql_cmd), pos1=0;

						//Checking if the command is a fk creation via ALTER TABLE
						if(pos >= 0)
						{
							QString constr_name;
							pos+=fk_reg.matchedLength();
							pos1=sql_cmd.indexOf(' ', pos);
							constr_name=sql_cmd.mid(pos, pos1 - pos);

							emit s_progressUpdated(aux_prog,
																		 trUtf8("Creating object `%1' (%2).").arg(constr_name).arg(BaseObject::getTypeName(OBJ_CONSTRAINT)),
																		 OBJ_CONSTRAINT, sql_cmd);
						}
						//Check if the regex matches the sql command
						else if(obj_reg.exactMatch(sql_cmd))
						{
							QString obj_type, obj_name;
							QRegExp reg_aux;
							unsigned obj_id;
							ObjectType obj_types[]={ OBJ_FUNCTION, OBJ_TRIGGER, OBJ_INDEX,
																			 OBJ_RULE,	OBJ_TABLE, OBJ_VIEW, OBJ_DOMAIN,
																			 OBJ_SCHEMA,	OBJ_AGGREGATE, OBJ_OPFAMILY,
																			 OBJ_OPCLASS, OBJ_OPERATOR,  OBJ_SEQUENCE,
																			 OBJ_CONVERSION, OBJ_CAST,	OBJ_LANGUAGE,
																			 OBJ_COLLATION, OBJ_EXTENSION, OBJ_TYPE };
							unsigned count=sizeof(obj_types)/sizeof(ObjectType);

							//Get the fisrt line of the sql command, that contains the CREATE ... statement
							lin=sql_cmd.mid(0, sql_cmd.indexOf('\n'));

							for(obj_id=0; obj_id < count; obj_id++)
							{
								//Check if the keyword for the current object exists on string
								reg_aux.setPattern(QString("(CREATE)(.)*(%1)%2")
																	 .arg(BaseObject::getSQLName(obj_types[obj_id]))
																	 .arg(obj_types[obj_id]==OBJ_INDEX ? "( )*(CONCURRENTLY)?" : ""));
								pos=reg_aux.indexIn(lin);

								if(pos >= 0)
								{
									//Extracts from the line the string starting with the object's name
                  lin=lin.mid(reg_aux.matchedLength(), sql_cmd.indexOf('\n')).simplified();

									//Stores the object type name
									obj_type=BaseObject::getTypeName(obj_types[obj_id]);

									if(obj_types[obj_id]!=OBJ_CAST)
									{
                    int last_quote_idx=lin.lastIndexOf('"'),
                        first_quote_idx=lin.indexOf('"'),
                        spc_idx=lin.indexOf(' ');

                    if(spc_idx >=0 && spc_idx < first_quote_idx)
                      //Getting the name at the first space before the first quote
                      obj_name=lin.split(' ').at(0);
                    else if(last_quote_idx >=0)
                      //Getting the name until the last quote
                      obj_name=lin.mid(0, last_quote_idx +1);
                    else
                      //Gettring the name until the first space or end of string
                      obj_name=lin.mid(0, (spc_idx >= 0 ? spc_idx + 1 : lin.size()));

                    obj_name.remove('(');
									}
									else
									{
										obj_name="cast" + lin.replace(" AS ",",");
									}

									obj_name.remove(';');
									break;
								}
							}

							emit s_progressUpdated(aux_prog,
                                     trUtf8("Creating object `%1' (%2).").arg(obj_name).arg(obj_type),
																		 obj_types[obj_id], sql_cmd);
						}
						else if(sql_cmd.trimmed()!="")
						{
							//General commands like alter / set aren't explicitly shown
							emit s_progressUpdated(aux_prog, trUtf8("Executing auxiliary command."), BASE_OBJECT, sql_cmd);
						}

						//Executes the extracted SQL command
						if(!sql_cmd.isEmpty())
							new_db_conn.executeDDLCommand(sql_cmd);

						sql_cmd.clear();
						ddl_tk_found=false;
					}
				}
				catch(Exception &e)
				{
					if(ddl_tk_found) ddl_tk_found=false;

					if(!ignore_dup ||
						 (ignore_dup &&
							std::find(err_codes_vect.begin(), err_codes_vect.end(), e.getExtraInfo())==err_codes_vect.end()))
						throw Exception(Exception::getErrorMessage(ERR_EXPORT_FAILURE).arg(Utf8String::create(sql_cmd)),
														ERR_EXPORT_FAILURE,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, sql_cmd);
					else
					{
						sql_cmd.clear();
						errors.push_back(e);
						sleepThread(10);
					}
				}
			}
		}

		disconnect(db_model, nullptr, this, nullptr);

		if(ignore_dup)
			restoreGenAtlerCmdsStatus();

		//Closes the new opened connection
		if(new_db_conn.isStablished()) new_db_conn.close();

		/* If the process was a simulation or even canceled undo the export
		removing the created objects */
		if(simulate || export_canceled)
      undoDBMSExport(db_model, conn, use_tmp_names);

		if(conn.isStablished())
			conn.close();

		if(!export_canceled)
			emit s_exportFinished();
		else
			emit s_exportCanceled();

		sleepThread(20);
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);

		if(ignore_dup)
			restoreGenAtlerCmdsStatus();

		try
		{
			//Closes the new opened connection
			if(new_db_conn.isStablished()) new_db_conn.close();

			//Undo the export removing the created objects
      undoDBMSExport(db_model, conn, use_tmp_names);
		}
		catch(Exception &){}

		if(conn.isStablished())	conn.close();

		/* When running in a separated thread (other than the main application thread)
		redirects the error in form of signal */
		if(this->thread() && this->thread()!=qApp->thread())
		{
			errors.push_back(e);
			emit s_exportAborted(Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, errors));
		}
		else
		{
			//Redirects any error to the user
			if(errors.empty())
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			else
			{
				errors.push_back(e);
				throw Exception(e.getErrorMessage(),__PRETTY_FUNCTION__,__FILE__,__LINE__, errors);
			}
		}
	}
}

void ModelExportHelper::saveGenAtlerCmdsStatus(DatabaseModel *db_model)
{
	vector<BaseObject *> objects;
	Table *tab=nullptr;
	Relationship *rel=nullptr;

	objects.insert(objects.end(), db_model->getObjectList(OBJ_TABLE)->begin(),
								 db_model->getObjectList(OBJ_TABLE)->end());

	//Store the relationship on the auxiliary vector but only many-to-many are considered
	objects.insert(objects.end(), db_model->getObjectList(OBJ_RELATIONSHIP)->begin(),
								 db_model->getObjectList(OBJ_RELATIONSHIP)->end());

	alter_cmds_status.clear();

	while(!objects.empty())
	{
		rel=dynamic_cast<Relationship *>(objects.back());

		/* If the current object is a relationship try to get the
		generated table (if the relationship is many-to-many only) */
		if(rel)
			tab=rel->getGeneratedTable();
		else
			tab=dynamic_cast<Table *>(objects.back());

		if(tab)
		{
			//Store the current alter state
			alter_cmds_status[tab]=tab->isGenerateAlterCmds();
			//Forcing columns and constraints to be generated via ALTER command
			tab->setGenerateAlterCmds(true);
		}

		objects.pop_back();
	}
}

void ModelExportHelper::restoreGenAtlerCmdsStatus(void)
{
	map<Table *, bool>::iterator itr=alter_cmds_status.begin();

	while(itr!=alter_cmds_status.end())
	{
		itr->first->setGenerateAlterCmds(itr->second);
		itr++;
	}

	alter_cmds_status.clear();
}

void ModelExportHelper::undoDBMSExport(DatabaseModel *db_model, Connection &conn, bool use_tmp_names)
{
 QString drop_cmd=QString("DROP %1 %2;");
 ObjectType types[]={OBJ_ROLE, OBJ_TABLESPACE};
 int type_id;
 BaseObject *object=nullptr;

 //In case of error during the export all created object are removed
 if(db_created || created_objs[OBJ_ROLE] >= 0 || created_objs[OBJ_TABLESPACE] >= 0)
 {
   emit s_progressUpdated(99, trUtf8("Destroying objects created on the server."));

	 //Dropping the database
	 if(db_created)
		 conn.executeDDLCommand(drop_cmd.arg(db_model->getSQLName()).arg(db_model->getName(true)));

	 //Drop the roles / tablespaces created
	 for(type_id=1; type_id >=0; type_id--)
	 {
		 while(created_objs[types[type_id]] >= 0)
		 {
			 object=db_model->getObject(created_objs[types[type_id]], types[type_id]);

			 try
			 {
				 if(!object->isSQLDisabled())
					 conn.executeDDLCommand(drop_cmd.arg(object->getSQLName()).arg(object->getName(true)));
			 }
			 catch(Exception &e){}

			 created_objs[types[type_id]]--;
		 }
	 }
 }

 if(use_tmp_names)
 {
  emit s_progressUpdated(100, trUtf8("Restoring orignal names of database, roles and tablespaces."));
  restoreObjectNames();
 }

 if(db_sql_reenabled)
 {
   db_model->setSQLDisabled(true);
   db_sql_reenabled=false;
 }
}

void ModelExportHelper::generateTempObjectNames(DatabaseModel *db_model)
{
  QString tmp_name;
  QTextStream stream(&tmp_name);
  QDateTime dt=QDateTime::currentDateTime();
  QCryptographicHash hash(QCryptographicHash::Md5);
  map<ObjectType, QString> obj_suffixes={ { OBJ_DATABASE, "db_" },
																					{ OBJ_ROLE, "rl_"},
																					{ OBJ_TABLESPACE, "tb_"} };

  orig_obj_names.clear();
  orig_obj_names[db_model]=db_model->getName();

  for(auto role : *db_model->getObjectList(OBJ_ROLE))
  {
    if(!role->isSystemObject())
      orig_obj_names[role]=role->getName();
  }

  for(auto tabspc : *db_model->getObjectList(OBJ_TABLESPACE))
  {
    if(!tabspc->isSystemObject())
      orig_obj_names[tabspc]=tabspc->getName();
  }


  for(auto obj : orig_obj_names)
  {
    stream << reinterpret_cast<unsigned *>(obj.first)  << "_" << dt.toTime_t();

    //Generates an unique name for the permission through md5 hash
    hash.addData(QByteArray(tmp_name.toStdString().c_str()));
    tmp_name=obj_suffixes[obj.first->getObjectType()] + hash.result().toHex();

    obj.first->setName(tmp_name.mid(0,15));
    tmp_name.clear();
  }

	/* Invalidates the codes of all objects on database model in order to generate the SQL referencing the
		 renamed object correctly */
	db_model->setCodesInvalidated();
}

void ModelExportHelper::restoreObjectNames(void)
{
  for(auto obj : orig_obj_names)
    obj.first->setName(obj.second);

	/* Invalidates the codes of all objects on database model in order to generate the SQL referencing the
		 object's with their original names */
	db_model->setCodesInvalidated();
}

void ModelExportHelper::updateProgress(int prog, QString object_id, unsigned obj_type)
{
	int aux_prog=progress + (prog/progress);
	sql_gen_progress=prog;
	if(aux_prog > 100) aux_prog=100;
	emit s_progressUpdated(aux_prog, object_id, static_cast<ObjectType>(obj_type));
}

void ModelExportHelper::setExportToDBMSParams(DatabaseModel *db_model, Connection *conn, const QString &pgsql_ver, bool ignore_dup, bool drop_db, bool simulate, bool use_rand_names)
{
	this->db_model=db_model;
	this->connection=conn;
	this->pgsql_ver=pgsql_ver;
	this->ignore_dup=ignore_dup;
	this->simulate=simulate;
	this->drop_db=drop_db;
  this->use_tmp_names=use_rand_names;
}

void ModelExportHelper::exportToDBMS(void)
{
	if(connection)
    exportToDBMS(db_model, *connection, pgsql_ver, ignore_dup, drop_db, simulate, use_tmp_names);
}

void ModelExportHelper::cancelExport(void)
{
	export_canceled=true;
}
