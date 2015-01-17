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

#include "generalconfigwidget.h"
#include "objectsscene.h"
#include "modelwidget.h"
#include "operationlist.h"
#include "syntaxhighlighter.h"
#include "mainwindow.h"

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

	for(int i=0; i < count; i++)
		paper_cmb->setItemData(i, QVariant(paper_ids[i]));

	connect(unity_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(convertMarginUnity(void)));
	connect(autosave_interv_chk, SIGNAL(toggled(bool)), autosave_interv_spb, SLOT(setEnabled(bool)));
	connect(paper_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPaperSize(void)));

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
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::INVERT_PANNING_RANGESEL]=QString();
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CHECK_UPDATE]=QString();
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_LAST_POSITION]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_MAIN_MENU]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISABLE_SMOOTHNESS]=QString();
	config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SIMPLIFIED_OBJ_CREATION]=QString();
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CONFIRM_VALIDATION]=QString();
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SHOW_MAIN_MENU]=QString();
  config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_COMPLETION]=QString();

  simp_obj_creation_ht=new HintTextWidget(simp_obj_creation_hint, this);
  simp_obj_creation_ht->setText(simple_obj_creation_chk->statusTip());

  confirm_validation_ht=new HintTextWidget(confirm_validation_hint, this);
  confirm_validation_ht->setText(confirm_validation_chk->statusTip());

  corner_move_ht=new HintTextWidget(corner_move_hint, this);
  corner_move_ht->setText(corner_move_chk->statusTip());

  save_last_pos_ht=new HintTextWidget(save_last_pos_hint, this);
  save_last_pos_ht->setText(save_last_pos_chk->statusTip());

  invert_pan_range_ht=new HintTextWidget(invert_pan_range_hint, this);
  invert_pan_range_ht->setText(invert_pan_range_chk->statusTip());

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
}

void GeneralConfigWidget::loadConfiguration(void)
{
	try
	{
		QStringList margin, custom_size;
		vector<QString> key_attribs;
		unsigned interv=0;

    for(QWidget *wgt : child_wgts)
      wgt->blockSignals(true);

		key_attribs.push_back(ParsersAttributes::ID);
    BaseConfigWidget::loadConfiguration(GlobalAttributes::GENERAL_CONF, config_params, key_attribs);

		grid_size_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]).toUInt());
		oplist_size_spb->setValue((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]).toUInt());

		interv=(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]).toUInt();
		autosave_interv_chk->setChecked(interv > 0);
		autosave_interv_spb->setValue(interv);
		autosave_interv_spb->setEnabled(autosave_interv_chk->isChecked());
		corner_move_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CANVAS_CORNER_MOVE]==ParsersAttributes::_TRUE_);
		invert_pan_range_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::INVERT_PANNING_RANGESEL]==ParsersAttributes::_TRUE_);
		check_upd_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CHECK_UPDATE]==ParsersAttributes::_TRUE_);
		save_last_pos_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_LAST_POSITION]==ParsersAttributes::_TRUE_);
		disable_smooth_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISABLE_SMOOTHNESS]==ParsersAttributes::_TRUE_);
		simple_obj_creation_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SIMPLIFIED_OBJ_CREATION]==ParsersAttributes::_TRUE_);
    confirm_validation_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CONFIRM_VALIDATION]==ParsersAttributes::_TRUE_);
    code_completion_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_COMPLETION]==ParsersAttributes::_TRUE_);

		print_grid_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_GRID]==ParsersAttributes::_TRUE_);
		print_pg_num_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PRINT_PG_NUM]==ParsersAttributes::_TRUE_);

		paper_cmb->setCurrentIndex((config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]).toUInt());
		portrait_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::PORTRAIT);
		landscape_rb->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]==ParsersAttributes::LANDSCAPE);

		margin=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_MARGIN].split(",");
		custom_size=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_CUSTOM_SIZE].split(",");

		left_marg->setValue((margin.count() >= 4 ? margin[0].toFloat() : 2));
		top_marg->setValue((margin.count()>= 4 ? margin[1].toFloat() : 2));
		right_marg->setValue((margin.count() >= 4 ? margin[2].toFloat() : 2));
		bottom_marg->setValue((margin.count() >= 4 ? margin[3].toFloat() : 2));

		width_spb->setValue((custom_size.count() >= 2 ? custom_size[0].toFloat() : 500));
		height_spb->setValue((custom_size.count() >= 2 ? custom_size[1].toFloat() : 500));

		hide_ext_attribs_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_EXT_ATTRIBS]==ParsersAttributes::_TRUE_);
		hide_rel_name_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_REL_NAME]==ParsersAttributes::_TRUE_);
		hide_table_tags_chk->setChecked(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::HIDE_TABLE_TAGS]==ParsersAttributes::_TRUE_);

		font_cmb->setCurrentFont(QFont(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT]));
		font_size_spb->setValue(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT_SIZE].toFloat());

    for(QWidget *wgt : child_wgts)
      wgt->blockSignals(false);

		this->applyConfiguration();
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
  }
}

