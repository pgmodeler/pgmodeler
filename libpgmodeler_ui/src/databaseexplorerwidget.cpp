/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "databaseexplorerwidget.h"
#include "databaseimportform.h"

DatabaseExplorerWidget::DatabaseExplorerWidget(QWidget *parent): QWidget(parent)
{
  setupUi(this);

  drop_action=new QAction(QIcon(":icones/icones/excluir.png"), trUtf8("Drop object"), &handle_menu);
  drop_action->setShortcut(QKeySequence(Qt::Key_Delete));

  drop_cascade_action=new QAction(QIcon(":icones/icones/excluir.png"), trUtf8("Drop cascade"), &handle_menu);
  drop_cascade_action->setShortcut(QKeySequence("Shift+Del"));

  show_data_action=new QAction(QIcon(":icones/icones/result.png"), trUtf8("Show data"), &handle_menu);

  refresh_action=new QAction(QIcon(":icones/icones/atualizar.png"), trUtf8("Update"), &handle_menu);
  refresh_action->setShortcut(QKeySequence(Qt::Key_F5));

  objects_trw->installEventFilter(this);

  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(listObjects(void)));
  connect(expand_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(expandAll(void)));
  connect(collapse_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(collapseAll(void)));
  connect(ext_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(sys_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(objects_trw, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(handleObject(QTreeWidgetItem *,int)));
}

bool DatabaseExplorerWidget::eventFilter(QObject *object, QEvent *event)
{
  if(object==objects_trw && event->type()==QEvent::KeyPress)
  {
    QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

    if(k_event->key()==Qt::Key_Delete || k_event->key()==Qt::Key_F5)
    {
      if(k_event->key()==Qt::Key_F5)
        updateCurrentItem();
      else
        dropObject(objects_trw->currentItem(), k_event->modifiers()==Qt::ShiftModifier);

      return(true);
    }
    else
      return(false);
  }

  return(QWidget::eventFilter(object, event));
}

void DatabaseExplorerWidget::setConnection(Connection conn)
{
  this->connection=conn;
}

Connection DatabaseExplorerWidget::getConnection(void)
{
  return(connection);
}

void DatabaseExplorerWidget::listObjects(void)
{
  try
  {
    configureImportHelper();
    DatabaseImportForm::listObjects(import_helper, objects_trw, false, false);
    import_helper.closeConnection();
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void DatabaseExplorerWidget::configureImportHelper(void)
{
  import_helper.setConnection(connection);
  import_helper.setCurrentDatabase(connection.getConnectionParam(Connection::PARAM_DB_NAME));
  import_helper.setImportOptions(sys_objs_chk->isChecked(), ext_objs_chk->isChecked(), false, false, false, false);
}

void DatabaseExplorerWidget::handleObject(QTreeWidgetItem *item, int)
{
  if(QApplication::mouseButtons()==Qt::RightButton)
  {
    ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
    unsigned obj_id=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

    for(auto act : handle_menu.actions())
      handle_menu.removeAction(act);

    if(obj_id > 0)
    {
      if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
        handle_menu.addAction(show_data_action);

      handle_menu.addAction(drop_action);
      handle_menu.addAction(drop_cascade_action);

      if(obj_type==OBJ_TABLE || obj_type==OBJ_SCHEMA)
        handle_menu.addAction(refresh_action);

      if(!handle_menu.actions().isEmpty())
      {
        QAction *exec_action=handle_menu.exec(QCursor::pos());

        if(exec_action==drop_action || exec_action==drop_cascade_action)
          dropObject(item,  exec_action==drop_cascade_action);
        else if(exec_action==show_data_action)
          emit s_dataGridOpenRequested(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
                                       item->text(0),
                                       item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt()!=OBJ_VIEW);
      }
    }
    else
    {
      handle_menu.addAction(refresh_action);
      handle_menu.exec(QCursor::pos());
    }
  }
  else if(QApplication::mouseButtons()==Qt::LeftButton)
  {
    loadObjectProperties();
  }
}

void DatabaseExplorerWidget::dropObject(QTreeWidgetItem *item, bool cascade)
{
  Messagebox msg_box;

  try
  {
    if(item && static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt()) > 0)
    {
      ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
      QString msg;

      if(!cascade)
        msg=trUtf8("Do you really want to drop the object <strong>%1</strong> <em>(%2)</em>?")
            .arg(item->text(0)).arg(BaseObject::getTypeName(obj_type));
      else
        msg=trUtf8("Do you really want to <strong>cascade</strong> drop the object <strong>%1</strong> <em>(%2)</em>? This action will drop all the other objects that depends on it?")
            .arg(item->text(0)).arg(BaseObject::getTypeName(obj_type));

      msg_box.show(trUtf8("Confirmation"), msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Accepted)
      {
        QTreeWidgetItem *parent=nullptr;
        attribs_map attribs;
        QStringList types;
        QString drop_cmd, obj_name=item->text(0);
        int idx=0, idx1=0;
        Connection conn;

        attribs[ParsersAttributes::SQL_OBJECT]=BaseObject::getSQLName(obj_type);
        attribs[ParsersAttributes::DECL_IN_TABLE]="";
        attribs[BaseObject::getSchemaName(obj_type)]="1";

        //For cast, operator and function is needed to extract the name and the params types
        if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION || obj_type==OBJ_CAST)
        {
          idx=obj_name.indexOf('(');
          idx1=obj_name.indexOf(')');
          types=obj_name.mid(idx+1, idx1-idx-1).split(',');
          types.removeAll("-");
          obj_name.remove(idx, obj_name.size());
        }

        //Formatting the names
        attribs[ParsersAttributes::NAME]=BaseObject::formatName(obj_name, obj_type==OBJ_OPERATOR);
        attribs[ParsersAttributes::TABLE]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString());
        attribs[ParsersAttributes::SCHEMA]=BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString());

        //For table objects the "table" attribute must be schema qualified
        if(obj_type!=OBJ_INDEX && TableObject::isTableObject(obj_type))
          attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::TABLE];
        //For operators and functions there must exist the signature attribute
        else if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION)
          attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::NAME] + QString("(%1)").arg(types.join(','));
        else if(obj_type==OBJ_CAST)
          attribs[ParsersAttributes::SIGNATURE]=QString("(%1 AS %2)").arg(types[0]).arg(types[1]);
        else
        {
          if(!attribs[ParsersAttributes::SCHEMA].isEmpty() &&
             attribs[ParsersAttributes::NAME].indexOf(attribs[ParsersAttributes::SCHEMA] + ".") < 0)
            attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::NAME];
          else
            attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::NAME];
        }

        //Generate the drop command
        schparser.setIgnoreEmptyAttributes(true);
        schparser.setIgnoreUnkownAttributes(true);
        drop_cmd=schparser.getCodeDefinition(ParsersAttributes::DROP, attribs, SchemaParser::SQL_DEFINITION);

        if(cascade)
          drop_cmd.replace(";"," CASCADE;");

        //Executes the drop cmd
        conn=connection;
        conn.connect();
        conn.executeDDLCommand(drop_cmd);

        //Updates the object count on the parent item
        parent=item->parent();
        if(parent && parent->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt()==0)
        {
          unsigned cnt=parent->data(DatabaseImportForm::OBJECT_COUNT, Qt::UserRole).toUInt();
          ObjectType parent_type=static_cast<ObjectType>(parent->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());

          cnt--;
          parent->setText(0, BaseObject::getTypeName(parent_type) + QString(" (%1)").arg(cnt));
          parent->setData(DatabaseImportForm::OBJECT_COUNT, Qt::UserRole, QVariant::fromValue<unsigned>(cnt));
        }

        if(parent)
          parent->takeChild(parent->indexOfChild(item));
        else
          objects_trw->takeTopLevelItem(objects_trw->indexOfTopLevelItem(item));

      }
    }
  }
  catch(Exception &e)
  {
    msg_box.show(e);
  }
}

