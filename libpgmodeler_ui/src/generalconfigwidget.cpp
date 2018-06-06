/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "generalconfigwidget.h"
#include "objectsscene.h"
#include "modelwidget.h"
#include "operationlist.h"
#include "syntaxhighlighter.h"
#include "mainwindow.h"
#include "numberedtexteditor.h"
#include "linenumberswidget.h"
#include "sqlexecutionwidget.h"

map<QString, attribs_map> GeneralConfigWidget::config_params;

GeneralConfigWidget::GeneralConfigWidget(QWidget * parent) : BaseConfigWidget(parent)
{
	QPrinter::PaperSize paper_ids[]={QPrinter::A0, QPrinter::A1, QPrinter::A2, QPrinter::A3, QPrinter::A4, QPrinter::A5,
									 QPrinter::A6, QPrinter::A7, QPrinter::A8, QPrinter::A9, QPrinter::B0, QPrinter::B1,
									 QPrinter::B10, QPrinter::B2, QPrinter::B3, QPrinter::B4, QPrinter::B5, QPrinter::B6,
									 QPrinter::B7, QPrinter::B8, QPrinter::B9, QPrinter::C5E, QPrinter::Comm10E, QPrinter::DLE,
									 QPrinter::Executive, QPrinter::Folio, QPrinter::Ledger, QPrinter::Legal, QPrinter::Letter,
									 QPrinter::Tabloid, QPrinter::Custom };
	int count=sizeof(paper_ids)/sizeof(QPrinter::PaperSize);

	Ui_GeneralConfigWidget::setupUi(this);

	line_numbers_cp=new ColorPickerWidget(1, this);
	line_numbers_cp->setButtonToolTip(0, trUtf8("Line numbers' font color"));

	line_numbers_bg_cp=new ColorPickerWidget(1, this);
	line_numbers_bg_cp->setButtonToolTip(0, trUtf8("Line numbers' background color"));

	line_highlight_cp=new ColorPickerWidget(1, this);
	line_highlight_cp->setButtonToolTip(0, trUtf8("Highlighted line color"));

	font_preview_txt=new NumberedTextEditor(this);
	font_preview_txt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	font_preview_txt->setPlainText(trUtf8("The little brown fox jumps over the lazy dog") + QString("\n\ttext with tab «") + QString("\n0123456789\n.()[]{};"));

	QBoxLayout *layout=new QBoxLayout(QBoxLayout::LeftToRight);
	QGridLayout *grid=dynamic_cast<QGridLayout *>(code_font_gb->layout());
	layout->addWidget(line_numbers_cp);
	layout->addWidget(line_numbers_bg_cp);
	layout->addWidget(line_highlight_cp);
	layout->addItem(new QSpacerItem(1000,20, QSizePolicy::Expanding));
	grid->addLayout(layout, 2, 1);
	grid->addWidget(font_preview_txt,grid->count(),0,1,5);

	for(int i=0; i < count; i++)
		paper_cmb->setItemData(i, QVariant(paper_ids[i]));

	connect(unity_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(convertMarginUnity(void)));
	connect(autosave_interv_chk, SIGNAL(toggled(bool)), autosave_interv_spb, SLOT(setEnabled(bool)));
	connect(paper_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPaperSize(void)));
	connect(font_size_spb, SIGNAL(valueChanged(double)), this, SLOT(updateFontPreview()));
	connect(font_cmb, SIGNAL(currentFontChanged(QFont)), this, SLOT(updateFontPreview()));

	connect(line_numbers_cp, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(updateFontPreview()));
	connect(line_numbers_cp, SIGNAL(s_colorsChanged(void)), this, SLOT(updateFontPreview()));

	connect(line_numbers_bg_cp, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(updateFontPreview()));
	connect(line_numbers_bg_cp, SIGNAL(s_colorsChanged(void)), this, SLOT(updateFontPreview()));

	connect(line_highlight_cp, SIGNAL(s_colorChanged(unsigned, QColor)), this, SLOT(updateFontPreview()));
	connect(line_highlight_cp, SIGNAL(s_colorsChanged(void)), this, SLOT(updateFontPreview()));

	connect(disp_line_numbers_chk, SIGNAL(toggled(bool)), this, SLOT(updateFontPreview()));
	connect(hightlight_lines_chk, SIGNAL(toggled(bool)), this, SLOT(updateFontPreview()));
	connect(tab_width_spb, SIGNAL(valueChanged(int)), this, SLOT(updateFontPreview()));
	connect(tab_width_chk, SIGNAL(toggled(bool)), tab_width_spb, SLOT(setEnabled(bool)));
	connect(tab_width_chk, SIGNAL(toggled(bool)), this, SLOT(updateFontPreview()));

	connect(font_preview_txt, SIGNAL(cursorPositionChanged()), this, SLOT(updateFontPreview()));
	connect(select_editor_btn, SIGNAL(clicked(bool)), this, SLOT(selectSourceEditor()));

	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_CUSTOM_SIZE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::RECENT_MODELS]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_REL_NAME]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_EXT_ATTRIBS]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_TABLE_TAGS]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::FILE_ASSOCIATED]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT_SIZE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CANVAS_CORNER_MOVE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::INVERT_RANGESEL_TRIGGER]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CHECK_UPDATE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_LAST_POSITION]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_MAIN_MENU]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISABLE_SMOOTHNESS]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SIMPLIFIED_OBJ_CREATION]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CONFIRM_VALIDATION]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_MAIN_MENU]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_COMPLETION]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISPLAY_LINE_NUMBERS]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_NUMBERS_COLOR]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_NUMBERS_BG_COLOR]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_HIGHLIGHT_COLOR]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIGHLIGHT_LINES]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::USE_PLACEHOLDERS]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::MIN_OBJECT_OPACITY]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HISTORY_MAX_LENGTH]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SOURCE_EDITOR_APP]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::UI_LANGUAGE]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::USE_CURVED_LINES]=QString();

	simp_obj_creation_ht=new HintTextWidget(simp_obj_creation_hint, this);
	simp_obj_creation_ht->setText(simple_obj_creation_chk->statusTip());

	confirm_validation_ht=new HintTextWidget(confirm_validation_hint, this);
	confirm_validation_ht->setText(confirm_validation_chk->statusTip());

	corner_move_ht=new HintTextWidget(corner_move_hint, this);
	corner_move_ht->setText(corner_move_chk->statusTip());

	save_last_pos_ht=new HintTextWidget(save_last_pos_hint, this);
	save_last_pos_ht->setText(save_last_pos_chk->statusTip());

	invert_rangesel_ht=new HintTextWidget(invert_rangesel_hint, this);
	invert_rangesel_ht->setText(invert_rangesel_chk->statusTip());

	disable_smooth_ht=new HintTextWidget(disable_smooth_hint, this);
	disable_smooth_ht->setText(disable_smooth_chk->statusTip());

	hide_ext_attribs_ht=new HintTextWidget(hide_ext_attribs_hint, this);
	hide_ext_attribs_ht->setText(hide_ext_attribs_chk->statusTip());

	hide_table_tags_ht=new HintTextWidget(hide_table_tags_hint, this);
	hide_table_tags_ht->setText(hide_table_tags_chk->statusTip());

	hide_rel_name_ht=new HintTextWidget(hide_rel_name_hint, this);
	hide_rel_name_ht->setText(hide_rel_name_chk->statusTip());

	code_completion_ht=new HintTextWidget(code_completion_hint, this);
	code_completion_ht->setText(code_completion_chk->statusTip());

	use_placeholders_ht=new HintTextWidget(use_placeholders_hint, this);
	use_placeholders_ht->setText(use_placeholders_chk->statusTip());

	min_obj_opacity_ht=new HintTextWidget(min_obj_opacity_hint, this);
	min_obj_opacity_ht->setText(min_obj_opacity_spb->statusTip());

	autosave_ht=new HintTextWidget(autosave_hint, this);
	autosave_ht->setText(autosave_interv_chk->statusTip());

	op_history_ht=new HintTextWidget(op_history_hint, this);
	op_history_ht->setText(oplist_size_spb->statusTip());

	ui_language_ht=new HintTextWidget(ui_language_hint, this);
	ui_language_ht->setText(ui_language_cmb->statusTip());

	grid_size_ht=new HintTextWidget(grid_size_hint, this);
	grid_size_ht->setText(grid_size_spb->statusTip());

	use_curved_lines_ht=new HintTextWidget(use_curved_lines_hint, this);
	use_curved_lines_ht->setText(use_curved_lines_chk->statusTip());

	selectPaperSize();

	QList<QCheckBox *> chk_boxes=this->findChildren<QCheckBox *>();
	QList<QSpinBox *> spin_boxes=this->findChildren<QSpinBox *>();
	QList<QDoubleSpinBox *> dspin_boxes=this->findChildren<QDoubleSpinBox *>();
	QList<QComboBox *> combos=this->findChildren<QComboBox *>();
	QList<QRadioButton *> radios=this->findChildren<QRadioButton *>();

	for(QCheckBox *chk : chk_boxes)
	{
		child_wgts.push_back(chk);
		connect(chk, SIGNAL(clicked()), this, SLOT(setConfigurationChanged()));
	}

	for(QSpinBox *spin : spin_boxes)
	{
		child_wgts.push_back(spin);
		connect(spin, SIGNAL(valueChanged(QString)), this, SLOT(setConfigurationChanged()));
	}

	for(QDoubleSpinBox *dspin : dspin_boxes)
	{
		child_wgts.push_back(dspin);
		connect(dspin, SIGNAL(valueChanged(QString)), this, SLOT(setConfigurationChanged()));
	}

	for(QComboBox *cmb : combos)
	{
		child_wgts.push_back(cmb);
		connect(cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setConfigurationChanged()));
	}

	for(QRadioButton *radio : radios)
	{
		child_wgts.push_back(radio);
		connect(radio, SIGNAL(clicked()), this, SLOT(setConfigurationChanged()));
	}

	confs_dir_edt->setText(GlobalAttributes::CONFIGURATIONS_DIR);

	connect(open_dir_tb, &QToolButton::clicked, [&](){
		QDesktopServices::openUrl(QUrl(QString("file://") + confs_dir_edt->text()));
	});

	connect(clear_sql_history_tb, &QToolButton::clicked, [](){
		SQLExecutionWidget::destroySQLHistory();
	});

