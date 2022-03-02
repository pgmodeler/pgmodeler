/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "appearanceconfigwidget.h"
#include "widgets/modelwidget.h"
#include "utils/syntaxhighlighter.h"

map<QString, attribs_map> AppearanceConfigWidget::config_params;

AppearanceConfigWidget::AppearanceConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
{
	setupUi(this);

	QStringList conf_ids={
	/* 00 */	Attributes::Global,
	/* 01 */	Attributes::Constraints,
	/* 02 */	Attributes::ObjSelection,
	/* 03 */	Attributes::ObjShadow,
	/* 04 */	Attributes::PositionInfo,
	/* 05 */	Attributes::PositionInfo,
	/* 06 */	Attributes::ObjectType,
	/* 07 */	Attributes::LockerArc,
	/* 08 */	Attributes::LockerBody,
	/* 09 */	Attributes::TableSchemaName,
	/* 10 */	Attributes::TableName,
	/* 11 */	Attributes::TableBody,
	/* 12 */	Attributes::TableExtBody,
	/* 13 */	Attributes::TableTitle,
	/* 14 */	Attributes::TableTogglerButtons,
	/* 15 */	Attributes::TableTogglerBody,
	/* 16 */	BaseObject::getSchemaName(ObjectType::Rule),
	/* 17 */	BaseObject::getSchemaName(ObjectType::Rule),
	/* 18 */	BaseObject::getSchemaName(ObjectType::Index),
	/* 19 */	BaseObject::getSchemaName(ObjectType::Index),
	/* 20 */	BaseObject::getSchemaName(ObjectType::Trigger),
	/* 21 */	BaseObject::getSchemaName(ObjectType::Trigger),
	/* 22 */	BaseObject::getSchemaName(ObjectType::Constraint),
	/* 23 */	BaseObject::getSchemaName(ObjectType::Constraint),
	/* 24 */	BaseObject::getSchemaName(ObjectType::Policy),
	/* 25 */	BaseObject::getSchemaName(ObjectType::Policy),
	/* 26 */	Attributes::ViewSchemaName,
	/* 27 */	Attributes::ViewName,
	/* 28 */	Attributes::ViewBody,
	/* 29 */	Attributes::ViewExtBody,
	/* 30 */	Attributes::ViewTitle,
	/* 31 */	Attributes::Alias,
	/* 32 */	Attributes::RefColumn,
	/* 33 */	Attributes::RefTable,
	/* 34 */	Attributes::Reference,
	/* 35 */	Attributes::ViewTogglerButtons,
	/* 36 */	Attributes::ViewTogglerBody,
	/* 37 */	BaseObject::getSchemaName(ObjectType::Textbox),
	/* 38 */	Attributes::Column,
	/* 39 */	Attributes::Column,
	/* 40 */	Attributes::InhColumn,
	/* 41 */	Attributes::ProtColumn,
	/* 42 */	Attributes::PkColumn,
	/* 43 */	Attributes::PkColumn,
	/* 44 */	Attributes::FkColumn,
	/* 45 */	Attributes::FkColumn,
	/* 46 */	Attributes::UqColumn,
	/* 47 */	Attributes::UqColumn,
	/* 48 */	Attributes::NnColumn,
	/* 49 */	Attributes::NnColumn,
	/* 50 */	Attributes::Relationship,
	/* 51 */	Attributes::Label,
	/* 52 */	Attributes::Label,
	/* 53 */	Attributes::Attribute,
	/* 54 */	Attributes::Attribute,
	/* 55 */	Attributes::Tag,
	/* 56 */	Attributes::Tag,
	/* 57 */	Attributes::Placeholder,
	/* 58 */	Attributes::ForeignTableSchemaName,
	/* 59 */	Attributes::ForeignTableName,
	/* 60 */	Attributes::ForeignTableBody,
	/* 61 */	Attributes::ForeignTableExtBody,
	/* 62 */	Attributes::ForeignTableTitle,
	/* 63 */	Attributes::ForeignTableTogglerButtons,
	/* 64 */	Attributes::ForeignTableTogglerBody };

	int i, count=element_cmb->count(),
			//This auxiliary vector stores the id of elements that represents color/font confing of graphical objects
			obj_conf_ids_vect[]={ 2, 3, 5, 7, 8, 11, 12, 13, 14, 15,
														17, 19, 21, 23, 25, 28, 29, 30,
														34, 35, 36, 37, 39, 43, 45, 47, 49,
														50, 52, 54, 56, 57, 60, 61, 62, 63, 64 };
	vector<int> conf_obj_ids(obj_conf_ids_vect, obj_conf_ids_vect + sizeof(obj_conf_ids_vect) / sizeof(int));

	conf_items.resize(count);
	for(i=0; i < count; i++)
	{
		conf_items[i].conf_id=conf_ids[i];
		conf_items[i].obj_conf=(std::find(conf_obj_ids.begin(), conf_obj_ids.end(), i) != conf_obj_ids.end());
	}

	elem_color_cp=new ColorPickerWidget(3, this);

	model=new DatabaseModel;
	scene=new ObjectsScene;
	scene->setSceneRect(QRectF(0,0,500,500));
	placeholder=new RoundedRectItem;

	viewp=new QGraphicsView(scene);
	viewp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	viewp->setRenderHint(QPainter::Antialiasing);
	viewp->setRenderHint(QPainter::TextAntialiasing);
	viewp->setRenderHint(QPainter::SmoothPixmapTransform);
	viewp->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	viewp->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	viewp->centerOn(0,0);

	QHBoxLayout *hbox = new QHBoxLayout(grid_color_wgt);
	hbox->setContentsMargins(0,0,0,0);
	grid_color_cp = new ColorPickerWidget(1, grid_color_wgt);
	grid_color_cp->setButtonToolTip(0, tr("Define a custom color for the grid lines"));
	hbox->addWidget(grid_color_cp);

	hbox = new QHBoxLayout(canvas_color_wgt);
	hbox->setContentsMargins(0,0,0,0);
	canvas_color_cp = new ColorPickerWidget(1, canvas_color_wgt);
	canvas_color_cp->setButtonToolTip(0, tr("Define a custom color for the canvas area"));
	hbox->addWidget(canvas_color_cp);

	hbox = new QHBoxLayout(delimiters_color_wgt);
	hbox->setContentsMargins(0,0,0,0);
	delimiters_color_cp = new ColorPickerWidget(1, delimiters_color_wgt);
	delimiters_color_cp->setButtonToolTip(0, tr("Define a custom color for the page delimiter lines"));
	hbox->addWidget(delimiters_color_cp);

	QGridLayout *grid=dynamic_cast<QGridLayout *>(appearance_frm->layout());
	grid->addWidget(elem_color_cp, 3, 1, 1, 4);
	grid->addWidget(viewp, 4 , 0, 1, 5);

	line_numbers_cp=new ColorPickerWidget(1, this);
	line_numbers_cp->setButtonToolTip(0, tr("Line numbers' font color"));

	line_numbers_bg_cp=new ColorPickerWidget(1, this);
	line_numbers_bg_cp->setButtonToolTip(0, tr("Line numbers' background color"));

	line_highlight_cp=new ColorPickerWidget(1, this);
	line_highlight_cp->setButtonToolTip(0, tr("Highlighted line color"));

	font_preview_txt=new NumberedTextEditor(this);
	font_preview_txt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	font_preview_txt->setPlainText(tr("The little brown fox jumps over the lazy dog") + QString("\n\ttext with tab «") + QString("\n0123456789\n.()[]{};"));

	QBoxLayout *layout=new QBoxLayout(QBoxLayout::LeftToRight);
	grid=dynamic_cast<QGridLayout *>(code_font_gb->layout());
	layout->addWidget(line_numbers_cp);
	layout->addWidget(line_numbers_bg_cp);
	layout->addWidget(line_highlight_cp);
	layout->addItem(new QSpacerItem(1000,20, QSizePolicy::Expanding));
	grid->addLayout(layout, 2, 1);
	grid->addWidget(font_preview_txt,grid->count(),0,1,5);

	connect(element_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableConfigElement()));
	connect(elem_font_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(applyElementFontStyle()));
	connect(elem_font_size_spb, SIGNAL(valueChanged(double)), this, SLOT(applyElementFontStyle()));
	connect(bold_chk, SIGNAL(toggled(bool)), this, SLOT(applyElementFontStyle()));
	connect(underline_chk, SIGNAL(toggled(bool)), this, SLOT(applyElementFontStyle()));
	connect(italic_chk, SIGNAL(toggled(bool)), this, SLOT(applyElementFontStyle()));

	connect(code_font_size_spb, SIGNAL(valueChanged(double)), this, SLOT(updateCodeFontPreview()));
	connect(code_font_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(updateCodeFontPreview()));
	connect(line_numbers_cp, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(updateCodeFontPreview()));
	connect(line_numbers_cp, SIGNAL(s_colorsChanged()), this, SLOT(updateCodeFontPreview()));
	connect(line_numbers_bg_cp, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(updateCodeFontPreview()));
	connect(line_numbers_bg_cp, SIGNAL(s_colorsChanged()), this, SLOT(updateCodeFontPreview()));
	connect(line_highlight_cp, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(updateCodeFontPreview()));
	connect(line_highlight_cp, SIGNAL(s_colorsChanged()), this, SLOT(updateCodeFontPreview()));
	connect(disp_line_numbers_chk, SIGNAL(toggled(bool)), this, SLOT(updateCodeFontPreview()));
	connect(hightlight_lines_chk, SIGNAL(toggled(bool)), this, SLOT(updateCodeFontPreview()));
	connect(tab_width_spb, SIGNAL(valueChanged(int)), this, SLOT(updateCodeFontPreview()));
	connect(tab_width_chk, SIGNAL(toggled(bool)), tab_width_spb, SLOT(setEnabled(bool)));
	connect(tab_width_chk, SIGNAL(toggled(bool)), this, SLOT(updateCodeFontPreview()));
	connect(font_preview_txt, SIGNAL(cursorPositionChanged()), this, SLOT(updateCodeFontPreview()));

	connect(elem_color_cp, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(applyElementColor(unsigned, QColor)));
	connect(elem_color_cp, &ColorPickerWidget::s_colorsChanged,
			[&](){
		for(unsigned i=0; i < elem_color_cp->getColorCount(); i++)
			applyElementColor(i, elem_color_cp->getColor(i));
	});
}

