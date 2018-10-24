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

#include "appearanceconfigwidget.h"

map<QString, attribs_map> AppearanceConfigWidget::config_params;

AppearanceConfigWidget::AppearanceConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
{
	setupUi(this);

	QString conf_ids[]={
		ParsersAttributes::GLOBAL, //0
		ParsersAttributes::CONSTRAINTS, //1
		ParsersAttributes::OBJ_SELECTION, //2
		ParsersAttributes::POSITION_INFO, //3
		ParsersAttributes::POSITION_INFO, //4
		ParsersAttributes::OBJECT_TYPE, //5
		ParsersAttributes::LOCKER_ARC, //6
		ParsersAttributes::LOCKER_BODY, //7
		ParsersAttributes::TABLE_SCHEMA_NAME, //8
		ParsersAttributes::TABLE_NAME, //9
		ParsersAttributes::TABLE_BODY, //10
		ParsersAttributes::TABLE_EXT_BODY, //11
		ParsersAttributes::TABLE_TITLE, //12
		BaseObject::getSchemaName(ObjectType::ObjRule), //13
		BaseObject::getSchemaName(ObjectType::ObjRule), //14
		BaseObject::getSchemaName(ObjectType::ObjIndex), //15
		BaseObject::getSchemaName(ObjectType::ObjIndex), //16
		BaseObject::getSchemaName(ObjectType::ObjTrigger), //17
		BaseObject::getSchemaName(ObjectType::ObjTrigger), //18
		BaseObject::getSchemaName(ObjectType::ObjConstraint), //19
		BaseObject::getSchemaName(ObjectType::ObjConstraint), //20
		BaseObject::getSchemaName(ObjectType::ObjPolicy), //21
		BaseObject::getSchemaName(ObjectType::ObjPolicy), //22
		ParsersAttributes::VIEW_SCHEMA_NAME, //21 -> 23
		ParsersAttributes::VIEW_NAME, //22
		ParsersAttributes::VIEW_BODY, //23
		ParsersAttributes::VIEW_EXT_BODY, //24
		ParsersAttributes::VIEW_TITLE, //25
		ParsersAttributes::ALIAS, //26
		ParsersAttributes::REF_COLUMN, //27
		ParsersAttributes::REF_TABLE, //28
		ParsersAttributes::REFERENCE, //29
		BaseObject::getSchemaName(ObjectType::ObjTextbox), //30
		ParsersAttributes::COLUMN, //31
		ParsersAttributes::COLUMN, //32
		ParsersAttributes::INH_COLUMN, //33
		ParsersAttributes::PROT_COLUMN, //34
		ParsersAttributes::PK_COLUMN, //35
		ParsersAttributes::PK_COLUMN, //36
		ParsersAttributes::FK_COLUMN, //37
		ParsersAttributes::FK_COLUMN, //38
		ParsersAttributes::UQ_COLUMN, //39
		ParsersAttributes::UQ_COLUMN, //40
		ParsersAttributes::NN_COLUMN, //41
		ParsersAttributes::NN_COLUMN, //42
		ParsersAttributes::RELATIONSHIP, //43
		ParsersAttributes::LABEL, //44
		ParsersAttributes::LABEL, //45
		ParsersAttributes::ATTRIBUTE, //46
		ParsersAttributes::ATTRIBUTE, //47
		ParsersAttributes::TAG, //48
		ParsersAttributes::TAG, //49
		ParsersAttributes::PLACEHOLDER /*50*/};
	int i, count=element_cmb->count(),
			//This auxiliary vector stores the id of elements that represents color/font conf. of objects
			obj_conf_ids_vect[]={ 2, 4, 6, 7, 10, 11, 12, 14, 16, 18, 20, 22, 25, 26, 27,
														31, 32, 34, 38, 40, 42, 44, 45, 47, 49, 51, 52 };
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

	connect(element_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableConfigElement(void)));
	connect(font_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(applyFontStyle(void)));
	connect(font_size_spb, SIGNAL(valueChanged(double)), this, SLOT(applyFontStyle(void)));
	connect(bold_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));
	connect(underline_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));
	connect(italic_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));

	connect(color_picker, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(applyElementColor(unsigned, QColor)));

	connect(color_picker, &ColorPickerWidget::s_colorsChanged,
			[&](){
		for(unsigned i=0; i < color_picker->getColorCount(); i++)
			applyElementColor(i, color_picker->getColor(i));
	});
}

