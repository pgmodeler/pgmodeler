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

#include "databaseimportform.h"
#include "taskprogresswidget.h"
#include "configurationform.h"
#include "taskprogresswidget.h"

extern ConfigurationForm *configuration_form;
extern TaskProgressWidget *task_prog_wgt;

DatabaseImportForm::DatabaseImportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	import_thread=new QThread(this);
	import_helper.moveToThread(import_thread);
	model_wgt=nullptr;

	connect(close_btn, SIGNAL(clicked(bool)), this, SLOT(close(void)));
	connect(connect_tb, SIGNAL(clicked(bool)), this, SLOT(listDatabases(void)));
	connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listObjects(void)));
	connect(import_sys_objs_chk, SIGNAL(clicked(bool)), this, SLOT(listObjects(void)));
	connect(import_ext_objs_chk, SIGNAL(clicked(bool)), this, SLOT(listObjects(void)));
	connect(by_oid_chk, SIGNAL(toggled(bool)), this, SLOT(filterObjects(void)));
	connect(expand_all_tb, SIGNAL(clicked(bool)), db_objects_tw, SLOT(expandAll(void)));
	connect(collapse_all_tb, SIGNAL(clicked(bool)), db_objects_tw, SLOT(collapseAll(void)));
	connect(db_objects_tw, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(setItemCheckState(QTreeWidgetItem*,int)));
	connect(select_all_tb, SIGNAL(clicked(bool)), this, SLOT(setItemsCheckState(void)));
	connect(clear_all_tb, SIGNAL(clicked(bool)), this, SLOT(setItemsCheckState(void)));
	connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterObjects(void)));
	connect(&import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
	connect(&import_helper, SIGNAL(s_importCanceled(void)), this, SLOT(handleImportCanceled(void)));
	connect(&import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	connect(&import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)));
	connect(import_btn, SIGNAL(clicked(bool)), this, SLOT(importDatabase(void)));
	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(cancelImport(void)));
	connect(import_thread, SIGNAL(started(void)), &import_helper, SLOT(importDatabase(void)));
	connect(&timer, SIGNAL(timeout(void)), this, SLOT(hideProgress()));
}

void DatabaseImportForm::updateProgress(int progress, QString msg, ObjectType obj_type)
{
	msg.replace(msg.indexOf('`'), 1 ,"<strong>");
	msg.replace(msg.indexOf('\''), 1,"</strong>");
	msg.replace(msg.indexOf('`'), 1 ,"<em>");
	msg.replace(msg.indexOf('\''), 1,"</em>");
	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=BASE_OBJECT)
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png")));
	else
		ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));

	this->repaint();
}

void DatabaseImportForm::setItemCheckState(QTreeWidgetItem *item, int)
{
	db_objects_tw->blockSignals(true);
	setItemCheckState(item, item->checkState(0));
	setParentItemChecked(item->parent());
	import_btn->setEnabled(hasCheckedItems());
	db_objects_tw->blockSignals(false);
}

void DatabaseImportForm::setItemsCheckState(void)
{
	QTreeWidgetItemIterator itr(db_objects_tw);
	Qt::CheckState chk_state=(sender()==select_all_tb ? Qt::Checked : Qt::Unchecked);

	db_objects_tw->blockSignals(true);
	while(*itr)
	{
		if(!(*itr)->isDisabled())
			(*itr)->setCheckState(0, chk_state);

		++itr;
	}
	db_objects_tw->blockSignals(false);
	import_btn->setEnabled(hasCheckedItems());
}

