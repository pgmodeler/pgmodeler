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

#include "basetableview.h"

bool BaseTableView::hide_ext_attribs=false;
bool BaseTableView::hide_tags=false;
bool BaseTableView::hide_tab_resize=false;

BaseTableView::BaseTableView(BaseTable *base_tab) : BaseObjectView(base_tab)
{
	if(!base_tab)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT, __PRETTY_FUNCTION__, __FILE__, __LINE__);

	body=new RoundedRectItem;
	body->setRoundedCorners(RoundedRectItem::BOTTOMLEFT_CORNER | RoundedRectItem::BOTTOMRIGHT_CORNER);

	title=new TableTitleView;

	ext_attribs_body=new RoundedRectItem;
	ext_attribs_body->setRoundedCorners(RoundedRectItem::NO_CORNERS);

	ext_attribs_toggler=new RoundedRectItem;
	ext_attribs_toggler->setRoundedCorners(RoundedRectItem::BOTTOMLEFT_CORNER | RoundedRectItem::BOTTOMRIGHT_CORNER);
	ext_attribs_toggler->setZValue(-1);

	ext_attribs=new QGraphicsItemGroup;
	ext_attribs->setZValue(1);

	ext_attribs_tog_arrow=new QGraphicsPolygonItem;
	ext_attribs_tog_arrow->setZValue(2);

	columns=new QGraphicsItemGroup;
	columns->setZValue(1);

	tab_resize_grip=new RoundedRectItem;
	tab_resize_grip->setZValue(1);

	top_LGrip=new QGraphicsLineItem;
	top_LGrip->setZValue(2);

	bottom_LGrip=new QGraphicsLineItem;
	bottom_LGrip->setZValue(2);

	tag_name=new QGraphicsSimpleTextItem;
	tag_name->setZValue(3);

	tag_body=new QGraphicsPolygonItem;
	tag_body->setZValue(2);

	obj_shadow=new RoundedRectItem;
	obj_shadow->setZValue(-1);

	obj_selection=new RoundedRectItem;
	obj_selection->setVisible(false);
	obj_selection->setZValue(4);

		body_height_being_resized=false;
		resized_body_idx=-1;

	this->addToGroup(obj_selection);
	this->addToGroup(obj_shadow);
	this->addToGroup(columns);
	this->addToGroup(body);
	this->addToGroup(title);
	this->addToGroup(tag_name);
	this->addToGroup(tag_body);
	this->addToGroup(ext_attribs);
	this->addToGroup(ext_attribs_body);
	this->addToGroup(ext_attribs_toggler);
	this->addToGroup(ext_attribs_tog_arrow);
	this->addToGroup(tab_resize_grip);
	this->addToGroup(top_LGrip);
	this->addToGroup(bottom_LGrip);

	this->setAcceptHoverEvents(true);
	sel_child_obj=nullptr;
	configurePlaceholder();
}

BaseTableView::~BaseTableView(void)
{
	this->removeFromGroup(body);
	this->removeFromGroup(title);
	this->removeFromGroup(ext_attribs_body);
	this->removeFromGroup(ext_attribs_toggler);
	this->removeFromGroup(ext_attribs_tog_arrow);
	this->removeFromGroup(ext_attribs);
	this->removeFromGroup(columns);
	this->removeFromGroup(tab_resize_grip);
	this->removeFromGroup(top_LGrip);
	this->removeFromGroup(bottom_LGrip);
	this->removeFromGroup(tag_name);
	this->removeFromGroup(tag_body);
	delete(ext_attribs_tog_arrow);
	delete(ext_attribs_toggler);
	delete(ext_attribs_body);
	delete(ext_attribs);
	delete(body);
	delete(title);
	delete(columns);
	delete(tab_resize_grip);
	delete(tag_name);
	delete(tag_body);
}

void BaseTableView::setHideExtAttributes(bool value)
{
	hide_ext_attribs=value;
}

void BaseTableView::setHideTags(bool value)
{
	hide_tags=value;
}

bool BaseTableView::isExtAttributesHidden(void)
{
	return(hide_ext_attribs);
}

bool BaseTableView::isTagsHidden(void)
{
	return(hide_tags);
}

