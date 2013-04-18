/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "typewidget.h"

TypeWidget::TypeWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_TYPE)
{
	try
	{
		QGridLayout *grid=NULL;
		QSpacerItem *spacer=NULL;
		QFrame *frame=NULL;
		QStringList list;
		unsigned i,i1;

		Ui_TypeWidget::setupUi(this);
		configureFormLayout(type_grid, OBJ_TYPE);

		like_type=new PgSQLTypeWidget(this, trUtf8("Like Type"));
		element_type=new PgSQLTypeWidget(this, trUtf8("Element Type"));
		range_subtype=new PgSQLTypeWidget(this, trUtf8("Subtype"));

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
																					 ObjectTableWidget::EDIT_BUTTON, true, this);
		grid=dynamic_cast<QGridLayout *>(enumerations_gb->layout());
		grid->addWidget(enumerations_tab,1,0,1,2);
		enumerations_gb->setVisible(false);

		attributes_tab=new ObjectTableWidget(ObjectTableWidget::ALL_BUTTONS, true, this);
		attributes_tab->setColumnCount(3);
		attributes_tab->setHeaderLabel(trUtf8("Name"),0);
		attributes_tab->setHeaderIcon(QPixmap(":/icones/icones/uid.png"),0);
		attributes_tab->setHeaderLabel(trUtf8("Type"),1);
		attributes_tab->setHeaderIcon(QPixmap(":/icones/icones/usertype.png"),1);
		attributes_tab->setHeaderLabel(trUtf8("Collation"),2);
		attributes_tab->setHeaderIcon(QPixmap(":/icones/icones/collation.png"),2);

		grid=dynamic_cast<QGridLayout *>(attributes_gb->layout());

		attrib_collation_sel=new ObjectSelectorWidget(OBJ_COLLATION, true, this);
		grid->addWidget(attrib_collation_sel, 1,1,1,2);

		attrib_type_wgt=new PgSQLTypeWidget(this);
		grid->addWidget(attrib_type_wgt,2,0,1,2);

		grid->addWidget(attributes_tab,3,0,1,2);
		attributes_gb->setVisible(false);

		grid=dynamic_cast<QGridLayout *>(base_attribs_twg->widget(1)->layout());
		frame=generateInformationFrame(trUtf8("The functions to be assigned to a type should be written in C language and possess, respectively, the following signatures:<br/>  <table>   <tr>    <td><strong>INPUT:</strong> <em>any function(cstring, oid, integer)</em></td>    <td><strong>OUTPUT:</strong> <em>cstring function(any)</em></td>   </tr>   <tr>    <td><strong>SEND:</strong> <em>byta function(any)</em></td>    <td><strong>RECV:</strong> <em>any function(internal, oid, integer)</em></td>   </tr>   <tr>    <td><strong>TPMOD_IN:</strong> <em>integer function(cstring[])</em></td>    <td><strong>TPMOD_OUT:</strong> <em>cstring function(integer)</em></td>   </tr>   <tr>    <td><strong>ANALYZE:</strong> <em>boolean function(internal)</em></td>    <tr>  </table>"));
		grid->addWidget(frame, grid->count()+1, 0, 1, 0);
		frame->setParent(base_attribs_twg->widget(1));

		grid=dynamic_cast<QGridLayout *>(range_attribs_gb->layout());
		opclass_sel=new ObjectSelectorWidget(OBJ_OPCLASS, true, this);
		grid->addWidget(opclass_sel,0,1,1,1);

		for(i1=1, i=Type::CANONICAL_FUNC; i <= Type::SUBTYPE_DIFF_FUNC; i++,i1++)
		{
			functions_sel[i]=NULL;
			functions_sel[i]=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
			grid->addWidget(functions_sel[i],i1,1,1,1);
		}

		grid->addWidget(range_subtype,3,0,1,2);
		spacer=new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
		frame=generateInformationFrame(trUtf8("The functions to be assigned to a range type should have the following signatures:<br/><br/><strong>Canonical:</strong> <em>any function(any)</em> <br/><strong>Subtype Diff:</strong> <em>double precision function(subtype, subtype)</em>"));
		grid->addWidget(frame, 4, 0, 1, 0);
		grid->addItem(spacer,5,0);

		range_attribs_gb->setVisible(false);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));
		connect(base_type_rb, SIGNAL(toggled(bool)), this, SLOT(selectTypeConfiguration(void)));
		connect(composite_rb, SIGNAL(toggled(bool)), this, SLOT(selectTypeConfiguration(void)));
		connect(enumeration_rb, SIGNAL(toggled(bool)), this, SLOT(selectTypeConfiguration(void)));
		connect(enumerations_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleEnumeration(int)));
		connect(enumerations_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleEnumeration(int)));
		connect(attributes_tab, SIGNAL(s_rowAdded(int)), this, SLOT(handleAttribute(int)));
		connect(attributes_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(handleAttribute(int)));
		connect(attributes_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editAttribute(int)));

		parent_form->setMinimumSize(620, 760);

		StorageType::getTypes(list);
		storage_cmb->addItems(list);

		CategoryType::getTypes(list);
		category_cmb->addItems(list);

		setRequiredField(range_subtype);
		setRequiredField(input_lbl);
		setRequiredField(output_lbl);
		setRequiredField(functions_sel[Type::INPUT_FUNC]);
		setRequiredField(functions_sel[Type::OUTPUT_FUNC]);
		setRequiredField(enumerations_gb);
		setRequiredField(attributes_gb);
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

	for(unsigned i=Type::INPUT_FUNC; i <= Type::SUBTYPE_DIFF_FUNC; i++)
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
	opclass_sel->clearSelector();

	collation_sel->setEnabled(true);
	collation_lbl->setEnabled(true);

	BaseObjectWidget::hideEvent(event);
}

