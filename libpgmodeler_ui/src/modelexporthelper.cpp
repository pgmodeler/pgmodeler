#include "modelexporthelper.h"

ModelExportHelper::ModelExportHelper(QObject *parent) : QObject(parent)
{
  resetExportParams();
}

void ModelExportHelper::resetExportParams(void)
{
  sql_gen_progress=progress=0;
  db_created=ignore_dup=drop_db=drop_objs=export_canceled=false;
  simulate=use_tmp_names=db_sql_reenabled=false;
  created_objs[OBJ_ROLE]=created_objs[OBJ_TABLESPACE]=-1;
  db_model=nullptr;
  connection=nullptr;
  scene=nullptr;
  zoom=100;
  show_grid=show_delim=page_by_page=false;
  viewp=nullptr;
}

void ModelExportHelper::setIgnoredErrors(const QStringList &err_codes)
{
  ignored_errors=err_codes;
  ignored_errors.removeDuplicates();
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
                           trUtf8("Generating SQL code for PostgreSQL `%1'").arg(BaseObject::getPgSQLVersion()),
                           BASE_OBJECT);
		progress=1;
		db_model->saveModel(filename, SchemaParser::SQL_DEFINITION);

    emit s_progressUpdated(100, trUtf8("Output SQL file `%1' successfully written.").arg(filename), BASE_OBJECT);
		emit s_exportFinished();
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	disconnect(db_model, nullptr, this, nullptr);
}

