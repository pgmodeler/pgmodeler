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

#include "baseform.h"
#include "modelwidget.h"
#include "sourcecodewidget.h"
#include "databasewidget.h"
#include "schemawidget.h"
#include "rolewidget.h"
#include "tablespacewidget.h"
#include "languagewidget.h"
#include "functionwidget.h"
#include "castwidget.h"
#include "conversionwidget.h"
#include "domainwidget.h"
#include "aggregatewidget.h"
#include "sequencewidget.h"
#include "operatorwidget.h"
#include "operatorfamilywidget.h"
#include "operatorclasswidget.h"
#include "typewidget.h"
#include "viewwidget.h"
#include "textboxwidget.h"
#include "columnwidget.h"
#include "constraintwidget.h"
#include "rulewidget.h"
#include "triggerwidget.h"
#include "indexwidget.h"
#include "relationshipwidget.h"
#include "tablewidget.h"
#include "taskprogresswidget.h"
#include "objectdepsrefswidget.h"
#include "objectrenamewidget.h"
#include "permissionwidget.h"
#include "collationwidget.h"
#include "extensionwidget.h"
#include "customsqlwidget.h"
#include "tagwidget.h"
#include "eventtriggerwidget.h"
#include "pgmodeleruins.h"
#include "swapobjectsidswidget.h"
#include "genericsqlwidget.h"
#include "policywidget.h"
#include "tabledatawidget.h"
#include "generalconfigwidget.h"
#include "foreigndatawrapperwidget.h"
#include "foreignserverwidget.h"
#include "usermappingwidget.h"

vector<BaseObject *> ModelWidget::copied_objects;
vector<BaseObject *> ModelWidget::cutted_objects;
bool ModelWidget::cut_operation=false;
bool ModelWidget::save_restore_pos=true;
bool ModelWidget::disable_render_smooth=false;
bool ModelWidget::simple_obj_creation=true;
ModelWidget *ModelWidget::src_model=nullptr;
double ModelWidget::min_object_opacity=0.10;

constexpr unsigned ModelWidget::BreakVertNinetyDegrees;
constexpr unsigned ModelWidget::BreakHorizNinetyDegrees;
constexpr unsigned ModelWidget::BreakVert2NinetyDegrees;
constexpr unsigned ModelWidget::BreakHoriz2NinetyDegrees;

ModelWidget::ModelWidget(QWidget *parent) : QWidget(parent)
{
	QFont font;
	QLabel *label=nullptr;
	QGridLayout *grid=nullptr;
	QAction *action=nullptr;
	QString str_ico;
	QStringList rel_types_cod={QString("11"), QString("1n"), QString("nn"), QString("dep"), QString("gen"), QString("part") };
	unsigned i,
			rel_types_id[]={ BaseRelationship::Relationship11, BaseRelationship::Relationship1n,
							 BaseRelationship::RelationshipNn, BaseRelationship::RelationshipDep,
							 BaseRelationship::RelationshipGen, BaseRelationship::RelationshipPart};

	vector<ObjectType> types_vect = BaseObject::getObjectTypes(true, { ObjectType::Database, ObjectType::Permission,
																																		 ObjectType::BaseRelationship});

	current_zoom=1;
	modified=panning_mode=false;
	new_obj_type=ObjectType::BaseObject;

	//Generating a temporary file name for the model
	QTemporaryFile tmp_file;

	//Configuring the template mask which includes the full path to temporary dir
	tmp_file.setFileTemplate(GlobalAttributes::getTemporaryFilePath("model_XXXXXX.dbm"));
	tmp_file.open();
	tmp_filename=tmp_file.fileName();
	tmp_file.close();

	protected_model_frm=new QFrame(this);
	protected_model_frm->setGeometry(QRect(20, 10, 500, 25));
	protected_model_frm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	protected_model_frm->setMinimumSize(QSize(0, 25));
	protected_model_frm->setFrameShape(QFrame::StyledPanel);
	protected_model_frm->setFrameShadow(QFrame::Raised);
	protected_model_frm->setVisible(false);

	label=new QLabel(protected_model_frm);
	label->setMinimumSize(QSize(20, 20));
	label->setMaximumSize(QSize(20, 20));
	label->setScaledContents(true);
	label->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")));

	grid=new QGridLayout;
	grid->addWidget(label, 0, 0, 1, 1);

	label=new QLabel(protected_model_frm);

	font.setBold(false);
	font.setItalic(false);
	font.setUnderline(false);
	font.setWeight(50);
	font.setStrikeOut(false);
	font.setKerning(true);
	label->setFont(font);
	label->setWordWrap(true);
	label->setText(tr("<strong>ATTENTION:</strong> The database model is protected! Operations that could modify it are disabled!"));
	PgModelerUiNs::configureWidgetFont(label, PgModelerUiNs::MediumFontFactor);

	grid->addWidget(label, 0, 1, 1, 1);
	protected_model_frm->setLayout(grid);
	protected_model_frm->adjustSize();

	db_model=new DatabaseModel(this);
	xmlparser=db_model->getXMLParser();
	op_list=new OperationList(db_model);
	scene=new ObjectsScene;
	scene->setSceneRect(QRectF(0,0,2000,2000));
	scene->installEventFilter(this);

	viewport=new QGraphicsView(scene);
	updateRenderHints();
	viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//Force the scene to be drawn from the left to right and from top to bottom
	viewport->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	viewport->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
	viewport->setCacheMode(QGraphicsView::CacheBackground);
	viewport->centerOn(0,0);
	viewport->setMouseTracking(true);

	grid=new QGridLayout;
	grid->addWidget(protected_model_frm, 0,0,1,1);
	grid->addWidget(viewport, 1,0,1,1);
	this->setLayout(grid);

	magnifier_frm = new QFrame(this);
	magnifier_frm->setVisible(false);
	magnifier_frm->installEventFilter(this);
	magnifier_frm->setMouseTracking(true);
	magnifier_frm->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	magnifier_frm->setGeometry(0,0,
														500 * BaseObjectView::getFontFactor() * BaseObjectView::getScreenDpiFactor(),
														500 * BaseObjectView::getFontFactor() * BaseObjectView::getScreenDpiFactor());
	magnifier_frm->setCursor(Qt::CrossCursor);

	QColor c1, c2;
	BaseObjectView::getFillStyle(Attributes::ObjSelection, c1, c2);
	c1.setAlpha(50);
	magnifier_frm->setStyleSheet(QString("background-color: %1; border: 1px solid %2;")
															 .arg(c1.name(QColor::HexArgb))
															 .arg(BaseObjectView::getBorderStyle(Attributes::ObjSelection).color().name(QColor::HexArgb)));

	magnifier_area_lbl = new QLabel(this);
	magnifier_area_lbl->raise();
	magnifier_area_lbl->setAutoFillBackground(false);
	magnifier_area_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	magnifier_area_lbl->setStyleSheet(QString("background-color: #C8f0f0f0;\
										border: 1px solid #C8808080;"));
	magnifier_area_lbl->setVisible(false);
	magnifier_area_lbl->setGeometry(magnifier_frm->geometry());
	magnifier_area_lbl->setCursor(Qt::BlankCursor);
	magnifier_area_lbl->installEventFilter(this);
	magnifier_area_lbl->setMouseTracking(true);

	zoom_info_lbl=new QLabel(this);
	zoom_info_lbl->raise();
	zoom_info_lbl->setAutoFillBackground(false);
	zoom_info_lbl->setText(QString("Zoom: 100%"));
	zoom_info_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	zoom_info_lbl->setStyleSheet(QString("color: #C8000000; \
								 background-color: #C8FFFFa0;\
								 border: 1px solid #C8B16380;"));

	font=zoom_info_lbl->font();
	font.setBold(true);
	font.setPointSizeF(12);
	zoom_info_lbl->setFont(font);
	zoom_info_lbl->adjustSize();
	zoom_info_lbl->setVisible(false);
	zoom_info_timer.setInterval(3000);

	action_edit_data=new QAction(QIcon(PgModelerUiNs::getIconPath("editdata")), tr("Edit data"), this);

	action_source_code=new QAction(QIcon(PgModelerUiNs::getIconPath("codigosql")), tr("Source"), this);
	action_source_code->setShortcut(QKeySequence(tr("Alt+S")));
	action_source_code->setToolTip(tr("Show object source code"));

	action_edit=new QAction(QIcon(PgModelerUiNs::getIconPath("editar")), tr("Properties"), this);
	action_edit->setShortcut(QKeySequence(tr("Space")));
	action_edit->setToolTip(tr("Edit the object properties"));

	action_protect=new QAction(QIcon(PgModelerUiNs::getIconPath("bloqobjeto")), tr("Protect"), this);
	action_unprotect=new QAction(QIcon(PgModelerUiNs::getIconPath("desbloqobjeto")), tr("Unprotect"), this);
	action_protect->setToolTip(tr("Protects object(s) from modifications"));

	action_remove=new QAction(QIcon(PgModelerUiNs::getIconPath("excluir")), tr("Delete"), this);
	action_remove->setShortcut(QKeySequence(tr("Del")));
	action_remove->setMenuRole(QAction::NoRole);

	action_cascade_del=new QAction(QIcon(PgModelerUiNs::getIconPath("delcascade")), tr("Del. cascade"), this);
	action_cascade_del->setShortcut(QKeySequence(tr("Shift+Del")));
	action_cascade_del->setMenuRole(QAction::NoRole);

	action_select_all=new QAction(QIcon(PgModelerUiNs::getIconPath("seltodos")), tr("Select all"), this);
	action_select_all->setToolTip(tr("Selects all the graphical objects in the model"));
	action_select_all->setMenu(&select_all_menu);

	action_convert_relnn=new QAction(QIcon(PgModelerUiNs::getIconPath("convrelnn")), tr("Convert"), this);
	action_convert_rel1n=new QAction(QIcon(PgModelerUiNs::getIconPath("convrel1n")), tr("Convert"), this);

	action_copy=new QAction(QIcon(PgModelerUiNs::getIconPath("copiar")), tr("Copy"), this);
	action_copy->setShortcut(QKeySequence(tr("Ctrl+C")));
	action_copy->setMenuRole(QAction::NoRole);

	action_paste=new QAction(QIcon(PgModelerUiNs::getIconPath("colar")), tr("Paste"), this);
	action_paste->setShortcut(QKeySequence(tr("Ctrl+V")));
	action_paste->setMenuRole(QAction::NoRole);

	action_cut=new QAction(QIcon(PgModelerUiNs::getIconPath("recortar")), tr("Cut"), this);
	action_cut->setShortcut(QKeySequence(tr("Ctrl+X")));
	action_cut->setMenuRole(QAction::NoRole);

	action_deps_refs=new QAction(QIcon(PgModelerUiNs::getIconPath("depsrefs")), tr("Deps && Referrers"), this);

	action_new_object=new QAction(QIcon(PgModelerUiNs::getIconPath("novoobjeto")), tr("New"), this);
	action_new_object->setToolTip(tr("Add a new object in the model"));

	action_quick_actions=new QAction(QIcon(PgModelerUiNs::getIconPath("quickactions")), tr("Quick"), this);
	action_quick_actions->setToolTip(tr("Quick action for the selected object"));
	action_quick_actions->setMenu(&quick_actions_menu);

	action_rename=new QAction(QIcon(PgModelerUiNs::getIconPath("rename")), tr("Rename"), this);
	action_rename->setShortcut(QKeySequence(tr("F2")));
	action_rename->setToolTip(tr("Quick rename the object"));

	action_moveto_schema=new QAction(QIcon(PgModelerUiNs::getIconPath("movetoschema")), tr("Move to schema"), this);
	action_moveto_schema->setMenu(&schemas_menu);

	action_moveto_layer=new QAction(QIcon(PgModelerUiNs::getIconPath("movetolayer")), tr("Move to layer"), this);
	action_moveto_layer->setMenu(&layers_menu);

	action_set_tag=new QAction(QIcon(PgModelerUiNs::getIconPath("tag")), tr("Set tag"), this);
	action_set_tag->setMenu(&tags_menu);

	action_edit_perms=new QAction(QIcon(PgModelerUiNs::getIconPath("permission")), tr("Edit permissions"), this);
	action_edit_perms->setShortcut(QKeySequence(tr("Ctrl+E")));

	action_change_owner=new QAction(QIcon(PgModelerUiNs::getIconPath("changeowner")), tr("Change owner"), this);
	action_change_owner->setMenu(&owners_menu);

	action_sel_sch_children=new QAction(QIcon(PgModelerUiNs::getIconPath("seltodos")), tr("Select children"), this);
	action_sel_tagged_tabs=new QAction(QIcon(PgModelerUiNs::getIconPath("seltodos")), tr("Select tagged"), this);

	action_select_object=new QAction(QIcon(PgModelerUiNs::getIconPath("movimentado")), tr("Select"), this);
	action_parent_rel=new QAction(QIcon(PgModelerUiNs::getIconPath("relationship")), tr("Open relationship"), this);

	action_append_sql=new QAction(QIcon(PgModelerUiNs::getIconPath("sqlappend")), tr("Custom SQL"), this);
	action_append_sql->setShortcut(QKeySequence(tr("Alt+Q")));

	action_create_seq_col=new QAction(QIcon(PgModelerUiNs::getIconPath("sequence")), tr("Convert to sequence"), this);
	action_conv_int_serial=new QAction(QIcon(PgModelerUiNs::getIconPath("sequence")), tr("Convert to serial"), this);

	action_break_rel_line=new QAction(QIcon(PgModelerUiNs::getIconPath("breakrelline")), tr("Break line"), this);

	action_remove_rel_points=new QAction(QIcon(PgModelerUiNs::getIconPath("removepoints")), tr("Remove points"), this);

	action_enable_sql=new QAction(QIcon(PgModelerUiNs::getIconPath("codigosql")), tr("Enable SQL"), this);
	action_disable_sql=new QAction(QIcon(PgModelerUiNs::getIconPath("disablesql")), tr("Disable SQL"), this);

	action_duplicate=new QAction(QIcon(PgModelerUiNs::getIconPath("duplicate")), tr("Duplicate"), this);
	action_duplicate->setShortcut(QKeySequence(tr("Ctrl+D")));
	action_duplicate->setMenuRole(QAction::NoRole);

	action_pagination=new QAction(QIcon(PgModelerUiNs::getIconPath("pagination")), tr("Pagination"), this);
	action_pagination->setMenu(&pagination_menu);

	action = pagination_menu.addAction(tr("Enable"));
	action->setData(true);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(togglePagination()));

	action = pagination_menu.addAction(tr("Disable"));
	action->setData(false);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(togglePagination()));

	action_collapse_mode=new QAction(QIcon(PgModelerUiNs::getIconPath("collapse")), tr("Collapse"), this);
	action_no_collapse_attribs=new QAction(tr("Not collapsed"), this);
	action_no_collapse_attribs->setData(enum_cast(CollapseMode::NotCollapsed));
	action_collapse_ext_attribs=new QAction(tr("Extended attributes"), this);
	action_collapse_ext_attribs->setData(enum_cast(CollapseMode::ExtAttribsCollapsed));
	action_collpase_all_attribs=new QAction(tr("All attributes"), this);
	action_collpase_all_attribs->setData(enum_cast(CollapseMode::AllAttribsCollapsed));

	action_jump_to_table=new QAction(QIcon(PgModelerUiNs::getIconPath("jumptotable")), tr("Jump to table"), this);
	action_jump_to_table->setMenu(&jump_to_tab_menu);

	toggle_attrs_menu.addAction(action_no_collapse_attribs);
	toggle_attrs_menu.addAction(action_collapse_ext_attribs);
	toggle_attrs_menu.addAction(action_collpase_all_attribs);
	action_collapse_mode->setMenu(&toggle_attrs_menu);

	action_schemas_rects=new QAction(QIcon(PgModelerUiNs::getIconPath("schemarect")), tr("Schemas rectangles"), this);
	action_show_schemas_rects=new QAction(tr("Show"), this);
	action_hide_schemas_rects=new QAction(tr("Hide"), this);
	toggle_sch_rects_menu.addAction(action_show_schemas_rects);
	toggle_sch_rects_menu.addAction(action_hide_schemas_rects);
	action_schemas_rects->setMenu(&toggle_sch_rects_menu);

	action_fade=new QAction(QIcon(PgModelerUiNs::getIconPath("fade")), tr("Fade in/out"), this);
	action_fade_in=new QAction(QIcon(PgModelerUiNs::getIconPath("fadein")), tr("Fade in"), this);
	action_fade_out=new QAction(QIcon(PgModelerUiNs::getIconPath("fadeout")), tr("Fade out"), this);

	action_fade_rels_in=new QAction(QIcon(PgModelerUiNs::getIconPath("fadein")), tr("Fade in"), this);
	action_fade_rels_out=new QAction(QIcon(PgModelerUiNs::getIconPath("fadeout")), tr("Fade out"), this);

	fade_rels_menu.addAction(action_fade_rels_in);
	fade_rels_menu.addAction(action_fade_rels_out);

	action_fade_rels=new QAction(QIcon(PgModelerUiNs::getIconPath("relationship_grp")), tr("Relationships"), this);
	action_fade_rels->setMenu(&fade_rels_menu);

	action_fade->setMenu(&fade_menu);
	action_fade_in->setMenu(&fade_in_menu);
	action_fade_out->setMenu(&fade_out_menu);

	action_edit_creation_order=new QAction(QIcon(PgModelerUiNs::getIconPath("swapobjs")), tr("Swap ids"), this);
	action_edit_creation_order->setToolTip(tr("Edit the objects creation order by swapping their ids"));
	connect(action_edit_creation_order, SIGNAL(triggered(bool)), this, SLOT(swapObjectsIds()));

	action=new QAction(QIcon(PgModelerUiNs::getIconPath("breakline_90dv")), tr("90° (vertical)"), this);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(breakRelationshipLine()));
	action->setData(QVariant::fromValue<unsigned>(BreakVertNinetyDegrees));
	break_rel_menu.addAction(action);

	action=new QAction(QIcon(PgModelerUiNs::getIconPath("breakline_90dh")), tr("90° (horizontal)"), this);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(breakRelationshipLine()));
	action->setData(QVariant::fromValue<unsigned>(BreakHorizNinetyDegrees));
	break_rel_menu.addAction(action);

	action=new QAction(QIcon(PgModelerUiNs::getIconPath("breakline_290dv")), tr("90° + 90° (vertical)"), this);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(breakRelationshipLine()));
	action->setData(QVariant::fromValue<unsigned>(BreakVert2NinetyDegrees));
	break_rel_menu.addAction(action);

	action=new QAction(QIcon(PgModelerUiNs::getIconPath("breakline_290dh")), tr("90° + 90° (horizontal)"), this);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(breakRelationshipLine()));
	action->setData(QVariant::fromValue<unsigned>(BreakHoriz2NinetyDegrees));
	break_rel_menu.addAction(action);

	action_break_rel_line->setMenu(&break_rel_menu);

	//Alocatting the object creation actions
	for(auto &type : types_vect)
	{
		actions_new_objects[type]=new QAction(QIcon(PgModelerUiNs::getIconPath(type)), BaseObject::getTypeName(type), this);
		actions_new_objects[type]->setData(QVariant(enum_cast(type)));
		connect(actions_new_objects[type], SIGNAL(triggered(bool)), this, SLOT(addNewObject()));
	}

	// Configuring the submenu of database level objects
	action_database_category = new QAction(QIcon(PgModelerUiNs::getIconPath(ObjectType::Database)), tr("Database object"), this);
	action_database_category->setMenu(&database_category_menu);
	types_vect = BaseObject::getChildObjectTypes(ObjectType::Database);

	for(auto &type : types_vect)
		database_category_menu.addAction(actions_new_objects[type]);

	// Configuring the submenu of schema level objects
	action_schema_category = new QAction(QIcon(PgModelerUiNs::getIconPath(ObjectType::Schema)), tr("Schema object"), this);
	action_schema_category->setMenu(&schema_category_menu);
	types_vect = BaseObject::getChildObjectTypes(ObjectType::Schema);

	for(auto &type : types_vect)
		schema_category_menu.addAction(actions_new_objects[type]);

	//Creating the relationship submenu
	rels_menu=new QMenu(this);
	actions_new_objects[ObjectType::Relationship]->setMenu(rels_menu);

	for(int i=0; i < rel_types_cod.size(); i++)
	{
		str_ico=BaseObject::getSchemaName(ObjectType::Relationship) + rel_types_cod[i];

		action=new QAction(QIcon(PgModelerUiNs::getIconPath(str_ico)),
						   BaseRelationship::getRelationshipTypeName(rel_types_id[i], false), this);

		//Storing a unique identifier for the relationship type
		action->setData(QVariant(enum_cast(ObjectType::Relationship) + rel_types_id[i]));

		connect(action, SIGNAL(triggered(bool)), this, SLOT(addNewObject()));
		rels_menu->addAction(action);
	}

	new_obj_overlay_wgt=new NewObjectOverlayWidget(this);
	new_obj_overlay_wgt->setObjectName(QString("new_obj_overlay_wgt"));
	new_obj_overlay_wgt->setVisible(false);

	vector<ObjectType> graph_types = { ObjectType::BaseObject, ObjectType::Schema, ObjectType::Table, ObjectType::ForeignTable,
																		 ObjectType::View, ObjectType::Relationship, ObjectType::Textbox };
	QStringList labels = { tr("All objects"), tr("Schemas"), tr("Tables"), tr("Foreign Tables"),
												 tr("Views"), tr("Relationships"), tr("Textboxes") };

	i=0;
	for(auto &obj_type : graph_types)
	{
		if(obj_type == ObjectType::BaseObject)
		{
			action=new QAction(labels[i++], this);
			action->setShortcut(QKeySequence(tr("Ctrl+A")));
			select_all_menu.addAction(action);
			select_all_menu.addSeparator();
		}
		else
		{
			action=new QAction(QIcon(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(obj_type) + QString("_grp"))), labels[i++], this);
			select_all_menu.addAction(action);
		}

		action->setData(QVariant(enum_cast(obj_type)));
		connect(action, SIGNAL(triggered(bool)), this, SLOT(selectAllObjects()));
	}

	action_stacking=new QAction(QIcon(PgModelerUiNs::getIconPath("stacking")), tr("Stacking"), this);
	action_send_to_back=new QAction(QIcon(PgModelerUiNs::getIconPath("sendtoback")), tr("Send to back"), this);
	action_bring_to_front=new QAction(QIcon(PgModelerUiNs::getIconPath("bringtofront")), tr("Bring to front"), this);
	stacking_menu.addAction(action_send_to_back);
	stacking_menu.addAction(action_bring_to_front);
	action_stacking->setMenu(&stacking_menu);

	connect(action_send_to_back, SIGNAL(triggered(bool)), this, SLOT(sendToBack()));
	connect(action_bring_to_front, SIGNAL(triggered(bool)), this, SLOT(bringToFront()));

	connect(action_edit_data, SIGNAL(triggered(bool)), this, SLOT(editTableData()));
	connect(&zoom_info_timer, SIGNAL(timeout()), zoom_info_lbl, SLOT(hide()));
	connect(action_source_code, SIGNAL(triggered(bool)), this, SLOT(showSourceCode()));
	connect(action_edit, SIGNAL(triggered(bool)),this,SLOT(editObject()));
	connect(action_protect, SIGNAL(triggered(bool)),this,SLOT(protectObject()));
	connect(action_unprotect, SIGNAL(triggered(bool)),this,SLOT(protectObject()));
	connect(action_select_all, SIGNAL(triggered(bool)),this,SLOT(selectAllObjects()));
	connect(action_convert_relnn, SIGNAL(triggered(bool)), this, SLOT(convertRelationshipNN()));
	connect(action_convert_rel1n, SIGNAL(triggered(bool)), this, SLOT(convertRelationship1N()));
	connect(action_deps_refs, SIGNAL(triggered(bool)), this, SLOT(showDependenciesReferences()));
	connect(action_copy, SIGNAL(triggered(bool)),this,SLOT(copyObjects()));
	connect(action_paste, SIGNAL(triggered(bool)),this,SLOT(pasteObjects()));
	connect(action_cut, SIGNAL(triggered(bool)),this,SLOT(cutObjects()));
	connect(action_duplicate, SIGNAL(triggered(bool)),this,SLOT(duplicateObject()));
	connect(action_rename, SIGNAL(triggered(bool)), this, SLOT(renameObjects()));
	connect(action_edit_perms, SIGNAL(triggered(bool)), this, SLOT(editPermissions()));
	connect(action_sel_sch_children, SIGNAL(triggered(bool)), this, SLOT(selectSchemaChildren()));
	connect(action_sel_tagged_tabs, SIGNAL(triggered(bool)), this, SLOT(selectTaggedTables()));
	connect(action_select_object, SIGNAL(triggered(bool)), this, SLOT(highlightObject()));
	connect(action_parent_rel, SIGNAL(triggered(bool)), this, SLOT(editObject()));
	connect(action_append_sql, SIGNAL(triggered(bool)), this, SLOT(editCustomSQL()));
	connect(action_create_seq_col, SIGNAL(triggered(bool)), this, SLOT(createSequenceFromColumn()));
	connect(action_conv_int_serial, SIGNAL(triggered(bool)), this, SLOT(convertIntegerToSerial()));
	connect(action_remove_rel_points, SIGNAL(triggered(bool)), this, SLOT(removeRelationshipPoints()));
	connect(action_enable_sql, SIGNAL(triggered(bool)), this, SLOT(toggleObjectSQL()));
	connect(action_disable_sql, SIGNAL(triggered(bool)), this, SLOT(toggleObjectSQL()));
	connect(action_remove, &QAction::triggered, [&](){ removeObjects(false); });
	connect(action_cascade_del, &QAction::triggered, [&](){ removeObjects(true); });
	connect(action_fade_in, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsIn()));
	connect(action_fade_out, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsOut()));
	connect(action_fade_rels_in, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsIn()));
	connect(action_fade_rels_out, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsOut()));
	connect(action_collapse_ext_attribs, SIGNAL(triggered(bool)), this, SLOT(setCollapseMode()));
	connect(action_collpase_all_attribs, SIGNAL(triggered(bool)), this, SLOT(setCollapseMode()));
	connect(action_no_collapse_attribs, SIGNAL(triggered(bool)), this, SLOT(setCollapseMode()));
	connect(action_show_schemas_rects, SIGNAL(triggered(bool)), this, SLOT(toggleSchemasRectangles()));
	connect(action_hide_schemas_rects, SIGNAL(triggered(bool)), this, SLOT(toggleSchemasRectangles()));
	connect(db_model, SIGNAL(s_objectAdded(BaseObject*)), this, SLOT(handleObjectAddition(BaseObject *)));
	connect(db_model, SIGNAL(s_objectRemoved(BaseObject*)), this, SLOT(handleObjectRemoval(BaseObject *)));
	connect(scene, SIGNAL(s_objectsMoved(bool)), this, SLOT(handleObjectsMovement(bool)));
	connect(scene, SIGNAL(s_objectModified(BaseGraphicObject*)), this, SLOT(handleObjectModification(BaseGraphicObject*)));
	connect(scene, SIGNAL(s_objectDoubleClicked(BaseGraphicObject*)), this, SLOT(handleObjectDoubleClick(BaseGraphicObject*)));
	connect(scene, SIGNAL(s_popupMenuRequested(BaseObject*)), this, SLOT(configurePopupMenu(BaseObject *)));
	connect(scene, SIGNAL(s_popupMenuRequested()), this, SLOT(showObjectMenu()));
	connect(scene, SIGNAL(s_objectSelected(BaseGraphicObject*,bool)), this, SLOT(configureObjectSelection()));
	connect(scene, SIGNAL(s_childrenSelectionChanged()), this, SLOT(configureObjectSelection()));
	connect(scene, SIGNAL(s_objectsSelectedInRange()), this, SLOT(configureObjectSelection()));
	connect(scene, &ObjectsScene::s_collapseModeChanged, [&](){ setModified(true); });
	connect(scene, &ObjectsScene::s_paginationToggled, [&](){ setModified(true); });
	connect(scene, &ObjectsScene::s_currentPageChanged, [&](){ setModified(true); });
	connect(scene, &ObjectsScene::s_objectsMovedLayer, [&](){ setModified(true); });
	connect(scene, SIGNAL(s_layersChanged()), this, SLOT(updateModelLayers()));
	connect(scene, SIGNAL(s_activeLayersChanged()), this, SLOT(updateModelLayers()));
	connect(scene, SIGNAL(s_popupMenuRequested(BaseObject*)), new_obj_overlay_wgt, SLOT(hide()));
	connect(scene, SIGNAL(s_popupMenuRequested()), new_obj_overlay_wgt, SLOT(hide()));
	connect(scene, SIGNAL(s_objectSelected(BaseGraphicObject*,bool)), new_obj_overlay_wgt, SLOT(hide()));
	connect(scene, SIGNAL(s_childrenSelectionChanged()), new_obj_overlay_wgt, SLOT(hide()));
	connect(scene, SIGNAL(s_objectsScenePressed(Qt::MouseButtons)), new_obj_overlay_wgt, SLOT(hide()));

	viewport->installEventFilter(this);
	viewport->horizontalScrollBar()->installEventFilter(this);
	viewport->verticalScrollBar()->installEventFilter(this);
}