QVariant BaseTableView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==ItemSelectedHasChanged)
	{
		this->setToolTip(this->table_tooltip);
		BaseObjectView::configureObjectSelection();
	}

	if(change==ItemPositionHasChanged)
		emit s_objectMoved();

	BaseObjectView::itemChange(change, value);

	return(value);
}

void BaseTableView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//Emit a signal containing the select child object if the user right-click the focused item
	if(!this->isSelected() && event->buttons()==Qt::RightButton && sel_child_obj)
	{
		if(this->scene())
			this->scene()->clearSelection();

		/* Deactivate the table in order not to hide the child object selection.
			 The table object is reativated when the context menu is hidden */
		this->setEnabled(false);

		emit s_childObjectSelected(sel_child_obj);
	}
	else
	{
				QPointF pnt_togg = this->ext_attribs_toggler->mapFromScene(event->scenePos());
				QPointF pnt_grip = this->tab_resize_grip->mapFromScene(event->scenePos());

		//If the user clicks the extended attributes toggler
		if(!this->isSelected() && event->buttons()==Qt::LeftButton &&
						 this->ext_attribs_toggler->boundingRect().contains(pnt_togg))
		{
			Schema *schema = dynamic_cast<Schema *>(this->getSourceObject()->getSchema());

			//We need to force the object to be not selectable so further calls to mousePressEvent doesn't select the object
			this->setFlag(QGraphicsItem::ItemIsSelectable, false);

			dynamic_cast<BaseTable *>(this->getSourceObject())
					->setExtAttribsHidden(!dynamic_cast<BaseTable *>(this->getSourceObject())->isExtAttribsHidden());

			//Updating the object geometry to show/hide the extended attributes
			this->configureObject();

			obj_selection->setVisible(false);

			// Using a single shot time to restore the selectable flag
			QTimer::singleShot(300, [&]{ this->setFlag(QGraphicsItem::ItemIsSelectable, true); });

			//Updating the schema box that holds the object (if visible)
			schema->setModified(true);

			emit s_extAttributesToggled();
		}

		else if(!this->isSelected() && event->buttons()==Qt::LeftButton &&
						this->tab_resize_grip->boundingRect().contains(pnt_grip))
		{
			 body_height_being_resized=true;
			 return;
		}

		BaseObjectView::mousePressEvent(event);
	}
}

void BaseTableView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
		QPointF pnt_release = this->body->mapFromScene(event->scenePos());
		if(/*event->buttons()==Qt::LeftButton && */body_height_being_resized)
		{
				if(pnt_release.y()<=0)
						resized_body_idx=0;
				else if(pnt_release.y() > static_cast<double>(max_body_idx*col_height))
						resized_body_idx=max_body_idx;
				else
						resized_body_idx=pnt_release.y()/col_height;

				this->setFlag(QGraphicsItem::ItemIsMovable, true);
				this->configureObject();
				body_height_being_resized=false;

		}
}

void BaseTableView::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	if(!this->isSelected() && obj_selection->isVisible())
		obj_selection->setVisible(false);

	sel_child_obj=nullptr;
}

