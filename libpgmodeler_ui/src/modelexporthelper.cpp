#include "modelexporthelper.h"
#include <QSvgGenerator>

ModelExportHelper::ModelExportHelper(QObject *parent) : QObject(parent)
{
	resetExportParams();
}

void ModelExportHelper::resetExportParams()
{
	sql_gen_progress=progress=0;
	db_created=ignore_dup=drop_db=drop_objs=export_canceled=false;
	simulate=use_tmp_names=db_sql_reenabled=false;
	created_objs[ObjectType::Role]=created_objs[ObjectType::Tablespace]=-1;
	db_model=nullptr;
	connection=nullptr;
	scene=nullptr;
	zoom=100;
	show_grid=show_delim=page_by_page=splitted=browsable=false;
	viewp=nullptr;
}

void ModelExportHelper::abortExport(Exception &e)
{
	resetExportParams();

	//When running in a separated thread (other than the main application thread) redirects the error in form of signal
	if(this->thread() && this->thread()!=qApp->thread())
		emit s_exportAborted(Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e));
	else
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void ModelExportHelper::handleSQLError(Exception &e, const QString &sql_cmd, bool ignore_dup)
{
	//Ignoring the error if it is in the ignored list
	if(ignored_errors.indexOf(e.getExtraInfo()) >= 0 ||
			(ignore_dup && isDuplicationError(e.getExtraInfo())))
		emit s_errorIgnored(e.getExtraInfo(), e.getErrorMessage(), sql_cmd);
	//Raises an excpetion if the error returned by the database is not listed in the ignored list of errors
	else if(ignored_errors.indexOf(e.getExtraInfo()) < 0)
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e, sql_cmd);
	else
		errors.push_back(e);
}

void ModelExportHelper::setIgnoredErrors(const QStringList &err_codes)
{
	QRegExp valid_code = QRegExp("([a-z]|[A-Z]|[0-9])+");
	QStringList error_codes=err_codes;

	ignored_errors.clear();
	error_codes.removeDuplicates();

	for(QString code : error_codes)
	{
		if(valid_code.exactMatch(code))
			ignored_errors.push_back(code);
	}
}

void ModelExportHelper::exportToSQL(DatabaseModel *db_model, const QString &filename, const QString &pgsql_ver)
{
	if(!db_model)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)));

	try
	{
		progress=sql_gen_progress=0;
		BaseObject::setPgSQLVersion(pgsql_ver);
		emit s_progressUpdated(progress,
							   tr("Generating SQL code for PostgreSQL `%1'").arg(BaseObject::getPgSQLVersion()),
							   ObjectType::BaseObject);
		progress=1;
		db_model->saveModel(filename, SchemaParser::SqlDefinition);

		emit s_progressUpdated(100, tr("Output SQL file `%1' successfully written.").arg(filename), ObjectType::BaseObject);
		emit s_exportFinished();
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	disconnect(db_model, nullptr, this, nullptr);
}

