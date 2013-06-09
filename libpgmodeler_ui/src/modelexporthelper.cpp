#include "modelexporthelper.h"

ModelExportHelper::ModelExportHelper(QObject *parent) : QObject(parent)
{
	sql_gen_progress=progress=0;
	db_created=false;
	created_objs[OBJ_ROLE]=created_objs[OBJ_TABLESPACE]=-1;
}

void ModelExportHelper::exportToSQL(DatabaseModel *db_model, const QString &filename, const QString &pgsql_ver)
{
	if(!db_model)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)));

	try
	{
		progress=sql_gen_progress=0;
		SchemaParser::setPgSQLVersion(pgsql_ver);
		emit s_progressUpdated(progress, trUtf8("PostgreSQL %1 version code generation...").arg(SchemaParser::getPgSQLVersion()));
		progress=1;
		db_model->saveModel(filename, SchemaParser::SQL_DEFINITION);
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	disconnect(db_model, nullptr, this, nullptr);
}

void ModelExportHelper::exportToPNG(ObjectsScene *scene, const QString &filename, bool show_grid, bool show_delim)
{
	if(!scene)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		QPixmap pix;
		QRectF ret=scene->itemsBoundingRect();
		bool shw_grd, shw_dlm, align_objs;

		//Make a backup of the current scene options
		ObjectsScene::getGridOptions(shw_grd, align_objs, shw_dlm);

		//Sets the options passed by the user
		ObjectsScene::setGridOptions(show_grid, false, show_delim);

		//Ceils the width and height of scene rectangle in order to not draw dirty areas on pixmap
		ret.setWidth(ceilf(ret.width()));
		ret.setHeight(ceilf(ret.height()));

		//Creates the output pixmap
		pix=QPixmap(ret.size().toSize());
		QPainter p(&pix);

		//Setting optimizations on the painter
		p.setRenderHint(QPainter::Antialiasing, true);
		p.setRenderHint(QPainter::TextAntialiasing, true);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		//Render the entire scene onto the pixmap
		scene->clearSelection();
		scene->update();
		scene->render(&p, QRectF(QPointF(0,0), pix.size()), ret);

		//Restore the scene options
		ObjectsScene::setGridOptions(shw_grd, align_objs, shw_dlm);
		scene->update();

		//If the pixmap is not saved raises an error
		if(!pix.save(filename))
			throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(Utf8String::create(filename)),
											ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelExportHelper::exportToDBMS(DatabaseModel *db_model, DBConnection &conn, const QString &pgsql_ver, bool ignore_dup, bool simulate)
{
	int type_id;
	QString  version, sql_buf, sql_cmd, lin;
	DBConnection new_db_conn;
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

		 Reference:
			http://www.postgresql.org/docs/current/static/errcodes-appendix.html*/
	QString error_codes[]={"42P04", "42723", "42P06", "42P07", "42710"};
	vector<QString> err_codes_vect(error_codes, error_codes + sizeof(error_codes) / sizeof(QString));

	if(!db_model)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)));

	try
	{
		db_created=false;
		progress=sql_gen_progress=0;
		created_objs[OBJ_ROLE]=created_objs[OBJ_TABLESPACE]=-1;
		conn.connect();

		//Retrive the DBMS version in order to generate the correct code
		version=(conn.getDBMSVersion()).mid(0,3);

		//Overriding the DBMS version case the version is informed on parameter
		if(!pgsql_ver.isEmpty())
		{
			SchemaParser::setPgSQLVersion(pgsql_ver);
			emit s_progressUpdated(progress, trUtf8("PostgreSQL version detection overrided. Using version %1...").arg(pgsql_ver));
		}
		else
		{
			SchemaParser::setPgSQLVersion(version);
			emit s_progressUpdated(progress, trUtf8("PostgreSQL %1 server detected...").arg(version));
		}

		if(ignore_dup)
			emit s_progressUpdated(progress, trUtf8("Ignoring object duplication error..."));

		//Creates the roles and tablespaces separately from the other objects
		for(type_id=0; type_id < 2; type_id++)
		{
			count=db_model->getObjectCount(types[type_id]);

			for(i=0; i < count; i++)
			{
				object=db_model->getObject(i, types[type_id]);
				progress=((10 * (type_id+1)) + ((i/static_cast<float>(count)) * 10));

				//Emits a signal indicating that the object is being exported
				emit s_progressUpdated(progress,
															 trUtf8("Creating object `%1' (%2)...").arg(Utf8String::create(object->getName())).arg(object->getTypeName()));

				try
				{
					if(!object->isSQLDisabled())
					{
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
			if(!db_model->isSQLDisabled())
			{
				//Creating the database on the DBMS
				emit s_progressUpdated(progress, trUtf8("Creating database `%1'...").arg(Utf8String::create(db_model->getName())));
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

		//Connects to the new created database to create the other objects
		progress=30;
		new_db_conn=conn;
		new_db_conn.setConnectionParam(DBConnection::PARAM_DB_NAME, db_model->getName());
		emit s_progressUpdated(progress, trUtf8("Connecting to database `%1'...").arg(Utf8String::create(db_model->getName())));

		new_db_conn.connect();
		progress=50;
		//Creating the other object types
		emit s_progressUpdated(progress, trUtf8("Creating objects on database `%1'...").arg(Utf8String::create(db_model->getName())));

		//Generates the sql from entire model
		sql_buf=db_model->getCodeDefinition(SchemaParser::SQL_DEFINITION, false);

		/* Extract each SQL command from the buffer and execute them separately. This is done
			 to permit the user, in case of error, identify what object is wrongly configured. */
		ts.setString(&sql_buf);
		i=1;
		progress+=(sql_gen_progress/progress);
		sql_cmd.clear();

		while(!ts.atEnd())
		{
			try
			{
				//Cleanup single line comments
				lin=ts.readLine();
				ddl_tk_found=(lin.indexOf(ParsersAttributes::DDL_END_TOKEN) >= 0);
				lin.remove(QRegExp("^(--)+(.)+$"));

				//If the line isn't empty after cleanup it will be included on sql command
				if(!lin.isEmpty())
					sql_cmd += lin + "\n";

				//If the ddl end token is found
				if(ddl_tk_found)
				{
					//Regexp used to extract the object being created
					QRegExp reg=QRegExp("(CREATE)(.)+(\n)", Qt::CaseSensitive);

					sql_cmd.simplified();

					//Check if the regex matches the sql command
					if(reg.exactMatch(sql_cmd))
					{
						QString obj_type, obj_name;
						ObjectType obj_types[]={ OBJ_FUNCTION, OBJ_TRIGGER, OBJ_INDEX,
																		 OBJ_RULE,	OBJ_TABLE, OBJ_VIEW, OBJ_DOMAIN,
																		 OBJ_SCHEMA,	OBJ_AGGREGATE, OBJ_OPFAMILY,
																		 OBJ_OPCLASS, OBJ_OPERATOR,  OBJ_SEQUENCE,	OBJ_ROLE,
																		 OBJ_CONVERSION, OBJ_CAST,	OBJ_LANGUAGE,	OBJ_TYPE,
																		 OBJ_TABLESPACE, OBJ_DATABASE, OBJ_COLLATION, OBJ_EXTENSION };
						unsigned count=sizeof(obj_types)/sizeof(ObjectType);
						int pos=0;

						for(unsigned i=0; i < count; i++)
						{
							//Check if the keyword for the current object exists on string
							pos=sql_cmd.indexOf(BaseObject::getSQLName(obj_types[i]));

							if(pos >= 0)
							{
								//Extracts from the line the string starting with the object's name
								lin=sql_cmd.mid(pos + BaseObject::getSQLName(obj_types[i]).size(),
																sql_cmd.indexOf('\n')).simplified();

								//Stores the object type name
								obj_type=BaseObject::getTypeName(obj_types[i]);

								//Special case of indexes, removes the "concurrently" keyword that cames after INDEX keyword
								if(obj_types[i]==OBJ_INDEX)
									lin.replace("CONCURRENTLY","");

								//The object name is the first element when splitting the string with space separator
								obj_name=lin.split(' ').at(0);
								obj_name.remove('(');
								obj_name.remove(';');
								break;
							}
						}

						emit s_progressUpdated(progress + (i/progress), trUtf8("Creating object `%1' (%2)...")
																	 .arg(obj_name)
																	 .arg(obj_type));
					}
					else
						//General commands like alter / set aren't explicitly shown
						emit s_progressUpdated(progress + (i/progress), trUtf8("Executing auxiliary command..."));

					//Executes the extracted SQL command
					if(!sql_cmd.isEmpty())
						new_db_conn.executeDDLCommand(sql_cmd);

					ddl_tk_found=false;
					sql_cmd.clear();
					i++;
				}
			}
			catch(Exception &e)
			{
				if(!ignore_dup ||
					 (ignore_dup &&
						std::find(err_codes_vect.begin(), err_codes_vect.end(), e.getExtraInfo())==err_codes_vect.end()))
					throw Exception(Exception::getErrorMessage(ERR_EXPORT_FAILURE).arg(Utf8String::create(sql_cmd)),
													ERR_EXPORT_FAILURE,__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, sql_cmd);
				else
					errors.push_back(e);
			}
		}

		disconnect(db_model, nullptr, this, nullptr);

		//Closes the new opened connection
		if(new_db_conn.isStablished()) new_db_conn.close();

		/* If the process was a simulation undo the export
		removing the created objects */
		if(simulate)
			undoDBMSExport(db_model, conn);
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);

		//Closes the new opened connection
		if(new_db_conn.isStablished()) new_db_conn.close();

		//Undo the export removing the created objects
		undoDBMSExport(db_model, conn);

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

void ModelExportHelper::undoDBMSExport(DatabaseModel *db_model, DBConnection &conn)
{
 QString drop_cmd=QString("DROP %1 %2;");
 ObjectType types[]={OBJ_ROLE, OBJ_TABLESPACE};
 int type_id;
 BaseObject *object=nullptr;

 //In case of error during the export all created object are removed
 if(db_created || created_objs[OBJ_ROLE] >= 0 || created_objs[OBJ_TABLESPACE] >= 0)
 {
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
				 conn.executeDDLCommand(drop_cmd.arg(object->getSQLName()).arg(object->getName(true)));
			 }
			 catch(Exception &e){}

			 created_objs[types[type_id]]--;
		 }
	 }
 }
}

void ModelExportHelper::updateProgress(int prog, QString object_id, unsigned)
{
	int aux_prog=progress + (prog/progress);
	sql_gen_progress=prog;
	if(aux_prog > 100) aux_prog=100;
	emit s_progressUpdated(aux_prog, object_id);
}
