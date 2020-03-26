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

#include "baseobjectview.h"
#include "textboxview.h"
#include "roundedrectitem.h"
#include "objectsscene.h"

map<QString, QTextCharFormat> BaseObjectView::font_config;
map<QString, vector<QColor>> BaseObjectView::color_config;
unsigned BaseObjectView::global_sel_order=1;
bool BaseObjectView::use_placeholder=true;
bool BaseObjectView::compact_view=false;

BaseObjectView::BaseObjectView(BaseObject *object)
{
	sel_order=0;
	protected_icon=nullptr;
	obj_shadow=nullptr;
	obj_selection=nullptr;
	pos_info_item=nullptr;
	sql_disabled_item=nullptr;
	placeholder=nullptr;
	setSourceObject(object);
}

BaseObjectView::~BaseObjectView()
{  
	setSourceObject(nullptr);
}

void BaseObjectView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::RightButton && !this->isSelected())
	{
		//Faking an left-click in order to force the object selection using the right button.
		QGraphicsSceneMouseEvent *m_event=new QGraphicsSceneMouseEvent;
		m_event->setPos(event->pos());
		m_event->setScenePos(event->scenePos());
		m_event->setScreenPos(event->screenPos());
		m_event->setButton(Qt::LeftButton);

		QGraphicsItemGroup::mousePressEvent(m_event);
		event->ignore();
	}
	else if(event->button()==Qt::LeftButton)
		QGraphicsItemGroup::mousePressEvent(event);
}

void BaseObjectView::setSourceObject(BaseObject *object)
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(object);

	//Stores the reference to the source object as the data of graphical object
	this->setData(0, QVariant::fromValue<void *>(object));

	if(!graph_obj)
	{
		if(obj_shadow)
		{
			this->removeFromGroup(obj_shadow);
			delete obj_shadow;
			obj_shadow=nullptr;
		}

		if(protected_icon)
		{
			this->removeFromGroup(protected_icon);
			delete protected_icon;
			protected_icon=nullptr;
		}

		if(pos_info_item)
		{
			this->removeFromGroup(pos_info_item);
			delete pos_info_item;
			pos_info_item=nullptr;
		}

		if(sql_disabled_item)
		{
			this->removeFromGroup(sql_disabled_item);
			delete sql_disabled_item;
			sql_disabled_item=nullptr;
		}

		if(placeholder)
		{
			delete placeholder;
			placeholder=nullptr;
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

		if(!pos_info_item)
		{
			pos_info_item=new TextPolygonItem;
			pos_info_item->setZValue(10);
			this->addToGroup(pos_info_item);
		}

		if(!sql_disabled_item && object->getObjectType()!=ObjectType::Textbox)
		{
			sql_disabled_item=new TextPolygonItem;
			sql_disabled_item->setZValue(100);
			this->addToGroup(sql_disabled_item);
		}
	}
}

BaseObject *BaseObjectView::getUnderlyingObject()
{
	return reinterpret_cast<BaseObject *>(this->data(0).value<void *>());
}

