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

#include "databaseimportform.h"
#include "taskprogresswidget.h"
#include "configurationform.h"
#include "taskprogresswidget.h"
#include "pgmodeleruins.h"
#include "pgmodelerns.h"

DatabaseImportForm::DatabaseImportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	model_wgt=nullptr;
  create_model=true;

  htmlitem_del=new HtmlItemDelegate;
  output_trw->setItemDelegateForColumn(0, htmlitem_del);

  rand_color_ht=new HintTextWidget(rand_color_hint, this);
  rand_color_ht->setText(rand_rel_color_chk->statusTip());

  auto_res_deps_ht=new HintTextWidget(auto_res_deps_hint, this);
  auto_res_deps_ht->setText(resolve_deps_chk->statusTip());

  imp_sys_objs_ht=new HintTextWidget(imp_sys_objs_hint, this);
  imp_sys_objs_ht->setText(import_sys_objs_chk->statusTip());

  imp_ext_objs_ht=new HintTextWidget(imp_ext_objs_hint, this);
  imp_ext_objs_ht->setText(import_ext_objs_chk->statusTip());

  debug_mode_ht=new HintTextWidget(debug_mode_hint, this);
  debug_mode_ht->setText(debug_mode_chk->statusTip());

  ignore_errors_ht=new HintTextWidget(ignore_errors_hint, this);
  ignore_errors_ht->setText(ignore_errors_chk->statusTip());

  import_to_model_ht=new HintTextWidget(import_to_model_hint, this);
  import_to_model_ht->setText(import_to_model_chk->statusTip());

  settings_tbw->setTabEnabled(1, false);

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
	connect(import_btn, SIGNAL(clicked(bool)), this, SLOT(importDatabase(void)));
	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(cancelImport(void)));

  connect(import_to_model_chk, &QCheckBox::toggled,
          [=](bool checked){ create_model=!checked; });

  connect(database_cmb, &QComboBox::currentTextChanged,
          [=]() {
                  if(database_cmb->currentIndex()==0)
                    db_objects_tw->clear();

                  import_btn->setEnabled(database_cmb->currentIndex() > 0);
                  objs_parent_wgt->setEnabled(database_cmb->currentIndex() > 0);
                });

  objs_parent_wgt->setEnabled(false);
  ConnectionsConfigWidget::fillConnectionsComboBox(connections_cmb);
  createThread();
}

DatabaseImportForm::~DatabaseImportForm(void)
{
  destroyThread();
}

void DatabaseImportForm::setModelWidget(ModelWidget *model)
{
  model_wgt=model;
  import_to_model_chk->setEnabled(model!=nullptr);
}

void DatabaseImportForm::createThread(void)
{
  import_thread=new QThread;
  import_helper=new DatabaseImportHelper;
  import_helper->moveToThread(import_thread);

  connect(import_thread, SIGNAL(started(void)), import_helper, SLOT(importDatabase()));
  connect(import_helper, SIGNAL(s_importCanceled()), this, SLOT(handleImportCanceled()));
  connect(import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
  connect(import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
  connect(import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)), Qt::BlockingQueuedConnection);
}

void DatabaseImportForm::destroyThread(void)
{
  if(import_thread)
  {
    import_thread->quit();
    import_thread->wait();
    delete(import_thread);
    import_thread=nullptr;
    delete(import_helper);
    import_helper=nullptr;
  }
}