void BaseTableView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
		QList<QGraphicsItem *> items;
		double cols_height, item_idx;
		QRectF rect, rect1;
		QPointF pnt_columns = this->body->mapFromScene(event->scenePos());
		QPointF pnt_grip = this->tab_resize_grip->mapFromScene(event->scenePos());
		QPointF pnt_ext_attribs = this->ext_attribs_body->mapFromScene(event->scenePos());
		QPointF pnt_ext_toggler = this->ext_attribs_toggler->mapFromScene(event->scenePos());

		if(this->body->boundingRect().contains(pnt_columns))
		{
				items.append(columns->childItems());
				if(!items.isEmpty())
				{
						//Calculates the default item height
						cols_height=roundf(body->boundingRect().height() / static_cast<double>(items.size()));

						//Calculates the item index based upon the mouse position
						rect=this->mapRectToItem(title, title->boundingRect());
						item_idx=(event->pos().y() - rect.bottom()) / cols_height;

						if(item_idx>0 && item_idx<=items.size())
						{
								//QPolygonF pol;
								BaseObjectView *item=dynamic_cast<TableObjectView *>(items[item_idx]);

								//Configures the selection with the item's dimension
								if(obj_selection->boundingRect().height()!=item->boundingRect().height())
								{
										dynamic_cast<RoundedRectItem *>(obj_selection)->setBorderRadius(2);
										dynamic_cast<RoundedRectItem *>(obj_selection)->setRect(QRectF(0,0,
																																									 title->boundingRect().width() - (2.5 * HORIZ_SPACING),
																																									 item->boundingRect().height()));
								}

								//Sets the selection position as same as item's position
								rect1=this->mapRectToItem(item, item->boundingRect());
								obj_selection->setVisible(true);
								obj_selection->setPos(QPointF(title->pos().x() + HORIZ_SPACING,-rect1.top()));

								//Stores the selected child object
								sel_child_obj=dynamic_cast<TableObject *>(item->getSourceObject());
								this->setToolTip(item->toolTip());
						}
				}
		}

		else if(tab_resize_grip->isVisible() && this->tab_resize_grip->boundingRect().contains(pnt_grip))
		{
				dynamic_cast<RoundedRectItem *>(obj_selection)->setBorderRadius(2);
				dynamic_cast<RoundedRectItem *>(obj_selection)->setRect(QRectF(0,0,
																																				title->boundingRect().width() - (2.5 * HORIZ_SPACING),
																																				tab_resize_grip->boundingRect().height() * 0.65f));

				//Sets the selection position as same as item's position
				rect1=this->mapRectToItem(tab_resize_grip, tab_resize_grip->boundingRect());
				obj_selection->setVisible(true);
				obj_selection->setPos(QPointF(title->pos().x() + HORIZ_SPACING, -rect1.top() + 1.5f));
				this->setToolTip(trUtf8("Adjust manually the height of the column section"));
		}

		else if(ext_attribs->isVisible() && this->ext_attribs_body->boundingRect().contains(pnt_ext_attribs))
		{
				items.append(ext_attribs->childItems());
				if(!items.isEmpty())
				{
						//Calculates the default item height
						cols_height=roundf(ext_attribs_body->boundingRect().height() / static_cast<double>(items.size()));

						//Calculates the item index based upon the mouse position
						/*if(!hide_tab_resize)
								rect=this->mapRectToItem(tab_resize_grip, tab_resize_grip->boundingRect());
						else
								rect=this->mapRectToItem(body, body->boundingRect());*/
						item_idx=pnt_ext_attribs.y() / cols_height;

						if(item_idx>0 && item_idx<=items.size())
						{
								//QPolygonF pol;
								BaseObjectView *item=dynamic_cast<TableObjectView *>(items[item_idx]);

								//Configures the selection with the item's dimension
								if(obj_selection->boundingRect().height()!=item->boundingRect().height())
								{
										dynamic_cast<RoundedRectItem *>(obj_selection)->setBorderRadius(2);
										dynamic_cast<RoundedRectItem *>(obj_selection)->setRect(QRectF(0,0,
																																									 title->boundingRect().width() - (2.5 * HORIZ_SPACING),
																																									 item->boundingRect().height()));
								}

								//Sets the selection position as same as item's position
								rect1=item->mapRectToItem(this, item->boundingRect());
								obj_selection->setVisible(true);
								obj_selection->setPos(QPointF(title->pos().x() + HORIZ_SPACING,rect1.top()));

								//Stores the selected child object
								sel_child_obj=dynamic_cast<TableObject *>(item->getSourceObject());
								this->setToolTip(item->toolTip());
						}
				}
		}

		else if(ext_attribs_toggler->isVisible() && this->ext_attribs_toggler->boundingRect().contains(pnt_ext_toggler))
		{
				dynamic_cast<RoundedRectItem *>(obj_selection)->setBorderRadius(2);
				dynamic_cast<RoundedRectItem *>(obj_selection)->setRect(QRectF(0,0,
																																				title->boundingRect().width() - (2.5 * HORIZ_SPACING),
																																				ext_attribs_toggler->boundingRect().height() * 0.65f));

				//Sets the selection position as same as item's position
				rect1=this->mapRectToItem(ext_attribs_toggler, ext_attribs_toggler->boundingRect());
				obj_selection->setVisible(true);
				obj_selection->setPos(QPointF(title->pos().x() + HORIZ_SPACING, -rect1.top() + 1.5f));
				this->setToolTip(trUtf8("Toggles the extended attributes display"));
		}
		else
		{
				this->hoverLeaveEvent(event);
				this->setToolTip(this->table_tooltip);
		}
}

