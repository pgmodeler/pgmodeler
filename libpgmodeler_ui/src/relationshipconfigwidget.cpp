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

#include "relationshipconfigwidget.h"

map<QString, attribs_map> RelationshipConfigWidget::config_params;

RelationshipConfigWidget::RelationshipConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
{
	QStringList list, rel_types={ ParsersAttributes::RELATIONSHIP_11, ParsersAttributes::RELATIONSHIP_1N,
																ParsersAttributes::RELATIONSHIP_NN, ParsersAttributes::RELATIONSHIP_GEN,
																ParsersAttributes::RELATIONSHIP_DEP };

	Ui_RelationshipConfigWidget::setupUi(this);

	SyntaxHighlighter *pattern_hl=nullptr;
	QList<QTextEdit *> pattern_fields={ src_col_pattern_txt, dst_col_pattern_txt,
																			src_fk_pattern_txt, dst_fk_pattern_txt,
                                      pk_pattern_txt, uq_pattern_txt, pk_col_pattern_txt };

	for(int i=0; i < pattern_fields.size(); i++)
	{
		pattern_hl=new SyntaxHighlighter(pattern_fields[i], true, true);
		pattern_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																	GlobalAttributes::DIR_SEPARATOR +
																	GlobalAttributes::PATTERN_HIGHLIGHT_CONF +
																	GlobalAttributes::CONFIGURATION_EXT);

		connect(pattern_fields[i], SIGNAL(textChanged()), this, SLOT(updatePattern()));
	}

  fk_to_pk_ht=new HintTextWidget(fk_to_pk_hint, this);
  fk_to_pk_ht->setText(fk_to_pk_chk->statusTip());

  center_pnts_ht=new HintTextWidget(center_pnts_hint, this);
  center_pnts_ht->setText(center_pnts_chk->statusTip());

  DeferralType::getTypes(list);
  deferral_cmb->addItems(list);

  ActionType::getTypes(list);
  list.push_front(trUtf8("Default"));
  del_action_cmb->addItems(list);
  upd_action_cmb->addItems(list);

  for(int i=0; i < rel_types.size(); i++)
    rel_type_cmb->setItemData(i, rel_types[i]);

	connect(fk_to_pk_chk, SIGNAL(toggled(bool)), conn_cnt_pnts_lbl, SLOT(setDisabled(bool)));
  connect(fk_to_pk_chk, SIGNAL(toggled(bool)), this, SLOT(setConfigurationChanged(bool)));

	connect(center_pnts_chk, SIGNAL(toggled(bool)), conn_fk_pk_lbl, SLOT(setDisabled(bool)));
  connect(center_pnts_chk, SIGNAL(toggled(bool)), this, SLOT(setConfigurationChanged(bool)));

	connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_lbl, SLOT(setEnabled(bool)));
	connect(deferrable_chk, SIGNAL(toggled(bool)), deferral_cmb, SLOT(setEnabled(bool)));
  connect(deferrable_chk, SIGNAL(toggled(bool)), this, SLOT(setConfigurationChanged(bool)));

	connect(rel_type_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(fillNamePatterns()));  
  connect(del_action_cmb, &QComboBox::currentTextChanged, [=](){ setConfigurationChanged(true); });
  connect(upd_action_cmb, &QComboBox::currentTextChanged, [=](){ setConfigurationChanged(true); });
  connect(deferral_cmb, &QComboBox::currentTextChanged, [=](){ setConfigurationChanged(true); });
}

map<QString, attribs_map> RelationshipConfigWidget::getConfigurationParams(void)
{
  return(config_params);
}

