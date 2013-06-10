#include "extensionwidget.h"

ExtensionWidget::ExtensionWidget(QWidget * parent) : BaseObjectWidget(parent, OBJ_EXTENSION)
{
	Ui_ExtensionWidget::setupUi(this);
	configureFormLayout(extension_grid, OBJ_EXTENSION);

	connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
	parent_form->setMinimumSize(480, 250);
	parent_form->setMaximumHeight(250);
}

void ExtensionWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Extension *ext)
{
	BaseObjectWidget::setAttributes(model, op_list, ext, schema);

	if(ext)
	{
		cur_ver_edt->setText(ext->getVersion(Extension::CUR_VERSION));
		old_ver_edt->setText(ext->getVersion(Extension::OLD_VERSION));

		handles_type_chk->setEnabled(false);
		handles_type_chk->setChecked(ext->handlesType());
	}
}

void ExtensionWidget::hideEvent(QHideEvent *event)
{
 handles_type_chk->setEnabled(true);
 handles_type_chk->setChecked(false);
 cur_ver_edt->clear();
 old_ver_edt->clear();
 BaseObjectWidget::hideEvent(event);
}

void ExtensionWidget::applyConfiguration(void)
{
	try
	{
		Extension *extension=nullptr;

		startConfiguration<Extension>();
		extension=dynamic_cast<Extension *>(this->object);
		BaseObjectWidget::applyConfiguration();
		extension->setHandlesType(handles_type_chk->isChecked());
		extension->setVersion(Extension::CUR_VERSION, cur_ver_edt->text());
		extension->setVersion(Extension::OLD_VERSION, old_ver_edt->text());

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
