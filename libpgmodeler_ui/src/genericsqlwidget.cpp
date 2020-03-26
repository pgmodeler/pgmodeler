/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

const QRegExp GenericSQLWidget::AttrDelimRegexp = QRegExp(QString("(\\%1)+|(\\%2)+")
																													.arg(SchemaParser::CharIniAttribute)
																													.arg(SchemaParser::CharEndAttribute));

GenericSQLWidget::GenericSQLWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::GenericSql)
{
	vector<ObjectType> types;

	Ui_GenericSQLWidget::setupUi(this);
	configureFormLayout(genericsql_grid, ObjectType::GenericSql);

	definition_txt = PgModelerUiNs::createNumberedTextEditor(attribs_tbw->widget(0), true);
	definition_hl = new SyntaxHighlighter(definition_txt);
	definition_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());
	definition_cp=new CodeCompletionWidget(definition_txt, true);

	comment_edt->setVisible(false);
	comment_lbl->setVisible(false);

	preview_txt = PgModelerUiNs::createNumberedTextEditor(attribs_tbw->widget(2), false);
	preview_txt->setReadOnly(true);
	preview_hl = new SyntaxHighlighter(preview_txt);
	preview_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

	attribs_tbw->widget(0)->layout()->setContentsMargins(4,4,4,4);
	attribs_tbw->widget(0)->layout()->addWidget(definition_txt);

	attribs_tbw->widget(2)->layout()->setContentsMargins(4,4,4,4);
	attribs_tbw->widget(2)->layout()->addWidget(preview_txt);

	// Configuring the object types accepted by object references
	types = BaseObject::getObjectTypes(false, { ObjectType::Database, ObjectType::GenericSql,
																							ObjectType::Permission, ObjectType::Relationship,
																							ObjectType::Tag, ObjectType::Textbox });
	types.push_back(ObjectType::Column);

	object_sel = new ObjectSelectorWidget(types, true, this);

	objects_refs_tab = new ObjectsTableWidget(ObjectsTableWidget::AllButtons, true, this);
	references_grid->addWidget(object_sel, 0, 1, 1, 1);
	references_grid->addWidget(objects_refs_tab, 2, 0, 1, 2);

	objects_refs_tab->setColumnCount(5);
	objects_refs_tab->setHeaderLabel(tr("Ref. name"), 0);
	objects_refs_tab->setHeaderIcon(QIcon(PgModelerUiNs::getIconPath("uid")), 0);

	objects_refs_tab->setHeaderLabel(tr("Object"), 1);
	objects_refs_tab->setHeaderIcon(QIcon(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(ObjectType::Table))), 1);

	objects_refs_tab->setHeaderLabel(tr("Type"), 2);
	objects_refs_tab->setHeaderIcon(QIcon(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(ObjectType::Type))), 2);

	objects_refs_tab->setHeaderLabel(tr("Signature"), 3);
	objects_refs_tab->setHeaderLabel(tr("Format name"), 4);

	setMinimumSize(700, 500);

	connect(object_sel, &ObjectSelectorWidget::s_selectorChanged, [&](bool selected){
			sel_obj_icon_lbl->setPixmap(selected ? PgModelerUiNs::getIconPath(object_sel->getSelectedObject()->getSchemaName()) : QPixmap());
			sel_obj_icon_lbl->setToolTip(selected ? object_sel->getSelectedObject()->getTypeName() : QString());
	});

	connect(objects_refs_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addObjectReference(int)));
	connect(objects_refs_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editObjectReference(int)));
	connect(objects_refs_tab, SIGNAL(s_rowUpdated(int)), this, SLOT(updateObjectReference(int)));

	connect(objects_refs_tab, &ObjectsTableWidget::s_rowAboutToRemove, [&](int row){
		QString ref_name = objects_refs_tab->getCellText(row, 0);
		dummy_gsql.removeObjectReference(ref_name);
	});

	connect(objects_refs_tab, &ObjectsTableWidget::s_rowsRemoved, [&](){
		dummy_gsql.removeObjectReferences();
	});

	connect(attribs_tbw, &QTabWidget::currentChanged, [&](int idx){
		if(idx == attribs_tbw->count() - 1)
			updateCodePreview();
	});
}