void RelationshipConfigWidget::loadConfiguration(void)
{
	try
	{
		int idx;
		vector<QString> key_attribs={ParsersAttributes::TYPE};
    BaseConfigWidget::loadConfiguration(GlobalAttributes::RELATIONSHIPS_CONF, config_params, key_attribs);

		fk_to_pk_chk->setChecked(config_params[ParsersAttributes::CONNECTION][ParsersAttributes::MODE]==ParsersAttributes::CONNECT_FK_TO_PK);
		center_pnts_chk->setChecked(config_params[ParsersAttributes::CONNECTION][ParsersAttributes::MODE]==ParsersAttributes::CONNECT_CENTER_PNTS);

		deferrable_chk->setChecked(config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEFERRABLE]==ParsersAttributes::_TRUE_);
		deferral_cmb->setCurrentText(config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEFER_TYPE]);

		idx=upd_action_cmb->findText(config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::UPD_ACTION]);
		upd_action_cmb->setCurrentIndex(idx < 0 ? 0 : idx);

    idx=del_action_cmb->findText(config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEL_ACTION]);
		del_action_cmb->setCurrentIndex(idx < 0 ? 0 : idx);

		patterns[ParsersAttributes::RELATIONSHIP_11]=config_params[ParsersAttributes::RELATIONSHIP_11];
		patterns[ParsersAttributes::RELATIONSHIP_1N]=config_params[ParsersAttributes::RELATIONSHIP_1N];
		patterns[ParsersAttributes::RELATIONSHIP_NN]=config_params[ParsersAttributes::RELATIONSHIP_NN];
		patterns[ParsersAttributes::RELATIONSHIP_GEN]=config_params[ParsersAttributes::RELATIONSHIP_GEN];
		patterns[ParsersAttributes::RELATIONSHIP_DEP]=config_params[ParsersAttributes::RELATIONSHIP_DEP];

		fillNamePatterns();
		this->applyConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipConfigWidget::saveConfiguration(void)
{  
	try
	{
		QString patterns_sch, root_dir;

		root_dir=GlobalAttributes::CONFIGURATIONS_DIR +
						 GlobalAttributes::DIR_SEPARATOR;

		patterns_sch=root_dir +
						 GlobalAttributes::SCHEMAS_DIR +
						 GlobalAttributes::DIR_SEPARATOR +
						 ParsersAttributes::PATTERNS +
						 GlobalAttributes::SCHEMA_EXT;


		config_params[ParsersAttributes::CONNECTION][ParsersAttributes::MODE]=(fk_to_pk_chk->isChecked() ? ParsersAttributes::CONNECT_FK_TO_PK : ParsersAttributes::CONNECT_CENTER_PNTS);

		config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEFERRABLE]=(deferrable_chk->isChecked() ? ParsersAttributes::_TRUE_ : ParsersAttributes::_FALSE_);
		config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEFER_TYPE]=deferral_cmb->currentText();
		config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::UPD_ACTION]=(upd_action_cmb->currentIndex() > 0 ? upd_action_cmb->currentText() : QString());
		config_params[ParsersAttributes::FOREIGN_KEYS][ParsersAttributes::DEL_ACTION]=(del_action_cmb->currentIndex() > 0 ? del_action_cmb->currentText() : QString());

		config_params[ParsersAttributes::NAME_PATTERNS][ParsersAttributes::PATTERNS]=QString();

		for(auto itr : patterns)
		{
			schparser.ignoreUnkownAttributes(true);
			schparser.ignoreEmptyAttributes(true);
			config_params[itr.first]=itr.second;
			config_params[ParsersAttributes::NAME_PATTERNS][ParsersAttributes::PATTERNS]+=schparser.getCodeDefinition(patterns_sch, itr.second);
		}

    BaseConfigWidget::saveConfiguration(GlobalAttributes::RELATIONSHIPS_CONF, config_params);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipConfigWidget::applyConfiguration(void)
{
	if(fk_to_pk_chk->isChecked())
		RelationshipView::setLineConnectionMode(RelationshipView::CONNECT_FK_TO_PK);
	else
		RelationshipView::setLineConnectionMode(RelationshipView::CONNECT_CENTER_PNTS);
}

void RelationshipConfigWidget::restoreDefaults(void)
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::RELATIONSHIPS_CONF);
		this->loadConfiguration();
    setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void RelationshipConfigWidget::fillNamePatterns(void)
{
	QString rel_type=rel_type_cmb->currentData().toString();
	bool relnn=false, reldep=false, relgen=false;
	QList<QTextEdit *> inputs={ pk_pattern_txt, uq_pattern_txt, src_col_pattern_txt,
                              dst_col_pattern_txt, src_fk_pattern_txt, dst_fk_pattern_txt,
                              pk_col_pattern_txt };

	 QList<QString> pattern_ids={ ParsersAttributes::PK_PATTERN,  ParsersAttributes::UQ_PATTERN,
																ParsersAttributes::SRC_COL_PATTERN, ParsersAttributes::DST_COL_PATTERN,
                                ParsersAttributes::SRC_FK_PATTERN, ParsersAttributes::DST_FK_PATTERN,
                                ParsersAttributes::PK_COL_PATTERN };

	relnn=(rel_type==ParsersAttributes::RELATIONSHIP_NN);
	reldep=(rel_type==ParsersAttributes::RELATIONSHIP_DEP);
	relgen=(rel_type==ParsersAttributes::RELATIONSHIP_GEN);

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

void RelationshipConfigWidget::updatePattern(void)
{
	QTextEdit *input=qobject_cast<QTextEdit *>(sender());
	QString rel_type=rel_type_cmb->currentData().toString();
	map<QTextEdit *, QString> inputs_map={ { pk_pattern_txt, ParsersAttributes::PK_PATTERN },
																				 { uq_pattern_txt, ParsersAttributes::UQ_PATTERN },
																				 { src_col_pattern_txt, ParsersAttributes::SRC_COL_PATTERN },
																				 { dst_col_pattern_txt, ParsersAttributes::DST_COL_PATTERN },
                                         { src_fk_pattern_txt, ParsersAttributes::SRC_FK_PATTERN   },
                                         { dst_fk_pattern_txt, ParsersAttributes::DST_FK_PATTERN   },
                                         { pk_col_pattern_txt, ParsersAttributes::PK_COL_PATTERN   } };


  setConfigurationChanged(true);
	patterns[rel_type][inputs_map[input]]=input->toPlainText();
}

void RelationshipConfigWidget::hideEvent(QHideEvent *)
{
  settings_twg->setCurrentIndex(0);
}
