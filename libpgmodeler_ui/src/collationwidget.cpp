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

#include "collationwidget.h"

CollationWidget::CollationWidget(QWidget *parent): BaseObjectWidget(parent, ObjectType::Collation)
{
	try
	{
		QStringList loc_list, encodings, providers;
		QFrame *frame=nullptr;

		Ui_CollationWidget::setupUi(this);

		frame=generateInformationFrame(tr("The fields <strong><em>Collation</em></strong>, <strong><em>Locale</em></strong>, <strong><em>LC_COLLATE & LC_CTYPE</em></strong> are mutually exclusive, so you have to set only one of them in order to properly handle a collation."));

		collation_grid->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum,QSizePolicy::Expanding), collation_grid->count()+1, 0, 1, 0);
		collation_grid->addWidget(frame, collation_grid->count()+1, 0, 1, 0);
		frame->setParent(this);

		configureFormLayout(collation_grid, ObjectType::Collation);

		map<QString, vector<QWidget *> > fields_map;
		fields_map[generateVersionsInterval(AfterVersion, PgSqlVersions::PgSqlVersion100)].push_back(provider_lbl);
		fields_map[generateVersionsInterval(AfterVersion, PgSqlVersions::PgSqlVersion120)].push_back(deterministic_chk);
		highlightVersionSpecificFields(fields_map);

		//Configures the encoding combobox
		encodings = EncodingType::getTypes();
		encodings.push_front(tr("Not defined"));
		encoding_cmb->addItems(encodings);

		//Configures the localizations combobox
		for(int i=QLocale::C; i <= QLocale::Chewa; i++)
		{
			for(int i1=QLocale::Afghanistan; i1 <= QLocale::Zimbabwe; i1++)
				loc_list.append(QLocale(static_cast<QLocale::Language>(i),static_cast<QLocale::Country>(i1)).name());
		}

		loc_list.removeDuplicates();
		loc_list.sort();
		loc_list.push_front(tr("Not defined"));

		lccollate_cmb->addItems(loc_list);
		lcctype_cmb->addItems(loc_list);
		locale_cmb->addItems(loc_list);

		providers = ProviderType::getTypes();
		providers.push_front(tr("Default"));
		provider_cmb->addItems(providers);

		connect(collation_sel, SIGNAL(s_objectSelected()), this, SLOT(resetFields()));
		connect(collation_sel, SIGNAL(s_selectorCleared()), this, SLOT(resetFields()));
		connect(locale_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(resetFields()));
		connect(lcctype_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(resetFields()));
		connect(lccollate_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(resetFields()));

		configureTabOrder({ locale_cmb, encoding_cmb, lccollate_cmb,
												lcctype_cmb, provider_cmb, deterministic_chk });

		locale_mod_lbl->setToolTip(tr("<p>The modifier is any value specified after the character <strong>@</strong>. For example: <em>en_US.utf8<strong>@modifier</strong></em></p>"));
		lcctype_mod_lbl->setToolTip(locale_mod_lbl->toolTip());
		lccollate_mod_lbl->setToolTip(locale_mod_lbl->toolTip());

		setMinimumSize(540, 500);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void CollationWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Collation *collation)
{
	BaseObjectWidget::setAttributes(model, op_list, collation, schema);

	if(collation)
	{
		int idx=-1;

		idx=encoding_cmb->findText(~collation->getEncoding());
		encoding_cmb->setCurrentIndex(idx < 0 ? 0 : idx);

		if(!collation_sel->getSelectedObject())
		{
			idx=locale_cmb->findText(collation->getLocale());
			locale_cmb->setCurrentIndex(idx < 0 ? 0 : idx);
			locale_mod_edt->setText(collation->getModifier(Collation::Locale));

			if(locale_cmb->currentIndex()==0)
			{
				idx=lcctype_cmb->findText(collation->getLocalization(Collation::LcCtype));
				lcctype_cmb->setCurrentIndex(idx < 0 ? 0 : idx);
				lcctype_mod_edt->setText(collation->getModifier(Collation::LcCtype));

				idx=lccollate_cmb->findText(collation->getLocalization(Collation::LcCollate));
				lccollate_cmb->setCurrentIndex(idx < 0 ? 0 : idx);
				lccollate_mod_edt->setText(collation->getModifier(Collation::LcCollate));
			}
		}

		provider_cmb->setCurrentText(~collation->getProvider());
		deterministic_chk->setChecked(collation->isDeterministic());
	}
}

void CollationWidget::resetFields()
{
	//Block object's signals to evict an infinite call to this method
	collation_sel->blockSignals(true);
	locale_cmb->blockSignals(true);
	lccollate_cmb->blockSignals(true);
	lcctype_cmb->blockSignals(true);

	//If there is no sender reset all fields
	if(!sender())
	{
		collation_sel->clearSelector();
		locale_cmb->setCurrentIndex(0);
		lccollate_cmb->setCurrentIndex(0);
		lcctype_cmb->setCurrentIndex(0);
	}
	//Resetting the collation selector and locale combo
	else if((sender()==lccollate_cmb || sender()==lcctype_cmb) &&
			(lccollate_cmb->currentIndex() > 0 || lcctype_cmb->currentIndex() > 0))
	{
		collation_sel->clearSelector();
		locale_cmb->setCurrentIndex(0);
		locale_mod_edt->clear();
	}
	//Resetting the lc_??? combos
	else if((sender()==collation_sel || sender()==locale_cmb) &&
			(collation_sel->getSelectedObject()!=nullptr || locale_cmb->currentIndex() > 0))
	{
		lccollate_cmb->setCurrentIndex(0);
		lcctype_cmb->setCurrentIndex(0);
		lccollate_mod_edt->clear();
		lcctype_mod_edt->clear();

		//Additionally resets the collation selector or locale combo depending on sender()
		if(sender()==collation_sel && collation_sel->getSelectedObject()!=nullptr)
			locale_cmb->setCurrentIndex(0);
		else
			collation_sel->clearSelector();
	}

	lcctype_mod_edt->setEnabled(lcctype_cmb->currentIndex() > 0);
	lccollate_mod_edt->setEnabled(lccollate_cmb->currentIndex() > 0);
	locale_mod_edt->setEnabled(locale_cmb->currentIndex() > 0);

	collation_sel->blockSignals(false);
	locale_cmb->blockSignals(false);
	lccollate_cmb->blockSignals(false);
	lcctype_cmb->blockSignals(false);
}

void CollationWidget::applyConfiguration()
{
	try
	{
		Collation *collation=nullptr;

		startConfiguration<Collation>();

		collation=dynamic_cast<Collation *>(this->object);
		BaseObjectWidget::applyConfiguration();

		if(encoding_cmb->currentIndex() > 0)
			collation->setEncoding(EncodingType(encoding_cmb->currentText()));

		if(locale_cmb->currentIndex() > 0)
		{
			collation->setLocale(locale_cmb->currentText());
			collation->setModifier(Collation::Locale, locale_mod_edt->text());
		}

		if(lccollate_cmb->currentIndex() > 0)
		{
			collation->setLocalization(Collation::LcCollate, lccollate_cmb->currentText());
			collation->setModifier(Collation::LcCollate, lccollate_mod_edt->text());
		}

		if(lcctype_cmb->currentIndex() > 0)
		{
			collation->setLocalization(Collation::LcCtype, lcctype_cmb->currentText());
			collation->setModifier(Collation::LcCtype, lcctype_mod_edt->text());
		}

		collation->setProvider(ProviderType(static_cast<unsigned>(provider_cmb->currentIndex())));
		collation->setDeterministic(deterministic_chk->isChecked());

		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}