ModelWidget::~ModelWidget()
{
	/* If there are copied/cutted objects that belongs to the database model
	 being destroyed, then the cut/copy operation are cancelled by emptying
	 the lists, avoiding crashes when trying to paste them */
	if((!copied_objects.empty() && copied_objects[0]->getDatabase()==db_model) ||
			(!cutted_objects.empty() && cutted_objects[0]->getDatabase()==db_model))
	{
		cut_operation=false;
		copied_objects.clear();
		cutted_objects.clear();
	}

	popup_menu.clear();
	new_object_menu.clear();
	quick_actions_menu.clear();
	schemas_menu.clear();
	owners_menu.clear();
	tags_menu.clear();
	break_rel_menu.clear();

	delete viewport;
	delete scene;
	delete op_list;
	delete db_model;
}

void ModelWidget::setModified(bool value)
{
	this->modified = value;
	emit s_modelModified(value);
}

void ModelWidget::resizeEvent(QResizeEvent *)
{
	QRectF ret=scene->sceneRect();

	//Validating the width and height of the scene, resizing if the dimension is invalid
	if(viewport->rect().width() > ret.width())
		ret.setWidth(viewport->rect().width());

	if(viewport->rect().height() > ret.height())
		ret.setHeight(viewport->rect().height());

	scene->setSceneRect(ret);

	zoom_info_lbl->move((this->width()/2) - (zoom_info_lbl->width()/2),
											(this->height()/2)  - (zoom_info_lbl->height()/2));

	magnifier_area_lbl->move(viewport->width() - magnifier_area_lbl->width(),
													 viewport->height() - magnifier_area_lbl->height());

	adjustOverlayPosition();

	emit s_modelResized();
}

bool ModelWidget::eventFilter(QObject *object, QEvent *event)
{
	QWheelEvent *w_event=dynamic_cast<QWheelEvent *>(event);
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);
	QGraphicsSceneMouseEvent *m_event = dynamic_cast<QGraphicsSceneMouseEvent *>(event);

	//Filters the Wheel event if it is raised by the viewport scrollbars
	if((object == viewport->horizontalScrollBar() ||
			object == viewport->verticalScrollBar())
		 && event->type() == QEvent::Wheel && w_event->modifiers()==Qt::ControlModifier)
	{
		double zoom_inc = round(fabs(w_event->angleDelta().y())/120) * ZoomIncrement;

		if(w_event->angleDelta().y() < 0)
			this->applyZoom(this->current_zoom - zoom_inc);
		else
			this->applyZoom(this->current_zoom + zoom_inc);

		return true;
	}
	else if(event->type() == QEvent::KeyPress && k_event->modifiers()==Qt::AltModifier)
	{
		this->keyPressEvent(k_event);
		return true;
	}
	else if((object == magnifier_area_lbl || object == magnifier_frm) &&
					(event->type() == QEvent::MouseMove || event->type() == QEvent::KeyRelease))
	{
		if(event->type() == QEvent::MouseMove)
			updateMagnifierArea();
		else if(k_event->modifiers() == (Qt::ControlModifier | Qt::AltModifier))
			showMagnifierArea(false);

		return true;
	}
	else if(object == magnifier_frm && event->type() == QEvent::MouseButtonPress)
	{
		QGraphicsSceneMouseEvent *gm_event = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
		QMouseEvent *m_event = dynamic_cast<QMouseEvent *>(event);

		showMagnifierArea(false);
		gm_event->setButton(m_event->button());
		gm_event->setButtons(m_event->buttons());
		gm_event->setScenePos(viewport->mapToScene(viewport->mapFromGlobal(QCursor::pos())));
		qApp->postEvent(scene, gm_event);

		return true;
	}
	else if(object == scene && m_event)
	{
		if(event->type() == QEvent::GraphicsSceneMouseMove)
		{
			emit s_sceneInteracted(m_event->scenePos());

			if(!selected_objects.empty())
			{
				if(selected_objects.size() == 1)
				{
					BaseGraphicObject *graph_obj = dynamic_cast<BaseGraphicObject *>(selected_objects[0]);
					emit s_sceneInteracted(graph_obj ? dynamic_cast<BaseObjectView *>(graph_obj->getOverlyingObject()) : nullptr);
				}
				else
					emit s_sceneInteracted(static_cast<int>(selected_objects.size()), scene->itemsBoundingRect(true, true));
			}
		}

		//Forcing the panning mode using the middle mouse button
		if(m_event->buttons() == Qt::MiddleButton && event->type() == QEvent::GraphicsSceneMouseMove)
		{
			if(!panning_mode)
			{
				panning_mode = true;

				//Forcing the closed hand cursor because the default behavior of panning mode in QGraphicsView is to set an open hand cursor
				QApplication::setOverrideCursor(Qt::ClosedHandCursor);
			}

			QPointF pos = m_event->lastScreenPos() - m_event->screenPos();
			int dx = viewport->horizontalScrollBar()->value() + pos.x(),
					dy = viewport->verticalScrollBar()->value() + pos.y();

			viewport->horizontalScrollBar()->setValue(dx);
			viewport->verticalScrollBar()->setValue(dy);

			return true;
		}
		else if(m_event->button() == Qt::NoButton && event->type() == QEvent::GraphicsSceneMouseMove)
		{
			if(magnifier_frm->isVisible())
				updateMagnifierArea();
		}
		//Activating the panning mode
		else if(m_event->button() == Qt::MiddleButton && event->type() == QEvent::GraphicsSceneMousePress)
		{
			viewport->setDragMode(QGraphicsView::ScrollHandDrag);
			QApplication::restoreOverrideCursor();
			QApplication::setOverrideCursor(Qt::OpenHandCursor);
			return true;
		}
		//Deactivating the panning mode
		else if(m_event->button() == Qt::MiddleButton && event->type() == QEvent::GraphicsSceneMouseRelease)
		{
			panning_mode = false;
			viewport->setDragMode(QGraphicsView::NoDrag);
			QApplication::restoreOverrideCursor();
			QApplication::restoreOverrideCursor();
			return true;
		}
	}

	return QWidget::eventFilter(object, event);
}

void ModelWidget::keyPressEvent(QKeyEvent *event)
{
	//Cancels the insertion action when ESC is pressed
	if(event->key()==Qt::Key_Escape)
	{
		if(new_obj_overlay_wgt->isVisible())
			new_obj_overlay_wgt->hide();
		else
		{
			this->cancelObjectAddition();

			if(!scene->isMovingObjects())
				scene->clearSelection();
		}
	}
	else if(event->key()==Qt::Key_N)
	{
		toggleNewObjectOverlay();
	}
	else if(event->modifiers() == (Qt::ControlModifier | Qt::AltModifier) && current_zoom < 1)
	{
		showMagnifierArea(true);
	}
}

void ModelWidget::keyReleaseEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift)
	{
		showMagnifierArea(false);
	}
}

void ModelWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons()==Qt::LeftButton)
	{
		/* If the user is adding a graphical object, the left click will set the initial position and
		show the editing form related to the object type */
		if(!simple_obj_creation && (BaseTable::isBaseTable(new_obj_type) || new_obj_type==ObjectType::Textbox))
		{
			this->scene->enableRangeSelection(false);
			this->showObjectForm(new_obj_type, nullptr, nullptr, viewport->mapToScene(event->pos()));
			this->cancelObjectAddition();
			this->scene->enableRangeSelection(true);
		}
	}
}

void ModelWidget::hideEvent(QHideEvent *)
{
	magnifier_area_lbl->hide();
}

bool ModelWidget::saveLastCanvasPosition()
{
	if(save_restore_pos)
	{
		QScrollBar *hscroll=viewport->horizontalScrollBar(),
				*vscroll=viewport->verticalScrollBar();
		QPoint pos=db_model->getLastPosition();

		//Save the zoom or position only one of these attributes has changed
		if(db_model->getLastZoomFactor()!=current_zoom ||
				pos.x()!=hscroll->value() || pos.y()!=vscroll->value())
		{
			db_model->setLastPosition(QPoint(viewport->horizontalScrollBar()->value(),
											 viewport->verticalScrollBar()->value()));
			db_model->setLastZoomFactor(this->current_zoom);
			return true;
		}
	}

	return false;
}

void ModelWidget::setUpdatesEnabled(bool value)
{
	viewport->setUpdatesEnabled(value);
	QWidget::setUpdatesEnabled(value);
}

void ModelWidget::restoreLastCanvasPosition()
{
	if(save_restore_pos)
	{
		QScrollBar *hscroll=viewport->horizontalScrollBar(),
				*vscroll=viewport->verticalScrollBar();

		if(db_model->getLastZoomFactor()!=1.0)
			this->applyZoom(db_model->getLastZoomFactor());

		hscroll->setValue(db_model->getLastPosition().x());
		vscroll->setValue(db_model->getLastPosition().y());

		scene->update();
	}
}

void ModelWidget::applyZoom(double zoom)
{
	if(zoom < MinimumZoom)
		zoom = MinimumZoom;
	else if(zoom > MaximumZoom)
		zoom = MaximumZoom;

	viewport->resetTransform();
	viewport->scale(zoom, zoom);
	this->current_zoom=zoom;

	zoom_info_lbl->setText(tr("Zoom: %1%").arg(QString::number(this->current_zoom * 100, 'g' , 3)));
	zoom_info_lbl->setVisible(true);
	zoom_info_timer.start();

	emit s_zoomModified(zoom);
}

double ModelWidget::getCurrentZoom()
{
	return current_zoom;
}

void ModelWidget::handleObjectAddition(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	if(graph_obj)
	{
		ObjectType obj_type=graph_obj->getObjectType();
		QGraphicsItem *item=nullptr;

		switch(obj_type)
		{
			case ObjectType::ForeignTable:
			case ObjectType::Table:
				item=new TableView(dynamic_cast<PhysicalTable *>(graph_obj));
			break;

			case ObjectType::View:
				item=new GraphicalView(dynamic_cast<View *>(graph_obj));
			break;

			case ObjectType::Relationship:
			case ObjectType::BaseRelationship:
				item=new RelationshipView(dynamic_cast<BaseRelationship *>(graph_obj));
			break;

			case ObjectType::Schema:
				if(!graph_obj->isSystemObject() ||
						(graph_obj->isSystemObject() && graph_obj->getName()==QString("public")))
				{
					item=new SchemaView(dynamic_cast<Schema *>(graph_obj));
				}
			break;

			default:
				item=new StyledTextboxView(dynamic_cast<Textbox *>(graph_obj));
			break;
		}

		if(item)
		{
			scene->addItem(item);
			setModified(true);
		}
	}
}

void ModelWidget::addNewObject()
{
	QAction *action=dynamic_cast<QAction *>(sender());

	if(action)
	{
		BaseObject *parent_obj=nullptr;
		ObjectType obj_type=static_cast<ObjectType>(action->data().toUInt());

		/* If the user wants to add a table object or a object inside a schema
		uses as parent object the selected object, because the user only can add
		these types after select a table or schema, respectively */
		if(selected_objects.size()==1 &&
				(TableObject::isTableObject(obj_type) ||
				 selected_objects[0]->getObjectType()==ObjectType::Schema))
			parent_obj=selected_objects[0];

		//Creating a table or view inside a schema
		if(parent_obj && parent_obj->getObjectType()==ObjectType::Schema && BaseTable::isBaseTable(obj_type))
		{
			BaseObjectView *sch_graph=dynamic_cast<BaseObjectView *>(dynamic_cast<Schema *>(parent_obj)->getOverlyingObject());
			QSizeF size = sch_graph->boundingRect().size();
			QPointF pos, menu_pos = viewport->mapToScene(this->mapFromGlobal(popup_menu.pos()));
			QRectF rect = QRectF(sch_graph->pos(), size);

			//Uses the popup menu position if it is inside the bounding rectangle
			if(rect.contains(menu_pos))
				pos=menu_pos;
			//Otherwise inserts the new object at the middle of bounding rect
			else
				pos=QPointF(sch_graph->pos().x() + (size.width()/2.0),
							sch_graph->pos().y() + (size.height()/2.0));

			this->showObjectForm(obj_type, nullptr, parent_obj, pos);
		}
		else if(!BaseTable::isBaseTable(obj_type) &&
						obj_type!=ObjectType::Textbox && obj_type <= ObjectType::BaseTable)
			this->showObjectForm(obj_type, nullptr, parent_obj);
		else
		{
			/* A small checking to enable the overlay widget to create relationships and
		 other graphical objects without the user click on the canvas area */
			if((obj_type > ObjectType::BaseObject &&
				selected_objects.size()==2 &&
				selected_objects.at(0)->getObjectType()==ObjectType::Table &&
				selected_objects.at(1)->getObjectType()==ObjectType::Table))
			{
				this->showObjectForm(obj_type);
			}
			else
			{
				//Simple table|view|textbox creation
				if(simple_obj_creation &&
						(BaseTable::isBaseTable(obj_type) || obj_type==ObjectType::Textbox))
					this->showObjectForm(obj_type, nullptr, parent_obj, viewport->mapToScene(viewport->rect().center()));
				else
				{
					//For the graphical object, changes the cursor icon until the user click on the model to show the editing form
					viewport->setCursor(QCursor(action->icon().pixmap(QSize(22,22)),0,0));
					this->new_obj_type=obj_type;
					this->enableModelActions(false);

					/* If a single table is selected and the user triggered a relationship creation action via popup menu,
					 * we force the enabling of the relationship creation steps. This will automatically selects the current table
					 * as source table of the relationship */
					if(selected_objects.size() == 1 &&
						 PhysicalTable::isPhysicalTable(selected_objects[0]->getObjectType()) && new_obj_type > ObjectType::BaseTable)
						configureObjectSelection();
				}
			}
		}
	}
}

void ModelWidget::handleObjectRemoval(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	if(graph_obj)
	{
		scene->removeItem(dynamic_cast<QGraphicsItem *>(graph_obj->getOverlyingObject()));

		//Updates the parent schema if the removed object were a table or view
		if(graph_obj->getSchema() &&
				(graph_obj->getObjectType()==ObjectType::Table || graph_obj->getObjectType()==ObjectType::View))
			dynamic_cast<Schema *>(graph_obj->getSchema())->setModified(true);
	}

	setModified(true);
}

void ModelWidget::handleObjectDoubleClick(BaseGraphicObject *object)
{
	if(object)
		this->showObjectForm(object->getObjectType(), object, nullptr, object->getPosition());
	else
		this->showObjectForm(ObjectType::Database, db_model);
}

void ModelWidget::handleObjectsMovement(bool end_moviment)
{
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> reg_tables;
	QList<BaseObjectView *> tables;
	BaseGraphicObject *obj=nullptr;
	Schema *schema=nullptr;

	itr=selected_objects.begin();
	itr_end=selected_objects.end();

	if(!end_moviment)
	{
		op_list->startOperationChain();

		while(itr!=itr_end)
		{
			obj=dynamic_cast<BaseGraphicObject *>(*itr);
			itr++;
			if(!obj)	continue;

			if(!dynamic_cast<BaseRelationship *>(obj) && (obj && !obj->isProtected()))
			{
				schema=dynamic_cast<Schema *>(obj);

				//Register the object if it is not a schema or a table already registered
				if(!schema && std::find(reg_tables.begin(), reg_tables.end(), obj)==reg_tables.end())
					op_list->registerObject(obj, Operation::ObjectMoved);
				else if(schema)
				{
					//For schemas, when they are moved, the original position of tables are registered instead of the position of schema itself
					tables=dynamic_cast<SchemaView *>(schema->getOverlyingObject())->getChildren();
					for(auto &tab : tables)
					{
						op_list->registerObject(tab->getUnderlyingObject(), Operation::ObjectMoved);

						//Registers the table on a auxiliary list to avoid multiple registration on operation history
						reg_tables.push_back(tab->getUnderlyingObject());
					}
				}
			}			
		}
	}
	else
	{
		vector<Schema *> schemas;

		while(itr!=itr_end)
		{
			obj = dynamic_cast<BaseGraphicObject *>(*itr);
			itr++;
			if(!obj) continue;

			if(BaseTable::isBaseTable(obj->getObjectType()))
			{
				Schema *schema=dynamic_cast<Schema *>(dynamic_cast<BaseTable *>(obj)->getSchema());

				//Update the schema if this isn't done yet
				if(std::find(schemas.begin(),schemas.end(),schema)==schemas.end())
				{
					schema->setModified(true);

					//Insert the updated schema to a list to avoid a second (unnecessary) update
					schemas.push_back(schema);
				}
			}
		}

		op_list->finishOperationChain();
		setModified(true);

		emit s_objectsMoved();
	}
}

void ModelWidget::handleObjectModification(BaseGraphicObject *object)
{
	op_list->registerObject(object, Operation::ObjectModified);
	setModified(true);

	if(object->getSchema())
		dynamic_cast<Schema *>(object->getSchema())->setModified(true);

	emit s_objectModified();
}

void ModelWidget::emitSceneInteracted()
{
	if(selected_objects.empty())
		emit s_sceneInteracted(nullptr);
	else if(selected_objects.size() == 1)
	{
		BaseGraphicObject *base_obj = dynamic_cast<BaseGraphicObject *>(selected_objects[0]);
		TableObject *tab_obj=dynamic_cast<TableObject *>(selected_objects[0]);

		if(base_obj)
			emit s_sceneInteracted(dynamic_cast<BaseObjectView *>(base_obj->getOverlyingObject()));
		else if(tab_obj)
			emit s_sceneInteracted(1, QRect());
		else
			emit s_sceneInteracted(nullptr);
	}
	else
		emit s_sceneInteracted(static_cast<int>(selected_objects.size()), scene->itemsBoundingRect(true, true));
}

void ModelWidget::configureObjectSelection()
{
	QList<QGraphicsItem *> items=scene->selectedItems();
	BaseObjectView *item=nullptr;
	map<unsigned, QGraphicsItem *> objs_map;
	map<unsigned, QGraphicsItem *>::iterator itr;

	selected_objects.clear();

	//Stores in a map each selected graphical object on the scene
	while(!items.isEmpty())
	{
		//Only store object that can be converted to BaseObjectView
		item=dynamic_cast<BaseObjectView *>(items.front());
		items.pop_front();

		if(item)
			objs_map[item->getSelectionOrder()]=item;
	}

	itr=objs_map.begin();
	while(itr!=objs_map.end())
	{
		item=dynamic_cast<BaseObjectView *>(itr->second);
		selected_objects.push_back(item->getUnderlyingObject());
		itr++;
	}

	/* Case the new_obj_type is a value greater the ObjectType::ObjBaseTable indicates that the user
	(un)selected a object using some "Relationship" action */
	if(new_obj_type > ObjectType::BaseTable)
	{
		unsigned count=selected_objects.size();
		ObjectType obj_type1, obj_type2;

		//If there is more than 2 object select cancel the operation
		if(count > 2 || count==0)
			this->cancelObjectAddition();
		else if(count >=1 && count <=2)
		{
			//Get the selected objects types
			obj_type1=selected_objects[0]->getObjectType();
			obj_type2=(count==2 ? selected_objects[1]->getObjectType() : ObjectType::BaseObject);

			//If there is only one selected object and this is a table, activates the relationship creation
			if(!scene->isRelationshipLineVisible() &&
				 count==1 && PhysicalTable::isPhysicalTable(obj_type1) &&
				 new_obj_type > ObjectType::BaseTable &&	QApplication::keyboardModifiers() == 0)
			{
				BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(selected_objects[0]);
				BaseObjectView *object=dynamic_cast<BaseObjectView *>(graph_obj->getOverlyingObject());

				scene->showRelationshipLine(true,
																		QPointF(object->scenePos().x() + object->boundingRect().width()/2,
																						object->scenePos().y() + object->boundingRect().height()/2));
			}
			//If the user has selected object that are not tables, cancel the operation
			else if(!PhysicalTable::isPhysicalTable(obj_type1) || (!PhysicalTable::isPhysicalTable(obj_type2) && obj_type2 != ObjectType::BaseObject))
			{
				this->cancelObjectAddition();
			}

			/* Case there is only one selected object (table) and the SHIFT key is pressed too, creates a self-relationship.
				 Case there is two selected objects, create a relationship between them */
			else if((count==1 && PhysicalTable::isPhysicalTable(obj_type1) && QApplication::keyboardModifiers()==Qt::ShiftModifier) ||
							(count==2 && PhysicalTable::isPhysicalTable(obj_type1) && PhysicalTable::isPhysicalTable(obj_type2)))
			{
				/* Forcing no signals to be emitted by the scene while the relationship is being configured to avoid this
				 * method to be called unecessarily */
				scene->blockSignals(true);
				this->showObjectForm(new_obj_type);
				scene->blockSignals(false);

				//Cancels the operation after showing the relationship editing form
				scene->clearSelection();
				this->cancelObjectAddition();
			}
		}
	}
	else
		this->configurePopupMenu(selected_objects);

	emitSceneInteracted();
}

