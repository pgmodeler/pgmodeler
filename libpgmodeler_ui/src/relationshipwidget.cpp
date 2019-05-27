/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "relationshipwidget.h"
#include "constraintwidget.h"
#include "columnwidget.h"
#include "tablewidget.h"
#include "baseform.h"
#include "relationshipconfigwidget.h"
#include "generalconfigwidget.h"

RelationshipWidget::RelationshipWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_RELATIONSHIP)
{
	try
	{
		Ui_RelationshipWidget::setupUi(this);

		QStringList list;
		QGridLayout *grid=nullptr;
		QVBoxLayout *vlayout=nullptr;
		QFrame *frame=nullptr;
		QWidgetList pattern_fields={ src_col_pattern_txt, dst_col_pattern_txt,
									 src_fk_pattern_txt, dst_fk_pattern_txt,
									 pk_pattern_txt, uq_pattern_txt, pk_col_pattern_txt };

		gen_tab_name_ht=new HintTextWidget(gen_tab_name_hint, this);
		gen_tab_name_ht->setText(relnn_tab_name_edt->statusTip());

		ref_table_ht=new HintTextWidget(ref_table_hint, this);
		recv_table_ht=new HintTextWidget(recv_table_hint, this);

		identifier_ht=new HintTextWidget(identifier_hint, this);
		identifier_ht->setText(identifier_chk->statusTip());

		single_pk_ht=new HintTextWidget(single_pk_hint, this);
		single_pk_ht->setText(single_pk_chk->statusTip());

		table1_hl=nullptr;
		table1_hl=new SyntaxHighlighter(ref_table_txt, true);
		table1_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		table2_hl=nullptr;
		table2_hl=new SyntaxHighlighter(recv_table_txt, true);
		table2_hl->loadConfiguration(GlobalAttributes::SQL_HIGHLIGHT_CONF_PATH);

		for(int i=0; i < pattern_fields.size(); i++)
		{
			patterns_hl[i]=new SyntaxHighlighter(qobject_cast<QPlainTextEdit *>(pattern_fields[i]), true);
			patterns_hl[i]->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
											  GlobalAttributes::DIR_SEPARATOR +
											  GlobalAttributes::PATTERN_HIGHLIGHT_CONF +
											  GlobalAttributes::CONFIGURATION_EXT);
		}

		attributes_tab=new ObjectsTableWidget(ObjectsTableWidget::ALL_BUTTONS ^
											 (ObjectsTableWidget::UPDATE_BUTTON |
											  ObjectsTableWidget::MOVE_BUTTONS), true, this);

		constraints_tab=new ObjectsTableWidget(ObjectsTableWidget::ALL_BUTTONS  ^
											  (ObjectsTableWidget::UPDATE_BUTTON |
											   ObjectsTableWidget::MOVE_BUTTONS), true, this);

		advanced_objs_tab=new ObjectsTableWidget(ObjectsTableWidget::EDIT_BUTTON, true, this);

		attributes_tab->setColumnCount(2);
		attributes_tab->setHeaderLabel(trUtf8("Attribute"), 0);
		attributes_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("column")),0);
		attributes_tab->setHeaderLabel(trUtf8("Type"), 1);
		attributes_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),1);

		constraints_tab->setColumnCount(2);
		constraints_tab->setHeaderLabel(trUtf8("Constraint"), 0);
		constraints_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("constraint")),0);
		constraints_tab->setHeaderLabel(trUtf8("Type"), 1);
		constraints_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),1);

		advanced_objs_tab->setColumnCount(2);
		advanced_objs_tab->setHeaderLabel(trUtf8("Name"), 0);
		advanced_objs_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("column")),0);
		advanced_objs_tab->setHeaderLabel(trUtf8("Type"), 1);
		advanced_objs_tab->setHeaderIcon(QPixmap(PgModelerUiNS::getIconPath("usertype")),1);

		connect(advanced_objs_tab, SIGNAL(s_rowEdited(int)), this, SLOT(showAdvancedObject(int)));

		grid=new QGridLayout;
		grid->addWidget(attributes_tab, 0,0,1,1);
		grid->setContentsMargins(4,4,4,4);
		rel_attribs_tbw->widget(1)->setLayout(grid);

		grid=new QGridLayout;
		grid->addWidget(constraints_tab, 0,0,1,1);
		grid->setContentsMargins(4,4,4,4);
		rel_attribs_tbw->widget(2)->setLayout(grid);

		grid=dynamic_cast<QGridLayout *>(rel_attribs_tbw->widget(3)->layout());
		frame=generateInformationFrame(trUtf8("Use the special primary key if you want to include a primary key containing generated columns to the receiver table. <strong>Important:</strong> if this is a new relationship there is a need to finish its creation and reopen this dialog to create the special primary key."));

		grid->addWidget(frame, 1, 0, 1, 1);
		frame->setParent(rel_attribs_tbw->widget(3));

		grid=new QGridLayout;
		grid->setContentsMargins(4,4,4,4);

		grid->addWidget(advanced_objs_tab, 0, 0, 1, 1);

		frame=generateInformationFrame(trUtf8("This advanced tab shows the objects (columns or table) auto created by the relationship's connection as well the foreign keys that represents the link between the participant tables."));
		grid->addWidget(frame, 1, 0, 1, 1);

		rel_attribs_tbw->widget(4)->setLayout(grid);

		color_picker=new ColorPickerWidget(1,this);
		color_picker->setEnabled(false);
		grid=dynamic_cast<QGridLayout *>(rel_attribs_tbw->widget(0)->layout());
		grid->addWidget(color_picker, 0, 1);

		configureFormLayout(relationship_grid, OBJ_RELATIONSHIP);

		DeferralType::getTypes(list);
		deferral_cmb->addItems(list);

		frame=generateInformationFrame(trUtf8("Available tokens to define name patterns:<br/>\
					<strong>%1</strong> = Reference (source) primary key column name. <em>(Ignored on constraint patterns)</em><br/> \
					<strong>%2</strong> = Reference (source) table name.<br/> \
					<strong>%3</strong> = Receiver (destination) table name.<br/> \
					<strong>%4</strong> = Generated table name. <em>(Only for n:n relationships)</em>")
					.arg(Relationship::SRC_COL_TOKEN)
					.arg(Relationship::SRC_TAB_TOKEN)
					.arg(Relationship::DST_TAB_TOKEN)
					.arg(Relationship::GEN_TAB_TOKEN));
		vlayout=dynamic_cast<QVBoxLayout *>(name_patterns_grp->layout());
		vlayout->addWidget(frame);

		ActionType::getTypes(list);
		list.push_front(trUtf8("Default"));
		del_action_cmb->addItems(list);
		upd_action_cmb->addItems(list);

		tabs={ nullptr, rel_attribs_tbw->widget(ATTRIBUTES_TAB), rel_attribs_tbw->widget(CONSTRAINTS_TAB),
			   rel_attribs_tbw->widget(SPECIAL_PK_TAB), rel_attribs_tbw->widget(ADVANCED_TAB) };

		tab_labels=QStringList{ QString(), rel_attribs_tbw->tabText(ATTRIBUTES_TAB), rel_attribs_tbw->tabText(CONSTRAINTS_TAB),
				   rel_attribs_tbw->tabText(SPECIAL_PK_TAB), rel_attribs_tbw->tabText(ADVANCED_TAB)};

		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_cmb, SLOT(setEnabled(bool)));
		connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_lbl, SLOT(setEnabled(bool)));

		connect(identifier_chk, &QCheckBox::toggled, [&](){
			table1_mand_chk->setDisabled(identifier_chk->isChecked());
			table2_mand_chk->setEnabled(!identifier_chk->isChecked() &&
																	this->object &&
																	dynamic_cast<BaseRelationship *>(this->object)->getRelationshipType() != BaseRelationship::RELATIONSHIP_1N);
		});

		connect(attributes_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeObjects(void)));
		connect(attributes_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addObject(void)));
		connect(attributes_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editObject(int)));
		connect(attributes_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeObject(int)));
		connect(attributes_tab, SIGNAL(s_rowDuplicated(int,int)), this, SLOT(duplicateObject(int,int)));

		connect(constraints_tab, SIGNAL(s_rowsRemoved(void)), this, SLOT(removeObjects(void)));
		connect(constraints_tab, SIGNAL(s_rowAdded(int)), this, SLOT(addObject(void)));
		connect(constraints_tab, SIGNAL(s_rowEdited(int)), this, SLOT(editObject(int)));
		connect(constraints_tab, SIGNAL(s_rowRemoved(int)), this, SLOT(removeObject(int)));
		connect(constraints_tab, SIGNAL(s_rowDuplicated(int,int)), this, SLOT(duplicateObject(int,int)));

		connect(defaults_rb, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));
		connect(including_rb, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));
		connect(excluding_rb, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));

		connect(defaults_chk, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));
		connect(constraints_chk, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));
		connect(comments_chk, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));
		connect(indexes_chk, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));
		connect(storage_chk, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));
		connect(all_chk, SIGNAL(toggled(bool)), this, SLOT(selectCopyOptions(void)));

		connect(custom_color_chk, SIGNAL(toggled(bool)), color_picker, SLOT(setEnabled(bool)));

		connect(fk_gconf_chk, SIGNAL(toggled(bool)), this, SLOT(useFKGlobalSettings(bool)));
		connect(patterns_gconf_chk, SIGNAL(toggled(bool)), this, SLOT(usePatternGlobalSettings(bool)));

		setMinimumSize(620, 670);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Table *src_tab, Table *dst_tab, unsigned rel_type)
{
	Relationship *rel=nullptr;

	try
	{
		rel=new Relationship(rel_type, src_tab, dst_tab);

		color_picker->generateRandomColors();
		rel->setCustomColor(color_picker->getColor(0));

		this->new_object=true;
		this->setAttributes(model, op_list, rel);

		op_list->startOperationChain();
		operation_count=op_list->getCurrentSize();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::setAttributes(DatabaseModel *model, OperationList *op_list, BaseRelationship *base_rel)
{
	unsigned rel_type, i;
	Relationship *aux_rel=nullptr;
	bool rel1n=false, relnn=false, relgen_dep=false, use_name_patterns=false;

	if(!base_rel)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	BaseObjectWidget::setAttributes(model, op_list, base_rel);

	if(!this->new_object)
	{
		op_list->startOperationChain();
		operation_count=op_list->getCurrentSize();
	}

	rel_type=base_rel->getRelationshipType();
	switch(rel_type)
	{
		case BaseRelationship::RELATIONSHIP_11: rel_11_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_1N: rel_1n_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_NN: rel_nn_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_GEN: rel_gen_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_FK:  rel_fk_rb->setChecked(true); break;
		case BaseRelationship::RELATIONSHIP_DEP: rel_dep_rb->setChecked(true); break;
	}

	aux_rel=dynamic_cast<Relationship *>(base_rel);

	if(base_rel->getObjectType()==BASE_RELATIONSHIP)
	{
		if(base_rel->getRelationshipType()!=BaseRelationship::RELATIONSHIP_FK)
		{
			ref_table_lbl->setText(trUtf8("Referer View:"));
			ref_table_ht->setText(trUtf8("Referer view references one or more columns of a table to construct it's own columns."));
			recv_table_ht->setText(trUtf8("Referenced table has its columns referenced by a view in order to construct the columns of this latter."));
		}
		else
		{
			ref_table_lbl->setText(trUtf8("Referer Table:"));
			ref_table_ht->setText(trUtf8("Referer table references one or more columns of a table through foreign keys. This is the (n) side of relationship."));
			recv_table_ht->setText(trUtf8("Referenced table has its columns referenced by a table's foreign key. This is the (1) side of relationship."));
		}

		recv_table_lbl->setText(trUtf8("Referenced Table:"));

		ref_table_txt->setPlainText(base_rel->getTable(BaseRelationship::SRC_TABLE)->getName(true));
		recv_table_txt->setPlainText(base_rel->getTable(BaseRelationship::DST_TABLE)->getName(true));
	}
	else if(aux_rel)
	{
		if(rel_type!=BaseRelationship::RELATIONSHIP_NN)
		{
			ref_table_lbl->setText(trUtf8("Reference Table:"));
			ref_table_ht->setText(trUtf8("Reference table has the columns from its primary key will copied to the receiver table in order to represent the linking between them. This is the (1) side of relationship."));

			recv_table_lbl->setText(trUtf8("Receiver Table:"));
			recv_table_ht->setText(trUtf8("Receiver (or referer) table will receive the generated columns and the foreign key in order to represent the linking between them. This is the (n) side of relationship."));

			ref_table_txt->setPlainText(aux_rel->getReferenceTable()->getName(true));
			recv_table_txt->setPlainText(aux_rel->getReceiverTable()->getName(true));
		}
		else
		{
			ref_table_lbl->setText(trUtf8("Reference Table:"));
			ref_table_ht->setText(trUtf8("In many-to-many relationships both tables are used as reference to generate the table that represents the linking. Columns from both tables are copied to the resultant table and two foreign keys are created as well in order to reference each participant table."));
			recv_table_lbl->setText(trUtf8("Reference Table:"));
			recv_table_ht->setText(ref_table_ht->getText());
			ref_table_txt->setPlainText(base_rel->getTable(BaseRelationship::SRC_TABLE)->getName(true));
			recv_table_txt->setPlainText(base_rel->getTable(BaseRelationship::DST_TABLE)->getName(true));
		}
	}

	disable_sql_chk->setVisible(base_rel->getObjectType()==OBJ_RELATIONSHIP);
	table1_mand_chk->setText(base_rel->getTable(BaseRelationship::SRC_TABLE)->getName() + trUtf8(" is required"));
	table2_mand_chk->setText(base_rel->getTable(BaseRelationship::DST_TABLE)->getName() + trUtf8(" is required"));

	if(aux_rel)
	{
		single_pk_chk->setChecked(aux_rel->isSiglePKColumn());
		table1_mand_chk->setChecked(aux_rel->isTableMandatory(BaseRelationship::SRC_TABLE));
		table2_mand_chk->setChecked(aux_rel->isTableMandatory(BaseRelationship::DST_TABLE));
		identifier_chk->setChecked(aux_rel->isIdentifier());
		relnn_tab_name_edt->setText(aux_rel->getTableNameRelNN());
		attributes_tab->setButtonsEnabled(ObjectsTableWidget::ALL_BUTTONS, !aux_rel->isProtected());
		constraints_tab->setButtonsEnabled(ObjectsTableWidget::ALL_BUTTONS, !aux_rel->isProtected());

		//Lists the relationship attributes
		listObjects(OBJ_COLUMN);

		//Lists the relationship constraints
		listObjects(OBJ_CONSTRAINT);

		listSpecialPkColumns();

		if(rel_type!=BaseRelationship::RELATIONSHIP_NN)
		{
			if(rel_type==BaseRelationship::RELATIONSHIP_DEP)
			{
				CopyOptions copy_op=aux_rel->getCopyOptions();

				including_rb->setChecked(copy_op.isIncluding());
				excluding_rb->setChecked(copy_op.isExcluding());
				all_chk->setChecked(copy_op.isOptionSet(CopyOptions::ALL));
				defaults_chk->setChecked(!all_chk->isChecked() && copy_op.isOptionSet(CopyOptions::DEFAULTS));
				constraints_chk->setChecked(!all_chk->isChecked() && copy_op.isOptionSet(CopyOptions::CONSTRAINTS));
				storage_chk->setChecked(!all_chk->isChecked() && copy_op.isOptionSet(CopyOptions::STORAGE));
				comments_chk->setChecked(!all_chk->isChecked() && copy_op.isOptionSet(CopyOptions::COMMENTS));
				indexes_chk->setChecked(!all_chk->isChecked() && copy_op.isOptionSet(CopyOptions::INDEXES));
				identity_chk->setChecked(!all_chk->isChecked() && copy_op.isOptionSet(CopyOptions::IDENTITY));
				statistics_chk->setChecked(!all_chk->isChecked() && copy_op.isOptionSet(CopyOptions::STATISTICS));
			}
		}
	}

	rel1n=(rel_type==BaseRelationship::RELATIONSHIP_11 ||
		   rel_type==BaseRelationship::RELATIONSHIP_1N);

	relnn=(rel_type==BaseRelationship::RELATIONSHIP_NN);

	relgen_dep=(rel_type==BaseRelationship::RELATIONSHIP_DEP ||
				rel_type==BaseRelationship::RELATIONSHIP_GEN ||
				rel_type==BaseRelationship::RELATIONSHIP_FK);

	use_name_patterns=(rel1n || relnn ||
					   (relgen_dep && base_rel->getObjectType()==OBJ_RELATIONSHIP));

	name_patterns_grp->setVisible(use_name_patterns);

	dst_col_pattern_txt->setEnabled(relnn);
	dst_fk_pattern_txt->setEnabled(relnn);
	dst_col_pattern_lbl->setEnabled(relnn);
	dst_fk_pattern_lbl->setEnabled(relnn);
	src_col_pattern_lbl->setEnabled(!relgen_dep);
	src_col_pattern_txt->setEnabled(!relgen_dep);
	src_fk_pattern_lbl->setEnabled(!relgen_dep);
	src_fk_pattern_txt->setEnabled(!relgen_dep);
	uq_pattern_lbl->setEnabled(!relgen_dep);
	uq_pattern_txt->setEnabled(!relgen_dep);

	pk_col_pattern_lbl->setEnabled(relnn);
	pk_col_pattern_txt->setEnabled(relnn);

	card_lbl->setVisible(rel1n);
	table1_mand_chk->setEnabled(rel1n);
	table1_mand_chk->setVisible(rel1n);
	table2_mand_chk->setEnabled(rel_type==BaseRelationship::RELATIONSHIP_11);
	table2_mand_chk->setVisible(rel1n);

	identifier_wgt->setVisible(rel1n && !base_rel->isSelfRelationship());
	foreign_key_gb->setVisible(rel1n || relnn);
	single_pk_wgt->setVisible(relnn);

	relnn_tab_name_lbl->setVisible(relnn);
	relnn_tab_name_edt->setVisible(relnn);
	gen_tab_name_hint->setVisible(relnn);

	for(i=ATTRIBUTES_TAB; i <= ADVANCED_TAB; i++)
		rel_attribs_tbw->removeTab(1);

	if(!relgen_dep)
	{
		for(i=ATTRIBUTES_TAB; i <= SPECIAL_PK_TAB; i++)
			rel_attribs_tbw->addTab(tabs[i], tab_labels[i]);
	}
	else if(relgen_dep && base_rel->getObjectType()==OBJ_RELATIONSHIP)
		rel_attribs_tbw->addTab(tabs[SPECIAL_PK_TAB], tab_labels[SPECIAL_PK_TAB]);

	if(base_rel->getObjectType()==OBJ_RELATIONSHIP ||
			(base_rel->getObjectType()==BASE_RELATIONSHIP &&
			 base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK))
		rel_attribs_tbw->addTab(tabs[ADVANCED_TAB], tab_labels[ADVANCED_TAB]);

	copy_options_grp->setVisible(base_rel->getObjectType()==OBJ_RELATIONSHIP &&
								 base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_DEP);

	custom_color_chk->setChecked(base_rel->getCustomColor()!=Qt::transparent);
	color_picker->setColor(0, base_rel->getCustomColor());
	listAdvancedObjects();

	if(rel1n || relnn)
	{
		fk_gconf_chk->blockSignals(true);
		fk_gconf_chk->setChecked(this->new_object);
		useFKGlobalSettings(this->new_object);
		fk_gconf_chk->blockSignals(false);
	}

	if(use_name_patterns)
	{
		patterns_gconf_chk->blockSignals(true);
		patterns_gconf_chk->setChecked(this->new_object);
		usePatternGlobalSettings(this->new_object);
		patterns_gconf_chk->blockSignals(false);
	}
}

QSize RelationshipWidget::getIdealSize(void)
{
	if(rel_fk_rb->isChecked() ||
		 (rel_dep_rb->isChecked() &&	this->object && this->object->getObjectType()==BASE_RELATIONSHIP))
		return(QSize(640, 320));
	else if(rel_gen_rb->isChecked())
		return(QSize(640, 520));
	else
		return(QSize(640, 680));
}

void RelationshipWidget::useFKGlobalSettings(bool value)
{
	fk_wgt->setEnabled(!value);

	if(value)
	{
		map<QString, attribs_map> confs=RelationshipConfigWidget::getConfigurationParams();
		deferrable_chk->setChecked(confs[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
		deferral_cmb->setCurrentText(confs[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEFER_TYPE]);
		upd_action_cmb->setCurrentText(confs[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::UPD_ACTION]);
		del_action_cmb->setCurrentText(confs[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEL_ACTION]);
	}
	else
	{
		Relationship *rel=dynamic_cast<Relationship *>(this->object);
		int idx=-1;

		//Using the settings of the relatinship itself
		if(rel)
		{
			deferrable_chk->setChecked(rel->isDeferrable());
			idx=deferral_cmb->findText(~rel->getDeferralType());
			deferral_cmb->setCurrentIndex(idx < 0 ? 0 : idx);

			idx=del_action_cmb->findText(~rel->getActionType(Constraint::DELETE_ACTION));
			del_action_cmb->setCurrentIndex(idx < 0 ? 0 : idx);

			idx=upd_action_cmb->findText(~rel->getActionType(Constraint::UPDATE_ACTION));
			upd_action_cmb->setCurrentIndex(idx < 0 ? 0 : idx);
		}
	}
}

void RelationshipWidget::usePatternGlobalSettings(bool value)
{
	Relationship *rel=dynamic_cast<Relationship *>(this->object);
	patterns_wgt->setEnabled(!value);

	if(rel)
	{
		if(value)
		{
			map<QString, attribs_map> confs=RelationshipConfigWidget::getConfigurationParams();
			QString rel_type=rel->getRelTypeAttribute();

			//Using the global settings
			pk_pattern_txt->setPlainText(confs[rel_type][ParsersAttributes::PK_PATTERN]);
			src_fk_pattern_txt->setPlainText(confs[rel_type][ParsersAttributes::SRC_FK_PATTERN]);
			dst_fk_pattern_txt->setPlainText(confs[rel_type][ParsersAttributes::DST_FK_PATTERN]);
			uq_pattern_txt->setPlainText(confs[rel_type][ParsersAttributes::UQ_PATTERN]);
			src_col_pattern_txt->setPlainText(confs[rel_type][ParsersAttributes::SRC_COL_PATTERN]);
			dst_col_pattern_txt->setPlainText(confs[rel_type][ParsersAttributes::DST_COL_PATTERN]);
			pk_col_pattern_txt->setPlainText(confs[rel_type][ParsersAttributes::PK_COL_PATTERN]);
		}
		else
		{
			//Using the settings of the relatinship itself
			pk_pattern_txt->setPlainText(rel->getNamePattern(Relationship::PK_PATTERN));
			src_fk_pattern_txt->setPlainText(rel->getNamePattern(Relationship::SRC_FK_PATTERN));
			dst_fk_pattern_txt->setPlainText(rel->getNamePattern(Relationship::DST_FK_PATTERN));
			uq_pattern_txt->setPlainText(rel->getNamePattern(Relationship::UQ_PATTERN));
			src_col_pattern_txt->setPlainText(rel->getNamePattern(Relationship::SRC_COL_PATTERN));
			dst_col_pattern_txt->setPlainText(rel->getNamePattern(Relationship::DST_COL_PATTERN));
			pk_col_pattern_txt->setPlainText(rel->getNamePattern(Relationship::PK_COL_PATTERN));
		}
	}
}

void RelationshipWidget::listObjects(ObjectType obj_type)
{
	ObjectsTableWidget *tab=nullptr;
	Relationship *rel=nullptr;
	unsigned count, i;

	try
	{
		if(obj_type==OBJ_COLUMN)
			tab=attributes_tab;
		else
			tab=constraints_tab;

		rel=dynamic_cast<Relationship *>(this->object);

		tab->blockSignals(true);
		tab->removeRows();

		count=rel->getObjectCount(obj_type);
		for(i=0; i < count; i++)
		{
			tab->addRow();
			showObjectData(rel->getObject(i, obj_type), i);
		}
		tab->clearSelection();
		tab->blockSignals(false);

		constraints_tab->setButtonsEnabled(ObjectsTableWidget::ADD_BUTTON,
										   attributes_tab->getRowCount() > 0);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::listAdvancedObjects(void)
{
	BaseRelationship *base_rel=nullptr;
	Relationship *rel=nullptr;
	Table *tab=nullptr;
	vector<Column *> cols;
	vector<Constraint *> constrs;
	unsigned count=0, i,i1;

	try
	{
		base_rel=dynamic_cast<BaseRelationship *>(this->object);
		rel=dynamic_cast<Relationship *>(base_rel);

		advanced_objs_tab->blockSignals(true);
		advanced_objs_tab->removeRows();

		if(rel)
		{
			if(rel->getRelationshipType()!=BaseRelationship::RELATIONSHIP_NN)
			{
				cols=rel->getGeneratedColumns();
				count=cols.size();

				for(i=0; i < count; i++)
				{
					advanced_objs_tab->addRow();
					advanced_objs_tab->setCellText(cols[i]->getName(),i,0);
					advanced_objs_tab->setCellText(cols[i]->getTypeName(),i,1);
					advanced_objs_tab->setRowData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(cols[i])), i);
				}

				constrs=rel->getGeneratedConstraints();
				count=constrs.size();

				for(i=0, i1=advanced_objs_tab->getRowCount(); i < count; i++,i1++)
				{
					advanced_objs_tab->addRow();
					advanced_objs_tab->setCellText(constrs[i]->getName(),i1,0);
					advanced_objs_tab->setCellText(constrs[i]->getTypeName(),i1,1);
					advanced_objs_tab->setRowData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constrs[i])), i1);
				}
			}
			else
			{
				tab=rel->getGeneratedTable();
				if(tab)
				{
					advanced_objs_tab->addRow();
					advanced_objs_tab->setCellText(tab->getName(),0,0);
					advanced_objs_tab->setCellText(tab->getTypeName(),0,1);
					advanced_objs_tab->setRowData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(tab)), 0);
				}
			}
		}
		else if(base_rel->getRelationshipType()==BaseRelationship::RELATIONSHIP_FK)
		{
			Constraint *fk = base_rel->getReferenceForeignKey();

			if(fk)
			{
				int row = 0;

				advanced_objs_tab->addRow();
				row = advanced_objs_tab->getRowCount() - 1;

				advanced_objs_tab->setCellText(fk->getName(), row ,0);
				advanced_objs_tab->setCellText(fk->getTypeName(), row, 1);
				advanced_objs_tab->setRowData(QVariant::fromValue<void *>(fk), row);
			}
		}

		advanced_objs_tab->clearSelection();
		advanced_objs_tab->blockSignals(false);

	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::showAdvancedObject(int row)
{
	BaseObject *object=reinterpret_cast<BaseObject *>(advanced_objs_tab->getRowData(row).value<void *>());
	Table *tab=nullptr;
	Constraint *constr=nullptr;
	Column *col=nullptr;
	ObjectType obj_type=object->getObjectType();
	bool is_protected = false;

	if(obj_type==OBJ_COLUMN)
	{
		col=dynamic_cast<Column *>(object);
		is_protected = col->isProtected();
		openEditingForm<Column,ColumnWidget>(col, col->getParentTable());
	}
	else if(obj_type==OBJ_CONSTRAINT)
	{
		constr=dynamic_cast<Constraint *>(object);

		if(!constr->isAddedByRelationship())
		{
			is_protected = constr->isProtected();
			constr->setProtected(true);
		}

		openEditingForm<Constraint, ConstraintWidget>(constr, constr->getParentTable());

		if(!constr->isAddedByRelationship())
			constr->setProtected(is_protected);
	}
	else
	{
		TableWidget *table_wgt=new TableWidget;
		BaseForm editing_form(this);

		tab=dynamic_cast<Table *>(object);
		tab->setProtected(true);

		table_wgt->setAttributes(this->model, this->op_list, dynamic_cast<Schema *>(tab->getSchema()),
														 tab,	tab->getPosition().x(), tab->getPosition().y());

		editing_form.setMainWidget(table_wgt);

		GeneralConfigWidget::restoreWidgetGeometry(&editing_form, table_wgt->metaObject()->className());
		editing_form.exec();
		GeneralConfigWidget::saveWidgetGeometry(&editing_form, table_wgt->metaObject()->className());

		tab->setProtected(false);
	}
}

template<class Class, class WidgetClass>
int RelationshipWidget::openEditingForm(TableObject *object, BaseObject *parent)
{
	BaseForm editing_form(this);
	WidgetClass *object_wgt=new WidgetClass;
	BaseObject *parent_aux = nullptr;
	int res = 0;

	if(this->object->getObjectType() == BASE_RELATIONSHIP)
		parent_aux = dynamic_cast<BaseRelationship *>(this->object)->getTable(BaseRelationship::SRC_TABLE);
	else
		parent_aux = !parent ? this->object : parent;

	object_wgt->setAttributes(this->model, this->op_list, parent_aux, dynamic_cast<Class *>(object));
	editing_form.setMainWidget(object_wgt);

	GeneralConfigWidget::restoreWidgetGeometry(&editing_form, object_wgt->metaObject()->className());
	res = editing_form.exec();
	GeneralConfigWidget::saveWidgetGeometry(&editing_form, object_wgt->metaObject()->className());

	return(res);
}

void RelationshipWidget::addObject(void)
{
	ObjectType obj_type=BASE_OBJECT;

	try
	{
		if(sender()==attributes_tab)
		{
			obj_type=OBJ_COLUMN;
			tab=attributes_tab;
		}
		else
		{
			obj_type=OBJ_CONSTRAINT;
			tab=constraints_tab;
		}

		if(obj_type==OBJ_COLUMN)
			openEditingForm<Column,ColumnWidget>(nullptr);
		else
			openEditingForm<Constraint,ConstraintWidget>(nullptr);

		listObjects(obj_type);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::duplicateObject(int curr_row, int new_row)
{
	ObjectType obj_type=BASE_OBJECT;
	BaseObject *object = nullptr, *dup_object = nullptr;
	Relationship *rel = dynamic_cast<Relationship *>(this->object);
	vector<TableObject *> obj_list;
	ObjectsTableWidget *tab = nullptr;
	int op_id = -1;

	if(!rel)
		return;

	try
	{
		if(sender()==attributes_tab)
		{
			obj_type=OBJ_COLUMN;
			tab=attributes_tab;
			obj_list = rel->getAttributes();
		}
		else
		{
			obj_type=OBJ_CONSTRAINT;
			tab=constraints_tab;
			obj_list = rel->getConstraints();
		}

		//Gets the object reference if there is an item select on table
		if(curr_row >= 0)
			object = reinterpret_cast<BaseObject *>(tab->getRowData(curr_row).value<void *>());

		PgModelerNS::copyObject(&dup_object, object, obj_type);
		dup_object->setName(PgModelerNS::generateUniqueName(dup_object, obj_list, false, QString("_cp")));

		op_id=op_list->registerObject(dup_object, Operation::OBJECT_CREATED, new_row, rel);

		rel->addObject(dynamic_cast<TableObject *>(dup_object));
		listObjects(obj_type);
	}
	catch(Exception &e)
	{
		//If operation was registered
		if(op_id >= 0)
		{
			op_list->ignoreOperationChain(true);
			op_list->removeLastOperation();
			op_list->ignoreOperationChain(false);
		}

		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::editObject(int row)
{
	ObjectType obj_type=OBJ_COLUMN;
	TableObject *tab_obj=nullptr;

	try
	{
		op_list->ignoreOperationChain(true);

		if(sender()==attributes_tab)
		{
			obj_type=OBJ_COLUMN;
			tab_obj=reinterpret_cast<TableObject *>(attributes_tab->getRowData(row).value<void *>());
			openEditingForm<Column,ColumnWidget>(tab_obj);
		}
		else
		{
			obj_type=OBJ_CONSTRAINT;
			tab_obj=reinterpret_cast<TableObject *>(constraints_tab->getRowData(row).value<void *>());
			openEditingForm<Constraint,ConstraintWidget>(tab_obj);
		}

		listObjects(obj_type);
		op_list->ignoreOperationChain(false);
	}
	catch(Exception &e)
	{
		listObjects(obj_type);
		op_list->ignoreOperationChain(false);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::showObjectData(TableObject *object, int row)
{
	ObjectsTableWidget *tab=nullptr;

	if(object->getObjectType()==OBJ_COLUMN)
	{
		tab=attributes_tab;
		attributes_tab->setCellText(*dynamic_cast<Column *>(object)->getType(),row,1);
	}
	else
	{
		tab=constraints_tab;
		constraints_tab->setCellText(~dynamic_cast<Constraint *>(object)->getConstraintType(),row,1);
	}

	tab->setCellText(object->getName(),row,0);
	tab->setRowData(QVariant::fromValue<void *>(object), row);
}

void RelationshipWidget::removeObjects(void)
{
	Relationship *rel=nullptr;
	ObjectType obj_type=BASE_OBJECT;
	unsigned count, op_count=0, i;
	TableObject *object=nullptr;

	try
	{
		rel=dynamic_cast<Relationship *>(this->object);

		if(sender()==attributes_tab)
		{
			obj_type=OBJ_COLUMN;
			count=rel->getAttributeCount();
		}
		else
		{
			obj_type=OBJ_CONSTRAINT;
			count=rel->getConstraintCount();
		}


		op_count=op_list->getCurrentSize();

		for(i=0; i < count; i++)
		{
			object=rel->getObject(0, obj_type);
			op_list->registerObject(object, Operation::OBJECT_REMOVED, 0, rel);
			rel->removeObject(object);
		}

		if(obj_type==OBJ_COLUMN)
			listSpecialPkColumns();
	}
	catch(Exception &e)
	{
		if(op_count < op_list->getCurrentSize())
		{
			count=op_list->getCurrentSize()-op_count;
			op_list->ignoreOperationChain(true);

			for(i=0; i < count; i++)
			{
				op_list->undoOperation();
				op_list->removeLastOperation();
			}

			op_list->ignoreOperationChain(false);
		}

		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::removeObject(int row)
{
	Relationship *rel=nullptr;
	ObjectType obj_type=BASE_OBJECT;
	TableObject *object=nullptr;
	int op_id=-1;

	try
	{
		rel=dynamic_cast<Relationship *>(this->object);

		if(sender()==attributes_tab)
			obj_type=OBJ_COLUMN;
		else
			obj_type=OBJ_CONSTRAINT;

		object=rel->getObject(row, obj_type);
		op_id=op_list->registerObject(object, Operation::OBJECT_REMOVED, 0, rel);
		rel->removeObject(object);

		if(obj_type==OBJ_COLUMN)
			listSpecialPkColumns();
	}
	catch(Exception &e)
	{
		//If operation was registered
		if(op_id >= 0)
		{
			op_list->ignoreOperationChain(true);
			op_list->removeLastOperation();
			op_list->ignoreOperationChain(false);
		}

		listObjects(obj_type);
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::selectCopyOptions(void)
{
	all_chk->setEnabled(!defaults_rb->isChecked());

	defaults_chk->setEnabled(!all_chk->isChecked() && !defaults_rb->isChecked());
	constraints_chk->setEnabled(!all_chk->isChecked() && !defaults_rb->isChecked());
	storage_chk->setEnabled(!all_chk->isChecked() && !defaults_rb->isChecked());
	comments_chk->setEnabled(!all_chk->isChecked() && !defaults_rb->isChecked());
	indexes_chk->setEnabled(!all_chk->isChecked() && !defaults_rb->isChecked());
	identity_chk->setEnabled(!all_chk->isChecked() && !defaults_rb->isChecked());
	statistics_chk->setEnabled(!all_chk->isChecked() && !defaults_rb->isChecked());

	if(all_chk->isChecked() || defaults_rb->isChecked())
	{
		if(defaults_rb->isChecked())
			all_chk->setChecked(false);

		defaults_chk->setChecked(false);
		constraints_chk->setChecked(false);
		storage_chk->setChecked(false);
		comments_chk->setChecked(false);
		indexes_chk->setChecked(false);
		identity_chk->setChecked(false);
		statistics_chk->setChecked(false);
	}
}

void RelationshipWidget::listSpecialPkColumns(void)
{
	Relationship *aux_rel=dynamic_cast<Relationship *>(this->object);

	if(aux_rel)
	{
		vector<Column *> cols;
		vector<unsigned> col_ids;
		int count, idx;
		QListWidgetItem *item=nullptr;

		rel_columns_lst->clear();

		if(aux_rel->getRelationshipType()!=BaseRelationship::RELATIONSHIP_NN)
			cols=aux_rel->getGeneratedColumns();

		for(auto &attrib : aux_rel->getAttributes())
			cols.push_back(dynamic_cast<Column *>(attrib));

		//Get the special primary key columns ids
		col_ids=aux_rel->getSpecialPrimaryKeyCols();

		count=cols.size();
		for(idx=0; idx < count; idx++)
		{
			rel_columns_lst->addItem(cols[idx]->getName().toUtf8() +
									 QString(" (") + *cols[idx]->getType() + QString(")"));
			item=rel_columns_lst->item(idx);
			item->setCheckState(Qt::Unchecked);
		}

		count=col_ids.size();
		for(idx=0; idx < count; idx++)
		{
			if(col_ids[idx] < static_cast<unsigned>(rel_columns_lst->count()))
				rel_columns_lst->item(col_ids[idx])->setCheckState(Qt::Checked);
		}
	}
}

void RelationshipWidget::applyConfiguration(void)
{
	try
	{
		Relationship *rel=nullptr;
		BaseRelationship *base_rel=dynamic_cast<BaseRelationship *>(this->object);
		unsigned rel_type, count, i, copy_mode=0, copy_ops=0;
		vector<unsigned> col_ids;

		/* Due to the complexity of the Relationship class and the strong link between all
		 the relationships on the model is necessary to store the XML of the special objects
		 and disconnect all relationships, edit the relationshi and revalidate all the relationships again */
		if(this->object->getObjectType()==OBJ_RELATIONSHIP)
		{
			model->storeSpecialObjectsXML();
			model->disconnectRelationships();
		}

		if(!this->new_object && this->object->getObjectType()==OBJ_RELATIONSHIP)
			op_list->registerObject(this->object, Operation::OBJECT_MODIFIED);
		else
			registerNewObject();

		BaseObjectWidget::applyConfiguration();

		if(custom_color_chk->isChecked())
			base_rel->setCustomColor(color_picker->getColor(0));
		else
			base_rel->setCustomColor(Qt::transparent);

		if(this->object->getObjectType()==OBJ_RELATIONSHIP)
		{
			QPlainTextEdit *pattern_fields[]={ src_col_pattern_txt, dst_col_pattern_txt,
																				 src_fk_pattern_txt, dst_fk_pattern_txt,
																				 pk_pattern_txt, uq_pattern_txt, pk_col_pattern_txt };
			unsigned pattern_ids[]= { Relationship::SRC_COL_PATTERN, Relationship::DST_COL_PATTERN,
																Relationship::SRC_FK_PATTERN, Relationship::DST_FK_PATTERN,
																Relationship::PK_PATTERN, Relationship::UQ_PATTERN, Relationship::PK_COL_PATTERN };

			rel=dynamic_cast<Relationship *>(base_rel);

			if(name_patterns_grp->isVisible())
			{
				count=sizeof(pattern_ids)/sizeof(unsigned);
				for(i=0; i < count; i++)
					rel->setNamePattern(pattern_ids[i], pattern_fields[i]->toPlainText());
			}

			rel_type=rel->getRelationshipType();
			rel->blockSignals(true);

			if(!defaults_rb->isChecked())
			{
				if(including_rb->isChecked())
					copy_mode=CopyOptions::INCLUDING;
				else
					copy_mode=CopyOptions::EXCLUDING;

				copy_ops+=(all_chk->isChecked() ? CopyOptions::ALL : 0);
				copy_ops+=(defaults_chk->isChecked() ? CopyOptions::DEFAULTS : 0);
				copy_ops+=(constraints_chk->isChecked() ? CopyOptions::CONSTRAINTS : 0);
				copy_ops+=(comments_chk->isChecked() ? CopyOptions::COMMENTS : 0);
				copy_ops+=(indexes_chk->isChecked() ? CopyOptions::INDEXES : 0);
				copy_ops+=(storage_chk->isChecked() ? CopyOptions::STORAGE : 0);
				copy_ops+=(identity_chk->isChecked() ? CopyOptions::IDENTITY : 0);
				copy_ops+=(statistics_chk->isChecked() ? CopyOptions::STATISTICS : 0);
			}

			rel->setCopyOptions(CopyOptions(copy_mode, copy_ops));
			rel->setMandatoryTable(BaseRelationship::SRC_TABLE, false);
			rel->setMandatoryTable(BaseRelationship::DST_TABLE, false);

			if(table1_mand_chk->isEnabled())
				rel->setMandatoryTable(BaseRelationship::SRC_TABLE, table1_mand_chk->isChecked());

			if(table2_mand_chk->isEnabled())
				rel->setMandatoryTable(BaseRelationship::DST_TABLE, table2_mand_chk->isChecked());

			if(rel_type==BaseRelationship::RELATIONSHIP_1N ||
					rel_type==BaseRelationship::RELATIONSHIP_11)
				rel->setIdentifier(identifier_chk->isChecked());
			else if(rel_type==BaseRelationship::RELATIONSHIP_NN)
				rel->setTableNameRelNN(relnn_tab_name_edt->text());

			if(rel_type==BaseRelationship::RELATIONSHIP_1N ||
					rel_type==BaseRelationship::RELATIONSHIP_11 ||
					rel_type==BaseRelationship::RELATIONSHIP_NN)
			{
				rel->setDeferrable(deferrable_chk->isChecked());
				rel->setDeferralType(DeferralType(deferral_cmb->currentText()));
				rel->setActionType((del_action_cmb->currentIndex()!=0 ? ActionType(del_action_cmb->currentText()) : ActionType::null), Constraint::DELETE_ACTION);
				rel->setActionType((upd_action_cmb->currentIndex()!=0 ? ActionType(upd_action_cmb->currentText()) : ActionType::null), Constraint::UPDATE_ACTION);

				if(rel_type==BaseRelationship::RELATIONSHIP_NN)
					rel->setSiglePKColumn(single_pk_chk->isChecked());
			}

			count=rel_columns_lst->count();
			for(i=0; i < count; i++)
			{
				if(rel_columns_lst->item(i)->checkState()==Qt::Checked)
					col_ids.push_back(i);
			}

			rel->setSpecialPrimaryKeyCols(col_ids);

			try
			{
				//Checking if there is relationship redundancy
				if(rel_type==BaseRelationship::RELATIONSHIP_DEP ||
						rel_type==BaseRelationship::RELATIONSHIP_GEN ||
						rel->isIdentifier())
					model->checkRelationshipRedundancy(rel);

				if(rel_type!=BaseRelationship::RELATIONSHIP_FK)
					model->validateRelationships();

				rel->blockSignals(false);
			}
			catch(Exception &e)
			{
				Messagebox msg_box;

				if(e.getErrorType()==ERR_INVALIDATED_OBJECTS)
					msg_box.show(e);
				else
					throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			}
		}

		op_list->finishOperationChain();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		model->validateRelationships();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipWidget::cancelConfiguration(void)
{
	BaseObjectWidget::cancelChainedOperation();
}
