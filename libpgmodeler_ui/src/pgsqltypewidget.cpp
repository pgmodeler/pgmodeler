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

#include "pgsqltypewidget.h"

PgSQLTypeWidget::PgSQLTypeWidget(QWidget *parent, const QString &label) : QWidget(parent)
{
	try
	{
		QStringList interval_lst, spatial_lst;

		setupUi(this);

		if(!label.isEmpty())
			groupBox->setTitle(label);

		this->setWindowTitle(groupBox->title());

		format_hl=NULL;
		format_hl=new SyntaxHighlighter(format_txt, false);

		format_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		GlobalAttributes::DIR_SEPARATOR +
																		GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		GlobalAttributes::CONFIGURATION_EXT);

		IntervalType::getTypes(interval_lst);
		interval_cmb->addItem("");
		interval_cmb->addItems(interval_lst);

		SpatialType::getTypes(spatial_lst);
		spatial_cmb->addItems(spatial_lst);

		connect(type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeFormat(void)));
		connect(precision_sb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat(void)));
		connect(length_sb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat(void)));
		connect(dimension_sb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat(void)));
		connect(interval_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeFormat(void)));
		connect(timezone_chk, SIGNAL(toggled(bool)), this, SLOT(updateTypeFormat(void)));
		connect(spatial_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeFormat(void)));
		connect(var_m_chk, SIGNAL(toggled(bool)), this, SLOT(updateTypeFormat(void)));
		connect(var_z_chk, SIGNAL(toggled(bool)), this, SLOT(updateTypeFormat(void)));
		connect(srid_spb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat(void)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PgSQLTypeWidget::updateTypeFormat(void)
{
	try
	{
		QVariant data;

		//Gets the data related to the current selected data type
		data=type_cmb->itemData(type_cmb->currentIndex());

		//If the data value (index) is 0 indicates that the type is a built-in one
		if(data.toUInt()==0)
			type=type_cmb->currentText();
		else
			//Case the index is greated than zero indicates that the type is a user-defined one
			type=data.toUInt();

		length_sb->setEnabled(type.hasVariableLength());
		timezone_chk->setVisible(type=="timestamp" || type=="time");
		timezone_lbl->setVisible(timezone_chk->isVisible());
		precision_sb->setEnabled(type.acceptsPrecision());
		dimension_sb->setEnabled(type!="void");
		interval_cmb->setVisible(type=="interval");
		interval_lbl->setVisible(interval_cmb->isVisible());

		spatial_cmb->setVisible(type.isGiSType());
		spatial_lbl->setVisible(type.isGiSType());
		variation_lbl->setVisible(type.isGiSType());
		srid_lbl->setVisible(type.isGiSType());
		srid_spb->setVisible(type.isGiSType());
		var_m_chk->setVisible(type.isGiSType());
		var_z_chk->setVisible(type.isGiSType());

		if(spatial_cmb->isVisible())
		{
			SpatialType spatial_tp;

			//For the geography type the SRID is always 4326
			if(type=="geography") srid_spb->setValue(4326);

			spatial_tp=SpatialType(spatial_cmb->currentText(), srid_spb->value());

			if(var_z_chk->isChecked() && var_m_chk->isChecked())
				spatial_tp.setVariation(SpatialType::var_zm);
			else if(var_m_chk->isChecked())
				spatial_tp.setVariation(SpatialType::var_m);
			else if(var_z_chk->isChecked())
				spatial_tp.setVariation(SpatialType::var_z);

			type.setSpatialType(spatial_tp);
		}

		type.setLength(length_sb->value());
		type.setPrecision(precision_sb->value());
		type.setDimension(dimension_sb->value());
		type.setIntervalType(interval_cmb->currentText());
		type.setWithTimezone(timezone_chk->isChecked());

		format_txt->setPlainText(Utf8String::create(*type));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void PgSQLTypeWidget::listPgSQLTypes(QComboBox *combo, DatabaseModel *model, unsigned user_type_conf, bool oid_types, bool pseudo_types)
{
	if(combo)
	{
		QStringList types;
		int idx, count;

		combo->clear();

		//Getting the user defined type adding them into the combo
		PgSQLType::getUserTypes(types,model, user_type_conf);
		types.sort();
		count=types.size();

		for(idx=0; idx < count; idx++)
			combo->addItem(Utf8String::create(types[idx]), QVariant(PgSQLType::getUserTypeIndex(types[idx],NULL,model)));

		//Getting the built-in type adding them into the combo
		PgSQLType::getTypes(types, oid_types, pseudo_types);
		types.sort();
		combo->addItems(types);
	}
}

void PgSQLTypeWidget::setAttributes(PgSQLType type, DatabaseModel *model,  unsigned usr_type_conf, bool oid_types, bool pseudo_types)
{
	try
	{
		int idx;

		type_cmb->blockSignals(true);
		listPgSQLTypes(type_cmb, model, usr_type_conf, oid_types, pseudo_types);
		type_cmb->blockSignals(false);

		//Get the passed type index
		idx=type_cmb->findText(Utf8String::create(~type));

		//Select the type on the combo
		type_cmb->setCurrentIndex(idx);

		precision_sb->setValue(type.getPrecision());
		dimension_sb->setValue(type.getDimension());
		length_sb->setValue(type.getLength());

		idx=interval_cmb->findText(~(type.getIntervalType()));
		interval_cmb->setCurrentIndex(idx);

		timezone_chk->setChecked(type.isWithTimezone());

		this->type=type;
		updateTypeFormat();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

PgSQLType PgSQLTypeWidget::getPgSQLType(void)
{
	return(type);
}