void DatabaseImportForm::importDatabase(void)
{
	try
	{
		map<ObjectType, vector<unsigned>> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;

		getCheckedItems(obj_oids, col_oids);
		obj_oids[OBJ_DATABASE].push_back(database_cmb->itemData(database_cmb->currentIndex()).value<unsigned>());

		model_wgt=new ModelWidget;
		model_wgt->getDatabaseModel()->createSystemObjects(true);

		import_helper.setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(),
																	 resolve_deps_chk->isChecked(), ignore_errors_chk->isChecked(), debug_mode_chk->isChecked());
		import_helper.setSelectedOIDs(model_wgt, obj_oids, col_oids);

		timer.stop();

		if(!progress_pb->isVisible())
			hideProgress(false);

		import_thread->start();
		cancel_btn->setEnabled(true);
		import_btn->setEnabled(false);
		database_gb->setEnabled(false);
		options_gb->setEnabled(false);
	}
	catch(Exception &e)
	{	
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportForm::setItemCheckState(QTreeWidgetItem *item, Qt::CheckState chk_state)
{
	for(int i=0; i < item->childCount(); i++)
	{
		if(!item->child(i)->isDisabled() && !db_objects_tw->isItemHidden(item->child(i)))
			item->child(i)->setCheckState(0, chk_state);

		setItemCheckState(item->child(i), chk_state);
	}
}

void DatabaseImportForm::setParentItemChecked(QTreeWidgetItem *item)
{
	if(item && !item->isDisabled())
	{
		if(item->checkState(0)!=Qt::Checked)
			item->setCheckState(0, Qt::Checked);

		setParentItemChecked(item->parent());
	}
}

bool DatabaseImportForm::hasCheckedItems(void)
{
	QTreeWidgetItemIterator itr(db_objects_tw);
	bool selected=false;

	while(*itr && !selected)
	{
		//Only valid items (OID > 0) and with Checked state are considered as selected
		selected=((*itr)->checkState(0)==Qt::Checked && (*itr)->data(0, Qt::UserRole).value<unsigned>() > 0);
		++itr;
	}

	return(selected);
}

void DatabaseImportForm::getCheckedItems(map<ObjectType, vector<unsigned>> &obj_oids, map<unsigned, vector<unsigned>> &col_oids)
{
	QTreeWidgetItemIterator itr(db_objects_tw);
	ObjectType obj_type;
	unsigned tab_oid=0;

	obj_oids.clear();
	col_oids.clear();

	while(*itr)
	{
		//If the item is checked and its OID is valid
		if((*itr)->checkState(0)==Qt::Checked && (*itr)->data(0, Qt::UserRole).value<unsigned>() > 0)
		{
			obj_type=static_cast<ObjectType>((*itr)->data(1, Qt::UserRole).value<unsigned>());

			//If the object is not a column store it on general object list
			if(obj_type!=OBJ_COLUMN)
			 obj_oids[obj_type].push_back((*itr)->data(0, Qt::UserRole).value<unsigned>());
			//If its a column
			else
			{
				//Get the table's oid from the parent item
				tab_oid=(*itr)->parent()->parent()->data(0, Qt::UserRole).value<unsigned>();

				//Store the column oid on the selected colums map using the table oid as key
				col_oids[tab_oid].push_back((*itr)->data(0, Qt::UserRole).value<unsigned>());
			}
		}

		++itr;
	}
}

void DatabaseImportForm::listObjects(void)
{
	try
	{
		bool enable=false;

		if(database_cmb->currentIndex() > 0)
		{
			//Configuring the task progress widget to show the object retrieving progress
      task_prog_wgt->setWindowTitle(trUtf8("Retrieving objects from database..."));
      task_prog_wgt->show();

			//Set the working database on import helper
			import_helper.setCurrentDatabase(database_cmb->currentText());
			import_helper.setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(),
																		 resolve_deps_chk->isChecked(), ignore_errors_chk->isChecked(), debug_mode_chk->isChecked());

      //List the objects using the static helper method
      DatabaseImportForm::listObjects(import_helper, db_objects_tw, true);

      task_prog_wgt->close();
      disconnect(this, nullptr, task_prog_wgt, nullptr);
		}

		//Enable the control buttons only when objects were retrieved
		enable=(db_objects_tw->topLevelItemCount() > 0);
		select_all_tb->setEnabled(enable);
		clear_all_tb->setEnabled(enable);
		expand_all_tb->setEnabled(enable);
		collapse_all_tb->setEnabled(enable);
		filter_lbl->setEnabled(enable);
		filter_edt->setEnabled(enable);
		by_oid_chk->setEnabled(enable);
		import_btn->setEnabled(hasCheckedItems());
	}
	catch(Exception &e)
	{
		task_prog_wgt->close();
		disconnect(this, nullptr, task_prog_wgt, nullptr);
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void DatabaseImportForm::listDatabases(void)
{
  try
  {
    Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

    //List the available databases using the selected connection
    import_helper.setConnection(*conn);
    DatabaseImportForm::listDatabases(import_helper, database_cmb);

    db_objects_tw->clear();
    database_gb->setEnabled(database_cmb->count() > 1);
    database_cmb->setEnabled(database_gb->isEnabled());
    db_objects_tw->setEnabled(database_gb->isEnabled());
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void DatabaseImportForm::hideProgress(bool value)
{
	ln2_frm->setHidden(value);
	progress_lbl->setHidden(value);
	progress_pb->setHidden(value);
	cancel_btn->setHidden(value);
	progress_pb->setValue(0);
	ico_lbl->setHidden(value);

	this->resize(this->width(),
							 (value ? this->height() - 50 : this->height() + 50));

	if(value)	timer.stop();
}

void DatabaseImportForm::showEvent(QShowEvent *)
{
	map<QString, Connection *> connections;
	map<QString, Connection *>::iterator itr;

	debug_mode_chk->setChecked(false);
	ignore_errors_chk->setChecked(false);
	import_sys_objs_chk->blockSignals(true);
	import_sys_objs_chk->setChecked(false);
	import_sys_objs_chk->blockSignals(false);
	obj_spacing_sb->setValue(50);
	origin_sb->setValue(50);
	tabs_per_row_sb->setValue(5);
	sch_per_row_sb->setValue(3);
	database_cmb->clear();
	db_objects_tw->clear();
	database_cmb->setEnabled(false);
	db_objects_tw->setEnabled(false);

	//Get the current connections configured on the connections widget
	dynamic_cast<ConnectionsConfigWidget *>(configuration_form->getConfigurationWidget(ConfigurationForm::CONNECTIONS_CONF_WGT))->getConnections(connections);

	connections_cmb->clear();
	itr=connections.begin();

	//Add the connections to the combo
	while(itr!=connections.end())
	{
		connections_cmb->addItem(itr->first, QVariant::fromValue<void *>(itr->second));
		itr++;
	}

	hideProgress();
}

void DatabaseImportForm::closeEvent(QCloseEvent *event)
{
	/* Ignore the close event when the thread is running this avoid
	close the form and make thread execute in background */
	if(import_thread->isRunning())
		event->ignore();
	else
	{
		if(!model_wgt)
			this->setResult(QDialog::Rejected);
	}
}

void DatabaseImportForm::captureThreadError(Exception e)
{
	destroyModelWidget();
	finishImport(trUtf8("Importing process aborted!"));
	ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_erro.png")));
	throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void DatabaseImportForm::filterObjects(void)
{
  DatabaseImportForm::filterObjects(db_objects_tw, filter_edt->text(), by_oid_chk->isChecked());
}

void DatabaseImportForm::filterObjects(QTreeWidget *tree_wgt, const QString &pattern, bool filter_by_oid)
{
  QList<QTreeWidgetItem*> items=tree_wgt->findItems(pattern, Qt::MatchStartsWith | Qt::MatchRecursive, filter_by_oid);
  QTreeWidgetItemIterator itr(tree_wgt);
  QTreeWidgetItem *parent=nullptr;

  tree_wgt->blockSignals(true);
  tree_wgt->collapseAll();
  while(*itr)
  {
    (*itr)->setHidden(true);
    ++itr;
  }

  while(!items.isEmpty())
  {
    items.front()->setExpanded(true);
    items.front()->setHidden(false);
    parent=items.front()->parent();

    while(parent)
    {
      parent->setHidden(false);
      parent->setExpanded(true);
      parent=parent->parent();
    }

    items.pop_front();
  }
  tree_wgt->blockSignals(false);
}

void DatabaseImportForm::cancelImport(void)
{
	import_helper.cancelImport();
	cancel_btn->setEnabled(false);
}

void DatabaseImportForm::destroyModelWidget(void)
{
	if(model_wgt)
	{
		delete(model_wgt);
		model_wgt=nullptr;
	}
}

void DatabaseImportForm::handleImportCanceled(void)
{
	destroyModelWidget();
	finishImport(trUtf8("Importing process canceled by user!"));
	ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_alerta.png")));
}

void DatabaseImportForm::handleImportFinished(Exception e)
{	
	if(!e.getErrorMessage().isEmpty())
	{
		Messagebox msgbox;
		msgbox.show(e, e.getErrorMessage(), Messagebox::ALERT_ICON);
	}

	model_wgt->rearrangeSchemas(QPointF(origin_sb->value(), origin_sb->value()),
															tabs_per_row_sb->value(), sch_per_row_sb->value(), obj_spacing_sb->value());

	finishImport(trUtf8("Importing process sucessfuly ended!"));
	ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));
	this->accept();
}

void DatabaseImportForm::finishImport(const QString &msg)
{
	if(import_thread->isRunning())
		import_thread->quit();

	import_btn->setEnabled(true);
	cancel_btn->setEnabled(false);
	options_gb->setEnabled(true);
	database_gb->setEnabled(true);
	progress_pb->setValue(100);
	progress_lbl->setText(msg);
	progress_lbl->repaint();
	timer.start(5000);
}

ModelWidget *DatabaseImportForm::getModelWidget(void)
{
  return(model_wgt);
}

void DatabaseImportForm::listDatabases(DatabaseImportHelper &import_helper, QComboBox *dbcombo)
{
  if(dbcombo)
  {
    try
    {
      attribs_map db_attribs;
      attribs_map::iterator itr;
      QStringList list;
      map<QString, unsigned> oids;

      db_attribs=import_helper.getObjects(OBJ_DATABASE);
      dbcombo->clear();

      if(db_attribs.empty())
        dbcombo->addItem(QString("No databases found"));
      else
      {
        dbcombo->blockSignals(true);

        itr=db_attribs.begin();
        while(itr!=db_attribs.end())
        {
          list.push_back(itr->second);
          oids[itr->second]=itr->first.toUInt();
          itr++;
        }

        list.sort();
        dbcombo->addItems(list);

        for(int i=0; i < list.count(); i++)
        {
          dbcombo->setItemIcon(i, QPixmap(":/icones/icones/" + BaseObject::getSchemaName(OBJ_DATABASE) + ".png"));
          dbcombo->setItemData(i, oids[list[i]]);
        }

        dbcombo->insertItem(0, QString("Found %1 database(s)").arg(db_attribs.size()));
        dbcombo->setCurrentIndex(0);
        dbcombo->blockSignals(false);
      }
    }
    catch(Exception &e)
    {
      throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
    }
  }
}

void DatabaseImportForm::listObjects(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, bool checkable_items)
{
  try
  {
    if(tree_wgt)
    {
      vector<QTreeWidgetItem *> sch_items, tab_items;
      int inc=0, inc1=0;

      tree_wgt->clear();
      tree_wgt->setColumnHidden(1, true);

      //Retrieving and listing the cluster scoped objects
      sch_items=DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
                                    {OBJ_CAST, OBJ_ROLE, OBJ_LANGUAGE, OBJ_TABLESPACE, OBJ_SCHEMA}, checkable_items);

      inc=40/static_cast<float>(sch_items.size());

      while(!sch_items.empty())
      {
        //Retrieving and listing the schema scoped objects
        tab_items=DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
                                    {OBJ_AGGREGATE, OBJ_CONVERSION, OBJ_COLLATION, OBJ_DOMAIN, OBJ_EXTENSION, OBJ_FUNCTION,
                                     OBJ_OPCLASS, OBJ_OPERATOR, OBJ_OPFAMILY, OBJ_SEQUENCE, OBJ_TYPE, OBJ_TABLE, OBJ_VIEW},
                                    checkable_items, sch_items.back(), sch_items.back()->text(0));


        inc1=(60/static_cast<float>(tab_items.size()))/static_cast<float>(sch_items.size());

        while(!tab_items.empty())
        {
          DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
                            { OBJ_COLUMN, OBJ_CONSTRAINT, OBJ_RULE, OBJ_TRIGGER, OBJ_INDEX }, checkable_items,
                            tab_items.back(), sch_items.back()->text(0), tab_items.back()->text(0));
          tab_items.pop_back();

          if(task_prog_wgt->isVisible())
            task_prog_wgt->progress_pb->setValue(task_prog_wgt->progress_pb->value() + inc1);
        }

        if(task_prog_wgt->isVisible())
          task_prog_wgt->progress_pb->setValue(task_prog_wgt->progress_pb->value() + inc);

        sch_items.pop_back();
      }

      tree_wgt->sortItems(0, Qt::AscendingOrder);

      if(task_prog_wgt->isVisible())
        task_prog_wgt->progress_pb->setValue(100);
    }
  }
  catch(Exception &e)
  {
    tree_wgt->clear();
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

vector<QTreeWidgetItem *> DatabaseImportForm::updateObjectsTree(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, vector<ObjectType> types, bool checkable_items,
                                                                QTreeWidgetItem *root, const QString &schema, const QString &table)
{
  vector<QTreeWidgetItem *> items_vect;

  if(tree_wgt)
  {
    QTreeWidgetItem *group=nullptr, *item=nullptr;
    QFont grp_fnt=tree_wgt->font();
    attribs_map objects, extra_attribs={{ParsersAttributes::FILTER_TABLE_TYPES, "1"}};
    attribs_map::iterator itr;
    QString tooltip="OID: %1", msg=trUtf8("Retrieving `%1'...");
    bool child_checked=false;
    int progress=0;

    grp_fnt.setItalic(true);
    tree_wgt->blockSignals(true);

    try
    {
      for(unsigned i=0; i < types.size(); i++)
      {
        if(task_prog_wgt->isVisible())
        {
         progress=task_prog_wgt->progress_pb->value() + (i/static_cast<float>(types.size()));
         task_prog_wgt->updateProgress(progress, msg.arg(BaseObject::getTypeName(types[i])), types[i]);
        }

        //Retrieve the objects of the current type from the database
        objects=import_helper.getObjects(types[i], schema, table, extra_attribs);

        //Create a group item for the current type
        group=new QTreeWidgetItem(root);
        group->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i]) + QString("_grp.png")));
        group->setText(0, BaseObject::getTypeName(types[i]) + QString(" (%1)").arg(objects.size()));
        group->setFont(0, grp_fnt);

        //Group items does contains a zero valued id to indicate that is not a valide object
        group->setData(OBJECT_ID, Qt::UserRole, 0);
        group->setData(OBJECT_TYPE, Qt::UserRole, types[i]);
        group->setData(OBJECT_COUNT, Qt::UserRole, QVariant::fromValue<unsigned>(objects.size()));

        //Creates individual items for each object of the current type
        itr=objects.begin();
        while(itr!=objects.end())
        {
          item=new QTreeWidgetItem(group);

          item->setIcon(0, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(types[i]) + QString(".png")));
          item->setText(0, itr->second);
          item->setText(1, itr->first);

          if(checkable_items)
          {
            if((itr->first.toUInt() > import_helper.getLastSystemOID()) ||
               (types[i]==OBJ_SCHEMA && itr->second=="public") ||
               (types[i]==OBJ_COLUMN && root && root->data(0, Qt::UserRole).toUInt() > import_helper.getLastSystemOID()))
            {
              item->setCheckState(0, Qt::Checked);
              child_checked=true;
            }
            else
              item->setCheckState(0, Qt::Unchecked);

            //Disabling items that refers to PostgreSQL's built-in data types
            if(types[i]==OBJ_TYPE && itr->first.toUInt() <= import_helper.getLastSystemOID())
            {
              item->setDisabled(true);
              item->setToolTip(0, trUtf8("This is a PostgreSQL built-in data type and cannot be imported."));
            }
            //Disabling items that refers to pgModeler's built-in system objects
            else if((types[i]==OBJ_TABLESPACE && (itr->second=="pg_default" || itr->second=="pg_global")) ||
                    (types[i]==OBJ_ROLE && (itr->second=="postgres")) ||
                    (types[i]==OBJ_SCHEMA && (itr->second=="pg_catalog" || itr->second=="public")) ||
                    (types[i]==OBJ_LANGUAGE && (itr->second==~LanguageType(LanguageType::c) ||
                                                itr->second==~LanguageType(LanguageType::sql) ||
                                                itr->second==~LanguageType(LanguageType::plpgsql))))
            {
              item->setFont(0, grp_fnt);
              item->setForeground(0, BaseObjectView::getFontStyle(ParsersAttributes::PROT_COLUMN).foreground());
              item->setToolTip(0, trUtf8("This is a pgModeler's built-in object. It will be ignored if checked by user."));
            }
          }

          //Stores the object's OID as the first data of the item
          item->setData(OBJECT_ID, Qt::UserRole, itr->first.toUInt());

          if(!item->toolTip(0).isEmpty())
            item->setToolTip(0,item->toolTip(0) + "\n" + tooltip.arg(itr->first));
          else
            item->setToolTip(0,tooltip.arg(itr->first));

          //Stores the object's type as the second data of the item
          item->setData(OBJECT_TYPE, Qt::UserRole, types[i]);

          //Stores the schema and the table's name of the object
          item->setData(OBJECT_SCHEMA, Qt::UserRole, schema);
          item->setData(OBJECT_TABLE, Qt::UserRole, table);

          if(types[i]==OBJ_SCHEMA || types[i]==OBJ_TABLE)
            items_vect.push_back(item);

          itr++;
        }

        //Disables the item if no object were retrieved from database
        group->setDisabled(objects.empty());

        if(checkable_items)
        {
          if(!group->isDisabled() && child_checked)
            group->setCheckState(0, Qt::Checked);
          else
            group->setCheckState(0, Qt::Unchecked);
        }

        tree_wgt->addTopLevelItem(group);
      }

      tree_wgt->blockSignals(false);
    }
    catch(Exception &e)
    {
      throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
    }
  }
  return(items_vect);
}