void GenericSQLWidget::setAttributes(DatabaseModel *model, OperationList *op_list, GenericSQL *genericsql)
{
	BaseObjectWidget::setAttributes(model, op_list, genericsql);

	if(genericsql)
	{
		dummy_gsql = *genericsql;
		definition_txt->setPlainText(genericsql->getDefinition());
		objects_refs_tab->blockSignals(true);

		for(auto &ref : genericsql->getObjectsReferences())
		{
			objects_refs_tab->addRow();
			showObjectReferenceData(objects_refs_tab->getRowCount() - 1,
															ref.object, ref.ref_name, ref.use_signature, ref.format_name);
		}

		objects_refs_tab->blockSignals(false);
	}

	object_sel->setModel(model);
	definition_cp->configureCompletion(model, definition_hl);
}

void GenericSQLWidget::addObjectReference(int row)
{
	try
	{
		QString ref_name = ref_name_edt->text().remove(AttrDelimRegexp);
		BaseObject *object = object_sel->getSelectedObject();
		bool use_signature = use_signature_chk->isChecked(),
				format_name = format_name_chk->isChecked();

		dummy_gsql.addObjectReference(object, ref_name, use_signature, format_name);
		showObjectReferenceData(row, object, ref_name, use_signature, format_name);
		clearObjectReferenceForm();
	}
	catch(Exception &e)
	{
		objects_refs_tab->blockSignals(true);
		objects_refs_tab->removeRow(row);
		objects_refs_tab->blockSignals(false);
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void GenericSQLWidget::editObjectReference(int row)
{
	ref_name_edt->setText(objects_refs_tab->getCellText(row, 0));
	use_signature_chk->setChecked(objects_refs_tab->getCellText(row, 3) == tr("Yes"));
	format_name_chk->setChecked(objects_refs_tab->getCellText(row, 4) == tr("Yes"));
	object_sel->setSelectedObject(reinterpret_cast<BaseObject *>(objects_refs_tab->getRowData(row).value<void *>()));
}

void GenericSQLWidget::updateObjectReference(int row)
{
	QString ref_name = objects_refs_tab->getCellText(row, 0),
			new_ref_name = ref_name_edt->text().remove(AttrDelimRegexp);
	BaseObject *object = object_sel->getSelectedObject();
	bool use_signature = use_signature_chk->isChecked(),
			format_name = format_name_chk->isChecked();

	try
	{
		dummy_gsql.updateObjectReference(ref_name, object, new_ref_name, use_signature, format_name);
		showObjectReferenceData(row, object, new_ref_name, use_signature, format_name);
		clearObjectReferenceForm();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void GenericSQLWidget::clearObjectReferenceForm()
{
	object_sel->clearSelector();
	ref_name_edt->clear();
	use_signature_chk->setChecked(false);
	format_name_chk->setChecked(false);
	objects_refs_tab->clearSelection();
}

void GenericSQLWidget::updateCodePreview()
{
	try
	{
		if(!name_edt->text().isEmpty() && !definition_txt->toPlainText().isEmpty())
		{
			if(!name_edt->text().isEmpty())
				dummy_gsql.setName(name_edt->text());

			dummy_gsql.setDefinition(definition_txt->toPlainText());
			dummy_gsql.setCodeInvalidated(true);
			preview_txt->setPlainText(dummy_gsql.getCodeDefinition(SchemaParser::SqlDefinition));
		}
		else
			preview_txt->setPlainText(QString("-- %1 --").arg(tr("No object name, SQL code or references defined! Preview unavailable.")));
	}
	catch(Exception &e)
	{
		preview_txt->setPlainText(QString("/* %1 */").arg(e.getExceptionsText()));
	}
}

void GenericSQLWidget::showObjectReferenceData(int row, BaseObject *object, const QString &ref_name, bool use_signature, bool format_name)
{
	objects_refs_tab->setCellText(ref_name, row, 0);
	objects_refs_tab->setCellText(use_signature ? object->getSignature(format_name) : object->getName(format_name), row, 1);
	objects_refs_tab->setCellText(object->getTypeName(), row, 2);
	objects_refs_tab->setCellText(use_signature ? tr("Yes") : tr("No"), row, 3);
	objects_refs_tab->setCellText(format_name ? tr("Yes") : tr("No"), row, 4);
	objects_refs_tab->setRowData(QVariant::fromValue<void *>(reinterpret_cast<void *>(object)), row);
}

void GenericSQLWidget::applyConfiguration()
{
	try
	{
		GenericSQL *genericsql=nullptr;

		startConfiguration<GenericSQL>();
		genericsql=dynamic_cast<GenericSQL *>(this->object);
		dummy_gsql.setDefinition(definition_txt->toPlainText());
		*genericsql = dummy_gsql;

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