void ModelWidget::selectAllObjects()
{
	QAction *act = qobject_cast<QAction *>(sender());

	if(!act)
		return;

	ObjectType obj_type = static_cast<ObjectType>(act->data().toUInt());

	if(obj_type == ObjectType::BaseObject)
	{
		QPainterPath pth;
		pth.addRect(scene->sceneRect());

		scene->blockItemsSignals(true);
		scene->setSelectionArea(pth);
		scene->blockItemsSignals(false);
	}
	else
	{
		BaseObjectView *obj_view = nullptr;
		vector<BaseObject *> objs = *db_model->getObjectList(obj_type);

		if(obj_type == ObjectType::Relationship)
			objs.insert(objs.end(), db_model->getObjectList(ObjectType::BaseRelationship)->begin(),  db_model->getObjectList(ObjectType::BaseRelationship)->end());

		for(auto &obj : objs)
		{
			obj_view = dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(obj)->getOverlyingObject());

			if(obj_view)
			{
				obj_view->blockSignals(true);
				obj_view->setSelected(true);
				obj_view->blockSignals(false);
			}
		}
	}

	configureObjectSelection();
}

void ModelWidget::convertRelationshipNN()
{
	Relationship *rel=reinterpret_cast<Relationship *>(action_convert_relnn->data().value<void *>());

	if(rel)
	{
		//Converts only Many-to-Many relationship
		if(rel->getRelationshipType()==Relationship::RelationshipNn)
		{
			Messagebox msg_box;
			msg_box.show(tr("Do you really want to convert the relationship into an intermediate table?"),
						 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

			if(msg_box.result()==QDialog::Accepted)
			{
				unsigned op_count=0;

				try
				{
					Relationship *rel1=nullptr, *rel2=nullptr;
					Table *tab=nullptr, *tab_nn=nullptr,
							*src_tab=dynamic_cast<Table *>(rel->getTable(Relationship::SrcTable)),
							*dst_tab=dynamic_cast<Table *>(rel->getTable(Relationship::DstTable));
					Constraint *constr=nullptr, *aux_constr=nullptr, *pk=nullptr;
					Column *col=nullptr;
					bool src_mand=true,	dst_mand=true;
					QString tab_name, xml_buf;
					QPointF pnt;
					unsigned i=1, idx, count, idx1, count1, x;
					vector<Constraint *> fks;
					vector<QString> pk_cols;
					int attr_idx=-1;

					op_count=op_list->getCurrentSize();

					//Stores the XML code definition for the table generated by the relationship
					tab_nn=dynamic_cast<Table *>(rel->getReceiverTable());
					pk=tab_nn->getPrimaryKey();

					if(!rel->isSelfRelationship())
					{
						/* Checking if there is some attribute that is a pk too. If so, store their names in a list
							 in order to create the pk further in this method */
						count=pk->getColumnCount(Constraint::SourceCols);
						for(i=0; i < count; i++)
						{
							col=pk->getColumn(i, Constraint::SourceCols);
							attr_idx=rel->getObjectIndex(col);

							if(attr_idx >= 0)
								pk_cols.push_back(col->getName());
						}
					}

					xml_buf=tab_nn->getCodeDefinition(SchemaParser::XmlDefinition);

					//Creates the table from the xml code
					xmlparser->restartParser();
					xmlparser->loadXMLBuffer(xml_buf);
					tab=db_model->createTable();
					tab_name=tab->getName();

					//Forcing the creation of the single pk column
					if(rel->isSiglePKColumn())
					{
						col=new Column;
						(*col)=(*pk->getColumn(0, Constraint::SourceCols));
						col->setParentTable(nullptr);
						tab->addColumn(col);
						pk_cols.push_back(col->getName());
					}

					if(rel->isSelfRelationship())
					{
						//Copy the columns from the table generated by relationship to the newly created table
						count=tab_nn->getColumnCount();
						for(idx=0; idx < count; idx++)
						{
							col=new Column;
							(*col)=(*tab_nn->getColumn(idx));
							col->setParentTable(nullptr);
							tab->addColumn(col);
						}

						/* Copy the constraints from the table generated by relationship to the newly created table
						using the xml definition */
						count=tab_nn->getConstraintCount();
						for(idx=0; idx < count; idx++)
						{
							xml_buf=tab_nn->getConstraint(idx)->getCodeDefinition(SchemaParser::XmlDefinition,true);

							xmlparser->restartParser();
							xmlparser->loadXMLBuffer(xml_buf);
							constr=db_model->createConstraint(tab);
							tab->addConstraint(constr);
						}

						tab->getForeignKeys(fks, false, src_tab);
					}
					else
					{
						//Copies the relationship attributes to the created table
						count=rel->getAttributeCount();
						for(idx=0; idx < count; idx++)
						{
							col=new Column;
							(*col)=(*rel->getAttribute(idx));
							col->setParentTable(nullptr);
							tab->addColumn(col);
						}

						//Copies the relationship constraints to the created table
						count=rel->getConstraintCount();
						for(idx=0; idx < count; idx++)
						{
							constr=new Constraint;
							aux_constr=rel->getConstraint(idx);
							(*constr)=(*aux_constr);
							constr->removeColumns();
							constr->setParentTable(nullptr);

							for(x=Constraint::SourceCols; x <= Constraint::ReferencedCols; x++)
							{
								count1=aux_constr->getColumnCount(x);
								for(idx1=0; idx1 < count1; idx1++)
								{
									col=tab->getColumn(aux_constr->getColumn(idx, x)->getName());
									if(col) constr->addColumn(col, x);
								}
							}
							tab->addConstraint(constr);
						}
					}

					//Renames the table if there is other with the same name on the model avoiding conflicts
					tab->setName(tab_name);
					tab->setName(PgModelerNs::generateUniqueName(tab, *db_model->getObjectList(ObjectType::Table)));

					op_list->startOperationChain();

					//Removes the many-to-many relationship from the model
					op_list->registerObject(rel, Operation::ObjectRemoved);

					//The default position for the table will be the middle point between the relationship participant tables
					pnt.setX((src_tab->getPosition().x() + dst_tab->getPosition().x())/2.0);
					pnt.setY((src_tab->getPosition().y() + dst_tab->getPosition().y())/2.0);
					tab->setPosition(pnt);

					//Adds the new table to the model
					db_model->addObject(tab);
					op_list->registerObject(tab, Operation::ObjectCreated);

					if(rel->isSelfRelationship())
					{
						//For self relationships register the created foreign keys on the operation list
						while(!fks.empty())
						{
							op_list->registerObject(fks.back(), Operation::ObjectCreated, -1, fks.back()->getParentTable());
							fks.pop_back();
						}
					}
					//If not self relationship creates two 1:n relationships
					else
					{
						//Creating the pk based upon the attributes of the relationship
						if(!pk_cols.empty())
						{
							aux_constr=new Constraint;

							for(QString pk_col : pk_cols)
								aux_constr->addColumn(tab->getColumn(pk_col), Constraint::SourceCols);

							aux_constr->setName(PgModelerNs::generateUniqueName(tab, *tab->getObjectList(ObjectType::Constraint), false, QString("_pk")));
							tab->addConstraint(aux_constr);

							op_list->registerObject(aux_constr, Operation::ObjectCreated, -1, tab);
						}

						/* Creates a one-to-many relationship that links the source table of the many-to-many rel. to the created table
				 The relationship will be identifier if the single pk column attribute of the original relationship is false */
						rel1=new Relationship(Relationship::Relationship1n,
												src_tab, tab, src_mand, false, !rel->isSiglePKColumn());
						db_model->addRelationship(rel1);
						op_list->registerObject(rel1, Operation::ObjectCreated);

						/*Creates a one-to-many relationship that links the destination table of the many-to-many rel. to the created table
				The relationship will be identifier if the single pk column attribute of the original relationship is false */
						rel2=new Relationship(Relationship::Relationship1n,
												dst_tab, tab, dst_mand, false, !rel->isSiglePKColumn());
						db_model->addRelationship(rel2);
						op_list->registerObject(rel2, Operation::ObjectCreated);
					}

					op_list->finishOperationChain();

					//Removes the n:n relationship after convert it
					db_model->removeObject(rel);

					emit s_objectCreated();
				}
				catch(Exception &e)
				{
					if(op_count < op_list->getCurrentSize())
					{
						unsigned qtd=op_list->getCurrentSize()-op_count;
						op_list->ignoreOperationChain(true);

						for(unsigned i=0; i < qtd; i++)
						{
							op_list->undoOperation();
							op_list->removeLastOperation();
						}

						op_list->ignoreOperationChain(false);
					}

					throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
				}
			}
		}
	}
}

void ModelWidget::convertRelationship1N()
{
	Relationship *rel=reinterpret_cast<Relationship *>(action_convert_rel1n->data().value<void *>());

	if(!rel || (rel &&
							rel->getRelationshipType() != Relationship::Relationship11 &&
							rel->getRelationshipType() != Relationship::Relationship1n))
		return;

	Messagebox msg_box;

	msg_box.show(tr("<strong>WARNING:</strong> Converting a <strong>one-to-one</strong> or <strong>one-to-many</strong>\
 relationship can lead to unreversible changes or break other relationships in the linking chain! Do you want to proceed?"),
							 Messagebox::AlertIcon, Messagebox::YesNoButtons);

	if(msg_box.result() == QDialog::Rejected)
		return;

	unsigned op_count = op_list->getCurrentSize();

	try
	{
		Table *recv_tab = dynamic_cast<Table *>(rel->getReceiverTable()),
				*ref_tab = dynamic_cast<Table *>(rel->getReferenceTable());
		QStringList constrs_xmls;
		Column *column = nullptr;
		Constraint *constr = nullptr, *pk = recv_tab->getPrimaryKey();
		vector<Column *> columns;
		QString pk_name, orig_name, rel_name = rel->getName();
		bool register_pk = false;
		QColor rel_color = rel->getCustomColor();

		// Storing the XML definition of table's PK
		if(pk && (pk->isReferRelationshipAddedColumn() || pk->isAddedByRelationship()))
		{
			/* This flag indicates that the pk removal (further in this method) should be registered in the operations list
			 * This will happen only if the pk wasn't added by the relationship */
			register_pk = !pk->isAddedByRelationship();
			pk_name = pk->getName();

			// Storing the pk XML definition so it can be recreated correctly even after the disconnection of all relationships
			constrs_xmls.append(recv_tab->getPrimaryKey()->getCodeDefinition(SchemaParser::XmlDefinition, true));
		}

		// Stores the XML definition of all generated constraints
		for(auto &constr : rel->getGeneratedConstraints())
		{
			if(constr->getConstraintType() == ConstraintType::PrimaryKey)
				continue;

			constrs_xmls.append(constr->getCodeDefinition(SchemaParser::XmlDefinition, true));
		}

		// Stores the XML definition of all relationship's constraints (added by the user)
		for(auto &obj : rel->getConstraints())
		{
			constr = dynamic_cast<Constraint *>(obj);
			constrs_xmls.append(constr->getCodeDefinition(SchemaParser::XmlDefinition, true));
		}

		// Copying all generated columns
		for(auto &col : rel->getGeneratedColumns())
		{
			column = new Column;
			*column = *col;
			columns.push_back(column);
		}

		// Copying all relationship attributes
		for(auto &col : rel->getAttributes())
		{
			column = new Column;
			*column = *(dynamic_cast<Column *>(col));
			columns.push_back(column);
		}

		QApplication::setOverrideCursor(Qt::WaitCursor);
		op_list->startOperationChain();

		db_model->storeSpecialObjectsXML();
		db_model->disconnectRelationships();
		db_model->__removeObject(rel);

		// Register the exclusion of the original relationship
		op_list->registerObject(rel, Operation::ObjectRemoved);

		/* If after the relationships disconnection the table still have a PK
		 * it means that it was not added by relationship so we can remove it from the table
		 * so it can be recreated further with the correct settings */
		 pk = recv_tab->getConstraint(pk_name);

		if(pk)
		{
			if(register_pk)
				op_list->registerObject(pk, Operation::ObjectRemoved, -1, recv_tab);

			recv_tab->removeObject(pk);
		}

		// Adding the copied columns to the receiver table
		for(auto &col : columns)
		{
			col->setParentRelationship(nullptr);
			col->setParentTable(nullptr);
			recv_tab->addColumn(col);
			op_list->registerObject(col, Operation::ObjectCreated, - 1, recv_tab);
		}

		// Recreating the constraints from XML code
		for(auto &constr_xml : constrs_xmls)
		{
			xmlparser->restartParser();
			xmlparser->loadXMLBuffer(constr_xml);
			constr = db_model->createConstraint(recv_tab);
			recv_tab->addConstraint(constr);
			op_list->registerObject(constr, Operation::ObjectCreated, - 1, recv_tab);
		}

		/* Resetting the relatinship added columns/constraint indexes in the table
		 * in order to avoid invalid indexes causing crashes */
		recv_tab->resetRelObjectsIndexes();
		recv_tab->setModified(true);

		db_model->validateRelationships();
		db_model->updateTableFKRelationships(recv_tab);

		// Setting up the same name and color of the generated relationship
		BaseRelationship *fk_rel = db_model->getRelationship(recv_tab, ref_tab);
		fk_rel->setName(rel_name);
		fk_rel->setCustomColor(rel_color);
		fk_rel->setModified(true);

		op_list->registerObject(fk_rel, Operation::ObjectModified);
		op_list->finishOperationChain();

		QApplication::restoreOverrideCursor();
		emit s_objectCreated();
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();

		if(op_count < op_list->getCurrentSize())
		{
			unsigned qtd = op_list->getCurrentSize() - op_count;
			op_list->ignoreOperationChain(true);

			for(unsigned i=0; i < qtd; i++)
			{
				op_list->undoOperation();
				op_list->removeLastOperation();
			}

			op_list->ignoreOperationChain(false);
		}

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::loadModel(const QString &filename)
{
	TaskProgressWidget task_prog_wgt(this);

	try
	{
		connect(db_model, SIGNAL(s_objectLoaded(int,QString,unsigned)), &task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
		task_prog_wgt.addIcon(enum_cast(ObjectType::BaseObject), QPixmap(PgModelerUiNs::getIconPath("design")));
		task_prog_wgt.setWindowTitle(tr("Loading database model"));
		task_prog_wgt.show();

		db_model->loadModel(filename);
		this->filename=filename;
		this->adjustSceneSize();
		this->updateObjectsOpacity();

		scene->blockSignals(true);

		for(auto &layer : db_model->getLayers())
			scene->addLayer(layer);

		scene->setActiveLayers(db_model->getActiveLayers());
		scene->blockSignals(false);

		task_prog_wgt.close();
		protected_model_frm->setVisible(db_model->isProtected());
		setModified(false);
	}
	catch(Exception &e)
	{
		task_prog_wgt.close();
		setModified(false);
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::adjustSceneSize()
{
	QRectF scene_rect, objs_rect;

	scene_rect=scene->sceneRect();
	objs_rect=scene->itemsBoundingRect();

	if(scene_rect.width() < objs_rect.left() + objs_rect.width())
		scene_rect.setWidth(objs_rect.left() + objs_rect.width());

	if(scene_rect.height() < objs_rect.top() + objs_rect.height())
		scene_rect.setHeight(objs_rect.top() + objs_rect.height());

	scene->setSceneRect(scene_rect);
	viewport->centerOn(0,0);

	if(ObjectsScene::isAlignObjectsToGrid())
	{
		scene->alignObjectsToGrid();
		db_model->setObjectsModified({ ObjectType::Relationship, ObjectType::BaseRelationship });
	}

	emit s_sceneInteracted(scene_rect.size());
}

void ModelWidget::printModel(QPrinter *printer, bool print_grid, bool print_page_nums)
{
	if(printer)
	{
		bool show_grid, align_objs, show_delims;
		unsigned page_cnt, page, h_page_cnt, v_page_cnt, h_pg_id, v_pg_id;
		vector<QRectF> pages;
		QRectF margins;
		QPrinter::PaperSize paper_size_id;
		QPrinter::Orientation orient;
		QSizeF paper_size, custom_p_size;
		QSize page_size;
		QPen pen;
		QFont font;
		QPointF top_left, top_right, bottom_left, bottom_right,
				h_top_mid, h_bottom_mid, v_left_mid, v_right_mid, dx, dy, dx1, dy1;

		//Make a backup of the current grid options
		show_grid = ObjectsScene::isShowGrid();
		align_objs = ObjectsScene::isAlignObjectsToGrid();
		show_delims = ObjectsScene::isShowPageDelimiters();

		//Reconfigure the grid options based upon the passed settings
		ObjectsScene::setGridOptions(print_grid, align_objs, false);

		scene->update();
		scene->clearSelection();

		//Get the page size based on the printer settings
		ObjectsScene::getPaperConfiguration(paper_size_id, orient, margins, custom_p_size);
		paper_size=printer->paperSize(QPrinter::Point);

		if(paper_size_id!=QPrinter::Custom)
			paper_size-=margins.size();

		//Get the pages rect for printing
		pages=scene->getPagesForPrinting(paper_size, margins.size(), h_page_cnt, v_page_cnt);
		page_size=printer->pageRect().size();

		//Creates a painter to draw the model directly on the printer
		QPainter painter(printer);
		painter.setRenderHint(QPainter::Antialiasing);
		font.setPointSizeF(7.5);
		pen.setColor(QColor(120,120,120));
		pen.setWidthF(1.0);

		//Calculates the auxiliary points to draw the page delimiter lines
		top_left.setX(0); top_left.setY(0);
		top_right.setX(page_size.width()); top_right.setY(0);

		bottom_left.setX(0); bottom_left.setY(page_size.height());
		bottom_right.setX(top_right.x()); bottom_right.setY(bottom_left.y());

		h_top_mid.setX(page_size.width()/2); h_top_mid.setY(0);
		h_bottom_mid.setX(h_top_mid.x()); h_bottom_mid.setY(bottom_right.y());

		v_left_mid.setX(top_left.x()); v_left_mid.setY(page_size.height()/2);
		v_right_mid.setX(top_right.x()); v_right_mid.setY(v_left_mid.y());

		dx.setX(margins.left());
		dx1.setX(margins.width());
		dy.setY(margins.top());
		dy1.setY(margins.height());

		page_cnt=pages.size();
		for(page=0, h_pg_id=0, v_pg_id=0; page < page_cnt; page++)
		{
			//Render the current page on the printer
			scene->render(&painter, QRect(), pages[page]);

			//Print the current page number is this option is marked
			if(print_page_nums)
			{
				painter.setPen(QColor(120,120,120));
				painter.drawText(-margins.left(), -margins.top(), QString("%1").arg(page+1));
			}

			//Print the guide lines at corners of the page
			painter.setPen(pen);
			if(h_pg_id==0 && v_pg_id==0)
			{
				painter.drawLine(top_left, top_left + dx);
				painter.drawLine(top_left, top_left + dy);
			}

			if(h_pg_id==h_page_cnt-1 && v_pg_id==0)
			{
				painter.drawLine(top_right, top_right - dx1);
				painter.drawLine(top_right, top_right + dy);
			}

			if(h_pg_id==0 && v_pg_id==v_page_cnt-1)
			{
				painter.drawLine(bottom_left, bottom_left + dx);
				painter.drawLine(bottom_left, bottom_left - dy1);
			}

			if(h_pg_id==h_page_cnt-1 && v_pg_id==v_page_cnt-1)
			{
				painter.drawLine(bottom_right, bottom_right - dx1);
				painter.drawLine(bottom_right, bottom_right - dy1);
			}

			if(h_pg_id >=1 && h_pg_id < h_page_cnt-1 && v_pg_id==0)
			{
				painter.drawLine(h_top_mid, h_top_mid - dx1);
				painter.drawLine(h_top_mid, h_top_mid + dx);
			}

			if(h_pg_id >=1 && h_pg_id < h_page_cnt-1 && v_pg_id==v_page_cnt-1)
			{
				painter.drawLine(h_bottom_mid, h_bottom_mid - dx1);
				painter.drawLine(h_bottom_mid, h_bottom_mid + dx);
			}

			if(v_pg_id >=1 && v_pg_id < v_page_cnt-1 && h_pg_id==0)
			{
				painter.drawLine(v_left_mid, v_left_mid - dy1);
				painter.drawLine(v_left_mid, v_left_mid + dy);
			}

			if(v_pg_id >=1 && v_pg_id < v_page_cnt-1 && h_pg_id==h_page_cnt-1)
			{
				painter.drawLine(v_right_mid, v_right_mid - dy1);
				painter.drawLine(v_right_mid, v_right_mid + dy);
			}

			h_pg_id++;

			if(h_pg_id==h_page_cnt)
			{
				h_pg_id=0;
				v_pg_id++;
			}

			if(page < page_cnt-1)
				printer->newPage();
		}

		//Restore the grid option backup
		ObjectsScene::setGridOptions(show_grid, align_objs, show_delims);
		scene->update();
	}
}

void ModelWidget::updateRenderHints()
{
	viewport->setRenderHint(QPainter::Antialiasing, !disable_render_smooth);
	viewport->setRenderHint(QPainter::TextAntialiasing, !disable_render_smooth);
	viewport->setRenderHint(QPainter::SmoothPixmapTransform, !disable_render_smooth);
}

void ModelWidget::update()
{
	updateRenderHints();
	scene->update();
	QWidget::update();
}

void ModelWidget::bringToFront()
{
	moveObjectsInZStack(BringToFront);
}

void ModelWidget::sendToBack()
{
	moveObjectsInZStack(SendToBack);
}

void ModelWidget::saveModel()
{
	saveModel(this->filename);
}

void ModelWidget::saveModel(const QString &filename)
{
	TaskProgressWidget task_prog_wgt(this);
	QString bkpfile;
	QTemporaryFile tmpfile;
	bool exists = QFile::exists(filename);

	try
	{
		connect(db_model, SIGNAL(s_objectLoaded(int,QString,unsigned)), &task_prog_wgt, SLOT(updateProgress(int,QString,unsigned)));
		task_prog_wgt.setWindowTitle(tr("Saving database model"));
		task_prog_wgt.show();

		/* If the original file exists we need to make a back first to avoid
		 * in order to recover it in case of failures */
		if(exists)
		{
			// Generate a temporary backup file
			tmpfile.setAutoRemove(false);
			tmpfile.setFileTemplate(GlobalAttributes::getTemporaryFilePath(QString("%1_XXXXXX.dbk").arg(this->db_model->getName())));
			tmpfile.open();
			bkpfile = tmpfile.fileName();
			tmpfile.close();
			tmpfile.remove();

			/* Copy the original database model file prior to the saving to store
			 * its last state in a safe place (temporary storage of the tool ~/.config/pgmodeler by default */
			QFile::copy(filename, bkpfile);

			// Remove the original filename before create a new one in the same path
			QFile::remove(filename);
		}

		saveLastCanvasPosition();
		db_model->saveModel(filename, SchemaParser::XmlDefinition);
		this->filename=filename;

		task_prog_wgt.close();
		disconnect(db_model, nullptr, &task_prog_wgt, nullptr);
		setModified(false);

		/* Doing a final check to the file regarding its size.
		 * If we have a zero-byte file something went wrong during the saving process (disk failure, thread errors, etc)
		 * so we raise an error to the user and restore the backup file to its original path */
		if(QFileInfo(filename).size() == 0)
			throw Exception(Exception::getErrorMessage(ErrorCode::ModelFileInvalidSize).arg(filename).arg(bkpfile),
											ErrorCode::ModelFileInvalidSize,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(exists)
			QFile::remove(bkpfile);
	}
	catch(Exception &e)
	{
		if(exists && QFile::exists(bkpfile))
		{
			QFile::remove(filename);
			QFile::copy(bkpfile, filename);
		}

		task_prog_wgt.close();
		disconnect(db_model, nullptr, &task_prog_wgt, nullptr);
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

QString ModelWidget::getFilename()
{
	return this->filename;
}

QString ModelWidget::getTempFilename()
{
	return this->tmp_filename;
}

int ModelWidget::openEditingForm(QWidget *widget, unsigned button_conf)
{
	BaseForm editing_form(this);
	BaseObjectWidget *base_obj_wgt=qobject_cast<BaseObjectWidget *>(widget);
	QString class_name = widget->metaObject()->className();
	int res = 0;

	if(base_obj_wgt)
	{
		BaseRelationship *rel = dynamic_cast<BaseRelationship *>(base_obj_wgt->getHandledObject());
		editing_form.setMainWidget(base_obj_wgt);

		if(rel)
			class_name.prepend(rel->getRelationshipTypeName().replace(QRegExp("( )+|(\\-)+"), QString()));
	}
	else
		editing_form.setMainWidget(widget);

	editing_form.setButtonConfiguration(button_conf);

	GeneralConfigWidget::restoreWidgetGeometry(&editing_form, class_name);
	res = editing_form.exec();
	GeneralConfigWidget::saveWidgetGeometry(&editing_form, class_name);

	return res;
}

template<class Class, class WidgetClass>
int ModelWidget::openEditingForm(BaseObject *object)
{
	WidgetClass *object_wgt=new WidgetClass;
	object_wgt->setAttributes(db_model, op_list, dynamic_cast<Class *>(object));
	return openEditingForm(object_wgt);
}

template<class Class, class WidgetClass, class ParentClass>
int ModelWidget::openEditingForm(BaseObject *object, BaseObject *parent_obj)
{
	WidgetClass *object_wgt=new WidgetClass;
	object_wgt->setAttributes(db_model, op_list, dynamic_cast<ParentClass *>(parent_obj), dynamic_cast<Class *>(object));
	return openEditingForm(object_wgt);
}

template<class Class, class WidgetClass, class ParentClass>
int ModelWidget::openEditingForm(BaseObject *object, BaseObject *parent_obj, const QPointF &pos)
{
	WidgetClass *object_wgt=new WidgetClass;
	object_wgt->setAttributes(db_model, op_list, dynamic_cast<ParentClass *>(parent_obj), dynamic_cast<Class *>(object), pos.x(), pos.y());
	return openEditingForm(object_wgt);
}

int ModelWidget::openTableEditingForm(ObjectType tab_type, PhysicalTable *object, Schema *schema, const QPointF &pos)
{
	TableWidget *tab_wgt=new TableWidget(nullptr, tab_type);

	if(tab_type == ObjectType::Table)
		tab_wgt->setAttributes(db_model, op_list, schema, dynamic_cast<Table *>(object), pos.x(), pos.y());
	else
		tab_wgt->setAttributes(db_model, op_list, schema, dynamic_cast<ForeignTable *>(object), pos.x(), pos.y());

	return openEditingForm(tab_wgt);
}

void ModelWidget::showObjectForm(ObjectType obj_type, BaseObject *object, BaseObject *parent_obj, const QPointF &pos)
{
	try
	{
		unsigned rel_type=0;
		int res = QDialog::Rejected;
		Schema *sel_schema=dynamic_cast<Schema *>(parent_obj);
		QPointF obj_pos=pos;

		/* Case the obj_type is greater than ObjectType::ObjBaseTable indicates that the object type is a
		 relationship. To get the specific relationship id (1-1, 1-n, n-n, gen, dep) is necessary
		 to subtract the ObjectType::ObjRelationship from the obj_type parameter, the result will point
		 to the BaseRelationship::RELATIONSHIP_??? constant. */
		if(obj_type > ObjectType::BaseTable)
		{
			rel_type=enum_cast(obj_type) - enum_cast(ObjectType::Relationship);
			obj_type=ObjectType::Relationship;
		}

		if(obj_type!=ObjectType::Permission)
		{
			if(object && obj_type!=object->getObjectType())
				throw Exception(ErrorCode::OprObjectInvalidType,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			//If the user try to call the table object form without specify a parent object
			else if(!parent_obj && TableObject::isTableObject(obj_type))
				throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
		}

		if(object && dynamic_cast<BaseGraphicObject *>(object))
			obj_pos=dynamic_cast<BaseGraphicObject *>(object)->getPosition();

		/* Raises an error if the user try to edit a reserverd object. The only exception is for "public" schema
		that can be edited only on its fill color an rectangle attributes */
		if(object && object->isSystemObject() &&
				(object->getObjectType()!=ObjectType::Schema || object->getName()!="public"))
			throw Exception(Exception::getErrorMessage(ErrorCode::OprReservedObject)
											.arg(object->getName()).arg(object->getTypeName()),
											ErrorCode::OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(obj_type==ObjectType::Schema)
			res=openEditingForm<Schema,SchemaWidget>(object);
		else if(obj_type==ObjectType::Role)
			res=openEditingForm<Role,RoleWidget>(object);
		else if(obj_type==ObjectType::Tablespace)
			res=openEditingForm<Tablespace,TablespaceWidget>(object);
		else if(obj_type==ObjectType::Language)
			res=openEditingForm<Language,LanguageWidget>(object);
		else if(obj_type==ObjectType::Cast)
			res=openEditingForm<Cast, CastWidget>(object);
		else if(obj_type==ObjectType::Tag)
			res=openEditingForm<Tag,TagWidget>(object);
		else if(obj_type== ObjectType::EventTrigger)
			res=openEditingForm<EventTrigger,EventTriggerWidget>(object);
		else if(obj_type==ObjectType::Function)
			res=openEditingForm<Function, FunctionWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Conversion)
			res=openEditingForm<Conversion, ConversionWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Domain)
			res=openEditingForm<Domain, DomainWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Aggregate)
			res=openEditingForm<Aggregate, AggregateWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Sequence)
			res=openEditingForm<Sequence, SequenceWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Operator)
			res=openEditingForm<Operator, OperatorWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::OpFamily)
			res=openEditingForm<OperatorFamily, OperatorFamilyWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::OpClass)
			res=openEditingForm<OperatorClass, OperatorClassWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Type)
			res=openEditingForm<Type, TypeWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Collation)
			res=openEditingForm<Collation, CollationWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Extension)
			res=openEditingForm<Extension, ExtensionWidget, Schema>(object, sel_schema);
		else if(obj_type==ObjectType::Table)
			res=openTableEditingForm(obj_type, dynamic_cast<Table *>(object), sel_schema, obj_pos);
		else if(obj_type==ObjectType::ForeignTable)
			res=openTableEditingForm(obj_type, dynamic_cast<ForeignTable *>(object), sel_schema, obj_pos);
		else if(obj_type==ObjectType::View)
			res=openEditingForm<View,ViewWidget,Schema>(object, sel_schema, obj_pos);
		else if(obj_type==ObjectType::Rule)
			res=openEditingForm<Rule, RuleWidget, BaseTable>(object, parent_obj);
		else if(obj_type== ObjectType::Trigger)
			res=openEditingForm<Trigger, TriggerWidget, BaseTable>(object, parent_obj);
		else if(obj_type== ObjectType::Index)
			res=openEditingForm<Index, IndexWidget, BaseTable>(object, parent_obj);
		else if(obj_type== ObjectType::Policy)
			res=openEditingForm<Policy, PolicyWidget, BaseTable>(object, parent_obj);
		else if(obj_type==ObjectType::Column || obj_type==ObjectType::Constraint)
		{
			TableObject *tab_obj=dynamic_cast<TableObject *>(object);

			if(obj_type==ObjectType::Column)
				res=openEditingForm<Column, ColumnWidget, BaseObject>(object, parent_obj);
			else
				res=openEditingForm<Constraint, ConstraintWidget, BaseObject>(object, parent_obj);

			if(res==QDialog::Accepted)
			{
				if(tab_obj && parent_obj->getObjectType()==ObjectType::Table)
					db_model->validateRelationships(tab_obj, dynamic_cast<Table *>(parent_obj));
				else
					db_model->validateRelationships();
			}
		}
		else if(obj_type==ObjectType::BaseRelationship || obj_type==ObjectType::Relationship)
		{
			RelationshipWidget *relationship_wgt=new RelationshipWidget;

			if(!object && rel_type > 0 &&
					selected_objects.size() > 0 &&
					PhysicalTable::isPhysicalTable(selected_objects[0]->getObjectType()))
			{
				PhysicalTable *tab1 = dynamic_cast<PhysicalTable *>(selected_objects[0]),
											*tab2 = (selected_objects.size()==2 ?
															 dynamic_cast<PhysicalTable *>(selected_objects[1]) : tab1);
				relationship_wgt->setAttributes(db_model, op_list, tab1, tab2, rel_type);
			}
			else
				relationship_wgt->setAttributes(db_model, op_list, dynamic_cast<BaseRelationship *>(object));

			res=openEditingForm(relationship_wgt);
			scene->clearSelection();
		}
		else if(obj_type==ObjectType::Textbox)
		{
			TextboxWidget *textbox_wgt=new TextboxWidget;
			textbox_wgt->setAttributes(db_model, op_list, dynamic_cast<Textbox *>(object), obj_pos.x(), obj_pos.y());
			res=openEditingForm(textbox_wgt);
		}
		else if(obj_type==ObjectType::Permission)
		{
			PermissionWidget *permission_wgt=new PermissionWidget;
			Permission *perm=dynamic_cast<Permission *>(object);
			permission_wgt->setAttributes(db_model, nullptr, (perm ? perm->getObject() : object));
			res=openEditingForm(permission_wgt, Messagebox::OkButton);
		}
		else if(obj_type==ObjectType::GenericSql)
		{
			GenericSQLWidget *genericsql_wgt=new GenericSQLWidget;
			genericsql_wgt->setAttributes(db_model, op_list, dynamic_cast<GenericSQL *>(object));
			res=openEditingForm(genericsql_wgt);
		}
		else if(obj_type==ObjectType::ForeignDataWrapper)
			res = openEditingForm<ForeignDataWrapper, ForeignDataWrapperWidget>(object);
		else if(obj_type==ObjectType::ForeignServer)
			res = openEditingForm<ForeignServer, ForeignServerWidget>(object);
		else if(obj_type==ObjectType::UserMapping)
			res = openEditingForm<UserMapping, UserMappingWidget>(object);
		else
		{
			DatabaseWidget *database_wgt=new DatabaseWidget;
			database_wgt->setAttributes(db_model);
			res=openEditingForm(database_wgt);
		}

		if(res==QDialog::Accepted)
		{
			setModified(true);
			this->db_model->setInvalidated(true);
			emit s_objectManipulated();
		}
		else
		  emit s_manipulationCanceled();

		this->setFocus();
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
	}
}

void ModelWidget::showDependenciesReferences()
{
	QAction *obj_sender=dynamic_cast<QAction *>(sender());

	if(obj_sender)
	{
		BaseObject *object=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

		if(object)
		{
			ObjectDepsRefsWidget *deps_refs_wgt=new ObjectDepsRefsWidget;
			deps_refs_wgt->setAttributes(this, object);
			openEditingForm(deps_refs_wgt, Messagebox::OkButton);
		}
	}
}

void ModelWidget::showSourceCode()
{
	QAction *obj_sender=dynamic_cast<QAction *>(sender());

	if(obj_sender)
	{
		BaseObject *object=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

		if(object)
		{
			SourceCodeWidget *sourcecode_wgt=new SourceCodeWidget;
			sourcecode_wgt->setAttributes(this->db_model, object);
			openEditingForm(sourcecode_wgt, Messagebox::OkButton);
		}
	}
}

void ModelWidget::cancelObjectAddition()
{
	//Reset the new object type to a invalid one forcing the user to select a correct type again
	new_obj_type=ObjectType::BaseObject;

	//Restore the cursor icon
	viewport->setCursor(QCursor(Qt::ArrowCursor));

	//Hide the line that simulates the relationship creation
	scene->showRelationshipLine(false);

	this->configurePopupMenu(this->selected_objects);
}

void ModelWidget::renameObjects()
{
	ObjectRenameWidget objectrename_wgt(this);

	if(selected_objects.empty())
		objectrename_wgt.setAttributes({ db_model }, db_model, op_list);
	else
		objectrename_wgt.setAttributes(selected_objects, db_model, op_list);

	objectrename_wgt.exec();

	if(objectrename_wgt.result() == QDialog::Accepted)
	{
		setModified(true);
		emit s_objectModified();
	}
}

void ModelWidget::moveToSchema()
{
	QAction *act=dynamic_cast<QAction *>(sender());
	Schema *schema=dynamic_cast<Schema *>(reinterpret_cast<BaseObject *>(act->data().value<void *>()));
	BaseGraphicObject *obj_graph=nullptr;
	vector<BaseObject *> ref_objs;
	int op_id=-1, op_curr_idx=op_list->getCurrentIndex();

	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);

		op_list->startOperationChain();

		for(BaseObject *obj : selected_objects)
		{
			//Change the object's schema only if the new schema is different from the current
			if(obj->acceptsSchema() && obj->getSchema()!=schema)
			{
				op_id=op_list->registerObject(obj, Operation::ObjectModified, -1);

				obj->setSchema(schema);
				obj_graph=dynamic_cast<BaseGraphicObject *>(obj);

				//If the object is a graphical one, move it to a position near to the new schema box
				if(obj_graph)
				{
					SchemaView *dst_schema=dynamic_cast<SchemaView *>(schema->getOverlyingObject());
					QPointF p;

					if(dst_schema && dst_schema->isVisible())
					{
						p.setX(dst_schema->pos().x());
						p.setY(dst_schema->pos().y() + dst_schema->boundingRect().height() + BaseObjectView::VertSpacing);
						dynamic_cast<BaseObjectView *>(obj_graph->getOverlyingObject())->setPos(p);
					}
				}

				//Invalidating the code of the object's references
				db_model->getObjectReferences(obj, ref_objs);

				for(BaseObject *ref_obj : ref_objs)
					ref_obj->setCodeInvalidated(true);
			}
		}

		op_list->finishOperationChain();
		db_model->setObjectsModified();
		this->setModified(true);

		emit s_objectModified();

		QApplication::restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();

		if(op_id >=0 && op_id > op_curr_idx)
			op_list->removeLastOperation();

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::moveToLayer()
{
	QAction *act = dynamic_cast<QAction *>(sender());
	BaseGraphicObject *graph_obj = nullptr;
	unsigned layer_id = act->data().toUInt();

	for(auto &obj : selected_objects)
	{
		graph_obj = dynamic_cast<BaseGraphicObject *>(obj);

		if(!graph_obj)
			continue;

		graph_obj->setLayer(layer_id);
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	scene->updateActiveLayers();
	QApplication::restoreOverrideCursor();
}

void ModelWidget::changeOwner()
{
	QAction *act=dynamic_cast<QAction *>(sender());
	BaseObject *owner=reinterpret_cast<BaseObject *>(act->data().value<void *>());
	vector<BaseObject *> sel_objs;
	int op_id=-1, op_curr_idx=op_list->getCurrentIndex();

	try
	{
		if(selected_objects.empty())
			sel_objs.push_back(this->db_model);
		else
			sel_objs=selected_objects;

		op_list->startOperationChain();

		for(BaseObject *obj : sel_objs)
		{
			if(obj->acceptsOwner() && obj->getOwner()!=owner)
			{
				if(obj->isSystemObject())
					throw Exception(Exception::getErrorMessage(ErrorCode::OprReservedObject)
									.arg(obj->getName())
									.arg(obj->getTypeName()),
									ErrorCode::OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				//Register an operation only if the object is not the database itself
				if(obj->getObjectType()!=ObjectType::Database)
					op_id=op_list->registerObject(obj, Operation::ObjectModified, -1);

				obj->setOwner(owner);
			}
		}

		op_list->finishOperationChain();
		emit s_objectModified();
	}
	catch(Exception &e)
	{
		if(op_id >=0 && op_id >= op_curr_idx)
			op_list->removeLastOperation();

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::setTag()
{
	QAction *act=dynamic_cast<QAction *>(sender());
	BaseObject *tag=reinterpret_cast<BaseObject *>(act->data().value<void *>());
	BaseTable *tab=nullptr;
	int op_id=-1, op_curr_idx=op_list->getCurrentIndex();

	try
	{
		op_list->startOperationChain();

		for(BaseObject *obj : selected_objects)
		{
			tab=dynamic_cast<BaseTable *>(obj);

			if(tab)
			{
				op_id=op_list->registerObject(obj, Operation::ObjectModified, -1);
				tab->setTag(dynamic_cast<Tag *>(tag));
			}
		}

		op_list->finishOperationChain();
		db_model->setObjectsModified(selected_objects);
		scene->clearSelection();

		emit s_objectModified();
	}
	catch(Exception &e)
	{
		if(op_id >=0 &&  op_id > op_curr_idx)
			op_list->removeLastOperation();

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::editPermissions()
{
	PermissionWidget *permission_wgt=new PermissionWidget;
	QAction *act=dynamic_cast<QAction *>(sender());
	BaseObject *obj=reinterpret_cast<BaseObject *>(act->data().value<void *>());

	permission_wgt->setAttributes(this->db_model, nullptr, obj);
	openEditingForm(permission_wgt, Messagebox::OkButton);

	this->setModified(true);
	emit s_objectManipulated();
}

void ModelWidget::editObject()
{
	QObject *obj_sender=dynamic_cast<QAction *>(sender());
	TableObject *tab_obj=nullptr;
	BaseObject *object=nullptr;

	/* Workaround: To permit the object edition via double click on the ModelObjectWidget the sender
	is configured as the edit action of the model widget */
	if(!obj_sender)
		obj_sender=action_edit;

	object=reinterpret_cast<BaseObject *>(dynamic_cast<QAction *>(obj_sender)->data().value<void *>());
	tab_obj=dynamic_cast<TableObject *>(object);

	if(object)
		showObjectForm(object->getObjectType(), object, (tab_obj ? tab_obj->getParentTable() : nullptr));
}

void ModelWidget::selectSchemaChildren()
{
	QObject *obj_sender=dynamic_cast<QAction *>(sender());
	Schema *schema=nullptr;

	schema=dynamic_cast<Schema *>(
				 reinterpret_cast<BaseObject *>(
					 dynamic_cast<QAction *>(obj_sender)->data().value<void *>()));

	scene->clearSelection();

	dynamic_cast<SchemaView *>(
				dynamic_cast<BaseObjectView *>(schema->getOverlyingObject()))->selectChildren();
}

void ModelWidget::selectTaggedTables()
{
	QObject *obj_sender=dynamic_cast<QAction *>(sender());
	Tag *tag=nullptr;
	vector<BaseObject *> objects;
	BaseObjectView *obj_view = nullptr;

	tag=dynamic_cast<Tag *>(
				reinterpret_cast<BaseObject *>(
					dynamic_cast<QAction *>(obj_sender)->data().value<void *>()));

	scene->clearSelection();
	db_model->getObjectReferences(tag, objects);

	for(auto object : objects)
	{
		obj_view = dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(object)->getOverlyingObject());
		obj_view->setSelected(true);
	}
}

void ModelWidget::protectObject()
{
	try
	{
		QObject *obj_sender=sender();
		ObjectType obj_type;
		TableObject *tab_obj=nullptr;
		bool protect=false;
		QList<BaseGraphicObject *> upd_objects;
		Messagebox msgbox;

		scene->blockSignals(true);

		//Protects the whole model if there is no selected object
		if(this->selected_objects.empty())
		{
			if(obj_sender==action_protect || obj_sender==action_unprotect)
				db_model->setProtected(!db_model->isProtected());
		}
		//If there is more than one selected object, make a batch protection/unprotection
		else
		{
			protect=(!this->selected_objects[0]->isProtected());

			for(auto &object : selected_objects)
			{
				obj_type=object->getObjectType();

				if(obj_type==ObjectType::Column || obj_type==ObjectType::Constraint)
				{
					tab_obj=dynamic_cast<TableObject *>(object);

					if(tab_obj->isAddedByRelationship())
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::OprRelationshipAddedObject)
										.arg(object->getName()).arg(object->getTypeName()),
										ErrorCode::OprRelationshipAddedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
				}

				// Applying protection status for the schema children objects
				if(obj_type==ObjectType::Schema)
				{
					if(!msgbox.isCustomOptionChecked())
					{
						msgbox.setCustomOptionText("Apply to all other selected schemas");
						msgbox.show(QString(QT_TR_NOOP("Do you want to %1 the children of the schema <strong>%2</strong> too?"))
												.arg(protect ? QT_TR_NOOP("protect") : QT_TR_NOOP("unprotect")).arg(object->getName()),
												Messagebox::ConfirmIcon, Messagebox::YesNoButtons);
					}

					if(msgbox.result()==QDialog::Accepted || msgbox.isCustomOptionChecked())
					{
						vector<BaseObject *> objects(db_model->getObjects(object));

						for(BaseObject *obj : objects)
							obj->setProtected(protect);
					}
				}

				object->setProtected(protect);
				tab_obj = dynamic_cast<TableObject *>(object);

				if(tab_obj && !upd_objects.contains(tab_obj->getParentTable()))
					upd_objects.push_back(tab_obj->getParentTable());
			}
		}

		for(auto &obj : upd_objects)
			obj->setModified(true);

		protected_model_frm->setVisible(db_model->isProtected());
		scene->blockSignals(false);
		scene->clearSelection();
		this->setModified(true);

		emit s_objectModified();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::cutObjects()
{
	/* Store the source model as 'this'. This attribute is used on the paste method
	to remove the selected object and updated the source model */
	ModelWidget::src_model=this;

	//Set the flag indicating that a cut operation started
	ModelWidget::cut_operation=true;
	this->copyObjects();
}

void ModelWidget::copyObjects(bool duplicate_mode)
{
	map<unsigned, BaseObject *> objs_map;
	map<unsigned, BaseObject *>::iterator obj_itr;
	vector<BaseObject *>::iterator itr, itr_end;
	vector<BaseObject *> deps;
	BaseObject *object=nullptr;
	TableObject *tab_obj=nullptr;
	BaseTable *table=nullptr;
	Constraint *constr=nullptr;
	ObjectType types[]={ ObjectType::Trigger, ObjectType::Rule, ObjectType::Index, ObjectType::Constraint, ObjectType::Policy };
	unsigned i, type_id, count;
	Messagebox msg_box;

	if(selected_objects.size()==1)
	{
		//Raise an error if the user try to copy a reserved object
		if(selected_objects[0]->isSystemObject())
			throw Exception(Exception::getErrorMessage(ErrorCode::OprReservedObject)
											.arg(selected_objects[0]->getName()).arg(selected_objects[0]->getTypeName()),
											ErrorCode::OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	if(!duplicate_mode)
	{
		//Ask for confirmation to copy the dependencies of the object(s)
		msg_box.show(tr("Also copy all dependencies of selected objects? This minimizes the breakdown of references when copied objects are pasted into another model."),
					 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);
	}

	/* When in cut operation is necessary to store the selected objects in a separeted list
	in order to correclty cut (remove) the object on the source model */
	if(ModelWidget::cut_operation)
		cutted_objects=selected_objects;

	itr=selected_objects.begin();
	itr_end=selected_objects.end();

	while(itr!=itr_end)
	{
		object=(*itr);

		//Table-view relationships and FK relationship aren't copied since they are created automatically when pasting the tables/views
		if(object->getObjectType()!=ObjectType::BaseRelationship)
		{
			if(msg_box.result()==QDialog::Accepted)
				db_model->getObjectDependecies(object, deps, true);
			else
				deps.push_back(object);

			/* Copying the special objects (which references columns added by relationship) in order
			to be correclty created when pasted */
			if(object->getObjectType()==ObjectType::Table || object->getObjectType() == ObjectType::View)
			{
				table=dynamic_cast<BaseTable *>(object);

				for(type_id=0; type_id <= 4; type_id++)
				{
					count=table->getObjectCount(types[type_id]);

					for(i=0; i < count; i++)
					{
						tab_obj=dynamic_cast<TableObject *>(table->getObject(i, types[type_id]));
						constr=dynamic_cast<Constraint *>(tab_obj);

						/* The object is only inserted at the list when it was not included by relationship but references
						columns added by relationship. Case the object is a constraint, it cannot be a primary key because
						this type of constraint is treated separetely by relationships */
						if(duplicate_mode ||
						   (!duplicate_mode &&
							!tab_obj->isAddedByRelationship() &&
							 (!constr ||
								(((constr &&
									(constr->getConstraintType()==ConstraintType::ForeignKey ||
									 (constr->getConstraintType()==ConstraintType::Unique &&
									constr->isReferRelationshipAddedColumn()))))))))
							deps.push_back(tab_obj);
					}

					if(object->getObjectType() == ObjectType::View && type_id >= 2)
						break;
				}
			}
		}
		itr++;
	}

	itr=deps.begin();
	itr_end=deps.end();

	//Storing the objects ids in a auxiliary vector
	while(itr!=itr_end)
	{
		object=(*itr);
		objs_map[object->getObjectId()]=object;
		itr++;
	}

	copied_objects.clear();
	obj_itr=objs_map.begin();
	while(obj_itr!=objs_map.end())
	{
		object=obj_itr->second;

		//Reserved object aren't copied
		if(!object->isSystemObject())
			copied_objects.push_back(object);

		obj_itr++;
	}
}

void ModelWidget::pasteObjects(bool duplicate_mode)
{
	map<BaseObject *, QString> xml_objs;
	BaseTable *orig_parent_tab=nullptr;
	vector<BaseObject *>::iterator itr, itr_end;
	map<BaseObject *, QString> orig_obj_names;
	BaseObject *object=nullptr, *aux_object=nullptr;
	TableObject *tab_obj=nullptr;
	Table *sel_table=nullptr, *aux_table = nullptr;
	View *sel_view=nullptr;
	BaseTable *parent=nullptr;
	Function *func=nullptr;
	Constraint *constr=nullptr;
	Operator *oper=nullptr;
	QString aux_name, copy_obj_name;
	ObjectType obj_type;
	vector<Exception> errors;
	unsigned pos=0;
	TaskProgressWidget task_prog_wgt(this);

	task_prog_wgt.setWindowTitle(tr("Pasting objects..."));
	task_prog_wgt.show();

	itr=copied_objects.begin();
	itr_end=copied_objects.end();

	/* If there is only one object selected, check if its a table or view.
	Because if the user try to paste a table object the receiver object (selected)
	must be a table or view */
	if(selected_objects.size()==1)
	{
		sel_table=dynamic_cast<Table *>(selected_objects[0]);
		sel_view=dynamic_cast<View *>(selected_objects[0]);
	}

	while(itr!=itr_end)
	{
		object=(*itr);
		obj_type=object->getObjectType();
		tab_obj=dynamic_cast<TableObject *>(object);
		itr++;
		pos++;
		task_prog_wgt.updateProgress((pos/static_cast<double>(copied_objects.size()))*100,
									 tr("Validating object: `%1' (%2)").arg(object->getName())
									 .arg(object->getTypeName()),
									 enum_cast(object->getObjectType()));

		if(!tab_obj || ((sel_table || sel_view) && tab_obj))
		{
			/* The first validation is to check if the object to be pasted does not conflict
			with any other object of the same type on the model */

			if(obj_type==ObjectType::Function)
				dynamic_cast<Function *>(object)->createSignature(true);
			else if(tab_obj)
				aux_name=tab_obj->getName(true);
			else
				aux_name=object->getSignature();

			if(!tab_obj)
				//Try to find the object on the model
				aux_object=db_model->getObject(aux_name, obj_type);
			else
			{
				if(sel_view && (obj_type==ObjectType::Trigger || obj_type==ObjectType::Rule || obj_type==ObjectType::Index))
					aux_object=sel_view->getObject(aux_name, obj_type);
				else if(sel_table)
					aux_object=sel_table->getObject(aux_name, obj_type);
			}

			/* The second validation is to check, when the object is found on the model, if the XML code of the found object
			 and the object to be pasted are different. When the XML defintion are the same the object isn't pasted because
			 the found object can be used as substitute of the object to be pasted. This operation is not applied to graphical
			 objects because they are ALWAYS pasted on the model. The only exception is that the below code is executed when the
			 found object is the same as the copied object (this means that user is copying and pasting the object at the same database) */
			if(tab_obj ||
					(aux_object &&
					 (dynamic_cast<BaseGraphicObject *>(object) ||
						(aux_object->getDatabase()==object->getDatabase()) ||
						(aux_object->getCodeDefinition(SchemaParser::SchemaParser::XmlDefinition) !=
						 object->getCodeDefinition(SchemaParser::SchemaParser::XmlDefinition)))))
			{
				//Resolving name conflicts
				if(obj_type!=ObjectType::Cast)
				{
					func=nullptr; oper=nullptr;

					//Store the orignal object name on a map
					orig_obj_names[object]=object->getName();

					/* For each object type as follow configures the name and the suffix and store them on the
						'copy_obj_name' variable. This string is used to check if there are objects with the same name
						on model. While the 'copy_obj_name' conflicts with other objects (of same type) this validation is made */
					if(obj_type==ObjectType::Function)
					{
						func=dynamic_cast<Function *>(object);
						func->setName(PgModelerNs::generateUniqueName(func, (*db_model->getObjectList(ObjectType::Function)), false, QString("_cp")));
						copy_obj_name=func->getName();
						func->setName(orig_obj_names[object]);
					}
					else if(obj_type==ObjectType::Operator)
					{
						oper=dynamic_cast<Operator *>(object);
						oper->setName(PgModelerNs::generateUniqueName(oper, (*db_model->getObjectList(ObjectType::Operator))));
						copy_obj_name=oper->getName();
						oper->setName(orig_obj_names[object]);
					}
					else
					{
						if(tab_obj)
						{
							if(sel_table)
								tab_obj->setName(PgModelerNs::generateUniqueName(tab_obj, (*sel_table->getObjectList(tab_obj->getObjectType())), false, QString("_cp"), true));
							else
								tab_obj->setName(PgModelerNs::generateUniqueName(tab_obj, (*sel_view->getObjectList(tab_obj->getObjectType())), false, QString("_cp"), true));
						}
						else
							object->setName(PgModelerNs::generateUniqueName(object, (*db_model->getObjectList(object->getObjectType())), false, QString("_cp"), true));

						copy_obj_name=object->getName();
						object->setName(orig_obj_names[object]);
					}

					//Sets the new object name concatenating the suffix to the original name
					object->setName(copy_obj_name);
				}
			}
		}
	}

	/* The third step is get the XML code definition of the copied objects, is
	with the xml code that the copied object are created and inserted on the model */
	itr=copied_objects.begin();
	itr_end=copied_objects.end();
	pos=0;
	while(itr!=itr_end)
	{
		object=(*itr);
		object->setCodeInvalidated(true);

		tab_obj=dynamic_cast<TableObject *>(object);
		itr++;

		pos++;
		task_prog_wgt.updateProgress((pos/static_cast<double>(copied_objects.size()))*100,
									 tr("Generating XML for: `%1' (%2)").arg(object->getName())
									 .arg(object->getTypeName()),
									 enum_cast(object->getObjectType()));

		if(!tab_obj)
		{
			aux_table =  dynamic_cast<Table *>(object);;

			//Stores the XML definition on a xml buffer map
			if(duplicate_mode && aux_table)
			{
				xml_objs[object] = aux_table->__getCodeDefinition(SchemaParser::XmlDefinition, true);
			  object->setCodeInvalidated(true);
			}
			else
				xml_objs[object]=object->getCodeDefinition(SchemaParser::XmlDefinition);
		}

		//Store the original parent table of the object
		else if(tab_obj && (sel_table || sel_view))
		{
			if(sel_table)
				parent=sel_table;
			else
				parent=sel_view;

			/* Only generates the XML for a table object when the selected receiver object
			 * is a table or is a view and the current object is a trigger, index, or rule (because
			 * view's only accepts this two types) */
			if(sel_table ||
					(sel_view && (tab_obj->getObjectType()==ObjectType::Trigger ||
									tab_obj->getObjectType()==ObjectType::Rule ||
									tab_obj->getObjectType()==ObjectType::Index)))
			{
				//Backups the original parent table
				orig_parent_tab=tab_obj->getParentTable();

				constr = dynamic_cast<Constraint *>(tab_obj);

				//Set the parent table as the selected table/view
				tab_obj->setParentTable(parent);

				//Generates the XML code with the new parent table
				if(constr)
				{
					xml_objs[object]=constr->getCodeDefinition(SchemaParser::XmlDefinition, duplicate_mode);
				  tab_obj->setCodeInvalidated(true);
				}
				else
					xml_objs[object]=object->getCodeDefinition(SchemaParser::XmlDefinition);

				//Restore the original parent table
				tab_obj->setParentTable(orig_parent_tab);
			}
		}
		else if(tab_obj)
		{
			//Generates the XML code with the new parent table
			constr = dynamic_cast<Constraint *>(tab_obj);

			if(constr)
			{
				xml_objs[object]=constr->getCodeDefinition(SchemaParser::XmlDefinition, duplicate_mode);
			  tab_obj->setCodeInvalidated(true);
			}
			else
				xml_objs[object]=tab_obj->getCodeDefinition(SchemaParser::XmlDefinition);
		}
	}

	//The fourth step is the restoration of original names of the copied objects
	itr=copied_objects.begin();
	itr_end=copied_objects.end();

	while(itr!=itr_end)
	{
		object = (*itr);
		obj_type = object->getObjectType();
		itr++;

		if(orig_obj_names[object].count() && obj_type!=ObjectType::Cast)
			object->setName(orig_obj_names[object]);
	}

	//The last step is create the object from the stored xmls
	itr=copied_objects.begin();
	itr_end=copied_objects.end();
	pos=0;

	op_list->startOperationChain();

	while(itr!=itr_end)
	{
		object = *itr;
		itr++;

		if(xml_objs.count(object))
		{
			xmlparser->restartParser();
			xmlparser->loadXMLBuffer(xml_objs[object]);

			try
			{
				pos++;
				task_prog_wgt.updateProgress((pos/static_cast<double>(copied_objects.size()))*100,
											 tr("Pasting object: `%1' (%2)").arg(object->getName())
											 .arg(object->getTypeName()),
											 enum_cast(object->getObjectType()));

				//Creates the object from the XML
				object=db_model->createObject(BaseObject::getObjectType(xmlparser->getElementName()));
				tab_obj=dynamic_cast<TableObject *>(object);
				constr=dynamic_cast<Constraint *>(tab_obj);

				/* Once created, the object is added on the model, except for relationships and table objects
				 * because they are inserted automatically */
				if(object && !tab_obj && !dynamic_cast<Relationship *>(object))
				{
					if(db_model->getObjectIndex(object->getSignature(), object->getObjectType()) >= 0)
						object->setName(PgModelerNs::generateUniqueName(object, *db_model->getObjectList(object->getObjectType()), false, QString("_cp")));

					db_model->addObject(object);
				}

				//Special case for table objects
				if(tab_obj)
				{
					if(sel_table && tab_obj->getObjectType()==ObjectType::Column)
					{
						sel_table->addObject(tab_obj);
						sel_table->setModified(true);
					}
					else if(constr && duplicate_mode &&
							constr->getConstraintType() == ConstraintType::PrimaryKey &&
							constr->getParentTable()->getObjectIndex(constr) < 0)
					{
					  constr->getParentTable()->addObject(constr);
					  constr->getParentTable()->setModified(true);
					}

					//Updates the fk relationships if the constraint is a foreign-key
					if(constr && constr->getConstraintType()==ConstraintType::ForeignKey)
						db_model->updateTableFKRelationships(dynamic_cast<Table *>(tab_obj->getParentTable()));

					op_list->registerObject(tab_obj, Operation::ObjectCreated, -1, tab_obj->getParentTable());
				}
				else
					op_list->registerObject(object, Operation::ObjectCreated);
			}
			catch(Exception &e)
			{
				errors.push_back(e);
			}
		}
	}
	op_list->finishOperationChain();

	//Validates the relationships to reflect any modification on the tables structures and not propagated columns
	db_model->validateRelationships();

	this->adjustSceneSize();
	task_prog_wgt.close();

	//If some error occur during the process show it to the user
	if(!errors.empty())
	{
		Messagebox msg_box;
		msg_box.show(Exception(tr("Not all objects were pasted to the model due to errors returned during the process! Refer to error stack for more details!"),
								 ErrorCode::Custom,__PRETTY_FUNCTION__,__FILE__,__LINE__, errors), QString(),
								 Messagebox::AlertIcon);
	}

	if(!ModelWidget::cut_operation)
	{
		//copied_objects.clear();
		emit s_objectCreated();
	}
	//If its a cut operatoin
	else
	{
		//Remove the objects from the source model
		ModelWidget::src_model->selected_objects=ModelWidget::cutted_objects;
		ModelWidget::src_model->removeObjects(false);

		//Uncheck the cut operation flag
		ModelWidget::cut_operation=false;

		copied_objects.clear();
		cutted_objects.clear();

		if(this!=ModelWidget::src_model)
			ModelWidget::src_model->configurePopupMenu();

		ModelWidget::src_model=nullptr;
	}

	this->configurePopupMenu();
	setModified(true);

	//Restoring the viewport position after paste objects
	viewport->verticalScrollBar()->setValue(db_model->getLastPosition().y());
	viewport->horizontalScrollBar()->setValue(db_model->getLastPosition().x());
}

void ModelWidget::duplicateObject()
{
	int op_id = -1;

	try
	{
		if(scene->hasOnlyTableChildrenSelection() ||
			 (selected_objects.size() == 1 && TableObject::isTableObject(selected_objects[0]->getObjectType())))
		{
			Schema *schema = nullptr;
			BaseObject *dup_object=nullptr;
			BaseTable *table = nullptr;
			ObjectType obj_type;
			QList<Schema *> upd_schemas;
			QList<BaseTable *> upd_view_ref_tables;
			QList<BaseTable *> upd_tables;
			QList<BaseTable *> upd_fk_rels;

			op_list->startOperationChain();

			for(auto &tab_obj : selected_objects)
			{
				dup_object=nullptr;
				obj_type = tab_obj->getObjectType();
				table = dynamic_cast<TableObject *>(tab_obj)->getParentTable();
				schema = dynamic_cast<Schema *>(table->getSchema());
				PgModelerNs::copyObject(&dup_object, tab_obj, obj_type);

				if(PhysicalTable::isPhysicalTable(table->getObjectType()))
					dup_object->setName(PgModelerNs::generateUniqueName(dup_object, *dynamic_cast<PhysicalTable *>(table)->getObjectList(obj_type), false, QString("_cp")));
				else
					dup_object->setName(PgModelerNs::generateUniqueName(dup_object, *dynamic_cast<View *>(table)->getObjectList(obj_type), false, QString("_cp")));

				op_id=op_list->registerObject(dup_object, Operation::ObjectCreated, -1, table);
				table->addObject(dup_object);

				// Flagging the table to be repainted
				if(!upd_tables.contains(table))
					upd_tables.append(table);

				// Flagging the schema to be repainted
				if(!upd_schemas.contains(schema))
					upd_schemas.append(schema);

				// Flagging the table to have the view references (relationships) updated
				if(!upd_view_ref_tables.contains(table) && obj_type == ObjectType::Column)
					upd_view_ref_tables.append(table);
				// Flagging the table to have its fk relationships updated
				else if(!upd_fk_rels.contains(table) &&
								obj_type == ObjectType::Constraint &&
								dynamic_cast<Constraint *>(tab_obj)->getConstraintType() == ConstraintType::ForeignKey)
					upd_fk_rels.append(table);
			}

			op_list->finishOperationChain();
			scene->clearSelection();

			for(auto &tab : upd_tables)
				tab->setModified(true);

			for(auto &sch : upd_schemas)
				sch->setModified(true);

			for(auto &tab : upd_view_ref_tables)
			{
				db_model->validateRelationships();
				db_model->updateViewsReferencingTable(dynamic_cast<PhysicalTable *>(tab));
			}

			for(auto &tab : upd_fk_rels)
				db_model->updateTableFKRelationships(dynamic_cast<Table *>(tab));

			this->setModified(true);
			emit s_objectCreated();
		}
		else if(!selected_objects.empty())
		{
			copyObjects(true);
			pasteObjects(true);
		}
	}
	catch(Exception &e)
	{
		//If operation was registered
		if(op_id >= 0)
			op_list->removeLastOperation();

		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::removeObjects(bool cascade)
{
	int obj_idx=-1;
	unsigned count, op_count=0, obj_id=0;
	Table *aux_table=nullptr;
	BaseTable *table=nullptr, *src_table=nullptr, *dst_table=nullptr;
	BaseRelationship *rel=nullptr;
	TableObject *tab_obj=nullptr;
	ObjectType obj_type=ObjectType::BaseObject, parent_type=ObjectType::BaseObject;
	BaseObject *object=nullptr, *aux_obj=nullptr;
	vector<BaseObject *> sel_objs, aux_sel_objs;

	map<unsigned, tuple<BaseObject *, QString, ObjectType, QString, ObjectType>> objs_map;
	map<unsigned, tuple<BaseObject *, QString, ObjectType, QString, ObjectType>>::reverse_iterator ritr, ritr_end;
	QAction *obj_sender=dynamic_cast<QAction *>(sender());
	QString obj_name, parent_name;
	vector<Exception> errors;

	if(obj_sender)
		object=reinterpret_cast<BaseObject *>(obj_sender->data().value<void *>());

	if(!object)
		sel_objs=selected_objects;
	else
		sel_objs.push_back(object);

	if(!sel_objs.empty())
	{
		Messagebox msg_box;

		//Cancel the cut operation if the user try to delete an object in the middle of the process
		if(ModelWidget::cut_operation && (sender()==action_remove || sender()==action_cascade_del))
		{
			ModelWidget::cut_operation=false;
			copied_objects.clear();
		}

		//If the removal is not due to a cut operation, ask for permission to remove the objects
		if(!ModelWidget::cut_operation)
		{
			if(cascade)
				msg_box.show(tr("<strong>CAUTION:</strong> You are about to delete objects in cascade mode which means more objects than the selected will be dropped too. Do you really want to proceed?"),
							 Messagebox::AlertIcon, Messagebox::YesNoButtons);
			else if(sel_objs.size() > 1)
			{
				msg_box.show(tr("<strong>CAUTION:</strong> Remove multiple objects at once can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?"),
							 Messagebox::AlertIcon, Messagebox::YesNoButtons);
			}
			else
			{
				if(sel_objs[0]->getObjectType()==ObjectType::Relationship)
					msg_box.show(tr("<strong>CAUTION:</strong> Remove a relationship can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?"),
								 Messagebox::AlertIcon, Messagebox::YesNoButtons);
				else
					msg_box.show(tr("Do you really want to delete the selected object?"),
								 Messagebox::ConfirmIcon, Messagebox::YesNoButtons);
			}
		}

		//If the user confirmed the removal or its a cut operation
		if(msg_box.result()==QDialog::Accepted || ModelWidget::cut_operation)
		{
			try
			{
				//If in cascade mode, retrieve all references to the object (direct and indirect)
				if(cascade)
				{
					vector<BaseObject *> refs;

					for(BaseObject *sel_obj : sel_objs)
					{
						refs.clear();
						db_model->__getObjectReferences(sel_obj, refs);

						for(BaseObject *ref_obj : refs)
						{
							obj_id=ref_obj->getObjectId();
							tab_obj=dynamic_cast<TableObject *>(ref_obj);

							//Store the base relationships in a auxiliary list to be processed ahead
							if(ref_obj->getObjectType()==ObjectType::BaseRelationship)
							{
								aux_sel_objs.push_back(ref_obj);
							}
							//Insert the reference object to the list of objects to be removed
							else if(objs_map.count(obj_id)==0 &&
									(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship())))
							{
								parent_type=(tab_obj ? tab_obj->getParentTable()->getObjectType() : ObjectType::Database);
								parent_name=(tab_obj ? tab_obj->getParentTable()->getName(true) : QString());
								obj_name=(tab_obj ? tab_obj->getName() : ref_obj->getSignature());

								objs_map[ref_obj->getObjectId()]=std::make_tuple(ref_obj,
																				 obj_name,
																				 ref_obj->getObjectType(),
																				 parent_name,
																				 parent_type);
							}
						}
					}
				}

				sel_objs.insert(sel_objs.end(), aux_sel_objs.begin(), aux_sel_objs.end());

				for(BaseObject *object : sel_objs)
				{
					obj_type=object->getObjectType();
					obj_id=object->getObjectId();

					//If the object is as FK relationship remove the foreign keys that generates it
					if(obj_type==ObjectType::BaseRelationship)
					{
						rel = dynamic_cast<BaseRelationship *>(object);

						if(rel->getRelationshipType()==BaseRelationship::RelationshipFk)
						{
							tab_obj=rel->getReferenceForeignKey();
							obj_id=tab_obj->getObjectId();

							if(objs_map.count(obj_id)==0)
							{
								objs_map[tab_obj->getObjectId()]=std::make_tuple(tab_obj,
																				 tab_obj->getName(true),
																				 tab_obj->getObjectType(),
																				 tab_obj->getParentTable()->getName(true),
																				 ObjectType::Table);

							}
						}
					}
					else if(objs_map.count(obj_id)==0)
					{
						tab_obj=dynamic_cast<TableObject *>(object);
						obj_name=(tab_obj ? object->getName(true) : object->getSignature());

						parent_name=(tab_obj ? tab_obj->getParentTable()->getName(true) : QString());
						parent_type=(tab_obj ? tab_obj->getParentTable()->getObjectType() : ObjectType::Database);

						objs_map[object->getObjectId()]=std::make_tuple(object,
																		obj_name,
																		obj_type,
																		parent_name,
																		parent_type);
					}
				}

				rel=nullptr;
				ritr=objs_map.rbegin();
				ritr_end=objs_map.rend();

				op_count=op_list->getCurrentSize();
				op_list->startOperationChain();

				do
				{
					object=std::get<0>(ritr->second);
					obj_name=std::get<1>(ritr->second);
					obj_type=std::get<2>(ritr->second);
					parent_name=std::get<3>(ritr->second);
					parent_type=std::get<4>(ritr->second);
					ritr++;

					if(obj_type==ObjectType::BaseRelationship)
						continue;
					else if(parent_type!=ObjectType::Database)
					{
						/* If the parent table does not exist on the model of the object to be removed
						 * does not exists in parent table, it'll not be processed */
						table=dynamic_cast<BaseTable *>(db_model->getObject(parent_name, parent_type));
						if(!table || (table && table->getObjectIndex(obj_name, obj_type) < 0))
							continue;
					}
					else
					{
						//If the object does not exists on the model it'll not be processed.
						aux_obj=db_model->getObject(obj_name, obj_type);
						if(aux_obj!=object)
							continue;
					}

					//Raises an error if the user try to remove a reserved object
					if(object->isSystemObject())
						throw Exception(Exception::getErrorMessage(ErrorCode::OprReservedObject)
														.arg(object->getName()).arg(object->getTypeName()),
														ErrorCode::OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					//Raises an error if the user try to remove a protected object
					else if(object->isProtected())
					{
						throw Exception(Exception::getErrorMessage(ErrorCode::RemProtectedObject)
														.arg(object->getName(true))
														.arg(object->getTypeName()),
														ErrorCode::RemProtectedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else
					{
						tab_obj=dynamic_cast<TableObject *>(object);

						if(tab_obj)
						{
							if(tab_obj->isAddedByRelationship())
							{
								throw Exception(Exception::getErrorMessage(ErrorCode::RemProtectedObject)
																.arg(tab_obj->getName(true))
																.arg(tab_obj->getTypeName()),
																ErrorCode::RemProtectedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);
							}

							table=dynamic_cast<BaseTable *>(tab_obj->getParentTable());
							obj_idx=table->getObjectIndex(tab_obj->getName(true), obj_type);

							try
							{
								//If the object is a column validates the column removal before remove it
								if(!cascade && obj_type==ObjectType::Column)
									db_model->validateColumnRemoval(dynamic_cast<Column *>(tab_obj));

								//Register the removed object on the operation list
								table->removeObject(obj_idx, obj_type);
								op_list->registerObject(tab_obj, Operation::ObjectRemoved, obj_idx, table);

								db_model->removePermissions(tab_obj);

								aux_table=dynamic_cast<Table *>(table);

								if(aux_table && obj_type==ObjectType::Constraint &&
									 (dynamic_cast<Constraint *>(tab_obj)->getConstraintType()==ConstraintType::ForeignKey ||
										dynamic_cast<Constraint *>(tab_obj)->getConstraintType()==ConstraintType::Unique))
									db_model->updateTableFKRelationships(aux_table);

								table->setModified(true);
								dynamic_cast<Schema *>(table->getSchema())->setModified(true);

								if(aux_table)
									db_model->validateRelationships(tab_obj, aux_table);

								if(obj_type == ObjectType::Column)
									db_model->updateViewsReferencingTable(aux_table);
							}
							catch(Exception &e)
							{
								if(cascade && (e.getErrorCode()==ErrorCode::RemInvalidatedObjects ||
															 e.getErrorCode()==ErrorCode::RemDirectReference ||
															 e.getErrorCode()==ErrorCode::RemInderectReference ||
															 e.getErrorCode()==ErrorCode::RemProtectedObject ||
															 e.getErrorCode()==ErrorCode::OprReservedObject))
									errors.push_back(e);
								else
									throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
							}
						}
						else
						{
							obj_idx=db_model->getObjectIndex(object);

							if(obj_idx >=0 )
							{
								if(obj_type==ObjectType::Relationship)
								{
									rel=dynamic_cast<BaseRelationship *>(object);
									src_table=rel->getTable(BaseRelationship::SrcTable);
									dst_table=rel->getTable(BaseRelationship::DstTable);
								}

								try
								{
									db_model->removeObject(object, obj_idx);
									op_list->registerObject(object, Operation::ObjectRemoved, obj_idx);
								}
								catch(Exception &e)
								{
									if(cascade && (e.getErrorCode()==ErrorCode::RemInvalidatedObjects ||
																 e.getErrorCode()==ErrorCode::RemDirectReference ||
																 e.getErrorCode()==ErrorCode::RemInderectReference ||
																 e.getErrorCode()==ErrorCode::RemProtectedObject ||
																 e.getErrorCode()==ErrorCode::OprReservedObject))
										errors.push_back(e);
									else
										throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
								}

								if(rel)
								{
									src_table->setModified(true);
									dst_table->setModified(true);
									rel=nullptr;
									dst_table=src_table=nullptr;
								}
							}
						}
					}
				}
				while(ritr!=ritr_end);

				op_list->finishOperationChain();
				scene->clearSelection();
				this->configurePopupMenu();
				setModified(true);
				emit s_objectRemoved();

				if(!errors.empty())
				{
					msg_box.show(Exception(ErrorCode::RemInvalidatedObjects, __PRETTY_FUNCTION__,__FILE__,__LINE__, errors),
								 tr("The cascade deletion found some problems when running! Some objects could not be deleted or registered in the operation's history! Please, refer to error stack for more details."),
								 Messagebox::AlertIcon);
				}
			}
			catch(Exception &e)
			{
				if(op_list->isOperationChainStarted())
					op_list->finishOperationChain();

				if(op_count < op_list->getCurrentSize())
				{
					count=op_list->getCurrentSize()-op_count;
					op_list->ignoreOperationChain(true);

					for(unsigned i=0; i < count; i++)
						op_list->removeLastOperation();

					op_list->ignoreOperationChain(false);
				}

				scene->clearSelection();
				setModified(true);
				emit s_objectRemoved();
				msg_box.show(e);
			}

			/* In case of any object removal we clear the copied objects list in order to avoid
			 * segfaults when trying to paste an object that was removed previously */
			copied_objects.clear();
		}
	}
}

void ModelWidget::removeObjectsCascade()
{
	removeObjects(true);
}

void ModelWidget::editCustomSQL()
{
	QAction *act=dynamic_cast<QAction *>(sender());
	BaseObject *obj=reinterpret_cast<BaseObject *>(act->data().value<void *>());
	CustomSQLWidget *customsql_wgt=new CustomSQLWidget;

	customsql_wgt->setAttributes(db_model, obj);
	setModified((openEditingForm(customsql_wgt)==QDialog::Accepted));
}

void ModelWidget::showObjectMenu()
{
	BaseTableView *tab=nullptr;

	/* When the popup is hidden check if there is a table object (colum, constraint, etc) selected,
		 if so, is necessary to reenable the table view deactivated before the menu activation */
	if(this->selected_objects.size()==1)
	{
		//Get the selected table object
		TableObject *tab_obj=dynamic_cast<TableObject *>(this->selected_objects[0]);

		if(tab_obj && tab_obj->getParentTable())
			//Get the graphical representation for table
			tab=dynamic_cast<BaseTableView *>(tab_obj->getParentTable()->getOverlyingObject());
	}

	magnifier_area_lbl->hide();
	popup_menu.exec(QCursor::pos());

	//If the table object has a parent table
	if(tab)
	{
		//Reacitvates the table
		tab->setEnabled(true);
		//Calls the hoverLeaveEvent in order to hide the child selection
		tab->hoverLeaveEvent(nullptr);
	}
}

void ModelWidget::configurePopupMenu(BaseObject *object)
{
	vector<BaseObject *> vet_obj;

	if(object)
		vet_obj.push_back(object);

	configurePopupMenu(vet_obj);
}

void ModelWidget::enableModelActions(bool value)
{
	action_source_code->setEnabled(value);
	action_edit->setEnabled(value);
	action_protect->setEnabled(value);
	action_unprotect->setEnabled(value);
	action_select_all->setEnabled(value);
	action_convert_relnn->setEnabled(value);
	action_convert_rel1n->setEnabled(value);
	action_deps_refs->setEnabled(value);
	action_new_object->setEnabled(value);
	action_copy->setEnabled(value);
	action_duplicate->setEnabled(value);
	action_paste->setEnabled(value);
	action_cut->setEnabled(value);
	action_remove->setEnabled(value);
	action_cascade_del->setEnabled(value);
	action_quick_actions->setEnabled(value);
}

void ModelWidget::configureQuickMenu(BaseObject *object)
{
	QAction *act=nullptr;
	vector<BaseObject *> sel_objs;
	ObjectType obj_type=ObjectType::BaseObject;
	bool tab_or_view=false, is_graph_obj = false, accepts_owner=false, accepts_schema=false;

	if(object)
		sel_objs.push_back(object);
	else
		sel_objs = selected_objects;

	/* Determining if one or more selected objects accepts schema, owner or are table/views,
	 this is done to correctly show the actions to the user */
	for(BaseObject *obj : sel_objs)
	{
		obj_type=obj->getObjectType();

		if(!tab_or_view)
			tab_or_view=BaseTable::isBaseTable(obj_type);

		if(!is_graph_obj)
			is_graph_obj = BaseGraphicObject::isGraphicObject(obj_type);

		if(!accepts_owner)
			accepts_owner=obj->acceptsOwner();

		if(!accepts_schema)
			accepts_schema=obj->acceptsSchema();

		if(tab_or_view && accepts_owner && accepts_schema)
			break;
	}

	if(!sel_objs.empty())
	{
		if(accepts_owner || accepts_schema)
		{
			vector<BaseObject *> obj_list;
			map<QString, QAction *> act_map;
			QStringList name_list;
			QMenu *menus[]={ &schemas_menu, &owners_menu, &tags_menu };
			ObjectType types[]={ ObjectType::Schema, ObjectType::Role, ObjectType::Tag };

			for(unsigned i=0; i < 3; i++)
			{
				menus[i]->clear();

				//Configuring actions "Move to schema", "Change Owner" and "Set tag"
				if((types[i] == ObjectType::Schema && accepts_schema) ||
						(types[i] == ObjectType::Role && accepts_owner) ||
						(types[i]==ObjectType::Tag && tab_or_view))
				{
					obj_list=db_model->getObjects(types[i]);

					if(obj_list.empty())
					{
						menus[i]->addAction(tr("(no objects)"));
						menus[i]->actions().at(0)->setEnabled(false);
					}
					else
					{
						if(types[i] == ObjectType::Tag)
						{
							menus[i]->addAction(tr("None"), this, SLOT(setTag()));
							menus[i]->addSeparator();
						}
						else if(types[i] == ObjectType::Role)
						{
							act = menus[i]->addAction(tr("None"), this, SLOT(changeOwner()));
							menus[i]->addSeparator();
						}

						while(!obj_list.empty())
						{
							act=new QAction(obj_list.back()->getName(), menus[i]);
							act->setIcon(QPixmap(PgModelerUiNs::getIconPath(types[i])));

							/* Check the current action only if there is only one selected object and the object representing
								 the action is assigned to the selected object */
							act->setCheckable(sel_objs.size() == 1);
							act->setChecked(sel_objs.size() == 1 &&
															(object->getSchema() == obj_list.back() ||
															 object->getOwner() == obj_list.back() ||
															 (tab_or_view && dynamic_cast<BaseTable *>(sel_objs[0])->getTag()==obj_list.back())));

							act->setEnabled(!act->isChecked());
							act->setData(QVariant::fromValue<void *>(obj_list.back()));

							if(i==0)
								connect(act, SIGNAL(triggered(bool)), this, SLOT(moveToSchema()));
							else if(i==1)
								connect(act, SIGNAL(triggered(bool)), this, SLOT(changeOwner()));
							else
								connect(act, SIGNAL(triggered(bool)), this, SLOT(setTag()));

							act_map[obj_list.back()->getName()]=act;
							name_list.push_back(obj_list.back()->getName());
							obj_list.pop_back();
						}

						name_list.sort();
						while(!name_list.isEmpty())
						{
							menus[i]->addAction(act_map[name_list.front()]);
							name_list.pop_front();
						}

						act_map.clear();
					}
				}
			}
		}

		// Configuring the layers menu
		if(is_graph_obj)
		{
			unsigned layer_id = ObjectsScene::DefaultLayer;
			layers_menu.clear();

			for(auto &layer : scene->getLayers())
			{
				act = layers_menu.addAction(layer);
				act->setData(layer_id++);
				connect(act, SIGNAL(triggered(bool)), this, SLOT(moveToLayer()));
			}
		}

		//Display the quick rename action is a single object is selected
		if((object && obj_type != ObjectType::Cast) || (sel_objs.size() > 1))
		{
			quick_actions_menu.addAction(action_rename);			
			action_rename->setData(QVariant::fromValue<void *>(object));
		}

		if(accepts_schema)
			quick_actions_menu.addAction(action_moveto_schema);

		if(is_graph_obj)
			quick_actions_menu.addAction(action_moveto_layer);

		if(accepts_owner)
			quick_actions_menu.addAction(action_change_owner);

		if(tab_or_view)
			quick_actions_menu.addAction(action_set_tag);

		if(object && Permission::acceptsPermission(obj_type))
		{
			quick_actions_menu.addAction(action_edit_perms);
			action_edit_perms->setData(QVariant::fromValue<void *>(object));
		}

		if(object && PhysicalTable::isPhysicalTable(obj_type))
			quick_actions_menu.addAction(action_edit_data);

		if(object && BaseObject::acceptsCustomSQL(obj_type))
		{
			action_append_sql->setData(QVariant::fromValue<void *>(object));
			quick_actions_menu.addAction(action_append_sql);
		}

		if(object &&
				((!TableObject::isTableObject(obj_type) && obj_type!=ObjectType::Textbox && obj_type!=ObjectType::BaseRelationship) ||
				 (TableObject::isTableObject(obj_type) && !dynamic_cast<TableObject*>(object)->isAddedByRelationship())))
		{
			action_enable_sql->setData(QVariant::fromValue<void *>(object));
			action_disable_sql->setData(QVariant::fromValue<void *>(object));

			if(object->isSQLDisabled())
				quick_actions_menu.addAction(action_enable_sql);
			else
				quick_actions_menu.addAction(action_disable_sql);
		}

		//Include the quick actions if it is not empty and the model is not protected
		if(!db_model->isProtected() && !quick_actions_menu.isEmpty())
			popup_menu.addAction(action_quick_actions);
	}
}

void ModelWidget::configureFadeMenu()
{
	bool is_db_selected = (selected_objects.empty() ||
												 (selected_objects.size() == 1 && selected_objects[0]->getObjectType() == ObjectType::Database));
	fade_menu.clear();
	fade_in_menu.clear();
	fade_out_menu.clear();

	if(is_db_selected || (selected_objects.size() > 1 && !scene->hasOnlyTableChildrenSelection()))
	{
		fade_menu.addAction(action_fade_in);
		fade_menu.addAction(action_fade_out);
		action_fade_in->setMenu(&fade_in_menu);
		action_fade_out->setMenu(&fade_out_menu);

		if(is_db_selected)
		{
			QAction *action = nullptr;
			vector<ObjectType> types = { ObjectType::Schema, ObjectType::Table, ObjectType::View, ObjectType::Relationship, ObjectType::Textbox };
			QStringList labels = { tr("Schemas"), tr("Tables"), tr("Views"), tr("Relationships"), tr("Textboxes") };
			unsigned id = 0;

			for(ObjectType type : types)
			{
				action = new QAction(QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(type) + QString("_grp"))),
																		 labels[id], &fade_in_menu);
				action->setData(enum_cast(type));
				fade_in_menu.addAction(action);
				connect(action, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsIn()));

				action = new QAction(QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(type) + QString("_grp"))),
																		 labels[id], &fade_out_menu);
				action->setData(enum_cast(type));
				fade_out_menu.addAction(action);

				id++;
				connect(action, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsOut()));
			}

			action = new QAction(tr("All objects"), &fade_in_menu);
			action->setData(enum_cast(ObjectType::BaseObject));
			connect(action, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsIn()));
			fade_in_menu.addSeparator();
			fade_in_menu.addAction(action);

			action = new QAction(tr("All objects"), &fade_out_menu);
			action->setData(enum_cast(ObjectType::BaseObject));
			connect(action, SIGNAL(triggered(bool)), this, SLOT(fadeObjectsOut()));
			fade_out_menu.addSeparator();
			fade_out_menu.addAction(action);
		}
		else
		{
			action_fade_in->setMenu(nullptr);
			action_fade_out->setMenu(nullptr);
		}
	}
	else if(selected_objects.size() == 1)
	{
		ObjectType obj_type = selected_objects[0]->getObjectType();

		if(obj_type == ObjectType::Tag)
		{
			fade_menu.addAction(action_fade_in);
			fade_menu.addAction(action_fade_out);
			action_fade_in->setMenu(nullptr);
			action_fade_out->setMenu(nullptr);
		}
		else
		{
			BaseObjectView *obj_view = dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(selected_objects[0])->getOverlyingObject());

			if(obj_view)
			{
				if(obj_view->opacity() == 1)
				{
					fade_menu.addAction(action_fade_out);
					action_fade_out->setMenu(nullptr);
				}
				else
				{
					fade_menu.addAction(action_fade_in);
					action_fade_in->setMenu(nullptr);
				}
			}

			if(obj_type == ObjectType::Table || obj_type == ObjectType::View)
			{
				fade_menu.addAction(action_fade_rels);
				action_fade_rels->setText(tr("Table && Relationships"));
			}
		}
	}
}

void ModelWidget::fadeObjects(const vector<BaseObject *> &objects, bool fade_in)
{
	BaseObjectView *obj_view = nullptr;
	Schema *schema = nullptr;

	for(auto obj : objects)
	{
	  schema = dynamic_cast<Schema *>(obj);

	  if(!BaseGraphicObject::isGraphicObject(obj->getObjectType()) ||
		 (schema && !schema->isRectVisible()))
			continue;

		obj_view = dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(obj)->getOverlyingObject());

		if(obj_view)
		{
			dynamic_cast<BaseGraphicObject *>(obj)->setFadedOut(!fade_in);

			obj_view->setOpacity(fade_in ? 1 : min_object_opacity);

			//If the minimum opacity is zero the object hidden
			obj_view->setVisible(scene->isLayerActive(obj_view->getLayer()) && (fade_in || (!fade_in && min_object_opacity > 0)));

			setModified(true);
		}
	}

	scene->clearSelection();
}

void ModelWidget::fadeObjects(QAction *action, bool fade_in)
{
	if(!action)
		return;

	vector<BaseObject *> list;

	//If the database object is selected or there is no object select
	if(selected_objects.empty() || (selected_objects.size() == 1 && selected_objects[0]->getObjectType() == ObjectType::Database))
	{
		ObjectType obj_type = static_cast<ObjectType>(action->data().toUInt());

		//If the action contains a data of type ObjectType::ObjBaseObject means that the user wants to fade all objects
		if(obj_type == ObjectType::BaseObject)
		{
			vector<ObjectType> types = { ObjectType::Schema, ObjectType::Table, ObjectType::View,
																	 ObjectType::Relationship, ObjectType::BaseRelationship, ObjectType::Textbox};

			for(ObjectType type : types)
			{
				list.insert(list.end(),
										db_model->getObjectList(type)->begin(),
										db_model->getObjectList(type)->end());
			}
		}
		else
		{
			//Fading objects of a certain type
			list = *db_model->getObjectList(obj_type);

			if(obj_type == ObjectType::Relationship)
			{
				list.insert(list.end(),
										db_model->getObjectList(ObjectType::BaseRelationship)->begin(),
										db_model->getObjectList(ObjectType::BaseRelationship)->end());
			}
		}
	}
	else
	{
		//For tag object the fade is applied in the tables/views related to it
		if(selected_objects.size() == 1 && selected_objects[0]->getObjectType() == ObjectType::Tag)
			db_model->getObjectReferences(selected_objects[0], list);
		else
		{
			if(action == action_fade_rels_in || action == action_fade_rels_out)
			{
				//Applying fade to the relationships linked to the selected table/view
				vector<BaseRelationship *> rel_list = db_model->getRelationships(dynamic_cast<BaseTable *>(selected_objects[0]));

				for(auto rel : rel_list)
				{
					list.push_back(rel);
					list.push_back(rel->getTable(BaseRelationship::SrcTable));
					list.push_back(rel->getTable(BaseRelationship::DstTable));
				}

				vector<BaseObject *>::iterator end;
				std::sort(list.begin(), list.end());
				end=std::unique(list.begin(), list.end());
				list.erase(end, list.end());
			}
			else
				//Applying fade to the selected objects
				list = selected_objects;
		}
	}

	fadeObjects(list, fade_in);
	scene->clearSelection();
}

void ModelWidget::fadeObjectsIn()
{
	fadeObjects(qobject_cast<QAction *>(sender()), true);
}

void ModelWidget::fadeObjectsOut()
{
	fadeObjects(qobject_cast<QAction *>(sender()), false);
}

void ModelWidget::setAllCollapseMode(CollapseMode mode)
{
	BaseTable *base_tab = nullptr;
	vector<BaseObject *> objects;

	this->scene->clearSelection();
	objects.assign(db_model->getObjectList(ObjectType::Table)->begin(), db_model->getObjectList(ObjectType::Table)->end());
	objects.insert(objects.end(), db_model->getObjectList(ObjectType::View)->begin(), db_model->getObjectList(ObjectType::View)->end());

	for(auto obj : objects)
	{
		base_tab = dynamic_cast<BaseTable *>(obj);

		if(base_tab)
			base_tab->setCollapseMode(mode);
	}

	this->setModified(true);
}

void ModelWidget::moveObjectsInZStack(int direction)
{
	BaseObjectView *obj_view = nullptr;
	BaseTableView *tab_view = nullptr;
	TextboxView *tbox_view = nullptr;
	QList<QGraphicsItem *> items;
	int zval = 0;
	bool changed = false;

	op_list->startOperationChain();

	for(auto &obj : selected_objects)
	{
		// Ignoring if the object is not a table or textbox
		if(!BaseTable::isBaseTable(obj->getObjectType()) &&
			 obj->getObjectType() != ObjectType::Textbox)
			continue;

		obj_view = dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(obj)->getOverlyingObject());

		// We'll check the items around the one we selected to move in the z stack
		items = obj_view->collidingItems();

		if(!obj_view->collidingItems().isEmpty())
		{
			zval = obj_view->zValue();

			for(auto &item : items)
			{
				tab_view  = dynamic_cast<BaseTableView *>(item);
				tbox_view  = dynamic_cast<TextboxView *>(item);

				// Ignoring if the object is not a table or textbox
				if(!tab_view && !tbox_view)
					continue;

				if((direction == BringToFront && zval < item->zValue()) ||
					 (direction == SendToBack && zval > item->zValue()))
					zval = item->zValue();
			}

			op_list->registerObject(obj_view->getUnderlyingObject(), Operation::ObjectModified);

			zval += (1 * direction);

			if(zval < BaseGraphicObject::MinZValue)
				zval = BaseGraphicObject::MinZValue;
			else if(zval > BaseGraphicObject::MaxZValue)
				zval = BaseGraphicObject::MaxZValue;

			obj_view->setZValue(zval);
			changed = true;
		}
	}

	op_list->finishOperationChain();
	setModified(changed);

	if(changed)
		emit s_objectModified();
}

void ModelWidget::setCollapseMode()
{
	CollapseMode mode = static_cast<CollapseMode>(dynamic_cast<QAction *>(sender())->data().toUInt());
	BaseTable *base_tab = nullptr;
	vector<BaseObject *> objects;

	if(selected_objects.empty() || (selected_objects.size() == 1 && selected_objects[0] == db_model))
	{
		objects.assign(db_model->getObjectList(ObjectType::Table)->begin(), db_model->getObjectList(ObjectType::Table)->end());
		objects.insert(objects.end(), db_model->getObjectList(ObjectType::ForeignTable)->begin(), db_model->getObjectList(ObjectType::ForeignTable)->end());
		objects.insert(objects.end(), db_model->getObjectList(ObjectType::View)->begin(), db_model->getObjectList(ObjectType::View)->end());
	}
	else
		objects = selected_objects;

	for(auto obj : objects)
	{
		base_tab = dynamic_cast<BaseTable *>(obj);

		if(base_tab && base_tab->getCollapseMode() != mode)
		{
			base_tab->setCollapseMode(mode);
			base_tab->setModified(true);
		}
	}

	db_model->setObjectsModified({ ObjectType::Schema });
	this->setModified(true);
}

void ModelWidget::togglePagination()
{
	bool enable = dynamic_cast<QAction *>(sender())->data().toBool();
	BaseTable *base_tab = nullptr;
	vector<BaseObject *> objects;

	if(selected_objects.empty() || (selected_objects.size() == 1 && selected_objects[0] == db_model))
	{
		objects.assign(db_model->getObjectList(ObjectType::Table)->begin(), db_model->getObjectList(ObjectType::Table)->end());
		objects.insert(objects.end(), db_model->getObjectList(ObjectType::ForeignTable)->begin(), db_model->getObjectList(ObjectType::ForeignTable)->end());
		objects.insert(objects.end(), db_model->getObjectList(ObjectType::View)->begin(), db_model->getObjectList(ObjectType::View)->end());
	}
	else
		objects = selected_objects;

	for(auto obj : objects)
	{
		base_tab = dynamic_cast<BaseTable *>(obj);

		if(base_tab && base_tab->isPaginationEnabled() != enable)
		{
			base_tab->setPaginationEnabled(enable);
			base_tab->setModified(true);
		}
	}

	db_model->setObjectsModified({ ObjectType::Schema });
	this->setModified(true);
}

void ModelWidget::toggleSchemasRectangles()
{
	bool visible = sender() == action_show_schemas_rects;
	Schema *schema = nullptr;

	for(auto obj : *db_model->getObjectList(ObjectType::Schema))
	{
		schema = dynamic_cast<Schema *>(obj);

		if(schema && schema->isRectVisible() != visible)
		{
			schema->setRectVisible(visible);
			schema->setModified(true);
		}
	}

	this->setModified(true);
}

void ModelWidget::updateObjectsOpacity()
{
	vector<ObjectType> types = { ObjectType::Schema, ObjectType::Table, ObjectType::View,
															 ObjectType::Relationship, ObjectType::BaseRelationship, ObjectType::Textbox};
	BaseObjectView *obj_view = nullptr;
	BaseGraphicObject *base_obj = nullptr;

	for(auto type : types)
	{
		for(auto object : *db_model->getObjectList(type))
		{
			base_obj = dynamic_cast<BaseGraphicObject *>(object);
			obj_view = dynamic_cast<BaseObjectView *>(base_obj->getOverlyingObject());

			if(obj_view &&
				 ((base_obj->isFadedOut() && obj_view->opacity() == 1) ||
					(obj_view->opacity() < 1.0 && obj_view->opacity() != min_object_opacity)))
			{
				obj_view->setOpacity(min_object_opacity);
				obj_view->setVisible(min_object_opacity > 0);
			}
		}
	}
}

void ModelWidget::configureConstraintsMenu(TableObject *tab_obj)
{
	if(!tab_obj)
		return;

	PhysicalTable *table=dynamic_cast<PhysicalTable *>(tab_obj->getParentTable());

	if(tab_obj->getObjectType()==ObjectType::Column)
	{
		unsigned count = table->getConstraintCount();
		Constraint *constr = nullptr;
		QString str_aux;
		QAction *action = nullptr;
		QMenu *submenu;
		QList<QMenu *> submenus;

		for(unsigned i=0; i < count; i++)
		{
			constr=table->getConstraint(i);
			if(constr->isColumnReferenced(dynamic_cast<Column *>(tab_obj), false))
			{
				switch(!constr->getConstraintType())
				{
					case ConstraintType::PrimaryKey: str_aux=QString("_%1").arg(TableObjectView::TextPrimaryKey); break;
					case ConstraintType::ForeignKey: str_aux=QString("_%1").arg(TableObjectView::TextForeignKey); break;
					case ConstraintType::Check: str_aux=QString("_%1").arg(TableObjectView::TextCheck); break;
					case ConstraintType::Unique: str_aux=QString("_%1").arg(TableObjectView::TextUnique); break;
					case ConstraintType::Exclude: str_aux=QString("_%1").arg(TableObjectView::TextExclude); break;
				}

				//For each constaint is created a menu with the edit, source code, protect/unprotect and delete actions
				submenu=new QMenu(&popup_menu);
				submenu->setIcon(QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(ObjectType::Constraint) + str_aux)));
				submenu->setTitle(constr->getName());

				action=new QAction(dynamic_cast<QObject *>(submenu));
				action->setIcon(QPixmap(PgModelerUiNs::getIconPath("editar")));
				action->setText(tr("Properties"));
				action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
				connect(action, SIGNAL(triggered(bool)), this, SLOT(editObject()));
				submenu->addAction(action);

				action=new QAction(dynamic_cast<QObject *>(submenu));
				action->setIcon(QPixmap(PgModelerUiNs::getIconPath("codigosql")));
				action->setText(tr("Source code"));
				action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
				connect(action, SIGNAL(triggered(bool)), this, SLOT(showSourceCode()));
				submenu->addAction(action);

				if(!constr->isAddedByRelationship())
				{
					if(!constr->getParentTable()->isProtected())
					{
						action=new QAction(dynamic_cast<QObject *>(&popup_menu));
						action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
						connect(action, SIGNAL(triggered(bool)), this, SLOT(protectObject()));
						submenu->addAction(action);

						if(constr->isProtected())
						{
							action->setIcon(QPixmap(PgModelerUiNs::getIconPath("desbloqobjeto")));
							action->setText(tr("Unprotect"));
						}
						else
						{
							action->setIcon(QPixmap(PgModelerUiNs::getIconPath("bloqobjeto")));
							action->setText(tr("Protect"));
						}
					}

					action=new QAction(dynamic_cast<QObject *>(submenu));
					action->setIcon(QPixmap(PgModelerUiNs::getIconPath("excluir")));
					action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
					action->setText(tr("Delete"));
					submenu->addAction(action);
					connect(action, SIGNAL(triggered()), this, SLOT(removeObjects()));

					action=new QAction(dynamic_cast<QObject *>(submenu));
					action->setIcon(QPixmap(PgModelerUiNs::getIconPath("delcascade")));
					action->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(constr)));
					action->setText(tr("Del. cascade"));
					submenu->addAction(action);
					connect(action, SIGNAL(triggered()), this, SLOT(removeObjectsCascade()));
				}
				submenus.push_back(submenu);
			}
		}

		//Adding the constraint submenus to the main popup menu
		if(!submenus.empty())
		{
			submenu=new QMenu(&popup_menu);
			submenu->setTitle(tr("Constraints"));
			submenu->setIcon(QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(ObjectType::Constraint) + QString("_grp"))));

			for(auto &menu : submenus)
				submenu->addMenu(menu);

			popup_menu.insertMenu(action_edit, submenu);
		}
	}

}

