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

#include "baseobjectview.h"

map<QString, QTextCharFormat> BaseObjectView::font_config;
map<QString, QColor *> BaseObjectView::color_config;
unsigned BaseObjectView::global_sel_order=1;

BaseObjectView::BaseObjectView(BaseObject *object)
{
	sel_order=0;
	protected_icon=NULL;
	obj_shadow=NULL;
	obj_selection=NULL;
	pos_info_pol=NULL;
	pos_info_txt=NULL;
	setSourceObject(object);
}

BaseObjectView::~BaseObjectView(void)
{
	setSourceObject(NULL);
}

void BaseObjectView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::RightButton && !this->isSelected())
	{
		if(this->scene()->selectedItems().count()==1)
			this->scene()->clearSelection();

		this->setSelected(true);
	}
	else
		QGraphicsItemGroup::mousePressEvent(event);
}

void BaseObjectView::setSourceObject(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	disconnect(this, SLOT(toggleProtectionIcon(bool)));

	if(graph_obj)
		graph_obj->setReceiverObject(NULL);

	//Stores the reference to the source object as the data of graphical object
	this->setData(0, QVariant::fromValue<void *>(object));

	if(!graph_obj)
	{
		if(obj_selection)
		{
			this->removeFromGroup(obj_selection);
			delete(obj_selection);
			obj_selection=NULL;
		}

		if(obj_shadow)
		{
			this->removeFromGroup(obj_shadow);
			delete(obj_shadow);
			obj_shadow=NULL;
		}

		if(protected_icon)
		{
			this->removeFromGroup(protected_icon);
			delete(protected_icon);
			protected_icon=NULL;
		}

		if(pos_info_txt)
		{
			this->removeFromGroup(pos_info_txt);
			delete(pos_info_txt);
			pos_info_txt=NULL;

			this->removeFromGroup(pos_info_pol);
			delete(pos_info_pol);
			pos_info_pol=NULL;
		}
	}
	else
	{
		QGraphicsPolygonItem *pol_item=NULL;

		connect(graph_obj, SIGNAL(s_objectProtected(bool)), this, SLOT(toggleProtectionIcon(bool)));
		graph_obj->setReceiverObject(this);

		//By default the item can be selected and send geometry changes to the scene
		this->setFlags(QGraphicsItem::ItemIsSelectable |
									 QGraphicsItem::ItemSendsGeometryChanges);
		//The object is only movable if is not protected
		this->setFlag(QGraphicsItem::ItemIsMovable, !graph_obj->isProtected());

		if(!protected_icon)
		{
			protected_icon=new QGraphicsItemGroup;
			protected_icon->setVisible(graph_obj->isProtected());
			protected_icon->setZValue(3);

			pol_item=new QGraphicsPolygonItem;
			protected_icon->addToGroup(pol_item);

			pol_item=new QGraphicsPolygonItem;
			protected_icon->addToGroup(pol_item);

			this->addToGroup(protected_icon);
		}

		if(!obj_selection)
		{
			obj_selection=new QGraphicsPolygonItem;
			this->addToGroup(obj_selection);
		}

		obj_selection->setVisible(false);
		obj_selection->setZValue(4);

		if(!obj_shadow)
		{
			obj_shadow=new QGraphicsPolygonItem;
			obj_shadow->setZValue(-1);
			this->addToGroup(obj_shadow);
		}

		if(!pos_info_txt)
		{
			pos_info_pol=new QGraphicsPolygonItem;
			pos_info_txt=new QGraphicsSimpleTextItem;
			pos_info_pol->setZValue(9);
			pos_info_txt->setZValue(10);
			this->addToGroup(pos_info_pol);
			this->addToGroup(pos_info_txt);
		}

		pos_info_pol->setVisible(false);
		pos_info_txt->setVisible(false);
	}
}

BaseObject *BaseObjectView::getSourceObject(void)
{
	return(reinterpret_cast<BaseObject *>(this->data(0).value<void *>()));
}

