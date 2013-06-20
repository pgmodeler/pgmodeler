#include "sqlappendwidget.h"

SQLAppendWidget::SQLAppendWidget(QWidget *parent) : BaseObjectWidget(parent)
{
	try
	{
		QFont font;

		Ui_SQLAppendWidget::setupUi(this);
		configureFormLayout(sqlappend_grid, BASE_OBJECT);

		sqlcode_hl=new SyntaxHighlighter(sqlcode_txt, true);
		sqlcode_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																	GlobalAttributes::DIR_SEPARATOR +
																	GlobalAttributes::SQL_HIGHLIGHT_CONF +
																	GlobalAttributes::CONFIGURATION_EXT);

		sqlcode_cp=new CodeCompletionWidget(sqlcode_txt);

		parent_form->setWindowTitle(trUtf8("Append SQL code"));
		parent_form->setButtonConfiguration(Messagebox::OK_BUTTON);
		parent_form->setMinimumSize(800, 600);

		font=name_edt->font();
		font.setItalic(true);

		name_edt->setReadOnly(true);
		comment_edt->setFont(font);
		comment_edt->setReadOnly(true);
		comment_lbl->setText(trUtf8("Type:"));

		action_basic_cmd=new QAction(trUtf8("Basic command"), this);
		action_inc_serials=new QAction(trUtf8("Include serials"), this);
		action_exc_serials=new QAction(trUtf8("Exclude serials"), this);
		insert_menu.addAction(action_basic_cmd);
		insert_menu.addAction(action_inc_serials);
		insert_menu.addAction(action_exc_serials);

		connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(insert_tb, SIGNAL(clicked(bool)), this, SLOT(addInsertCommand(void)));
		connect(action_basic_cmd, SIGNAL(triggered(void)), this, SLOT(addInsertCommand(void)));
		connect(action_inc_serials, SIGNAL(triggered(void)), this, SLOT(addInsertCommand(void)));
		connect(action_exc_serials, SIGNAL(triggered(void)), this, SLOT(addInsertCommand(void)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void SQLAppendWidget::configureMenus(void)
{
	ObjectType obj_type=this->object->getObjectType();

	if(obj_type==OBJ_TABLE)
		insert_tb->setMenu(&insert_menu);
	else
		insert_tb->setMenu(nullptr);
}

void SQLAppendWidget::setAttributes(DatabaseModel *model, BaseObject *object)
{
	if(object)
	{
		if(TableObject::isTableObject(object->getObjectType()))
			throw Exception(ERR_OPR_OBJ_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		else if(!object)
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		try
		{
			BaseObjectWidget::setAttributes(model, object, nullptr);

			if(object->getObjectType()==OBJ_DATABASE)
				end_of_model_chk->setChecked(dynamic_cast<DatabaseModel *>(object)->isAppendAtEOD());

			sqlcode_txt->setFocus();
			sqlcode_txt->setPlainText(object->getAppendedSQL());
			sqlcode_cp->configureCompletion(model, sqlcode_hl);

			sqlcode_txt->moveCursor(QTextCursor::End);

			end_of_model_chk->setVisible(object->getObjectType()==OBJ_DATABASE);
			comment_edt->setText(object->getTypeName());
			protected_obj_frm->setVisible(false);
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
}

void SQLAppendWidget::applyConfiguration(void)
{
	if(this->object->getObjectType()==OBJ_DATABASE)
		dynamic_cast<DatabaseModel *>(this->object)->setAppendAtEOD(end_of_model_chk->isChecked());

	this->object->setAppendedSQL(sqlcode_txt->toPlainText());
	parent_form->close();
}

void SQLAppendWidget::addInsertCommand(void)
{
	Table *table=dynamic_cast<Table *>(this->object);

	if(!table || sender()==action_basic_cmd)
		sqlcode_txt->insertPlainText("\nINSERT INTO object VALUES (values);");
	else if(table)
	{
		bool inc_serials=(sender()==action_inc_serials);
		QString cols, vals;
		unsigned val_id=1;

		for(unsigned i=0; i < table->getColumnCount(); i++)
		{
			if(inc_serials ||
				 (!inc_serials && table->getColumn(i)->getType()!="serial" &&
					table->getColumn(i)->getType()!="smallserial" &&
					table->getColumn(i)->getType()!="bigserial"))
			{
				cols+=table->getColumn(i)->getName(true) + ",";
				vals+=QString("val%1,").arg(val_id++);
			}
		}

		cols.remove(cols.size()-1, 1);
		vals.remove(vals.size()-1, 1);

		sqlcode_txt->insertPlainText(QString("\nINSERT INTO %1 (%2) VALUES (%3);")
																 .arg(table->getName(true)).arg(cols).arg(vals));
	}
}
