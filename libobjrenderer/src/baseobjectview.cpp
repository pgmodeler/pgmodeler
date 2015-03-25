/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
#include "textboxview.h"
#include "roundedrectitem.h"

map<QString, QTextCharFormat> BaseObjectView::font_config;
map<QString, QColor *> BaseObjectView::color_config;
unsigned BaseObjectView::global_sel_order=1;

const float BaseObjectView::VERT_SPACING=2.0f;
const float BaseObjectView::HORIZ_SPACING=2.0f;
const float BaseObjectView::DEFAULT_FONT_SIZE=9.0f;
const float BaseObjectView::OBJ_BORDER_WIDTH=0.80f;

BaseObjectView::BaseObjectView(BaseObject *object)
{
	sel_order=0;
	protected_icon=nullptr;
	obj_shadow=nullptr;
	obj_selection=nullptr;
  pos_info_rect=nullptr;
	pos_info_txt=nullptr;
  sql_disabled_txt=nullptr;
  sql_disabled_box=nullptr;
	setSourceObject(object);
}

BaseObjectView::~BaseObjectView(void)
{  
	setSourceObject(nullptr);
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

	//Stores the reference to the source object as the data of graphical object
	this->setData(0, QVariant::fromValue<void *>(object));

	if(!graph_obj)
	{
    if(obj_selection)
		{
			this->removeFromGroup(obj_selection);
			delete(obj_selection);
			obj_selection=nullptr;
    }

    if(obj_shadow)
		{
			this->removeFromGroup(obj_shadow);
			delete(obj_shadow);
			obj_shadow=nullptr;
    }

		if(protected_icon)
		{
			this->removeFromGroup(protected_icon);
			delete(protected_icon);
			protected_icon=nullptr;
		}

		if(pos_info_txt)
		{
			this->removeFromGroup(pos_info_txt);
			delete(pos_info_txt);
			pos_info_txt=nullptr;

      this->removeFromGroup(pos_info_rect);
      delete(pos_info_rect);
      pos_info_rect=nullptr;
		}

    if(sql_disabled_box)
    {
      this->removeFromGroup(sql_disabled_txt);
      delete(sql_disabled_txt);
      sql_disabled_txt=nullptr;

      this->removeFromGroup(sql_disabled_box);
      delete(sql_disabled_box);
      sql_disabled_box=nullptr;
    }
	}
	else
	{
		QGraphicsPolygonItem *pol_item=nullptr;

    graph_obj->disconnect();
    graph_obj->setReceiverObject(this);
    connect(graph_obj, SIGNAL(s_objectProtected(bool)), this, SLOT(toggleProtectionIcon(bool)));

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

		if(!pos_info_txt)
		{
      pos_info_rect=new QGraphicsRectItem;
			pos_info_txt=new QGraphicsSimpleTextItem;
      pos_info_rect->setZValue(9);
			pos_info_txt->setZValue(10);      

      this->addToGroup(pos_info_rect);
      this->addToGroup(pos_info_txt);
    }

    if(!sql_disabled_box && object->getObjectType()!=OBJ_TEXTBOX)
    {
      sql_disabled_txt=new QGraphicsSimpleTextItem;
      sql_disabled_box=new QGraphicsRectItem;
      sql_disabled_txt->setZValue(100);
      sql_disabled_box->setZValue(99);

      this->addToGroup(sql_disabled_box);
      this->addToGroup(sql_disabled_txt);
    }
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
	QColor *colors=nullptr;
	QTextCharFormat font_fmt;
	QFont font;
	attribs_map attribs;
	map<QString, QTextCharFormat>::iterator itr;
	QStringList list;
	QString elem,
			config_file=GlobalAttributes::CONFIGURATIONS_DIR + GlobalAttributes::DIR_SEPARATOR +
									GlobalAttributes::OBJECTS_STYLE_CONF + GlobalAttributes::CONFIGURATION_EXT;
	XMLParser xmlparser;

	try
	{
		xmlparser.restartParser();
		xmlparser.setDTDFile(GlobalAttributes::CONFIGURATIONS_DIR +
													GlobalAttributes::DIR_SEPARATOR +
													GlobalAttributes::OBJECT_DTD_DIR +
													GlobalAttributes::DIR_SEPARATOR +
													GlobalAttributes::OBJECTS_STYLE_CONF +
													GlobalAttributes::OBJECT_DTD_EXT, GlobalAttributes::OBJECTS_STYLE_CONF);
		xmlparser.loadXMLFile(config_file);

		if(xmlparser.accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					xmlparser.getElementAttributes(attribs);
					elem=xmlparser.getElementName();

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
            list=attribs[ParsersAttributes::FILL_COLOR].split(',');
						colors=new QColor[3];
						colors[0]=(!list.isEmpty() ? QColor(list[0]) : QColor(0,0,0));
						colors[1]=(list.size()==2 ? QColor(list[1]) : colors[0]);
						colors[2]=QColor(attribs[ParsersAttributes::BORDER_COLOR]);
						color_config[attribs[ParsersAttributes::ID]]=colors;
					}
				}
			}
			while(xmlparser.accessElement(XMLParser::NEXT_ELEMENT));
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

QColor BaseObjectView::getElementColor(const QString &id, unsigned color_id)
{
  if(color_config.count(id) > 0 && color_id < 3)
   return(color_config[id][color_id]);
  else
   return(QColor(0,0,0));
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
	QColor *colors=nullptr;
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
	QColor *colors=nullptr;

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
    pos_info_rect->setVisible(value.toBool());
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
    QFont fnt=font_config[ParsersAttributes::POSITION_INFO].font();

    pos_info_rect->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::POSITION_INFO));
    pos_info_rect->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::POSITION_INFO));

    fnt.setPointSizeF(fnt.pointSizeF() * 0.95);
    pos_info_txt->setFont(fnt);
		pos_info_txt->setBrush(font_config[ParsersAttributes::POSITION_INFO].foreground());

    pos_info_txt->setText(QString(" x:%1 y:%2 ").arg(pos.x()).arg(pos.y()));
    pos_info_rect->setRect(pos_info_txt->boundingRect());
    pos_info_txt->setPos(-0.5, -pos_info_txt->boundingRect().height()/2);
    pos_info_rect->setPos(-0.5, -pos_info_rect->boundingRect().height()/2);
  }
}