#ifdef NO_UPDATE_CHECK
	check_upd_chk->setChecked(false);
	check_upd_chk->setVisible(false);
#endif

	//Retrieving the available UI dictionaries
	QStringList langs = QDir(GlobalAttributes::LANGUAGES_DIR +
													 GlobalAttributes::DIR_SEPARATOR,
													 QString("*.qm"), QDir::Name, QDir::AllEntries | QDir::NoDotAndDotDot).entryList();

	langs.replaceInStrings(QString(".qm"), QString());
	ui_language_cmb->addItem(trUtf8("System default"));
	QString native_lang;

	for(QString lang : langs)
	{
		native_lang = QLocale(lang).nativeLanguageName();
		native_lang[0] = native_lang[0].toUpper();
		ui_language_cmb->addItem(QString("%1 (%2 : %3)")
														 .arg(native_lang)
														 .arg(QLocale::languageToString(QLocale(lang).language()))
														 .arg(lang), lang);
	}
}

void GeneralConfigWidget::loadConfiguration(void)
{
	try
	{
		QStringList margin, custom_size;
		vector<QString> key_attribs;
		unsigned interv=0;
		int tab_width=0;

		for(QWidget *wgt : child_wgts)
			wgt->blockSignals(true);

		key_attribs.push_back(ParsersAttributes::ID);
		BaseConfigWidget::loadConfiguration(GlobalAttributes::GENERAL_CONF, config_params, key_attribs);

		grid_size_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]).toUInt());
		oplist_size_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]).toUInt());
		history_max_length_spb->setValue(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HISTORY_MAX_LENGTH].toUInt());

		interv=(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]).toUInt();
		tab_width=(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_TAB_WIDTH]).toInt();

		autosave_interv_chk->setChecked(interv > 0);
		autosave_interv_spb->setValue(interv);
		autosave_interv_spb->setEnabled(autosave_interv_chk->isChecked());

		tab_width_chk->setChecked(tab_width > 0);
		tab_width_spb->setEnabled(tab_width_chk->isChecked());
		tab_width_spb->setValue(tab_width);

		corner_move_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CANVAS_CORNER_MOVE]==ParsersAttributes::_TRUE_);
		invert_rangesel_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::INVERT_RANGESEL_TRIGGER]==ParsersAttributes::_TRUE_);
		check_upd_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CHECK_UPDATE]==ParsersAttributes::_TRUE_);
		save_last_pos_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_LAST_POSITION]==ParsersAttributes::_TRUE_);
		disable_smooth_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISABLE_SMOOTHNESS]==ParsersAttributes::_TRUE_);
		simple_obj_creation_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SIMPLIFIED_OBJ_CREATION]==ParsersAttributes::_TRUE_);
		confirm_validation_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CONFIRM_VALIDATION]==ParsersAttributes::_TRUE_);
		code_completion_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_COMPLETION]==ParsersAttributes::_TRUE_);
		use_placeholders_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::USE_PLACEHOLDERS]==ParsersAttributes::_TRUE_);
		use_curved_lines_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::USE_CURVED_LINES]==ParsersAttributes::_TRUE_);

		print_grid_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]==ParsersAttributes::_TRUE_);
		print_pg_num_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]==ParsersAttributes::_TRUE_);

		paper_cmb->setCurrentIndex((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]).toUInt());
		portrait_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::PORTRAIT);
		landscape_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::LANDSCAPE);

		min_obj_opacity_spb->setValue(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::MIN_OBJECT_OPACITY].toUInt());

		margin=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN].split(',');
		custom_size=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_CUSTOM_SIZE].split(',');

		left_marg->setValue((margin.count() >= 4 ? margin[0].toDouble() : 2));
		top_marg->setValue((margin.count()>= 4 ? margin[1].toDouble() : 2));
		right_marg->setValue((margin.count() >= 4 ? margin[2].toDouble() : 2));
		bottom_marg->setValue((margin.count() >= 4 ? margin[3].toDouble() : 2));

		width_spb->setValue((custom_size.count() >= 2 ? custom_size[0].toDouble() : 500));
		height_spb->setValue((custom_size.count() >= 2 ? custom_size[1].toDouble() : 500));

		hide_ext_attribs_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_EXT_ATTRIBS]==ParsersAttributes::_TRUE_);
		hide_rel_name_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_REL_NAME]==ParsersAttributes::_TRUE_);
		hide_table_tags_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_TABLE_TAGS]==ParsersAttributes::_TRUE_);

		font_cmb->setCurrentFont(QFont(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT]));
		font_size_spb->setValue(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT_SIZE].toDouble());
		disp_line_numbers_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISPLAY_LINE_NUMBERS]==ParsersAttributes::_TRUE_);
		hightlight_lines_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIGHLIGHT_LINES]==ParsersAttributes::_TRUE_);
		line_numbers_cp->setColor(0, config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_NUMBERS_COLOR]);
		line_numbers_bg_cp->setColor(0, config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_NUMBERS_BG_COLOR]);
		line_highlight_cp->setColor(0, config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_HIGHLIGHT_COLOR]);

		source_editor_edt->setText(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SOURCE_EDITOR_APP]);
		source_editor_args_edt->setText(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SOURCE_EDITOR_ARGS]);

		int ui_idx = ui_language_cmb->findData(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::UI_LANGUAGE]);
		ui_language_cmb->setCurrentIndex(ui_idx >= 0 ? ui_idx : 0);

		for(QWidget *wgt : child_wgts)
			wgt->blockSignals(false);

		updateFontPreview();
		this->applyConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e, e.getExtraInfo());
	}
}