void DatabaseImportForm::updateProgress(int progress, QString msg, ObjectType obj_type)
{
  QPixmap ico;

  msg=PgModelerUiNS::formatMessage(msg);
	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=BASE_OBJECT)
    ico=QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(obj_type) + QString(".png"));
	else
    ico=QPixmap(QString(":/icones/icones/msgbox_info.png"));

  ico_lbl->setPixmap(ico);
  PgModelerUiNS::createOutputTreeItem(output_trw, msg, ico);
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
    Messagebox msg_box;

		map<ObjectType, vector<unsigned>> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;   

    if(import_to_model_chk->isChecked())
    {
      msg_box.show(trUtf8("<strong>ATTENTION:</strong> You are about to import objects to the current working model! This action will cause irreversible changes to it even in case of critical errors during the process. Do you want to proceed?"),
                   Messagebox::ALERT_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Rejected)
        return;
    }

    output_trw->clear();
    settings_tbw->setTabEnabled(1, true);
    settings_tbw->setCurrentIndex(1);

		getCheckedItems(obj_oids, col_oids);
		obj_oids[OBJ_DATABASE].push_back(database_cmb->itemData(database_cmb->currentIndex()).value<unsigned>());

    if(create_model)
    {
      model_wgt=new ModelWidget;
      model_wgt->getDatabaseModel()->createSystemObjects(true);
    }

    model_wgt->setUpdatesEnabled(false);
    import_helper->setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(),
																	 resolve_deps_chk->isChecked(), ignore_errors_chk->isChecked(),
																	 debug_mode_chk->isChecked(), rand_rel_color_chk->isChecked());

    import_helper->setSelectedOIDs(model_wgt->getDatabaseModel(), obj_oids, col_oids);
		import_thread->start();
		cancel_btn->setEnabled(true);
		import_btn->setEnabled(false);
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
		selected=((*itr)->checkState(0)==Qt::Checked && (*itr)->data(OBJECT_ID, Qt::UserRole).value<unsigned>() > 0);
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
    if((*itr)->checkState(0)==Qt::Checked && (*itr)->data(OBJECT_ID, Qt::UserRole).value<unsigned>() > 0)
		{
      obj_type=static_cast<ObjectType>((*itr)->data(OBJECT_TYPE, Qt::UserRole).value<unsigned>());

			//If the object is not a column store it on general object list
			if(obj_type!=OBJ_COLUMN)
       obj_oids[obj_type].push_back((*itr)->data(OBJECT_ID, Qt::UserRole).value<unsigned>());
			//If its a column
			else
			{
				//Get the table's oid from the parent item
        tab_oid=(*itr)->parent()->parent()->data(OBJECT_ID, Qt::UserRole).value<unsigned>();

				//Store the column oid on the selected colums map using the table oid as key
        col_oids[tab_oid].push_back((*itr)->data(OBJECT_ID, Qt::UserRole).value<unsigned>());
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
      Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

      //Set the working database on import helper
      import_helper->closeConnection();
      import_helper->setConnection(*conn);
      import_helper->setCurrentDatabase(database_cmb->currentText());
      import_helper->setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(),
																		 resolve_deps_chk->isChecked(), ignore_errors_chk->isChecked(),
																		 debug_mode_chk->isChecked(), rand_rel_color_chk->isChecked());

      //List the objects using the static helper method
      DatabaseImportForm::listObjects(*import_helper, db_objects_tw, true, true, false);
		}

		//Enable the control buttons only when objects were retrieved
		enable=(db_objects_tw->topLevelItemCount() > 0);
    objs_parent_wgt->setEnabled(enable);
		import_btn->setEnabled(hasCheckedItems());
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void DatabaseImportForm::listDatabases(void)
{
  try
  {
    Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

    //Close a previous connection opened by the import helper
    import_helper->closeConnection();

    //List the available databases using the selected connection
    import_helper->setConnection(*conn);
    DatabaseImportForm::listDatabases(*import_helper, database_cmb);

    db_objects_tw->clear();
    database_cmb->setEnabled(database_cmb->count() > 1);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void DatabaseImportForm::closeEvent(QCloseEvent *event)
{
	/* Ignore the close event when the thread is running this avoid
	close the form and make thread execute in background */
	if(import_thread->isRunning())
		event->ignore();
	else
	{
    if(create_model && !model_wgt)
			this->setResult(QDialog::Rejected);

    import_helper->closeConnection();
	}
}

void DatabaseImportForm::captureThreadError(Exception e)
{
  QPixmap ico;
  QTreeWidgetItem *item=nullptr;

  if(!create_model)
    model_wgt->rearrangeSchemas(QPointF(origin_sb->value(), origin_sb->value()),
                                tabs_per_row_sb->value(), sch_per_row_sb->value(), obj_spacing_sb->value());

  destroyModelWidget();
	finishImport(trUtf8("Importing process aborted!"));

  ico=QPixmap(QString(":/icones/icones/msgbox_erro.png"));
  ico_lbl->setPixmap(ico);

  item=PgModelerUiNS::createOutputTreeItem(output_trw, PgModelerUiNS::formatMessage(e.getErrorMessage()), ico);

  if(!e.getExtraInfo().isEmpty())
   PgModelerUiNS::createOutputTreeItem(output_trw, PgModelerUiNS::formatMessage(e.getExtraInfo()), ico, item);

  //Destroy the current import thread and helper to avoid reuse
  destroyThread();

  //Recreates a new import thread and helper to force user to reconfigure the import
  createThread();

  database_cmb->setCurrentIndex(0);
	throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void DatabaseImportForm::filterObjects(void)
{
	DatabaseImportForm::filterObjects(db_objects_tw, filter_edt->text(), (by_oid_chk->isChecked() ? OBJECT_ID : 0));
}

void DatabaseImportForm::filterObjects(QTreeWidget *tree_wgt, const QString &pattern, int search_column)
{
	if(!tree_wgt)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QList<QTreeWidgetItem*> items=tree_wgt->findItems(pattern, Qt::MatchStartsWith | Qt::MatchRecursive, search_column);
	QTreeWidgetItemIterator itr(tree_wgt);
  QTreeWidgetItem *parent=nullptr, *item=nullptr;

	tree_wgt->blockSignals(true);
	tree_wgt->collapseAll();

	while(*itr)
	{
		(*itr)->setHidden(!pattern.isEmpty());
		++itr;
	}

	if(pattern.isEmpty())
	{
		tree_wgt->topLevelItem(0)->setExpanded(true);
	}
	else
	{
		while(!items.isEmpty())
		{
      item=items.front();
      item->setExpanded(true);
      item->setHidden(false);
      parent=item->parent();

			while(parent)
			{
				parent->setHidden(false);
				parent->setExpanded(true);
				parent=parent->parent();
			}

      items.pop_front();
		}
	}

	tree_wgt->blockSignals(false);
}

void DatabaseImportForm::cancelImport(void)
{
  import_helper->cancelImport();
	cancel_btn->setEnabled(false);
  database_cmb->setCurrentIndex(0);
}

void DatabaseImportForm::destroyModelWidget(void)
{
  if(create_model && model_wgt)
	{
		delete(model_wgt);
		model_wgt=nullptr;
	}
}

void DatabaseImportForm::handleImportCanceled(void)
{
  QPixmap ico=QPixmap(QString(":/icones/icones/msgbox_alerta.png"));
  QString msg=trUtf8("Importing process canceled by user!");

  if(!create_model)
    model_wgt->rearrangeSchemas(QPointF(origin_sb->value(), origin_sb->value()),
                                tabs_per_row_sb->value(), sch_per_row_sb->value(), obj_spacing_sb->value());

	destroyModelWidget();
  finishImport(msg);
  ico_lbl->setPixmap(ico);

  PgModelerUiNS::createOutputTreeItem(output_trw, msg, ico);
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
  model_wgt->getDatabaseModel()->setInvalidated(false);

	finishImport(trUtf8("Importing process sucessfuly ended!"));
  ico_lbl->setPixmap(QPixmap(QString(":/icones/icones/msgbox_info.png")));

  import_helper->closeConnection();
  import_thread->quit();
  import_thread->wait();

	this->accept();
}

void DatabaseImportForm::finishImport(const QString &msg)
{
  if(import_thread->isRunning())
    import_thread->quit();

	cancel_btn->setEnabled(false);
	options_gb->setEnabled(true);
	progress_pb->setValue(100);
	progress_lbl->setText(msg);
	progress_lbl->repaint();

  if(model_wgt)
  {
    model_wgt->setUpdatesEnabled(true);

    if(!create_model)
      model_wgt->getOperationList()->removeOperations();
  }
}

ModelWidget *DatabaseImportForm::getModelWidget(void)
{
  if(create_model)
    return(model_wgt);
  else
    return(nullptr);
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
				dbcombo->addItem(trUtf8("No databases found"));
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
          dbcombo->setItemIcon(i, QPixmap(QString(":/icones/icones/") + BaseObject::getSchemaName(OBJ_DATABASE) + QString(".png")));
          dbcombo->setItemData(i, oids[list[i]]);
        }

				dbcombo->insertItem(0, trUtf8("Found %1 database(s)").arg(db_attribs.size()));
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

void DatabaseImportForm::listObjects(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, bool checkable_items, bool disable_empty_grps, bool create_db_item)
{
	TaskProgressWidget task_prog_wgt;

  try
  {
    if(tree_wgt)
    {
      QTreeWidgetItem *db_item=nullptr;
      vector<QTreeWidgetItem *> sch_items, tab_items;
      int inc=0, inc1=0;

			task_prog_wgt.setWindowTitle(trUtf8("Retrieving objects from database..."));
			task_prog_wgt.show();

      tree_wgt->clear();
      tree_wgt->setColumnHidden(1, true);
			task_prog_wgt.updateProgress(1, trUtf8("Retrieving cluster level objects..."), OBJ_DATABASE);

      if(create_db_item)
      {
        Catalog catalog=import_helper.getCatalog();
        vector<attribs_map> attribs;

        //Creating database item
        db_item=new QTreeWidgetItem;
        db_item->setText(0, import_helper.getCurrentDatabase());
        db_item->setIcon(0, QPixmap(QString(":/icones/icones/database.png")));
        attribs=catalog.getObjectsAttributes(OBJ_DATABASE, QString(), QString(), {}, {{ParsersAttributes::NAME, import_helper.getCurrentDatabase()}});

        db_item->setData(OBJECT_ID, Qt::UserRole, attribs[0].at(ParsersAttributes::OID));
        db_item->setData(OBJECT_TYPE, Qt::UserRole, OBJ_DATABASE);
        db_item->setData(OBJECT_TYPE, Qt::UserRole, OBJ_DATABASE);
        db_item->setToolTip(0, QString("OID: %1").arg(attribs[0].at(ParsersAttributes::OID)));
        tree_wgt->addTopLevelItem(db_item);
      }

      //Retrieving and listing the cluster scoped objects
      sch_items=DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
                                    BaseObject::getChildObjectTypes(OBJ_DATABASE), checkable_items, disable_empty_grps, db_item);

      inc=40/static_cast<float>(sch_items.size());


      while(!sch_items.empty())
      {
				task_prog_wgt.updateProgress(task_prog_wgt.progress_pb->value(), trUtf8("Retrieving objects of schema `%1'...").arg(sch_items.back()->text(0)), OBJ_SCHEMA);

        //Retrieving and listing the schema scoped objects
        tab_items=DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
                                     BaseObject::getChildObjectTypes(OBJ_SCHEMA),
                                    checkable_items, disable_empty_grps, sch_items.back(), sch_items.back()->text(0));

        inc1=(60/static_cast<float>(tab_items.size()))/static_cast<float>(sch_items.size());

        while(!tab_items.empty())
        {
          DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
                              BaseObject::getChildObjectTypes(OBJ_TABLE), checkable_items, disable_empty_grps,
                            tab_items.back(), sch_items.back()->text(0), tab_items.back()->text(0));

					task_prog_wgt.updateProgress(task_prog_wgt.progress_pb->value() + inc1, trUtf8("Retrieving objects of table `%1'...").arg(tab_items.back()->text(0)), OBJ_TABLE);
					tab_items.pop_back();
				}

				task_prog_wgt.progress_pb->setValue(task_prog_wgt.progress_pb->value() + inc);
        sch_items.pop_back();
      }

      tree_wgt->sortItems(0, Qt::AscendingOrder);

      if(db_item)
        db_item->setExpanded(true);

      task_prog_wgt.progress_pb->setValue(100);
			task_prog_wgt.close();

    }
  }
  catch(Exception &e)
  {
		task_prog_wgt.close();
    tree_wgt->clear();
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

vector<QTreeWidgetItem *> DatabaseImportForm::updateObjectsTree(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, vector<ObjectType> types, bool checkable_items,
                                                                bool disable_empty_grps, QTreeWidgetItem *root, const QString &schema, const QString &table)
{
  vector<QTreeWidgetItem *> items_vect;

  if(tree_wgt)
  {
    QTreeWidgetItem *group=nullptr, *item=nullptr;
    QFont grp_fnt=tree_wgt->font();
    attribs_map objects, extra_attribs={{ParsersAttributes::FILTER_TABLE_TYPES, ParsersAttributes::_TRUE_}};
    attribs_map::iterator itr;
    QString tooltip=QString("OID: %1");
    bool child_checked=false;

    grp_fnt.setItalic(true);
    tree_wgt->blockSignals(true);

    try
    {
      for(unsigned i=0; i < types.size(); i++)
      {
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
        group->setData(OBJECT_SCHEMA, Qt::UserRole, schema);
        group->setData(OBJECT_TABLE, Qt::UserRole, table);

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
               (types[i]==OBJ_SCHEMA && itr->second==QString("public")) ||
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
            else if((types[i]==OBJ_TABLESPACE && (itr->second==QString("pg_default") || itr->second==QString("pg_global"))) ||
                    (types[i]==OBJ_ROLE && (itr->second==QString("postgres"))) ||
                    (types[i]==OBJ_SCHEMA && (itr->second==QString("pg_catalog") || itr->second==QString("public"))) ||
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
            item->setToolTip(0,item->toolTip(0) + QString("\n") + tooltip.arg(itr->first));
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

        group->setDisabled(disable_empty_grps && objects.empty());

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
