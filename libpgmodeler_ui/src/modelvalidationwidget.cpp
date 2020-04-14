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

#include "modelvalidationwidget.h"
#include "configurationform.h"
#include "pgmodeleruins.h"

ModelValidationWidget::ModelValidationWidget(QWidget *parent): QWidget(parent)
{
	try
	{
		setupUi(this);

		output_menu.addAction(tr("Copy as text"), this, SLOT(copyTextOutput()), QKeySequence("Ctrl+Shift+C"));
		output_menu.addAction(tr("Clear"), this, SLOT(clearOutput()));
		output_btn->setMenu(&output_menu);

		htmlitem_del=new HtmlItemDelegate(this);
		output_trw->setItemDelegateForColumn(0, htmlitem_del);

		version_cmb->addItem(tr("Autodetect"));
		version_cmb->addItems(PgSqlVersions::AllVersions);

		options_frm->setVisible(false);
		curr_step=0;

		validation_thread=nullptr;
		validation_helper=nullptr;
		this->setModel(nullptr);

		connect(hide_tb, SIGNAL(clicked()), this, SLOT(hide()));
		connect(options_btn, SIGNAL(toggled(bool)), options_frm, SLOT(setVisible(bool)));
		connect(sql_validation_chk, SIGNAL(toggled(bool)), connections_cmb, SLOT(setEnabled(bool)));
		connect(sql_validation_chk, SIGNAL(toggled(bool)), version_cmb, SLOT(setEnabled(bool)));
		connect(sql_validation_chk, SIGNAL(toggled(bool)), use_tmp_names_chk, SLOT(setEnabled(bool)));
		connect(validate_btn, SIGNAL(clicked()), this, SLOT(validateModel()));
		connect(fix_btn, SIGNAL(clicked()), this, SLOT(applyFixes()));
		connect(cancel_btn, SIGNAL(clicked()), this, SLOT(cancelValidation()));
		connect(connections_cmb, SIGNAL(activated(int)), this, SLOT(editConnections()));
		connect(swap_ids_btn, SIGNAL(clicked()), this, SLOT(swapObjectsIds()));

		connect(sql_validation_chk, &QCheckBox::toggled, [&](){
			configureValidation();
			clearOutput();
		});

		connect(use_tmp_names_chk, &QCheckBox::toggled, [&](){
			configureValidation();
			clearOutput();
		});

		connect(connections_cmb, &QComboBox::currentTextChanged, [&](){
			configureValidation();
			clearOutput();
		});

		connect(version_cmb, &QComboBox::currentTextChanged, [&](){
			configureValidation();
			clearOutput();
		});

		ConnectionsConfigWidget::fillConnectionsComboBox(connections_cmb, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

bool ModelValidationWidget::eventFilter(QObject *object, QEvent *event)
{
	QMouseEvent *m_event=dynamic_cast<QMouseEvent *>(event);

	//Executes the search when user press enter/return on the pattern field
	if(m_event && m_event->buttons() == Qt::RightButton)
	{
		QLabel *label = dynamic_cast<QLabel *>(object);

		if(label->hasSelectedText())
		{
			label->setContextMenuPolicy(Qt::DefaultContextMenu);
		}
		else
		{
			label->setContextMenuPolicy(Qt::NoContextMenu);
			selectObject();
		}
	}

	return QWidget::eventFilter(object, event);
}

void ModelValidationWidget::createThread()
{
	if(!validation_thread)
	{
		validation_thread=new QThread(this);
		validation_helper=new ModelValidationHelper;
		validation_helper->moveToThread(validation_thread);

		connect(validation_thread, &QThread::started, [&](){
			output_trw->setUniformRowHeights(true);
		});

		connect(validation_thread, &QThread::finished, [&](){
			output_trw->setUniformRowHeights(false);
		});

		connect(validation_thread, SIGNAL(started()), validation_helper, SLOT(validateModel()));
		connect(validation_thread, SIGNAL(started()), validation_helper, SLOT(applyFixes()));
		connect(validation_thread, SIGNAL(finished()), this, SLOT(updateGraphicalObjects()));
		connect(validation_thread, SIGNAL(finished()), this, SLOT(destroyThread()));

		connect(validation_helper, SIGNAL(s_validationInfoGenerated(ValidationInfo)), this, SLOT(updateValidation(ValidationInfo)), Qt::QueuedConnection);
		connect(validation_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType,QString,bool)), this, SLOT(updateProgress(int,QString,ObjectType,QString,bool)), Qt::BlockingQueuedConnection);
		connect(validation_helper, SIGNAL(s_objectProcessed(QString,ObjectType)), this, SLOT(updateObjectName(QString,ObjectType)), Qt::QueuedConnection);
		connect(validation_helper, SIGNAL(s_validationFinished()), this, SLOT(reenableValidation()), Qt::QueuedConnection);
		connect(validation_helper, SIGNAL(s_validationCanceled()), this, SLOT(reenableValidation()), Qt::QueuedConnection);
		connect(validation_helper, SIGNAL(s_sqlValidationStarted()), this, SLOT(handleSQLValidationStarted()), Qt::QueuedConnection);
		connect(validation_helper, SIGNAL(s_fixApplied()), this, SLOT(clearOutput()), Qt::QueuedConnection);
		connect(validation_helper, SIGNAL(s_fixApplied()), prog_info_wgt, SLOT(show()), Qt::QueuedConnection);
		connect(validation_helper, SIGNAL(s_relsValidationRequested()), this, SLOT(validateRelationships()));

		connect(validation_helper, &ModelValidationHelper::s_validationCanceled, [&](){
			emit s_validationCanceled();
		});

		connect(validation_helper, &ModelValidationHelper::s_fixApplied, [&](){
			emit s_fixApplied();
		});

		connect(validation_helper, &ModelValidationHelper::s_objectIdChanged, [&](BaseObject *obj) {
			BaseGraphicObject *graph_obj=dynamic_cast<BaseGraphicObject *>(obj);
			if(graph_obj) graph_objects.push_back(graph_obj);
		});
	}
}

void ModelValidationWidget::destroyThread(bool force)
{
	if(validation_thread && (force || validation_helper->getErrorCount()==0))
	{
		validation_thread->wait();
		delete validation_thread;
		delete validation_helper;
		validation_thread=nullptr;
		validation_helper=nullptr;
	}
}

void ModelValidationWidget::hide()
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}