void BaseObjectView::resizePolygon(QPolygonF &pol, float width, float height)
{
	QVector<QPointF>::iterator itr,itr_end;
	float coef_a, coef_b;

	itr=pol.begin();
	itr_end=pol.end();

	//Calculates the resize factor
	coef_a=width / pol.boundingRect().width();
	coef_b=height / pol.boundingRect().height();

	//Applies the resize factor to all the polygon points
	while(itr!=itr_end)
	{
		itr->setX(itr->x() * coef_a);
		itr->setY(itr->y() * coef_b);
		itr++;
	}
}

void BaseObjectView::loadObjectsStyle(void)
{
	QColor *colors=NULL;
	QTextCharFormat font_fmt;
	QFont font;
	map<QString, QString> attribs;
	map<QString, QTextCharFormat>::iterator itr;
	QStringList list;
	QString elem,
			config_file=GlobalAttributes::CONFIGURATIONS_DIR + GlobalAttributes::DIR_SEPARATOR +
									GlobalAttributes::OBJECTS_STYLE_CONF + GlobalAttributes::CONFIGURATION_EXT;

	try
	{
		XMLParser::restartParser();
		XMLParser::setDTDFile(GlobalAttributes::CONFIGURATIONS_DIR +
													GlobalAttributes::DIR_SEPARATOR +
													GlobalAttributes::OBJECT_DTD_DIR +
													GlobalAttributes::DIR_SEPARATOR +
													GlobalAttributes::OBJECTS_STYLE_CONF +
													GlobalAttributes::OBJECT_DTD_EXT, GlobalAttributes::OBJECTS_STYLE_CONF);
		XMLParser::loadXMLFile(config_file);

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					XMLParser::getElementAttributes(attribs);
					elem=XMLParser::getElementName();

					if(elem==ParsersAttributes::GLOBAL)
					{
						font.setFamily(attribs[ParsersAttributes::FONT]);
						font.setPointSizeF(attribs[ParsersAttributes::SIZE].toFloat());
						font.setBold(attribs[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_);
						font.setItalic(attribs[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_);
						font.setUnderline(attribs[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_);
						font_fmt.setFont(font);
						font_config[ParsersAttributes::GLOBAL]=font_fmt;
					}
					else if(elem==ParsersAttributes::FONT)
					{
						font_config[attribs[ParsersAttributes::ID]]=font_fmt;
						itr=font_config.find(attribs[ParsersAttributes::ID]);
						font=font_fmt.font();
						font.setBold(attribs[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_);
						font.setItalic(attribs[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_);
						font.setUnderline(attribs[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_);
						(itr->second).setFont(font);
						(itr->second).setForeground(QColor(attribs[ParsersAttributes::COLOR]));
					}
					else if(elem==ParsersAttributes::OBJECT)
					{
						list=attribs[ParsersAttributes::FILL_COLOR].split(",");
						colors=new QColor[3];
						colors[0]=(!list.isEmpty() ? QColor(list[0]) : QColor(0,0,0));
						colors[1]=(list.size()==2 ? QColor(list[1]) : colors[0]);
						colors[2]=QColor(attribs[ParsersAttributes::BORDER_COLOR]);
						color_config[attribs[ParsersAttributes::ID]]=colors;
					}
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
	}
}

void BaseObjectView::setFontStyle(const QString &id, QTextCharFormat font_fmt)
{
	QFont font;

	if(id!=ParsersAttributes::GLOBAL)
	{
		font=font_config[ParsersAttributes::GLOBAL].font();
		font.setItalic(font_fmt.font().italic());
		font.setBold(font_fmt.font().bold());
		font.setUnderline(font_fmt.font().underline());
		font_fmt.setFont(font);
	}
	else
	{
		map<QString, QTextCharFormat>::iterator itr, itr_end;

		itr=font_config.begin();
		itr_end=font_config.end();
		font=font_fmt.font();

		while(itr!=itr_end)
		{
			font.setItalic((itr->second).font().italic());
			font.setBold((itr->second).font().bold());
			font.setUnderline((itr->second).font().underline());
			(itr->second).setFont(font);
			itr++;
		}
	}

	if(font_config.count(id))
		font_config[id]=font_fmt;
}

void BaseObjectView::setElementColor(const QString &id, QColor color, unsigned color_id)
{
	if(color_id < 3 && color_config.count(id))
		color_config[id][color_id]=color;
}

void BaseObjectView::getFillStyle(const QString &id, QColor &color1, QColor &color2)
{
	if(color_config.count(id) > 0)
	{
		color1=color_config[id][0];
		color2=color_config[id][1];
	}
}

QLinearGradient BaseObjectView::getFillStyle(const QString &id)
{
	QColor *colors=NULL;
	QLinearGradient grad(QPointF(0,0),QPointF(0,1));

	if(color_config.count(id) > 0)
	{
		colors=color_config[id];
		if(colors)
		{
			if(id==ParsersAttributes::OBJ_SELECTION)
			{
				colors[0].setAlpha(128);
				colors[1].setAlpha(128);
			}

			grad.setCoordinateMode(QGradient::ObjectBoundingMode);
			grad.setColorAt(0, colors[0]);
			grad.setColorAt(1, colors[1]);
		}
	}

	return(grad);
}

QPen BaseObjectView::getBorderStyle(const QString &id)
{
	QPen pen;
	QColor *colors=NULL;

	if(color_config.count(id) > 0)
	{
		colors=color_config[id];
		if(colors)
		{
			if(id==ParsersAttributes::OBJ_SELECTION)
				colors[2].setAlpha(128);

			pen.setWidthF(OBJ_BORDER_WIDTH);
			pen.setColor(colors[2]);
		}
	}

	return(pen);
}

QTextCharFormat BaseObjectView::getFontStyle(const QString &id)
{
	if(font_config.count(id))
		return(font_config[id]);
	else
		return(QTextCharFormat());
}


QVariant BaseObjectView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemPositionHasChanged)
	{
		BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(this->getSourceObject());

		if(graph_obj && !graph_obj->isProtected())
		{
			graph_obj->setPosition(this->scenePos());
			this->configurePositionInfo(this->pos());
		}
	}
	else if(change==ItemSelectedHasChanged && obj_selection)
	{
		this->setSelectionOrder(value.toBool());
		pos_info_pol->setVisible(value.toBool());
		pos_info_txt->setVisible(value.toBool());
		obj_selection->setVisible(value.toBool());
		this->configurePositionInfo(this->pos());

		emit s_objectSelected(dynamic_cast<BaseGraphicObject *>(this->getSourceObject()),
													value.toBool());
	}

	return(value);
}

void BaseObjectView::setSelectionOrder(bool selected)
{
	if(this->sel_order==0 && selected)
		this->sel_order=++BaseObjectView::global_sel_order;
	else if(!selected)
		this->sel_order=0;
}

QRectF BaseObjectView::boundingRect(void) const
{
	return(bounding_rect);
}

void BaseObjectView::toggleProtectionIcon(bool value)
{
	BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(this->getSourceObject());

	protected_icon->setVisible(value);
	this->setFlag(QGraphicsItem::ItemIsMovable, !value);

	if(obj_graf)
		obj_graf->setModified(true);
}

void BaseObjectView::configurePositionInfo(QPointF pos)
{
	if(this->isSelected())
	{
		QPolygonF pol;

		pos_info_pol->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::POSITION_INFO));
		pos_info_pol->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::POSITION_INFO));
		pos_info_txt->setFont(font_config[ParsersAttributes::POSITION_INFO].font());
		pos_info_txt->setBrush(font_config[ParsersAttributes::POSITION_INFO].foreground());

		pos_info_txt->setText(QString(" x=%1 y=%2 ").arg(pos.x()).arg(pos.y()));
		pol.append(pos_info_txt->boundingRect().topLeft());
		pol.append(pos_info_txt->boundingRect().topRight());
		pol.append(pos_info_txt->boundingRect().bottomRight());
		pol.append(pos_info_txt->boundingRect().bottomLeft());
		pos_info_pol->setPolygon(pol);
		pos_info_txt->setPos(0,
												 - pos_info_txt->boundingRect().height());
		pos_info_pol->setPos(0,
												 - pos_info_pol->boundingRect().height());
	}
}