void BaseObjectView::configureSQLDisabledInfo(void)
{
  if(sql_disabled_box)
  {
    float px=0, py=0;

    sql_disabled_txt->setVisible(this->getSourceObject()->isSQLDisabled());
    sql_disabled_box->setVisible(this->getSourceObject()->isSQLDisabled());

    if(this->getSourceObject()->isSQLDisabled())
    {    
      QTextCharFormat char_fmt;
      char_fmt=BaseObjectView::getFontStyle(ParsersAttributes::POSITION_INFO);
      char_fmt.setFontPointSize(char_fmt.font().pointSizeF() * 0.80);

      sql_disabled_txt->setFont(char_fmt.font());
      sql_disabled_txt->setText(trUtf8("SQL off"));
      sql_disabled_txt->setBrush(char_fmt.foreground());

      sql_disabled_box->setRect(QRectF(QPointF(0,0), sql_disabled_txt->boundingRect().size() + QSizeF(1.5 * HORIZ_SPACING, 1.5 * VERT_SPACING)));
      sql_disabled_box->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::POSITION_INFO));
      sql_disabled_box->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::POSITION_INFO));

      px=bounding_rect.width() - sql_disabled_box->boundingRect().width() + (1.5 * HORIZ_SPACING),
      py=-(sql_disabled_box->boundingRect().height()/2);

      sql_disabled_txt->setPos(px + (HORIZ_SPACING * 0.75), py + (VERT_SPACING * 0.75));
      sql_disabled_box->setPos(px, py);
    }
  }
}

void BaseObjectView::configureObjectShadow(void)
{
  RoundedRectItem *rect_item=dynamic_cast<RoundedRectItem *>(obj_shadow);

  if(rect_item)
	{
    rect_item->setPen(Qt::NoPen);
    rect_item->setBrush(QColor(50,50,50,60));
    rect_item->setRect(this->boundingRect());
    rect_item->setPos(3.5,3.5);
	}
}

void BaseObjectView::configureObjectSelection(void)
{
  RoundedRectItem *rect_item=dynamic_cast<RoundedRectItem *>(obj_selection);

  if(rect_item)
	{
    rect_item->setRect(this->boundingRect());
    rect_item->setPos(0,0);
    rect_item->setBorderRadius(5);
    rect_item->setBrush(this->getFillStyle(ParsersAttributes::OBJ_SELECTION));
    rect_item->setPen(this->getBorderStyle(ParsersAttributes::OBJ_SELECTION));
	}
}

void BaseObjectView::configureProtectedIcon(void)
{
	if(protected_icon)
	{
		QGraphicsPolygonItem *pol_item=nullptr;
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

		pol_item=dynamic_cast<QGraphicsPolygonItem *>(protected_icon->childItems().at(0));
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

		pol_item=dynamic_cast<QGraphicsPolygonItem *>(protected_icon->childItems().at(1));
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
    this->setToolTip(/*Utf8String::create(*/graph_obj->getName(true) +
                     QString(" (") + graph_obj->getTypeName() +
                     QString(") ") + QString("\nId: %1").arg(graph_obj->getObjectId()));
		this->configurePositionInfo(graph_obj->getPosition());
		this->configureProtectedIcon();
	}
}

unsigned BaseObjectView::getSelectionOrder(void)
{
	return(sel_order);
}

QPointF BaseObjectView::getCenter(void)
{
	return(QPointF(this->pos().x() + this->boundingRect().width()/2.0f,
								 this->pos().y() + this->boundingRect().height()/2.0f));
}
