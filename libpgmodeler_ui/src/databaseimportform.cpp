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

#include "databaseimportform.h"
#include "taskprogresswidget.h"
#include "configurationform.h"
#include "taskprogresswidget.h"
#include "pgmodeleruins.h"
#include "pgmodelerns.h"
#include "defaultlanguages.h"

bool DatabaseImportForm::low_verbosity = false;

DatabaseImportForm::DatabaseImportForm(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);

	model_wgt=nullptr;
	create_model=true;

	objs_filter_wgt = new ObjectsFilterWidget(options_tbw->widget(1));
	QVBoxLayout *vbox = new QVBoxLayout(options_tbw->widget(1));
	vbox->setContentsMargins(4,4,4,4);
	vbox->addWidget(objs_filter_wgt);

	htmlitem_del=new HtmlItemDelegate(this);
	output_trw->setItemDelegateForColumn(0, htmlitem_del);

	settings_tbw->setTabEnabled(1, false);
	objs_parent_wgt->setEnabled(false);
	buttons_wgt->setEnabled(false);

	connect(close_btn, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(connections_cmb, SIGNAL(activated(int)), this, SLOT(listDatabases()));
	connect(database_cmb, SIGNAL(activated(int)), this, SLOT(listObjects()));
	connect(import_sys_objs_chk, SIGNAL(clicked(bool)), this, SLOT(listObjects()));
	connect(import_ext_objs_chk, SIGNAL(clicked(bool)), this, SLOT(listObjects()));
	connect(by_oid_chk, SIGNAL(toggled(bool)), this, SLOT(filterObjects()));
	connect(expand_all_tb, SIGNAL(clicked(bool)), db_objects_tw, SLOT(expandAll()));
	connect(collapse_all_tb, SIGNAL(clicked(bool)), db_objects_tw, SLOT(collapseAll()));
	connect(db_objects_tw, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(setItemCheckState(QTreeWidgetItem*,int)));
	connect(select_all_tb, SIGNAL(clicked(bool)), this, SLOT(setItemsCheckState()));
	connect(clear_all_tb, SIGNAL(clicked(bool)), this, SLOT(setItemsCheckState()));
	connect(filter_edt, SIGNAL(textChanged(QString)), this, SLOT(filterObjects()));
	connect(import_btn, SIGNAL(clicked(bool)), this, SLOT(importDatabase()));
	connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(cancelImport()));
	connect(objs_filter_wgt, SIGNAL(s_filterApplyingRequested()), this, SLOT(listObjects()));

	connect(import_to_model_chk, &QCheckBox::toggled,
			[&](bool checked){ create_model=!checked; });

	connect(database_cmb, &QComboBox::currentTextChanged,
	[&]() {
		bool enable = database_cmb->currentIndex() > 0;

		if(database_cmb->currentIndex()==0)
		{
			db_objects_tw->clear();
			filtered_objs_tbw->setRowCount(0);
		}

		import_btn->setEnabled(enable);
		objs_parent_wgt->setEnabled(enable);
		filtered_objs_tbw->setEnabled(enable);
	});


#ifdef DEMO_VERSION
	#warning "DEMO VERSION: forcing ignore errors in reverse engineering due to the object count limit."
	ignore_errors_chk->setChecked(true);
	ignore_errors_chk->setEnabled(false);
#endif
}

DatabaseImportForm::~DatabaseImportForm()
{
	destroyThread();
}

void DatabaseImportForm::setModelWidget(ModelWidget *model)
{
	model_wgt=model;
	import_to_model_chk->setEnabled(model!=nullptr);
}

void DatabaseImportForm::setLowVerbosity(bool value)
{
	low_verbosity = value;
}

void DatabaseImportForm::createThread()
{
	import_thread=new QThread;

	import_helper=new DatabaseImportHelper;
	import_helper->moveToThread(import_thread);

	connect(import_thread, &QThread::started, [&](){
		output_trw->setUniformRowHeights(true);
	});

	connect(import_thread, &QThread::finished, [&](){
		output_trw->setUniformRowHeights(false);
	});

	connect(import_thread, SIGNAL(started()), import_helper, SLOT(importDatabase()));
	connect(import_helper, SIGNAL(s_importCanceled()), this, SLOT(handleImportCanceled()));
	connect(import_helper, SIGNAL(s_importFinished(Exception)), this, SLOT(handleImportFinished(Exception)));
	connect(import_helper, SIGNAL(s_importAborted(Exception)), this, SLOT(captureThreadError(Exception)));
	connect(import_helper, SIGNAL(s_progressUpdated(int,QString,ObjectType)), this, SLOT(updateProgress(int,QString,ObjectType)), Qt::BlockingQueuedConnection);
}

