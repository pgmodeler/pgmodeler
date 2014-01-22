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
  results_parent->setVisible(false);

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

  connect(hide_tb, SIGNAL(clicked(void)), this, SLOT(hide(void)));
  connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearAll(void)));
  connect(connect_tb, SIGNAL(clicked(void)), this, SLOT(connectToDatabase(void)));
  connect(database_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(listObjects(void)));
  connect(refresh_tb, SIGNAL(clicked(void)), this, SLOT(listObjects(void)));
  connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterObjects(void)));
  connect(expand_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(expandAll(void)));
  connect(collapse_all_tb, SIGNAL(clicked(bool)), objects_trw, SLOT(collapseAll(void)));
  connect(sql_cmd_txt, SIGNAL(textChanged(void)), this, SLOT(enableCommandButtons(void)));
  connect(run_sql_tb, SIGNAL(clicked(void)), this, SLOT(runSQLCommand(void)));
  connect(save_tb, SIGNAL(clicked(void)), this, SLOT(saveCommands(void)));
  connect(load_tb, SIGNAL(clicked(void)), this, SLOT(loadCommands(void)));
  connect(export_tb, SIGNAL(clicked(void)), this, SLOT(exportResults(void)));
  connect(results_trw, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(copySelection(void)));
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
    import_helper.setImportOptions(true, true, false, false, false);
    DatabaseImportForm::listDatabases(import_helper, database_cmb);
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
      DatabaseImportForm::listObjects(import_helper, objects_trw, false);
      task_prog_wgt->close();
    }
    else
      objects_trw->clear();

    refresh_tb->setEnabled(database_cmb->currentIndex() > 0);
    filter_parent->setEnabled(refresh_tb->isEnabled());
    filter_edt->clear();
    enableSQLExecution(refresh_tb->isEnabled());
  }
  catch(Exception &e)
  {
    throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void SQLToolWidget::filterObjects(void)
{
  DatabaseImportForm::filterObjects(objects_trw, filter_edt->text(), false);
}

void SQLToolWidget::enableCommandButtons(void)
{
  run_sql_tb->setEnabled(!sql_cmd_txt->toPlainText().isEmpty());
  clear_btn->setEnabled(run_sql_tb->isEnabled());
  save_tb->setEnabled(run_sql_tb->isEnabled());
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

    results_parent->setVisible(!res.isEmpty());
    export_tb->setEnabled(!res.isEmpty());
    msgoutput_lst->setVisible(res.isEmpty());

    if(results_trw->isVisible())
    {
      int col=0, row=0, row_cnt=res.getTupleCount(), col_cnt=res.getColumnCount();

      row_cnt_lbl->setText(QString::number(row_cnt));
      export_tb->setEnabled(row_cnt > 0);

      while(results_trw->rowCount() > 1)
        results_trw->setRowCount(0);

      results_trw->setColumnCount(col_cnt);

      for(col=0; col < col_cnt; col++)
        results_trw->setHorizontalHeaderItem(col, new QTableWidgetItem(res.getColumnName(col)));

      if(res.accessTuple(ResultSet::FIRST_TUPLE))
      {
        results_trw->setRowCount(row_cnt);

        do
        {
          for(col=0; col < col_cnt; col++)
            results_trw->setItem(row, col, new QTableWidgetItem(res.getColumnValue(col)));

          results_trw->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row + 1)));
          row++;
        }
        while(res.accessTuple(ResultSet::NEXT_TUPLE));
      }
    }
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
    QListWidgetItem *item=new QListWidgetItem(QIcon(":/icones/icones/msgbox_erro.png"), e.getErrorMessage());
    msgoutput_lst->clear();
    msgoutput_lst->addItem(item);
    msgoutput_lst->setVisible(true);
    results_parent->setVisible(false);
    export_tb->setEnabled(false);
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
    //QByteArray buf;
    //int col=0, row=0, col_cnt=0, row_cnt=0;

    file.setFileName(csv_file_dlg.selectedFiles().at(0));

    if(!file.open(QFile::WriteOnly))
      throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED)
                      .arg(csv_file_dlg.selectedFiles().at(0))
                      , ERR_FILE_DIR_NOT_ACCESSED ,__PRETTY_FUNCTION__,__FILE__,__LINE__);


    /*col_cnt=results_trw->columnCount();
    row_cnt=results_trw->rowCount();

    for(col=0; col < col_cnt; col++)
    {
      buf.append(QString("\"%1\"").arg(results_trw->horizontalHeaderItem(col)->text()));
      buf.append(';');
    }

    buf.append('\n');

    for(row=0; row < row_cnt; row++)
    {
      for(col=0; col < col_cnt; col++)
      {
        buf.append(QString("\"%1\"").arg(results_trw->item(row, col)->text()));
        buf.append(';');
      }

      buf.append('\n');
    }*/

    file.write(generateCSVBuffer(0, 0, results_trw->rowCount(), results_trw->columnCount()));
    file.close();
  }
}

QByteArray SQLToolWidget::generateCSVBuffer(int start_row, int start_col, int row_cnt, int col_cnt)
{
  QByteArray buf;

  if(start_row >=0 && start_col >=0 &&
     start_row + row_cnt <= results_trw->rowCount() &&
     start_col + col_cnt <= results_trw->columnCount())
  {
    int col=0, row=0,
        max_col=start_col + col_cnt,
        max_row=start_row + row_cnt;

    for(col=start_col; col < max_col; col++)
    {
      buf.append(QString("\"%1\"").arg(results_trw->horizontalHeaderItem(col)->text()));
      buf.append(';');
    }

    buf.append('\n');

    for(row=start_row; row < max_row; row++)
    {
      for(col=start_col; col < max_col; col++)
      {
        buf.append(QString("\"%1\"").arg(results_trw->item(row, col)->text()));
        buf.append(';');
      }

      buf.append('\n');
    }
  }

  return(buf);
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
      QList<QTableWidgetSelectionRange> sel_range=results_trw->selectedRanges();

      if(!sel_range.isEmpty())
      {
        QTableWidgetSelectionRange selection=sel_range.at(0);
        QByteArray buf=generateCSVBuffer(selection.topRow(), selection.leftColumn(),
                                         selection.rowCount(), selection.columnCount());
        qApp->clipboard()->setText(buf);
      }
    }
  }
}

void SQLToolWidget::enableSQLExecution(bool enable)
{
  try
  {
    sql_cmd_txt->setEnabled(enable);
    load_tb->setEnabled(enable);

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
