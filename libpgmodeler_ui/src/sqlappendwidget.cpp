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

		sqlcode_cp=new CodeCompletionWidget(sqlcode_hl);

		parent_form->setWindowTitle(trUtf8("Append SQL code"));
		parent_form->setButtonConfiguration(Messagebox::OK_BUTTON);
		parent_form->setMinimumSize(600, 400);

		font=name_edt->font();
		font.setItalic(true);

		name_edt->setReadOnly(true);
		comment_edt->setFont(font);
		comment_edt->setReadOnly(true);
		comment_lbl->setText(trUtf8("Type:"));

		connect(parent_form->apply_ok_btn, SIGNAL(clicked(bool)), parent_form, SLOT(close(void)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
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

			sqlcode_txt->setFocus();
			sqlcode_cp->setModel(model);
			end_of_model_chk->setVisible(object->getObjectType()==OBJ_DATABASE);
			comment_edt->setText(object->getTypeName());
			protected_obj_frm->setVisible(false);
			parent_form->apply_ok_btn->setEnabled(true);
			obj_icon_lbl->setPixmap(QPixmap(QString(":/icones/icones/") +
																			BaseObject::getSchemaName(object->getObjectType()) + QString(".png")));
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}
