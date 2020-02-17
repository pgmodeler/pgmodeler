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

#include "modeloverviewwidget.h"
#include "modelwidget.h"

ModelOverviewWidget::ModelOverviewWidget(QWidget *parent) : QWidget(parent, Qt::WindowCloseButtonHint | Qt::Tool)
{
	setupUi(this);

	scrollarea = nullptr;
	this->model=nullptr;
	zoom_factor=1;
	curr_resize_factor=ResizeFactor;
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QVBoxLayout *vbox = new QVBoxLayout;
	scrollarea=new QScrollArea(frame);
	scrollarea->setWidgetResizable(true);
	scrollarea->setFrameStyle(QFrame::Box);
	scrollarea->setFrameShadow(QFrame::Plain);
	scrollarea->setVisible(false);
	vbox->addWidget(scrollarea);
	vbox->setContentsMargins(0,0,0,0);
	frame->setLayout(vbox);
	label->setStyleSheet("QLabel#label{ border: 0px; }");
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
		connect(this->model, SIGNAL(s_objectCreated()), this, SLOT(updateOverview()));
		connect(this->model, SIGNAL(s_objectRemoved()), this, SLOT(updateOverview()));
		connect(this->model, SIGNAL(s_objectsMoved()), this, SLOT(updateOverview()));
		connect(this->model, SIGNAL(s_objectModified()), this, SLOT(updateOverview()));
		connect(this->model, SIGNAL(s_zoomModified(double)), this, SLOT(updateZoomFactor(double)));

		connect(this->model, SIGNAL(s_modelResized()), this, SLOT(resizeOverview()));
		connect(this->model, SIGNAL(s_modelResized()), this, SLOT(resizeWindowFrame()));
		connect(this->model, SIGNAL(s_modelResized()), this, SLOT(updateOverview()));

		connect(this->model->viewport->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(resizeWindowFrame()));
		connect(this->model->viewport->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(resizeWindowFrame()));

		connect(this->model->scene, SIGNAL(selectionChanged()), this, SLOT(updateOverview()));
		connect(this->model->scene, SIGNAL(sceneRectChanged(QRectF)),this, SLOT(resizeOverview()));
		connect(this->model->scene, SIGNAL(sceneRectChanged(QRectF)),this, SLOT(updateOverview()));

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
			model->applyZoom(model->getCurrentZoom() - ModelWidget::ZoomIncrement);
		else
			model->applyZoom(model->getCurrentZoom() + ModelWidget::ZoomIncrement);

		return false;
	}

	return QWidget::eventFilter(object, event);
}

void ModelOverviewWidget::updateOverview()
{
	this->updateOverview(false);
}