void ModelValidationWidget::reenableValidation()
{
	if(!validation_helper->isInFixMode())
	{
		validation_thread->quit();
		model_wgt->setEnabled(true);
		validate_btn->setEnabled(true);
		cancel_btn->setEnabled(false);
		fix_btn->setEnabled(model_wgt->getDatabaseModel()->isInvalidated());
		output_btn->setEnabled(true);
		options_btn->setEnabled(true);
		options_frm->setEnabled(true);
		ico_lbl->setVisible(false);
		object_lbl->setVisible(false);
		swap_ids_btn->setEnabled(true);

		emit s_validationInProgress(false);
	}
}

void ModelValidationWidget::emitValidationInProgress()
{
	clearOutput();
	emit s_validationInProgress(true);

	ico_lbl->setVisible(true);
	object_lbl->setVisible(true);
	prog_info_wgt->setVisible(true);
	validate_btn->setEnabled(false);
	options_btn->setEnabled(false);
	model_wgt->setEnabled(false);
	cancel_btn->setEnabled(true);
	options_frm->setEnabled(false);
	swap_ids_btn->setEnabled(false);
}

void ModelValidationWidget::clearOutput()
{
	output_trw->clear();
	output_btn->setEnabled(false);
	prog_info_wgt->setVisible(false);
	fix_btn->setEnabled(false);
	validation_prog_pb->setValue(0);
	warn_lbl->setEnabled(false);
	error_lbl->setEnabled(false);
	warn_count_lbl->setText(QString("%1").arg(0));
	error_count_lbl->setText(QString("%1").arg(0));
}

void ModelValidationWidget::cancelValidation()
{
	validation_helper->cancelValidation();
	validation_thread->quit();
	cancel_btn->setEnabled(false);
}

void ModelValidationWidget::setModel(ModelWidget *model_wgt)
{
	bool enable=model_wgt!=nullptr;

	this->model_wgt=model_wgt;
	output_trw->setEnabled(enable);
	validate_btn->setEnabled(enable);
	options_btn->setEnabled(enable);
	options_frm->setEnabled(enable);
	fix_btn->setEnabled(false);
	swap_ids_btn->setEnabled(enable);
	curr_step=0;
	clearOutput();
	destroyThread(true);
}