void BaseObjectView::configureObjectShadow(void)
{
	if(obj_shadow)
	{
		QRectF ret;
		QPolygonF pol;

		ret=this->boundingRect();
		pol.append(QPointF(ret.right()-1, ret.top()+7.5));
		pol.append(QPointF(ret.right()+2.5f, ret.top()+7.5f));
		pol.append(QPointF(ret.right()+2.5f, ret.bottom()+2.5f));
		pol.append(QPointF(ret.left()+7.5f, ret.bottom()+2.5f));
		pol.append(QPointF(ret.left()+7.5f, ret.bottom()-1));
		pol.append(QPointF(ret.right()-1, ret.bottom()-1));
		obj_shadow->setPolygon(pol);
		obj_shadow->setPos(0,0);
		obj_shadow->setPen(QColor(0,0,0,100));
		obj_shadow->setBrush(QColor(0,0,0,100));
	}
}

void BaseObjectView::configureObjectSelection(void)
{
	if(obj_selection)
	{
		QRectF ret;
		QPolygonF pol;

		ret=this->boundingRect();
		pol.append(ret.topLeft());
		pol.append(ret.topRight());
		pol.append(ret.bottomRight());
		pol.append(ret.bottomLeft());
		obj_selection->setPolygon(pol);
		obj_selection->setPos(0,0);
		obj_selection->setBrush(this->getFillStyle(ParsersAttributes::OBJ_SELECTION));
		obj_selection->setPen(this->getBorderStyle(ParsersAttributes::OBJ_SELECTION));
	}
}
void BaseObjectView::configureProtectedIcon(void)
{
	if(protected_icon)
	{
		QGraphicsPolygonItem *pol_item=NULL;
		QPolygonF pol;
		float factor;

		//Calculates the factor used to resize the protection icon accordding the font size
		factor=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;

		pol.append(QPointF(2,5)); pol.append(QPointF(2,2));
		pol.append(QPointF(3,1)); pol.append(QPointF(4,0));
		pol.append(QPointF(7,0)); pol.append(QPointF(8,1));
		pol.append(QPointF(9,2)); pol.append(QPointF(9,5));
		pol.append(QPointF(7,5)); pol.append(QPointF(7,3));
		pol.append(QPointF(6,2)); pol.append(QPointF(5,2));
		pol.append(QPointF(4,3)); pol.append(QPointF(4,5));

		if(factor!=1.0f)
			this->resizePolygon(pol, pol.boundingRect().width() * factor,
													pol.boundingRect().height() * factor);

		pol_item=dynamic_cast<QGraphicsPolygonItem *>(protected_icon->children().at(0));
		pol_item->setPolygon(pol);
		pol_item->setBrush(this->getFillStyle(ParsersAttributes::LOCKER_ARC));
		pol_item->setPen(this->getBorderStyle(ParsersAttributes::LOCKER_ARC));

		pol.clear();
		pol.append(QPointF(1,5));  pol.append(QPointF(10,5));
		pol.append(QPointF(11,6)); pol.append(QPointF(11,9));
		pol.append(QPointF(10,10)); pol.append(QPointF(1,10));
		pol.append(QPointF(0,9)); pol.append(QPointF(0,6));

		if(factor!=1.0f)
			this->resizePolygon(pol, pol.boundingRect().width() * factor,
													pol.boundingRect().height() * factor);

		pol_item=dynamic_cast<QGraphicsPolygonItem *>(protected_icon->children().at(1));
		pol_item->setPolygon(pol);
		pol_item->setBrush(this->getFillStyle(ParsersAttributes::LOCKER_BODY));
		pol_item->setPen(this->getBorderStyle(ParsersAttributes::LOCKER_BODY));
	}
}

void BaseObjectView::__configureObject(void)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(this->getSourceObject());

	if(graph_obj)
	{
		this->setPos(graph_obj->getPosition());
		this->setToolTip(Utf8String::create(graph_obj->getName(true)) +
										 " (" + graph_obj->getTypeName() + ")");
		this->configurePositionInfo(graph_obj->getPosition());
		this->configureProtectedIcon();
	}
}

unsigned BaseObjectView::getSelectionOrder(void)
{
	return(sel_order);
}
