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

#include "appearanceconfigwidget.h"

map<QString, attribs_map> AppearanceConfigWidget::config_params;

AppearanceConfigWidget::AppearanceConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
{
	setupUi(this);

	QStringList conf_ids={
	/* 00 */	Attributes::Global,
	/* 01 */	Attributes::Constraints,
	/* 02 */	Attributes::ObjSelection,
	/* 03 */	Attributes::PositionInfo,
	/* 04 */	Attributes::PositionInfo,
	/* 05 */	Attributes::ObjectType,
	/* 06 */	Attributes::LockerArc,
	/* 07 */	Attributes::LockerBody,
	/* 08 */	Attributes::TableSchemaName,
	/* 09 */	Attributes::TableName,
	/* 10 */	Attributes::TableBody,
	/* 11 */	Attributes::TableExtBody,
	/* 12 */	Attributes::TableTitle,
	/* 13 */	Attributes::TableTogglerButtons,
	/* 14 */	Attributes::TableTogglerBody,
	/* 15 */	BaseObject::getSchemaName(ObjectType::Rule),
	/* 16 */	BaseObject::getSchemaName(ObjectType::Rule),
	/* 17 */	BaseObject::getSchemaName(ObjectType::Index),
	/* 18 */	BaseObject::getSchemaName(ObjectType::Index),
	/* 19 */	BaseObject::getSchemaName(ObjectType::Trigger),
	/* 20 */	BaseObject::getSchemaName(ObjectType::Trigger),
	/* 21 */	BaseObject::getSchemaName(ObjectType::Constraint),
	/* 22 */	BaseObject::getSchemaName(ObjectType::Constraint),
	/* 23 */	BaseObject::getSchemaName(ObjectType::Policy),
	/* 24 */	BaseObject::getSchemaName(ObjectType::Policy),
	/* 25 */	Attributes::ViewSchemaName,
	/* 26 */	Attributes::ViewName,
	/* 27 */	Attributes::ViewBody,
	/* 28 */	Attributes::ViewExtBody,
	/* 29 */	Attributes::ViewTitle,
	/* 30 */	Attributes::Alias,
	/* 31 */	Attributes::RefColumn,
	/* 32 */	Attributes::RefTable,
	/* 33 */	Attributes::Reference,
	/* 34 */	Attributes::ViewTogglerButtons,
	/* 35 */	Attributes::ViewTogglerBody,
	/* 36 */	BaseObject::getSchemaName(ObjectType::Textbox),
	/* 37 */	Attributes::Column,
	/* 38 */	Attributes::Column,
	/* 39 */	Attributes::InhColumn,
	/* 40 */	Attributes::ProtColumn,
	/* 41 */	Attributes::PkColumn,
	/* 42 */	Attributes::PkColumn,
	/* 43 */	Attributes::FkColumn,
	/* 44 */	Attributes::FkColumn,
	/* 45 */	Attributes::UqColumn,
	/* 46 */	Attributes::UqColumn,
	/* 47 */	Attributes::NnColumn,
	/* 48 */	Attributes::NnColumn,
	/* 49 */	Attributes::Relationship,
	/* 50 */	Attributes::Label,
	/* 51 */	Attributes::Label,
	/* 52 */	Attributes::Attribute,
	/* 53 */	Attributes::Attribute,
	/* 54 */	Attributes::Tag,
	/* 55 */	Attributes::Tag,
	/* 56 */	Attributes::Placeholder,
	/* 57 */	Attributes::ForeignTableSchemaName,
	/* 58 */	Attributes::ForeignTableName,
	/* 59 */	Attributes::ForeignTableBody,
	/* 60 */	Attributes::ForeignTableExtBody,
	/* 61 */	Attributes::ForeignTableTitle,
	/* 62 */	Attributes::ForeignTableTogglerButtons,
	/* 63 */	Attributes::ForeignTableTogglerBody };

	int i, count=element_cmb->count(),
			//This auxiliary vector stores the id of elements that represents color/font confing of graphical objects
			obj_conf_ids_vect[]={ 2, 4, 6, 7, 10, 11, 12, 13, 14,
														16, 18, 20, 22, 24, 27, 28, 29,
														33, 34, 35, 36, 38, 42, 44, 46, 48,
														49, 51, 53, 55, 56, 59, 60, 61, 62, 63 };
	vector<int> conf_obj_ids(obj_conf_ids_vect, obj_conf_ids_vect + sizeof(obj_conf_ids_vect) / sizeof(int));

	conf_items.resize(count);
	for(i=0; i < count; i++)
	{
		conf_items[i].conf_id=conf_ids[i];
		conf_items[i].obj_conf=(std::find(conf_obj_ids.begin(), conf_obj_ids.end(), i) != conf_obj_ids.end());
	}

	color_picker=new ColorPickerWidget(3, this);

	model=new DatabaseModel;
	scene=new ObjectsScene;
	scene->setSceneRect(QRectF(0,0,500,500));
	placeholder=new RoundedRectItem;

	viewp=new QGraphicsView(scene);
	viewp->setEnabled(false);
	viewp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	viewp->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	viewp->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	viewp->setRenderHint(QPainter::Antialiasing);
	viewp->setRenderHint(QPainter::TextAntialiasing);
	viewp->setRenderHint(QPainter::SmoothPixmapTransform);
	viewp->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	viewp->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	viewp->centerOn(0,0);

	QGridLayout *grid=dynamic_cast<QGridLayout *>(appearance_frm->layout());
	grid->addWidget(color_picker, 3, 1, 1, 4);
	grid->addWidget(viewp, 4 , 0, 1, 5);

	connect(element_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableConfigElement()));
	connect(font_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(applyFontStyle()));
	connect(font_size_spb, SIGNAL(valueChanged(double)), this, SLOT(applyFontStyle()));
	connect(bold_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle()));
	connect(underline_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle()));
	connect(italic_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle()));

	connect(color_picker, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(applyElementColor(unsigned, QColor)));

	connect(color_picker, &ColorPickerWidget::s_colorsChanged,
			[&](){
		for(unsigned i=0; i < color_picker->getColorCount(); i++)
			applyElementColor(i, color_picker->getColor(i));
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

		if(model->getObjectCount()==0)
		{
			model->loadModel(GlobalAttributes::getTmplConfigurationFilePath("", GlobalAttributes::ExampleModel));

			count=model->getObjectCount(ObjectType::Table);
			for(i=0; i < count; i++)
			{
				tab=new TableView(model->getTable(i));
				tab->setSelected(i==1);
				scene->addItem(tab);
			}

			count=model->getObjectCount(ObjectType::ForeignTable);
			for(i=0; i < count; i++)
			{
				tab=new TableView(model->getForeignTable(i));
				scene->addItem(tab);
			}

			count=model->getObjectCount(ObjectType::View);
			for(i=0; i < count; i++)
			{
				view=new GraphicalView(model->getView(i));
				scene->addItem(view);
			}

			count=model->getObjectCount(ObjectType::Relationship);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, ObjectType::Relationship));
				scene->addItem(rel);
			}

			count=model->getObjectCount(ObjectType::BaseRelationship);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, ObjectType::BaseRelationship));
				scene->addItem(rel);
			}

			count=model->getObjectCount(ObjectType::Textbox);
			for(i=0; i < count; i++)
			{
				txtbox=new StyledTextboxView(model->getTextbox(i));
				txtbox->setSelected(i==0);
				scene->addItem(txtbox);
			}

			placeholder->setRect(QRectF(400, 280, 200, 150));
			updatePlaceholderItem();
			scene->addItem(placeholder);
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
		int i, count=conf_items.size();

		BaseObjectView::loadObjectsStyle();
		this->loadExampleModel();

		for(i=0; i < count; i++)
		{
			if(conf_items[i].obj_conf)
			{
				BaseObjectView::getFillStyle(conf_items[i].conf_id, conf_items[i].colors[0], conf_items[i].colors[1]);
				conf_items[i].colors[2]=BaseObjectView::getBorderStyle(conf_items[i].conf_id).color();
			}
			else
				conf_items[i].font_fmt=BaseObjectView::getFontStyle(conf_items[i].conf_id);
		}

		this->enableConfigElement();
		font_cmb->setCurrentFont(BaseObjectView::getFontStyle(Attributes::Global).font());
		model->setObjectsModified();
		updatePlaceholderItem();
		scene->update();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo());
	}
}

