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

#include "taskprogresswidget.h"
#include "baseobject.h"
#include "pgmodeleruins.h"

TaskProgressWidget::TaskProgressWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	vector<ObjectType> obj_types=BaseObject::getObjectTypes(true);

	setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	for(auto &obj_tp : obj_types)
		addIcon(enum_cast(obj_tp), QIcon(PgModelerUiNs::getIconPath(obj_tp)));
}

void TaskProgressWidget::addIcon(unsigned id, const QIcon &ico)
{
	icons[id]=ico;
}

void TaskProgressWidget::show()
{
	/* Using a event loop as a workaround to give a little time to task progress
	 to be shown before start the progress update. In tasks too quick, if the event loop above
	 isn't used the task is not shown properly and sometimes stay only on taskbar not poping up
	 to the user. */
	QEventLoop eventLoop;
	PgModelerUiNs::resizeDialog(this);
	QDialog::show();
	QTimer t;

	//Gives 100ms to the task to be shown and update its contents
	t.singleShot(100, &eventLoop, SLOT(quit()));
	text_lbl->setText(tr("Waiting task to start..."));
	eventLoop.exec(QEventLoop::AllEvents);
}

void TaskProgressWidget::updateProgress(int progress, unsigned icon_id)
{
	updateProgress(progress, QString(), icon_id);
}

void TaskProgressWidget::updateProgress(int progress, QString text, unsigned icon_id)
{
	if(progress > progress_pb->maximum())
		progress=progress_pb->maximum();

	progress_pb->setValue(progress);

	if(!text.isEmpty())
		text_lbl->setText(PgModelerUiNs::formatMessage(text));

	if(icons.count(icon_id))
		icon_lbl->setPixmap(icons[icon_id].pixmap(QSize(32,32)));
	else
		icon_lbl->clear();

	this->repaint();

	/* MacOSX workaround: The event loop below is needed because on this system
	   the task progress is not correctly updated. The event loop causes a little
	   delay (1ms) and it`s sufficient to update the entire widget */
#ifdef Q_OS_MAC
	QEventLoop eventLoop;
	QTimer t;
	//Gives 1ms to the task to be shown and update its contents
	t.singleShot(1, &eventLoop, SLOT(quit()));
	eventLoop.exec(QEventLoop::AllEvents);
#endif
}

void TaskProgressWidget::close()
{
	QDialog::close();
	progress_pb->setValue(0);
	text_lbl->clear();
	icon_lbl->clear();
}