bool ModelValidationWidget::isValidationRunning()
{
	return (validation_thread && validation_thread->isRunning());
}

void ModelValidationWidget::updateValidation(ValidationInfo val_info)
{
	if(validation_thread &&
			val_info.getValidationType()!=ValidationInfo::ValidationAborted &&
			!validation_thread->isRunning() &&
			validation_helper->isValidationCanceled())
		return;

	QTreeWidgetItem *item=new QTreeWidgetItem, *item1=nullptr, *item2=nullptr;
	QLabel *label=new QLabel, *label1=nullptr, *label2=nullptr;
	vector<BaseObject *> refs;
	BaseTable *table=nullptr;
	TableObject *tab_obj=nullptr;
	QString ref_name;
	BaseObject *ref_obj=nullptr;

	label->installEventFilter(this);
	label->setTextInteractionFlags(Qt::TextSelectableByMouse);

	if(val_info.getValidationType()==ValidationInfo::BrokenReference)
		label->setText(tr("The object <strong>%1</strong> <em>(%2)</em> [id: %3] is being referenced by <strong>%4</strong> object(s) before its creation.")
					   .arg(val_info.getObject()->getName(true).remove('"'))
					   .arg(val_info.getObject()->getTypeName())
					   .arg(val_info.getObject()->getObjectId())
					   .arg(val_info.getReferences().size()));
	else if(val_info.getValidationType()==ValidationInfo::SpObjBrokenReference)
	{
		QString str_aux;

		if(TableObject::isTableObject(val_info.getObject()->getObjectType()))
		{
			TableObject *tab_obj=dynamic_cast<TableObject *>(val_info.getObject());
			str_aux=QString(" owned by table <strong>%1</strong> ").arg(tab_obj->getParentTable()->getName(true).remove('"'));
		}

		label->setText(tr("The object <strong>%1</strong> <em>(%2)</em> [id: %3]%4 is referencing columns created by <strong>%5</strong> relationship(s) but is created before them.")
					   .arg(val_info.getObject()->getName(true).remove('"'))
					   .arg(val_info.getObject()->getTypeName())
					   .arg(val_info.getObject()->getObjectId())
					   .arg(str_aux)
					   .arg(val_info.getReferences().size()));
	}
	else if(val_info.getValidationType()==ValidationInfo::NoUniqueName)
	{
		tab_obj=dynamic_cast<TableObject *>(val_info.getObject());

		if(tab_obj)
		{
			table=tab_obj->getParentTable();
			ref_name=table->getName(true).remove('"') + QString(".") + val_info.getObject()->getName(true).remove('"');
		}
		else
			ref_name=val_info.getObject()->getName(true).remove('"');

		label->setText(tr("The object <strong>%1</strong> <em>(%2)</em> has a name that conflicts with <strong>%3</strong> object name(s).")
					   .arg(ref_name)
					   .arg(val_info.getObject()->getTypeName())
					   .arg(val_info.getReferences().size()));

	}
	else if(val_info.getValidationType()==ValidationInfo::BrokenRelConfig)
		label->setText(tr("The relationship <strong>%1</strong> [id: %2] is in a permanent invalidation state and needs to be relocated.")
					   .arg(val_info.getObject()->getName(true).remove('"'))
					   .arg(val_info.getObject()->getObjectId()));
	else if(val_info.getValidationType()==ValidationInfo::SqlValidationError)
		label->setText(tr("SQL validation failed due to the error(s) below. <strong>NOTE:</strong><em> Errors during SQL validation don't invalidate the model but may affect operations like <strong>export</strong> and <strong>diff</strong>.</em>"));
	else if(val_info.getValidationType() == ValidationInfo::MissingExtension)
	{
		Column *col = dynamic_cast<Column *>(val_info.getObject());

		label->setText(tr("The column <strong>%1</strong> on <strong>%2</strong> <em>(%3)</em> is referencing the geospatial data type <strong>%4</strong> but the <strong>postgis</strong> extension is not present in the model!")
									 .arg(col->getName())
									 .arg(col->getParentTable()->getSignature(true))
									 .arg(BaseObject::getTypeName(ObjectType::Table))
									 .arg(~col->getType()));
	}
	else
		label->setText(val_info.getErrors().at(0));

	/* Store the reference of the object in order to allow opening the editing form when the user clicks the item on the output
	 * So the needed fixes can be done manually */
	item->setData(1, Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(val_info.getObject())));

	if(val_info.getValidationType()==ValidationInfo::SqlValidationError ||
			val_info.getValidationType()==ValidationInfo::ValidationAborted)
	{
		QStringList errors=val_info.getErrors();
		item->setIcon(0, QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")));
		validation_prog_pb->setValue(validation_prog_pb->maximum());
		reenableValidation();

		if(val_info.getValidationType()==ValidationInfo::SqlValidationError)
		{
			//Adding all the sql errors into the output pane
			while(!errors.isEmpty())
			{
				item1=new QTreeWidgetItem(item);
				label1=new QLabel;
				label1->setTextInteractionFlags(Qt::TextSelectableByMouse);
				label1->setText(errors.back());
				label1->setTextInteractionFlags(Qt::TextSelectableByMouse);
				output_trw->setItemWidget(item1, 0, label1);
				errors.pop_back();
			}
		}
	}
	else
	{
		item->setIcon(0, QPixmap(PgModelerUiNs::getIconPath("msgbox_erro")));

		if(val_info.getValidationType()==ValidationInfo::BrokenRelConfig)
		{
			PgModelerUiNs::createOutputTreeItem(output_trw, tr("<strong>HINT:</strong> try to swap the relationship by another ones that somehow are linked to it through generated columns or constraints to solve this issue. Note that other objects may be lost in the swap process."),
												QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), item);
		}
		else if(val_info.getValidationType()==ValidationInfo::MissingExtension)
		{
			PgModelerUiNs::createOutputTreeItem(output_trw, tr("<strong>HINT:</strong> Create the extension in the model or let it be created by applying the needed fixes."),
																					QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), item);
		}
		else
		{
			//Listing the referrer object on output pane
			refs=val_info.getReferences();
			while(!refs.empty())
			{
				ref_obj = refs.back();

				item1=new QTreeWidgetItem(item);
				label1=new QLabel;
				label1->setTextInteractionFlags(Qt::TextSelectableByMouse);

				label1->installEventFilter(this);
				item1->setIcon(0, QPixmap(PgModelerUiNs::getIconPath(ref_obj->getSchemaName())));

				/* Store the reference of the referrer object in order to allow opening the editing form when the user clicks the item on the output
				 * So the needed fixes can be done manually */
				item1->setData(1, Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(ref_obj)));

				tab_obj=dynamic_cast<TableObject *>(ref_obj);
				ref_name=ref_obj->getName(true);

				if(tab_obj)
					ref_name=dynamic_cast<TableObject *>(ref_obj)->getParentTable()->getName(true) + QString(".") + ref_name;

				if(val_info.getValidationType()==ValidationInfo::NoUniqueName)
				{
					//If the referrer object is a table object, concatenates the parent table name
					if(tab_obj)
					{
						if(tab_obj->isAddedByRelationship())
						{
							QPalette pal;
							item2=new QTreeWidgetItem(item1);
							label2=new QLabel;
							label2->setTextInteractionFlags(Qt::TextSelectableByMouse);
							pal.setColor(QPalette::Text, QColor(255,0,0));
							label2->setPalette(pal);
							label2->setText(tr("<em>The above object was created by a relationship. Change the name pattern on it's generator relationship. Fix will not be applied!</em>"));
							output_trw->setItemWidget(item2, 0, label2);
							item1->setExpanded(true);
						}
					}

					label1->setText(tr("Conflicting object: <strong>%1</strong> <em>(%2)</em>.")
									.arg(ref_name.remove('"'))
									.arg(ref_obj->getTypeName()));
				}
				else
				{
					if(val_info.getValidationType()==ValidationInfo::SpObjBrokenReference)
						label1->setText(tr("Relationship: <strong>%1</strong> [id: %2].")
										.arg(ref_name.remove('"'))
										.arg(ref_obj->getObjectId()));
					else
					{
						label1->setText(tr("Referrer object: <strong>%1</strong> <em>(%2)</em> [id: %3].")
										.arg(ref_name.remove('"'))
										.arg(ref_obj->getTypeName())
										.arg(ref_obj->getObjectId()));
					}
				}

				output_trw->setItemWidget(item1, 0, label1);
				refs.pop_back();
			}
		}
	}

	output_trw->addTopLevelItem(item);
	output_trw->setItemWidget(item, 0, label);
	item->setExpanded(false);

	//Stores the validatin on the current tree item
	item->setData(0, Qt::UserRole, QVariant::fromValue<ValidationInfo>(val_info));
	warn_lbl->setEnabled(validation_helper->getWarningCount() > 0);
	error_lbl->setEnabled(validation_helper->getErrorCount() > 0);
	warn_count_lbl->setText(QString("%1").arg(validation_helper->getWarningCount()));
	error_count_lbl->setText(QString("%1").arg(validation_helper->getErrorCount()));
	item->setHidden(false);
	output_trw->scrollToBottom();

	if(val_info.getValidationType()==ValidationInfo::SqlValidationError)
		emit s_validationFinished(validation_helper->getErrorCount() != 0);
}