void ModelWidget::configureBasicActions(BaseObject *obj)
{
	if(!obj)
		return;

	BaseRelationship *rel=dynamic_cast<BaseRelationship *>(obj);
	ObjectType obj_type=obj->getObjectType();
	QAction *action = nullptr;

	configureQuickMenu(obj);
	popup_menu.addAction(action_edit);

	if((obj_type==ObjectType::Schema && obj->isSystemObject()) ||
			(!obj->isProtected() && (BaseTable::isBaseTable(obj_type) || obj_type==ObjectType::BaseRelationship ||
															 obj_type==ObjectType::Relationship || obj_type==ObjectType::Schema ||
															 obj_type == ObjectType::Tag)))
	{
		if(BaseTable::isBaseTable(obj_type))
		{
			for(auto type : BaseObject::getChildObjectTypes(obj_type))
				new_object_menu.addAction(actions_new_objects[type]);

			if(obj_type==ObjectType::Table)
				new_object_menu.addAction(actions_new_objects[ObjectType::Relationship]);

			action_new_object->setMenu(&new_object_menu);
			popup_menu.insertAction(action_quick_actions, action_new_object);
		}
		else if(obj_type==ObjectType::Relationship || obj_type==ObjectType::BaseRelationship)
		{
			if(obj_type==ObjectType::Relationship)
			{
				new_object_menu.addAction(actions_new_objects[ObjectType::Column]);
				new_object_menu.addAction(actions_new_objects[ObjectType::Constraint]);

				action_new_object->setMenu(&new_object_menu);
				popup_menu.insertAction(action_quick_actions, action_new_object);
			}

			if(rel->getRelationshipType()==Relationship::RelationshipNn)
			{
				action_convert_relnn->setData(QVariant::fromValue<void *>(rel));
				popup_menu.addAction(action_convert_relnn);
			}

			if(rel->getRelationshipType() == Relationship::Relationship11 ||
				 rel->getRelationshipType() == Relationship::Relationship1n)
			{
				action_convert_rel1n->setData(QVariant::fromValue<void *>(rel));
				popup_menu.addAction(action_convert_rel1n);
			}

			if(!rel->isSelfRelationship())
			{
				if(rel->getPoints().empty())
				{
					action_break_rel_line->setData(QVariant::fromValue<void *>(rel));
					popup_menu.addAction(action_break_rel_line);
				}
				else
				{
					action_remove_rel_points->setData(QVariant::fromValue<void *>(rel));
					popup_menu.addAction(action_remove_rel_points);
				}

				popup_menu.addAction(action_jump_to_table);
				jump_to_tab_menu.clear();

				action = jump_to_tab_menu.addAction(QIcon(PgModelerUiNs::getIconPath(rel->getTable(BaseRelationship::SrcTable)->getObjectType())),
																						rel->getTable(BaseRelationship::SrcTable)->getSignature(), this, SLOT(jumpToTable()));
				action->setData(QVariant::fromValue<void *>(reinterpret_cast<void *>(rel->getTable(BaseRelationship::SrcTable))));

				action = jump_to_tab_menu.addAction(QIcon(PgModelerUiNs::getIconPath(rel->getTable(BaseRelationship::DstTable)->getObjectType())),
																						rel->getTable(BaseRelationship::DstTable)->getSignature(), this, SLOT(jumpToTable()));
				action->setData(QVariant::fromValue<void *>(reinterpret_cast<void *>(rel->getTable(BaseRelationship::DstTable))));
			}
		}
		else if(obj_type == ObjectType::Schema)
		{
			for(auto type : BaseObject::getChildObjectTypes(ObjectType::Schema))
				new_object_menu.addAction(actions_new_objects[type]);

			action_new_object->setMenu(&new_object_menu);
			popup_menu.insertAction(action_quick_actions, action_new_object);

			popup_menu.addAction(action_sel_sch_children);
			action_sel_sch_children->setData(QVariant::fromValue<void *>(obj));
		}
		else if(obj_type == ObjectType::Tag)
		{
			popup_menu.addAction(action_sel_tagged_tabs);
			action_sel_tagged_tabs->setData(QVariant::fromValue<void *>(obj));
		}
	}

	/* Adding the action to highlight the object only when the sender is not one of the
	the objects that calls this method from inside the ModelWidget instance. This action
	is mainly used when the user wants to find a graphical object from the ModelObjects dockwidget*/
	if((sender()!=this && sender()!=scene) && dynamic_cast<BaseGraphicObject *>(obj))
	{
		popup_menu.addAction(action_select_object);
		action_select_object->setData(QVariant::fromValue<void *>(obj));
	}

	action_edit->setData(QVariant::fromValue<void *>(obj));
	action_source_code->setData(QVariant::fromValue<void *>(obj));
	action_deps_refs->setData(QVariant::fromValue<void *>(obj));
	TableObject *tab_obj = dynamic_cast<TableObject *>(obj);

	if(tab_obj &&  tab_obj->getObjectType()==ObjectType::Column)
	{
		Column *col=dynamic_cast<Column *>(tab_obj);

		if(tab_obj->isAddedByRelationship())
		{
			action_parent_rel->setData(QVariant::fromValue<void *>(dynamic_cast<Column *>(tab_obj)->getParentRelationship()));
			popup_menu.addAction(action_parent_rel);
		}
		else if(col->getType().isSerialType())
		{
			action_create_seq_col->setData(QVariant::fromValue<void *>(col));
			popup_menu.addAction(action_create_seq_col);
		}
		else if(col->getType().isIntegerType())
		{
			action_conv_int_serial->setData(QVariant::fromValue<void *>(col));
			popup_menu.addAction(action_conv_int_serial);
		}
	}

	popup_menu.addSeparator();
	popup_menu.addAction(action_source_code);

	if(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship()))
		popup_menu.addAction(action_deps_refs);
}