void ModelOverviewWidget::updateOverview(bool force_update)
{
	if(this->model && (this->isVisible() || force_update))
	{
		QPixmap pix;

		QApplication::setOverrideCursor(Qt::WaitCursor);

		//Creates a pixmap with the size of the scene
		pix=QPixmap(pixmap_size);

		//Draw the scene onto the pixmap
		QPainter p(&pix);

		if(!p.isActive())
		{
			label->setPixmap(QPixmap());
			label->setText(tr("Failed to generate the overview image.\nThe requested size %1 x %2 was too big and there was not enough memory to allocate!")
										 .arg(pixmap_size.width()).arg(pixmap_size.height()));
			frame->setEnabled(false);
		}
		else
		{
			frame->setEnabled(true);

			p.setRenderHints(QPainter::Antialiasing, false);
			p.setRenderHints(QPainter::TextAntialiasing, false);
			this->model->scene->render(&p, pix.rect(), scene_rect.toRect());

			//Resizes the pixmap to the previous configured QSize
			label->setPixmap(pix.scaled(curr_size.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}

		label->resize(curr_size.toSize());
		QApplication::restoreOverrideCursor();
	}
}

void ModelOverviewWidget::resizeWindowFrame()
{
	if(this->model)
	{
		QSizeF size;
		double factor=curr_resize_factor/zoom_factor;
		QScrollBar *h_scroll=this->model->viewport->horizontalScrollBar(),
				*v_scroll=this->model->viewport->verticalScrollBar();

		//Resizes the window frame based upon the model's viewport dimensions
		size=this->model->viewport->geometry().size();
		size.setWidth(size.width() * factor);
		size.setHeight(size.height() * factor);
		window_frm->resize(size.toSize());

		//Set the frame position based upon the viewport scroll bar values
		window_frm->move(QPoint(h_scroll->value() * factor - (scrollarea->isVisible() ? scrollarea->horizontalScrollBar()->value() : 0),
								v_scroll->value() * factor - (scrollarea->isVisible() ? scrollarea->verticalScrollBar()->value() : 0)));

		if(scrollarea->isVisible())
		{
		  if(window_frm->geometry().bottom() > frame->geometry().bottom())
			 scrollarea->verticalScrollBar()->setValue(scrollarea->verticalScrollBar()->value() + (scrollarea->verticalScrollBar()->maximum() * 0.30));
		  else if(window_frm->geometry().top() < 0)
			scrollarea->verticalScrollBar()->setValue(scrollarea->verticalScrollBar()->value() - (scrollarea->verticalScrollBar()->maximum() * 0.30));

		  if(window_frm->geometry().right() > frame->geometry().right())
			 scrollarea->horizontalScrollBar()->setValue(scrollarea->horizontalScrollBar()->value() + (scrollarea->horizontalScrollBar()->maximum() * 0.30));
		  else if(window_frm->geometry().left() < 0)
			scrollarea->horizontalScrollBar()->setValue(scrollarea->horizontalScrollBar()->value() - (scrollarea->horizontalScrollBar()->maximum() * 0.30));
		}
	}
}

void ModelOverviewWidget::resizeOverview()
{
	if(this->model)
	{
		QDesktopWidget desktop;
		QRect screen_rect=qApp->primaryScreen()->geometry();

		//Make an initial calculation of the overview window size
		scene_rect=this->model->scene->sceneRect();
		curr_size=scene_rect.size();
		curr_size.setWidth(curr_size.width() * ResizeFactor);
		curr_size.setHeight(curr_size.height() * ResizeFactor);

		//If the size exceeds the screen's width or height in 90%
		if(curr_size.width() > screen_rect.width() * 0.90 ||
			 curr_size.height() > screen_rect.height() * 0.90)
		{
			int max_val = std::max(scene_rect.width(), scene_rect.height());

			//Reduce the resize factor and recalculates the new size
			if(max_val >= 16384)
			{
				curr_resize_factor=screen_rect.width()/static_cast<double>(max_val);
				pixmap_size.setWidth(scene_rect.size().width() * (curr_resize_factor * 10));
				pixmap_size.setHeight(scene_rect.size().height() * (curr_resize_factor * 10));
			}
			else
			{
				curr_resize_factor=ResizeFactor/2;
				pixmap_size=scene_rect.size().toSize();
			}

			curr_size=scene_rect.size();
			curr_size.setWidth(curr_size.width() * curr_resize_factor);
			curr_size.setHeight(curr_size.height() * curr_resize_factor);
		}
		else
		{
			curr_resize_factor=ResizeFactor;
			pixmap_size=scene_rect.size().toSize();
		}

		QSize size = curr_size.toSize();
		bool show_scrollarea = false;

		if(curr_size.width() > screen_rect.width() * 0.90 )
		{
		  size.setWidth(screen_rect.width() * 0.90);
		  show_scrollarea = true;
		}

		if(curr_size.height() > screen_rect.height() * 0.90)
		{
		  size.setHeight(screen_rect.height() * 0.90);
		  show_scrollarea = true;
		}

		if(show_scrollarea && !scrollarea->isVisible())
		{
		  frame->setStyleSheet("QFrame#frame{ border: 0px; }");
		  frame->layout()->removeWidget(label);
		  frame->layout()->addWidget(scrollarea);
		  scrollarea->setVisible(true);
		  scrollarea->setWidget(label);
		  window_frm->setParent(scrollarea);
		}
		else if(!show_scrollarea)
		{
		  frame->setStyleSheet("");
		  scrollarea->setVisible(false);
		  scrollarea->takeWidget();
		  frame->layout()->removeWidget(scrollarea);
		  frame->layout()->addWidget(label);
		  window_frm->setParent(frame);
		}

		window_frm->setVisible(true);
		this->resize(size);
		this->setMaximumSize(size);
		this->setMinimumSize(size);
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
	if(!frame->isEnabled())
		return;

	if(event->buttons()==Qt::LeftButton)
	{
		QRect rect=window_frm->geometry(), rect1;
		int width = 0, height = 0, x=event->x(), y=event->y() + scrollarea->verticalScrollBar()->value();

		width=rect.width()/2;
		height=rect.height()/2;

		//Configures a rectangle having as central point the event position
		rect.setLeft(x - width);
		rect.setTop(y - height);
		rect.setRight(x + width);
		rect.setBottom(y + height);

		rect1=label->geometry();
		this->model->viewport->horizontalScrollBar()->setValue(ceil(zoom_factor * scene_rect.width() * (rect.x()/static_cast<double>(rect1.width()))));
		this->model->viewport->verticalScrollBar()->setValue(ceil(zoom_factor * scene_rect.height() * (rect.y()/static_cast<double>(rect1.height()))));
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