AppearanceConfigWidget::~AppearanceConfigWidget()
{
	scene->removeItem(placeholder);

	delete placeholder;
	delete viewp;
	delete scene;
	delete model;
}

map<QString, attribs_map> AppearanceConfigWidget::getConfigurationParams()
{
	return config_params;
}

void AppearanceConfigWidget::loadExampleModel()
{
	try
	{
		RelationshipView *rel=nullptr;
		StyledTextboxView *txtbox=nullptr;
		TableView *tab=nullptr;
		GraphicalView *view=nullptr;
		unsigned count, i;
		QList<BaseObjectView *> graph_objs;

		if(model->getObjectCount()==0)
		{
			model->loadModel(GlobalAttributes::getTmplConfigurationFilePath("", GlobalAttributes::ExampleModel));

			count=model->getObjectCount(ObjectType::Table);
			for(i=0; i < count; i++)
			{
				tab=new TableView(model->getTable(i));
				tab->setSelected(i==1);
				scene->addItem(tab);
				graph_objs.append(tab);
			}

			count=model->getObjectCount(ObjectType::ForeignTable);
			for(i=0; i < count; i++)
			{
				tab=new TableView(model->getForeignTable(i));
				scene->addItem(tab);
				graph_objs.append(tab);
			}

			count=model->getObjectCount(ObjectType::View);
			for(i=0; i < count; i++)
			{
				view=new GraphicalView(model->getView(i));
				scene->addItem(view);
				graph_objs.append(view);
			}

			count=model->getObjectCount(ObjectType::Relationship);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, ObjectType::Relationship));
				scene->addItem(rel);
				graph_objs.append(rel);
			}

			count=model->getObjectCount(ObjectType::BaseRelationship);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, ObjectType::BaseRelationship));
				scene->addItem(rel);
				graph_objs.append(rel);
			}

			count=model->getObjectCount(ObjectType::Textbox);
			for(i=0; i < count; i++)
			{
				txtbox=new StyledTextboxView(model->getTextbox(i));
				txtbox->setSelected(i==0);
				scene->addItem(txtbox);
				graph_objs.append(txtbox);
			}

			for(auto &obj : graph_objs)
			{
				obj->setFlag(QGraphicsItem::ItemIsSelectable, false);
				obj->setFlag(QGraphicsItem::ItemIsMovable, false);
			}

			placeholder->setRect(QRectF(400, 280, 200, 150));
			updatePlaceholderItem();
			scene->addItem(placeholder);
			scene->setActiveLayers(QList<unsigned>({0}));
			scene->setSceneRect(scene->itemsBoundingRect(false));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void AppearanceConfigWidget::updatePlaceholderItem()
{
	placeholder->setBrush(BaseObjectView::getFillStyle(Attributes::Placeholder));
	QPen pen=BaseObjectView::getBorderStyle(Attributes::Placeholder);
	pen.setStyle(Qt::DashLine);
	placeholder->setPen(pen);
}

