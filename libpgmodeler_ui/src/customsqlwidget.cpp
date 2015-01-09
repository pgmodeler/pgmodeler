#include "customsqlwidget.h"

CustomSQLWidget::CustomSQLWidget(QWidget *parent) : BaseObjectWidget(parent)
{
	try
	{
		QFont font;

		Ui_CustomSQLWidget::setupUi(this);
		configureFormLayout(sqlappend_grid, BASE_OBJECT);

    append_sql_hl=new SyntaxHighlighter(append_sql_txt, false);
    append_sql_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);
    append_sql_cp=new CodeCompletionWidget(append_sql_txt);

    prepend_sql_hl=new SyntaxHighlighter(prepend_sql_txt, false);
    prepend_sql_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);
    prepend_sql_cp=new CodeCompletionWidget(prepend_sql_txt);

    parent_form->setWindowTitle(trUtf8("Append / Prepend SQL code"));
		parent_form->setButtonConfiguration(Messagebox::OK_CANCEL_BUTTONS);
    parent_form->setMinimumSize(640, 480);

		font=name_edt->font();
		font.setItalic(true);

		name_edt->setReadOnly(true);
		comment_edt->setFont(font);
		comment_edt->setReadOnly(true);
		comment_lbl->setText(trUtf8("Type:"));

		action_gen_insert=new QAction(trUtf8("Generic INSERT"), this);
		action_gen_insert->setObjectName("action_gen_insert");
		action_inc_serials=new QAction(trUtf8("Include serial columns"), this);
		action_inc_serials->setObjectName("action_inc_serials");
		action_exc_serials=new QAction(trUtf8("Exclude serial columns"), this);
		action_exc_serials->setObjectName("action_exc_serials");
		action_gen_select=new QAction(trUtf8("Generic SELECT"), this);
		action_gen_select->setObjectName("action_gen_select");
		action_tab_select=new QAction(trUtf8("Table SELECT"), this);
		action_tab_select->setObjectName("action_tab_select");
		action_gen_update=new QAction(trUtf8("Generic UPDATE"), this);
		action_gen_update->setObjectName("action_gen_update");
		action_tab_update=new QAction(trUtf8("Table UPDATE"), this);
		action_tab_update->setObjectName("action_tab_update");
		action_gen_delete=new QAction(trUtf8("Generic DELETE"), this);
		action_gen_delete->setObjectName("action_gen_delete");
		action_tab_delete=new QAction(trUtf8("Table DELETE"), this);
		action_tab_delete->setObjectName("action_tab_delete");

		insert_menu.addAction(action_inc_serials);
		insert_menu.addAction(action_exc_serials);
		insert_menu.addAction(action_gen_insert);
		select_menu.addAction(action_tab_select);
		select_menu.addAction(action_gen_select);
		update_menu.addAction(action_tab_update);
		update_menu.addAction(action_gen_update);
		delete_menu.addAction(action_tab_delete);
		delete_menu.addAction(action_gen_delete);

		connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(parent_form->cancel_btn, SIGNAL(clicked(bool)), parent_form, SLOT(reject(void)));

		connect(clear_tb, SIGNAL(clicked(bool)), this, SLOT(clearCode(void)));
		connect(insert_tb, SIGNAL(clicked(bool)), this, SLOT(addCommand(void)));
		connect(select_tb, SIGNAL(clicked(bool)), this, SLOT(addCommand(void)));
		connect(update_tb, SIGNAL(clicked(bool)), this, SLOT(addCommand(void)));
		connect(delete_tb, SIGNAL(clicked(bool)), this, SLOT(addCommand(void)));
		connect(action_gen_insert, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_inc_serials, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_exc_serials, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_gen_select, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_tab_select, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_gen_update, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_tab_update, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_gen_delete, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
		connect(action_tab_delete, SIGNAL(triggered(void)), this, SLOT(addCommand(void)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void CustomSQLWidget::configureMenus(void)
{
	ObjectType obj_type=this->object->getObjectType();
	QToolButton *btns[]={ insert_tb, select_tb , delete_tb, update_tb };
	int count=sizeof(btns)/sizeof(QToolButton *);

	for(int i=0; i < count; i++)
		btns[i]->setMenu(nullptr);

	if(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW)
	{
		if(obj_type==OBJ_TABLE)
		{
			insert_tb->setMenu(&insert_menu);
			delete_tb->setMenu(&delete_menu);
			update_tb->setMenu(&update_menu);
		}

		select_tb->setMenu(&select_menu);
	}
}

void CustomSQLWidget::setAttributes(DatabaseModel *model, BaseObject *object)
{
	if(!object)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(!BaseObject::acceptsCustomSQL(object->getObjectType()))
		throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		BaseObjectWidget::setAttributes(model, object, nullptr);

		if(object->getObjectType()==OBJ_DATABASE)
			end_of_model_chk->setChecked(dynamic_cast<DatabaseModel *>(object)->isAppendAtEOD());

		append_sql_txt->setFocus();
		append_sql_txt->setPlainText(object->getAppendedSQL());
		append_sql_cp->configureCompletion(model, append_sql_hl);
		append_sql_txt->moveCursor(QTextCursor::End);

		append_sql_txt->setFocus();
		prepend_sql_txt->setPlainText(object->getPrependedSQL());
		prepend_sql_cp->configureCompletion(model, prepend_sql_hl);
		prepend_sql_txt->moveCursor(QTextCursor::End);

		end_of_model_chk->setVisible(object->getObjectType()==OBJ_DATABASE);
		begin_of_model_chk->setVisible(object->getObjectType()==OBJ_DATABASE);

		comment_edt->setText(object->getTypeName());
		protected_obj_frm->setVisible(false);
		obj_id_lbl->setVisible(false);
		parent_form->apply_ok_btn->setEnabled(true);

		obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
																		BaseObject::getSchemaName(object->getObjectType()) + QString(".png")));

		configureMenus();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}

}