void BaseTableView::addConnectedRelationship(BaseRelationship *base_rel)
{
	BaseTable *tab = dynamic_cast<BaseTable *>(getSourceObject());

	if(!base_rel ||
		 (base_rel &&
			base_rel->getTable(BaseRelationship::SRC_TABLE) != tab &&
			base_rel->getTable(BaseRelationship::DST_TABLE) != tab))
		return;

	connected_rels.push_back(base_rel);
}

void BaseTableView::removeConnectedRelationship(BaseRelationship *base_rel)
{
	connected_rels.erase(std::find(connected_rels.begin(), connected_rels.end(), base_rel));
}

int BaseTableView::getConnectedRelationshipIndex(BaseRelationship *base_rel)
{
	vector<BaseRelationship *>::iterator itr = std::find(connected_rels.begin(), connected_rels.end(), base_rel);

	if(itr != connected_rels.end())
		return(itr - connected_rels.begin());

	return(-1);
}

unsigned BaseTableView::getConnectedRelsCount(BaseTable *src_tab, BaseTable *dst_tab)
{
	unsigned count = 0;

	for(auto &rel : connected_rels)
	{
		if((rel->getTable(BaseRelationship::SRC_TABLE) == src_tab &&
				rel->getTable(BaseRelationship::DST_TABLE) == dst_tab) ||
			 (rel->getTable(BaseRelationship::SRC_TABLE) == dst_tab &&
				rel->getTable(BaseRelationship::DST_TABLE) == src_tab))
			count++;
	}

	return(count);
}

void BaseTableView::configureTag(void)
{
	BaseTable *tab=dynamic_cast<BaseTable *>(this->getSourceObject());
	Tag *tag=tab->getTag();

	tag_body->setVisible(tag!=nullptr && !hide_tags);
	tag_name->setVisible(tag!=nullptr && !hide_tags);

	if(!hide_tags && tag)
	{
		QPolygonF pol;
		QPointF p1, p2;
		double bottom;
		QFont fnt=BaseObjectView::getFontStyle(ParsersAttributes::TAG).font();

		fnt.setPointSizeF(fnt.pointSizeF() * 0.80f);
		tag_name->setFont(fnt);
		tag_name->setText(tag->getName());
		tag_name->setBrush(BaseObjectView::getFontStyle(ParsersAttributes::TAG).foreground());

		p1=tag_name->boundingRect().topLeft();
		p2=tag_name->boundingRect().bottomRight();
		bottom=this->boundingRect().bottom();

		pol.append(QPointF(p1.x()-BaseObjectView::HORIZ_SPACING, p1.y() - BaseObjectView::VERT_SPACING));
		pol.append(QPointF(p2.x(), p1.y() - BaseObjectView::VERT_SPACING));
		pol.append(QPointF(p2.x() + BaseObjectView::HORIZ_SPACING + 5, p2.y()/2));
		pol.append(QPointF(p2.x(), p2.y() + BaseObjectView::VERT_SPACING));
		pol.append(QPointF(p1.x(), p2.y() + BaseObjectView::VERT_SPACING));
		pol.append(QPointF(p1.x()-BaseObjectView::HORIZ_SPACING, p2.y() + BaseObjectView::VERT_SPACING));

		tag_body->setPolygon(pol);
		tag_body->setPen(BaseObjectView::getBorderStyle(ParsersAttributes::TAG));
		tag_body->setBrush(BaseObjectView::getFillStyle(ParsersAttributes::TAG));

		tag_name->setPos(-5, bottom - 1.5f);
		tag_body->setPos(-5, bottom - 1.5f);
	}
}