void ModelValidationWidget::validateModel()
{
	createThread();
	configureValidation();
	emitValidationInProgress();
	validation_helper->switchToFixMode(false);
	validation_thread->start();
}

void ModelValidationWidget::applyFixes()
{
	emitValidationInProgress();
	validation_helper->switchToFixMode(true);
	disconnect(validation_thread, SIGNAL(started()), validation_helper, SLOT(validateModel()));
	validation_thread->start();
	connect(validation_thread, SIGNAL(started()), validation_helper, SLOT(validateModel()));
}

void ModelValidationWidget::updateProgress(int prog, QString msg, ObjectType obj_type, QString cmd, bool is_code_gen)
{
	if(validation_thread &&
			(!validation_thread->isRunning() || validation_helper->isValidationCanceled()))
		return;

	QTreeWidgetItem *item=nullptr;

	validation_prog_pb->setValue(prog);

	if(prog >= 100 &&
			validation_helper->getErrorCount()==0 &&
			validation_helper->getWarningCount()==0)
	{
		error_lbl->setEnabled(false);
		error_count_lbl->setText(QString::number(0));
		fix_btn->setEnabled(false);

		if(sql_validation_chk->isChecked() && connections_cmb->currentIndex() <= 0)
		{
			warn_lbl->setEnabled(true);
			warn_count_lbl->setText(QString::number(1));
			PgModelerUiNs::createOutputTreeItem(output_trw,
												tr("SQL validation not executed! No connection defined."),
												QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")));
		}		
		else
		{
			warn_lbl->setEnabled(false);
			warn_count_lbl->setText(QString::number(0));
		}

		PgModelerUiNs::createOutputTreeItem(output_trw,
											tr("Database model successfully validated."),
											QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));

		emit s_validationFinished(validation_helper->getErrorCount() != 0);
	}
	else if(!msg.isEmpty())
	{
		QPixmap ico;

		msg=PgModelerUiNs::formatMessage(msg);

		if(obj_type!=ObjectType::BaseObject)
			ico=QPixmap(PgModelerUiNs::getIconPath(obj_type));
		else if(!cmd.isEmpty())
			ico=QPixmap(PgModelerUiNs::getIconPath("sqlcmd"));
		else
			ico=QPixmap(PgModelerUiNs::getIconPath("msgbox_info"));

		if(is_code_gen)
		{
			ico_lbl->setPixmap(ico);
			object_lbl->setText(msg);
		}
		else
		{
			ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("codigosql")));
			object_lbl->setText(tr("Running SQL commands on server..."));

			item=PgModelerUiNs::createOutputTreeItem(output_trw, msg, ico, nullptr, false);

			if(!cmd.isEmpty())
				PgModelerUiNs::createOutputTreeItem(output_trw, cmd, QPixmap(), item, false);
		}
	}
}