void BaseObjectView::loadObjectsStyle()
{
	QTextCharFormat font_fmt;
	QFont font;
	attribs_map attribs;
	map<QString, QTextCharFormat>::iterator itr;
	QStringList list;
	QString elem,
			config_file=GlobalAttributes::getConfigurationFilePath(GlobalAttributes::ObjectsStyleConf);
	XmlParser xmlparser;

	try
	{
		xmlparser.restartParser();

		xmlparser.setDTDFile(GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::ObjectDTDDir,
																																				GlobalAttributes::ObjectsStyleConf +
																																				GlobalAttributes::ObjectDTDExt),
												 GlobalAttributes::ObjectsStyleConf);

		xmlparser.loadXMLFile(config_file);

		if(xmlparser.accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser.getElementType()==XML_ELEMENT_NODE)
				{
					xmlparser.getElementAttributes(attribs);
					elem=xmlparser.getElementName();

					if(elem==Attributes::Global)
					{
						font.setFamily(attribs[Attributes::Font]);
						font.setPointSizeF(attribs[Attributes::Size].toDouble());
						font.setBold(attribs[Attributes::Bold]==Attributes::True);
						font.setItalic(attribs[Attributes::Italic]==Attributes::True);
						font.setUnderline(attribs[Attributes::Underline]==Attributes::True);
						font_fmt.setFont(font);
						font_config[Attributes::Global]=font_fmt;
					}
					else if(elem==Attributes::Font)
					{
						font_config[attribs[Attributes::Id]]=font_fmt;
						itr=font_config.find(attribs[Attributes::Id]);
						font=font_fmt.font();
						font.setBold(attribs[Attributes::Bold]==Attributes::True);
						font.setItalic(attribs[Attributes::Italic]==Attributes::True);
						font.setUnderline(attribs[Attributes::Underline]==Attributes::True);
						(itr->second).setFont(font);
						(itr->second).setForeground(QColor(attribs[Attributes::Color]));
					}
					else if(elem==Attributes::Object)
					{
						list=attribs[Attributes::FillColor].split(',');

						vector<QColor> colors;
						colors.push_back(!list.isEmpty() ? QColor(list[0]) : QColor(0,0,0));
						colors.push_back(list.size()==2 ? QColor(list[1]) : colors[0]);
						colors.push_back(QColor(attribs[Attributes::BorderColor]));

						color_config[attribs[Attributes::Id]]=colors;
					}
				}
			}
			while(xmlparser.accessElement(XmlParser::NextElement));
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e, config_file);
	}
}

void BaseObjectView::setFontStyle(const QString &id, QTextCharFormat font_fmt)
{
	QFont font;

	if(id!=Attributes::Global)
	{
		font=font_config[Attributes::Global].font();
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
		return color_config[id][color_id];
	else
		return QColor(0,0,0);
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
	vector<QColor> colors;
	QLinearGradient grad(QPointF(0,0),QPointF(0,1));

	if(color_config.count(id) > 0)
	{
		colors=color_config[id];

		if(!colors.empty())
		{
			if(id==Attributes::ObjSelection || id==Attributes::Placeholder)
			{
				colors[0].setAlpha(ObjectAlphaChannel);
				colors[1].setAlpha(ObjectAlphaChannel);
			}

			grad.setCoordinateMode(QGradient::ObjectBoundingMode);
			grad.setColorAt(0, colors[0]);
			grad.setColorAt(1, colors[1]);
		}
	}

	return grad;
}

QPen BaseObjectView::getBorderStyle(const QString &id)
{
	QPen pen;
	vector<QColor> colors;

	if(color_config.count(id) > 0)
	{
		colors=color_config[id];

		if(!colors.empty())
		{
			if(id==Attributes::ObjSelection)
				colors[2].setAlpha(ObjectAlphaChannel);

			pen.setWidthF(ObjectBorderWidth);
			pen.setColor(colors[2]);
		}
	}

	return pen;
}

QTextCharFormat BaseObjectView::getFontStyle(const QString &id)
{
	if(font_config.count(id))
		return font_config[id];
	else
		return QTextCharFormat();
}

void BaseObjectView::setPlaceholderEnabled(bool value)
{
	use_placeholder=value;
}

bool BaseObjectView::isPlaceholderEnabled()
{
	return use_placeholder;
}

void BaseObjectView::setCompactViewEnabled(bool value)
{
	compact_view = value;
}

bool BaseObjectView::isCompactViewEnabled()
{
	return compact_view;
}

QVariant BaseObjectView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemPositionHasChanged)
	{
		BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(this->getUnderlyingObject());

		if(graph_obj && !graph_obj->isProtected())
		{
			if(ObjectsScene::isAlignObjectsToGrid())
				this->setPos(ObjectsScene::alignPointToGrid(this->scenePos()));

			graph_obj->setPosition(this->scenePos());
			this->configurePositionInfo(this->pos());
		}
	}
	else if(change == ItemSelectedHasChanged && obj_selection)
	{
		this->setSelectionOrder(value.toBool());
		pos_info_item->setVisible(value.toBool());
		obj_selection->setVisible(value.toBool());

		this->configurePositionInfo(this->pos());
		emit s_objectSelected(dynamic_cast<BaseGraphicObject *>(this->getUnderlyingObject()), value.toBool());
	}

	return value;
}

void BaseObjectView::setSelectionOrder(bool selected)
{
	if(this->sel_order==0 && selected)
		this->sel_order=++BaseObjectView::global_sel_order;
	else if(!selected)
		this->sel_order=0;
}

