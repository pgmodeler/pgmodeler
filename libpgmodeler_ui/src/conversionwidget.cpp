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

#include "conversionwidget.h"

ConversionWidget::ConversionWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_CONVERSION)
{
	try
	{
		QFrame *frame=nullptr;
		QStringList encodings;

		Ui_ConversionWidget::setupUi(this);

		conv_func_sel=nullptr;
		conv_func_sel=new ObjectSelectorWidget(OBJ_FUNCTION, true, this);
		convcod_grid->addWidget(conv_func_sel,1,1,1,3);

        setRequiredField(src_encoding_lbl);
        setRequiredField(trg_encoding_lbl);
        setRequiredField(conv_func_lbl);
        setRequiredField(conv_func_sel);

		configureFormLayout(convcod_grid, OBJ_CONVERSION);
		frame=generateInformationFrame(trUtf8("The function to be assigned to an encoding conversion must have the following signature: <em>void function(integer, integer, cstring, internal, integer)</em>."));
		convcod_grid->addWidget(frame, convcod_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		EncodingType::getTypes(encodings);
		src_encoding_cmb->addItems(encodings);
		trg_encoding_cmb->addItems(encodings);

		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->setMinimumSize(600, 350);
		parent_form->setMaximumHeight(350);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void ConversionWidget::hideEvent(QHideEvent *event)
{
	conv_func_sel->clearSelector();
	default_conv_chk->setChecked(false);
	BaseObjectWidget::hideEvent(event);
}

void ConversionWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Conversion *conv)
{
	BaseObjectWidget::setAttributes(model, op_list, conv, schema);
	conv_func_sel->setModel(model);

	if(conv)
	{
		conv_func_sel->setSelectedObject(conv->getConversionFunction());
		conv->setDefault(default_conv_chk->isChecked());
		src_encoding_cmb->setCurrentIndex(trg_encoding_cmb->findText(~(conv->getEncoding(Conversion::SRC_ENCODING))));
		trg_encoding_cmb->setCurrentIndex(trg_encoding_cmb->findText(~(conv->getEncoding(Conversion::DST_ENCODING))));
	}
}

void ConversionWidget::applyConfiguration(void)
{
	try
	{
		Conversion *conv=nullptr;

		startConfiguration<Conversion>();
		conv=dynamic_cast<Conversion *>(this->object);

		BaseObjectWidget::applyConfiguration();

		conv->setEncoding(Conversion::SRC_ENCODING, src_encoding_cmb->currentText());
		conv->setEncoding(Conversion::DST_ENCODING, trg_encoding_cmb->currentText());
		conv->setDefault(default_conv_chk->isChecked());
		conv->setConversionFunction(dynamic_cast<Function*>(conv_func_sel->getSelectedObject()));

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