void ModelExportHelper::exportToPNG(ObjectsScene *scene, const QString &filename, double zoom, bool show_grid, bool show_delim, bool page_by_page, QGraphicsView *viewp)
{
  if(!scene)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
  {
		QPixmap pix;
		bool shw_grd, shw_dlm, align_objs;
    QGraphicsView *view=nullptr;
		QRect retv;
		QPolygon pol;
    vector<QRectF> pages;
    unsigned v_cnt=0, h_cnt=0, page_idx=1;
    QString tmpl_filename, file;

    /* If an external view is specified it will be used instead of creating a local one,
       this is a workaround to the error below when running the helper in a separated thread

       QCoreApplication::sendPostedEvents: Cannot send posted events for objects in another thread */
    if(viewp)
      view=viewp;
    else
      view=new QGraphicsView(scene);

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
      tmpl_filename=fi.absolutePath() + GlobalAttributes::DIR_SEPARATOR + fi.baseName() + QString("_p%1.") + fi.completeSuffix();
    }
    else
    {
      QRectF rect=scene->itemsBoundingRect(true);

      //Give some margin to the resulting image
      QSizeF margin=QSizeF(5 * BaseObjectView::HORIZ_SPACING, 5 * BaseObjectView::VERT_SPACING);
      rect.setTopLeft(rect.topLeft() - QPointF(margin.width(), margin.height()));
      rect.setSize(rect.size() + margin);

      pages.push_back(rect);
      file=filename;
    }

		//Updates the scene to apply the change on grid and delimiter
    scene->update();

		//Configures the viewport alignment to top-left coordinates.
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

		//Apply the zoom factor on the viewport
    view->resetTransform();
    view->centerOn(0,0);
    view->scale(zoom, zoom);

    QPainter painter;
    vector<QRectF>::iterator itr=pages.begin(), itr_end=pages.end();

    while(itr!=itr_end && !export_canceled)
    {
      //Convert the objects bounding rect to viewport coordinates to correctly draw them onto pixmap
      pol=view->mapFromScene(*itr);
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

      emit s_progressUpdated((page_idx/static_cast<float>(pages.size())) * 90,
                             trUtf8("Rendering objects to page %1/%2.").arg(page_idx).arg(pages.size()), BASE_OBJECT);

      //Render the entire viewport onto the pixmap
      view->render(&painter, QRectF(QPointF(0,0), pix.size()), retv);
      painter.end();

      if(page_by_page)
        file=tmpl_filename.arg(page_idx++);

      //If the pixmap is not saved raises an error
      if(!pix.save(file))
      {
        //Restoring the scene settings before throw error
        ObjectsScene::setGridOptions(shw_grd, align_objs, shw_dlm);
        scene->update();

        throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_WRITTEN).arg(file),
                        ERR_FILE_DIR_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
      }
    }

    //Restoring the scene settings
    ObjectsScene::setGridOptions(shw_grd, align_objs, shw_dlm);
    scene->update();

    if(!export_canceled)
    {
      emit s_progressUpdated(100, trUtf8("Output image `%1' successfully written.").arg(filename), BASE_OBJECT);
      emit s_exportFinished();
    }
    else
      emit s_exportCanceled();

    if(view!=viewp)
      delete(view);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelExportHelper::exportToDBMS(DatabaseModel *db_model, Connection conn, const QString &pgsql_ver, bool ignore_dup, bool drop_db, bool drop_objs, bool simulate, bool use_tmp_names)
{
	int type_id;
  QString  version, sql_cmd, buf;
	Connection new_db_conn;
	unsigned i, count;
	ObjectType types[]={OBJ_ROLE, OBJ_TABLESPACE};
	BaseObject *object=nullptr;

	try
	{
		if(!db_model)
			throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* If the export is called using ignore duplications or drop database and simulation mode at same time
		an error is raised because the simulate mode (mainly used as SQL validation) cannot
		undo column addition (this can be changed in the future) */
    if(simulate && (ignore_dup || drop_db || drop_objs))
			throw Exception(ERR_MIX_INCOMP_EXPORT_OPTS,__PRETTY_FUNCTION__,__FILE__,__LINE__);
    else if(drop_db && drop_objs)
      throw Exception(ERR_MIX_INCOMP_DROP_OPTS,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)), Qt::DirectConnection);

		export_canceled=false;
		db_created=false;
		progress=sql_gen_progress=0;
		created_objs[OBJ_ROLE]=created_objs[OBJ_TABLESPACE]=-1;
    errors.clear();

		//Retrive the DBMS version in order to generate the correct code
    conn.connect();
    version=conn.getPgSQLVersion(true);

    emit s_progressUpdated(progress, trUtf8("Starting export to DBMS."));

		//Overriding the DBMS version case the version is informed on parameter
		if(!pgsql_ver.isEmpty())
		{
			BaseObject::setPgSQLVersion(pgsql_ver);
      emit s_progressUpdated(progress, trUtf8("PostgreSQL version detection overridden. Using version `%1'.").arg(pgsql_ver));
		}
		else
		{
			BaseObject::setPgSQLVersion(version);
      emit s_progressUpdated(progress, trUtf8("PostgreSQL `%1' server detected.").arg(version));
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
      emit s_progressUpdated(progress, trUtf8("Enabling the SQL code for database `%1' to avoid errors.").arg(db_model->getName()));
    }


		if(ignore_dup)
		{
      emit s_progressUpdated(progress, trUtf8("Ignoring object duplication errors."));

			//Save the current status for ALTER command generation for table columns/constraints
			saveGenAtlerCmdsStatus(db_model);
		}

    if(!ignored_errors.isEmpty())
      emit s_progressUpdated(progress, trUtf8("Ignoring the following error code(s): `%1'.").arg(ignored_errors.join(", ")));

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
                                   trUtf8("Creating object `%1' (%2).")
                                   .arg(object->getName())
                                   .arg(object->getTypeName()),
																	 object->getObjectType());

						sql_cmd=object->getCodeDefinition(SchemaParser::SQL_DEFINITION);
						conn.executeDDLCommand(sql_cmd);
					}
				}
				catch(Exception &e)
				{
          //Ignoring the error if it is in the ignored list
          if(ignored_errors.indexOf(e.getExtraInfo()) >= 0 ||
             (ignore_dup && isDuplicationError(e.getExtraInfo())))
            emit s_errorIgnored(e.getExtraInfo(), e.getErrorMessage(), sql_cmd);
          //Raises an excpetion if the error returned by the database is not listed in the ignored list of errors
          else if(ignored_errors.indexOf(e.getExtraInfo()) < 0)
						throw Exception(e.getErrorMessage(),
														e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, sql_cmd);
					else
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
                               trUtf8("Creating database `%1'.")
                               .arg(db_model->getName()),
															 OBJ_DATABASE);

				sql_cmd=db_model->__getCodeDefinition(SchemaParser::SQL_DEFINITION);
				conn.executeDDLCommand(sql_cmd);
				db_created=true;
			}
		}
		catch(Exception &e)
		{
      //Ignoring the error if it is in the ignored list
      if(ignored_errors.indexOf(e.getExtraInfo()) >= 0 ||
         (ignore_dup && isDuplicationError(e.getExtraInfo())))
        emit s_errorIgnored(e.getExtraInfo(), e.getErrorMessage(), sql_cmd);
      //Raises an excpetion if the error returned by the database is not listed in the ignored list of errors
      else if(ignored_errors.indexOf(e.getExtraInfo()) < 0)
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
                             trUtf8("Connecting to database `%1'.")
                             .arg(db_model->getName()));

			new_db_conn.connect();
			progress=30;

      //Creating the other object types
      emit s_progressUpdated(progress,
                             trUtf8("Generating SQL for `%1' objects...").arg(db_model->getObjectCount()));

      //Exporting the database model definition using the opened connection
      buf=db_model->getCodeDefinition(SchemaParser::SQL_DEFINITION, false);
      progress=40;
      exportBufferToDBMS(buf, new_db_conn, drop_objs);
    }

		disconnect(db_model, nullptr, this, nullptr);

		if(ignore_dup)
			restoreGenAtlerCmdsStatus();

		//Closes the new opened connection
    new_db_conn.close();

		/* If the process was a simulation or even canceled undo the export
		removing the created objects */
		if(simulate || export_canceled)
      undoDBMSExport(db_model, conn, use_tmp_names);

    conn.close();

		if(!export_canceled)
			emit s_exportFinished();
		else
			emit s_exportCanceled();
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);

		if(ignore_dup)
			restoreGenAtlerCmdsStatus();

		try
		{
			//Closes the new opened connection
      new_db_conn.close();

			//Undo the export removing the created objects
      undoDBMSExport(db_model, conn, use_tmp_names);
		}
		catch(Exception &){}

    conn.close();

    /* When running in a separated thread (other than the main application thread)
    redirects the error in form of signal */
    if(this->thread() && this->thread()!=qApp->thread())
    {
      errors.push_back(e);
      emit s_exportAborted(Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, errors));
    }
    else
    {
      //Redirects any error to terrorsr
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
       catch(Exception &){}

       created_objs[types[type_id]]--;
     }
   }

   if(use_tmp_names)
   {
     emit s_progressUpdated(100, trUtf8("Restoring original names of database, roles and tablespaces."));
     restoreObjectNames();
   }
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
  map<ObjectType, QString> obj_suffixes={ { OBJ_DATABASE, QString("db_") },
                                          { OBJ_ROLE, QString("rl_")},
                                          { OBJ_TABLESPACE, QString("tb_")} };

  orig_obj_names.clear();
  orig_obj_names[db_model]=db_model->getName();

  for(auto &role : *db_model->getObjectList(OBJ_ROLE))
  {
    if(!role->isSystemObject())
      orig_obj_names[role]=role->getName();
  }

  for(auto &tabspc : *db_model->getObjectList(OBJ_TABLESPACE))
  {
    if(!tabspc->isSystemObject())
      orig_obj_names[tabspc]=tabspc->getName();
  }


  for(auto &obj : orig_obj_names)
  {
    stream << reinterpret_cast<unsigned *>(obj.first) << QString("_") << dt.toTime_t();

    //Generates an unique name for the object through md5 hash
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
  for(auto &obj : orig_obj_names)
    obj.first->setName(obj.second);

	/* Invalidates the codes of all objects on database model in order to generate the SQL referencing the
		 object's with their original names */
  db_model->setCodesInvalidated();
}

