/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "sceneinfowidget.h"
#include "pgmodeleruins.h"
#include "relationshipview.h"

SceneInfoWidget::SceneInfoWidget(QWidget *parent): QWidget(parent)
{
	QFont font;

	setupUi(this);

	font = obj_name_lbl->font();
	font.setPointSizeF(font.pointSizeF() * 0.90);

	obj_name_lbl->setFont(font);
	obj_pos_info_lbl->setFont(font);
	mouse_pos_info_lbl->setFont(font);
}

void SceneInfoWidget::updateSelectedObject(BaseObjectView *object)
{
	if(!object)
	{
		obj_icon_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("fechar1")));
		obj_name_lbl->setText(trUtf8("No selection"));
		obj_pos_info_lbl->setText(trUtf8("N/A"));
	}
	else
	{
		RelationshipView *rel_view = dynamic_cast<RelationshipView *>(object);
		QRectF rect = rel_view ? rel_view->__boundingRect() :
														 QRectF(object->pos().x(),
																		object->pos().y(),
																		object->boundingRect().width(),
																		object->boundingRect().height());

		obj_icon_lbl->setPixmap(PgModelerUiNs::getIconPath(object->getSourceObject()->getSchemaName()));
		obj_name_lbl->setText(QString("<strong>%1</strong> <em>(%2)</em>").arg(object->getSourceObject()->getSignature()).arg(object->getSourceObject()->getTypeName()));
		obj_pos_info_lbl->setText(QString("(%1, %2) [w: %3, h: %4]")
															.arg(roundf(rect.left()))
															.arg(roundf(rect.top()))
															.arg(roundf(rect.width()))
															.arg(roundf(rect.height())));
	}
}

void SceneInfoWidget::updateSelectedObjects(int obj_count, const QRectF &objs_rect)
{
	QRect rect = objs_rect.toRect();

	obj_icon_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("seltodos")));
	obj_name_lbl->setText(trUtf8("Sel. objects: %1").arg(obj_count));
	obj_pos_info_lbl->setText(QString("(%1, %2) [w: %3, h: %4]")
														.arg(rect.left())
														.arg(rect.top())
														.arg(rect.size().width())
														.arg(rect.size().height()));
}

void SceneInfoWidget::updateMousePosition(const QPointF &mouse_pos)
{
	mouse_pos_info_lbl->setText(QString("%1, %2").arg(roundf(mouse_pos.x())).arg(roundf(mouse_pos.y())));
}

void SceneInfoWidget::updateSceneZoom(double zoom)
{
	zoom_info_lbl->setText(QString("%1%").arg(zoom * 100));
}