void BaseTableView::__configureObject(float width)
{
	BaseTable *tab = dynamic_cast<BaseTable *>(this->getSourceObject());

	//Configure extended attributes toggler objects : a rectangle and an arrow.
	if(!ext_attribs->childItems().isEmpty() && !hide_ext_attribs)
	{
		QPen pen = ext_attribs_body->pen();
		float py = 0;
		float factor = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->logicalDotsPerInch() / 96.0f;
		float pixel_ratio = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->devicePixelRatio();

		ext_attribs_toggler->setVisible(true);
		ext_attribs_tog_arrow->setVisible(true);

		ext_attribs_toggler->setPen(pen);
		ext_attribs_toggler->setBrush(ext_attribs_body->brush());
		ext_attribs_toggler->setRect(QRectF(0, 0, width, 12 * factor * pixel_ratio));

		if(!tab->isExtAttribsHidden())
		{
			py = title->boundingRect().height() +
					 body->boundingRect().height() +
					 (hide_tab_resize ? 0 : this->DEFAULT_FONT_SIZE * factor * pixel_ratio) +
					 ext_attribs_body->boundingRect().height() - VERT_SPACING - 1;
		}
		else
		{
			py = title->boundingRect().height() +
					 body->boundingRect().height() - 2 +
					 (hide_tab_resize ? 0 : this->DEFAULT_FONT_SIZE * factor * pixel_ratio);
		}

		ext_attribs_toggler->setPos(ext_attribs_body->pos().x(), py);

		QPolygonF pol;

		if(!tab->isExtAttribsHidden())
		{
			pol.append(QPointF(0,0));
			pol.append(QPointF(-5 * factor * pixel_ratio, 6 * factor * pixel_ratio));
			pol.append(QPointF(5 * factor * pixel_ratio, 6 * factor * pixel_ratio));
		}
		else
		{
			pol.append(QPointF(0,6 * factor * pixel_ratio));
			pol.append(QPointF(-5 * factor * pixel_ratio, 0));
			pol.append(QPointF(5 * factor * pixel_ratio, 0));
		}

		QLinearGradient grad(QPointF(0,0),QPointF(0,1));
		grad.setCoordinateMode(QGradient::ObjectBoundingMode);
		grad.setColorAt(0, ext_attribs_body->pen().color().lighter(200));
		grad.setColorAt(1, ext_attribs_body->pen().color().lighter());

		pen.setStyle(Qt::SolidLine);
		ext_attribs_tog_arrow->setPen(pen);
		ext_attribs_tog_arrow->setBrush(grad);
		ext_attribs_tog_arrow->setPolygon(pol);

		ext_attribs_tog_arrow->setPos(ext_attribs_body->boundingRect().width() * 0.51f,
																	ext_attribs_toggler->pos().y() + pol.boundingRect().height() * 0.525f);
	}
	else
	{
		ext_attribs_tog_arrow->setVisible(false);
		ext_attribs_toggler->setVisible(false);
	}

	//Configure the table's height-resize grip
	if(!hide_tab_resize)
	{
		float py2 = 0;
		float factor2 = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->logicalDotsPerInch() / 96.0f;
		float pixel_ratio2 = qApp->screens().at(qApp->desktop()->screenNumber(qApp->activeWindow()))->devicePixelRatio();
		QPen pen = body->pen();

		//An enclosing rectangle and two horizontal parallel lines will make the grip.
		tab_resize_grip->setVisible(true);
				tab_resize_grip->setRect(QRectF(0, 0, width, DEFAULT_FONT_SIZE * 1.25 * factor2 * pixel_ratio2));

				tab_resize_grip->setRoundedCorners((ext_attribs->isVisible() || ext_attribs_toggler->isVisible()) ?
						RoundedRectItem::NO_CORNERS : RoundedRectItem::BOTTOMLEFT_CORNER | RoundedRectItem::BOTTOMRIGHT_CORNER );
		tab_resize_grip->setPen(pen);
		tab_resize_grip->setBrush(body->brush());

		//Position the grip
		py2 = title->boundingRect().height() + body->boundingRect().height() - VERT_SPACING;
		tab_resize_grip->setPos(body->pos().x(), py2);

		//Make the lines
		top_LGrip->setVisible(true);
		top_LGrip->setPen(pen);
		top_LGrip->setLine(QLineF(
			body->pos().x() + tab_resize_grip->boundingRect().width()*2/5,
			py2 + tab_resize_grip->boundingRect().height()*2/5,
			body->pos().x() + tab_resize_grip->boundingRect().width()*3/5,
			py2 + tab_resize_grip->boundingRect().height()*2/5));

		bottom_LGrip->setVisible(true);
		bottom_LGrip->setPen(pen);
		bottom_LGrip->setLine(QLineF(
			body->pos().x() + tab_resize_grip->boundingRect().width()*2/5,
			py2 + tab_resize_grip->boundingRect().height()*3/5,
			body->pos().x() + tab_resize_grip->boundingRect().width()*3/5,
			py2 + tab_resize_grip->boundingRect().height()*3/5));

	}
	else
	{
		tab_resize_grip->setVisible(false);
	}

	//Set the protected icon position to the top-right on the title
	protected_icon->setPos(title->pos().x() + title->boundingRect().width() * 0.90f, 2 * VERT_SPACING);

	this->bounding_rect.setTopLeft(title->boundingRect().topLeft());
	this->bounding_rect.setWidth(title->boundingRect().width());

	if(!ext_attribs->isVisible() && hide_tab_resize)
	{
		this->bounding_rect.setHeight(title->boundingRect().height() +
																	body->boundingRect().height() - 1);
		body->setRoundedCorners(RoundedRectItem::BOTTOMLEFT_CORNER | RoundedRectItem::BOTTOMRIGHT_CORNER);
	}
	else if(ext_attribs->isVisible() && hide_tab_resize)
	{
		this->bounding_rect.setHeight(title->boundingRect().height() +
										body->boundingRect().height() +
										(!tab->isExtAttribsHidden() ? ext_attribs_body->boundingRect().height() : 0) +
										ext_attribs_toggler->boundingRect().height() - VERT_SPACING - 1);
		body->setRoundedCorners(RoundedRectItem::NO_CORNERS);
	}
	else if(!ext_attribs->isVisible() && !hide_tab_resize)
	{
		this->bounding_rect.setHeight(title->boundingRect().height() +
										body->boundingRect().height() +
										tab_resize_grip->boundingRect().height());

		body->setRoundedCorners(RoundedRectItem::NO_CORNERS);
	}
	else if(ext_attribs->isVisible() && !hide_tab_resize)
		{
			this->bounding_rect.setHeight(title->boundingRect().height() +
											body->boundingRect().height() +
											(!tab->isExtAttribsHidden() ? ext_attribs_body->boundingRect().height() : 0) +
											ext_attribs_toggler->boundingRect().height() - VERT_SPACING - 1 +
											tab_resize_grip->boundingRect().height());

			body->setRoundedCorners(RoundedRectItem::NO_CORNERS);
		}

	this->table_tooltip=this->getSourceObject()->getName(true) +
						QString(" (") + this->getSourceObject()->getTypeName() + QString(") \n") +
						QString("Id: %1\n").arg(this->getSourceObject()->getObjectId()) +
						TableObjectView::CONSTR_DELIM_START +
						trUtf8("Connected rels: %1").arg(this->getConnectRelsCount()) +
						TableObjectView::CONSTR_DELIM_END;

	this->setToolTip(this->table_tooltip);
}

