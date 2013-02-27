#include "typewidget.h"
#include "parameterwidget.h"

extern ParameterWidget *parameter_wgt;

TypeWidget::TypeWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TYPE)
{
	try
	{
		QGridLayout *grid=NULL;
		QFrame *frame=NULL;
		QSpacerItem *spacer=NULL;
		map<QString, vector<QWidget *> > field_map;
		QStringList list;
		unsigned i;

		Ui_TypeWidget::setupUi(this);
		configureFormLayout(type_grid, OBJ_TYPE);

		like_type=new PgSQLTypeWidget(this, trUtf8("Like Type"));
		element_type=new PgSQLTypeWidget(this, trUtf8("Element Type"));

		grid=dynamic_cast<QGridLayout *>(base_attribs_twg->widget(0)->layout());
		spacer=new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
		grid->addWidget(like_type,6,0,1,0);
		grid->addWidget(element_type,7,0,1,0);
		grid->addItem(spacer,8,0);

		grid=dynamic_cast<QGridLayout *>(base_attribs_twg->widget(1)->layout());
		for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
		{
			functions_sel[i]=NULL;
			functions_sel[i]=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
			grid->addWidget(functions_sel[i],i,1,1,1);
		}

		enumerations_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																					ObjectTableWidget::EDIT_BUTTON, false, this);
		grid=dynamic_cast<QGridLayout *>(enumerations_gb->layout());
		grid->addWidget(enumerations_tab,1,0,1,2);
		enumerations_gb->setVisible(false);

		attributes_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS ^
																					ObjectTableWidget::UPDATE_BUTTON, false, this);
		attributes_tab->setColumnCount(2);
		attributes_tab->setHeaderLabel(trUtf8("Name"),0);
		attributes_tab->setHeaderIcon(QPixmap(":/icones/icones/uid.png"),0);
		attributes_tab->setHeaderLabel(trUtf8("Type"),1);
		attributes_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);

		grid=new QGridLayout;
		grid->setContentsMargins(2,2,2,2);
		grid->addWidget(attributes_tab,0,0);
		attributes_gb->setLayout(grid);
		attributes_gb->setVisible(false);

		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_83)].push_back(enumeration_rb);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(category_lbl);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(preferred_lbl);
		field_map[generateVersionsInterval(AFTER_VERSION, SchemaParser::PGSQL_VERSION_84)].push_back(like_type);
		frame=generateVersionWarningFrame(field_map);
		type_grid->addWidget(frame, type_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		grid=dynamic_cast<QGridLayout *>(base_attribs_twg->widget(1)->layout());
		frame=generateInformationFrame(trUtf8("The functions to be assigned to a type should be written in C language and possess, respectively, the following signatures:<br/>  <table>   <tr>    <td><strong>INPUT:</strong> <em>any function(cstring, oid, integer)</em></td>    <td><strong>OUTPUT:</strong> <em>cstring function(any)</em></td>   </tr>   <tr>    <td><strong>SEND:</strong> <em>byta function(any)</em></td>    <td><strong>RECV:</strong> <em>any function(internal, oid, integer)</em></td>   </tr>   <tr>    <td><strong>TPMOD_IN:</strong> <em>integer function(cstring[])</em></td>    <td><strong>TPMOD_OUT:</strong> <em>cstring function(integer)</em></td>   </tr>   <tr>    <td><strong>ANALYZE:</strong> <em>boolean function(internal)</em></td>    <tr>  </table>"));
		grid->addWidget(frame, grid->count()+1, 0, 1, 0);
		frame->setParent(base_attribs_twg->widget(1));

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(base_type_rb, SIGNAL(toggled(bool)), this, SLOT(selectTypeConfiguration(void)));
		connect(composite_rb, SIGNAL(toggled(bool)), this, SLOT(selectTypeConfiguration(void)));
		connect(enumeration_rb, SIGNAL(toggled(bool)), this, SLOT(selectTypeConfiguration(void)));
		connect(enumerations_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleEnumeration(int)));
		connect(enumerations_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleEnumeration(int)));
		connect(attributes_tab, SIGNAL(s_rowAdded(int)), this, SLOT(showAttributeForm(void)));
		connect(attributes_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showAttributeForm(void)));

		parent_form->setMinimumSize(625, 765);

		StorageType::getTypes(list);
		storage_cmb->addItems(list);

		CategoryType::getTypes(list);
		category_cmb->addItems(list);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TypeWidget::hideEvent(QHideEvent *event)
{
	enumerations_tab->removeRows();
	attributes_tab->removeRows();

	for(unsigned i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
		functions_sel[i]->clearSelector();

	base_type_rb->setChecked(true);
	internal_len_sb->setValue(0);
	by_value_chk->setChecked(false);
	preferred_chk->setChecked(false);
	delimiter_edt->clear();
	default_value_edt->clear();;
	category_cmb->setCurrentIndex(0);
	storage_cmb->setCurrentIndex(0);
	alignment_cmb->setCurrentIndex(0);
	base_attribs_twg->setCurrentIndex(0);

	disconnect(parameter_wgt,NULL,this,NULL);
	BaseObjectWidget::hideEvent(event);
}

void TypeWidget::selectTypeConfiguration(void)
{
	base_attribs_twg->setVisible(base_type_rb->isChecked());
	enumerations_gb->setVisible(enumeration_rb->isChecked());
	attributes_gb->setVisible(composite_rb->isChecked());
}

void TypeWidget::handleEnumeration(int row)
{
	if(!enum_name_edt->text().isEmpty())
	{
		enumerations_tab->setCellText(enum_name_edt->text(), row, 0);
		enum_name_edt->clear();
	}
	else if(enumerations_tab->getCellText(row, 0).isEmpty())
		enumerations_tab->removeRow(row);
}

void TypeWidget::showAttributeData(int res)
{
	int row;
	Parameter param;

	row=attributes_tab->getSelectedRow();

	//Case the user clicked apply on the attribute form
	if(res==QDialog::Accepted)
	{
		//Get the configured attribute
		param=parameter_wgt->getParameter();

		//Add some information of it onto table
		attributes_tab->setCellText(Utf8String::create(param.getName()), row, 0);
		attributes_tab->setCellText(Utf8String::create(*param.getType()), row, 1);
		attributes_tab->setRowData(QVariant::fromValue<Parameter>(param), row);
	}
	else if(res==QDialog::Rejected)
	{
		if(attributes_tab->getCellText(row,0).isEmpty())
			attributes_tab->removeRow(row);
	}
}

void TypeWidget::showAttributeForm(void)
{
	int row;

	//Disables the fields not used for type attributes on the form
	parameter_wgt->param_in_chk->setEnabled(false);
	parameter_wgt->param_out_chk->setEnabled(false);
	parameter_wgt->default_value_edt->setEnabled(false);
	row=attributes_tab->getSelectedRow();

	if(row >= 0)
		parameter_wgt->setAttributes(attributes_tab->getRowData(row).value<Parameter>(), this->model);

	parameter_wgt->show();
}

void TypeWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Type *type)
{
	PgSQLType like_tp, elem_tp;
	unsigned type_conf, i, count;
	Parameter param;

	connect(parameter_wgt, SIGNAL(finished(int)), this, SLOT(showAttributeData(int)));

	BaseObjectWidget::setAttributes(model, op_list, type, schema);

	for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
		functions_sel[i]->setModel(model);

	if(type)
	{
		type_conf=type->getConfiguration();

		if(type_conf==Type::COMPOSITE_TYPE)
		{
			composite_rb->setChecked(true);
			attributes_tab->blockSignals(true);
			count=type->getAttributeCount();

			for(i=0; i < count; i++)
			{
				attributes_tab->addRow();

				param=type->getAttribute(i);
				attributes_tab->setCellText(Utf8String::create(param.getName()), i, 0);
				attributes_tab->setCellText(Utf8String::create(*param.getType()), i, 1);
				attributes_tab->setRowData(QVariant::fromValue<Parameter>(param), i);
			}

			attributes_tab->blockSignals(false);
			attributes_tab->clearSelection();
		}
		else if(type_conf==Type::ENUMERATION_TYPE)
		{
			enumeration_rb->setChecked(true);
			enumerations_tab->blockSignals(true);
			count=type->getEnumerationCount();

			for(i=0; i < count; i++)
			{
				enumerations_tab->addRow();
				enumerations_tab->setCellText(Utf8String::create(type->getEnumeration(i)), i, 0);
			}

			enumerations_tab->blockSignals(false);
			enumerations_tab->clearSelection();
		}
		else
		{
			base_type_rb->setChecked(true);

			like_tp=type->getLikeType();
			elem_tp=type->getElement();

			internal_len_sb->setValue(type->getInternalLength());
			by_value_chk->setChecked(type->isByValue());
			preferred_chk->setChecked(type->isPreferred());
			delimiter_edt->setText(QString(type->getDelimiter()));
			default_value_edt->setText(Utf8String::create(type->getDefaultValue()));
			category_cmb->setCurrentIndex(category_cmb->findText(~type->getCategory()));
			storage_cmb->setCurrentIndex(storage_cmb->findText(~type->getStorage()));
			alignment_cmb->setCurrentIndex(alignment_cmb->findText(~type->getAlignment()));

			for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
				functions_sel[i]->setSelectedObject(type->getFunction(i));
		}
	}

	like_type->setAttributes(like_tp, model);
	element_type->setAttributes(elem_tp, model);
}