QRectF BaseObjectView::boundingRect() const
{
	return bounding_rect;
}

void BaseObjectView::toggleProtectionIcon(bool value)
{
	BaseGraphicObject *obj_graf=dynamic_cast<BaseGraphicObject *>(this->getUnderlyingObject());

	protected_icon->setVisible(value);
	this->setFlag(QGraphicsItem::ItemIsMovable, !value);

	if(obj_graf)
		obj_graf->setModified(true);
}

void BaseObjectView::configureObjectSelection()
{
	RoundedRectItem *rect_item=dynamic_cast<RoundedRectItem *>(obj_selection);

	if(rect_item)
	{
		rect_item->setRect(this->boundingRect());
		rect_item->setPos(0, 0);
		rect_item->setBorderRadius(5);
		rect_item->setBrush(this->getFillStyle(Attributes::ObjSelection));
		rect_item->setPen(this->getBorderStyle(Attributes::ObjSelection));
	}
}

void BaseObjectView::configurePositionInfo(QPointF pos)
{
	if(this->isSelected())
	{
		QFont fnt=font_config[Attributes::PositionInfo].font();

		pos_info_item->setBrush(BaseObjectView::getFillStyle(Attributes::PositionInfo));
		pos_info_item->setPen(BaseObjectView::getBorderStyle(Attributes::PositionInfo));

		fnt.setPointSizeF(fnt.pointSizeF() * 0.95);
		pos_info_item->setFont(fnt);
		pos_info_item->setTextBrush(font_config[Attributes::PositionInfo].foreground());

		pos_info_item->setText(QString(" x:%1 y:%2 ").arg(round(pos.x())).arg(round(pos.y())));
		pos_info_item->setPolygon(QPolygonF(pos_info_item->getTextBoundingRect()));

		pos_info_item->setPos(-0.5, -pos_info_item->boundingRect().height()/2);
	}
}

void BaseObjectView::configureSQLDisabledInfo()
{
	if(sql_disabled_item)
	{
		double px=0, py=0;

		sql_disabled_item->setVisible(this->getUnderlyingObject()->isSQLDisabled());

		if(this->getUnderlyingObject()->isSQLDisabled())
		{
			QTextCharFormat char_fmt;
			char_fmt=BaseObjectView::getFontStyle(Attributes::PositionInfo);
			char_fmt.setFontPointSize(char_fmt.font().pointSizeF() * 0.80);

			sql_disabled_item->setFont(char_fmt.font());
			sql_disabled_item->setText(tr("SQL off"));
			sql_disabled_item->setTextBrush(char_fmt.foreground());

			sql_disabled_item->setPolygon(QRectF(QPointF(0,0), sql_disabled_item->getTextBoundingRect().size() + QSizeF(1.5 * HorizSpacing, 1.5 * VertSpacing)));
			sql_disabled_item->setPen(BaseObjectView::getBorderStyle(Attributes::PositionInfo));
			sql_disabled_item->setBrush(BaseObjectView::getFillStyle(Attributes::PositionInfo));

			px=bounding_rect.width() - sql_disabled_item->boundingRect().width() + (1.5 * HorizSpacing);
			py=-(sql_disabled_item->boundingRect().height()/2);

			sql_disabled_item->setPos(px, py);
			sql_disabled_item->setTextPos(HorizSpacing * 0.75, VertSpacing * 0.75);
		}
	}
}

