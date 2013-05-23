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

#include "modeloverviewwidget.h"
#include "modelwidget.h"

ModelOverviewWidget::ModelOverviewWidget(QWidget *parent) : QWidget(parent, Qt::WindowCloseButtonHint)
{
	setupUi(this);
	this->model=nullptr;
	zoom_factor=1;
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void ModelOverviewWidget::show(ModelWidget *model)
{
	if(this->model)
	{
		disconnect(this->model, nullptr, this, nullptr);
		disconnect(this->model->viewport, nullptr,  this, nullptr);
		disconnect(this->model->scene, nullptr,  this, nullptr);
	}

	this->model=model;

	if(this->model)
	{
		connect(this->model, SIGNAL(s_objectCreated(void)), this, SLOT(updateOverview(void)));
		connect(this->model, SIGNAL(s_objectRemoved(void)), this, SLOT(updateOverview(void)));
		connect(this->model, SIGNAL(s_objectsMoved(void)), this, SLOT(updateOverview(void)));
		connect(this->model, SIGNAL(s_objectModified(void)), this, SLOT(updateOverview(void)));
		connect(this->model, SIGNAL(s_zoomModified(float)), this, SLOT(updateZoomFactor(float)));

		connect(this->model, SIGNAL(s_modelResized(void)), this, SLOT(resizeOverview(void)));
		connect(this->model, SIGNAL(s_modelResized(void)), this, SLOT(resizeWindowFrame(void)));
		connect(this->model, SIGNAL(s_modelResized(void)), this, SLOT(updateOverview(void)));

		connect(this->model->viewport->horizontalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(resizeWindowFrame(void)));
		connect(this->model->viewport->verticalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(resizeWindowFrame(void)));

		connect(this->model->scene, SIGNAL(selectionChanged(void)), this, SLOT(updateOverview(void)));
		connect(this->model->scene, SIGNAL(sceneRectChanged(QRectF)),this, SLOT(resizeOverview(void)));
		connect(this->model->scene, SIGNAL(sceneRectChanged(QRectF)),this, SLOT(updateOverview(void)));

		this->resizeOverview();
		this->resizeWindowFrame();
		this->updateOverview(true);

		this->move(this->model->geometry().right() - this->width(),
							 this->model->geometry().bottom() - this->height());
	}

	this->raise();
	QWidget::show();
}

void ModelOverviewWidget::closeEvent(QCloseEvent *event)
{
	emit s_overviewVisible(false);
	QWidget::closeEvent(event);
}

void ModelOverviewWidget::showEvent(QShowEvent *event)
{
	emit s_overviewVisible(true);
	QWidget::showEvent(event);
}

void ModelOverviewWidget::updateOverview(void)
{
	this->updateOverview(false);
}

void ModelOverviewWidget::updateOverview(bool force_update)
{
	if(this->model && (this->isVisible() || force_update))
	{
		QSize size;
		QPixmap pix;

		//Creates a pixmap with the size of the scene
		pix=QPixmap(this->model->scene->sceneRect().size().toSize());

		//Configures a QSize instance with 20% of the scene size
		size=this->model->scene->sceneRect().size().toSize();
		size.setWidth(size.width() * RESIZE_FACTOR);
		size.setHeight(size.height() * RESIZE_FACTOR);

		//Draw the scene onto the pixmap
		QPainter p(&pix);
		this->model->scene->render(&p, pix.rect(), this->model->scene->sceneRect().toRect());

		//Resizes the pixmap to the previous configured QSize
		label->setPixmap(pix.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		label->resize(size);
	}
}

void ModelOverviewWidget::resizeWindowFrame(void)
{
	if(this->model)
	{
		QSizeF size;
		float factor=RESIZE_FACTOR/zoom_factor;

		//Resizes the window frame based upon the model's viewport dimensions
		size=this->model->viewport->geometry().size();
		size.setWidth(size.width() * factor);
		size.setHeight(size.height() * factor);
		window_frm->resize(size.toSize());

		//Set the frame position based upon the viewport scroll bar values
		window_frm->move(QPoint(this->model->viewport->horizontalScrollBar()->value() * factor,
														this->model->viewport->verticalScrollBar()->value() * factor));

	}
}

void ModelOverviewWidget::resizeOverview(void)
{
	if(this->model)
	{
		QSizeF size;

		size=this->model->scene->sceneRect().size();
		size.setWidth(size.width() * RESIZE_FACTOR);
		size.setHeight(size.height() * RESIZE_FACTOR);
		this->resize(size.toSize());
		this->setMaximumSize(size.toSize());
		this->setMinimumSize(size.toSize());
	}
}

void ModelOverviewWidget::updateZoomFactor(float zoom)
{
	this->zoom_factor=zoom;
	this->resizeWindowFrame();
}

void ModelOverviewWidget::mouseDoubleClickEvent(QMouseEvent *)
{
	this->close();
}

void ModelOverviewWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons()==Qt::LeftButton)
	{
		QRect rect=window_frm->geometry(), rect1;
		int width, height, x=event->x(), y=event->y();

		width=rect.width()/2;
		height=rect.height()/2;

		//Configures a rectangle having as central point the event position
		rect.setLeft(x - width);
		rect.setTop(y - height);
		rect.setRight(x + width);
		rect.setBottom(y + height);

		rect1=frame->geometry();

		if(rect.left() < 0)
			rect.translate(abs(rect.left()),0);

		if(rect.top() < 0)
			rect.translate(0, abs(rect.top()));

		if(rect.right() >= rect1.right())
			rect.translate((rect1.right() - rect.right())-rect1.left(),0);

		if(rect.bottom() >= rect1.bottom())
			rect.translate(0,(rect1.bottom() - rect.bottom())-rect1.top());

		window_frm->setGeometry(rect);
		this->model->viewport->horizontalScrollBar()->setValue(ceilf(zoom_factor * this->model->scene->sceneRect().width() * (rect.x()/static_cast<float>(rect1.width()))));
		this->model->viewport->verticalScrollBar()->setValue(ceilf(zoom_factor * this->model->scene->sceneRect().height() * (rect.y()/static_cast<float>(rect1.height()))));
	}
}

void ModelOverviewWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->button()==Qt::LeftButton)
	{
		window_frm->setCursor(QCursor(Qt::OpenHandCursor));
		this->setCursor(QCursor(Qt::OpenHandCursor));
	}
}

void ModelOverviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button()==Qt::LeftButton)
	{
		window_frm->setCursor(QCursor(Qt::ArrowCursor));
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}