void ModelWidget::configureDatabaseActions()
{
	new_object_menu.addAction(action_database_category);
	new_object_menu.addAction(action_schema_category);
	new_object_menu.addAction(actions_new_objects[ObjectType::Relationship]);
	new_object_menu.addAction(actions_new_objects[ObjectType::GenericSql]);
	new_object_menu.addAction(actions_new_objects[ObjectType::Tag]);
	new_object_menu.addAction(actions_new_objects[ObjectType::Textbox]);
	action_new_object->setMenu(&new_object_menu);
	popup_menu.addAction(action_new_object);

	configureQuickMenu(db_model);

	action_edit->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(db_model)));
	action_source_code->setData(QVariant::fromValue<void *>(dynamic_cast<BaseObject *>(db_model)));

	popup_menu.addAction(action_edit);

	popup_menu.addSeparator();
	popup_menu.addAction(action_source_code);

	if(db_model->isProtected())
		popup_menu.addAction(action_unprotect);
	else
		popup_menu.addAction(action_protect);

	if(scene->items().count() > 1)
		popup_menu.addAction(action_select_all);
}

void ModelWidget::configurePopupMenu(const vector<BaseObject *> &objects)
{
	unsigned count, i;
	vector<QMenu *> submenus;
	TableObject *tab_obj=nullptr;
	QString str_aux;
	bool protected_obj=false, model_protected=db_model->isProtected();

	new_object_menu.clear();
	quick_actions_menu.clear();
	popup_menu.clear();

	enableModelActions(false);
	selected_objects=objects;
	new_object_menu.setEnabled(!this->db_model->isProtected());

	if(objects.size() <= 1)
	{
		//Case there is no selected object or the selected object is the database model
		if(objects.empty() || (objects.size()==1 && objects[0]==db_model))
			configureDatabaseActions();
		else if(objects.size() == 1)
		{
			tab_obj = dynamic_cast<TableObject *>(objects.front());
			configureBasicActions(objects.front());
		}
	}
	else
		configureQuickMenu(nullptr);

	if(objects.size() > 1)
	{
		bool rem_points = true;

		for(auto &obj : objects)
		{
			rem_points = obj->getObjectType() == ObjectType::Relationship || obj->getObjectType() == ObjectType::BaseRelationship;
			if(!rem_points) break;
		}

		if(rem_points)
		{
			action_remove_rel_points->setData(QVariant());
			popup_menu.addAction(action_remove_rel_points);
		}
	}

	/* Adds the protect/unprotect action when the selected object was not included by relationship
	and if its a table object and the parent table is not protected. */
	if(!objects.empty() &&
			!this->db_model->isProtected() &&
			(!tab_obj || (tab_obj && !tab_obj->getParentTable()->isProtected() && !tab_obj->isAddedByRelationship())))
	{
		/* Special case for systema objects: The actions protect/unprotect will be displayed only for
		 system schemas. The rest of system objects those actions aren't available */
		if(!objects[0]->isSystemObject() ||
				(objects[0]->isSystemObject() && objects[0]->getObjectType()==ObjectType::Schema))
		{
			if(!objects[0]->isProtected())
				popup_menu.addAction(action_protect);
			else
				popup_menu.addAction(action_unprotect);
		}
	}

	//Adding the extended attributes action (only for table/view/database)
	if(objects.size() > 1 ||
		 (objects.empty() && (db_model->getObjectCount(ObjectType::Table) > 0 ||
													db_model->getObjectCount(ObjectType::ForeignTable) > 0 ||
													db_model->getObjectCount(ObjectType::View) > 0)) ||
		 (objects.size() == 1 && (BaseTable::isBaseTable(objects[0]->getObjectType()) ||
															objects[0]->getObjectType() == ObjectType::Database)))
	{
		bool tab_or_view = false;

		for(BaseObject *obj : objects)
		{
			if(!tab_or_view)
			{
				tab_or_view=(PhysicalTable::isPhysicalTable(obj->getObjectType()) || obj->getObjectType()==ObjectType::View);
				break;
			}
		}

		if(tab_or_view ||  objects.empty() || objects.size() == 1)
		{
			popup_menu.addAction(action_collapse_mode);
			popup_menu.addAction(action_pagination);
		}

		if(objects.empty() || (objects.size() == 1 && objects[0]->getObjectType() == ObjectType::Database))
			popup_menu.addAction(action_schemas_rects);
	}

	if((objects.size() == 1 &&
			(BaseTable::isBaseTable(objects[0]->getObjectType()) ||
			 objects[0]->getObjectType() == ObjectType::Textbox)) ||
		 (objects.size() > 1 && !scene->hasOnlyTableChildrenSelection()))
		popup_menu.addAction(action_stacking);

	if(!tab_obj &&
		 (objects.empty() || (objects.size() > 1 && !scene->hasOnlyTableChildrenSelection()) ||
			(objects.size() == 1 && (objects[0]->getObjectType() == ObjectType::Database ||
															 objects[0]->getObjectType() == ObjectType::Tag ||
															 BaseGraphicObject::isGraphicObject(objects[0]->getObjectType())))))
	{
		//Adding fade inout action only for graphical objects or when there is no objects selected or many objects seleted
		popup_menu.addAction(action_fade);
		popup_menu.addSeparator();
		configureFadeMenu();
	}

	//Adding the copy and paste if there is selected objects
	if(!model_protected &&
			!(objects.size()==1 && (objects[0]==db_model || objects[0]->getObjectType()==ObjectType::BaseRelationship)) &&
			!objects.empty())
	{
		popup_menu.addAction(action_copy);

		count=objects.size();
		i=0;
		while(i < count && !protected_obj)
			protected_obj=objects[i++]->isProtected();

		if(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship()))
			popup_menu.addAction(action_cut);

		popup_menu.addAction(action_duplicate);
	}

	//If there is copied object adds the paste action
	if(!model_protected && !copied_objects.empty())
		popup_menu.addAction(action_paste);

	/* Adding the delete object action. This action will be unavailable on following conditions:
	1) The selected object is the database itself
	2) The object is protected
	3) The object is table child object and it was added by relationship
	4) The object is a base relationship (table-view) */
	if((tab_obj && !tab_obj->isAddedByRelationship() && !tab_obj->isProtected()) ||
			(objects.size()==1 && objects[0]->isProtected()) ||
			(!tab_obj && objects.size()==1 && objects[0]!=db_model && objects[0]->getObjectType()!=ObjectType::BaseRelationship) ||
			(objects.size()==1 && objects[0]->getObjectType()==ObjectType::BaseRelationship &&
			 dynamic_cast<BaseRelationship *>(objects[0])->getRelationshipType()==BaseRelationship::RelationshipFk) ||
			objects.size() > 1)
	{
		popup_menu.addAction(action_remove);
		popup_menu.addAction(action_cascade_del);
	}

	//If the table object is a column creates a special menu to acess the constraints that is applied to the column
	configureConstraintsMenu(tab_obj);

	//Enable the popup actions that are visible
	QList<QAction *> actions=popup_menu.actions();
	actions.append(quick_actions_menu.actions());
	while(!actions.isEmpty())
	{
		actions.back()->setEnabled(true);
		actions.pop_back();
	}

	if(objects.size() <= 2 && !scene->hasOnlyTableChildrenSelection())
	{
		popup_menu.addSeparator();
		popup_menu.addAction(action_edit_creation_order);
	}
}

