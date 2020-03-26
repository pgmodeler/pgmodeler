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

#include "relationshipconfigwidget.h"

map<QString, attribs_map> RelationshipConfigWidget::config_params;

RelationshipConfigWidget::RelationshipConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
{
	QStringList list, rel_types={ Attributes::Relationship11, Attributes::Relationship1n,
								  Attributes::RelationshipNn, Attributes::RelationshipGen,
								  Attributes::RelationshipDep, Attributes::RelationshipPart };
	unsigned rel_types_id[]={ BaseRelationship::Relationship11, BaseRelationship::Relationship1n,
							  BaseRelationship::RelationshipNn, BaseRelationship::RelationshipGen,
							  BaseRelationship::RelationshipDep, BaseRelationship::RelationshipPart};

	Ui_RelationshipConfigWidget::setupUi(this);

	SyntaxHighlighter *pattern_hl=nullptr;
	QList<QPlainTextEdit *> pattern_fields={ src_col_pattern_txt, dst_col_pattern_txt,
											 src_fk_pattern_txt, dst_fk_pattern_txt,
											 pk_pattern_txt, uq_pattern_txt, pk_col_pattern_txt };

	for(int i=0; i < pattern_fields.size(); i++)
	{
		pattern_hl=new SyntaxHighlighter(pattern_fields[i], true);
		pattern_hl->loadConfiguration(GlobalAttributes::getConfigurationFilePath(GlobalAttributes::PatternHighlightConf));

		connect(pattern_fields[i], SIGNAL(textChanged()), this, SLOT(updatePattern()));
	}

	deferral_cmb->addItems(DeferralType::getTypes());

	list = ActionType::getTypes();
	list.push_front(tr("Default"));
	del_action_cmb->addItems(list);
	upd_action_cmb->addItems(list);

	for(int i=0; i < rel_types.size(); i++)
		rel_type_cmb->addItem(BaseRelationship::getRelationshipTypeName(rel_types_id[i]), rel_types[i]);

	connect(crows_foot_rb, SIGNAL(toggled(bool)), this, SLOT(enableConnModePreview()));
	connect(fk_to_pk_rb, SIGNAL(toggled(bool)), this, SLOT(enableConnModePreview()));
	connect(center_pnts_rb, SIGNAL(toggled(bool)), this, SLOT(enableConnModePreview()));
	connect(tab_edges_rb, SIGNAL(toggled(bool)), this, SLOT(enableConnModePreview()));

	connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_lbl, SLOT(setEnabled(bool)));
	connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_cmb, SLOT(setEnabled(bool)));
	connect(deferrable_chk, SIGNAL(toggled(bool)), this, SLOT(setConfigurationChanged(bool)));

	connect(rel_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(fillNamePatterns()));
	connect(del_action_cmb, &QComboBox::currentTextChanged, [&](){ setConfigurationChanged(true); });
	connect(upd_action_cmb, &QComboBox::currentTextChanged, [&](){ setConfigurationChanged(true); });
	connect(deferral_cmb, &QComboBox::currentTextChanged, [&](){ setConfigurationChanged(true); });
}

map<QString, attribs_map> RelationshipConfigWidget::getConfigurationParams()
{
	return config_params;
}

void RelationshipConfigWidget::loadConfiguration()
{
	try
	{
		int idx;
		vector<QString> key_attribs={Attributes::Type};
		BaseConfigWidget::loadConfiguration(GlobalAttributes::RelationshipsConf, config_params, key_attribs);

		fk_to_pk_rb->setChecked(config_params[Attributes::Connection][Attributes::Mode]==Attributes::ConnectFkToPk);
		center_pnts_rb->setChecked(config_params[Attributes::Connection][Attributes::Mode]==Attributes::ConnectCenterPnts);
		tab_edges_rb->setChecked(config_params[Attributes::Connection][Attributes::Mode]==Attributes::ConnectTableEdges);
		crows_foot_rb->setChecked(config_params[Attributes::Connection][Attributes::Mode]==Attributes::CrowsFoot);

		deferrable_chk->setChecked(config_params[Attributes::ForeignKeys][Attributes::Deferrable]==Attributes::True);
		deferral_cmb->setCurrentText(config_params[Attributes::ForeignKeys][Attributes::DeferType]);

		idx=upd_action_cmb->findText(config_params[Attributes::ForeignKeys][Attributes::UpdAction]);
		upd_action_cmb->setCurrentIndex(idx < 0 ? 0 : idx);

		idx=del_action_cmb->findText(config_params[Attributes::ForeignKeys][Attributes::DelAction]);
		del_action_cmb->setCurrentIndex(idx < 0 ? 0 : idx);

		patterns[Attributes::Relationship11]=config_params[Attributes::Relationship11];
		patterns[Attributes::Relationship1n]=config_params[Attributes::Relationship1n];
		patterns[Attributes::RelationshipNn]=config_params[Attributes::RelationshipNn];
		patterns[Attributes::RelationshipGen]=config_params[Attributes::RelationshipGen];
		patterns[Attributes::RelationshipDep]=config_params[Attributes::RelationshipDep];
		patterns[Attributes::RelationshipPart]=config_params[Attributes::RelationshipPart];

		fillNamePatterns();
		this->applyConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo());
	}
}

