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
map<QString, GeneralConfigWidget::WidgetState> GeneralConfigWidget::widgets_geom;

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
	connect(save_restore_geometry_chk, SIGNAL(toggled(bool)), reset_sizes_tb, SLOT(setEnabled(bool)));
	connect(reset_sizes_tb, SIGNAL(clicked(bool)), this, SLOT(resetDialogsSizes()));

	config_params[Attributes::Configuration][Attributes::GridSize]=QString();
	config_params[Attributes::Configuration][Attributes::OpListSize]=QString();
	config_params[Attributes::Configuration][Attributes::AutoSaveInterval]=QString();
	config_params[Attributes::Configuration][Attributes::PaperType]=QString();
	config_params[Attributes::Configuration][Attributes::PaperOrientation]=QString();
	config_params[Attributes::Configuration][Attributes::PaperMargin]=QString();
	config_params[Attributes::Configuration][Attributes::PaperCustomSize]=QString();
	config_params[Attributes::Configuration][Attributes::File]=QString();
	config_params[Attributes::Configuration][Attributes::RecentModels]=QString();
	config_params[Attributes::Configuration][Attributes::PrintPgNum]=QString();
	config_params[Attributes::Configuration][Attributes::PrintGrid]=QString();
	config_params[Attributes::Configuration][Attributes::HideRelName]=QString();
	config_params[Attributes::Configuration][Attributes::HideExtAttribs]=QString();
	config_params[Attributes::Configuration][Attributes::HideTableTags]=QString();
	config_params[Attributes::Configuration][Attributes::FileAssociated]=QString();
	config_params[Attributes::Configuration][Attributes::CodeFont]=QString();
	config_params[Attributes::Configuration][Attributes::CodeFontSize]=QString();
	config_params[Attributes::Configuration][Attributes::CanvasCornerMove]=QString();
	config_params[Attributes::Configuration][Attributes::InvertRangeSelTrigger]=QString();
	config_params[Attributes::Configuration][Attributes::CheckUpdate]=QString();
	config_params[Attributes::Configuration][Attributes::SaveLastPosition]=QString();
	config_params[Attributes::Configuration][Attributes::ShowMainMenu]=QString();
	config_params[Attributes::Configuration][Attributes::DisableSmoothness]=QString();
	config_params[Attributes::Configuration][Attributes::SimplifiedObjCreation]=QString();
	config_params[Attributes::Configuration][Attributes::ConfirmValidation]=QString();
	config_params[Attributes::Configuration][Attributes::ShowMainMenu]=QString();
	config_params[Attributes::Configuration][Attributes::CodeCompletion]=QString();
	config_params[Attributes::Configuration][Attributes::DisplayLineNumbers]=QString();
	config_params[Attributes::Configuration][Attributes::LineNumbersColor]=QString();
	config_params[Attributes::Configuration][Attributes::LineNumbersBgColor]=QString();
	config_params[Attributes::Configuration][Attributes::LineHighlightColor]=QString();
	config_params[Attributes::Configuration][Attributes::HighlightLines]=QString();
	config_params[Attributes::Configuration][Attributes::UsePlaceholders]=QString();
	config_params[Attributes::Configuration][Attributes::MinObjectOpacity]=QString();
	config_params[Attributes::Configuration][Attributes::HistoryMaxLength]=QString();
	config_params[Attributes::Configuration][Attributes::SourceEditorApp]=QString();
	config_params[Attributes::Configuration][Attributes::UiLanguage]=QString();
	config_params[Attributes::Configuration][Attributes::UseCurvedLines]=QString();
	config_params[Attributes::Configuration][Attributes::SaveRestoreGeometry]=QString();
	config_params[Attributes::Configuration][Attributes::AttribsPerPage]=QString();
	config_params[Attributes::Configuration][Attributes::ExtAttribsPerPage]=QString();

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

	attribs_per_page_ht=new HintTextWidget(attributes_per_page_hint, this);
	attribs_per_page_ht->setText(attribs_per_page_spb->statusTip());

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

	confs_dir_edt->setText(GlobalAttributes::ConfigurationsDir);

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
	QStringList langs = QDir(GlobalAttributes::LanguagesDir +
													 GlobalAttributes::DirSeparator,
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
		int tab_width=0, x=0, y=0, w=0, h=0;

		for(QWidget *wgt : child_wgts)
			wgt->blockSignals(true);

		key_attribs.push_back(Attributes::Id);
		BaseConfigWidget::loadConfiguration(GlobalAttributes::GeneralConf, config_params, key_attribs);

		grid_size_spb->setValue((config_params[Attributes::Configuration][Attributes::GridSize]).toUInt());
		oplist_size_spb->setValue((config_params[Attributes::Configuration][Attributes::OpListSize]).toUInt());
		history_max_length_spb->setValue(config_params[Attributes::Configuration][Attributes::HistoryMaxLength].toUInt());

		interv=(config_params[Attributes::Configuration][Attributes::AutoSaveInterval]).toUInt();
		tab_width=(config_params[Attributes::Configuration][Attributes::CodeTabWidth]).toInt();

		autosave_interv_chk->setChecked(interv > 0);
		autosave_interv_spb->setValue(interv);
		autosave_interv_spb->setEnabled(autosave_interv_chk->isChecked());

		tab_width_chk->setChecked(tab_width > 0);
		tab_width_spb->setEnabled(tab_width_chk->isChecked());
		tab_width_spb->setValue(tab_width);

		corner_move_chk->setChecked(config_params[Attributes::Configuration][Attributes::CanvasCornerMove]==Attributes::True);
		invert_rangesel_chk->setChecked(config_params[Attributes::Configuration][Attributes::InvertRangeSelTrigger]==Attributes::True);
		check_upd_chk->setChecked(config_params[Attributes::Configuration][Attributes::CheckUpdate]==Attributes::True);
		save_last_pos_chk->setChecked(config_params[Attributes::Configuration][Attributes::SaveLastPosition]==Attributes::True);
		disable_smooth_chk->setChecked(config_params[Attributes::Configuration][Attributes::DisableSmoothness]==Attributes::True);
		simple_obj_creation_chk->setChecked(config_params[Attributes::Configuration][Attributes::SimplifiedObjCreation]==Attributes::True);
		confirm_validation_chk->setChecked(config_params[Attributes::Configuration][Attributes::ConfirmValidation]==Attributes::True);
		code_completion_chk->setChecked(config_params[Attributes::Configuration][Attributes::CodeCompletion]==Attributes::True);
		use_placeholders_chk->setChecked(config_params[Attributes::Configuration][Attributes::UsePlaceholders]==Attributes::True);
		use_curved_lines_chk->setChecked(config_params[Attributes::Configuration][Attributes::UseCurvedLines]==Attributes::True);

		print_grid_chk->setChecked(config_params[Attributes::Configuration][Attributes::PrintGrid]==Attributes::True);
		print_pg_num_chk->setChecked(config_params[Attributes::Configuration][Attributes::PrintPgNum]==Attributes::True);

		paper_cmb->setCurrentIndex((config_params[Attributes::Configuration][Attributes::PaperType]).toUInt());
		portrait_rb->setChecked(config_params[Attributes::Configuration][Attributes::PaperOrientation]==Attributes::Portrait);
		landscape_rb->setChecked(config_params[Attributes::Configuration][Attributes::PaperOrientation]==Attributes::Landscape);

		min_obj_opacity_spb->setValue(config_params[Attributes::Configuration][Attributes::MinObjectOpacity].toUInt());
		attribs_per_page_spb->setValue(config_params[Attributes::Configuration][Attributes::AttribsPerPage].toUInt());
		ext_attribs_per_page_spb->setValue(config_params[Attributes::Configuration][Attributes::ExtAttribsPerPage].toUInt());

		margin=config_params[Attributes::Configuration][Attributes::PaperMargin].split(',');
		custom_size=config_params[Attributes::Configuration][Attributes::PaperCustomSize].split(',');

		left_marg->setValue((margin.count() >= 4 ? margin[0].toDouble() : 2));
		top_marg->setValue((margin.count()>= 4 ? margin[1].toDouble() : 2));
		right_marg->setValue((margin.count() >= 4 ? margin[2].toDouble() : 2));
		bottom_marg->setValue((margin.count() >= 4 ? margin[3].toDouble() : 2));

		width_spb->setValue((custom_size.count() >= 2 ? custom_size[0].toDouble() : 500));
		height_spb->setValue((custom_size.count() >= 2 ? custom_size[1].toDouble() : 500));

		hide_ext_attribs_chk->setChecked(config_params[Attributes::Configuration][Attributes::HideExtAttribs]==Attributes::True);
		hide_rel_name_chk->setChecked(config_params[Attributes::Configuration][Attributes::HideRelName]==Attributes::True);
		hide_table_tags_chk->setChecked(config_params[Attributes::Configuration][Attributes::HideTableTags]==Attributes::True);

		font_cmb->setCurrentFont(QFont(config_params[Attributes::Configuration][Attributes::CodeFont]));
		font_size_spb->setValue(config_params[Attributes::Configuration][Attributes::CodeFontSize].toDouble());
		disp_line_numbers_chk->setChecked(config_params[Attributes::Configuration][Attributes::DisplayLineNumbers]==Attributes::True);
		hightlight_lines_chk->setChecked(config_params[Attributes::Configuration][Attributes::HighlightLines]==Attributes::True);
		line_numbers_cp->setColor(0, config_params[Attributes::Configuration][Attributes::LineNumbersColor]);
		line_numbers_bg_cp->setColor(0, config_params[Attributes::Configuration][Attributes::LineNumbersBgColor]);
		line_highlight_cp->setColor(0, config_params[Attributes::Configuration][Attributes::LineHighlightColor]);

		source_editor_edt->setText(config_params[Attributes::Configuration][Attributes::SourceEditorApp]);
		source_editor_args_edt->setText(config_params[Attributes::Configuration][Attributes::SourceEditorArgs]);

		save_restore_geometry_chk->setChecked(config_params[Attributes::Configuration][Attributes::SaveRestoreGeometry]==Attributes::True);
		reset_sizes_tb->setEnabled(save_restore_geometry_chk->isChecked());

		int ui_idx = ui_language_cmb->findData(config_params[Attributes::Configuration][Attributes::UiLanguage]);
		ui_language_cmb->setCurrentIndex(ui_idx >= 0 ? ui_idx : 0);

		for(QWidget *wgt : child_wgts)
			wgt->blockSignals(false);

		widgets_geom.clear();
		for(auto itr : config_params)
		{
			if(itr.second.count(Attributes::XPos))
			{
				x = itr.second[Attributes::XPos].toInt();
				y = itr.second[Attributes::YPos].toInt();
				w = itr.second[Attributes::Width].toInt();
				h = itr.second[Attributes::Height].toInt();
				widgets_geom[itr.first].geometry = QRect(QPoint(x,y), QSize(w, h));
				widgets_geom[itr.first].maximized = itr.second[Attributes::Maximized] == Attributes::True;
			}
		}

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

void GeneralConfigWidget::saveWidgetGeometry(QWidget *widget, const QString &custom_wgt_name)
{
	if(!widget ||
		 config_params[Attributes::Configuration][Attributes::SaveRestoreGeometry] != Attributes::True)
		return;

	QString dlg_name = custom_wgt_name.isEmpty() ? widget->metaObject()->className() : custom_wgt_name;

	widgets_geom[dlg_name.toLower()].geometry = widget->geometry();
	widgets_geom[dlg_name.toLower()].maximized = widget->isMaximized();
}

bool GeneralConfigWidget::restoreWidgetGeometry(QWidget *widget, const QString &custom_wgt_name)
{
	if(!widget ||
		 config_params[Attributes::Configuration][Attributes::SaveRestoreGeometry] != Attributes::True)
		return(false);

	QString dlg_name = custom_wgt_name.isEmpty() ? widget->metaObject()->className() : custom_wgt_name;
	dlg_name = dlg_name.toLower();

	if(widgets_geom.count(dlg_name) &&
		 (widgets_geom[dlg_name].maximized ||
			(widgets_geom[dlg_name].geometry.width() > 0 && widgets_geom[dlg_name].geometry.height() > 0)))
	{
		if(widgets_geom[dlg_name].maximized)
		{
			widget->move(widgets_geom[dlg_name].geometry.topLeft());
			widget->setWindowState(Qt::WindowMaximized);
		}
		else
			widget->setGeometry(widgets_geom[dlg_name].geometry);

		return(true);
	}

	return(false);
}

void GeneralConfigWidget::saveConfiguration(void)
{
	try
	{
		attribs_map attribs;
		map<QString, attribs_map >::iterator itr, itr_end;
		QString file_sch, root_dir, widget_sch;
		bool show_grid=false, show_delim=false, align_grid=false;

		root_dir=GlobalAttributes::TmplConfigurationDir +
				 GlobalAttributes::DirSeparator;

		file_sch=root_dir +
				 GlobalAttributes::SchemasDir +
				 GlobalAttributes::DirSeparator +
				 Attributes::File +
				 GlobalAttributes::SchemaExt;

		widget_sch=root_dir +
				   GlobalAttributes::SchemasDir +
				   GlobalAttributes::DirSeparator +
				   Attributes::Widget +
				   GlobalAttributes::SchemaExt;

		config_params[Attributes::Configuration][Attributes::GridSize]=QString::number(grid_size_spb->value());
		config_params[Attributes::Configuration][Attributes::OpListSize]=QString::number(oplist_size_spb->value());
		config_params[Attributes::Configuration][Attributes::AutoSaveInterval]=QString::number(autosave_interv_chk->isChecked() ? autosave_interv_spb->value() : 0);
		config_params[Attributes::Configuration][Attributes::PaperType]=QString::number(paper_cmb->currentIndex());
		config_params[Attributes::Configuration][Attributes::PaperOrientation]=(portrait_rb->isChecked() ? Attributes::Portrait : Attributes::Landscape);
		config_params[Attributes::Configuration][Attributes::CanvasCornerMove]=(corner_move_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::InvertRangeSelTrigger]=(invert_rangesel_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::CheckUpdate]=(check_upd_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::SaveLastPosition]=(save_last_pos_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::DisableSmoothness]=(disable_smooth_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::SimplifiedObjCreation]=(simple_obj_creation_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::ConfirmValidation]=(confirm_validation_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::CodeCompletion]=(code_completion_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::CodeTabWidth]=QString::number(tab_width_chk->isChecked() ? tab_width_spb->value() : 0);
		config_params[Attributes::Configuration][Attributes::MinObjectOpacity]=QString::number(min_obj_opacity_spb->value());
		config_params[Attributes::Configuration][Attributes::UsePlaceholders]=(use_placeholders_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::HistoryMaxLength]=QString::number(history_max_length_spb->value());
		config_params[Attributes::Configuration][Attributes::UseCurvedLines]=(use_curved_lines_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::AttribsPerPage]=QString::number(attribs_per_page_spb->value());
		config_params[Attributes::Configuration][Attributes::ExtAttribsPerPage]=QString::number(ext_attribs_per_page_spb->value());

		ObjectsScene::getGridOptions(show_grid, align_grid, show_delim);
		config_params[Attributes::Configuration][Attributes::ShowCanvasGrid]=(show_grid ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::ShowPageDelimiters]=(show_delim ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::AlignObjsToGrid]=(align_grid ? Attributes::True : QString());

		unity_cmb->setCurrentIndex(UnitMilimeters);
		config_params[Attributes::Configuration][Attributes::PaperMargin]=QString("%1,%2,%3,%4").arg(left_marg->value())
																						 .arg(top_marg->value())
																						 .arg(right_marg->value())
																						 .arg(bottom_marg->value());

		if(paper_cmb->currentIndex()!=paper_cmb->count()-1)
			config_params[Attributes::Configuration][Attributes::PaperCustomSize]=QString();
		else
			config_params[Attributes::Configuration][Attributes::PaperCustomSize]=QString("%1,%2").arg(width_spb->value()).arg(height_spb->value());

		config_params[Attributes::Configuration][Attributes::PrintPgNum]=(print_pg_num_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::PrintGrid]=(print_grid_chk->isChecked() ? Attributes::True : QString());

		config_params[Attributes::Configuration][Attributes::HideExtAttribs]=(hide_ext_attribs_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::HideRelName]=(hide_rel_name_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::HideTableTags]=(hide_table_tags_chk->isChecked() ? Attributes::True : QString());

		config_params[Attributes::Configuration][Attributes::CodeFont]=font_cmb->currentText();
		config_params[Attributes::Configuration][Attributes::CodeFontSize]=QString::number(font_size_spb->value());
		config_params[Attributes::Configuration][Attributes::DisplayLineNumbers]=(disp_line_numbers_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::HighlightLines]=(hightlight_lines_chk->isChecked() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::LineNumbersColor]=line_numbers_cp->getColor(0).name();
		config_params[Attributes::Configuration][Attributes::LineNumbersBgColor]=line_numbers_bg_cp->getColor(0).name();
		config_params[Attributes::Configuration][Attributes::LineHighlightColor]=line_highlight_cp->getColor(0).name();

		config_params[Attributes::Configuration][Attributes::SourceEditorApp]=source_editor_edt->text();
		config_params[Attributes::Configuration][Attributes::SourceEditorArgs]=source_editor_args_edt->text();
		config_params[Attributes::Configuration][Attributes::UiLanguage]=ui_language_cmb->currentData().toString();

		config_params[Attributes::Configuration][Attributes::CompactView]=(BaseObjectView::isCompactViewEnabled() ? Attributes::True : QString());
		config_params[Attributes::Configuration][Attributes::SaveRestoreGeometry]=(save_restore_geometry_chk->isChecked() ? Attributes::True : QString());

		config_params[Attributes::Configuration][Attributes::File]=QString();
		config_params[Attributes::Configuration][Attributes::RecentModels]=QString();

		itr=config_params.begin();
		itr_end=config_params.end();

		config_params[Attributes::Configuration][Attributes::DockWidgets]=QString();
		config_params[Attributes::Configuration][Attributes::WidgetsGeometry]=QString();
		config_params[Attributes::Configuration][Attributes::RecentModels]=QString();
		config_params[Attributes::Configuration][Attributes::File]=QString();

		while(itr!=itr_end)
		{
			//Checking if the current attribute is a file to be stored in a <session> tag
			if((itr->first).contains(QRegExp(QString("(") + Attributes::File + QString(")([0-9]+)"))))
			{
				config_params[Attributes::Configuration][Attributes::File]+=
						schparser.convertCharsToXMLEntities(schparser.getCodeDefinition(file_sch, itr->second));
			}
			//Checking if the current attribute is a file to be stored in a <recent-models> tag
			else if((itr->first).contains(QRegExp(QString("(") + Attributes::Recent + QString(")([0-9]+)"))))
			{
				config_params[Attributes::Configuration][Attributes::RecentModels]+=
						schparser.convertCharsToXMLEntities(schparser.getCodeDefinition(file_sch, itr->second));
			}
			else if(itr->first==Attributes::Validator ||
					itr->first==Attributes::ObjectFinder ||
                    itr->first==Attributes::GraphicalQueryBuilder ||
					itr->first==Attributes::SqlTool)
			{
				schparser.ignoreUnkownAttributes(true);
				schparser.ignoreEmptyAttributes(true);
				config_params[Attributes::Configuration][Attributes::DockWidgets]+=
						schparser.getCodeDefinition(widget_sch, itr->second);
				schparser.ignoreUnkownAttributes(false);
				schparser.ignoreEmptyAttributes(false);
			}

			itr++;
		}

		if(save_restore_geometry_chk->isChecked())
		{
		  for(auto &itr : widgets_geom)
		  {
			attribs[Attributes::Id] = itr.first;
			attribs[Attributes::XPos] = QString::number(itr.second.geometry.left());
			attribs[Attributes::YPos] = QString::number(itr.second.geometry.top());
			attribs[Attributes::Width] = QString::number(itr.second.geometry.width());
			attribs[Attributes::Height] = QString::number(itr.second.geometry.height());
			attribs[Attributes::Maximized] = itr.second.maximized ? Attributes::True : QString();

			schparser.ignoreUnkownAttributes(true);
			config_params[Attributes::Configuration][Attributes::WidgetsGeometry]+=
				schparser.getCodeDefinition(widget_sch, attribs);
			schparser.ignoreUnkownAttributes(false);
		  }
		}

		BaseConfigWidget::saveConfiguration(GlobalAttributes::GeneralConf, config_params);
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
	double fnt_size=config_params[Attributes::Configuration][Attributes::CodeFontSize].toDouble();

	if(fnt_size < 5.0f)
		fnt_size=5.0f;

	if(!save_restore_geometry_chk->isChecked())
	  widgets_geom.clear();

	unity_cmb->setCurrentIndex(UnitPoint);
	ObjectsScene::setPaperConfiguration(static_cast<QPrinter::PaperSize>(paper_cmb->itemData(paper_cmb->currentIndex()).toInt()),
										(portrait_rb->isChecked() ? QPrinter::Portrait : QPrinter::Landscape),
										QRectF(left_marg->value(), top_marg->value(), right_marg->value(), bottom_marg->value()),
										QSizeF(width_spb->value(), height_spb->value()));
	unity_cmb->setCurrentIndex(unit);

	ObjectsScene::setEnableCornerMove(corner_move_chk->isChecked());
	ObjectsScene::setInvertRangeSelectionTrigger(invert_rangesel_chk->isChecked());
	ObjectsScene::setGridSize(grid_size_spb->value());

	ObjectsScene::setGridOptions(config_params[Attributes::Configuration][Attributes::ShowCanvasGrid]==Attributes::True,
															 config_params[Attributes::Configuration][Attributes::AlignObjsToGrid]==Attributes::True,
															 config_params[Attributes::Configuration][Attributes::ShowPageDelimiters]==Attributes::True);

	OperationList::setMaximumSize(oplist_size_spb->value());
	BaseTableView::setHideExtAttributes(hide_ext_attribs_chk->isChecked());
	BaseTableView::setHideTags(hide_table_tags_chk->isChecked());
	BaseTableView::setAttributesPerPage(BaseTable::AttribsSection, attribs_per_page_spb->value());
	BaseTableView::setAttributesPerPage(BaseTable::ExtAttribsSection, ext_attribs_per_page_spb->value());
	RelationshipView::setHideNameLabel(hide_rel_name_chk->isChecked());
	RelationshipView::setCurvedLines(use_curved_lines_chk->isChecked());
	ModelWidget::setSaveLastCanvasPosition(save_last_pos_chk->isChecked());
	ModelWidget::setRenderSmoothnessDisabled(disable_smooth_chk->isChecked());
	ModelWidget::setSimplifiedObjectCreation(simple_obj_creation_chk->isChecked());
	ModelWidget::setMinimumObjectOpacity(min_obj_opacity_spb->value());
	MainWindow::setConfirmValidation(confirm_validation_chk->isChecked());
	BaseObjectView::setPlaceholderEnabled(use_placeholders_chk->isChecked());
	SQLExecutionWidget::setSQLHistoryMaxLength(history_max_length_spb->value());

	fnt.setFamily(config_params[Attributes::Configuration][Attributes::CodeFont]);
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
		BaseConfigWidget::restoreDefaults(GlobalAttributes::GeneralConf, false);
		BaseConfigWidget::restoreDefaults(GlobalAttributes::XMLHighlightConf, true);
		BaseConfigWidget::restoreDefaults(GlobalAttributes::SQLHighlightConf, true);
		BaseConfigWidget::restoreDefaults(GlobalAttributes::UiStyleConf, true);
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
	static int prev_unity=UnitMilimeters;
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

void GeneralConfigWidget::resetDialogsSizes(void)
{
	Messagebox msg_box;
	msg_box.show(trUtf8("This action will reset all dialogs to their default size and positions on the screen! Do you really want to proceed?"),
						Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

	if(msg_box.result() == QDialog::Accepted)
	  widgets_geom.clear();
}