float BaseTableView::calculateWidth(void)
{
	/* Calculating the maximum width between the title, columns and extended attributes.
		This width is used to set the uniform width of table */
	if(!columns->childItems().isEmpty() &&
			(columns->boundingRect().width() > title->boundingRect().width() &&
			 (hide_ext_attribs || dynamic_cast<BaseTable *>(this->getSourceObject())->isExtAttribsHidden() ||
				(columns->boundingRect().width() > ext_attribs->boundingRect().width()))))
		return(columns->boundingRect().width() + (2 * HORIZ_SPACING));

	if(!ext_attribs->childItems().isEmpty() && !hide_ext_attribs &&
		 !dynamic_cast<BaseTable *>(this->getSourceObject())->isExtAttribsHidden() &&
			(ext_attribs->boundingRect().width() > title->boundingRect().width() &&
			 ext_attribs->boundingRect().width() > columns->boundingRect().width()))
		return(ext_attribs->boundingRect().width() + (2 * HORIZ_SPACING));

	return(title->boundingRect().width() + (2 * HORIZ_SPACING));
}

int BaseTableView::getConnectRelsCount(void)
{
	return(connected_rels.size());
}

void BaseTableView::requestRelationshipsUpdate(void)
{
	emit s_relUpdateRequest();
}

void BaseTableView::togglePlaceholder(bool value)
{
	BaseObjectView::togglePlaceholder(!connected_rels.empty() && value);
}