void TypeWidget::selectTypeConfiguration(void)
{
	base_attribs_twg->setVisible(base_type_rb->isChecked());
	enumerations_gb->setVisible(enumeration_rb->isChecked());
	attributes_gb->setVisible(composite_rb->isChecked());
	range_attribs_gb->setVisible(range_rb->isChecked());
	collation_sel->setEnabled(range_rb->isChecked());
	collation_lbl->setEnabled(range_rb->isChecked());

	if(!range_rb->isChecked())
		collation_sel->clearSelector();
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

void TypeWidget::showAttributeData(TypeAttribute attrib, int row)
{
	attributes_tab->setCellText(Utf8String::create(attrib.getName()), row, 0);
	attributes_tab->setCellText(Utf8String::create(*attrib.getType()), row, 1);

	if(attrib.getCollation())
		attributes_tab->setCellText(Utf8String::create(attrib.getCollation()->getName(true)), row, 2);
	else
		attributes_tab->clearCellText(row,2);

	attributes_tab->setRowData(QVariant::fromValue<TypeAttribute>(attrib), row);
}

void TypeWidget::editAttribute(int row)
{
	TypeAttribute attrib=attributes_tab->getRowData(row).value<TypeAttribute>();

	attrib_name_edt->setText(Utf8String::create(attrib.getName()));
	attrib_collation_sel->setSelectedObject(attrib.getCollation());
	attrib_type_wgt->setAttributes(attrib.getType(), this->model);
}

void TypeWidget::handleAttribute(int row)
{
	try
	{
		TypeAttribute attrib;

		attrib.setName(attrib_name_edt->text().toUtf8());
		attrib.setType(attrib_type_wgt->getPgSQLType());
		attrib.setCollation(attrib_collation_sel->getSelectedObject());
		showAttributeData(attrib, row);

		attrib_name_edt->clear();
		attrib_collation_sel->clearSelector();
	}
	catch(Exception &e)
	{
		if(attributes_tab->getCellText(row,0).isEmpty())
				attributes_tab->removeRow(row);

		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void TypeWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Type *type)
{
	PgSQLType like_tp, elem_tp;
	unsigned type_conf, i, count;

	BaseObjectWidget::setAttributes(model, op_list, type, schema);

	attrib_collation_sel->setModel(model);
	attrib_type_wgt->setAttributes(PgSQLType(), model);
	range_subtype->setAttributes(PgSQLType(), model);

	opclass_sel->setModel(model);

	for(i=Type::INPUT_FUNC; i <= Type::SUBTYPE_DIFF_FUNC; i++)
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
				showAttributeData(type->getAttribute(i),i);
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
		else if(type_conf==Type::RANGE_TYPE)
		{
			range_rb->setChecked(true);
			opclass_sel->setSelectedObject(type->getSubtypeOpClass());
			range_subtype->setAttributes(type->getSubtype(), model);
			functions_sel[Type::CANONICAL_FUNC]->setSelectedObject(type->getFunction(Type::CANONICAL_FUNC));
			functions_sel[Type::SUBTYPE_DIFF_FUNC]->setSelectedObject(type->getFunction(Type::SUBTYPE_DIFF_FUNC));
		}
		else
		{
			base_type_rb->setChecked(true);

			like_tp=type->getLikeType();
			elem_tp=type->getElement();

			internal_len_sb->setValue(type->getInternalLength());
			by_value_chk->setChecked(type->isByValue());
			preferred_chk->setChecked(type->isPreferred());
			collatable_chk->setChecked(type->isCollatable());
			delimiter_edt->setText(QString(type->getDelimiter()));
			default_value_edt->setText(Utf8String::create(type->getDefaultValue()));
			category_cmb->setCurrentIndex(category_cmb->findText(~type->getCategory()));
			storage_cmb->setCurrentIndex(storage_cmb->findText(~type->getStorage()));
			alignment_cmb->setCurrentIndex(alignment_cmb->findText(~type->getAlignment()));

			for(i=Type::INPUT_FUNC; i <= Type::ANALYZE_FUNC; i++)
				functions_sel[i]->setSelectedObject(type->getFunction(i));
		}
	}
	else
		selectTypeConfiguration();

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

			count=enumerations_tab->getRowCount();
			for(i=0; i < count; i++)
				type->addEnumeration(enumerations_tab->getCellText(i,0).toUtf8());
		}
		//Configuration a composite type
		else if(composite_rb->isChecked())
		{
			type->setConfiguration(Type::COMPOSITE_TYPE);

			count=attributes_tab->getRowCount();
			for(i=0; i < count; i++)
				type->addAttribute(attributes_tab->getRowData(i).value<TypeAttribute>());
		}
		//Configuration a range type
		else if(range_rb->isChecked())
		{
			type->setConfiguration(Type::RANGE_TYPE);
			type->setCollation(collation_sel->getSelectedObject());
			type->setSubtype(range_subtype->getPgSQLType());
			type->setSubtypeOpClass(dynamic_cast<OperatorClass *>(opclass_sel->getSelectedObject()));
			type->setFunction(Type::CANONICAL_FUNC, dynamic_cast<Function *>(functions_sel[Type::CANONICAL_FUNC]->getSelectedObject()));
			type->setFunction(Type::SUBTYPE_DIFF_FUNC, dynamic_cast<Function *>(functions_sel[Type::SUBTYPE_DIFF_FUNC]->getSelectedObject()));
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
			type->setCollatable(collatable_chk->isChecked());

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