bool ModelExportHelper::isDuplicationError(const QString &error_code)
{
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
  static QStringList err_codes = {QString("42P04"), QString("42723"), QString("42P06"),
                                  QString("42P07"), QString("42710"), QString("42701"),
                                  QString("42P16")};

  return(err_codes.contains(error_code));
}

void ModelExportHelper::exportBufferToDBMS(const QString &buffer, Connection &conn, bool drop_objs)
{
  Connection aux_conn;
  QString sql_buf=buffer, sql_cmd, aux_cmd, lin, msg,
          obj_name, obj_tp_name, tab_name,
          alter_tab=QString("ALTER TABLE");
  //vector<Exception> errors;
  vector<QString> db_sql_cmds;
  QTextStream ts;
  ObjectType obj_type=BASE_OBJECT;
  bool ddl_tk_found=false, is_create=false, is_drop=false;
  unsigned aux_prog=0, curr_size=0, buf_size=sql_buf.size(),
           factor=(db_name.isEmpty() ? 70 : 90);
  int pos=0, pos1=0, comm_cnt=0;

  //Regexp used to extract the object being created
  QRegExp obj_reg(QString("(CREATE|DROP|ALTER)(.)+(\n)")),
          tab_obj_reg(QString("^(%1)(.)+(ADD|DROP)( )(COLUMN|CONSTRAINT)( )*").arg(alter_tab)),
          drop_reg(QString("^((\\-\\-)+( )*)+(DROP)(.)+")),
          drop_tab_obj_reg(QString("^((\\-\\-)+( )*)+(%1)(.)+(DROP)(.)+").arg(alter_tab)),
          reg_aux;

  vector<ObjectType> obj_types={ OBJ_ROLE, OBJ_FUNCTION, OBJ_TRIGGER, OBJ_INDEX,
                                 OBJ_RULE,	OBJ_TABLE, OBJ_VIEW, OBJ_DOMAIN,
                                 OBJ_SCHEMA,	OBJ_AGGREGATE, OBJ_OPFAMILY,
                                 OBJ_OPCLASS, OBJ_OPERATOR,  OBJ_SEQUENCE,
                                 OBJ_CONVERSION, OBJ_CAST,	OBJ_LANGUAGE,
                                 OBJ_COLLATION, OBJ_EXTENSION, OBJ_TYPE,
                                 OBJ_DATABASE };


  /* Extract each SQL command from the buffer and execute them separately. This is done
   to permit the user, in case of error, identify what object is wrongly configured. */
  ts.setString(&sql_buf);

  if(!conn.isStablished())
  {
    if(!db_name.isEmpty())
      conn.setConnectionParam(Connection::PARAM_DB_NAME, db_name);

    conn.connect();
  }

  while(!ts.atEnd() && !export_canceled)
  {
    try
    {
      //Cleanup single line comments
      lin=ts.readLine();
      curr_size+=lin.size();
      aux_prog=progress + ((curr_size/static_cast<float>(buf_size)) * factor);

      /* If the simulation mode is off and the drop objects option is checked,
         check if the current line matches one of the accepted drop commands
         (DROP [OBJECT] or ALTER TABLE...DROP) */
      if(drop_objs && (drop_reg.exactMatch(lin) || drop_tab_obj_reg.exactMatch(lin)))
      {
        comm_cnt=lin.count(QString("--"));
        lin=lin.remove(QString("--")).trimmed();

        /* If the count of comment indicators (--) is 1 indicates that the DDL of the
           object related to the DROP is enabled, so the DROP is executed otherwise ignored */
        if(comm_cnt==1)
        {
         sql_cmd=lin + QString("\n");
         ddl_tk_found=true;
        }
      }
      else
      {
        ddl_tk_found=(lin.indexOf(ParsersAttributes::DDL_END_TOKEN) >= 0);
        lin.remove(QRegExp(QString("^(--)+(.)+$")));

        //If the line isn't empty after cleanup it will be included on sql command
        if(!lin.isEmpty())
          sql_cmd += lin + QString("\n");
      }

      //If the ddl end token is found
      if(ddl_tk_found || (!sql_cmd.isEmpty() && ts.atEnd()))
      {
        //Checking if the command is a column or constraint creation via ALTER TABLE
        aux_cmd=sql_cmd;
        pos=tab_obj_reg.indexIn(aux_cmd);

        if(pos >= 0)
        {        
          aux_cmd.remove('"');
          aux_cmd.remove(QString("IF EXISTS "));
          obj_type=(aux_cmd.contains(QString("COLUMN")) ? OBJ_COLUMN : OBJ_CONSTRAINT);
          reg_aux=QRegExp(QString("(COLUMN|CONSTRAINT)( )+"));

          pos+=tab_obj_reg.matchedLength();
          pos1=aux_cmd.indexOf(' ', pos);

          //Extracting the table name
          pos=aux_cmd.indexOf(alter_tab) + alter_tab.size();
          pos1=aux_cmd.indexOf(QString("ADD"));

          if(pos1 < 0)
          {
            pos1=aux_cmd.indexOf(QString("DROP"));
            is_drop=true;
          }

          tab_name=aux_cmd.mid(pos, pos1 - pos).simplified();

          //Extracting the child object name (column | constraint) the one between
          pos=reg_aux.indexIn(aux_cmd, pos1);
          pos+=reg_aux.matchedLength();

          pos1=aux_cmd.indexOf(" ", pos);
          obj_name=aux_cmd.mid(pos, pos1 - pos).simplified();

          //Creating a fully qualified name for the object (schema.table.name)
          obj_name=tab_name + QString(".") + obj_name;

          if(is_drop)
            msg=trUtf8("Dropping object `%1' (%2).").arg(obj_name).arg(BaseObject::getTypeName(obj_type));
          else
            msg=trUtf8("Creating object `%1' (%2).").arg(obj_name).arg(BaseObject::getTypeName(obj_type));

          emit s_progressUpdated(aux_prog, msg, obj_type, sql_cmd);
          is_drop=false;
        }
        //Check if the regex matches the sql command
        else if(obj_reg.exactMatch(sql_cmd))
        {
          //Get the fisrt line of the sql command, that contains the CREATE/DROP/ALTER ... statement
          lin=sql_cmd.mid(0, sql_cmd.indexOf('\n'));

          for(ObjectType obj_tp : obj_types)
          {
            if(export_canceled) break;

            obj_type=obj_tp;

            //Appeding special tokens when the object is an index or view
            if(lin.startsWith(QString("CREATE")) || lin.startsWith(QString("ALTER")))
            {
              if(obj_tp==OBJ_INDEX)
              {
                lin.remove(QString("UNIQUE"));
                lin.remove(QString("CONCURRENTLY"));
              }
              else if(obj_tp==OBJ_VIEW)
              {
                lin.remove(QString("MATERIALIZED"));
                lin.remove(QString("RECURSIVE"));
              }
            }
            else if(lin.startsWith(QString("DROP")))
            {
              lin.remove(QString("IF EXISTS"));
              lin.remove(QString("MATERIALIZED"));
            }

            lin=lin.simplified();

            //Check if the keyword for the current object exists on string
            reg_aux.setPattern(QString("(CREATE|DROP|ALTER)( )(%1)").arg(BaseObject::getSQLName(obj_tp)));
            pos=reg_aux.indexIn(lin);

            if(pos >= 0)
            {
              is_create=lin.startsWith(QString("CREATE"));
              is_drop=(!is_create && lin.startsWith(QString("DROP")));

              //Extracts from the line the string starting with the object's name
              lin=lin.mid(reg_aux.matchedLength(), sql_cmd.indexOf('\n')).simplified();
              lin.remove('"');

              if(obj_tp!=OBJ_CAST)
              {
                int spc_idx=lin.indexOf(' ');
                obj_name=lin.mid(0, (spc_idx >= 0 ? spc_idx + 1 : lin.size()));

                if(obj_tp!=OBJ_FUNCTION)
                {
                  obj_name=obj_name.remove('(').simplified();
                  obj_name=obj_name.remove(')').simplified();
                }
              }
              else
              {
                obj_name=QString("cast") + lin.replace(QString(" AS "),QString(","));
              }

              //Stores the object type name
              obj_tp_name=BaseObject::getTypeName(obj_tp);
              obj_name.remove(';');

              if(is_create)
                msg=trUtf8("Creating object `%1' (%2)").arg(obj_name).arg(obj_tp_name);
              else if(is_drop)
                msg=trUtf8("Dropping object `%1' (%2)").arg(obj_name).arg(obj_tp_name);
              else
                msg=trUtf8("Changing object `%1' (%2)").arg(obj_name).arg(obj_tp_name);

              break;
            }
          }

          emit s_progressUpdated(aux_prog, msg, obj_type, sql_cmd);
          is_create=is_drop=false;
          msg.clear();
        }
        else if(!sql_cmd.trimmed().isEmpty())
        {
          //General commands like grant, revoke or set aren't explicitly shown
          emit s_progressUpdated(aux_prog, trUtf8("Running auxiliary command."), BASE_OBJECT, sql_cmd);
        }

        //Executes the extracted SQL command
        if(!sql_cmd.isEmpty())
        {
          if(obj_type!=OBJ_DATABASE)
            conn.executeDDLCommand(sql_cmd);
          else
            db_sql_cmds.push_back(sql_cmd);
        }

        sql_cmd.clear();
        ddl_tk_found=false;
      }

      if(ts.atEnd() && !db_sql_cmds.empty())
      {
        conn.close();
        aux_conn=conn;
        aux_conn.connect();
        for(QString cmd : db_sql_cmds)
          aux_conn.executeDDLCommand(cmd);
      }
    }
    catch(Exception &e)
    {
      if(ddl_tk_found) ddl_tk_found=false;

      //Ignoring the error if it is in the ignored list
      if(ignored_errors.indexOf(e.getExtraInfo()) >= 0 ||
         (ignore_dup && isDuplicationError(e.getExtraInfo())))
        emit s_errorIgnored(e.getExtraInfo(), e.getErrorMessage(), sql_cmd);
      //Raises an excpetion if the error returned by the database is not listed in the ignored list of errors
      else if(ignored_errors.indexOf(e.getExtraInfo()) < 0)
        throw Exception(e.getErrorMessage(),
                        e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, sql_cmd);
      else
        errors.push_back(e);

      sql_cmd.clear();
    }
  }

  if(!db_name.isEmpty())
    emit s_exportFinished();
}