bool ModelWidget::isModified()
{
	return modified;
}

DatabaseModel *ModelWidget::getDatabaseModel()
{
	return db_model;
}

ObjectsScene *ModelWidget::getObjectsScene()
{
	return scene;
}

QGraphicsView *ModelWidget::getViewport()
{
	return viewport;
}

OperationList *ModelWidget::getOperationList()
{
	return op_list;
}

void ModelWidget::setSaveLastCanvasPosition(bool value)
{
	ModelWidget::save_restore_pos=value;
}

void ModelWidget::setRenderSmoothnessDisabled(bool value)
{
	ModelWidget::disable_render_smooth=value;
}

void ModelWidget::setSimplifiedObjectCreation(bool value)
{
	ModelWidget::simple_obj_creation=value;
}

void ModelWidget::setMinimumObjectOpacity(unsigned min_opacity)
{
	if(min_opacity > 70)
		min_opacity = 70;

	ModelWidget::min_object_opacity = static_cast<double>(min_opacity)/100.0;
}

void ModelWidget::highlightObject()
{
	QAction *action=dynamic_cast<QAction *>(sender());

	if(action )
	{
		BaseObject *obj=reinterpret_cast<BaseObject *>(action->data().value<void *>());
		BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(obj);

		if(graph_obj)
		{
			BaseObjectView *obj_view=dynamic_cast<BaseObjectView *>(graph_obj->getOverlyingObject());

			scene->clearSelection();
			obj_view->setSelected(true);
			viewport->centerOn(obj_view);
		}
	}
}