void RelationshipConfigWidget::saveConfiguration()
{  
	try
	{
		QString patterns_sch;

		patterns_sch=GlobalAttributes::getTmplConfigurationFilePath(GlobalAttributes::SchemasDir,
																																Attributes::Patterns +
																																GlobalAttributes::SchemaExt);
		if(crows_foot_rb->isChecked())
			config_params[Attributes::Connection][Attributes::Mode]=Attributes::CrowsFoot;
		else if(fk_to_pk_rb->isChecked())
			config_params[Attributes::Connection][Attributes::Mode]=Attributes::ConnectFkToPk;
		else if(tab_edges_rb->isChecked())
			config_params[Attributes::Connection][Attributes::Mode]=Attributes::ConnectTableEdges;
		else
			config_params[Attributes::Connection][Attributes::Mode]=Attributes::ConnectCenterPnts;

		config_params[Attributes::ForeignKeys][Attributes::Deferrable]=(deferrable_chk->isChecked() ? Attributes::True : Attributes::False);
		config_params[Attributes::ForeignKeys][Attributes::DeferType]=deferral_cmb->currentText();
		config_params[Attributes::ForeignKeys][Attributes::UpdAction]=(upd_action_cmb->currentIndex() > 0 ? upd_action_cmb->currentText() : QString());
		config_params[Attributes::ForeignKeys][Attributes::DelAction]=(del_action_cmb->currentIndex() > 0 ? del_action_cmb->currentText() : QString());

		config_params[Attributes::NamePatterns][Attributes::Patterns]=QString();

		for(auto &itr : patterns)
		{
			schparser.ignoreUnkownAttributes(true);
			schparser.ignoreEmptyAttributes(true);
			config_params[itr.first]=itr.second;
			config_params[Attributes::NamePatterns][Attributes::Patterns]+=schparser.getCodeDefinition(patterns_sch, itr.second);
		}

		BaseConfigWidget::saveConfiguration(GlobalAttributes::RelationshipsConf, config_params);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipConfigWidget::applyConfiguration()
{
	RelationshipView::setCrowsFoot(crows_foot_rb->isChecked());

	if(!crows_foot_rb->isChecked())
	{
		if(fk_to_pk_rb->isChecked())
			RelationshipView::setLineConnectionMode(RelationshipView::ConnectFkToPk);
		else if(tab_edges_rb->isChecked())
			RelationshipView::setLineConnectionMode(RelationshipView::ConnectTableEdges);
		else
			RelationshipView::setLineConnectionMode(RelationshipView::ConnectCenterPoints);
	}
}

void RelationshipConfigWidget::restoreDefaults()
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::RelationshipsConf, false);
		this->loadConfiguration();
		setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipConfigWidget::fillNamePatterns()
{
	QString rel_type=rel_type_cmb->currentData().toString();
	bool relnn=false, reldep=false, relgen=false;
	QList<QPlainTextEdit *> inputs={ pk_pattern_txt, uq_pattern_txt, src_col_pattern_txt,
									 dst_col_pattern_txt, src_fk_pattern_txt, dst_fk_pattern_txt,
									 pk_col_pattern_txt };

	QList<QString> pattern_ids={ Attributes::PkPattern,  Attributes::UqPattern,
								 Attributes::SrcColPattern, Attributes::DstColPattern,
								 Attributes::SrcFkPattern, Attributes::DstFkPattern,
								 Attributes::PkColPattern };

	relnn=(rel_type==Attributes::RelationshipNn);
	reldep=(rel_type==Attributes::RelationshipDep || rel_type==Attributes::RelationshipPart);
	relgen=(rel_type==Attributes::RelationshipGen);

	dst_col_pattern_txt->setEnabled(relnn);
	dst_fk_pattern_txt->setEnabled(relnn);
	src_col_pattern_txt->setEnabled(!relgen && !reldep);
	src_fk_pattern_txt->setEnabled(!relgen && !reldep);
	uq_pattern_txt->setEnabled(!relgen && !reldep);
	pk_col_pattern_txt->setEnabled(relnn);

	dst_col_pattern_lbl->setEnabled(relnn);
	dst_fk_pattern_lbl->setEnabled(relnn);
	src_col_pattern_lbl->setEnabled(!relgen && !reldep);
	src_fk_pattern_lbl->setEnabled(!relgen && !reldep);
	uq_pattern_lbl->setEnabled(!relgen && !reldep);
	pk_col_pattern_lbl->setEnabled(relnn);

	for(int i=0; i < inputs.size(); i++)
	{
		inputs[i]->blockSignals(true);
		inputs[i]->clear();

		if(inputs[i]->isEnabled() && patterns[rel_type].count(pattern_ids[i]))
			inputs[i]->setPlainText(patterns[rel_type][pattern_ids[i]]);

		inputs[i]->blockSignals(false);
	}
}

void RelationshipConfigWidget::updatePattern()
{
	QPlainTextEdit *input=qobject_cast<QPlainTextEdit *>(sender());
	QString rel_type=rel_type_cmb->currentData().toString();
	map<QPlainTextEdit *, QString> inputs_map={ { pk_pattern_txt, Attributes::PkPattern },
												{ uq_pattern_txt, Attributes::UqPattern },
												{ src_col_pattern_txt, Attributes::SrcColPattern },
												{ dst_col_pattern_txt, Attributes::DstColPattern },
												{ src_fk_pattern_txt, Attributes::SrcFkPattern   },
												{ dst_fk_pattern_txt, Attributes::DstFkPattern   },
												{ pk_col_pattern_txt, Attributes::PkColPattern   } };

	setConfigurationChanged(true);
	patterns[rel_type][inputs_map[input]]=input->toPlainText();
}

void RelationshipConfigWidget::enableConnModePreview()
{
	crows_foot_lbl->setEnabled(crows_foot_rb->isChecked());
	conn_cnt_pnts_lbl->setEnabled(center_pnts_rb->isChecked());
	conn_tab_edges_lbl->setEnabled(tab_edges_rb->isChecked());
	conn_fk_pk_lbl->setEnabled(fk_to_pk_rb->isChecked());
	setConfigurationChanged(true);
}

void RelationshipConfigWidget::hideEvent(QHideEvent *)
{
	settings_twg->setCurrentIndex(0);
}
