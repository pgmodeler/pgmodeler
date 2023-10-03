#include "extensionwidget.h"
#include "guiutilsns.h"

ExtensionWidget::ExtensionWidget(QWidget * parent) : BaseObjectWidget(parent, ObjectType::Extension)
{
	Ui_ExtensionWidget::setupUi(this);

	types_tab = new ObjectsTableWidget(ObjectsTableWidget::AddButton |
																		 ObjectsTableWidget::RemoveButton |
																		 ObjectsTableWidget::RemoveAllButton, true, this);
	types_tab->setCellsEditable(true);
	types_tab->setColumnCount(1);
	types_tab->setHeaderLabel(tr("Name"), 0);

	QVBoxLayout *vbox = new QVBoxLayout(types_gb);
	vbox->addWidget(types_tab);
	vbox->setContentsMargins(GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin,
													 GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin);

	configureFormLayout(extension_grid, ObjectType::Extension);
	configureTabOrder({ cur_ver_edt, old_ver_edt, handles_type_chk, types_tab });

	setMinimumSize(500, 500);
}

void ExtensionWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Extension *ext)
{
	BaseObjectWidget::setAttributes(model, op_list, ext, schema);

	if(ext)
	{
		cur_ver_edt->setText(ext->getVersion(Extension::CurVersion));
		old_ver_edt->setText(ext->getVersion(Extension::OldVersion));

		handles_type_chk->setEnabled(false);
		handles_type_chk->setChecked(ext->handlesType());
	}
}

void ExtensionWidget::applyConfiguration()
{
	try
	{
		Extension *extension = nullptr;

		startConfiguration<Extension>();
		extension = dynamic_cast<Extension *>(this->object);
		BaseObjectWidget::applyConfiguration();
		extension->setHandlesType(handles_type_chk->isChecked());
		extension->setVersion(Extension::CurVersion, cur_ver_edt->text());
		extension->setVersion(Extension::OldVersion, old_ver_edt->text());

//		std::vector<Type *> curr_types = extension->getTypes();
		QStringList type_names = types_tab->getCellTexts(0, Qt::Vertical);
		extension->setTypes(type_names);

		/* for(auto &type : curr_types)
			model->removeType(type);

		for(auto &type : extension->getTypes())
			model->addType(type); */

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