void ModelExportHelper::exportToPNG(ObjectsScene *scene, const QString &filename, double zoom, bool show_grid, bool show_delim, bool page_by_page, QGraphicsView *viewp)
{
	if(!scene)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

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
		shw_grd = ObjectsScene::isShowGrid();
		align_objs = ObjectsScene::isAlignObjectsToGrid();
		shw_dlm = ObjectsScene::isShowPageDelimiters();

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
			tmpl_filename=fi.absolutePath() + GlobalAttributes::DirSeparator + fi.baseName() + QString("_p%1.") + fi.completeSuffix();
		}
		else
		{
			QRectF rect=scene->itemsBoundingRect(true);

			//Give some margin to the resulting image
			QSizeF margin=QSizeF(5 * BaseObjectView::HorizSpacing, 5 * BaseObjectView::VertSpacing);
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

			emit s_progressUpdated((page_idx/static_cast<double>(pages.size())) * 90,
														 tr("Rendering objects to page %1/%2.").arg(page_idx).arg(pages.size()), ObjectType::BaseObject);

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

				throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(file),
												ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}
		}

		//Restoring the scene settings
		ObjectsScene::setGridOptions(shw_grd, align_objs, shw_dlm);
		scene->update();

		if(!export_canceled)
		{
			emit s_progressUpdated(100, tr("Output image `%1' successfully written.").arg(filename), ObjectType::BaseObject);
			emit s_exportFinished();
		}
		else
			emit s_exportCanceled();

		if(view!=viewp)
			delete view;
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelExportHelper::exportToSVG(ObjectsScene *scene, const QString &filename, bool show_grid, bool show_delim)
{
	if(!scene)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	bool shw_dlm=false, shw_grd=false, align_objs=false;
	QSvgGenerator svg_gen;
	QRectF scene_rect=scene->itemsBoundingRect(true);
	QFileInfo fi(filename);

	//Making a backup of the current scene options
	shw_grd = ObjectsScene::isShowGrid();
	shw_dlm = ObjectsScene::isShowPageDelimiters();
	align_objs = ObjectsScene::isAlignObjectsToGrid();
	scene->setBackgroundBrush(Qt::NoBrush);

	//Disabling grid and delimiters
	ObjectsScene::setGridOptions(show_grid, false, show_delim);
	scene->update();

	emit s_progressUpdated(0, tr("Exporting model to SVG file."));

	svg_gen.setFileName(filename);
	svg_gen.setTitle(tr("SVG representation of database model"));
	svg_gen.setDescription(tr("SVG file generated by pgModeler"));

	QPainter *painter=new QPainter(&svg_gen);
	scene->render(painter, QRectF(0, 0, scene_rect.size().width(), scene_rect.size().height()), scene_rect);
	delete painter;

	//Restoring the scene settings
	ObjectsScene::setGridOptions(shw_grd, align_objs, shw_dlm);
	scene->update();

	if(!fi.exists() || !fi.isWritable() || !fi.isReadable())
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotWritten).arg(filename),
											ErrorCode::FileDirectoryNotWritten,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QFile svg_file;
	svg_file.setFileName(filename);
	svg_file.open(QFile::ReadOnly);

	if(svg_file.isOpen())
	{
		QByteArray buf;
		QString svg_def, font_attr=QString("font-family=\"%1\"");

		svg_def.append(svg_file.readAll());
		svg_file.close();

		//Forcing the usage of the font settings defined for BaseObjectView and its subclasses
		svg_def.replace(font_attr.arg(scene->font().family()),
										font_attr.arg(BaseObjectView::getFontStyle(Attributes::Global).font().family()));

		/* Removing the empty (transparent) backgound object in order to save some space in the file if
		the grid or delimiter is displayed */
		if(!show_delim && !show_grid)
			svg_def.replace(QRegExp("(<image)(.)*(xlink:href)(=)(\")(\\w|=|/|\\+|:|;|,|\n)+(\")( )+(/>)"), QString());

		buf.append(svg_def);

		svg_file.open(QFile::WriteOnly | QFile::Truncate);
		svg_file.write(buf);
		svg_file.close();
	}

	emit s_progressUpdated(100, tr("Output file `%1' successfully written.").arg(filename), ObjectType::BaseObject);
	emit s_exportFinished();
}