void CustomSQLWidget::applyConfiguration(void)
{
	if(this->object->getObjectType()==OBJ_DATABASE)
  {
		dynamic_cast<DatabaseModel *>(this->object)->setAppendAtEOD(end_of_model_chk->isChecked());
    dynamic_cast<DatabaseModel *>(this->object)->setPrependAtBOD(begin_of_model_chk->isChecked());
  }

  this->object->setAppendedSQL(append_sql_txt->toPlainText());
  this->object->setPrependedSQL(prepend_sql_txt->toPlainText());
  this->sqlcodes_twg->setCurrentIndex(0);
	this->accept();
}

void CustomSQLWidget::addCommand(void)
{
	Table *table=dynamic_cast<Table *>(this->object);
	BaseTable *base_table=dynamic_cast<BaseTable *>(this->object);
	QString cmd,
			ins_cmd=QString("INSERT INTO %1 (%2) VALUES (%3);"),
			sel_cmd=QString("SELECT * FROM %1;"),
			del_cmd=QString("DELETE * FROM %1;"),
			upd_cmd=QString("UPDATE %1 SET ;");
  QTextEdit *sqlcode_txt=(sqlcodes_twg->currentIndex()==0 ? append_sql_txt : prepend_sql_txt);

	if(sender()->objectName().contains("insert") ||
		 sender()->objectName().contains("serial"))
	{
		if(!table || sender()==action_gen_insert)
			cmd=ins_cmd.arg("table").arg("cols").arg("values");
		else if(table)
		{
			bool inc_serials=(sender()==action_inc_serials);
			QString cols, vals;
			unsigned val_id=1;

			for(unsigned i=0; i < table->getColumnCount(); i++)
			{
				if(inc_serials ||
					 (!inc_serials && !table->getColumn(i)->getType().isSerialType()))
				{
					cols+=table->getColumn(i)->getName(true) + ",";
					vals+=QString("val%1,").arg(val_id++);
				}
			}

			cols.remove(cols.size()-1, 1);
			vals.remove(vals.size()-1, 1);
			cmd=ins_cmd.arg(table->getName(true)).arg(cols).arg(vals);
		}
	}
	else if(sender()->objectName().contains("select"))
	{
		if(!base_table || sender()==action_gen_select)
			cmd=sel_cmd.arg("object");
		else if(base_table)
			cmd=sel_cmd.arg(base_table->getName(true));
	}
	else if(sender()->objectName().contains("delete"))
	{
		if(!table || sender()==action_gen_delete)
			cmd=del_cmd.arg("object");
		else if(table)
			cmd=del_cmd.arg(table->getName(true));
	}
	else
	{
		if(!table || sender()==action_gen_update)
			cmd=upd_cmd.arg("object");
		else if(table)
			cmd=upd_cmd.arg(table->getName(true));
	}

	if(!sqlcode_txt->toPlainText().isEmpty())
		sqlcode_txt->insertPlainText("\n");

	sqlcode_txt->insertPlainText(cmd);
}

void CustomSQLWidget::clearCode(void)
{
  QTextEdit *sqlcode_txt=(sqlcodes_twg->currentIndex()==0 ? append_sql_txt : prepend_sql_txt);
	QTextCursor tc=sqlcode_txt->textCursor();
	tc.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
	tc.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
	tc.removeSelectedText();
}