void AppearanceConfigWidget::loadConfiguration()
{
	try
	{
		BaseConfigWidget::loadConfiguration(GlobalAttributes::AppearanceConf, config_params, { Attributes::Id }, true);

		applyConfiguration();
		loadExampleModel();
		model->setObjectsModified();
		updatePlaceholderItem();
		scene->update();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo());
	}
}

void AppearanceConfigWidget::applyDesignCodeStyle()
{
	grid_size_spb->setValue((config_params[Attributes::Design][Attributes::GridSize]).toUInt());
	min_obj_opacity_spb->setValue(config_params[Attributes::Design][Attributes::MinObjectOpacity].toUInt());
	attribs_per_page_spb->setValue(config_params[Attributes::Design][Attributes::AttribsPerPage].toUInt());
	ext_attribs_per_page_spb->setValue(config_params[Attributes::Design][Attributes::ExtAttribsPerPage].toUInt());

	/* If we can't identify at least one of the colors that compose the grid then we use default colors
	 * avoiding black canvas or black grid color */
	if(config_params[Attributes::Design].count(Attributes::GridColor) == 0 ||
		 config_params[Attributes::Design].count(Attributes::CanvasColor) == 0 ||
		 config_params[Attributes::Design].count(Attributes::DelimitersColor) == 0)
	{
		grid_color_cp->setColor(0, ObjectsScene::DefaultGridColor);
		canvas_color_cp->setColor(0, ObjectsScene::DefaultCanvasColor);
		delimiters_color_cp->setColor(0, ObjectsScene::DefaultDelimitersColor);
	}
	else
	{
		grid_color_cp->setColor(0, QColor(config_params[Attributes::Design][Attributes::GridColor]));
		canvas_color_cp->setColor(0, QColor(config_params[Attributes::Design][Attributes::CanvasColor]));
		delimiters_color_cp->setColor(0, QColor(config_params[Attributes::Design][Attributes::DelimitersColor]));
	}

	code_font_cmb->setCurrentFont(QFont(config_params[Attributes::Code][Attributes::Font]));
	code_font_size_spb->setValue(config_params[Attributes::Code][Attributes::FontSize].toDouble());
	disp_line_numbers_chk->setChecked(config_params[Attributes::Code][Attributes::DisplayLineNumbers]==Attributes::True);
	hightlight_lines_chk->setChecked(config_params[Attributes::Code][Attributes::HighlightLines]==Attributes::True);
	line_numbers_cp->setColor(0, config_params[Attributes::Code][Attributes::LineNumbersColor]);
	line_numbers_bg_cp->setColor(0, config_params[Attributes::Code][Attributes::LineNumbersBgColor]);
	line_highlight_cp->setColor(0, config_params[Attributes::Code][Attributes::LineHighlightColor]);

	int tab_width=(config_params[Attributes::Code][Attributes::TabWidth]).toInt();
	tab_width_chk->setChecked(tab_width > 0);
	tab_width_spb->setEnabled(tab_width_chk->isChecked());
	tab_width_spb->setValue(tab_width);
}

