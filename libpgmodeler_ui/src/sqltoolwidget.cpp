/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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

#include "sqltoolwidget.h"
#include "taskprogresswidget.h"

extern TaskProgressWidget *task_prog_wgt;

SQLToolWidget::SQLToolWidget(QWidget * parent) : QWidget(parent)
{
  setupUi(this);

  sql_cmd_hl=new SyntaxHighlighter(sql_cmd_txt, false, false);
  sql_cmd_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
                                GlobalAttributes::DIR_SEPARATOR +
                                GlobalAttributes::SQL_HIGHLIGHT_CONF +
                                GlobalAttributes::CONFIGURATION_EXT);

  h_splitter->setSizes({0, 10000});
  h_splitter1->setSizes({1000, 250});
  results_parent->setVisible(false);
  cmd_history_gb->setVisible(false);

  sql_file_dlg.setDefaultSuffix("sql");
  sql_file_dlg.setFileMode(QFileDialog::AnyFile);
  sql_file_dlg.setNameFilter(tr("SQL file (*.sql);;All files (*.*)"));
  sql_file_dlg.setModal(true);

  csv_file_dlg.setDefaultSuffix("csv");
  csv_file_dlg.setFileMode(QFileDialog::AnyFile);
  csv_file_dlg.setWindowTitle(trUtf8("Save CSV file"));
  csv_file_dlg.setNameFilter(tr("Comma-separated values file (*.csv);;All files (*.*)"));
  csv_file_dlg.setModal(true);
  csv_file_dlg.setAcceptMode(QFileDialog::AcceptSave);

  copy_action=new QAction(trUtf8("Copy"), &copy_menu);
  copy_menu.addAction(copy_action);

  drop_action=new QAction(QIcon(":icones/icones/excluir.png"), trUtf8("Drop object"), &handle_menu);
  drop_action->setShortcut(QKeySequence(Qt::Key_Delete));

  show_data_action=new QAction(QIcon(":icones/icones/result.png"), trUtf8("Show data"), &handle_menu);

  refresh_action=new QAction(QIcon(":icones/icones/atualizar.png"), trUtf8("Update"), &handle_menu);
  refresh_action->setShortcut(QKeySequence(Qt::Key_F5));

  connect(hide_tb, SIGNAL(clicked(void)), this, SLOT(hide(void)));
  connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearAll(void)));
  connect(connect_tb, SIGNAL(clicked(void)), this, SLOT(connectToDatabase(void)));
  connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listObjects(void)));
  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(listObjects(void)));
  connect(expand_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(expandAll(void)));
  connect(collapse_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(collapseAll(void)));
  connect(sql_cmd_txt, SIGNAL(textChanged(void)), this, SLOT(enableCommandButtons(void)));
  connect(run_sql_tb, SIGNAL(clicked(void)), this, SLOT(runSQLCommand(void)));
  connect(save_tb, SIGNAL(clicked(void)), this, SLOT(saveCommands(void)));
  connect(load_tb, SIGNAL(clicked(void)), this, SLOT(loadCommands(void)));
  connect(export_tb, SIGNAL(clicked(void)), this, SLOT(exportResults(void)));
  connect(history_tb, SIGNAL(toggled(bool)), cmd_history_gb, SLOT(setVisible(bool)));
  connect(results_tbw, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(copySelection(void)));
  connect(objects_trw, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(handleObject(QTreeWidgetItem *,int)));
  connect(clear_history_btn, SIGNAL(clicked(void)), cmd_history_lst, SLOT(clear(void)));
  connect(hide_ext_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(hide_sys_objs_chk, SIGNAL(toggled(bool)), this, SLOT(listObjects(void)));
  connect(refresh_action, SIGNAL(triggered()), this, SLOT(updateCurrentItem()));

  //Signal handling with C++11 lambdas Slots
  connect(clear_history_btn, &QPushButton::clicked,
          [=](){ clear_history_btn->setDisabled(true); });

  connect(cmd_history_lst, &QListWidget::itemDoubleClicked,
          [=](){ sql_cmd_txt->setText(cmd_history_lst->currentItem()->data(Qt::UserRole).toString()); });

  connect(filter_edt, &QLineEdit::textChanged,
          [=](){ DatabaseImportForm::filterObjects(objects_trw, filter_edt->text(), false); });

  objects_trw->installEventFilter(this);
}

void SQLToolWidget::updateConnections(map<QString, Connection *> &conns)
{
  map<QString, Connection *>::iterator itr=conns.begin();
  connections_cmb->clear();

  //Add the connections to the combo
  while(itr!=conns.end())
  {
    connections_cmb->addItem(itr->first, QVariant::fromValue<void *>(itr->second));
    itr++;
  }

  connect_tb->setEnabled(connections_cmb->count() > 0);
  enableSQLExecution(false);
}

