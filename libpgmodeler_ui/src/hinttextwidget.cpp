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

#include "hinttextwidget.h"
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "pgmodeleruins.h"

HintTextWidget::HintTextWidget(QWidget *btn_parent, QWidget *parent): QWidget(parent)
{
	if(!btn_parent)
		throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QHBoxLayout *layout=new QHBoxLayout(btn_parent);
	QGraphicsDropShadowEffect *shadow=nullptr;

	Ui_HintTextWidget::setupUi(this);

	this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);

	hint_tb=new QToolButton(this);
	hint_tb->setCheckable(true);
	hint_tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
	hint_tb->setIcon(QPixmap(QString(":/icones/icones/help.png")));

	shadow=new QGraphicsDropShadowEffect(this);
	shadow->setOffset(5,5);
	shadow->setBlurRadius(30);
	this->setGraphicsEffect(shadow);

	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(hint_tb);
	btn_parent->setLayout(layout);

	this->setVisible(false);
	text_lbl->installEventFilter(this);
	PgModelerUiNS::configureWidgetFont(text_lbl, PgModelerUiNS::MEDIUM_FONT_FACTOR);

	parent->installEventFilter(this);

	setIconSize(SMALL_ICON);
	connect(hint_tb, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
}

void HintTextWidget::setVisible(bool value)
{
	if(value)
	{
		setWidgetPosition();
		text_lbl->setFocus();
	}

	QWidget::setVisible(value);
}

void HintTextWidget::setWidgetPosition(void)
{
	QPoint pos=hint_tb->mapToGlobal(hint_tb->pos()), new_pos;
	QWidget *parent_wgt=qobject_cast<QWidget *>(this->parent());
	int right_pos=0, bottom_pos=0;

	pos.setX(pos.x() - 5);
	pos.setY(pos.y() + hint_tb->height() - 2);
	new_pos=parent_wgt->mapFromGlobal(pos);

	//Adjusting the hint position if the current pos extrapolates the parent's rect
	right_pos=new_pos.x() + this->width();
	if(right_pos > parent_wgt->width())
		new_pos.setX(new_pos.x() - (right_pos - parent_wgt->width()) - (hint_tb->width()/2));

	bottom_pos=new_pos.y() + this->height();
	if(bottom_pos > parent_wgt->height())
		new_pos.setY(new_pos.y() - (hint_tb->height() + this->height()));

	this->move(new_pos);
}

void HintTextWidget::setText(const QString &text)
{
	QFontMetrics f=text_lbl->fontMetrics();
	QString txt=text;
	QRect ret;
	QSize txt_size;

	txt.replace(QRegExp(QString("(<)(br)(/)?(>)")), QString("\n"));
	txt.remove(QRegExp(QString("(<)(/)?([a-z]|[A-Z])+(>)")));
	ret=f.boundingRect(0,0, this->maximumWidth(), this->maximumHeight(), Qt::TextWordWrap, txt);
	txt_size=QSize(ret.size().width(), ret.size().height() + f.lineSpacing() + (text_lbl->margin() * 1.5));

	text_lbl->setText(text);
	text_lbl->setMaximumSize(txt_size);
	text_lbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	this->setMinimumSize(txt_size + QSize(0, text_lbl->margin()/2));
	this->adjustSize();
}

void HintTextWidget::setIconSize(unsigned icon_sz)
{
	if(icon_sz==0)
		icon_sz=SMALL_ICON;
	else if(icon_sz > LARGE_ICON)
		icon_sz=LARGE_ICON;

	hint_tb->setMaximumSize(icon_sz + 8, icon_sz + 8);
	hint_tb->setIconSize(QSize(icon_sz, icon_sz));
}

QString HintTextWidget::getText(void)
{
	return(text_lbl->text());
}

bool HintTextWidget::eventFilter(QObject *object, QEvent *event)
{
	//Close the hint when it text lost its focus
	if(object==text_lbl && (event->type()==QEvent::MouseButtonPress || event->type()==QEvent::FocusOut))
	{
		hint_tb->setChecked(false);
		return(true);
	}
	//Reconfigures the popup position when the parent is resized
	else if(object==this->parent() && event->type()==QEvent::Resize)
		setWidgetPosition();

	return(QWidget::eventFilter(object, event));
}