void AppearanceConfigWidget::applyObjectsStyle()
{
	QTextCharFormat font_fmt;
	QFont font;
	attribs_map attribs;
	QStringList list, colors;
	QString elem;

	for(auto &itr : config_params)
	{
		elem = itr.first;
		attribs = itr.second;

		if(elem==Attributes::Global)
		{
			font.setFamily(attribs[Attributes::Font]);
			font.setPointSizeF(attribs[Attributes::Size].toDouble());
			font.setBold(attribs[Attributes::Bold]==Attributes::True);
			font.setItalic(attribs[Attributes::Italic]==Attributes::True);
			font.setUnderline(attribs[Attributes::Underline]==Attributes::True);
			font_fmt.setFont(font);
			BaseObjectView::setFontStyle(elem, font_fmt);
		}
		else if(elem.startsWith(Attributes::Font + "-"))
		{
			elem.remove(Attributes::Font + "-");
			font = font_fmt.font();
			font.setBold(attribs[Attributes::Bold]==Attributes::True);
			font.setItalic(attribs[Attributes::Italic]==Attributes::True);
			font.setUnderline(attribs[Attributes::Underline]==Attributes::True);
			font_fmt.setFont(font);
			font_fmt.setForeground(QColor(attribs[Attributes::Color]));
			BaseObjectView::setFontStyle(elem, font_fmt);
		}
		else if(elem.startsWith(Attributes::Object + "-"))
		{
			elem.remove(Attributes::Object + "-");
			list = attribs[Attributes::FillColor].split(',');

			colors.clear();
			colors.append(!list.isEmpty() ? list.at(0) : "#000");
			colors.append(list.size()==2 ? list.at(1) : colors.at(0));
			BaseObjectView::setElementColor(elem, QColor(colors.at(0)), 0);
			BaseObjectView::setElementColor(elem, QColor(colors.at(1)), 1);
			BaseObjectView::setElementColor(elem, QColor(attribs[Attributes::BorderColor]), 2);
		}
	}

	for(auto &cnf_item : conf_items)
	{
		if(cnf_item.obj_conf)
		{
			BaseObjectView::getFillStyle(cnf_item.conf_id, cnf_item.colors[0], cnf_item.colors[1]);
			cnf_item.colors[2]=BaseObjectView::getBorderStyle(cnf_item.conf_id).color();
		}
		else
			cnf_item.font_fmt=BaseObjectView::getFontStyle(cnf_item.conf_id);
	}

	enableConfigElement();
	elem_font_cmb->setCurrentFont(BaseObjectView::getFontStyle(Attributes::Global).font());
}

