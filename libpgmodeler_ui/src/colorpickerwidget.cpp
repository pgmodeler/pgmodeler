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

ColorPickerWidget::ColorPickerWidget(unsigned color_count, QWidget * parent) : QWidget(parent)
{
	random_device rand_seed;
	rand_num_engine.seed(rand_seed());

	QToolButton *btn=nullptr;
	QHBoxLayout *hbox=nullptr;

	setupUi(this);

	if(color_count==0)
		color_count=1;
	else if(color_count > MAX_COLOR_BUTTONS)
		color_count=MAX_COLOR_BUTTONS;

	hbox=new QHBoxLayout(this);
	hbox->setContentsMargins(0,0,0,0);
	for(unsigned i=0; i < color_count; i++)
	{
		btn=new QToolButton(this);
		btn->setMinimumHeight(random_color_tb->height() - 3);
		btn->setMinimumWidth(55);

		disable_pal=btn->palette();
		buttons.push_back(btn);
		colors.push_back(btn->palette().color(QPalette::Button));

		hbox->addWidget(btn);
		connect(btn, SIGNAL(clicked()), this, SLOT(selectColor()));
	}

	hbox->addWidget(random_color_tb);
	this->adjustSize();

	connect(random_color_tb, SIGNAL(clicked()), this, SLOT(generateRandomColors()));
}

void ColorPickerWidget::setColor(unsigned color_idx, const QColor &color)
{
	QPalette palette;

	if(color_idx >  static_cast<unsigned>(colors.size()))
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	if(this->isEnabled())
	 palette.setColor(QPalette::Button, color);
	else
	 palette=disable_pal;

	buttons[color_idx]->setPalette(palette);
	colors[color_idx]=color;
}

QColor ColorPickerWidget::getColor(unsigned color_idx)
{
	if(color_idx > static_cast<unsigned>(colors.size()))
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(buttons[color_idx]->palette().color(QPalette::Button));
}

unsigned ColorPickerWidget::getColorCount(void)
{
	return(colors.size());
}

bool ColorPickerWidget::isButtonVisible(unsigned idx)
{
	if(idx > static_cast<unsigned>(buttons.size()))
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(buttons[idx]->isVisible());
}

void ColorPickerWidget::setEnabled(bool value)
{
	int i=0;

	for(auto btn : buttons)
		btn->setPalette(value ? colors[i++] : disable_pal);

	QWidget::setEnabled(value);
}

void ColorPickerWidget::setButtonVisible(unsigned idx, bool value)
{
	if(idx > static_cast<unsigned>(buttons.size()))
		throw Exception(ERR_REF_ELEM_INV_INDEX ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	buttons[idx]->setVisible(value);
}

void ColorPickerWidget::selectColor(void)
{
	QColorDialog color_dlg;
	QPalette palette;
	QToolButton *btn=qobject_cast<QToolButton *>(sender());

	color_dlg.setWindowTitle(trUtf8("Select color"));
	color_dlg.setCurrentColor(btn->palette().color(QPalette::Button));
	color_dlg.exec();

	if(color_dlg.result()==QDialog::Accepted)
	{
		palette.setColor(QPalette::Button, color_dlg.selectedColor());
		btn->setPalette(palette);

		colors[buttons.indexOf(btn)]=palette.color(QPalette::Button);
		emit s_colorChanged(static_cast<unsigned>(buttons.indexOf(btn)), color_dlg.selectedColor());
	}
}

void ColorPickerWidget::generateRandomColors(void)
{
	QPalette palette;
	QColor color;
	uniform_int_distribution<unsigned> dist(0,255);
	int i=0;

	for(auto btn : buttons)
	{
		color=QColor(dist(rand_num_engine),
								 dist(rand_num_engine),
								 dist(rand_num_engine));
		colors[i++]=color;
		palette.setColor(QPalette::Button, color);
		btn->setPalette(palette);
	}

	emit s_colorsChanged();
}

