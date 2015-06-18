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

#include "colorpickerwidget.h"
#include <QToolTip>

ColorPickerWidget::ColorPickerWidget(int color_count, QWidget * parent) : QWidget(parent)
{
	random_device rand_seed;
	rand_num_engine.seed(rand_seed());

	QToolButton *btn=nullptr;
	QHBoxLayout *hbox=nullptr;
  QSpacerItem *spacer=new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Fixed);

	setupUi(this);

	if(color_count==0)
		color_count=1;
	else if(color_count > MAX_COLOR_BUTTONS)
		color_count=MAX_COLOR_BUTTONS;

  hbox=new QHBoxLayout(this);
	hbox->setContentsMargins(0,0,0,0);

  for(int i=0; i < color_count; i++)
	{
		btn=new QToolButton(this);
		btn->setMinimumHeight(random_color_tb->height() - 3);
		btn->setMinimumWidth(55);
    btn->installEventFilter(this);

    disable_color=btn->palette().color(QPalette::Button);
		buttons.push_back(btn);
    colors.push_back(disable_color);

		hbox->addWidget(btn);
		connect(btn, SIGNAL(clicked()), this, SLOT(selectColor()));
	}

	hbox->addWidget(random_color_tb);
  hbox->addSpacerItem(spacer);
	this->adjustSize();

	connect(random_color_tb, SIGNAL(clicked()), this, SLOT(generateRandomColors()));
}

bool ColorPickerWidget::eventFilter(QObject *object, QEvent *event)
{
  QToolButton *button=qobject_cast<QToolButton *>(object);

  if(event->type()==QEvent::ToolTip && button && button!=random_color_tb)
  {
    QToolTip::showText(QCursor::pos(), button->toolTip());
    return(true);
  }

  return(QWidget::eventFilter(object, event));
}

void ColorPickerWidget::setColor(int color_idx, const QColor &color)
{
  QString cl_name;

  if(color_idx < 0 || color_idx >=  colors.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  if(this->isEnabled())
    cl_name=color.name();
  else
    cl_name=disable_color.name();

  buttons[color_idx]->setStyleSheet(QString("background-color: %1").arg(cl_name));
	colors[color_idx]=color;
}

QColor ColorPickerWidget::getColor(int color_idx)
{
  if(color_idx < 0 || color_idx >= colors.size())
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  return(colors[color_idx]);
}

unsigned ColorPickerWidget::getColorCount(void)
{
	return(colors.size());
}

bool ColorPickerWidget::isButtonVisible(unsigned idx)
{
  if(idx >= static_cast<unsigned>(buttons.size()))
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  return(buttons[idx]->isVisible());
}

void ColorPickerWidget::setButtonToolTip(unsigned button_idx, const QString &tooltip)
{
  if(button_idx >= static_cast<unsigned>(buttons.size()))
    throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

  buttons[button_idx]->setToolTip(tooltip);
}

void ColorPickerWidget::setEnabled(bool value)
{
	int i=0;

  for(auto &btn : buttons)
    btn->setStyleSheet(QString("background-color: %1")
                       .arg(value ? colors[i++].name() : disable_color.name()));

	QWidget::setEnabled(value);
}

void ColorPickerWidget::setButtonVisible(unsigned idx, bool value)
{
  if(idx >= static_cast<unsigned>(buttons.size()))
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	buttons[idx]->setVisible(value);
}

void ColorPickerWidget::selectColor(void)
{
	QColorDialog color_dlg;
	QToolButton *btn=qobject_cast<QToolButton *>(sender());
  int btn_idx=buttons.indexOf(btn);

	color_dlg.setWindowTitle(trUtf8("Select color"));
  color_dlg.setCurrentColor(colors[btn_idx]);
	color_dlg.exec();

	if(color_dlg.result()==QDialog::Accepted)
	{
    setColor(btn_idx, color_dlg.selectedColor());
		emit s_colorChanged(static_cast<unsigned>(buttons.indexOf(btn)), color_dlg.selectedColor());
	}
}

void ColorPickerWidget::generateRandomColors(void)
{
	QColor color;
	uniform_int_distribution<unsigned> dist(0,255);
	int i=0;

  for(i=0; i < buttons.size(); i++)
	{
		color=QColor(dist(rand_num_engine),
								 dist(rand_num_engine),
								 dist(rand_num_engine));
    setColor(i, color);
	}

	emit s_colorsChanged();
}