void ModelExportHelper::updateProgress(int prog, QString object_id, unsigned obj_type)
{
	int aux_prog=progress + (prog/progress);
	sql_gen_progress=prog;
	if(aux_prog > 100) aux_prog=100;
  emit s_progressUpdated(aux_prog, object_id, static_cast<ObjectType>(obj_type), "", sender()==db_model);
}

void ModelExportHelper::setExportToDBMSParams(DatabaseModel *db_model, Connection *conn, const QString &pgsql_ver, bool ignore_dup, bool drop_db, bool drop_objs, bool simulate, bool use_rand_names)
{
	this->db_model=db_model;
	this->connection=conn;
	this->pgsql_ver=pgsql_ver;
	this->ignore_dup=ignore_dup;
	this->simulate=simulate;
  this->drop_db=drop_db && !drop_objs;
  this->drop_objs=drop_objs && !drop_db;
  this->use_tmp_names=use_rand_names;
  this->sql_buffer.clear();
  this->db_name.clear();
  this->errors.clear();
}

void ModelExportHelper::setExportToDBMSParams(const QString &sql_buffer, Connection *conn, const QString &db_name, bool ignore_dup)
{
  this->sql_buffer=sql_buffer;
  this->connection=conn;
  this->db_name=db_name;
  this->ignore_dup=ignore_dup;
  this->simulate=false;
  this->drop_db=false;
  this->use_tmp_names=false;
  this->errors.clear();
}