void ModelExportHelper::exportToDBMS(DatabaseModel *db_model, Connection conn, const QString &pgsql_ver, bool ignore_dup, bool drop_db, bool drop_objs, bool simulate, bool use_tmp_names)
{
	int type_id = 0, pos = -1;
	QString  version, sql_cmd, buf, sql_cmd_comment;
	Connection new_db_conn;
	unsigned i, count;
	ObjectType types[]={ObjectType::Role, ObjectType::Tablespace};
	BaseObject *object=nullptr;
	QString tmpl_comm_regexp = QString("(COMMENT)( )+(ON)( )+(%1)(.)+(\n)(") + Attributes::DdlEndToken + QString(")");
	QRegExp comm_regexp;

	try
	{
		if(!db_model)
			throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		/* If the export is called using ignore duplications or drop database and simulation mode at same time
		an error is raised because the simulate mode (mainly used as SQL validation) cannot
		undo column addition (this can be changed in the future) */
		if(simulate && (ignore_dup || drop_db || drop_objs))
			throw Exception(ErrorCode::MixingIncompExportOptions,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(drop_db && drop_objs)
			throw Exception(ErrorCode::MixingIncompDropOptions,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)), Qt::DirectConnection);

		export_canceled=false;
		db_created=false;
		progress=sql_gen_progress=0;
		created_objs[ObjectType::Role]=created_objs[ObjectType::Tablespace]=-1;
		errors.clear();

		//Retrive the DBMS version in order to generate the correct code
		conn.connect();
		version=conn.getPgSQLVersion(true);

		emit s_progressUpdated(progress, tr("Starting export to DBMS."));

		//Overriding the DBMS version case the version is informed on parameter
		if(!pgsql_ver.isEmpty())
		{
			BaseObject::setPgSQLVersion(pgsql_ver);
			emit s_progressUpdated(progress, tr("PostgreSQL version detection overridden. Using version `%1'.").arg(pgsql_ver));
		}
		else
		{
			BaseObject::setPgSQLVersion(version);
			emit s_progressUpdated(progress, tr("PostgreSQL `%1' server detected.").arg(version));
		}

		if(simulate && use_tmp_names)
		{
			emit s_progressUpdated(progress, tr("Generating temporary names for database, roles and tablespaces."));
			generateTempObjectNames(db_model);
		}
		else if(use_tmp_names)
			throw Exception(ErrorCode::InvUsageTempNamesExportOption,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		if(simulate && db_model->isSQLDisabled())
		{
			db_model->setSQLDisabled(false);
			db_sql_reenabled=true;
			emit s_progressUpdated(progress, tr("Enabling the SQL code for database `%1' to avoid errors.").arg(db_model->getName()));
		}


		if(ignore_dup)
		{
			emit s_progressUpdated(progress, tr("Ignoring object duplication errors."));

			//Save the current status for ALTER command generation for table columns/constraints
			saveGenAtlerCmdsStatus(db_model);
		}

		if(!ignored_errors.isEmpty())
			emit s_progressUpdated(progress, tr("Ignoring the following error code(s): `%1'.").arg(ignored_errors.join(", ")));

		if(drop_db)
		{
			emit s_progressUpdated(progress, tr("Trying to drop database `%1'.").arg(db_model->getName()));

			try
			{
				sql_cmd = QString("DROP DATABASE IF EXISTS %1;").arg(db_model->getName(true));
				conn.executeDDLCommand(sql_cmd);
			}
			catch(Exception &e)
			{
				handleSQLError(e, sql_cmd, ignore_dup);
			}

			sql_cmd.clear();
		}

		if(simulate)
			emit s_progressUpdated(progress, tr("Simulation mode activated."));

		//Creates the roles and tablespaces separately from the other objects
		for(type_id=0; type_id < 2 && !export_canceled; type_id++)
		{
			count=db_model->getObjectCount(types[type_id]);

			for(i=0; i < count && !export_canceled; i++)
			{
				object=db_model->getObject(i, types[type_id]);
				progress=((10 * (type_id+1)) + ((i/static_cast<double>(count)) * 10));

				try
				{
					if(!object->isSQLDisabled())
					{
						//Emits a signal indicating that the object is being exported
						emit s_progressUpdated(progress,
											   tr("Creating object `%1' (%2)")
											   .arg(object->getName())
											   .arg(object->getTypeName()),
											   object->getObjectType());

						sql_cmd=object->getCodeDefinition(SchemaParser::SqlDefinition);

						if(types[type_id] == ObjectType::Tablespace)
						{
							comm_regexp = QRegExp(tmpl_comm_regexp.arg(object->getSQLName()));
							pos = comm_regexp.indexIn(sql_cmd);

							/* If we find a comment on statement we should strip it from the tablespace definition in
							 * order to execute it after creating the db */
							if(pos >= 0)
							{
								sql_cmd_comment = sql_cmd.mid(pos, comm_regexp.matchedLength());
								sql_cmd.remove(pos, comm_regexp.matchedLength());
								pos = -1;
							}
						}

						conn.executeDDLCommand(sql_cmd);

						if(!sql_cmd_comment.isEmpty())
							conn.executeDDLCommand(sql_cmd_comment);
					}
				}
				catch(Exception &e)
				{
					handleSQLError(e, sql_cmd, ignore_dup);
				}

				created_objs[types[type_id]]++;
			}
		}

		try
		{
			if(!db_model->isSQLDisabled() && !export_canceled)
			{
				comm_regexp = QRegExp(tmpl_comm_regexp.arg(db_model->getSQLName()));

				//Creating the database on the DBMS
				emit s_progressUpdated(progress,
									   tr("Creating database `%1'")
									   .arg(db_model->getName()),
									   ObjectType::Database);

				sql_cmd=db_model->__getCodeDefinition(SchemaParser::SqlDefinition);
				pos = comm_regexp.indexIn(sql_cmd);

				/* If we find a comment on statment we should strip it from the DB definition in
				 * order to execute it after creating the db */
				if(pos >= 0)
				{
					sql_cmd_comment = sql_cmd.mid(pos, comm_regexp.matchedLength());
					sql_cmd.remove(pos, comm_regexp.matchedLength());
				}

				conn.executeDDLCommand(sql_cmd);
				db_created=true;

				if(!sql_cmd_comment.isEmpty())
					conn.executeDDLCommand(sql_cmd_comment);
			}
		}
		catch(Exception &e)
		{
			handleSQLError(e, sql_cmd, ignore_dup);
		}

		if(!export_canceled)
		{
			//Connects to the new created database to create the other objects
			progress=20;
			new_db_conn=conn;
			new_db_conn.setConnectionParam(Connection::ParamDbName, db_model->getName());
			emit s_progressUpdated(progress,
								   tr("Connecting to database `%1'")
								   .arg(db_model->getName()));

			new_db_conn.connect();
			progress=30;

			//Creating the other object types
			emit s_progressUpdated(progress, tr("Generating SQL for `%1' objects...").arg(db_model->getObjectCount()));

			//Exporting the database model definition using the opened connection
			buf=db_model->getCodeDefinition(SchemaParser::SqlDefinition, false);
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
			emit s_exportAborted(Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, errors));
		}
		else
		{
			//Redirects any error to terrorsr
			if(errors.empty())
				throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			else
			{
				errors.push_back(e);
				throw Exception(e.getErrorMessage(),__PRETTY_FUNCTION__,__FILE__,__LINE__, errors);
			}
		}
	}
}