void GeneralConfigWidget::addConfigurationParam(const QString &param, const attribs_map &attribs)
{
  BaseConfigWidget::addConfigurationParam(config_params, param, attribs);
}

map<QString, attribs_map> GeneralConfigWidget::getConfigurationParams(void)
{
  return(config_params);
}

void GeneralConfigWidget::saveConfiguration(void)
{
  try
	{
    map<QString, attribs_map >::iterator itr, itr_end;
    QString file_sch, root_dir, widget_sch;

    root_dir=GlobalAttributes::CONFIGURATIONS_DIR +
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

    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::GRID_SIZE]=QString("%1").arg(grid_size_spb->value());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::OP_LIST_SIZE]=QString("%1").arg(oplist_size_spb->value());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::AUTOSAVE_INTERVAL]=QString("%1").arg(autosave_interv_chk->isChecked() ? autosave_interv_spb->value() : 0);
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_TYPE]=QString("%1").arg(paper_cmb->currentIndex());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::PAPER_ORIENTATION]=(portrait_rb->isChecked() ? ParsersAttributes::PORTRAIT : ParsersAttributes::LANDSCAPE);
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CANVAS_CORNER_MOVE]=(corner_move_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::INVERT_PANNING_RANGESEL]=(invert_pan_range_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CHECK_UPDATE]=(check_upd_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SAVE_LAST_POSITION]=(save_last_pos_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::DISABLE_SMOOTHNESS]=(disable_smooth_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::SIMPLIFIED_OBJ_CREATION]=(simple_obj_creation_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CONFIRM_VALIDATION]=(confirm_validation_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());
    config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_COMPLETION]=(code_completion_chk->isChecked() ? ParsersAttributes::_TRUE_ : QString());

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
              itr->first==ParsersAttributes::OBJECT_FINDER)
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
	float fnt_size=config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT_SIZE].toFloat();

	if(fnt_size < 5.0f)
		fnt_size=5.0f;

  unity_cmb->setCurrentIndex(UNIT_POINT);
	ObjectsScene::setPaperConfiguration(static_cast<QPrinter::PaperSize>(paper_cmb->itemData(paper_cmb->currentIndex()).toInt()),
																		 (portrait_rb->isChecked() ? QPrinter::Portrait : QPrinter::Landscape),
																		 QRectF(left_marg->value(), top_marg->value(), right_marg->value(), bottom_marg->value()),
																		 QSizeF(width_spb->value(), height_spb->value()));
	unity_cmb->setCurrentIndex(unit);

  ObjectsScene::setEnableCornerMove(corner_move_chk->isChecked());
  ObjectsScene::setInvertPanningRangeSelection(invert_pan_range_chk->isChecked());
	ObjectsScene::setGridSize(grid_size_spb->value());
	OperationList::setMaximumSize(oplist_size_spb->value());
  BaseTableView::setHideExtAttributes(hide_ext_attribs_chk->isChecked());
  BaseTableView::setHideTags(hide_table_tags_chk->isChecked());
  RelationshipView::setHideNameLabel(hide_rel_name_chk->isChecked());
	ModelWidget::setSaveLastCanvasPosition(save_last_pos_chk->isChecked());
	ModelWidget::setRenderSmoothnessDisabled(disable_smooth_chk->isChecked());
	ModelWidget::setSimplifiedObjectCreation(simple_obj_creation_chk->isChecked());
  MainWindow::setConfirmValidation(confirm_validation_chk->isChecked());

	fnt.setFamily(config_params[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_FONT]);
	fnt.setPointSize(fnt_size);
	SyntaxHighlighter::setDefaultFont(fnt);
}

