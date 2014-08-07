/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

AppearanceConfigWidget::AppearanceConfigWidget(QWidget * parent) : QWidget(parent)
{
	setupUi(this);

	QString conf_ids[]={
		ParsersAttributes::GLOBAL,ParsersAttributes::CONSTRAINTS, ParsersAttributes::OBJ_SELECTION,
		ParsersAttributes::POSITION_INFO, ParsersAttributes::POSITION_INFO,
		ParsersAttributes::OBJECT_TYPE, ParsersAttributes::LOCKER_ARC, ParsersAttributes::LOCKER_BODY,
		ParsersAttributes::TABLE_SCHEMA_NAME, ParsersAttributes::TABLE_NAME, ParsersAttributes::TABLE_BODY,
		ParsersAttributes::TABLE_EXT_BODY, ParsersAttributes::TABLE_TITLE, BaseObject::getSchemaName(OBJ_RULE),
		BaseObject::getSchemaName(OBJ_RULE), BaseObject::getSchemaName(OBJ_INDEX),
		BaseObject::getSchemaName(OBJ_INDEX), BaseObject::getSchemaName(OBJ_TRIGGER),
		BaseObject::getSchemaName(OBJ_TRIGGER), ParsersAttributes::VIEW_SCHEMA_NAME, ParsersAttributes::VIEW_NAME,
		ParsersAttributes::VIEW_BODY /*21*/, ParsersAttributes::VIEW_EXT_BODY /*22*/, ParsersAttributes::VIEW_TITLE /*23*/, ParsersAttributes::ALIAS,
		ParsersAttributes::REF_COLUMN, ParsersAttributes::REF_TABLE, ParsersAttributes::REFERENCE,
		BaseObject::getSchemaName(OBJ_TEXTBOX), ParsersAttributes::COLUMN, ParsersAttributes::COLUMN,
		ParsersAttributes::INH_COLUMN, ParsersAttributes::PROT_COLUMN, ParsersAttributes::PK_COLUMN,
		ParsersAttributes::PK_COLUMN, ParsersAttributes::FK_COLUMN, ParsersAttributes::FK_COLUMN,
		ParsersAttributes::UQ_COLUMN, ParsersAttributes::UQ_COLUMN, ParsersAttributes::NN_COLUMN,
		ParsersAttributes::NN_COLUMN, ParsersAttributes::RELATIONSHIP, ParsersAttributes::LABEL,
    ParsersAttributes::LABEL, ParsersAttributes::ATTRIBUTE, ParsersAttributes::ATTRIBUTE,
    ParsersAttributes::TAG, ParsersAttributes::TAG };
	int i, count=element_cmb->count(),
			//This auxiliary vector stores the id of elements that represents color/font conf. of objects
			obj_conf_ids_vect[]={ 2, 4, 6, 7, 10, 11, 12, 14, 16, 18, 21, 22, 23,
                            27, 28, 30, 34, 36, 38, 40, 41, 43, 45, 47 };
	vector<int> conf_obj_ids(obj_conf_ids_vect, obj_conf_ids_vect + sizeof(obj_conf_ids_vect) / sizeof(int));

	conf_items.resize(count);
	for(i=0; i < count; i++)
	{
		conf_items[i].conf_id=conf_ids[i];
		conf_items[i].obj_conf=(std::find(conf_obj_ids.begin(), conf_obj_ids.end(), i)!=conf_obj_ids.end());
	}

	color_picker=new ColorPickerWidget(3, this);

	model=new DatabaseModel;
	scene=new ObjectsScene;
	scene->setSceneRect(QRectF(0,0,500,500));

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

	appearanceconfig_grid->addWidget(color_picker, 3, 2, 1, 4);
	appearanceconfig_grid->addWidget(viewp, 4 , 0, 1, 6);

	connect(element_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(enableConfigElement(void)));
	connect(font_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(applyFontStyle(void)));
	connect(font_size_spb, SIGNAL(valueChanged(double)), this, SLOT(applyFontStyle(void)));
	connect(bold_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));
	connect(underline_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));
	connect(italic_chk, SIGNAL(toggled(bool)), this, SLOT(applyFontStyle(void)));

	connect(color_picker, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(applyElementColor(unsigned, QColor)));

	connect(color_picker, &ColorPickerWidget::s_colorsChanged,
					[=](){
									for(unsigned i=0; i < color_picker->getColorCount(); i++)
										applyElementColor(i, color_picker->getColor(i));
							 });
}

AppearanceConfigWidget::~AppearanceConfigWidget(void)
{
	delete(viewp);
	delete(scene);
	delete(model);
}

void AppearanceConfigWidget::loadExampleModel(void)
{
	try
	{
		RelationshipView *rel=nullptr;
		TextboxView *txtbox=nullptr;
		TableView *tab=nullptr;
		GraphicalView *view=nullptr;
		unsigned count, i;

		if(model->getObjectCount()==0)
		{
			model->loadModel(GlobalAttributes::CONFIGURATIONS_DIR +
											 GlobalAttributes::DIR_SEPARATOR +
											 GlobalAttributes::EXAMPLE_MODEL);

			count=model->getObjectCount(OBJ_TABLE);
			for(i=0; i < count; i++)
			{
				tab=new TableView(model->getTable(i));
				tab->setSelected(i==1);
				scene->addItem(tab);
			}

			count=model->getObjectCount(OBJ_VIEW);
			for(i=0; i < count; i++)
			{
				view=new GraphicalView(model->getView(i));
				scene->addItem(view);
			}

			count=model->getObjectCount(OBJ_RELATIONSHIP);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, OBJ_RELATIONSHIP));
				scene->addItem(rel);
			}

			count=model->getObjectCount(BASE_RELATIONSHIP);
			for(i=0; i < count; i++)
			{
				rel=new RelationshipView(model->getRelationship(i, BASE_RELATIONSHIP));
				scene->addItem(rel);
			}

			count=model->getObjectCount(OBJ_TEXTBOX);
			for(i=0; i < count; i++)
			{
				txtbox=new TextboxView(model->getTextbox(i));
				txtbox->setSelected(i==0);
				scene->addItem(txtbox);
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
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
		scene->update();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
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
				attribs["font-name"]=QFontInfo(item.font_fmt.font()).family();
				attribs["font-size"]=QString("%1").arg(item.font_fmt.font().pointSizeF());
			}
		}

		config_params[GlobalAttributes::OBJECTS_STYLE_CONF]=attribs;
		BaseConfigWidget::saveConfiguration(GlobalAttributes::OBJECTS_STYLE_CONF);
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
	}
	else
	{
		conf_items[element_cmb->currentIndex()].font_fmt.setForeground(color);
		BaseObjectView::setFontStyle(conf_items[element_cmb->currentIndex()].conf_id,
				conf_items[element_cmb->currentIndex()].font_fmt);
	}

	model->setObjectsModified();
	scene->update();
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
}

void AppearanceConfigWidget::restoreDefaults(void)
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::OBJECTS_STYLE_CONF);
		this->loadConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