void TypeWidget::applyConfiguration(void)
{
	try
	{
		Type *type=NULL;
		unsigned i, count;

		startConfiguration<Type>();
		type=dynamic_cast<Type *>(this->object);

		BaseObjectWidget::applyConfiguration();

		//Configuring an enumaration type
		if(enumeration_rb->isChecked())
		{
			type->setConfiguration(Type::ENUMERATION_TYPE);
			type->removeEnumerations();

			count=enumerations_tab->getRowCount();
			for(i=0; i < count; i++)
				type->addEnumeration(enumerations_tab->getCellText(i,0).toUtf8());
		}
		//Configuration a composite type
		else if(composite_rb->isChecked())
		{
			type->setConfiguration(Type::COMPOSITE_TYPE);
			type->removeAttributes();

			count=attributes_tab->getRowCount();
			for(i=0; i < count; i++)
				type->addAttribute(attributes_tab->getRowData(i).value<Parameter>());
		}
		//Configuring a base type
		else
		{
			type->setConfiguration(Type::BASE_TYPE);
			type->setLikeType(like_type->getPgSQLType());
			type->setElement(element_type->getPgSQLType());
			type->setInternalLength(internal_len_sb->value());
			type->setByValue(by_value_chk->isChecked());
			type->setPreferred(preferred_chk->isChecked());

			if(!delimiter_edt->text().isEmpty())
				type->setDelimiter(delimiter_edt->text().at(0).toAscii());

			type->setDefaultValue(default_value_edt->text());
			type->setCategory(CategoryType(category_cmb->currentText()));
			type->setAlignment(PgSQLType(alignment_cmb->currentText()));
			type->setStorage(StorageType(storage_cmb->currentText()));

			for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
				type->setFunction(i, dynamic_cast<Function *>(functions_sel[i]->getSelectedObject()));
		}

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