void GeneralConfigWidget::restoreDefaults(void)
{
	try
	{
		BaseConfigWidget::restoreDefaults(GlobalAttributes::GENERAL_CONF);
		this->loadConfiguration();
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
	float conv_factor[]={1.0f, 2.83f, 0.04f, 0.1f},
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

void GeneralConfigWidget::updateFileAssociation(void)
{
 Messagebox msg_box;
 QString title=trUtf8("File association missing"),
         msg=trUtf8("It seems that .dbm files aren't associated with pgModeler. Do you want to do it now?");

 #ifdef Q_OS_LINUX
  attribs_map attribs;
	QString str_aux,

			 //Configures the path to the application logo
			 exec_icon=QDir(GlobalAttributes::CONFIGURATIONS_DIR +
											GlobalAttributes::DIR_SEPARATOR + "pgmodeler_logo.png").absolutePath(),

			 //Configures the path to the document logo
			 dbm_icon=QDir(GlobalAttributes::CONFIGURATIONS_DIR +
										 GlobalAttributes::DIR_SEPARATOR + "pgmodeler_dbm.png").absolutePath(),

			 //Path to directory that register mime types
			 mime_db_dir=QDir::homePath() + "/.local/share/mime",

			 //Path to the file that associates apps to mimetypes
			 mimeapps=QDir::homePath() + "/.local/share/applications/mimeapps.list",

			 base_conf_dir=GlobalAttributes::CONFIGURATIONS_DIR + GlobalAttributes::DIR_SEPARATOR +
										 GlobalAttributes::SCHEMAS_DIR + GlobalAttributes::DIR_SEPARATOR,

			 //Files generated after update file association (application-dbm.xml and pgModeler.desktop)
			 files[] = { QDir::homePath() + "/.local/share/applications/pgModeler.desktop",
									 mime_db_dir + "/packages/application-dbm.xml" },

			 schemas[] = { base_conf_dir + "desktop" + GlobalAttributes::SCHEMA_EXT,
										 base_conf_dir + "application-dbm" + GlobalAttributes::SCHEMA_EXT };
	QByteArray buf, buf_aux;
	QFile out;

	//Check if the necessary file exists. If not asks the user to update file association
	if(!QFileInfo(files[0]).exists() || !QFileInfo(files[1]).exists())
    msg_box.show(title, msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

	if(msg_box.result()==QDialog::Accepted)
	{
		//file_associated=true;
		attribs[ParsersAttributes::ROOT_DIR]=QApplication::applicationDirPath();
		attribs[ParsersAttributes::ICON]=exec_icon;

		try
		{
			for(unsigned i=0; i < 2; i++)
			{
				schparser.loadFile(schemas[i]);
				buf.append(schparser.getCodeDefinition(attribs));
				QDir(".").mkpath(QFileInfo(files[i]).absolutePath());

				out.setFileName(files[i]);
				out.open(QFile::WriteOnly);

				if(!out.isOpen())
					throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(files[i]),
													ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				out.write(buf.data(), buf.size());
				out.close();
				buf.clear();
				attribs[ParsersAttributes::ICON]=dbm_icon;
			}

			out.setFileName(mimeapps);

			//If the file mimeapps.list doesn't exists (generally in Ubuntu) creates a new one
			if(!QFileInfo(mimeapps).exists())
			{
				out.open(QFile::WriteOnly);
				out.write(QByteArray("[Added Associations]\napplication/dbm=pgModeler.desktop;\n"));
				out.close();
			}
			else
			{

				out.open(QFile::ReadOnly);

				if(!out.isOpen())
					throw Exception(Exception::getErrorMessage(ERR_FILE_NOT_WRITTEN).arg(mimeapps),
													ERR_FILE_NOT_WRITTEN,__PRETTY_FUNCTION__,__FILE__,__LINE__);

				//Opens the mimeapps.list to add a entry linking pgModeler to .dbm files
				buf=out.readAll();
				out.close();

				QTextStream ts(&buf);
				while(!ts.atEnd())
				{
					//Remove any reference to application/dbm mime from file
					str_aux=ts.readLine();
					str_aux.replace(QRegExp("application/dbm*",Qt::CaseSensitive,QRegExp::Wildcard),"");

					if(!str_aux.isEmpty())
					{
						//Updates the application/dbm mime association
						if(str_aux.contains("[Added Associations]"))
							str_aux.append("\napplication/dbm=pgModeler.desktop;\n");
						else
							str_aux+="\n";

						buf_aux.append(str_aux);
					}
				}

				//Write a new copy of the mimeapps.list file
				out.open(QFile::Truncate | QFile::WriteOnly);
				out.write(buf_aux.data(), buf_aux.size());
				out.close();
			}

			//Update the mime database
			QProcess::execute("update-mime-database", QStringList { mime_db_dir });
		}
		catch(Exception &e)
		{
			msg_box.show(e);
		}
	}
 #else
		#ifdef Q_OS_WIN
     //Checking if the .dbm registry key exists
     QSettings dbm_ext("HKEY_CURRENT_USER\\Software\\Classes\\.dbm", QSettings::NativeFormat);
     QString exe_path=QDir::toNativeSeparators(QApplication::applicationDirPath() + "\\pgmodeler.exe");

     //If there is no value assigned to .dbm/Default key shows the update extension confirmation message
     if(dbm_ext.value("Default").toString().isEmpty())
       msg_box.show(title, msg, Messagebox::CONFIRM_ICON, Messagebox::YES_NO_BUTTONS);

      if(msg_box.result()==QDialog::Accepted)
      {
        //Write the default value for .dbm registry key
        dbm_ext.setValue("Default", "dbm_auto_file");
        dbm_ext.sync();

        //Other registry keys values
        map<QString, QStringList> confs = {
          { "\\HKEY_CURRENT_USER\\Software\\Classes\\dbm_auto_file", { "FriendlyTypeName" , "pgModeler Database Model" } },
          { "\\HKEY_CURRENT_USER\\Software\\Classes\\dbm_auto_file\\DefaultIcon", { "Default" , QString("%1,1").arg(exe_path) } },
          { "\\HKEY_CURRENT_USER\\Software\\Classes\\dbm_auto_file\\shell\\open\\command", { "Default" , QString("\"%1\" \"%2\"").arg(exe_path).arg("%1") } },
          { "\\HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.dbm", { "OpenWithList/a", "pgmodeler.exe", "OpenWithList/MRUList","a"} }
        };

        map<QString, QStringList>::iterator itr;
        itr=confs.begin();

        //Iterates over the configuration map writing the other keys on registry
        while(itr!=confs.end())
        {
          QSettings s(itr->first, QSettings::NativeFormat);
          for(int i=0; i < itr->second.size(); i+=2)
            s.setValue(itr->second[i], itr->second[i+1]);

          s.sync();
          itr++;
        }
     }
		#endif
#endif
}

void GeneralConfigWidget::selectPaperSize()
{
	bool visible=paper_cmb->currentIndex()==paper_cmb->count()-1;

	custom_lbl->setVisible(visible);
	width_lbl->setVisible(visible);
	height_lbl->setVisible(visible);
	width_spb->setVisible(visible);
	height_spb->setVisible(visible);
}

void GeneralConfigWidget::hideEvent(QHideEvent *)
{
  settings_twg->setCurrentIndex(0);
}
