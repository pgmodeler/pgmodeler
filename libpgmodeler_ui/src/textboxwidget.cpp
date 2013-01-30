#include "textboxwidget.h"

TextboxWidget::TextboxWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TEXTBOX)
{
	Ui_TextboxWidget::setupUi(this);
	configureFormLayout(caixatexto_grid, OBJ_TEXTBOX);
	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(color_select_tb, SIGNAL(clicked(void)), this, SLOT(selectTextColor(void)));
	parent_form->definirBotoes(MessageBox::OK_CANCEL_BUTTONS);
	parent_form->setMinimumSize(500,250);
}

void TextboxWidget::hideEvent(QHideEvent *event)
{
	QPalette palette;

	text_txt->clear();
	bold_chk->setChecked(false);
	italic_chk->setChecked(false);
	underline_chk->setChecked(false);

	palette.setColor(QPalette::Button, QColor(0,0,0));
	color_select_tb->setPalette(palette);

	BaseObjectWidget::hideEvent(event);
}

void TextboxWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Textbox *txtbox, float obj_px, float obj_py)
{
	if(txtbox)
	{
		QPalette palette;
		palette.setColor(QPalette::Button, txtbox->getTextColor());
		color_select_tb->setPalette(palette);

		text_txt->setPlainText(QString::fromUtf8(txtbox->getComment()));
		bold_chk->setChecked(txtbox->getTextAttribute(Textbox::BOLD_TXT));
		italic_chk->setChecked(txtbox->getTextAttribute(Textbox::ITALIC_TXT));
		underline_chk->setChecked(txtbox->getTextAttribute(Textbox::UNDERLINE_TXT));
	}

	BaseObjectWidget::setAttributes(model, op_list, txtbox, NULL, obj_px, obj_py);
}

void TextboxWidget::selectTextColor(void)
{
	QColorDialog color_dlg;
	QPalette palette;

	color_dlg.setWindowTitle(trUtf8("Select text color"));
	color_dlg.exec();

	if(color_dlg.result()==QDialog::Accepted)
	{
		palette.setColor(QPalette::Button, color_dlg.selectedColor());
		color_select_tb->setPalette(palette);
	}
}

void TextboxWidget::applyConfiguration(void)
{
	try
	{
		Textbox *txtbox=NULL;

		startConfiguration<Textbox>();

		txtbox=dynamic_cast<Textbox *>(this->object);
		txtbox->setComment(text_txt->toPlainText());
		txtbox->setTextAttribute(Textbox::ITALIC_TXT, italic_chk->isChecked());
		txtbox->setTextAttribute(Textbox::BOLD_TXT, bold_chk->isChecked());
		txtbox->setTextAttribute(Textbox::UNDERLINE_TXT, underline_chk->isChecked());
		txtbox->setTextColor(color_select_tb->palette().color(QPalette::Button));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