void GeneralConfigWidget::addConfigurationParam(const QString &param, const attribs_map &attribs)
{
	BaseConfigWidget::addConfigurationParam(config_params, param, attribs);
}

void GeneralConfigWidget::removeConfigurationParam(const QRegExp &param_reg)
{
	map<QString, attribs_map>::iterator itr, itr_end;

	itr=config_params.begin();
	itr_end=config_params.end();

	while(itr!=itr_end)
	{
		if(param_reg.exactMatch(itr->first))
		{
			config_params.erase(itr);
			itr=config_params.begin();
			itr_end=config_params.end();
		}

		itr++;
	}
}

map<QString, attribs_map> GeneralConfigWidget::getConfigurationParams(void)
{
	return(config_params);
}

QString GeneralConfigWidget::getConfigurationParam(const QString &section_id, const QString &param_name)
{
	if(config_params.count(section_id) &&
			config_params[section_id].count(param_name))
		return(config_params[section_id][param_name]);
	else
		return(QString());
}

void GeneralConfigWidget::saveConfiguration(void)
{
	try
	{
		map<QString, attribs_map >::iterator itr, itr_end;
		QString file_sch, root_dir, widget_sch;
		bool show_grid=false, show_delim=false, align_grid=false;

		root_dir=GlobalAttributes::TMPL_CONFIGURATIONS_DIR +
				 GlobalAttributes::DIR_SEPARATOR;

		file_sch=root_dir +
				 GlobalAttributes::SCHEMAS_DIR +
				 GlobalAttributes::DIR_SEPARATOR +
				 ParsersAttributes::_FILE_ +
				 GlobalAttributes::SCHEMA_EXT;

		widget_sch=root_dir +
				   GlobalAttributes::SCHEMAS_DIR +
				   GlobalAttributes::DIR_SEPARATOR +
				   ParsersAttributes::WIDGET +
				   GlobalAttributes::SCHEMA_EXT;

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]=QString::number(grid_size_spb->value());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]=QString::number(oplist_size_spb->value());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]=QString::number(autosave_interv_chk->isChecked() ? autosave_interv_spb->value() : 0);
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]=QString::number(paper_cmb->currentIndex());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]=(portrait_rb->isChecked() ? ParsersAttributes::PORTRAIT : ParsersAttributes::LANDSCAPE);
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CANVAS_CORNER_MOVE]=(corner_move_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::INVERT_RANGESEL_TRIGGER]=(invert_rangesel_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CHECK_UPDATE]=(check_upd_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_LAST_POSITION]=(save_last_pos_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISABLE_SMOOTHNESS]=(disable_smooth_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SIMPLIFIED_OBJ_CREATION]=(simple_obj_creation_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CONFIRM_VALIDATION]=(confirm_validation_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_COMPLETION]=(code_completion_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_TAB_WIDTH]=QString::number(tab_width_chk->isChecked() ? tab_width_spb->value() : 0);
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::MIN_OBJECT_OPACITY]=QString::number(min_obj_opacity_spb->value());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::USE_PLACEHOLDERS]=(use_placeholders_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HISTORY_MAX_LENGTH]=QString::number(history_max_length_spb->value());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::USE_CURVED_LINES]=(use_curved_lines_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());

		ObjectsScene::getGridOptions(show_grid, align_grid, show_delim);
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_CANVAS_GRID]=(show_grid ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_PAGE_DELIMITERS]=(show_delim ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::ALIGN_OBJS_TO_GRID]=(align_grid ? ParsersAttributes::_TRUE_ : QString());

		unity_cmb->setCurrentIndex(UNIT_MILIMETERS);
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN]=QString("%1,%2,%3,%4").arg(left_marg->value())
																						 .arg(top_marg->value())
																						 .arg(right_marg->value())
																						 .arg(bottom_marg->value());

		if(paper_cmb->currentIndex()!=paper_cmb->count()-1)
			config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_CUSTOM_SIZE]=QString();
		else
			config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_CUSTOM_SIZE]=QString("%1,%2").arg(width_spb->value()).arg(height_spb->value());

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]=(print_pg_num_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]=(print_grid_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_EXT_ATTRIBS]=(hide_ext_attribs_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_REL_NAME]=(hide_rel_name_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_TABLE_TAGS]=(hide_table_tags_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT]=font_cmb->currentText();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT_SIZE]=QString::number(font_size_spb->value());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISPLAY_LINE_NUMBERS]=(disp_line_numbers_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIGHLIGHT_LINES]=(hightlight_lines_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_NUMBERS_COLOR]=line_numbers_cp->getColor(0).name();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_NUMBERS_BG_COLOR]=line_numbers_bg_cp->getColor(0).name();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::LINE_HIGHLIGHT_COLOR]=line_highlight_cp->getColor(0).name();

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SOURCE_EDITOR_APP]=source_editor_edt->text();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SOURCE_EDITOR_ARGS]=source_editor_args_edt->text();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::UI_LANGUAGE]=ui_language_cmb->currentData().toString();

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::COMPACT_VIEW]=(BaseObjectView::isCompactViewEnabled() ? ParsersAttributes::_TRUE_ : QString());

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]=QString();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::RECENT_MODELS]=QString();

		itr=config_params.begin();
		itr_end=config_params.end();

		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DOCK_WIDGETS]=QString();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::RECENT_MODELS]=QString();
		config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]=QString();

		while(itr!=itr_end)
		{
			//Checking if the current attribute is a file to be stored in a <session> tag
			if((itr->first).contains(QRegExp(QString("(") + ParsersAttributes::_FILE_ + QString(")([0-9]+)"))))
			{
				config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::_FILE_]+=
						schparser.convertCharsToXMLEntities(schparser.getCodeDefinition(file_sch, itr->second));
			}
			//Checking if the current attribute is a file to be stored in a <recent-models> tag
			else if((itr->first).contains(QRegExp(QString("(") + ParsersAttributes::RECENT + QString(")([0-9]+)"))))
			{
				config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::RECENT_MODELS]+=
						schparser.convertCharsToXMLEntities(schparser.getCodeDefinition(file_sch, itr->second));
			}
			else if(itr->first==ParsersAttributes::VALIDATOR ||
							itr->first==ParsersAttributes::OBJECT_FINDER ||
							itr->first==ParsersAttributes::SQL_TOOL)
			{
				schparser.ignoreUnkownAttributes(true);
				config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DOCK_WIDGETS]+=
						schparser.getCodeDefinition(widget_sch, itr->second);
				schparser.ignoreUnkownAttributes(false);
			}

			itr++;
		}

		BaseConfigWidget::saveConfiguration(GlobalAttributes::GENERAL_CONF, config_params);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void GeneralConfigWidget::applyConfiguration(void)
{
	int unit=unity_cmb->currentIndex();
	QFont fnt;
	double fnt_size=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT_SIZE].toDouble();

	if(fnt_size < 5.0f)
		fnt_size=5.0f;

	unity_cmb->setCurrentIndex(UNIT_POINT);
	ObjectsScene::setPaperConfiguration(static_cast<QPrinter::PaperSize>(paper_cmb->itemData(paper_cmb->currentIndex()).toInt()),
										(portrait_rb->isChecked() ? QPrinter::Portrait : QPrinter::Landscape),
										QRectF(left_marg->value(), top_marg->value(), right_marg->value(), bottom_marg->value()),
										QSizeF(width_spb->value(), height_spb->value()));
	unity_cmb->setCurrentIndex(unit);

	ObjectsScene::setEnableCornerMove(corner_move_chk->isChecked());
	ObjectsScene::setInvertRangeSelectionTrigger(invert_rangesel_chk->isChecked());
	ObjectsScene::setGridSize(grid_size_spb->value());

	ObjectsScene::setGridOptions(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_CANVAS_GRID]==ParsersAttributes::_TRUE_,
															 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::ALIGN_OBJS_TO_GRID]==ParsersAttributes::_TRUE_,
															 config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_PAGE_DELIMITERS]==ParsersAttributes::_TRUE_);

	OperationList::setMaximumSize(oplist_size_spb->value());
	BaseTableView::setHideExtAttributes(hide_ext_attribs_chk->isChecked());
	BaseTableView::setHideTags(hide_table_tags_chk->isChecked());
	RelationshipView::setHideNameLabel(hide_rel_name_chk->isChecked());
	RelationshipView::setCurvedLines(use_curved_lines_chk->isChecked());
	ModelWidget::setSaveLastCanvasPosition(save_last_pos_chk->isChecked());
	ModelWidget::setRenderSmoothnessDisabled(disable_smooth_chk->isChecked());
	ModelWidget::setSimplifiedObjectCreation(simple_obj_creation_chk->isChecked());
	ModelWidget::setMinimumObjectOpacity(min_obj_opacity_spb->value());
	MainWindow::setConfirmValidation(confirm_validation_chk->isChecked());
	BaseObjectView::setPlaceholderEnabled(use_placeholders_chk->isChecked());
	SQLExecutionWidget::setSQLHistoryMaxLength(history_max_length_spb->value());

	fnt.setFamily(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT]);
	fnt.setPointSize(fnt_size);
	NumberedTextEditor::setLineNumbersVisible(disp_line_numbers_chk->isChecked());
	NumberedTextEditor::setLineHighlightColor(line_highlight_cp->getColor(0));
	NumberedTextEditor::setHighlightLines(hightlight_lines_chk->isChecked());
	NumberedTextEditor::setDefaultFont(fnt);
	NumberedTextEditor::setSourceEditorApp(source_editor_edt->text());
	NumberedTextEditor::setSourceEditorAppArgs(source_editor_args_edt->text());
	LineNumbersWidget::setColors(line_numbers_cp->getColor(0), line_numbers_bg_cp->getColor(0));
	SyntaxHighlighter::setDefaultFont(fnt);
}