void SQLToolWidget::hide(void)
{
  QWidget::hide();
  emit s_visibilityChanged(false);
}

void SQLToolWidget::connectToDatabase(void)
{
  try
  {
    Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

    import_helper.setConnection(*conn);
    DatabaseImportForm::listDatabases(import_helper, false, database_cmb);
    database_cmb->setEnabled(database_cmb->count() > 1);

    sql_cmd_conn=(*conn);
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void SQLToolWidget::listObjects(void)
{
  try
  {
    if(database_cmb->currentIndex() > 0)
    {
      task_prog_wgt->setWindowTitle(trUtf8("Retrieving objects from database..."));
      task_prog_wgt->show();

      import_helper.setCurrentDatabase(database_cmb->currentText());
      import_helper.setImportOptions(!hide_sys_objs_chk->isChecked(), !hide_ext_objs_chk->isChecked(), false, false, false);

      DatabaseImportForm::listObjects(import_helper, objects_trw, false, false);

      task_prog_wgt->close();
    }
    else
      objects_trw->clear();

    refresh_tb->setEnabled(database_cmb->currentIndex() > 0);
    filter_parent->setEnabled(refresh_tb->isEnabled());
    filter_edt->clear();
    filter_lbl->setEnabled(refresh_tb->isEnabled());
    filter_edt->setEnabled(refresh_tb->isEnabled());
    expand_all_tb->setEnabled(refresh_tb->isEnabled());
    collapse_all_tb->setEnabled(refresh_tb->isEnabled());
    hide_ext_objs_chk->setEnabled(refresh_tb->isEnabled());
    hide_sys_objs_chk->setEnabled(refresh_tb->isEnabled());

    enableSQLExecution(refresh_tb->isEnabled());
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void SQLToolWidget::updateCurrentItem(void)
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


    objects_trw->sortItems(0, Qt::AscendingOrder);
  }
}

void SQLToolWidget::enableCommandButtons(void)
{
  run_sql_tb->setEnabled(!sql_cmd_txt->toPlainText().isEmpty());
  clear_btn->setEnabled(run_sql_tb->isEnabled());
  save_tb->setEnabled(run_sql_tb->isEnabled());
}

void SQLToolWidget::fillResultsTable(ResultSet &res)
{
  try
  {
    int col=0, row=0, row_cnt=res.getTupleCount(), col_cnt=res.getColumnCount();
    QTableWidgetItem *item=nullptr;

    row_cnt_lbl->setText(QString::number(row_cnt));
    export_tb->setEnabled(row_cnt > 0);
    results_tbw->setRowCount(0);
    results_tbw->setColumnCount(col_cnt);

    //Configuring the grid columns with the names of retrived table columns
    for(col=0; col < col_cnt; col++)
      results_tbw->setHorizontalHeaderItem(col, new QTableWidgetItem(res.getColumnName(col)));

    if(res.accessTuple(ResultSet::FIRST_TUPLE))
    {
      results_tbw->setRowCount(row_cnt);

      do
      {
        //Fills the current row with the values of current tuple
        for(col=0; col < col_cnt; col++)
        {
          item=new QTableWidgetItem;

          if(res.isColumnBinaryFormat(col))
            item->setText(trUtf8("[binary data]"));
          else
            item->setText(res.getColumnValue(col));

          results_tbw->setItem(row, col, item);
        }

        //Configure the vertical header to show the current tuple id
        results_tbw->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row + 1)));
        row++;
      }
      while(res.accessTuple(ResultSet::NEXT_TUPLE));
    }

    results_tbw->resizeColumnsToContents();
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void SQLToolWidget::showError(Exception &e)
{
  QListWidgetItem *item=new QListWidgetItem(QIcon(":/icones/icones/msgbox_erro.png"), e.getErrorMessage());
  msgoutput_lst->clear();
  msgoutput_lst->addItem(item);
  msgoutput_lst->setVisible(true);
  results_parent->setVisible(false);
  export_tb->setEnabled(false);
}

bool SQLToolWidget::eventFilter(QObject *object, QEvent *event)
{
  if(object==objects_trw && event->type()==QEvent::KeyPress)
  {
    QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

    if(k_event->key()==Qt::Key_Delete || k_event->key()==Qt::Key_F5)
    {
      if(k_event->key()==Qt::Key_F5)
        updateCurrentItem();
      else
        dropObject(objects_trw->currentItem());

      return(true);
    }
    else
      return(false);
  }

  return(QWidget::eventFilter(object, event));
}

void SQLToolWidget::registerSQLCommand(const QString &cmd)
{
  if(!cmd.isEmpty())
  {
    QListWidgetItem *item=new QListWidgetItem;
    item->setData(Qt::UserRole, QVariant(cmd));

    if(cmd.size() > 500)
      item->setText(cmd.mid(0, 500) + "...");
    else
      item->setText(cmd);

    if(cmd_history_lst->count() > 100)
      cmd_history_lst->clear();

    cmd_history_lst->addItem(item);
    clear_history_btn->setEnabled(true);
  }
}

void SQLToolWidget::runSQLCommand(void)
{
  try
  {
    ResultSet res;
    QString cmd=sql_cmd_txt->textCursor().selectedText();

    if(cmd.isEmpty())
      cmd=sql_cmd_txt->toPlainText();

    sql_cmd_conn.executeDMLCommand(cmd, res);
    registerSQLCommand(cmd);

    results_parent->setVisible(!res.isEmpty());
    export_tb->setEnabled(!res.isEmpty());
    msgoutput_lst->setVisible(res.isEmpty());

    if(results_tbw->isVisible())
      fillResultsTable(res);
    else
    {
      QLabel *label=new QLabel(trUtf8("SQL command successfully executed. <em>Rows affected <strong>%1</strong></em>").arg(res.getTupleCount()));
      QListWidgetItem *item=new QListWidgetItem;

      item->setIcon(QIcon(":/icones/icones/msgbox_info.png"));
      msgoutput_lst->clear();
      msgoutput_lst->addItem(item);
      msgoutput_lst->setItemWidget(item, label);
    }
  }
  catch(Exception &e)
  {
    showError(e);
  }
}

void SQLToolWidget::saveCommands(void)
{
  sql_file_dlg.setWindowTitle(trUtf8("Save SQL commands"));
  sql_file_dlg.setAcceptMode(QFileDialog::AcceptSave);
  sql_file_dlg.exec();

  if(sql_file_dlg.result()==QDialog::Accepted)
  {
    QFile file;
    file.setFileName(sql_file_dlg.selectedFiles().at(0));

    if(!file.open(QFile::WriteOnly))
      throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)
                      .arg(sql_file_dlg.selectedFiles().at(0))
                      , ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    file.write(sql_cmd_txt->toPlainText().toUtf8());
    file.close();
  }
}