void DatabaseExplorerWidget::updateCurrentItem(void)
{
  QTreeWidgetItem *item=objects_trw->currentItem();

  if(item)
  {
    QTreeWidgetItem *root=nullptr, *parent=nullptr;
    ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
    unsigned obj_id=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt());
    QString sch_name, tab_name;
    vector<QTreeWidgetItem *> gen_items, gen_items1;
    vector<ObjectType> db_types=BaseObject::getChildObjectTypes(OBJ_DATABASE);

    parent=item->parent();
    objects_trw->takeTopLevelItem(objects_trw->indexOfTopLevelItem(item));
    sch_name=item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString();
    tab_name=item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString();

    if(parent)
    {
      if(obj_id==0)
      {
        root=parent;
        parent->takeChild(parent->indexOfChild(item));
      }
      else
      {
        if(obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE)
        {
          root=item;
          root->takeChildren();

          if(obj_type==OBJ_TABLE)
            tab_name=item->text(0);
          else
            sch_name=item->text(0);
        }
        else
        {
          //If the object type is a database child one
          if(std::find(db_types.begin(), db_types.end(), obj_type)!=db_types.end())
          {
            root=nullptr;
            objects_trw->takeTopLevelItem(objects_trw->indexOfTopLevelItem(parent));
          }
          else
          {
            //If the type is a table child object remove the group of current type
            if(TableObject::isTableObject(obj_type))
            {
              root=parent->parent();
              root->takeChild(root->indexOfChild(parent));
            }
            else
            {
              root=parent;
              parent->takeChild(parent->indexOfChild(item));
            }
          }
        }
      }
    }
    else
      objects_trw->takeTopLevelItem(objects_trw->indexOfTopLevelItem(item));

    configureImportHelper();

    //Updates the group type only
    if(obj_id==0 || (obj_type!=OBJ_TABLE && obj_type!=OBJ_SCHEMA))
      gen_items=DatabaseImportForm::updateObjectsTree(import_helper, objects_trw, { obj_type }, false, false, root, sch_name, tab_name);
    else
      //Updates all child objcts when the selected object is a schema or table
      gen_items=DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                      BaseObject::getChildObjectTypes(obj_type), false, false, root, sch_name, tab_name);

    //Updating the subtree for schemas / tables
    if(obj_type==OBJ_SCHEMA || obj_type==OBJ_TABLE)
    {
      for(auto item : gen_items)
      {
        //When the user refresh a single schema or table
        if(obj_id > 0 || obj_type==OBJ_TABLE)
        {
          //Updates the table subtree
          DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                BaseObject::getChildObjectTypes(OBJ_TABLE),
                                                false, false, item,
                                                item->parent()->data(DatabaseImportForm::OBJECT_SCHEMA,Qt::UserRole).toString(),
                                                item->text(0));
        }
        //When the user refresh the schema group
        else
        {
          //Updates the entire schema subtree
          gen_items1= DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                            BaseObject::getChildObjectTypes(OBJ_SCHEMA),
                                                            false, false, item, item->text(0));

          //Updates the table group for the current schema
          for(auto item1 : gen_items1)
          {
            DatabaseImportForm::updateObjectsTree(import_helper, objects_trw,
                                                  BaseObject::getChildObjectTypes(OBJ_TABLE),
                                                  false, false, item1,
                                                  item1->parent()->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
                                                  item1->text(0));
          }
        }
      }
    }

    import_helper.closeConnection();
    objects_trw->sortItems(0, Qt::AscendingOrder);
  }
}