void AppearanceConfigWidget::saveConfiguration()
{
	try
	{
		attribs_map attribs;
		AppearanceConfigItem item;
		QString attrib_id;
		QFont font;

		attribs[Attributes::GridSize]=QString::number(grid_size_spb->value());
		attribs[Attributes::MinObjectOpacity]=QString::number(min_obj_opacity_spb->value());
		attribs[Attributes::AttribsPerPage]=QString::number(attribs_per_page_spb->value());
		attribs[Attributes::ExtAttribsPerPage]=QString::number(ext_attribs_per_page_spb->value());
		attribs[Attributes::GridColor] = grid_color_cp->getColor(0).name();
		attribs[Attributes::CanvasColor] = canvas_color_cp->getColor(0).name();
		attribs[Attributes::DelimitersColor] = delimiters_color_cp->getColor(0).name();
		config_params[Attributes::Design] = attribs;
		attribs.clear();

		attribs[Attributes::Font]=code_font_cmb->currentText();
		attribs[Attributes::FontSize]=QString::number(code_font_size_spb->value());
		attribs[Attributes::DisplayLineNumbers]=(disp_line_numbers_chk->isChecked() ? Attributes::True : "");
		attribs[Attributes::HighlightLines]=(hightlight_lines_chk->isChecked() ? Attributes::True : "");
		attribs[Attributes::LineNumbersColor]=line_numbers_cp->getColor(0).name();
		attribs[Attributes::LineNumbersBgColor]=line_numbers_bg_cp->getColor(0).name();
		attribs[Attributes::LineHighlightColor]=line_highlight_cp->getColor(0).name();
		attribs[Attributes::TabWidth]=QString::number(tab_width_chk->isChecked() ? tab_width_spb->value() : 0);
		config_params[Attributes::Code] = attribs;
		attribs.clear();

		for(auto &item : conf_items)
		{
			//If the item is a object color config
			if(item.obj_conf)
			{
				//Creates an attribute that stores the fill color
				attrib_id=item.conf_id + "-color";
				if(item.colors[0]==item.colors[1])
					attribs[attrib_id]=item.colors[0].name();
				else
					attribs[attrib_id]=item.colors[0].name() + "," + item.colors[1].name();

				//Creates an attribute that stores the border color
				attrib_id=item.conf_id + "-bcolor";
				attribs[attrib_id]=item.colors[2].name();
			}
			//If the item is a font config
			else if(item.conf_id!=Attributes::Global && !item.obj_conf)
			{
				font=item.font_fmt.font();

				//Creates an attribute to store the font color
				attrib_id=item.conf_id + "-fcolor";
				attribs[attrib_id]=item.font_fmt.foreground().color().name();

				attrib_id=item.conf_id + "-" + Attributes::Italic;
				attribs[attrib_id]=(font.italic() ? Attributes::True : Attributes::False);

				attrib_id=item.conf_id + "-" + Attributes::Bold;
				attribs[attrib_id]=(font.bold() ? Attributes::True : Attributes::False);

				attrib_id=item.conf_id + "-" + Attributes::Underline;
				attribs[attrib_id]=(font.underline() ? Attributes::True : Attributes::False);
			}
			//Special case: treating the global font element
			else
			{
				attribs[Attributes::Global + "-font"]=QFontInfo(item.font_fmt.font()).family();
				attribs[Attributes::Global + "-font-size"]=QString("%1").arg(item.font_fmt.font().pointSizeF());
			}
		}

		config_params[GlobalAttributes::AppearanceConf] = attribs;
		BaseConfigWidget::saveConfiguration(GlobalAttributes::AppearanceConf, config_params);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void AppearanceConfigWidget::enableConfigElement()
{
	//QPalette pal;
	int idx=element_cmb->currentIndex();

	//Widgets enabled only when the global font element is selected (idx==0)
	elem_font_cmb->setEnabled(idx==0);
	font_lbl->setEnabled(idx==0);
	elem_font_size_spb->setEnabled(idx==0);
	unity_lbl->setEnabled(idx==0);

	//Widgets enabled when a font configuration element is selected
	underline_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);
	bold_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);
	italic_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);

	colors_lbl->setVisible(idx!=0);
	elem_color_cp->setVisible(colors_lbl->isVisible());

	//Buttons visible when a object configuration element is selected
	elem_color_cp->setButtonVisible(1, conf_items[idx].obj_conf);
	/* The border color picker is hidden only for Attributes:ObjShadow since
	 * this element has no border drawn */
	elem_color_cp->setButtonVisible(2,
																 conf_items[idx].obj_conf &&
																 conf_items[idx].conf_id != Attributes::ObjShadow);

	underline_chk->blockSignals(true);
	italic_chk->blockSignals(true);
	bold_chk->blockSignals(true);
	elem_font_cmb->blockSignals(true);
	elem_font_size_spb->blockSignals(true);

	if(!conf_items[idx].obj_conf)
	{
		QTextCharFormat fmt=BaseObjectView::getFontStyle(conf_items[idx].conf_id);
		elem_color_cp->setColor(0, fmt.foreground().color());
		underline_chk->setChecked(fmt.font().underline());
		italic_chk->setChecked(fmt.font().italic());
		bold_chk->setChecked(fmt.font().bold());
		elem_font_cmb->setCurrentFont(fmt.font());
		elem_font_size_spb->setValue(fmt.font().pointSizeF());
	}
	else
	{
		QColor color1, color2;
		BaseObjectView::getFillStyle(conf_items[idx].conf_id, color1, color2);

		elem_color_cp->setColor(0, color1);
		elem_color_cp->setColor(1, color2);
		elem_color_cp->setColor(2, BaseObjectView::getBorderStyle(conf_items[idx].conf_id).color());
		underline_chk->setChecked(false);
		italic_chk->setChecked(false);
		bold_chk->setChecked(false);
	}

	underline_chk->blockSignals(false);
	italic_chk->blockSignals(false);
	bold_chk->blockSignals(false);
	elem_font_cmb->blockSignals(false);
	elem_font_size_spb->blockSignals(false);
}