void SQLToolWidget::loadCommands(void)
{
  sql_file_dlg.setWindowTitle(trUtf8("Load SQL commands"));
  sql_file_dlg.setAcceptMode(QFileDialog::AcceptOpen);
  sql_file_dlg.exec();

  if(sql_file_dlg.result()==QDialog::Accepted)
  {
    QFile file;
    file.setFileName(sql_file_dlg.selectedFiles().at(0));

    if(!file.open(QFile::ReadOnly))
      throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)
                      .arg(sql_file_dlg.selectedFiles().at(0))
                      ,ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    sql_cmd_txt->clear();
    sql_cmd_txt->setPlainText(file.readAll());
    file.close();
  }
}

void SQLToolWidget::exportResults(void)
{
  csv_file_dlg.exec();

  if(csv_file_dlg.result()==QDialog::Accepted)
  {
    QFile file;
    file.setFileName(csv_file_dlg.selectedFiles().at(0));

    if(!file.open(QFile::WriteOnly))
      throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)
                      .arg(csv_file_dlg.selectedFiles().at(0))
                      , ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

    file.write(generateCSVBuffer(0, 0, results_tbw->rowCount(), results_tbw->columnCount()));
    file.close();
  }
}

QByteArray SQLToolWidget::generateCSVBuffer(int start_row, int start_col, int row_cnt, int col_cnt)
{
  QByteArray buf;

  //If the selection interval is valid
  if(start_row >=0 && start_col >=0 &&
     start_row + row_cnt <= results_tbw->rowCount() &&
     start_col + col_cnt <= results_tbw->columnCount())
  {
    int col=0, row=0,
        max_col=start_col + col_cnt,
        max_row=start_row + row_cnt;

    //Creating the header of csv
    for(col=start_col; col < max_col; col++)
    {
      buf.append(QString("\"%1\"").arg(results_tbw->horizontalHeaderItem(col)->text()));
      buf.append(';');
    }

    buf.append('\n');

    //Creating the content
    for(row=start_row; row < max_row; row++)
    {
      for(col=start_col; col < max_col; col++)
      {
        buf.append(QString("\"%1\"").arg(results_tbw->item(row, col)->text()));
        buf.append(';');
      }

      buf.append('\n');
    }
  }

  return(buf);
}

