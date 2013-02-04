#include "schemawidget.h"

SchemaWidget::SchemaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_SCHEMA)
{
	Ui_SchemaWidget::setupUi(this);
	configureFormLayout(NULL, OBJ_SCHEMA);

	baseobject_grid->addWidget(fill_color_lbl, baseobject_grid->count(), 0, 1, 1);
	baseobject_grid->addWidget(fill_color_tb, baseobject_grid->count()-1, 1, 1, 1);
	baseobject_grid->addWidget(show_rect_chk, baseobject_grid->count()-2, 2, 1, 1);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(fill_color_tb, SIGNAL(clicked(void)), this, SLOT(selectFillColor(void)));

	parent_form->setMinimumSize(500, 250);
	parent_form->setMaximumHeight(250);
}

void SchemaWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema)
{
	bool enable=false;
	QPalette palette;

	BaseObjectWidget::setAttributes(model, op_list, schema);

	enable=!(this->object && this->object->getName()=="public");
	edt_perms_tb->setEnabled(enable);
	name_edt->setEnabled(enable);
	comment_edt->setEnabled(enable);
	owner_sel->setEnabled(enable);

	if(schema)
		palette.setColor(QPalette::Button, schema->getFillColor());
	else
		palette.setColor(QPalette::Button, QColor(225,225,225));

	fill_color_tb->setPalette(palette);
	show_rect_chk->setChecked(schema && schema->isRectVisible());
}

void SchemaWidget::selectFillColor(void)
{
	QColorDialog color_dlg;
	QPalette palette;

	color_dlg.setWindowTitle(trUtf8("Select fill color"));
	color_dlg.exec();

	if(color_dlg.result()==QDialog::Accepted)
	{
		palette.setColor(QPalette::Button, color_dlg.selectedColor());
		fill_color_tb->setPalette(palette);
	}
}

void SchemaWidget::applyConfiguration(void)
{
	try
	{
		Schema *schema=NULL;

		startConfiguration<Schema>();
		schema=dynamic_cast<Schema *>(this->object);
		BaseObjectWidget::applyConfiguration();

		schema->setRectVisible(show_rect_chk->isChecked());
		schema->setFillColor(fill_color_tb->palette().color(QPalette::Button));
		model->validateSchemaRenaming(dynamic_cast<Schema *>(this->object), this->prev_name);

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