void ModelValidationWidget::updateObjectName(QString obj_name, ObjectType obj_type)
{
	object_lbl->setText(tr("Processing object: %1").arg(PgModelerUiNs::formatMessage(obj_name)));
	ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath(obj_type)));
}

void ModelValidationWidget::configureValidation()
{
	if(model_wgt && validation_helper)
	{
		Connection *conn=nullptr;
		QString ver;

		//Get the connection only the checkbox is checked.
		if(sql_validation_chk->isChecked() && connections_cmb->currentIndex() > 0 && connections_cmb->currentIndex()!=connections_cmb->count()-1)
		{
			conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
			ver=(version_cmb->currentIndex() > 0 ? version_cmb->currentText() : QString());
		}

		validation_helper->setValidationParams(model_wgt->getDatabaseModel(), conn, ver, use_tmp_names_chk->isChecked());
	}
}

void ModelValidationWidget::resizeEvent(QResizeEvent *event)
{
	Qt::ToolButtonStyle style=Qt::ToolButtonTextBesideIcon;

	if(event->size().width() < this->baseSize().width())
		style=Qt::ToolButtonIconOnly;

	if(validate_btn->toolButtonStyle()!=style)
	{
		validate_btn->setToolButtonStyle(style);
		fix_btn->setToolButtonStyle(style);
		output_btn->setToolButtonStyle(style);
		cancel_btn->setToolButtonStyle(style);
		options_btn->setToolButtonStyle(style);
		swap_ids_btn->setToolButtonStyle(style);
	}
}