void DatabaseExplorerWidget::loadObjectProperties(void)
{
  try
  {
    QTreeWidgetItem *item=objects_trw->currentItem();
    unsigned oid=item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt();

    while(properties_tbw->rowCount() > 0)
      properties_tbw->removeRow(0);

    if(oid != 0)
    {
      Catalog catalog;
      Connection conn=connection;
      unsigned filter=Catalog::LIST_ALL_OBJS;
      ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());
      QString sch_name=item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString(),
              tab_name=item->data(DatabaseImportForm::OBJECT_TABLE, Qt::UserRole).toString();
      vector<attribs_map> attribs;
      QTableWidgetItem *item=nullptr;
      int row=0;
      QFont font;

      if(!sys_objs_chk->isChecked())
        filter=filter | Catalog::EXCL_SYSTEM_OBJS;

      if(!ext_objs_chk->isChecked())
        filter=filter | Catalog::EXCL_EXTENSION_OBJS;

      catalog.setFilter(filter);
      catalog.setConnection(conn);
      attribs=catalog.getObjectsAttributes(obj_type, sch_name, tab_name, { oid });

      if(attribs.size()==1)
      {
        for(auto attrib : attribs[0])
        {
          properties_tbw->insertRow(properties_tbw->rowCount());
          row=properties_tbw->rowCount() - 1;

          item=new QTableWidgetItem;
          font=item->font();
          font.setItalic(true);
          item->setText(attrib.first);
          item->setFont(font);
          item->setIcon(QPixmap(":/icones/icones/attribute.png"));
          properties_tbw->setItem(row, 0, item);

          item=new QTableWidgetItem;

          if(attrib.first==ParsersAttributes::OBJECT_TYPE)
            item->setText(BaseObject::getTypeName(static_cast<ObjectType>(attrib.second.toUInt())));
          else
            item->setText(attrib.second);

          properties_tbw->setItem(row, 1, item);
        }
      }
    }

    properties_tbw->horizontalHeader()->setVisible(properties_tbw->rowCount() > 0);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
  }
}
