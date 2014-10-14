/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

HintTextWidget::HintTextWidget(QWidget *hint_wgt, QWidget *btn_parent, QWidget *parent): QWidget(parent)
{
  if(!hint_wgt || !btn_parent)
    throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  QHBoxLayout *layout=new QHBoxLayout(btn_parent);
  QGraphicsDropShadowEffect *shadow=nullptr;

  Ui_HintTextWidget::setupUi(this);

  this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
  text_lbl->setText(hint_wgt->statusTip());

  hint_tb=new QToolButton(this);
  hint_tb->setCheckable(true);
  hint_tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
  hint_tb->setMaximumSize(22,22);
  hint_tb->setIconSize(QSize(16,16));
  hint_tb->setIcon(QPixmap(":/icones/icones/help.png"));

  shadow=new QGraphicsDropShadowEffect(this);
  shadow->setOffset(5,5);
  shadow->setBlurRadius(30);
  this->setGraphicsEffect(shadow);

  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(hint_tb);
  btn_parent->setLayout(layout);

  QFontMetrics f(text_lbl->font());
  QRect ret=f.boundingRect(0,0, this->maximumWidth(), this->maximumHeight(), Qt::TextWordWrap, text_lbl->text());
  this->setMinimumSize(QSize(ret.size().width(), ret.size().height() + text_lbl->margin()));
  this->setVisible(false);

  text_lbl->installEventFilter(this);
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

  pos.setX(pos.x() - 5);
  pos.setY(pos.y() + hint_tb->height() - 2);
  new_pos=qobject_cast<QWidget *>(this->parent())->mapFromGlobal(pos);
  this->move(new_pos);
}

bool HintTextWidget::eventFilter(QObject *object, QEvent *event)
{
  if(object==text_lbl && (event->type()==QEvent::MouseButtonPress || event->type()==QEvent::FocusOut))
  {
    hint_tb->setChecked(false);
    return(true);
  }

  return(QWidget::eventFilter(object, event));
}