void ModelValidationWidget::validateRelationships()
{
	try
	{
		model_wgt->getDatabaseModel()->validateRelationships();
		model_wgt->setModified(true);
	}
	catch(Exception &e)
	{
		Messagebox msg_box;
		msg_box.show(e);
	}
}

void ModelValidationWidget::updateGraphicalObjects()
{
	if(!graph_objects.empty())
	{
		vector<BaseGraphicObject *>::iterator end;

		std::sort(graph_objects.begin(), graph_objects.end());
		end=std::unique(graph_objects.begin(), graph_objects.end());
		graph_objects.erase(end, graph_objects.end());

		while(!graph_objects.empty())
		{
			graph_objects.back()->setModified(true);
			graph_objects.pop_back();
		}

		emit s_graphicalObjectsUpdated();
	}
}

void ModelValidationWidget::editConnections()
{
	if(connections_cmb->currentIndex()==connections_cmb->count()-1)
	{
		ConnectionsConfigWidget::openConnectionsConfiguration(connections_cmb, true);
		emit s_connectionsUpdateRequest();
	}
}

void ModelValidationWidget::handleSQLValidationStarted()
{
	options_btn->setEnabled(false);
	output_btn->setEnabled(false);
	options_frm->setEnabled(false);
}

void ModelValidationWidget::swapObjectsIds()
{
	if(model_wgt)
		model_wgt->swapObjectsIds();
}

void ModelValidationWidget::selectObject()
{
	QTreeWidgetItem *item = output_trw->currentItem();

	if(item && validation_thread && !validation_thread->isRunning())
	{
		BaseObject *selected_obj=reinterpret_cast<BaseObject *>(item->data(1, Qt::UserRole).value<void *>());

		if(selected_obj && QApplication::mouseButtons() == Qt::RightButton)
		{
			model_wgt->configurePopupMenu(selected_obj);
			model_wgt->showObjectMenu();
		}
	}
}

void ModelValidationWidget::copyTextOutput()
{
	qApp->clipboard()->setText(generateOutputText());
}

QString ModelValidationWidget::generateOutputText()
{
	QString output;
	QTreeWidgetItem *item = nullptr;

	for(int idx = 0; idx < output_trw->topLevelItemCount(); idx ++)
	{
		item = output_trw->topLevelItem(idx);
		generateOutputItemText(item, output, 0);
		output += '\n';
	}

	return output;
}

void ModelValidationWidget::generateOutputItemText(QTreeWidgetItem *item, QString &output, int level)
{
	if(!item)	return;

	QLabel *label = dynamic_cast<QLabel *>(item->treeWidget()->itemWidget(item, 0));
	QString text,
			filler = "\n" + QString().fill(' ', level * 2);

	text = item->text(0);

	if(label && text.isEmpty())
		text = label->text();

	text.replace(QRegExp("(\\<)(\\/)?(br|strong|em)(\\/)?(\\>)"), "");
	text.prepend(level == 0 ? "* " : "\n");

	text.replace("\n", filler);
	output += text;

	for(int child = 0; child < item->childCount(); child++)
		generateOutputItemText(item->child(child), output, level + 1);
}