void DatabaseImportForm::destroyThread()
{
	if(import_thread)
	{
		import_thread->quit();
		import_thread->wait();
		delete import_thread;
		import_thread=nullptr;
		delete import_helper;
		import_helper=nullptr;
	}
}

void DatabaseImportForm::listFilteredObjects()
{
	vector<ObjectType> types = import_helper->getCatalog().getFilteredObjectTypes();
	vector<attribs_map> obj_attrs;
	QTableWidgetItem *item = nullptr;
	int row = 0;
	ObjectType obj_type;

	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		obj_attrs = import_helper->getObjects(types);
		filtered_objs_tbw->clearContents();
		filtered_objs_tbw->setRowCount(0);

		filtered_objs_tbw->setUpdatesEnabled(false);
		filtered_objs_tbw->setSortingEnabled(false);

		for(auto &attr : obj_attrs)
		{
			filtered_objs_tbw->insertRow(row);

			// Object name column
			item = new QTableWidgetItem;
			item->setText(attr[Attributes::Name]);
			item->setData(Qt::UserRole, attr[Attributes::Oid].toUInt());
			item->setCheckState(Qt::Checked);
			filtered_objs_tbw->setItem(row, 0, item);

			// Object type column
			item = new QTableWidgetItem;
			obj_type = static_cast<ObjectType>(attr[Attributes::ObjectType].toUInt());
			item->setText(BaseObject::getTypeName(obj_type));
			item->setIcon(QIcon(PgModelerUiNs::getIconPath(obj_type)));
			item->setData(Qt::UserRole, enum_cast(obj_type));
			filtered_objs_tbw->setItem(row, 1, item);

			// Parent name column
			item = new QTableWidgetItem;
			item->setText(attr[Attributes::Parent]);
			filtered_objs_tbw->setItem(row, 2, item);

			// Parent type column
			item = new QTableWidgetItem;
			obj_type = BaseObject::getObjectType(attr[Attributes::ParentType]);
			item->setText(BaseObject::getTypeName(obj_type));
			item->setIcon(QIcon(PgModelerUiNs::getIconPath(obj_type)));
			filtered_objs_tbw->setItem(row, 3, item);

			row++;
		}

		filtered_objs_tbw->setUpdatesEnabled(true);
		filtered_objs_tbw->setSortingEnabled(true);
		filtered_objs_tbw->resizeColumnsToContents();
		filtered_objs_tbw->setEnabled(filtered_objs_tbw->rowCount() > 0);
		QApplication::restoreOverrideCursor();
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void DatabaseImportForm::updateProgress(int progress, QString msg, ObjectType obj_type)
{
	QPixmap ico;

	msg=PgModelerUiNs::formatMessage(msg);
	progress_lbl->setText(msg);
	progress_pb->setValue(progress);

	if(obj_type!=ObjectType::BaseObject)
		ico=QPixmap(PgModelerUiNs::getIconPath(obj_type));
	else
		ico=QPixmap(PgModelerUiNs::getIconPath("msgbox_info"));

	ico_lbl->setPixmap(ico);

	if(!low_verbosity)
		PgModelerUiNs::createOutputTreeItem(output_trw, msg, ico);
}

void DatabaseImportForm::setItemCheckState(QTreeWidgetItem *item, int)
{
	db_objects_tw->blockSignals(true);
	setItemCheckState(item, item->checkState(0));
	setParentItemChecked(item->parent());
	import_btn->setEnabled(hasCheckedItems());
	db_objects_tw->blockSignals(false);
}

void DatabaseImportForm::setItemsCheckState()
{
	Qt::CheckState chk_state=(sender()==select_all_tb ? Qt::Checked : Qt::Unchecked);

	if(db_objects_stw->currentIndex() == 0)
	{
		QTreeWidgetItemIterator itr(db_objects_tw);

		db_objects_tw->blockSignals(true);
		while(*itr)
		{
			if(!(*itr)->isDisabled())
				(*itr)->setCheckState(0, chk_state);

			++itr;
		}
		db_objects_tw->blockSignals(false);
	}
	else
	{
		int row_cnt = filtered_objs_tbw->rowCount();

		filtered_objs_tbw->blockSignals(true);

		for(int row = 0; row < row_cnt; row++)
			filtered_objs_tbw->item(row, 0)->setCheckState(chk_state);

		filtered_objs_tbw->blockSignals(false);
	}

	import_btn->setEnabled(chk_state == Qt::Checked);
}

void DatabaseImportForm::importDatabase()
{
	try
	{
		Messagebox msg_box;

		map<ObjectType, vector<unsigned>> obj_oids;
		map<unsigned, vector<unsigned>> col_oids;

		if(import_to_model_chk->isChecked())
		{
			msg_box.show(tr("<strong>ATTENTION:</strong> You are about to import objects to the current working model! This action will cause irreversible changes to it even in case of critical errors during the process. Do you want to proceed?"),
						 Messagebox::AlertIcon, Messagebox::YesNoButtons);

			if(msg_box.result()==QDialog::Rejected)
				return;
		}

		output_trw->clear();
		settings_tbw->setTabEnabled(1, true);
		settings_tbw->setCurrentIndex(1);

		if(low_verbosity)
			PgModelerUiNs::createOutputTreeItem(output_trw, tr("<strong>Low verbosity is set:</strong> only key informations and errors will be displayed."),
																					QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta")), nullptr, false);

		getCheckedItems(obj_oids, col_oids);
		obj_oids[ObjectType::Database].push_back(database_cmb->itemData(database_cmb->currentIndex()).value<unsigned>());

		if(create_model)
		{
			model_wgt=new ModelWidget;
			model_wgt->getDatabaseModel()->createSystemObjects(true);
		}

		model_wgt->setUpdatesEnabled(false);
		import_helper->setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(),
																		resolve_deps_chk->isChecked(), ignore_errors_chk->isChecked(),
																		debug_mode_chk->isChecked(), rand_rel_color_chk->isChecked(), true);

		import_helper->setSelectedOIDs(model_wgt->getDatabaseModel(), obj_oids, col_oids);
		import_thread->start();
		cancel_btn->setEnabled(true);
		import_btn->setEnabled(false);
		database_gb->setEnabled(false);
		options_tbw->setEnabled(false);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportForm::setItemCheckState(QTreeWidgetItem *item, Qt::CheckState chk_state)
{
	for(int i=0; i < item->childCount(); i++)
	{
		if(!item->child(i)->isDisabled() && !item->child(i)->isHidden())
			item->child(i)->setCheckState(0, chk_state);

		setItemCheckState(item->child(i), chk_state);
	}
}

void DatabaseImportForm::setParentItemChecked(QTreeWidgetItem *item)
{
	if(item && !item->isDisabled())
	{
		if(item->checkState(0)!=Qt::Checked)
			item->setCheckState(0, Qt::Checked);

		setParentItemChecked(item->parent());
	}
}

bool DatabaseImportForm::hasCheckedItems()
{
	bool selected=false;

	if(db_objects_stw->currentIndex() == 0)
	{
		QTreeWidgetItemIterator itr(db_objects_tw);
		while(*itr && !selected)
		{
			//Only valid items (OID > 0) and with Checked state are considered as selected
			selected=((*itr)->checkState(0)==Qt::Checked && (*itr)->data(ObjectId, Qt::UserRole).value<unsigned>() > 0);
			++itr;
		}
	}
	else
	{
		int row_cnt = filtered_objs_tbw->rowCount(), curr_row = 0;

		while(!selected && curr_row < row_cnt)
		{
			selected = (filtered_objs_tbw->item(curr_row, 0)->checkState() == Qt::Checked);
			curr_row++;
		}
	}

	return selected;
}

void DatabaseImportForm::getCheckedItems(map<ObjectType, vector<unsigned>> &obj_oids, map<unsigned, vector<unsigned>> &col_oids)
{
	ObjectType obj_type;
	unsigned tab_oid=0;

	obj_oids.clear();
	col_oids.clear();

	// Retriving selected items from objects tree
	if(db_objects_stw->currentIndex() == 0)
	{
		QTreeWidgetItemIterator itr(db_objects_tw);
		while(*itr)
		{
			//If the item is checked and its OID is valid
			if((*itr)->checkState(0)==Qt::Checked && (*itr)->data(ObjectId, Qt::UserRole).value<unsigned>() > 0)
			{
				obj_type=static_cast<ObjectType>((*itr)->data(ObjectTypeId, Qt::UserRole).value<unsigned>());

				//If the object is not a column store it on general object list
				if(obj_type!=ObjectType::Column)
					obj_oids[obj_type].push_back((*itr)->data(ObjectId, Qt::UserRole).value<unsigned>());
				//If its a column
				else
				{
					//Get the table's oid from the parent item
					tab_oid=(*itr)->parent()->parent()->data(ObjectId, Qt::UserRole).value<unsigned>();

					//Store the column oid on the selected colums map using the table oid as key
					col_oids[tab_oid].push_back((*itr)->data(ObjectId, Qt::UserRole).value<unsigned>());
				}
			}

			++itr;
		}
	}
	// Retriving selected items from filtered objects grid
	else
	{
		int row_cnt = filtered_objs_tbw->rowCount();
		QTableWidgetItem *oid_item = nullptr, *type_item  = nullptr;

		for(int row = 0; row < row_cnt; row++)
		{
			oid_item = filtered_objs_tbw->item(row, 0);
			type_item = filtered_objs_tbw->item(row, 1);

			if(oid_item->checkState() == Qt::Checked)
			{
				obj_type = static_cast<ObjectType>(type_item->data(Qt::UserRole).toUInt());
				obj_oids[obj_type].push_back(oid_item->data(Qt::UserRole).toUInt());
			}
		}
	}
}

void DatabaseImportForm::listObjects()
{
	try
	{
		if(database_cmb->currentIndex() > 0)
		{
			Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());
			QStringList obj_filter = objs_filter_wgt->getObjectFilters();

			//Set the working database on import helper
			import_helper->closeConnection();
			import_helper->setConnection(*conn);
			import_helper->setCurrentDatabase(database_cmb->currentText());
			import_helper->setImportOptions(import_sys_objs_chk->isChecked(), import_ext_objs_chk->isChecked(),
																			resolve_deps_chk->isChecked(), ignore_errors_chk->isChecked(),
																			debug_mode_chk->isChecked(), rand_rel_color_chk->isChecked(), true);

			import_helper->setObjectFilters(obj_filter,
																			objs_filter_wgt->isOnlyMatching(),
																			objs_filter_wgt->getForceObjectsFilter());
			if(obj_filter.isEmpty() && import_helper->getCatalog().getObjectCount(false) > ObjectCountThreshould)
			{
				Messagebox msgbox;
				msgbox.show(tr("The selected database seems to have a huge amount of objects! \
Trying to import such database can take minutes or even hours and, in extreme cases, crash the application. \
Please, consider using the <strong>Filter</strong> tab in order to refine the set of objects to be imported. \
Do you really want to proceed?"),
										Messagebox::AlertIcon, Messagebox::YesNoButtons);

				if(msgbox.result() == Messagebox::Rejected)
				{
					database_cmb->setCurrentIndex(0);
					return;
				}
			}

			/* If the filter is set and the non matches need to be ignored
			 * switches to the strict view of listing filtered objects */
			if(!obj_filter.isEmpty() && objs_filter_wgt->isOnlyMatching())
			{
				db_objects_tw->clear();
				db_objects_stw->setCurrentIndex(1);
				listFilteredObjects();
			}
			else
			{
				//List the objects using the static helper method
				db_objects_tw->clear();
				filtered_objs_tbw->clearContents();
				filtered_objs_tbw->setRowCount(0);
				db_objects_stw->setCurrentIndex(0);
				DatabaseImportForm::listObjects(*import_helper, db_objects_tw, true, true, false);
				objs_parent_wgt->setEnabled(db_objects_tw->topLevelItemCount() > 0);
			}
		}

		import_btn->setEnabled(hasCheckedItems());
		buttons_wgt->setEnabled(db_objects_tw->topLevelItemCount() > 0 || filtered_objs_tbw->rowCount() > 0);
		expand_all_tb->setVisible(db_objects_stw->currentIndex() == 0);
		collapse_all_tb->setVisible(db_objects_stw->currentIndex() == 0);
	}
	catch(Exception &e)
	{
		import_btn->setEnabled(false);
		objs_parent_wgt->setEnabled(false);
		buttons_wgt->setEnabled(false);
		filtered_objs_tbw->setEnabled(false);
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportForm::listDatabases()
{
	try
	{
		//Close a previous connection opened by the import helper
		import_helper->closeConnection();

		if(connections_cmb->currentIndex()==connections_cmb->count()-1)
		{
			ConnectionsConfigWidget::openConnectionsConfiguration(connections_cmb, true);
			emit s_connectionsUpdateRequest();
		}

		Connection *conn=reinterpret_cast<Connection *>(connections_cmb->itemData(connections_cmb->currentIndex()).value<void *>());

		if(conn)
		{
			//List the available databases using the selected connection
			import_helper->setConnection(*conn);
			DatabaseImportForm::listDatabases(*import_helper, database_cmb);
		}
		else
			database_cmb->clear();

		db_objects_tw->clear();
		database_cmb->setEnabled(database_cmb->count() > 1);

	}
	catch(Exception &e)
	{
		db_objects_tw->clear();
		database_cmb->clear();
		database_cmb->setEnabled(false);
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DatabaseImportForm::closeEvent(QCloseEvent *event)
{
	/* Ignore the close event when the thread is running this avoid
	close the form and make thread execute in background */
	if(import_thread->isRunning())
		event->ignore();
	else
	{
		if(create_model && !model_wgt)
			this->setResult(QDialog::Rejected);

		import_helper->closeConnection();
	}
}

void DatabaseImportForm::captureThreadError(Exception e)
{
	QPixmap ico;
	QTreeWidgetItem *item=nullptr;

	if(!create_model)
		model_wgt->rearrangeSchemasInGrid();

	destroyModelWidget();
	finishImport(tr("Importing process aborted!"));

	ico=QPixmap(PgModelerUiNs::getIconPath("msgbox_erro"));
	ico_lbl->setPixmap(ico);

	item=PgModelerUiNs::createOutputTreeItem(output_trw, PgModelerUiNs::formatMessage(e.getErrorMessage()), ico, nullptr, false, true);
	PgModelerUiNs::createExceptionsTree(output_trw, e, item);

	//Destroy the current import thread and helper to avoid reuse
	destroyThread();

	//Recreates a new import thread and helper to force user to reconfigure the import
	createThread();

	database_cmb->setCurrentIndex(0);
	throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
}

void DatabaseImportForm::filterObjects()
{
	DatabaseImportForm::filterObjects(db_objects_tw,
																		filter_edt->text(),
																		(by_oid_chk->isChecked() ? ObjectId : 0), false);
}

void DatabaseImportForm::filterObjects(QTreeWidget *tree_wgt, const QString &pattern, int search_column, bool sel_single_leaf)
{
	if(!tree_wgt)
		throw Exception(ErrorCode::OprNotAllocatedObject ,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	QList<QTreeWidgetItem*> items;
	QTreeWidgetItemIterator itr(tree_wgt);

	if(search_column == DatabaseImportForm::ObjectId)
		items = tree_wgt->findItems(QString("^(0)*(%1)(.)*").arg(pattern), Qt::MatchRegExp | Qt::MatchRecursive, search_column);
	else
		items = tree_wgt->findItems(pattern, Qt::MatchStartsWith | Qt::MatchRecursive, search_column);

	tree_wgt->blockSignals(true);
	tree_wgt->collapseAll();
	tree_wgt->clearSelection();

	while(*itr)
	{
		(*itr)->setHidden(!pattern.isEmpty());
		++itr;
	}

	if(pattern.isEmpty())
	{
		tree_wgt->topLevelItem(0)->setExpanded(true);
	}
	else
	{
		QTreeWidgetItem *parent=nullptr, *item=nullptr, *leaf=nullptr;
		int leaf_count=0;

		while(!items.isEmpty())
		{
			item=items.front();
			item->setExpanded(true);
			item->setHidden(false);
			parent=item->parent();

			while(parent)
			{
				parent->setHidden(false);
				parent->setExpanded(true);
				parent=parent->parent();
			}

			items.pop_front();

			//Counting the leaf items found so far
			if(sel_single_leaf && item->childCount()==0 && item->parent())
			{
				leaf_count++;
				leaf=item;
			}
		}

		//Selecting the single leaf item
		if(sel_single_leaf && leaf_count == 1 && leaf)
		{
			leaf->setSelected(true);
			tree_wgt->setCurrentItem(leaf);
		}
	}

	tree_wgt->blockSignals(false);
}

void DatabaseImportForm::cancelImport()
{
	import_helper->cancelImport();
	cancel_btn->setEnabled(false);
	database_cmb->setCurrentIndex(0);
}

void DatabaseImportForm::destroyModelWidget()
{
	if(create_model && model_wgt)
	{
		delete model_wgt;
		model_wgt=nullptr;
	}
}

void DatabaseImportForm::handleImportCanceled()
{
	QPixmap ico=QPixmap(PgModelerUiNs::getIconPath("msgbox_alerta"));
	QString msg=tr("Importing process canceled by user!");

	if(!create_model)
		model_wgt->rearrangeSchemasInGrid();

	destroyModelWidget();
	finishImport(msg);
	ico_lbl->setPixmap(ico);

	PgModelerUiNs::createOutputTreeItem(output_trw, msg, ico);
}

void DatabaseImportForm::handleImportFinished(Exception e)
{	
	if(!e.getErrorMessage().isEmpty())
	{
		Messagebox msgbox;
		msgbox.show(e, e.getErrorMessage(), Messagebox::AlertIcon);
	}

	model_wgt->rearrangeSchemasInGrid();
	model_wgt->getDatabaseModel()->setInvalidated(false);

	ico_lbl->setPixmap(QPixmap(PgModelerUiNs::getIconPath("msgbox_info")));
	finishImport(tr("Importing process sucessfuly ended!"));

	import_helper->closeConnection();
	import_thread->quit();
	import_thread->wait();
	this->accept();
}

void DatabaseImportForm::finishImport(const QString &msg)
{
	if(import_thread->isRunning())
		import_thread->quit();

	cancel_btn->setEnabled(false);
	options_tbw->setEnabled(true);
	database_gb->setEnabled(true);
	progress_pb->setValue(100);
	progress_lbl->setText(msg);
	progress_lbl->repaint();

	if(model_wgt)
	{
		model_wgt->setUpdatesEnabled(true);

		if(!create_model)
			model_wgt->getOperationList()->removeOperations();
	}
}

void DatabaseImportForm::showEvent(QShowEvent *)
{
	ConnectionsConfigWidget::fillConnectionsComboBox(connections_cmb, true, Connection::OpImport);
	createThread();

	if(connections_cmb->currentIndex() > 0)
		listDatabases();
}

ModelWidget *DatabaseImportForm::getModelWidget()
{
	if(create_model)
		return model_wgt;
	else
		return nullptr;
}

void DatabaseImportForm::listDatabases(DatabaseImportHelper &import_helper, QComboBox *dbcombo)
{
	if(dbcombo)
	{
		try
		{
			attribs_map db_attribs;
			attribs_map::iterator itr;
			QStringList list;
			map<QString, unsigned> oids;

			db_attribs=import_helper.getObjects(ObjectType::Database);
			dbcombo->blockSignals(true);
			dbcombo->clear();

			if(db_attribs.empty())
				dbcombo->addItem(tr("No databases found"));
			else
			{
				itr=db_attribs.begin();
				while(itr!=db_attribs.end())
				{
					list.push_back(itr->second);
					oids[itr->second]=itr->first.toUInt();
					itr++;
				}

				list.sort();
				dbcombo->addItems(list);

				for(int i=0; i < list.count(); i++)
				{
					dbcombo->setItemIcon(i, QPixmap(PgModelerUiNs::getIconPath(ObjectType::Database)));
					dbcombo->setItemData(i, oids[list[i]]);
				}

				dbcombo->insertItem(0, tr("Found %1 database(s)").arg(db_attribs.size()));
			}

			dbcombo->setCurrentIndex(0);
			dbcombo->blockSignals(false);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void DatabaseImportForm::listObjects(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, bool checkable_items,
																		 bool disable_empty_grps, bool create_db_item, bool create_dummy_item, int sort_by)
{
	TaskProgressWidget task_prog_wgt;

	try
	{
		if(tree_wgt)
		{
			QTreeWidgetItem *db_item=nullptr, *item=nullptr;
			vector<QTreeWidgetItem *> sch_items, tab_items;
			double inc=0, inc1=0, aux_prog=0;

			if(!create_dummy_item)
			{
				task_prog_wgt.setWindowTitle(tr("Retrieving objects from database..."));
				task_prog_wgt.show();
				task_prog_wgt.updateProgress(1, tr("Retrieving cluster level objects..."), enum_cast(ObjectType::Database));
			}

			tree_wgt->clear();
			tree_wgt->setColumnHidden(1, true);

			if(create_db_item)
			{
				Catalog catalog=import_helper.getCatalog();
				vector<attribs_map> attribs;

				//Creating database item
				db_item=new QTreeWidgetItem;
				db_item->setText(0, import_helper.getCurrentDatabase());
				db_item->setIcon(0, QPixmap(PgModelerUiNs::getIconPath(ObjectType::Database)));
				attribs=catalog.getObjectsAttributes(ObjectType::Database, QString(), QString(), {}, {{Attributes::Name, import_helper.getCurrentDatabase()}});

				db_item->setData(ObjectId, Qt::UserRole, attribs[0].at(Attributes::Oid).toUInt());
				db_item->setData(ObjectTypeId, Qt::UserRole, enum_cast(ObjectType::Database));

				db_item->setToolTip(0, QString("OID: %1").arg(attribs[0].at(Attributes::Oid)));
				tree_wgt->addTopLevelItem(db_item);
			}

			//Retrieving and listing the cluster scoped objects
			sch_items=DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
																											BaseObject::getChildObjectTypes(ObjectType::Database),
																											checkable_items, disable_empty_grps, db_item);

			if(create_dummy_item)
			{
				while(!sch_items.empty())
				{
					item=new QTreeWidgetItem(sch_items.back());
					item->setText(0, QString("..."));
					item->setData(ObjectOtherData, Qt::UserRole, QVariant::fromValue<int>(-1));
					sch_items.pop_back();
				}
			}
			else
			{
				ObjectType obj_type = ObjectType::BaseObject;
				aux_prog=task_prog_wgt.progress_pb->value();
				inc=40/static_cast<double>(sch_items.size());

				while(!sch_items.empty())
				{
					task_prog_wgt.updateProgress(static_cast<int>(aux_prog), tr("Retrieving objects of schema `%1'...").arg(sch_items.back()->text(0)), enum_cast(ObjectType::Schema));

					//Retrieving and listing the schema scoped objects
					tab_items=DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
																													BaseObject::getChildObjectTypes(ObjectType::Schema),
																													checkable_items, disable_empty_grps, sch_items.back(), sch_items.back()->text(0));

					inc1=(60/static_cast<double>(tab_items.size()))/static_cast<double>(sch_items.size());

					while(!tab_items.empty())
					{
						aux_prog+=inc1;
						if(aux_prog > 99)	aux_prog=99;

						obj_type = static_cast<ObjectType>(tab_items.back()->data(ObjectTypeId, Qt::UserRole).toUInt());
						task_prog_wgt.updateProgress(static_cast<int>(aux_prog), tr("Retrieving objects of `%1' (%2)...").arg(tab_items.back()->text(0)).arg(BaseObject::getTypeName(obj_type)), enum_cast(obj_type));
						DatabaseImportForm::updateObjectsTree(import_helper, tree_wgt,
																									BaseObject::getChildObjectTypes(obj_type), checkable_items, disable_empty_grps,
																									tab_items.back(), sch_items.back()->text(0), tab_items.back()->text(0));
						tab_items.pop_back();
					}

					aux_prog+=inc;
					if(aux_prog > 99)	aux_prog=99;

					task_prog_wgt.progress_pb->setValue(static_cast<int>(aux_prog));
					sch_items.pop_back();
				}
			}

			/*if(checkable_items)
			{
				map<ObjectType, QStringList> objs_filter = import_helper.getObjectFilters();

				// If we have filters configured only the items matching the object types are checked (and their parents too)
				if(!objs_filter.empty())
				{
					ObjectType obj_type;
					QList<QTreeWidgetItem *> list = tree_wgt->findItems("*", Qt::MatchWrap | Qt::MatchWildcard | Qt::MatchRecursive);

					for(auto &item : list)
					{
						obj_type = static_cast<ObjectType>(item->data(ObjectTypeId, Qt::UserRole).toUInt());

						if(!item->isDisabled() && objs_filter.count(obj_type))
							item->setCheckState(0, Qt::Checked);
					}
				}
			}*/

			tree_wgt->sortItems(sort_by, Qt::AscendingOrder);

			if(db_item)
				db_item->setExpanded(true);

			if(!create_dummy_item)
			{
				task_prog_wgt.progress_pb->setValue(100);
				task_prog_wgt.close();
			}
		}

	}
	catch(Exception &e)
	{
		task_prog_wgt.close();
		tree_wgt->clear();
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

vector<QTreeWidgetItem *> DatabaseImportForm::updateObjectsTree(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, vector<ObjectType> types, bool checkable_items,
																																bool disable_empty_grps, QTreeWidgetItem *root, const QString &schema, const QString &table)
{
	vector<QTreeWidgetItem *> items_vect;

	if(tree_wgt)
	{
		QTreeWidgetItem *group=nullptr, *item=nullptr;
		QFont grp_fnt=tree_wgt->font();
		attribs_map extra_attribs={{Attributes::FilterTableTypes, Attributes::True}};
		QString tooltip=QString("OID: %1"), name, label;
		bool child_checked=false;
		vector<attribs_map> objects_vect;
		map<ObjectType, QTreeWidgetItem *> gen_groups;
		ObjectType obj_type;
		QList<QTreeWidgetItem*> groups_list;
		unsigned oid=0;
		int start=-1, end=-1;
		//bool has_obj_filters = !import_helper.getObjectFilters().empty();

		grp_fnt.setItalic(true);
		tree_wgt->blockSignals(true);
		tree_wgt->setUpdatesEnabled(false);
		tree_wgt->setSortingEnabled(false);

		try
		{
			for(ObjectType grp_type : types)
			{
				//Create a group item for the current type
				group=new QTreeWidgetItem(root);
				group->setIcon(0, QPixmap(PgModelerUiNs::getIconPath(BaseObject::getSchemaName(grp_type) + QString("_grp"))));
				group->setFont(0, grp_fnt);

				//Group items does contains a zero valued id to indicate that is not a valide object
				group->setData(ObjectId, Qt::UserRole, 0);
				group->setData(ObjectTypeId, Qt::UserRole, enum_cast(grp_type));
				group->setData(ObjectCount, Qt::UserRole, 0);
				group->setData(ObjectSchema, Qt::UserRole, schema);
				group->setData(ObjectTable, Qt::UserRole, table);

				gen_groups[grp_type]=group;
				groups_list.push_back(group);
			}

			objects_vect = import_helper.getObjects(types, schema, table, extra_attribs);

			for(attribs_map &attribs : objects_vect)
			{
				obj_type=static_cast<ObjectType>(attribs[Attributes::ObjectType].toUInt());
				group=gen_groups[obj_type];
				group->setData(ObjectCount, Qt::UserRole,
											 group->data(ObjectCount, Qt::UserRole).toUInt() + 1);

				//Creates individual items for each object of the current type
				oid=attribs[Attributes::Oid].toUInt();

				attribs[Attributes::Name].remove(QRegExp(QString("( )(without)( time zone)")));
				label=name=attribs[Attributes::Name];

				//Removing the trailing type string from op. families or op. classes names
				if(obj_type==ObjectType::OpFamily || obj_type==ObjectType::OpClass)
				{
					start=name.indexOf(QChar('['));
					end=name.lastIndexOf(QChar(']'));
					name.remove(start, (end-start)+1);
					name=name.trimmed();
				}

				item=new QTreeWidgetItem(group);
				item->setIcon(0, QPixmap(PgModelerUiNs::getIconPath(obj_type)));
				item->setText(0, label);
				item->setText(ObjectId, attribs[Attributes::Oid].rightJustified(10, '0'));
				item->setData(ObjectId, Qt::UserRole, attribs[Attributes::Oid].toUInt());
				item->setData(ObjectName, Qt::UserRole, name);

				if(checkable_items)
				{
					/* If the current import helper has objects filter we will not mark the items in the tree as check
					 * since only the ones matching the object types are checked in the final step of the tree creation */
					if(/*!has_obj_filters &&*/
						 ((oid > import_helper.getLastSystemOID()) ||
							(obj_type==ObjectType::Schema && name==QString("public")) ||
							(obj_type==ObjectType::Column && root && root->data(0, Qt::UserRole).toUInt() > import_helper.getLastSystemOID())))
					{
						item->setCheckState(0, Qt::Checked);
						child_checked=true;
					}
					else
						item->setCheckState(0, Qt::Unchecked);

					//Disabling items that refers to PostgreSQL's built-in data types
					if(obj_type==ObjectType::Type && oid <= import_helper.getLastSystemOID())
					{
						item->setDisabled(true);
						item->setToolTip(0, tr("This is a PostgreSQL built-in data type and cannot be imported."));
					}
					//Disabling items that refers to pgModeler's built-in system objects
					else if((obj_type==ObjectType::Tablespace && (name==QString("pg_default") || name==QString("pg_global"))) ||
									(obj_type==ObjectType::Role && (name==QString("postgres"))) ||
									(obj_type==ObjectType::Schema && (name==QString("pg_catalog") || name==QString("public"))) ||
									(obj_type==ObjectType::Language && (name.toLower() == DefaultLanguages::C ||
																											name.toLower() == DefaultLanguages::Sql ||
																											name.toLower() == DefaultLanguages::PlPgsql)))
					{
						item->setFont(0, grp_fnt);
						item->setForeground(0, BaseObjectView::getFontStyle(Attributes::ProtColumn).foreground());
						item->setToolTip(0, tr("This is a pgModeler's built-in object. It will be ignored if checked by user."));
					}
				}

				//Stores the object's OID as the first data of the item
				item->setData(ObjectId, Qt::UserRole, oid);

				if(!item->toolTip(0).isEmpty())
					item->setToolTip(0,item->toolTip(0) + QString("\n") + tooltip.arg(oid));
				else
					item->setToolTip(0,tooltip.arg(oid));

				//Stores the object's type as the second data of the item
				item->setData(ObjectTypeId, Qt::UserRole, enum_cast(obj_type));

				//Stores the schema and the table's name of the object
				item->setData(ObjectSchema, Qt::UserRole, schema);
				item->setData(ObjectTable, Qt::UserRole, table);

				if(obj_type==ObjectType::Schema || BaseTable::isBaseTable(obj_type))
					items_vect.push_back(item);
			}

			//Updating the object count in each group
			for(ObjectType grp_type : types)
			{
				group=gen_groups[grp_type];
				group->setDisabled(disable_empty_grps && group->data(ObjectCount, Qt::UserRole).toUInt() == 0);
				group->setText(0, BaseObject::getTypeName(grp_type) + QString(" (%1)").arg(group->data(ObjectCount, Qt::UserRole).toUInt()));

				if(checkable_items)
				{
					if(!group->isDisabled() && child_checked)
						group->setCheckState(0, Qt::Checked);
					else
						group->setCheckState(0, Qt::Unchecked);
				}
			}

			tree_wgt->addTopLevelItems(groups_list);
			tree_wgt->setUpdatesEnabled(true);
			tree_wgt->blockSignals(false);
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
	return items_vect;
}
