#include "esquemawidget.h"

EsquemaWidget::EsquemaWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_SCHEMA)
{
	Ui_SchemaWidget::setupUi(this);
	configureFormLayout(NULL, OBJ_SCHEMA);

	baseobject_grid->addWidget(fill_color_lbl, baseobject_grid->count(), 0, 1, 1);
	baseobject_grid->addWidget(fill_color_tb, baseobject_grid->count()-1, 1, 1, 1);
	baseobject_grid->addWidget(show_rect_chk, baseobject_grid->count()-2, 2, 1, 1);

	connect(parent_form->aplicar_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	connect(fill_color_tb, SIGNAL(clicked(void)), this, SLOT(selectFillColor(void)));

	parent_form->setMinimumSize(500, 250);
	parent_form->setMaximumSize(16777215, 250);
}

void EsquemaWidget::setAttributes(DatabaseModel *modelo, OperationList *lista_op, Schema *esquema)
{
	bool enable=false;
	QPalette palette;

	//Define os atributos do formulários e da janela pai
	BaseObjectWidget::setAttributes(modelo, lista_op, esquema);

	enable=!(this->object && this->object->getName()=="public");
	edt_perms_tb->setEnabled(enable);
	name_edt->setEnabled(enable);
	comment_edt->setEnabled(enable);
	owner_sel->setEnabled(enable);

	if(esquema)
		palette.setColor(QPalette::Button, esquema->getFillColor());
	else
		palette.setColor(QPalette::Button, QColor(225,225,225));

	fill_color_tb->setPalette(palette);
	show_rect_chk->setChecked(esquema && esquema->isRectVisible());
}

void EsquemaWidget::selectFillColor(void)
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

void EsquemaWidget::applyConfiguration(void)
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
		/* Cancela a configuração o objeto removendo a ultima operação adicionada
		 referente ao objeto editado/criado e desaloca o objeto
		 caso o mesmo seja novo */
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