void SQLToolWidget::dropObject(QTreeWidgetItem *item)
{
  Messagebox msg_box;

  try
  {
    if(item && static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_ID, Qt::UserRole).toUInt()) > 0)
    {
      ObjectType obj_type=static_cast<ObjectType>(item->data(DatabaseImportForm::OBJECT_TYPE, Qt::UserRole).toUInt());

      msg_box.show(trUtf8("Confirmation"),
                   trUtf8("Do you really want to drop the object <strong>%1</strong> <em>(%2)</em>?")
                   .arg(item->text(0)).arg(BaseObject::getTypeName(obj_type)),
                   Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Accepted)
      {
        QTreeWidgetItem *parent=nullptr;
        attribs_map attribs;
        QStringList types;
        QString drop_cmd, obj_name=item->text(0);
        int idx=0, idx1=0;

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
          attribs[ParsersAttributes::TABLE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::TABLE];
        //For operators and functions there must exist the signature attribute
        else if(obj_type==OBJ_OPERATOR || obj_type==OBJ_FUNCTION)
          attribs[ParsersAttributes::SIGNATURE]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::NAME] + QString("(%1)").arg(types.join(','));
        else if(obj_type==OBJ_CAST)
        {
          attribs[ParsersAttributes::SOURCE_TYPE]=types[0];
          attribs[ParsersAttributes::DEST_TYPE]=types[1];
        }
        else
        {
          if(!attribs[ParsersAttributes::SCHEMA].isEmpty() &&
             attribs[ParsersAttributes::NAME].indexOf(attribs[ParsersAttributes::SCHEMA] + ".") < 0)
            attribs[ParsersAttributes::NAME]=attribs[ParsersAttributes::SCHEMA] + "." + attribs[ParsersAttributes::NAME];
        }

        //Generate the drop command
        SchemaParser::setIgnoreEmptyAttributes(true);
        SchemaParser::setIgnoreUnkownAttributes(true);
        drop_cmd=SchemaParser::getCodeDefinition(ParsersAttributes::DROP, attribs, SchemaParser::SQL_DEFINITION);
        drop_cmd.remove(QRegExp("^(--)"));

        //Executes the drop cmd
        sql_cmd_conn.executeDDLCommand(drop_cmd);

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

        if(item->parent())
          item->parent()->takeChild(item->parent()->indexOfChild(item));
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

void SQLToolWidget::showObjectData(QTreeWidgetItem *item)
{
  try
  {
    QString cmd;
    ResultSet res;

    cmd=QString("SELECT * FROM %1.%2;")
        .arg(BaseObject::formatName(item->data(DatabaseImportForm::OBJECT_SCHEMA, Qt::UserRole).toString()))
        .arg(BaseObject::formatName(item->text(0)));

    registerSQLCommand(sql_cmd_txt->toPlainText());
    sql_cmd_txt->setText(cmd);
    sql_cmd_conn.executeDMLCommand(cmd, res);

    results_parent->setVisible(true);
    msgoutput_lst->setVisible(false);

    fillResultsTable(res);
  }
  catch(Exception &e)
  {
    showError(e);
  }
}

void SQLToolWidget::clearAll(void)
{
  Messagebox msg_box;

  msg_box.show(trUtf8("Confirmation"),
               trUtf8("The SQL input field and the results grid will be cleared! Want to proceed?"),
               Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

  if(msg_box.result()==QDialog::Accepted)
  {
    sql_cmd_txt->setText("");
    msgoutput_lst->clear();
    msgoutput_lst->setVisible(true);
    results_parent->setVisible(false);
  }
}

void SQLToolWidget::copySelection(void)
{
  if(QApplication::mouseButtons()==Qt::RightButton)
  {
    if(copy_menu.exec(QCursor::pos()))
    {
      QList<QTableWidgetSelectionRange> sel_range=results_tbw->selectedRanges();

      if(!sel_range.isEmpty())
      {
        QTableWidgetSelectionRange selection=sel_range.at(0);

        //Generates the csv buffer and assigns it to application's clipboard
        QByteArray buf=generateCSVBuffer(selection.topRow(), selection.leftColumn(),
                                         selection.rowCount(), selection.columnCount());
        qApp->clipboard()->setText(buf);
      }
    }
  }
}

void SQLToolWidget::handleObject(QTreeWidgetItem *item, int)
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
      handle_menu.addAction(refresh_action);

      if(!handle_menu.actions().isEmpty())
      {
        QAction *exec_action=handle_menu.exec(QCursor::pos());

        if(exec_action==drop_action)
          dropObject(item);
        else if(exec_action==show_data_action)
          showObjectData(item);
      }
    }
    else
    {
      handle_menu.addAction(refresh_action);
      handle_menu.exec(QCursor::pos());
    }
  }
}

void SQLToolWidget::enableSQLExecution(bool enable)
{
  try
  {
    sql_cmd_txt->setEnabled(enable);
    load_tb->setEnabled(enable);
    history_tb->setEnabled(enable);

    if(history_tb->isChecked() && !enable)
      history_tb->setChecked(false);

    if(enable)
      sql_cmd_conn.switchToDatabase(database_cmb->currentText());
    else if(!enable && sql_cmd_conn.isStablished())
      sql_cmd_conn.close();
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}