void AppearanceConfigWidget::applyElementColor(unsigned color_idx, QColor color)
{
	if(conf_items[element_cmb->currentIndex()].obj_conf)
	{
		conf_items[element_cmb->currentIndex()].colors[color_idx]=color;
		BaseObjectView::setElementColor(conf_items[element_cmb->currentIndex()].conf_id, color, color_idx);
		updatePlaceholderItem();
	}
	else if(color_idx == 0)
	{
		conf_items[element_cmb->currentIndex()].font_fmt.setForeground(color);
		BaseObjectView::setFontStyle(conf_items[element_cmb->currentIndex()].conf_id,
				conf_items[element_cmb->currentIndex()].font_fmt);
	}

	model->setObjectsModified();
	scene->update();
	setConfigurationChanged(true);
}

void AppearanceConfigWidget::applyConfiguration()
{
	applyDesignCodeStyle();
	applyObjectsStyle();

	ObjectsScene::setCanvasColor(canvas_color_cp->getColor(0));
	ObjectsScene::setGridColor(grid_color_cp->getColor(0));
	ObjectsScene::setDelimitersColor(delimiters_color_cp->getColor(0));
	ObjectsScene::setGridSize(grid_size_spb->value());
	BaseTableView::setAttributesPerPage(BaseTable::AttribsSection, attribs_per_page_spb->value());
	BaseTableView::setAttributesPerPage(BaseTable::ExtAttribsSection, ext_attribs_per_page_spb->value());
	ModelWidget::setMinimumObjectOpacity(min_obj_opacity_spb->value());

	double fnt_size = config_params[Attributes::Code][Attributes::FontSize].toDouble();
	QFont fnt;

	if(fnt_size < 5.0)
		fnt_size = 5.0;

	fnt.setFamily(config_params[Attributes::Code][Attributes::Font]);
	fnt.setPointSizeF(fnt_size);
	NumberedTextEditor::setLineNumbersVisible(disp_line_numbers_chk->isChecked());
	NumberedTextEditor::setLineHighlightColor(line_highlight_cp->getColor(0));
	NumberedTextEditor::setHighlightLines(hightlight_lines_chk->isChecked());
	NumberedTextEditor::setDefaultFont(fnt);
	LineNumbersWidget::setColors(line_numbers_cp->getColor(0), line_numbers_bg_cp->getColor(0));
	SyntaxHighlighter::setDefaultFont(fnt);
}