void ModelWidget::toggleNewObjectOverlay()
{
	if(new_obj_overlay_wgt->isHidden() &&
			(selected_objects.empty() || selected_objects[0]->getObjectType()!=ObjectType::BaseRelationship))
	{
		new_obj_overlay_wgt->raise();
		new_obj_overlay_wgt->show();
		new_obj_overlay_wgt->setSelectedObjects(selected_objects);
		this->adjustOverlayPosition();
	}
	else
		new_obj_overlay_wgt->hide();
}

void ModelWidget::adjustOverlayPosition()
{
	int px=0, py=0;

	px=(this->width()/2) - (new_obj_overlay_wgt->width()/2);
	py=(this->height()/2) - (new_obj_overlay_wgt->height()/2);
	new_obj_overlay_wgt->move(px, py);
}

void ModelWidget::toggleObjectSQL()
{
	QAction *action=dynamic_cast<QAction *>(sender());

	if(action)
	{
		BaseObject *object=reinterpret_cast<BaseObject *>(action->data().value<void *>());
		PgModelerUiNs::disableObjectSQL(object, !object->isSQLDisabled());
		setModified(true);
		emit s_objectModified();
	}
}

void ModelWidget::createSequenceFromColumn()
{
	try
	{
		QAction *action=dynamic_cast<QAction *>(sender());
		Column *col=reinterpret_cast<Column *>(action->data().value<void *>());
		Sequence *seq=nullptr;
		Table *tab=dynamic_cast<Table *>(col->getParentTable());

		op_list->startOperationChain();

		//Creates a sequence which name is like the ones auto generated by PostgreSQL
		seq=new Sequence;
		seq->setName(BaseObject::formatName(tab->getName() + QString("_") + col->getName() + QString("_seq")));
		seq->setName(PgModelerNs::generateUniqueName(seq, *db_model->getObjectList(ObjectType::Sequence), false));

		seq->setSchema(tab->getSchema());
		seq->setDefaultValues(col->getType());

		op_list->registerObject(seq, Operation::ObjectCreated);
		db_model->addSequence(seq);

		BaseObject::swapObjectsIds(tab, seq, false);

		op_list->registerObject(col, Operation::ObjectModified, -1, tab);
		//Changes the column type to the alias for serial type
		col->setType(col->getType().getAliasType());
		col->setSequence(seq);

		op_list->finishOperationChain();

		//Revalidate the relationships since the modified column can be a primary key
		if(tab->getPrimaryKey() && tab->getPrimaryKey()->isColumnReferenced(col))
			db_model->validateRelationships();

		tab->setModified(true);
		this->setModified(true);
		emit s_objectCreated();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::convertIntegerToSerial()
{
	try
	{
		QAction *action=dynamic_cast<QAction *>(sender());
		Column *col=reinterpret_cast<Column *>(action->data().value<void *>());
		Table *tab=dynamic_cast<Table *>(col->getParentTable());
		PgSqlType col_type=col->getType();
		QRegExp regexp(QString("^nextval\\(.+\\:\\:regclass\\)"));
		QString serial_tp;

		if(!col_type.isIntegerType() || (!col->getDefaultValue().contains(regexp) && !col->getSequence()))
			throw Exception(Exception::getErrorMessage(ErrorCode::InvConversionIntegerToSerial).arg(col->getName()),
											ErrorCode::InvConversionIntegerToSerial ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		op_list->registerObject(col, Operation::ObjectModified, -1, tab);

		if(col_type==QString("integer") || col_type==QString("int4"))
			serial_tp=QString("serial");
		else if(col_type==QString("smallint") || col_type==QString("int2"))
			serial_tp=QString("smallserial");
		else
			serial_tp=QString("bigserial");

		col->setType(PgSqlType(serial_tp));
		col->setDefaultValue(QString());

		//Revalidate the relationships since the modified column can be a primary key
		if(tab->getPrimaryKey()->isColumnReferenced(col))
			db_model->validateRelationships();

		tab->setModified(true);
		emit s_objectModified();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::breakRelationshipLine()
{
	try
	{
		QAction *action=dynamic_cast<QAction *>(sender());
		BaseRelationship *rel=dynamic_cast<BaseRelationship *>(selected_objects[0]);

		op_list->registerObject(rel, Operation::ObjectModified);
		breakRelationshipLine(rel, action->data().toUInt());
		rel->setModified(true);
		this->setModified(true);

		emit s_objectModified();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::breakRelationshipLine(BaseRelationship *rel, unsigned break_type)
{
	if(!rel) return;

	try
	{
		RelationshipView *rel_view=dynamic_cast<RelationshipView *>(rel->getOverlyingObject());
		double dx, dy;
		QPointF src_pnt, dst_pnt;

		src_pnt=rel_view->getConnectionPoint(BaseRelationship::SrcTable);
		dst_pnt=rel_view->getConnectionPoint(BaseRelationship::DstTable);

		if(break_type==BreakVertNinetyDegrees)
			rel->setPoints({ QPointF(src_pnt.x(), dst_pnt.y()) });
		else if(break_type==BreakHorizNinetyDegrees)
			rel->setPoints({ QPointF(dst_pnt.x(), src_pnt.y()) });
		else if(break_type==BreakHoriz2NinetyDegrees)
		{
			//Calculates the midle vertical point between the tables centers
			dy=(src_pnt.y() + dst_pnt.y())/2;

			//Adds two points on the middle space between tables creating two 90° angles
			rel->setPoints({ QPointF(src_pnt.x(), dy), QPointF(dst_pnt.x(), dy) });
		}
		else
		{
			//Calculates the middle horizontal point between the tables centers
			dx=(src_pnt.x() + dst_pnt.x())/2;

			//Adds two points on the middle space between tables creating two 90° angles
			rel->setPoints({ QPointF(dx, src_pnt.y()), QPointF(dx, dst_pnt.y()) });
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::removeRelationshipPoints()
{
	try
	{
		QAction *action=dynamic_cast<QAction *>(sender());
		BaseRelationship *rel=reinterpret_cast<BaseRelationship *>(action->data().value<void *>());

		//Remove points from all selected relationships
		if(!rel && !selected_objects.empty())
		{
			vector<BaseObject *> rels;

			rels = *db_model->getObjectList(ObjectType::BaseRelationship);
			rels.insert(rels.end(), db_model->getObjectList(ObjectType::Relationship)->begin(),  db_model->getObjectList(ObjectType::Relationship)->end());

			op_list->startOperationChain();
			for(auto &obj : rels)
			{
				rel = dynamic_cast<BaseRelationship *>(obj);

				if(!rel->isProtected())
				{
					op_list->registerObject(rel, Operation::ObjectModified);
					rel->setPoints({});
					rel->setModified(true);
				}
			}
			op_list->finishOperationChain();
		}
		else
		{
			op_list->registerObject(rel, Operation::ObjectModified);
			rel->setPoints({});
			rel->setModified(true);
		}

		scene->clearSelection();
		this->setModified(true);
		emit s_objectModified();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ModelWidget::rearrangeSchemasInGrid(unsigned tabs_per_row, unsigned sch_per_row, QPointF origin, double obj_spacing)
{
	vector<BaseObject *> *objects=nullptr;
	Schema *schema=nullptr;
	SchemaView *sch_view=nullptr;
	unsigned sch_id=0, min_cnt = 0;
	double x=origin.x(), y=origin.y(), max_y=-1, cy=0;

	objects=db_model->getObjectList(ObjectType::Schema);

	/* If schemas per row or tables per row isn't specified
	 * we calculate an minimal number of objects to be arranged per row */
	if(sch_per_row == 0)
	{
		min_cnt = objects->size() * 0.10;
		sch_per_row = min_cnt < 3 ? 3 : min_cnt;
	}

	if(tabs_per_row == 0)
	{
		min_cnt = (db_model->getObjectCount(ObjectType::Table) +
							 db_model->getObjectCount(ObjectType::View) +
							 db_model->getObjectCount(ObjectType::ForeignTable)) * 0.05;
		tabs_per_row = min_cnt < 5 ? 5 : min_cnt;
	}

	for(BaseObject *obj : *objects)
	{
		schema=dynamic_cast<Schema *>(obj);

		/* Forcing the schema rectangle to be visible in order to correctly positioin
			 schemas over the screen */
		schema->setRectVisible(true);

		sch_view=dynamic_cast<SchemaView *>(schema->getOverlyingObject());
		schema->setModified(true);

		//The schema is processed only there are tables inside of it
		if(sch_view && sch_view->getChildrenCount() > 0)
		{
			//Organizing the tables inside the schema
			rearrangeTablesInGrid(schema, tabs_per_row, QPointF(x,y), obj_spacing);
			schema->setModified(true);

			cy=sch_view->pos().y() + sch_view->boundingRect().height();

			//Defining the maximum y position to avoid schema boxes colliding vertically
			if(max_y < cy)
				max_y=cy;

			sch_id++;

			//It the current schema is the last of it`s row
			if(sch_id >= sch_per_row)
			{
				//Incrementing the row position
				sch_id=0;
				y=max_y + obj_spacing;
				x=origin.x();
				max_y=-1;
			}
			else
				//Configuring the x position for the next schema on the current row
				x=sch_view->pos().x() + sch_view->boundingRect().width() + obj_spacing;
		}
	}

	objects=db_model->getObjectList(ObjectType::Relationship);
	for(BaseObject *obj : *objects)
	{
		dynamic_cast<BaseRelationship *>(obj)->setModified(true);
	}

	objects=db_model->getObjectList(ObjectType::BaseRelationship);
	for(BaseObject *obj : *objects)
	{
		dynamic_cast<BaseRelationship *>(obj)->setModified(true);
	}

	//Adjust the whole scene size due to table/schema repositioning
	this->adjustSceneSize();
}

void ModelWidget::rearrangeTablesInGrid(Schema *schema, unsigned tabs_per_row,  QPointF origin, double obj_spacing)
{
	if(schema)
	{
		vector<BaseObject *> tables, views, ftables;
		vector<BaseObject *>::iterator itr;
		BaseTableView *tab_view=nullptr;
		BaseTable *base_tab=nullptr;
		unsigned tab_id=0;
		double max_y=-1, x=origin.x(), y=origin.y(), cy=0;

		//Get the tables and views for the specified schema
		tables=db_model->getObjects(ObjectType::Table, schema);
		ftables=db_model->getObjects(ObjectType::ForeignTable, schema);
		views=db_model->getObjects(ObjectType::View, schema);
		tables.insert(tables.end(), ftables.begin(), ftables.end());
		tables.insert(tables.end(), views.begin(), views.end());

		itr=tables.begin();
		while(itr!=tables.end())
		{
			base_tab=dynamic_cast<BaseTable *>(*itr);
			tab_view=dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
			tab_view->setPos(QPointF(x,y));

			//Defining the maximum y position to avoid table boxes colliding vertically
			cy=tab_view->pos().y()  + tab_view->boundingRect().bottomRight().y();
			if(max_y < cy)
				max_y=cy;

			//It the current table is the last of it's row
			tab_id++;
			if(tab_id >= tabs_per_row)
			{
				//Incrementing the row position
				tab_id=0;
				y=max_y + obj_spacing;
				x=origin.x();
				max_y=-1;
			}
			else
				//Configuring the x position for the next table on the current row
				x=tab_view->pos().x() + tab_view->boundingRect().width() + obj_spacing;

			itr++;
		}
	}
}

void ModelWidget::swapObjectsIds()
{
	BaseForm parent_form(this);
	SwapObjectsIdsWidget *swap_ids_wgt=new SwapObjectsIdsWidget;

	swap_ids_wgt->setModel(this->getDatabaseModel());

	if(!selected_objects.empty())
		swap_ids_wgt->setSelectedObjects(selected_objects[0], selected_objects.size() == 2 ? selected_objects[1] : nullptr);

	connect(swap_ids_wgt, &SwapObjectsIdsWidget::s_objectsIdsSwapped, [&](){
			this->op_list->removeOperations();
			emit s_objectManipulated();
	});

	parent_form.setMainWidget(swap_ids_wgt, SLOT(swapObjectsIds()));
	parent_form.setButtonConfiguration(Messagebox::OkCancelButtons);
	parent_form.apply_ok_btn->setEnabled(false);
	parent_form.apply_ok_btn->setIcon(QPixmap(PgModelerUiNs::getIconPath("swapobjs")));
	parent_form.apply_ok_btn->setText(tr("Swap ids"));
	connect(swap_ids_wgt, SIGNAL(s_objectsIdsSwapReady(bool)), parent_form.apply_ok_btn, SLOT(setEnabled(bool)));

	GeneralConfigWidget::restoreWidgetGeometry(&parent_form, swap_ids_wgt->metaObject()->className());
	parent_form.exec();
	GeneralConfigWidget::saveWidgetGeometry(&parent_form, swap_ids_wgt->metaObject()->className());
}

void ModelWidget::jumpToTable()
{
	QAction *act = qobject_cast<QAction *>(sender());
	BaseTable *tab = nullptr;
	BaseTableView *tab_view = nullptr;

	if(!act)
		return;

	tab = reinterpret_cast<BaseTable *>(act->data().value<void *>());
	scene->clearSelection();
	tab_view = dynamic_cast<BaseTableView *>(tab->getOverlyingObject());
	tab_view->setSelected(true);
	viewport->centerOn(tab_view);
}

void ModelWidget::editTableData()
{
	TableDataWidget *tab_data_wgt=new TableDataWidget;

	tab_data_wgt->setAttributes(db_model, dynamic_cast<PhysicalTable *>(selected_objects.at(0)));
	openEditingForm(tab_data_wgt);
	this->setModified(true);
	emit s_objectManipulated();
}

void ModelWidget::updateModelLayers()
{
	QStringList layers = scene->getLayers();

	layers.removeAt(0);
	db_model->setLayers(layers);
	db_model->setActiveLayers(scene->getActiveLayersIds());
	setModified(true);
}

void ModelWidget::rearrangeTablesHierarchically()
{
	vector<BaseObject *> objects;
	BaseGraphicObject *graph_obj = nullptr;
	BaseTableView *tab_view = nullptr, *root = nullptr;
	int num_rels = 0;

	scene->clearSelection();

	objects.assign(db_model->getObjectList(ObjectType::Table)->begin(), db_model->getObjectList(ObjectType::Table)->end());
	objects.insert(objects.end(), db_model->getObjectList(ObjectType::View)->begin(), db_model->getObjectList(ObjectType::View)->end());

	//We determine the root by searching the table/view which contains the more amount of relationships connected
	for(auto obj : objects)
	{
		graph_obj = dynamic_cast<BaseGraphicObject *>(obj);
		dynamic_cast<Schema *>(graph_obj->getSchema())->setRectVisible(false);

		tab_view = dynamic_cast<BaseTableView *>(graph_obj->getOverlyingObject());

		if(tab_view->getConnectRelsCount() > num_rels)
		{
			root = tab_view;
			num_rels = tab_view->getConnectRelsCount();
		}
	}

	if(root)
	{
		BaseObjectView *obj_view = nullptr;
		BaseRelationship *rel = nullptr;
		QRectF items_rect;
		vector<BaseObject *> evaluated_tabs, not_evaluated, not_linked_tabs;
		double px = 0, py = 0, max_h = 0, max_w = 0;

		//Positioning the root object at the top-left portion of canvas
		root->setPos(QPointF(50, 50));
		evaluated_tabs.push_back(root->getUnderlyingObject());
		items_rect = rearrangeTablesHierarchically(root, evaluated_tabs);
		max_w = items_rect.width();

		objects.clear();
		objects.assign(db_model->getObjectList(ObjectType::Table)->begin(), db_model->getObjectList(ObjectType::Table)->end());
		objects.insert(objects.end(), db_model->getObjectList(ObjectType::View)->begin(), db_model->getObjectList(ObjectType::View)->end());

		//Retrieving the rest of tables/views that were not evaluated in the previous iteration
		std::sort(objects.begin(), objects.end());
		std::sort(evaluated_tabs.begin(), evaluated_tabs.end());
		std::set_difference(objects.begin(), objects.end(), evaluated_tabs.begin(), evaluated_tabs.end(),
												 std::inserter(not_evaluated, not_evaluated.begin()));

		/* While there is not evaluated objects (tables/views that are linked to each other but
		none of them are linked to the root (in)directly) we need to perform the same operation
		done for the root previously */
		while(!not_evaluated.empty())
		{
			num_rels = 0;
			root = nullptr;

			//Determining which table has the greater number of relationships attached
			for(auto &tab : not_evaluated)
			{
				tab_view = dynamic_cast<BaseTableView *>(dynamic_cast<BaseTable *>(tab)->getOverlyingObject());

				if(tab_view->getConnectRelsCount() > num_rels)
				{
					root = tab_view;
					num_rels = tab_view->getConnectRelsCount();
				}
			}

			//Once determined the new root we perform the positioning of its "children"
			if(root && std::find(evaluated_tabs.begin(), evaluated_tabs.end(), root->getUnderlyingObject()) == evaluated_tabs.end())
			{
				root->setPos(QPointF(50, items_rect.bottom() + 50));
				evaluated_tabs.push_back(root->getUnderlyingObject());
				items_rect = rearrangeTablesHierarchically(root, evaluated_tabs);
				not_evaluated.erase(std::find(not_evaluated.begin(), not_evaluated.end(),  root->getUnderlyingObject()));

				if(items_rect.width() > max_w)
					max_w = items_rect.width();
			}
			else
			{
				tab_view = dynamic_cast<BaseTableView *>(dynamic_cast<BaseTable *>(not_evaluated.front())->getOverlyingObject());

				//If the table/view has not relationships connected we separate it in a new list for further rearrangement
				if(tab_view->getConnectRelsCount() == 0)
					not_linked_tabs.push_back(not_evaluated.front());

				not_evaluated.erase(not_evaluated.begin());
			}
		}

		//Repositioning remaining tables (without relationships) and textboxes
		objects.clear();
		objects.assign(not_linked_tabs.begin(), not_linked_tabs.end());
		objects.insert(objects.end(), db_model->getObjectList(ObjectType::Textbox)->begin(), db_model->getObjectList(ObjectType::Textbox)->end());

		px = 50;
		py = items_rect.bottom() + 100;
		max_h = 0;

		for(auto &obj : objects)
		{
			obj_view = dynamic_cast<BaseObjectView *>(dynamic_cast<BaseGraphicObject *>(obj)->getOverlyingObject());
			obj_view->setPos(px, py);
			px += obj_view->boundingRect().width() + 100;

			if(obj_view->boundingRect().height() > max_h)
				max_h = obj_view->boundingRect().height();

			if(px > max_w)
			{
				px = 50;
				py += max_h + 100;
			}
		}

		objects.clear();
		objects.assign(db_model->getObjectList(ObjectType::Relationship)->begin(), db_model->getObjectList(ObjectType::Relationship)->end());
		objects.insert(objects.end(), db_model->getObjectList(ObjectType::BaseRelationship)->begin(), db_model->getObjectList(ObjectType::BaseRelationship)->end());

		for(auto obj : objects)
		{
			rel = dynamic_cast<BaseRelationship *>(obj);
			rel->setPoints({});
			rel->resetLabelsDistance();

			if(!RelationshipView::isCurvedLines() &&
				 round(rel->getTable(BaseRelationship::SrcTable)->getPosition().y()) !=
				 round(rel->getTable(BaseRelationship::DstTable)->getPosition().y()))
				breakRelationshipLine(dynamic_cast<BaseRelationship *>(obj), ModelWidget::BreakVert2NinetyDegrees);
		}

		db_model->setObjectsModified({ ObjectType::Table, ObjectType::View, ObjectType::Schema, ObjectType::Relationship, ObjectType::BaseRelationship });
	}
	else
	{
		//This is a fallback arrangement when the model does not have relationships
		rearrangeSchemasInGrid();
	}

	adjustSceneSize();
	viewport->updateScene({ scene->sceneRect() });
}

QRectF ModelWidget::rearrangeTablesHierarchically(BaseTableView *root, vector<BaseObject *> &evaluated_tabs)
{
	BaseTable *base_tab = dynamic_cast<BaseTable *>(root->getUnderlyingObject()),
			*src_tab = nullptr, *dst_tab = nullptr, *curr_tab = nullptr;
	vector<BaseRelationship *> rels ;
	double px = 0, py = 0, px1 = 0, py1 = 0;
	BaseTableView *tab_view = nullptr;
	vector<BaseTable *> tabs = { base_tab }, next_tabs;
	bool is_protected = false;

	while(!tabs.empty())
	{
		base_tab = tabs.front();
		tabs.erase(tabs.begin());
		tab_view = dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
		rels = db_model->getRelationships(base_tab);

		for(auto &rel : rels)
		{
			if(rel->isSelfRelationship())
				continue;

			src_tab = rel->getTable(BaseRelationship::SrcTable);
			dst_tab = rel->getTable(BaseRelationship::DstTable);

			if(src_tab != base_tab)
				curr_tab = src_tab;
			else if(dst_tab != base_tab)
				curr_tab = dst_tab;
			else
				curr_tab = nullptr;

			if(curr_tab && std::find(evaluated_tabs.begin(), evaluated_tabs.end(), dynamic_cast<BaseObject *>(curr_tab)) == evaluated_tabs.end())
			{
				next_tabs.push_back(curr_tab);
				evaluated_tabs.push_back(dynamic_cast<BaseObject *>(curr_tab));
			}
		}

		if(tabs.empty())
		{
			px = tab_view->pos().x() + (tab_view->boundingRect().width() * 1.50);
			py = root->pos().y() + 75;

			for(auto &next_tab : next_tabs)
			{
				tab_view = dynamic_cast<BaseTableView *>(next_tab->getOverlyingObject());

				//Temporarily unprotecting the table so it can be moved
				if(next_tab->isProtected())
				{
					next_tab->setProtected(false);
					is_protected = true;
				}
				else
					is_protected = false;

				tab_view->setPos(QPointF(px, py));
				next_tab->setProtected(is_protected);

				py += tab_view->boundingRect().height() + 75;
				px += 50;
			}

			if(px > px1) px1 = px;
			if(py > py1) py1 = py;

			tabs.assign(next_tabs.begin(), next_tabs.end());
			next_tabs.clear();
		}
	}

	return QRectF(root->pos(), QPointF(px1, py1));
}

void ModelWidget::rearrangeTablesInSchema(Schema *schema, QPointF start)
{
	vector<BaseObject *> tables, views;

	if(!schema) return;

	tables = db_model->getObjects(ObjectType::Table, schema);
	views = db_model->getObjects(ObjectType::View, schema);
	tables.insert(tables.end(), views.begin(), views.end());

	if(!tables.empty())
	{
		BaseTable *base_tab = nullptr;
		BaseTableView *tab_view = nullptr,  *comp_tab = nullptr, *curr_tab = nullptr;

		//If there two or less tables we put them side-by-side
		if(tables.size() <= 2)
		{
			base_tab = dynamic_cast<BaseTable *>(tables[0]);
			curr_tab = dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
			curr_tab->setPos(start);

			if(tables.size() > 1)
			{
				tab_view = curr_tab;
				base_tab = dynamic_cast<BaseTable *>(tables[1]);
				curr_tab = dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
				curr_tab->setPos(start + QPointF(tab_view->boundingRect().width() * 1.25, 0));
			}
		}
		else
		{
			double max_w = 0, max_h = 0;
			bool has_collision = false;
			QRectF curr_brect, comp_brect, irect;
			QPointF pos;
			random_device rand_seed;
			default_random_engine rand_num_engine;
			unsigned tries = 0;

			rand_num_engine.seed(rand_seed());

			/* Calculating the maximum width and height
			 * The new tables' positions are calculated using these dimensions */
			for(auto &tab : tables)
			{
				base_tab = dynamic_cast<BaseTable *>(tab);
				curr_tab = dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
				max_w += curr_tab->boundingRect().width();
				max_h += curr_tab->boundingRect().height();
			}

			if(tables.size() >= 4)
			{
				max_w *= 0.50;
				max_h *= 0.50;
			}
			else
			{
				max_w *= 1.15;
				max_h *= 1.15;
			}

			uniform_int_distribution<unsigned> dist_x(start.x(), start.x() + max_w),
					dist_y(start.y(), start.y() + max_h);

			//Doing the first random positioning on all tables
			for(auto &tab : tables)
			{
				base_tab = dynamic_cast<BaseTable *>(tab);
				curr_tab = dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
				pos.setX(dist_x(rand_num_engine));
				pos.setY(dist_y(rand_num_engine));
				curr_tab->setPos(pos);
			}

			/* Collision detection: If a table collides with other tables it'll then repositioned
			 * until no interception is detected */
			for(auto &tab : tables)
			{
				base_tab = dynamic_cast<BaseTable *>(tab);
				curr_tab = dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
				curr_brect = QRectF(curr_tab->pos(), curr_tab->boundingRect().size());
				tries = 0;

				do
				{
					has_collision = false;

					for(auto &tab1 : tables)
					{
						if(tab == tab1)
							continue;

						base_tab = dynamic_cast<BaseTable *>(tab1);
						comp_tab = dynamic_cast<BaseTableView *>(base_tab->getOverlyingObject());
						comp_brect = QRectF(comp_tab->pos(), comp_tab->boundingRect().size());
						irect = comp_brect.intersected(curr_brect);

						if(irect.isValid())
						{
							has_collision = true;
							pos.setX(dist_x(rand_num_engine));
							pos.setY(dist_y(rand_num_engine));
							curr_tab->setPos(pos);
							curr_brect = QRectF(curr_tab->pos(), curr_tab->boundingRect().size());
							break;
						}
					}

					tries++;
				}
				while(has_collision && tries < (tables.size() * 100));
			}
		}

		schema->setRectVisible(true);
		schema->setModified(true);
	}
}

void ModelWidget::rearrangeTablesInSchemas()
{
	BaseRelationship *base_rel = nullptr;
	Schema *schema = nullptr;
	SchemaView *sch_view = nullptr, *sch_view_aux = nullptr;
	QRectF curr_brect, comp_brect, irect;
	random_device rand_seed;
	default_random_engine rand_num_engine;
	double max_w = 1000, max_h = 1000;
	vector<BaseObject *> schemas = *db_model->getObjectList(ObjectType::Schema), rels;
	bool has_collision = false;
	uniform_int_distribution<unsigned> dist_x(0, max_w), dist_y(0, max_h);
	unsigned tries = 0,
			max_tries = (db_model->getObjectCount(ObjectType::Table) +
									 db_model->getObjectCount(ObjectType::View) +
									 db_model->getObjectCount(ObjectType::Schema)) * 100;


	rand_num_engine.seed(rand_seed());

	/* Rearraging tables inside schemas and determining the maximum width and height by summing
	 * all schemas widths and heights. These values will be serve as the maximum
	 * position limit for the schemas */
	for(auto &sch : schemas)
	{
		schema = dynamic_cast<Schema *>(sch);
		sch_view = dynamic_cast<SchemaView *>(schema->getOverlyingObject());

		if(!sch_view)	continue;

		rearrangeTablesInSchema(schema, QPointF(dist_x(rand_num_engine), dist_y(rand_num_engine)));

		max_w += sch_view->boundingRect().width();
		max_h += sch_view->boundingRect().height();
	}

	uniform_int_distribution<unsigned>::param_type new_dx(0, max_w * 0.40);
	dist_x.param(new_dx);

	uniform_int_distribution<unsigned>::param_type new_dy(0, max_h * 0.40);
	dist_y.param(new_dy);

	/* Collision detection: If a schema collides with other schemas it'll then repositioned
	 * until no interception is detected or the tries reached the max_tries value */
	for(auto &sch : schemas)
	{
		schema = dynamic_cast<Schema *>(sch);
		sch_view = dynamic_cast<SchemaView *>(schema->getOverlyingObject());
		tries = 0;

		if(!sch_view)	continue;

		curr_brect = QRectF(sch_view->pos(), sch_view->boundingRect().size());

		do
		{
			has_collision = false;

			for(auto &sch1 : schemas)
			{
				schema = dynamic_cast<Schema *>(sch1);
				sch_view_aux = dynamic_cast<SchemaView *>(schema->getOverlyingObject());

				if(sch == sch1 || !sch_view_aux)
					continue;

				comp_brect = QRectF(sch_view_aux->pos(), sch_view_aux->boundingRect().size());
				irect = comp_brect.intersected(curr_brect);

				if(irect.isValid())
				{
					has_collision = true;
					sch_view->moveTo(QPointF(dist_x(rand_num_engine), dist_y(rand_num_engine)));
					curr_brect = QRectF(sch_view->pos(), sch_view->boundingRect().size());
					break;
				}
			}

			tries++;
		}
		while(has_collision && tries < max_tries);
	}

	//Removing all custom points from relationships
	rels.assign(db_model->getObjectList(ObjectType::Relationship)->begin(), db_model->getObjectList(ObjectType::Relationship)->end());
	rels.insert(rels.end(), db_model->getObjectList(ObjectType::BaseRelationship)->begin(), db_model->getObjectList(ObjectType::BaseRelationship)->end());

	for(auto &rel : rels)
	{
		base_rel = dynamic_cast<BaseRelationship *>(rel);
		base_rel->setPoints({});
		base_rel->resetLabelsDistance();
	}

	db_model->setObjectsModified({ ObjectType::Table, ObjectType::View, ObjectType::Schema, ObjectType::Relationship, ObjectType::BaseRelationship });
	adjustSceneSize();
	viewport->updateScene({ scene->sceneRect() });
}

void ModelWidget::updateMagnifierArea()
{
	QPoint pos = viewport->mapFromGlobal(QCursor::pos());
	QPointF scene_pos = viewport->mapToScene(pos);
	QSize size = magnifier_area_lbl->size();
	QPixmap pix = QPixmap(size);
	double cx = magnifier_area_lbl->width() / 2, cy =  magnifier_area_lbl->height() / 2;

	magnifier_frm->setGeometry(0, 0,
														 magnifier_area_lbl->width() * current_zoom,
														 magnifier_area_lbl->height() * current_zoom);
	magnifier_frm->move(pos - QPoint(magnifier_frm->width()/2, magnifier_frm->height()/2));

	if(magnifier_frm->geometry().left() <= magnifier_area_lbl->geometry().right())
		magnifier_area_lbl->move(viewport->width() - magnifier_area_lbl->width(), magnifier_area_lbl->geometry().top());

	if(magnifier_frm->geometry().right() >= magnifier_area_lbl->geometry().left())
		magnifier_area_lbl->move(5, magnifier_area_lbl->geometry().top());

	QPainter p(&pix);
	p.setRenderHints(viewport->renderHints());

	scene->blockSignals(true);
	scene->render(&p, QRectF(QPointF(0,0), size), QRectF(scene_pos - QPointF(cx, cy), size));

	p.setPen(QColor(80,0,0));
	p.drawLine(QPointF(cx, cy - 10), QPointF(cx, cy + 10));
	p.drawLine(QPointF(cx - 10, cy), QPointF(cx + 10, cy));

	magnifier_area_lbl->setPixmap(pix);
	scene->blockSignals(false);
}

void ModelWidget::showMagnifierArea(bool show)
{
	if(show)
	{
		updateMagnifierArea();
		viewport->setCursor(Qt::CrossCursor);
	}
	else
		viewport->setCursor(Qt::ArrowCursor);

	magnifier_area_lbl->setVisible(show);
	magnifier_frm->setVisible(show);
}