AppearanceConfigWidget::~AppearanceConfigWidget(void)
{
	scene->removeItem(placeholder);

	delete(placeholder);
	delete(viewp);
	delete(scene);
	delete(model);
}

map<QString, attribs_map> AppearanceConfigWidget::getConfigurationParams(void)
{
	return(config_params);
}

void AppearanceConfigWidget::loadExampleModel(void)
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
			model->loadModel(GlobalAttributes::TmplConfigurationDir +
							 GlobalAttributes::DirSeparator +
							 GlobalAttributes::ExampleModel);

			count=model->getObjectCount(ObjectType::ObjTable);
			for(i=0; i < count; i++)
			{
				tab=new TableView(model->getTable(i));
				tab->setSelected(i==1);
				scene->addItem(tab);
			}

			count=model->getObjectCount(ObjectType::ObjView);
			for(i=0; i < count; i++)
			{
				view=new GraphicalView(model->getView(i));
				scene->addItem(view);
			}

			count=model->getObjectCount(ObjectType::ObjRelationship);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, ObjectType::ObjRelationship));
				scene->addItem(rel);
			}

			count=model->getObjectCount(ObjectType::ObjBaseRelationship);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, ObjectType::ObjBaseRelationship));
				scene->addItem(rel);
			}

			count=model->getObjectCount(ObjectType::ObjTextbox);
			for(i=0; i < count; i++)
			{
				txtbox=new StyledTextboxView(model->getTextbox(i));
				txtbox->setSelected(i==0);
				scene->addItem(txtbox);
			}

			placeholder->setRect(QRectF(170, 190, 100,50));
			updatePlaceholderItem();
			scene->addItem(placeholder);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void AppearanceConfigWidget::updatePlaceholderItem(void)
{
	placeholder->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::PLACEHOLDER));
	QPen pen=BaseObjectView::getBorderStyle(ParsersAttributes::PLACEHOLDER);
	pen.setStyle(Qt::DashLine);
	placeholder->setPen(pen);
}

void AppearanceConfigWidget::loadConfiguration(void)
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
		font_cmb->setCurrentFont(BaseObjectView::getFontStyle(ParsersAttributes::GLOBAL).font());
		model->setObjectsModified();
		updatePlaceholderItem();
		scene->update();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo());
	}
}

void AppearanceConfigWidget::saveConfiguration(void)
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
			else if(item.conf_id!=ParsersAttributes::GLOBAL && !item.obj_conf)
			{
				font=item.font_fmt.font();

				//Creates an attribute to store the font color
				attrib_id=item.conf_id + QString("-fcolor");
				attribs[attrib_id]=item.font_fmt.foreground().color().name();

				attrib_id=item.conf_id + QString("-") + ParsersAttributes::ITALIC;
				attribs[attrib_id]=(font.italic() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

				attrib_id=item.conf_id + QString("-") + ParsersAttributes::BOLD;
				attribs[attrib_id]=(font.bold() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);

				attrib_id=item.conf_id + QString("-") + ParsersAttributes::UNDERLINE;
				attribs[attrib_id]=(font.underline() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
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
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void AppearanceConfigWidget::enableConfigElement(void)
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

void AppearanceConfigWidget::applyFontStyle(void)
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

void AppearanceConfigWidget::restoreDefaults(void)
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::ObjectsStyleConf, false);
		this->loadConfiguration();
		setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