void BaseObjectView::configureProtectedIcon()
{
	if(protected_icon)
	{
		QGraphicsPolygonItem *pol_item=nullptr;
		QPolygonF pol;
		double factor;

		//Calculates the factor used to resize the protection icon accordding the font size
		factor=font_config[Attributes::Global].font().pointSizeF()/DefaultFontSize;

		pol.append(QPointF(2,5)); pol.append(QPointF(2,2));
		pol.append(QPointF(3,1)); pol.append(QPointF(4,0));
		pol.append(QPointF(7,0)); pol.append(QPointF(8,1));
		pol.append(QPointF(9,2)); pol.append(QPointF(9,5));
		pol.append(QPointF(7,5)); pol.append(QPointF(7,3));
		pol.append(QPointF(6,2)); pol.append(QPointF(5,2));
		pol.append(QPointF(4,3)); pol.append(QPointF(4,5));

		if(factor!=1.0)
			TextPolygonItem::resizePolygon(pol, pol.boundingRect().width() * factor,
																					pol.boundingRect().height() * factor);

		pol_item=dynamic_cast<QGraphicsPolygonItem *>(protected_icon->childItems().at(0));
		pol_item->setPolygon(pol);
		pol_item->setBrush(this->getFillStyle(Attributes::LockerArc));
		pol_item->setPen(this->getBorderStyle(Attributes::LockerArc));

		pol.clear();
		pol.append(QPointF(1,5));  pol.append(QPointF(10,5));
		pol.append(QPointF(11,6)); pol.append(QPointF(11,9));
		pol.append(QPointF(10,10)); pol.append(QPointF(1,10));
		pol.append(QPointF(0,9)); pol.append(QPointF(0,6));

		if(factor!=1.0)
			TextPolygonItem::resizePolygon(pol, pol.boundingRect().width() * factor,
																					pol.boundingRect().height() * factor);

		pol_item=dynamic_cast<QGraphicsPolygonItem *>(protected_icon->childItems().at(1));
		pol_item->setPolygon(pol);
		pol_item->setBrush(this->getFillStyle(Attributes::LockerBody));
		pol_item->setPen(this->getBorderStyle(Attributes::LockerBody));
	}
}

void BaseObjectView::configurePlaceholder()
{
	if(!placeholder)
	{
		placeholder=new RoundedRectItem();
		placeholder->setVisible(false);
		placeholder->setZValue(-1);
		placeholder->setFlag(QGraphicsItem::ItemIsMovable, false);
		placeholder->setFlag(QGraphicsItem::ItemIsSelectable, false);
	}
}

void BaseObjectView::__configureObject()
{
	BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(this->getUnderlyingObject());

	if(graph_obj)
	{
		this->setPos(graph_obj->getPosition());
		this->setToolTip(graph_obj->getName(true) +
						 QString(" (") + graph_obj->getTypeName() +
						 QString(") ") + QString("\nId: %1").arg(graph_obj->getObjectId()));
		this->configurePositionInfo(graph_obj->getPosition());
		this->configureProtectedIcon();
	}
}

unsigned BaseObjectView::getSelectionOrder()
{
	return sel_order;
}

QPointF BaseObjectView::getCenter()
{
	return QPointF(this->pos().x() + this->boundingRect().width()/2.0,
								 this->pos().y() + this->boundingRect().height()/2.0);
}

void BaseObjectView::togglePlaceholder(bool visible)
{
	if(use_placeholder && placeholder && this->scene())
	{
		if(!placeholder->scene())
			this->scene()->addItem(placeholder);

		if(visible)
		{
			QPen pen=BaseObjectView::getBorderStyle(Attributes::Placeholder);
			pen.setStyle(Qt::DashLine);

			placeholder->setZValue(this->zValue() - 1);
			placeholder->setBrush(BaseObjectView::getFillStyle(Attributes::Placeholder));
			placeholder->setPen(pen);
			placeholder->setRect(QRectF(QPointF(0,0),this->bounding_rect.size()));
			placeholder->setPos(this->mapToScene(this->bounding_rect.topLeft()));
		}

		placeholder->setVisible(visible);
	}
}

double BaseObjectView::getFontFactor()
{
	return font_config[Attributes::Global].font().pointSizeF()/DefaultFontSize;
}

void BaseObjectView::setLayer(unsigned layer)
{
	BaseGraphicObject *graph_obj = dynamic_cast<BaseGraphicObject *>(this->getUnderlyingObject());

	if(graph_obj)
		graph_obj->setLayer(layer);
}

unsigned BaseObjectView::getLayer()
{
	BaseGraphicObject *graph_obj = dynamic_cast<BaseGraphicObject *>(this->getUnderlyingObject());

	if(graph_obj)
		return graph_obj->getLayer();

	return 0;
}

double BaseObjectView::getScreenDpiFactor()
{
	QScreen *screen = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()));
	double factor = screen->logicalDotsPerInch() / 96.0;
	double pixel_ratio = screen->devicePixelRatio();

	if(factor < 1)
		return 1;

	return factor * pixel_ratio;
}