void ModelExportHelper::setExportToSQLParams(DatabaseModel *db_model, const QString &filename, const QString &pgsql_ver)
{
  this->db_model=db_model;
  this->filename=filename;
  this->pgsql_ver=pgsql_ver;
}

void ModelExportHelper::setExportToPNGParams(ObjectsScene *scene, QGraphicsView *viewp, const QString &filename, double zoom, bool show_grid, bool show_delim, bool page_by_page)
{
  this->scene=scene;
  this->viewp=viewp;
  this->filename=filename;
  this->zoom=zoom;
  this->show_grid=show_grid;
  this->show_delim=show_delim;
  this->page_by_page=page_by_page;
}

void ModelExportHelper::exportToDBMS(void)
{
	if(connection)
  {
    if(sql_buffer.isEmpty())
     exportToDBMS(db_model, *connection, pgsql_ver, ignore_dup, drop_db, drop_objs, simulate, use_tmp_names);
    else
    {
      try
      {
        exportBufferToDBMS(sql_buffer, *connection);
      }
      catch(Exception &e)
      {
        resetExportParams();
        /* When running in a separated thread (other than the main application thread)
        redirects the error in form of signal */
        if(this->thread() && this->thread()!=qApp->thread())
          emit s_exportAborted(Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e));
        else
          throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
      }
    }
    resetExportParams();
  }
}

void ModelExportHelper::exportToPNG(void)
{
  try
  {
    exportToPNG(scene, filename, zoom, show_grid, show_delim, page_by_page, viewp);
    resetExportParams();
  }
  catch(Exception &e)
  {
    resetExportParams();

    /* When running in a separated thread (other than the main application thread)
    redirects the error in form of signal */
    if(this->thread() && this->thread()!=qApp->thread())
      emit s_exportAborted(Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e));
    else
      throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void ModelExportHelper::exportToSQL(void)
{
  try
  {
    exportToSQL(db_model, filename, pgsql_ver);
    resetExportParams();
  }
  catch(Exception &e)
  {
    resetExportParams();
    /* When running in a separated thread (other than the main application thread)
    redirects the error in form of signal */
    if(this->thread() && this->thread()!=qApp->thread())
      emit s_exportAborted(Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e));
    else
      throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void ModelExportHelper::cancelExport(void)
{
	export_canceled=true;
}
