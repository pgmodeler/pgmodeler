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

#include "pgsqltypewidget.h"

const QString PgSQLTypeWidget::InvalidType("invalid_type");

PgSQLTypeWidget::PgSQLTypeWidget(QWidget *parent, const QString &label) : QWidget(parent)
{
	try
	{
		QStringList interval_lst, spatial_lst;

		setupUi(this);

		if(!label.isEmpty())
			groupBox->setTitle(label);

		this->setWindowTitle(groupBox->title());

		format_hl=nullptr;
		format_hl=new SyntaxHighlighter(format_txt, true);
		format_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());
		this->adjustSize();

		interval_lst = IntervalType::getTypes();
		interval_cmb->addItem("");
		interval_cmb->addItems(interval_lst);

		spatial_lst = SpatialType::getTypes();
		spatial_lst.sort();
		spatial_cmb->addItem(tr("NONE"));
		spatial_cmb->addItems(spatial_lst);

		type_cmb->installEventFilter(this);

		connect(type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeFormat()));
		connect(precision_sb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat()));
		connect(length_sb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat()));
		connect(dimension_sb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat()));
		connect(interval_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeFormat()));
		connect(timezone_chk, SIGNAL(toggled(bool)), this, SLOT(updateTypeFormat()));
		connect(spatial_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTypeFormat()));
		connect(var_m_chk, SIGNAL(toggled(bool)), this, SLOT(updateTypeFormat()));
		connect(var_z_chk, SIGNAL(toggled(bool)), this, SLOT(updateTypeFormat()));
		connect(srid_spb, SIGNAL(valueChanged(int)), this, SLOT(updateTypeFormat()));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

bool PgSQLTypeWidget::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::KeyRelease && object == type_cmb)
		updateTypeFormat();

	return QWidget::eventFilter(object, event);
}

void PgSQLTypeWidget::updateTypeFormat()
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
		timezone_chk->setVisible(type==QString("timestamp") || type==QString("time"));
		timezone_lbl->setVisible(timezone_chk->isVisible());
		precision_sb->setEnabled(type.acceptsPrecision());
		dimension_sb->setEnabled(type!=QString("void"));
		interval_cmb->setVisible(type==QString("interval"));
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
			QString sp_type_name = spatial_cmb->currentIndex() > 0 ? spatial_cmb->currentText() : "";
			spatial_tp = SpatialType(sp_type_name, srid_spb->value());

			if(var_z_chk->isChecked() && var_m_chk->isChecked())
				spatial_tp.setVariation(SpatialType::VarZm);
			else if(var_m_chk->isChecked())
				spatial_tp.setVariation(SpatialType::VarM);
			else if(var_z_chk->isChecked())
				spatial_tp.setVariation(SpatialType::VarZ);

			type.setSpatialType(spatial_tp);
		}

		type.setLength(length_sb->value());
		type.setPrecision(precision_sb->value());
		type.setDimension(dimension_sb->value());
		type.setIntervalType(interval_cmb->currentText());
		type.setWithTimezone(timezone_chk->isChecked());

		format_txt->setPlainText(*type);
	}
	catch(Exception &)
	{
		format_txt->setPlainText(InvalidType);
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
		PgSqlType::getUserTypes(types,model, user_type_conf);
		types.sort();
		count=types.size();

		for(idx=0; idx < count; idx++)
			combo->addItem(types[idx], QVariant(PgSqlType::getUserTypeIndex(types[idx],nullptr,model)));

		//Getting the built-in type adding them into the combo
		types = PgSqlType::getTypes(oid_types, pseudo_types);
		types.sort();
		combo->addItems(types);
	}
}

void PgSQLTypeWidget::setAttributes(PgSqlType type, DatabaseModel *model,  unsigned usr_type_conf, bool oid_types, bool pseudo_types)
{
	try
	{
		int idx;
		QString type_name;

		type_cmb->blockSignals(true);
		listPgSQLTypes(type_cmb, model, usr_type_conf, oid_types, pseudo_types);
		type_cmb->blockSignals(false);

		//Get the passed type index
		type_name=~type;
		type_name.remove(QRegExp(QString("( )(with)(out)?(.)*")));
		idx=type_cmb->findText(type_name);

		//Select the type on the combo
		type_cmb->setCurrentIndex(idx);

		length_sb->setValue(type.getLength());
		precision_sb->setValue(type.getPrecision());
		dimension_sb->setValue(type.getDimension());


		idx=interval_cmb->findText(~(type.getIntervalType()));
		interval_cmb->setCurrentIndex(idx);

		idx=spatial_cmb->findText(~(type.getSpatialType()));
		if(idx < 0) idx = 0;
		spatial_cmb->setCurrentIndex(idx);

		timezone_chk->setChecked(type.isWithTimezone());

		this->type=type;
		updateTypeFormat();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

PgSqlType PgSQLTypeWidget::getPgSQLType()
{
	if(format_txt->toPlainText() == InvalidType)
		throw Exception(ErrorCode::AsgInvalidTypeObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return type;
}