void ModelExportHelper::exportToDataDict(DatabaseModel *db_model, const QString &path, bool browsable, bool splitted)
{
	if(!db_model)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	connect(db_model, SIGNAL(s_objectLoaded(int,QString,uint)), this, SLOT(updateProgress(int,QString,uint)));

	try
	{
		progress=0;
		emit s_progressUpdated(progress,
													 tr("Starting data dictionary generation..."),
													 ObjectType::BaseObject);
		progress=1;
		db_model->saveDataDictionary(path, browsable, splitted);

		emit s_progressUpdated(100, tr("Data dictionary successfully saved into `%1'.").arg(path), ObjectType::BaseObject);
		emit s_exportFinished();
	}
	catch(Exception &e)
	{
		disconnect(db_model, nullptr, this, nullptr);
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

	disconnect(db_model, nullptr, this, nullptr);
}

void ModelExportHelper::saveGenAtlerCmdsStatus(DatabaseModel *db_model)
{
	vector<BaseObject *> objects;
	PhysicalTable *tab=nullptr;
	Relationship *rel=nullptr;

	objects.insert(objects.end(), db_model->getObjectList(ObjectType::Table)->begin(),
								 db_model->getObjectList(ObjectType::Table)->end());

	objects.insert(objects.end(), db_model->getObjectList(ObjectType::ForeignTable)->begin(),
								 db_model->getObjectList(ObjectType::ForeignTable)->end());

	//Store the relationship on the auxiliary vector but only many-to-many are considered
	objects.insert(objects.end(), db_model->getObjectList(ObjectType::Relationship)->begin(),
				   db_model->getObjectList(ObjectType::Relationship)->end());

	alter_cmds_status.clear();

	while(!objects.empty())
	{
		rel=dynamic_cast<Relationship *>(objects.back());

		/* If the current object is a relationship try to get the
		generated table (if the relationship is many-to-many only) */
		if(rel)
			tab=rel->getGeneratedTable();
		else
			tab=dynamic_cast<PhysicalTable *>(objects.back());

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

void ModelExportHelper::restoreGenAtlerCmdsStatus()
{
	for(auto &itr : alter_cmds_status)
		itr.first->setGenerateAlterCmds(itr.second);

	alter_cmds_status.clear();
}

void ModelExportHelper::undoDBMSExport(DatabaseModel *db_model, Connection &conn, bool use_tmp_names)
{
	QString drop_cmd=QString("DROP %1 %2;");
	ObjectType types[]={ObjectType::Role, ObjectType::Tablespace};
	int type_id;
	BaseObject *object=nullptr;

	//In case of error during the export all created object are removed
	if(db_created || created_objs[ObjectType::Role] >= 0 || created_objs[ObjectType::Tablespace] >= 0)
	{
		emit s_progressUpdated(99, tr("Destroying objects created on the server."));

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
			emit s_progressUpdated(100, tr("Restoring original names of database, roles and tablespaces."));
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
	QString tmp_name, old_name;
	QTextStream stream(&tmp_name);
	QDateTime dt=QDateTime::currentDateTime();
	QCryptographicHash hash(QCryptographicHash::Md5);
	map<ObjectType, QString> obj_suffixes={ { ObjectType::Database, QString("db_") },
											{ ObjectType::Role, QString("rl_")},
											{ ObjectType::Tablespace, QString("tb_")} };

	orig_obj_names.clear();
	orig_obj_names[db_model]=db_model->getName();

	for(auto &role : *db_model->getObjectList(ObjectType::Role))
	{
		if(!role->isSystemObject())
			orig_obj_names[role]=role->getName();
	}

	for(auto &tabspc : *db_model->getObjectList(ObjectType::Tablespace))
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

		old_name=obj.first->getName();
		obj.first->setName(tmp_name.mid(0,15));
		tmp_name.clear();

		emit s_progressUpdated(progress, tr("Renaming `%1' (%2) to `%3'")
							   .arg(old_name)
							   .arg(obj.first->getTypeName())
							   .arg(obj.first->getName()));
	}

	/* Invalidates the codes of all objects on database model in order to generate the SQL referencing the
		 renamed object correctly */
	db_model->setCodesInvalidated();
}

void ModelExportHelper::restoreObjectNames()
{
	for(auto &obj : orig_obj_names)
		obj.first->setName(obj.second);

	/* Invalidates the codes of all objects on database model in order to generate the SQL referencing the
		 object's with their original names */
	if(db_model)
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

	return err_codes.contains(error_code);
}

void ModelExportHelper::exportBufferToDBMS(const QString &buffer, Connection &conn, bool drop_objs)
{
	Connection aux_conn;
	QString sql_buf=buffer, sql_cmd, aux_cmd, lin, msg,
			obj_name, obj_tp_name, tab_name, orig_conn_db_name,
			alter_tab=QString("ALTER TABLE");
	vector<QString> db_sql_cmds;
	QTextStream ts;
	ObjectType obj_type=ObjectType::BaseObject;
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

	vector<ObjectType> obj_types={ ObjectType::Role, ObjectType::Function, ObjectType::Trigger, ObjectType::Index,
																 ObjectType::Policy, ObjectType::Rule,	ObjectType::Table, ObjectType::View, ObjectType::Domain,
																 ObjectType::Schema,	ObjectType::Aggregate, ObjectType::OpFamily,
																 ObjectType::OpClass, ObjectType::Operator,  ObjectType::Sequence,
																 ObjectType::Conversion, ObjectType::Cast,	ObjectType::Language,
																 ObjectType::Collation, ObjectType::Extension, ObjectType::Type,
																 ObjectType::EventTrigger, ObjectType::ForeignDataWrapper, ObjectType::ForeignServer,
																 ObjectType::UserMapping, ObjectType::ForeignTable, ObjectType::Database, ObjectType::BaseObject };

	/* Extract each SQL command from the buffer and execute them separately. This is done
   to permit the user, in case of error, identify what object is wrongly configured. */
	ts.setString(&sql_buf);

	if(!conn.isStablished())
	{
		orig_conn_db_name = conn.getConnectionParam(Connection::ParamDbName);

		if(!db_name.isEmpty())
			conn.setConnectionParam(Connection::ParamDbName, db_name);

		conn.connect();
	}

	while(!ts.atEnd() && !export_canceled)
	{
		try
		{
			//Cleanup single line comments
			lin=ts.readLine();
			curr_size+=lin.size();
			aux_prog=progress + ((curr_size/static_cast<double>(buf_size)) * factor);

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
				ddl_tk_found=(lin.indexOf(Attributes::DdlEndToken) >= 0);
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
					obj_type=(aux_cmd.contains(QString("COLUMN")) ? ObjectType::Column : ObjectType::Constraint);
					reg_aux=QRegExp(QString("(COLUMN|CONSTRAINT)( )+"));

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
						msg=tr("Dropping object `%1' (%2)").arg(obj_name).arg(BaseObject::getTypeName(obj_type));
					else
						msg=tr("Creating object `%1' (%2)").arg(obj_name).arg(BaseObject::getTypeName(obj_type));

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

						if(lin.startsWith(QString("CREATE")) || lin.startsWith(QString("ALTER")))
						{
							if(obj_tp==ObjectType::Index)
							{
								lin.remove(QString("UNIQUE"));
								lin.remove(QString("CONCURRENTLY"));
							}
							else if(obj_tp==ObjectType::View)
							{
								lin.remove(QString("MATERIALIZED"));
								lin.remove(QString("RECURSIVE"));
							}
							else if(obj_tp==ObjectType::Table)
							{
								lin.remove(QString("UNLOGGED"));
							}
							else if(obj_tp==ObjectType::Function)
							{
								lin.remove(QString("OR REPLACE"));
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

							if(obj_tp != ObjectType::BaseObject)
							{
								if(obj_tp!=ObjectType::Cast && obj_tp != ObjectType::UserMapping)
								{
									int spc_idx=lin.indexOf(' ');
									obj_name=lin.mid(0, (spc_idx >= 0 ? spc_idx + 1 : lin.size()));

									if(obj_tp!=ObjectType::Function)
									{
										obj_name=obj_name.remove('(').simplified();
										obj_name=obj_name.remove(')').simplified();
									}
								}
								else if(obj_tp == ObjectType::UserMapping)
								{
									obj_name.prepend(lin.remove(QString("FOR")).trimmed() + QChar('@'));
								}
								else
								{
									obj_name=QString("cast") + lin.replace(QString(" AS "),QString(","));
								}

								//Stores the object type name
								obj_tp_name=BaseObject::getTypeName(obj_tp);
								obj_name.remove(';');

								if(is_create)
									msg=tr("Creating object `%1' (%2)").arg(obj_name).arg(obj_tp_name);
								else if(is_drop)
									msg=tr("Dropping object `%1' (%2)").arg(obj_name).arg(obj_tp_name);
								else
									msg=tr("Changing object `%1' (%2)").arg(obj_name).arg(obj_tp_name);
							}
							// If the type of the object being create can't be identified
							else
							{
								QString aux_cmd_type;

								if(is_create)
									aux_cmd_type = QString("CREATE");
								else if(is_drop)
									aux_cmd_type = QString("DROP");
								else
									aux_cmd_type = QString("ALTER");

								msg=tr("Running auxiliary `%1' command...").arg(aux_cmd_type);
							}

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
					emit s_progressUpdated(aux_prog, tr("Running auxiliary command."), ObjectType::BaseObject, sql_cmd);
				}

				//Executes the extracted SQL command
				if(!sql_cmd.isEmpty())
				{
					if(obj_type != ObjectType::Database)
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

				if(!orig_conn_db_name.isEmpty())
					aux_conn.setConnectionParam(Connection::ParamDbName, orig_conn_db_name);

				aux_conn.connect();
				for(QString cmd : db_sql_cmds)
					aux_conn.executeDDLCommand(cmd);
			}
		}
		catch(Exception &e)
		{
			if(ddl_tk_found) ddl_tk_found=false;
			handleSQLError(e, sql_cmd, ignore_dup);
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

void ModelExportHelper::setExportToSVGParams(ObjectsScene *scene, const QString &filename, bool show_grid, bool show_delim)
{
	this->scene=scene;
	this->filename=filename;
	this->show_grid=show_grid;
	this->show_delim=show_delim;
}

void ModelExportHelper::setExportToDataDictParams(DatabaseModel *db_model, const QString &path, bool browsable, bool splitted)
{
	this->db_model=db_model;
	this->filename=path;
	this->browsable=browsable;
	this->splitted=splitted;
}

void ModelExportHelper::exportToDBMS()
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
				abortExport(e);
			}
		}
		resetExportParams();
	}
}

void ModelExportHelper::exportToPNG()
{
	try
	{
		exportToPNG(scene, filename, zoom, show_grid, show_delim, page_by_page, viewp);
		resetExportParams();
	}
	catch(Exception &e)
	{
		abortExport(e);
	}
}

void ModelExportHelper::exportToSVG()
{
	try
	{
		exportToSVG(scene, filename, show_grid, show_delim);
		resetExportParams();
	}
	catch(Exception &e)
	{
		abortExport(e);
	}
}

void ModelExportHelper::exportToSQL()
{
	try
	{
		exportToSQL(db_model, filename, pgsql_ver);
		resetExportParams();
	}
	catch(Exception &e)
	{
		abortExport(e);
	}
}

void ModelExportHelper::exportToDataDict()
{
	try
	{
		exportToDataDict(db_model, filename, browsable, splitted);
		resetExportParams();
	}
	catch(Exception &e)
	{
		abortExport(e);
	}
}

void ModelExportHelper::cancelExport()
{
	export_canceled=true;
}
