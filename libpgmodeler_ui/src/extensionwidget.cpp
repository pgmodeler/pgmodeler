#include "extensionwidget.h"

ExtensionWidget::ExtensionWidget(QWidget * parent) : BaseObjectWidget(parent, OBJ_EXTENSION)
{
	Ui_ExtensionWidget::setupUi(this);
	configureFormLayout(extension_grid, OBJ_EXTENSION);

	extension_grid->addItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding), extension_grid->count()+1, 0, 1, 0);
	configureTabOrder({ cur_ver_edt, old_ver_edt, handles_type_chk });

	setMinimumSize(500, 180);
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