void AppearanceConfigWidget::saveConfiguration()
{
	try
	{
		attribs_map attribs;
		vector<AppearanceConfigItem>::iterator itr, itr_end;
		AppearanceConfigItem item;
		QString attrib_id;
		QFont font;

		itr=conf_items.begin();
		itr_end=conf_items.end();

		while(itr!=itr_end)
		{
			item=(*itr);
			itr++;

			//If the item is a object color config
			if(item.obj_conf)
			{
				//Creates an attribute that stores the fill color
				attrib_id=item.conf_id + QString("-color");
				if(item.colors[0]==item.colors[1])
					attribs[attrib_id]=item.colors[0].name();
				else
					attribs[attrib_id]=item.colors[0].name() + QString(",") + item.colors[1].name();

				//Creates an attribute that stores the border color
				attrib_id=item.conf_id + QString("-bcolor");
				attribs[attrib_id]=item.colors[2].name();
			}
			//If the item is a font config
			else if(item.conf_id!=Attributes::Global && !item.obj_conf)
			{
				font=item.font_fmt.font();

				//Creates an attribute to store the font color
				attrib_id=item.conf_id + QString("-fcolor");
				attribs[attrib_id]=item.font_fmt.foreground().color().name();

				attrib_id=item.conf_id + QString("-") + Attributes::Italic;
				attribs[attrib_id]=(font.italic() ? Attributes::True : Attributes::False);

				attrib_id=item.conf_id + QString("-") + Attributes::Bold;
				attribs[attrib_id]=(font.bold() ? Attributes::True : Attributes::False);

				attrib_id=item.conf_id + QString("-") + Attributes::Underline;
				attribs[attrib_id]=(font.underline() ? Attributes::True : Attributes::False);
			}
			//Special case: treating the global font element
			else
			{
				attribs[QString("font-name")]=QFontInfo(item.font_fmt.font()).family();
				attribs[QString("font-size")]=QString("%1").arg(item.font_fmt.font().pointSizeF());
			}
		}

		config_params[GlobalAttributes::ObjectsStyleConf]=attribs;
		BaseConfigWidget::saveConfiguration(GlobalAttributes::ObjectsStyleConf, config_params);
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
	font_cmb->setEnabled(idx==0);
	font_lbl->setEnabled(idx==0);
	font_size_spb->setEnabled(idx==0);
	unity_lbl->setEnabled(idx==0);

	//Widgets enabled when a font configuration element is selected
	underline_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);
	bold_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);
	italic_chk->setEnabled(idx!=0 && !conf_items[idx].obj_conf);

	colors_lbl->setVisible(idx!=0);
	color_picker->setVisible(colors_lbl->isVisible());

	//Buttons visible when a object configuration element is selected
	color_picker->setButtonVisible(1, conf_items[idx].obj_conf);
	color_picker->setButtonVisible(2, conf_items[idx].obj_conf);

	underline_chk->blockSignals(true);
	italic_chk->blockSignals(true);
	bold_chk->blockSignals(true);
	font_cmb->blockSignals(true);
	font_size_spb->blockSignals(true);

	if(!conf_items[idx].obj_conf)
	{
		QTextCharFormat fmt=BaseObjectView::getFontStyle(conf_items[idx].conf_id);
		color_picker->setColor(0, fmt.foreground().color());
		underline_chk->setChecked(fmt.font().underline());
		italic_chk->setChecked(fmt.font().italic());
		bold_chk->setChecked(fmt.font().bold());
		font_cmb->setCurrentFont(fmt.font());
		font_size_spb->setValue(fmt.font().pointSizeF());
	}
	else
	{
		QColor color1, color2;
		BaseObjectView::getFillStyle(conf_items[idx].conf_id, color1, color2);

		color_picker->setColor(0, color1);
		color_picker->setColor(1, color2);
		color_picker->setColor(2, BaseObjectView::getBorderStyle(conf_items[idx].conf_id).color());
		underline_chk->setChecked(false);
		italic_chk->setChecked(false);
		bold_chk->setChecked(false);
	}

	underline_chk->blockSignals(false);
	italic_chk->blockSignals(false);
	bold_chk->blockSignals(false);
	font_cmb->blockSignals(false);
	font_size_spb->blockSignals(false);
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

void AppearanceConfigWidget::applyFontStyle()
{
	QFont font;

	font=font_cmb->currentFont();
	font.setBold(bold_chk->isChecked());
	font.setItalic(italic_chk->isChecked());
	font.setUnderline(underline_chk->isChecked());
	font.setPointSizeF(font_size_spb->value());

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
		BaseConfigWidget::restoreDefaults(GlobalAttributes::ObjectsStyleConf, false);
		this->loadConfiguration();
		setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

