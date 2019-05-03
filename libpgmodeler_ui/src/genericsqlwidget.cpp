/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "genericsqlwidget.h"

GenericSQLWidget::GenericSQLWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::GenericSql)
{
	Ui_GenericSQLWidget::setupUi(this);
	configureFormLayout(genericsql_grid, ObjectType::GenericSql);

	ref_name_ht = new HintTextWidget(ref_name_hint, this);
	ref_name_ht->setText(ref_name_edt->statusTip());

	use_signature_ht = new HintTextWidget(use_signature_hint, this);
	use_signature_ht->setText(use_signature_chk->statusTip());

	format_name_ht = new HintTextWidget(format_name_hint, this);
	format_name_ht->setText(format_name_chk->statusTip());

	definition_txt = PgModelerUiNs::createNumberedTextEditor(attribs_tbw->widget(0), true);
	definition_hl = new SyntaxHighlighter(definition_txt);
	definition_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);
	definition_cp=new CodeCompletionWidget(definition_txt, true);

	comment_edt->setVisible(false);
	comment_lbl->setVisible(false);

	preview_txt = PgModelerUiNs::createNumberedTextEditor(attribs_tbw->widget(2), false);
	preview_txt->setReadOnly(true);
	preview_hl = new SyntaxHighlighter(preview_txt);
	preview_hl->loadConfiguration(GlobalAttributes::SQLHighlightConfPath);

	attribs_tbw->widget(0)->layout()->setContentsMargins(4,4,4,4);
	attribs_tbw->widget(0)->layout()->addWidget(definition_txt);

	attribs_tbw->widget(2)->layout()->setContentsMargins(4,4,4,4);
	attribs_tbw->widget(2)->layout()->addWidget(preview_txt);

	object_sel = new ObjectSelectorWidget(BaseObject::getObjectTypes(false, { ObjectType::Database, ObjectType::GenericSql,
																																						ObjectType::Permission, ObjectType::Relationship,
																																						ObjectType::Tag, ObjectType::Textbox }), true, this);
	objects_refs_tab = new ObjectsTableWidget(ObjectsTableWidget::AllButtons, true, this);
	references_grid->addWidget(object_sel, 0, 1, 1, 1);
	references_grid->addWidget(objects_refs_tab, 2, 0, 1, 2);

	objects_refs_tab->setColumnCount(5);
	objects_refs_tab->setHeaderLabel(trUtf8("Ref. name"), 0);
	objects_refs_tab->setHeaderIcon(QIcon(PgModelerUiNs::getIconPath("uid")), 0);

	objects_refs_tab->setHeaderLabel(trUtf8("Object"), 1);
	objects_refs_tab->setHeaderIcon(QIcon(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(ObjectType::Table))), 1);

	objects_refs_tab->setHeaderLabel(trUtf8("Type"), 2);
	objects_refs_tab->setHeaderIcon(QIcon(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(ObjectType::Type))), 2);

	objects_refs_tab->setHeaderLabel(trUtf8("Signature"), 3);
	objects_refs_tab->setHeaderLabel(trUtf8("Format name"), 4);

	setMinimumSize(700, 500);
	updateCodePreview();

	connect(object_sel, &ObjectSelectorWidget::s_selectorChanged, [&](bool selected){
			sel_obj_icon_lbl->setPixmap(selected ? PgModelerUiNs::getIconPath(object_sel->getSelectedObject()->getSchemaName()) : QPixmap());
			sel_obj_icon_lbl->setToolTip(selected ? object_sel->getSelectedObject()->getTypeName() : QString());
	});

	connect(objects_refs_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addObjectReference(int)));

	connect(objects_refs_tab, &ObjectsTableWidget::s_rowAboutToRemove, [&](int row){
		QString ref_name = objects_refs_tab->getCellText(row, 0);
		dummy_gsql.removeObjectReference(ref_name);
		updateCodePreview();
	});

	connect(objects_refs_tab, &ObjectsTableWidget::s_rowsRemoved, [&](){
		dummy_gsql.removeObjectReferences();
		updateCodePreview();
	});
}

void GenericSQLWidget::setAttributes(DatabaseModel *model, OperationList *op_list, GenericSQL *genericsql)
{
	if(genericsql)
		definition_txt->setPlainText(genericsql->getDefinition());

	BaseObjectWidget::setAttributes(model, op_list, genericsql);

	object_sel->setModel(model);
	definition_cp->configureCompletion(model, definition_hl);
}

void GenericSQLWidget::applyConfiguration(void)
{
	try
	{
		GenericSQL *genericsql=nullptr;

		startConfiguration<GenericSQL>();
		genericsql=dynamic_cast<GenericSQL *>(this->object);
		genericsql->setDefinition(definition_txt->toPlainText());

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void GenericSQLWidget::addObjectReference(int row)
{
	try
	{
		QString ref_name = ref_name_edt->text();
		BaseObject *object = object_sel->getSelectedObject();
		bool use_signature = use_signature_chk->isChecked(),
				format_name = format_name_chk->isChecked();

		dummy_gsql.addObjectReference(object, ref_name, use_signature, format_name);
		objects_refs_tab->setCellText(ref_name, row, 0);
		objects_refs_tab->setCellText(use_signature ? object->getSignature(format_name) : object->getName(format_name), row, 1);
		objects_refs_tab->setCellText(object->getTypeName(), row, 2);
		objects_refs_tab->setCellText(use_signature ? trUtf8("Yes") : trUtf8("No"), row, 3);
		objects_refs_tab->setCellText(use_signature ? trUtf8("Yes") : trUtf8("No"), row, 4);

		clearObjectReferenceForm();
		updateCodePreview();
	}
	catch(Exception &e)
	{
		objects_refs_tab->removeRow(row);
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void GenericSQLWidget::clearObjectReferenceForm(void)
{
	object_sel->clearSelector();
	ref_name_edt->clear();
	use_signature_chk->setChecked(false);
	format_name_chk->setChecked(false);
}

void GenericSQLWidget::updateCodePreview(void)
{
	try
	{
		if(objects_refs_tab->getRowCount() > 0 && !definition_txt->toPlainText().isEmpty())
		{
			dummy_gsql.setDefinition(definition_txt->toPlainText());
			preview_txt->setPlainText(dummy_gsql.getCodeDefinition(SchemaParser::SqlDefinition));
		}
		else
			preview_txt->setPlainText(QString("-- %1 --").arg(trUtf8("No definition SQL code or references defined! Preview unavailable.")));
	}
	catch(Exception &e)
	{
		preview_txt->setPlainText(QString("/* %1 */").arg(e.getExceptionsText()));
	}
}

