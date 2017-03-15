/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2016 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
	curr_resize_factor=RESIZE_FACTOR;
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
		connect(this->model, SIGNAL(s_zoomModified(double)), this, SLOT(updateZoomFactor(double)));

		connect(this->model, SIGNAL(s_modelResized(void)), this, SLOT(resizeOverview(void)));
		connect(this->model, SIGNAL(s_modelResized(void)), this, SLOT(resizeWindowFrame(void)));
		connect(this->model, SIGNAL(s_modelResized(void)), this, SLOT(updateOverview(void)));

		connect(this->model->viewport->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(resizeWindowFrame(void)));
		connect(this->model->viewport->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(resizeWindowFrame(void)));

		connect(this->model->scene, SIGNAL(selectionChanged(void)), this, SLOT(updateOverview(void)));
		connect(this->model->scene, SIGNAL(sceneRectChanged(QRectF)),this, SLOT(resizeOverview(void)));
		connect(this->model->scene, SIGNAL(sceneRectChanged(QRectF)),this, SLOT(updateOverview(void)));

		this->resizeOverview();
		this->updateZoomFactor(this->model->getCurrentZoom());
		this->updateOverview(true);

		this->move(this->model->geometry().right() - this->width(),
				   this->model->geometry().bottom() - this->height());

		frame->installEventFilter(this);
	}

	this->raise();
	QWidget::show();
}

void ModelOverviewWidget::closeEvent(QCloseEvent *event)
{
	model=nullptr;
	emit s_overviewVisible(false);
	QWidget::closeEvent(event);
}

void ModelOverviewWidget::showEvent(QShowEvent *event)
{
	emit s_overviewVisible(true);
	QWidget::showEvent(event);
}

bool ModelOverviewWidget::eventFilter(QObject *object, QEvent *event)
{
	if(object == frame && event->type()==QEvent::Wheel)
	{
		QWheelEvent *w_event = static_cast<QWheelEvent *>(event);
		QPoint dt_angle = w_event->angleDelta();

		if(dt_angle.y() < 0)
			model->applyZoom(model->getCurrentZoom() - ModelWidget::ZOOM_INCREMENT);
		else
			model->applyZoom(model->getCurrentZoom() + ModelWidget::ZOOM_INCREMENT);

		return(false);
	}

	return(QWidget::eventFilter(object, event));
}

void ModelOverviewWidget::updateOverview(void)
{
	this->updateOverview(false);
}

void ModelOverviewWidget::updateOverview(bool force_update)
{
	if(this->model && (this->isVisible() || force_update))
	{
		QPixmap pix;
		QSize size=scene_rect.size().toSize();

		//Creates a pixmap with the size of the scene
		pix=QPixmap(size);

		//Draw the scene onto the pixmap
		QPainter p(&pix);
		this->model->scene->render(&p, pix.rect(), scene_rect.toRect());

		//Resizes the pixmap to the previous configured QSize
		label->setPixmap(pix.scaled(curr_size.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		label->resize(curr_size.toSize());
	}
}

void ModelOverviewWidget::resizeWindowFrame(void)
{
	if(this->model)
	{
		QSizeF size;
		double factor=curr_resize_factor/zoom_factor;
		QScrollBar *h_scroll=this->model->viewport->horizontalScrollBar(),
				*v_scroll=this->model->viewport->verticalScrollBar();

		//Resizes the window frame based upon the model's viewport dimensions
		size=this->model->viewport->geometry().size();
		size.setWidth((size.width() * factor));
		size.setHeight((size.height() * factor));
		window_frm->resize(size.toSize());

		//Set the frame position based upon the viewport scroll bar values
		window_frm->move(QPoint(h_scroll->value() * factor,	v_scroll->value() * factor));

	}
}

void ModelOverviewWidget::resizeOverview(void)
{
	if(this->model)
	{
		QDesktopWidget desktop;
		QRect screen_rect=desktop.screenGeometry(desktop.primaryScreen());

		//Make an initial calculation of the overview window size
		scene_rect=this->model->scene->sceneRect();
		curr_size=scene_rect.size();
		curr_size.setWidth(curr_size.width() * RESIZE_FACTOR);
		curr_size.setHeight(curr_size.height() * RESIZE_FACTOR);

		//If the size exceeds the screen half width or height
		if(curr_size.width() > screen_rect.width()/2 ||
				curr_size.height() > screen_rect.height()/2)
		{
			//Reduce the resize factor and recalculates the new size
			curr_resize_factor=RESIZE_FACTOR/2;
			curr_size=scene_rect.size();
			curr_size.setWidth(curr_size.width() * curr_resize_factor);
			curr_size.setHeight(curr_size.height() * curr_resize_factor);
		}
		else
			curr_resize_factor=RESIZE_FACTOR;

		this->resize(curr_size.toSize());
		this->setMaximumSize(curr_size.toSize());
		this->setMinimumSize(curr_size.toSize());
	}
}

void ModelOverviewWidget::updateZoomFactor(double zoom)
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
		this->model->viewport->horizontalScrollBar()->setValue(ceilf(zoom_factor * scene_rect.width() * (rect.x()/static_cast<double>(rect1.width()))));
		this->model->viewport->verticalScrollBar()->setValue(ceilf(zoom_factor * scene_rect.height() * (rect.y()/static_cast<double>(rect1.height()))));
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

