/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "objectreferenceswidget.h"
#include "guiutilsns.h"

const QRegularExpression ObjectReferencesWidget::AttrDelimRegexp = QRegularExpression(QString("(\\%1)+|(\\%2)+")
																																	.arg(SchemaParser::CharStartAttribute)
																																	.arg(SchemaParser::CharEndAttribute));

ObjectReferencesWidget::ObjectReferencesWidget(const std::vector<ObjectType> &types, bool use_ref_alias, QWidget *parent): QWidget(parent)
{
	Ui_ObjectReferencesWidget::setupUi(this);

	this->use_ref_alias = use_ref_alias;

	object_sel = new ObjectSelectorWidget(types, this);
	objects_refs_tab = new ObjectsTableWidget(ObjectsTableWidget::AllButtons ^
																								 ObjectsTableWidget::DuplicateButton, true, this);

	references_grid->addWidget(object_sel, 0, 1, 1, 1);
	references_grid->addWidget(objects_refs_tab, 3, 0, 1, 2);

	objects_refs_tab->setColumnCount(use_ref_alias ? 6 : 5);
	objects_refs_tab->setHeaderLabel(tr("Ref. name"), 0);
	objects_refs_tab->setHeaderIcon(QIcon(GuiUtilsNs::getIconPath("uid")), 0);

	objects_refs_tab->setHeaderLabel(tr("Object"), 1);
	objects_refs_tab->setHeaderIcon(QIcon(GuiUtilsNs::getIconPath(BaseObject::getSchemaName(ObjectType::Table))), 1);

	objects_refs_tab->setHeaderLabel(tr("Type"), 2);
	objects_refs_tab->setHeaderIcon(QIcon(GuiUtilsNs::getIconPath(BaseObject::getSchemaName(ObjectType::Type))), 2);

	objects_refs_tab->setHeaderLabel(tr("Signature"), 3);
	objects_refs_tab->setHeaderLabel(tr("Format name"), 4);

	if(use_ref_alias)
		objects_refs_tab->setHeaderLabel(tr("Ref. alias"), 5);

	ref_alias_lbl->setVisible(use_ref_alias);
	ref_alias_edt->setVisible(use_ref_alias);

	connect(object_sel, &ObjectSelectorWidget::s_selectorChanged, this, [this](bool selected){
			sel_obj_icon_lbl->setPixmap(selected ? GuiUtilsNs::getIconPath(object_sel->getSelectedObject()->getSchemaName()) : QPixmap());
			sel_obj_icon_lbl->setToolTip(selected ? object_sel->getSelectedObject()->getTypeName() : "");
	});

	connect(objects_refs_tab, &ObjectsTableWidget::s_rowAdded, this, &ObjectReferencesWidget::handleObjectReference);
	connect(objects_refs_tab, &ObjectsTableWidget::s_rowEdited, this, &ObjectReferencesWidget::editObjectReference);
	connect(objects_refs_tab, &ObjectsTableWidget::s_rowUpdated, this, &ObjectReferencesWidget::handleObjectReference);

	connect(ref_name_edt, &QLineEdit::textChanged, this, [this](const QString &txt){
		objects_refs_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, !txt.isEmpty() && object_sel->getSelectedObject());
	});

	connect(object_sel, &ObjectSelectorWidget::s_selectorChanged, this, [this](bool obj_selected){
		objects_refs_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, !ref_name_edt->text().isEmpty() && obj_selected);
	});
}

void ObjectReferencesWidget::setAttributes(DatabaseModel *model, const std::vector<GenericSQL::Reference> &refs)
{
	objects_refs_tab->blockSignals(true);

	for(auto &ref : refs)
	{
		objects_refs_tab->addRow();
		showObjectReferenceData(objects_refs_tab->getRowCount() - 1,
														 ref.getObject(), ref.getRefName(), ref.isUseSignature(),
														 ref.isFormatName(), ref.getRefAlias());
	}

	objects_refs_tab->clearSelection();
	objects_refs_tab->blockSignals(false);
	object_sel->setModel(model);
	objects_refs_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, false);
}

std::vector<GenericSQL::Reference> ObjectReferencesWidget::getObjectReferences()
{
	std::vector<GenericSQL::Reference> refs;

	for(unsigned row = 0; row < objects_refs_tab->getRowCount(); row++)
		refs.push_back(objects_refs_tab->getRowData(row).value<GenericSQL::Reference>());

	return refs;
}

void ObjectReferencesWidget::handleObjectReference(int row)
{
	showObjectReferenceData(row,
													object_sel->getSelectedObject(),
													ref_name_edt->text().remove(AttrDelimRegexp),
													use_signature_chk->isChecked(),
													format_name_chk->isChecked(),
													use_ref_alias ? ref_alias_edt->text().remove(AttrDelimRegexp) : "");
	clearObjectReferenceForm();
}

void ObjectReferencesWidget::editObjectReference(int row)
{
	GenericSQL::Reference ref = objects_refs_tab->getRowData(row).value<GenericSQL::Reference>();

	ref_name_edt->setText(ref.getRefName());
	ref_alias_edt->setText(ref.getRefAlias());
	use_signature_chk->setChecked(ref.isUseSignature());
	format_name_chk->setChecked(ref.isFormatName());
	object_sel->setSelectedObject(ref.getObject());
}

void ObjectReferencesWidget::clearObjectReferenceForm()
{
	object_sel->clearSelector();
	ref_name_edt->clear();
	ref_alias_edt->clear();
	use_signature_chk->setChecked(false);
	format_name_chk->setChecked(false);
	objects_refs_tab->clearSelection();
	objects_refs_tab->setButtonsEnabled(ObjectsTableWidget::AddButton, false);
}

void ObjectReferencesWidget::showObjectReferenceData(int row, BaseObject *object, const QString &ref_name, bool use_signature, bool format_name, const QString &ref_alias)
{
	GenericSQL::Reference ref = GenericSQL::Reference(ref_name, object, use_signature, format_name, ref_alias);

	objects_refs_tab->setCellText(ref_name, row, 0);
	objects_refs_tab->setCellText(use_signature ? object->getSignature(format_name) : object->getName(format_name), row, 1);
	objects_refs_tab->setCellText(object->getTypeName(), row, 2);
	objects_refs_tab->setCellText(use_signature ? tr("Yes") : tr("No"), row, 3);
	objects_refs_tab->setCellText(format_name ? tr("Yes") : tr("No"), row, 4);
	objects_refs_tab->setRowData(QVariant::fromValue<GenericSQL::Reference>(ref), row);

	if(use_ref_alias)
		objects_refs_tab->setCellText(ref_alias, row, 5);

}