void GeneralConfigWidget::restoreDefaults(void)
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::GENERAL_CONF, false);
		BaseConfigWidget::restoreDefaults(GlobalAttributes::XML_HIGHLIGHT_CONF, true);
		BaseConfigWidget::restoreDefaults(GlobalAttributes::SQL_HIGHLIGHT_CONF, true);
		BaseConfigWidget::restoreDefaults(GlobalAttributes::UI_STYLE_CONF, true);
		this->loadConfiguration();
		this->applyConfiguration();
		setConfigurationChanged(true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void GeneralConfigWidget::convertMarginUnity(void)
{
	static int prev_unity=UNIT_MILIMETERS;
	double conv_factor[]={1.0f, 2.83f, 0.04f, 0.1f},
			left, right, top, bottom, width, height;

	left=left_marg->value() / conv_factor[prev_unity];
	right=right_marg->value() / conv_factor[prev_unity];
	bottom=bottom_marg->value() / conv_factor[prev_unity];
	top=top_marg->value() / conv_factor[prev_unity];
	width=width_spb->value() / conv_factor[prev_unity];
	height=height_spb->value() / conv_factor[prev_unity];

	left_marg->setValue(left * conv_factor[unity_cmb->currentIndex()]);
	right_marg->setValue(right * conv_factor[unity_cmb->currentIndex()]);
	bottom_marg->setValue(bottom * conv_factor[unity_cmb->currentIndex()]);
	top_marg->setValue(top * conv_factor[unity_cmb->currentIndex()]);
	width_spb->setValue(width * conv_factor[unity_cmb->currentIndex()]);
	height_spb->setValue(height * conv_factor[unity_cmb->currentIndex()]);

	prev_unity=unity_cmb->currentIndex();
}

void GeneralConfigWidget::updateFontPreview(void)
{
	QFont fnt;

	fnt=font_cmb->currentFont();
	fnt.setPointSizeF(font_size_spb->value());

	NumberedTextEditor::setDefaultFont(fnt);
	NumberedTextEditor::setLineNumbersVisible(disp_line_numbers_chk->isChecked());
	NumberedTextEditor::setLineHighlightColor(line_highlight_cp->getColor(0));
	NumberedTextEditor::setHighlightLines(hightlight_lines_chk->isChecked());
	NumberedTextEditor::setTabWidth(tab_width_chk->isChecked() ? tab_width_spb->value() : 0);
	LineNumbersWidget::setColors(line_numbers_cp->getColor(0), line_numbers_bg_cp->getColor(0));

	font_preview_txt->setReadOnly(false);
	font_preview_txt->updateLineNumbersSize();
	font_preview_txt->updateLineNumbers();
	font_preview_txt->highlightCurrentLine();
	font_preview_txt->setReadOnly(true);

	setConfigurationChanged(true);
}

void GeneralConfigWidget::selectPaperSize(void)
{
	bool visible=paper_cmb->currentIndex()==paper_cmb->count()-1;

	custom_lbl->setVisible(visible);
	width_lbl->setVisible(visible);
	height_lbl->setVisible(visible);
	width_spb->setVisible(visible);
	height_spb->setVisible(visible);
}

void GeneralConfigWidget::selectSourceEditor(void)
{
	QFileDialog sel_editor_dlg;

	sel_editor_dlg.setFileMode(QFileDialog::ExistingFile);
	sel_editor_dlg.setNameFilter(trUtf8("All files (*.*)"));
	sel_editor_dlg.setModal(true);
	sel_editor_dlg.setWindowTitle(trUtf8("Load file"));
	sel_editor_dlg.setAcceptMode(QFileDialog::AcceptOpen);
	sel_editor_dlg.exec();

	if(sel_editor_dlg.result()==QDialog::Accepted)
		source_editor_edt->setText(sel_editor_dlg.selectedFiles().at(0));
}