void AppearanceConfigWidget::applyElementFontStyle()
{
	QFont font;

	font=elem_font_cmb->currentFont();
	font.setBold(bold_chk->isChecked());
	font.setItalic(italic_chk->isChecked());
	font.setUnderline(underline_chk->isChecked());
	font.setPointSizeF(elem_font_size_spb->value());

	conf_items[element_cmb->currentIndex()].font_fmt.setFont(font);
	BaseObjectView::setFontStyle(conf_items[element_cmb->currentIndex()].conf_id,
			conf_items[element_cmb->currentIndex()].font_fmt);

	model->setObjectsModified();
	scene->update();
	setConfigurationChanged(true);
}

void AppearanceConfigWidget::restoreDefaults()
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::AppearanceConf, false);
		loadConfiguration();
		setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void AppearanceConfigWidget::updateCodeFontPreview()
{
	QFont fnt;

	fnt=code_font_cmb->currentFont();
	fnt.setPointSizeF(code_font_size_spb->value());

	NumberedTextEditor::setDefaultFont(fnt);
	NumberedTextEditor::setLineNumbersVisible(disp_line_numbers_chk->isChecked());
	NumberedTextEditor::setLineHighlightColor(line_highlight_cp->getColor(0));
	NumberedTextEditor::setHighlightLines(hightlight_lines_chk->isChecked());
	NumberedTextEditor::setTabDistance(tab_width_chk->isChecked() ? tab_width_spb->value() : 0);
	LineNumbersWidget::setColors(line_numbers_cp->getColor(0), line_numbers_bg_cp->getColor(0));

	font_preview_txt->setReadOnly(false);
	font_preview_txt->updateLineNumbersSize();
	font_preview_txt->updateLineNumbers();
	font_preview_txt->highlightCurrentLine();
	font_preview_txt->setReadOnly(true);

	setConfigurationChanged(true);
}
