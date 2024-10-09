#include "extensionwidget.h"
#include "guiutilsns.h"

ExtensionWidget::ExtensionWidget(QWidget * parent) : BaseObjectWidget(parent, ObjectType::Extension)
{
	Ui_ExtensionWidget::setupUi(this);

	types_tab = new CustomTableWidget(CustomTableWidget::AddButton |
																		 CustomTableWidget::RemoveButton |
																		 CustomTableWidget::RemoveAllButton, true, this);
	types_tab->setCellsEditable(true);
	types_tab->setColumnCount(1);
	types_tab->setHeaderLabel(tr("Name"), 0);

	QVBoxLayout *vbox = new QVBoxLayout(types_gb);
	vbox->addWidget(types_tab);
	vbox->setContentsMargins(GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin,
													 GuiUtilsNs::LtMargin, GuiUtilsNs::LtMargin);

	configureFormLayout(extension_grid, ObjectType::Extension);
	configureTabOrder({ cur_ver_edt, old_ver_edt, types_tab });

	setMinimumSize(500, 500);
}

void ExtensionWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Extension *ext)
{
	BaseObjectWidget::setAttributes(model, op_list, ext, schema);

	if(ext)
	{
		cur_ver_edt->setText(ext->getVersion(Extension::CurVersion));
		old_ver_edt->setText(ext->getVersion(Extension::OldVersion));

		for(auto &tp_name : ext->getTypeNames())
		{
			types_tab->addRow();
			types_tab->setCellText(tp_name, types_tab->getRowCount() - 1, 0);
		}

		types_tab->clearSelection();
	}
}

void ExtensionWidget::applyConfiguration()
{
	try
	{
		startConfiguration<Extension>();

		bool update_types = !new_object;
		Extension *extension = dynamic_cast<Extension *>(this->object);

		BaseObjectWidget::applyConfiguration();

		extension->setVersion(Extension::CurVersion, cur_ver_edt->text());
		extension->setVersion(Extension::OldVersion, old_ver_edt->text());

		QStringList type_names = types_tab->getCellTexts(0, Qt::Vertical);
		extension->setTypeNames(type_names);
		finishConfiguration();

		if(update_types && !model->updateExtensionTypes(extension))
			Messagebox::alert(tr("Some removed data types were restored because they are still being referenced in the model! Please, undo the link between those types and the objects in the database model before trying to remove them."));
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
