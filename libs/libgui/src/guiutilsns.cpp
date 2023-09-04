/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include "guiutilsns.h"
#include "messagebox.h"
#include "databasemodel.h"
#include "widgets/numberedtexteditor.h"
#include "baseform.h"
#include "widgets/columndatawidget.h"
#include "utilsns.h"
#include "objectstablewidget.h"
#include "generalconfigwidget.h"
#include "appearanceconfigwidget.h"
#include "objectslistmodel.h"

namespace GuiUtilsNs {

	NumberedTextEditor *createNumberedTextEditor(QWidget *parent, bool handle_ext_files)
	{
		NumberedTextEditor *editor=new NumberedTextEditor(parent, handle_ext_files);

		if(parent && !parent->layout())
		{
			QHBoxLayout *layout=new QHBoxLayout(parent);
			layout->setContentsMargins(0,0,0,0);
			layout->addWidget(editor);
		}

		return editor;
	}

	QTreeWidgetItem *createOutputTreeItem(QTreeWidget *output_trw, const QString &text, const QPixmap &ico, QTreeWidgetItem *parent, bool expand_item, bool word_wrap)
	{
		if(!output_trw)
			throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		QTreeWidgetItem *item=nullptr;

		item=new QTreeWidgetItem(parent);
		item->setIcon(0, ico);

		if(!parent)
			output_trw->insertTopLevelItem(output_trw->topLevelItemCount(), item);

		if(!word_wrap)
			item->setText(0, text);
		else
		{
			QLabel *label=new QLabel;
			label->setUpdatesEnabled(false);
			label->setTextFormat(Qt::AutoText);
			label->setText(text);
			label->setWordWrap(true);
			label->setTextInteractionFlags(Qt::TextSelectableByMouse);
			label->setUpdatesEnabled(true);
			label->setMinimumHeight(output_trw->iconSize().height() * 1.5);
			label->setMaximumHeight(label->heightForWidth(label->width()));

			item->setSizeHint(0, QSize(label->width(), label->minimumHeight()));
			output_trw->setItemWidget(item, 0, label);
		}

		item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		item->setExpanded(expand_item);
		item->setHidden(false);
		output_trw->scrollToBottom();

		return item;
	}

	void createOutputListItem(QListWidget *output_lst, const QString &text, const QPixmap &ico, bool is_formated)
	{
		if(!output_lst)
			throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		QListWidgetItem *item=new QListWidgetItem;

		item->setIcon(ico);
		output_lst->addItem(item);

		if(!is_formated)
			item->setText(text);
		else
		{
			QLabel *label=new QLabel;
			int txt_height = 0;

			txt_height = output_lst->fontMetrics().height() * text.count("<br/>");

			if(txt_height == 0)
				txt_height = output_lst->fontMetrics().height() * 1.25;
			else
				txt_height *= 1.05;

			label->setUpdatesEnabled(false);
			label->setTextFormat(Qt::AutoText);
			label->setText(text);
			label->setWordWrap(true);
			label->setTextInteractionFlags(Qt::TextSelectableByMouse);
			label->setUpdatesEnabled(true);

			item->setSizeHint(QSize(output_lst->width(), txt_height));
			output_lst->setItemWidget(item, label);
		}
	}

	void disableObjectSQL(BaseObject *object, bool disable)
	{
		if(object && object->getObjectType()!=ObjectType::BaseRelationship)
		{
			Messagebox msgbox;
			ObjectType obj_type=object->getObjectType();
			bool curr_val=object->isSQLDisabled();
			TableObject *tab_obj = dynamic_cast<TableObject *>(object);

			if(object->isSystemObject())
				throw Exception(Exception::getErrorMessage(ErrorCode::OprReservedObject)
								.arg(object->getName(true))
								.arg(object->getTypeName()),
								ErrorCode::OprReservedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			object->setSQLDisabled(disable);

			if(tab_obj && tab_obj->getParentTable())
				tab_obj->getParentTable()->setModified(true);

			if(obj_type!=ObjectType::Database && curr_val!=disable)
			{
				msgbox.show(QString(QT_TR_NOOP("Do you want to apply the <strong>SQL %1 status</strong> to the object's references too? This will avoid problems when exporting or validating the model.")).arg(disable ? QT_TR_NOOP("disabling") : QT_TR_NOOP("enabling")),
							Messagebox::ConfirmIcon, Messagebox::YesNoButtons);

				if(msgbox.result()==QDialog::Accepted)
					disableReferencesSQL(object);
			}

			/* Special case for tables. When disable the code there is the need to disable constraints
			 * codes when the code of parent table is disabled too in order to avoid export errors */
			if(PhysicalTable::isPhysicalTable(object->getObjectType()))
			{
				Constraint *constr = nullptr;
				std::vector<TableObject *> *objects=dynamic_cast<PhysicalTable *>(object)->getObjectList(ObjectType::Constraint);

				for(auto &obj : (*objects))
				{
					constr=dynamic_cast<Constraint *>(obj);

					/* If the constraint is not FK but is declared outside table via alter (ALTER TABLE...ADD CONSTRAINT...) or
					 * The constraint is FK and the reference table is disabled the FK will not be enabled */
					if((constr->getConstraintType()!=ConstraintType::ForeignKey && !constr->isDeclaredInTable()) ||
							(constr->getConstraintType()==ConstraintType::ForeignKey &&
							 (disable || (!disable && !constr->getReferencedTable()->isSQLDisabled()))))
						constr->setSQLDisabled(disable);
				}
			}
		}
	}

	void disableReferencesSQL(BaseObject *object)
	{
		if(object && object->getDatabase())
		{
			std::vector<BaseObject *> refs;
			TableObject *tab_obj=nullptr;
			DatabaseModel *model=dynamic_cast<DatabaseModel *>(object->getDatabase());

			model->getObjectReferences(object, refs);

			while(!refs.empty())
			{
				tab_obj=dynamic_cast<TableObject *>(refs.back());

				//If the object is a relationship added does not do anything since the relationship itself will be disabled
				if(refs.back()->getObjectType()!=ObjectType::BaseRelationship &&
						(!tab_obj || (tab_obj && !tab_obj->isAddedByRelationship())))
				{
					refs.back()->setSQLDisabled(object->isSQLDisabled());

					//Update the parent table graphical representation to show the disabled child object
					if(tab_obj)
						tab_obj->getParentTable()->setModified(true);

					//Disable the references of the current object too
					disableReferencesSQL(refs.back());
				}

				refs.pop_back();
			}
		}
	}

	QString formatMessage(const QString &msg)
	{
		QString fmt_msg=msg;
		QChar start_chrs[2]={'`','('},
				end_chrs[2]={'\'', ')'};
		QStringList start_tags={ "<strong>", "<em>(" },
				end_tags={ "</strong>", ")</em>" };
		int pos=-1, pos1=-1;

		// Replacing the form `' by <strong></strong> and () by <em></em>
		for(int chr_idx=0; chr_idx < 2; chr_idx++)
		{
			pos=0;
			do
			{
				pos=fmt_msg.indexOf(start_chrs[chr_idx], pos);
				pos1=fmt_msg.indexOf(end_chrs[chr_idx], pos);

				if(pos >= 0 && pos1 >=0)
				{
					fmt_msg.replace(pos, 1 , start_tags[chr_idx]);
					pos1 += start_tags[chr_idx].length() - 1;
					fmt_msg.replace(pos1, 1, end_tags[chr_idx]);
				}
				else
					break;

				pos=pos1;
			}
			while(pos >= 0 && pos < fmt_msg.size());
		}

		fmt_msg.replace("\n", "<br/>");

		return fmt_msg;
	}

	void configureWidgetFont(QWidget *widget, FontFactorId factor_id)
	{
		double factor = 1;

		switch(factor_id)
		{
			case SmallFontFactor:
				factor=0.80;
			break;
			case MediumFontFactor:
				factor=0.90;
			break;
			case BigFontFactor:
				factor=1.10;
			break;
			case HugeFontFactor:
			default:
				factor=1.40;
			break;
		}

		__configureWidgetFont(widget, factor);
	}

	void __configureWidgetFont(QWidget *widget, double factor)
	{
		if(!widget)
			return;

		QFont font=widget->font();
		font.setPointSizeF(font.pointSizeF() * factor);
		widget->setFont(font);
	}

	void createExceptionsTree(QTreeWidget *exceptions_trw, Exception &e, QTreeWidgetItem *root)
	{
		std::vector<Exception> list;
		std::vector<Exception>::reverse_iterator itr, itr_end;
		QString text;
		int idx=0;
		QTreeWidgetItem *item=nullptr, *child_item=nullptr;

		if(!exceptions_trw)
			return;

		e.getExceptionsList(list);
		itr = list.rbegin();
		itr_end = list.rend();

		while(itr != itr_end)
		{
			text=QString("[%1] - %2").arg(idx).arg(itr->getMethod());
			item=createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("function1")), root, false, true);

			text=QString("%1 (%2)").arg(itr->getFile()).arg(itr->getLine());
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("sourcecode")), item, false, true);

			text=QString("%1 (%2)").arg(Exception::getErrorCode(itr->getErrorCode())).arg(enum_t(itr->getErrorCode()));
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("alert")), item, false, true);

			child_item=createOutputTreeItem(exceptions_trw, itr->getErrorMessage(), QPixmap(getIconPath("error")), item, false, true);
			exceptions_trw->itemWidget(child_item, 0)->setStyleSheet("color: #ff0000;");

			if(!itr->getExtraInfo().isEmpty())
				child_item=createOutputTreeItem(exceptions_trw, itr->getExtraInfo(), QPixmap(getIconPath("info")), item, false, true);

			idx++; itr++;

			/* If we have a stack bigger than 50 items we just ignore the rest in order to avoid
			 * the production or reduntant/useless information on the exception message box */
			if(static_cast<unsigned>(idx) >= Exception::MaximumStackSize)
			{
				text = QT_TR_NOOP("Another %1 error(s) were suppressed due to stacktrace size limits.");
				text = text.arg(list.size() - idx);
				createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("alert")), item, false, false);
				break;
			}
		}
	}

	QString getIconPath(const QString &icon)
	{
		return QString(":/icons/icons/%1.png").arg(icon);
	}

	QString getIconPath(ObjectType obj_type, int sub_type)
	{
		QString suffix;

		if(sub_type >= 0)
		{
			if(obj_type == ObjectType::BaseRelationship || obj_type == ObjectType::Relationship)
			{
				BaseRelationship::RelType rel_type = static_cast<BaseRelationship::RelType>(sub_type);

				if(obj_type == ObjectType::BaseRelationship)
				{
					if(rel_type==BaseRelationship::RelationshipFk)
						suffix = "fk";
					else
						suffix = "tv";
				}
				else if(rel_type == BaseRelationship::Relationship11)
					suffix = "11";
				else if(rel_type == BaseRelationship::Relationship1n)
					suffix = "1n";
				else if(rel_type == BaseRelationship::RelationshipNn)
					suffix = "nn";
				else if(rel_type == BaseRelationship::RelationshipDep)
					suffix = "dep";
				else if(rel_type == BaseRelationship::RelationshipGen)
					suffix = "gen";
			}
			else if(obj_type==ObjectType::Constraint)
			{
				if(sub_type == ConstraintType::PrimaryKey)
					suffix = QString("_%1").arg(TableObjectView::TextPrimaryKey);
				else if(sub_type == ConstraintType::ForeignKey)
					suffix = QString("_%1").arg(TableObjectView::TextForeignKey);
				else if(sub_type == ConstraintType::Check)
					suffix = QString("_%1").arg(TableObjectView::TextCheck);
				else if(sub_type == ConstraintType::Unique)
					suffix = QString("_%1").arg(TableObjectView::TextUnique);
				else if(sub_type == ConstraintType::Exclude)
					suffix = QString("_%1").arg(TableObjectView::TextExclude);
			}
		}

		return getIconPath(BaseObject::getSchemaName(obj_type) + suffix);
	}

	void resizeDialog(QWidget *widget)
	{
		if(!widget)
			return;

		int max_h = 0, curr_w =0, curr_h = 0;
		QScreen *screen = qApp->primaryScreen();
		QSize min_size = widget->minimumSize(),
				screen_sz = screen->size();

		max_h = screen_sz.height() * 0.70;

		/* If the widget's minimum size is zero then we need to do
				a size adjustment on the widget prior to insert it into the dialog */
		if(min_size.height() <= 0 || min_size.width() <= 0)
		{
			widget->adjustSize();
			min_size=widget->size();
		}

		widget->adjustSize();
		curr_h=widget->height();
		curr_w=min_size.width();

		// If the current height is greater than the widget's minimum height we will use a medium value
		if(curr_h > min_size.height() && min_size.height() < max_h)
			curr_h = (curr_h + min_size.height())/2.5;
		//Using the maximum height if the widget's minimum height exceeds the maximum allowed
		else if(min_size.height() >= max_h)
			curr_h = max_h;

		/* curr_w *= dpi_factor * pixel_ratio;
		curr_h *= dpi_factor * pixel_ratio; */

		if(curr_w > screen_sz.width())
			curr_w = screen_sz.width() * 0.80;

		if(curr_h > screen_sz.height())
			curr_h = screen_sz.height() * 0.80;

		widget->setMinimumSize(widget->minimumSize());
		widget->resize(curr_w, curr_h);
		//widget->adjustSize();
	}

	void openColumnDataForm(const QModelIndex &index)
	{
		if(!index.isValid())
			return;

		BaseForm base_form;
		ColumnDataWidget *col_data_wgt = new ColumnDataWidget;

		base_form.setMainWidget(col_data_wgt);
		base_form.setButtonConfiguration(Messagebox::OkButton);

		col_data_wgt->setData(index.data().toString());
		col_data_wgt->setReadOnly(true);

		GeneralConfigWidget::restoreWidgetGeometry(&base_form, col_data_wgt->metaObject()->className());
		base_form.exec();
		GeneralConfigWidget::saveWidgetGeometry(&base_form, col_data_wgt->metaObject()->className());
	}

	void openColumnDataForm(QTableWidget *results_tbw)
	{
		if(!results_tbw)
			return;

		BaseForm base_form;
		ColumnDataWidget *col_data_edit_wgt = new ColumnDataWidget;

		base_form.setMainWidget(col_data_edit_wgt);
		base_form.setButtonConfiguration(Messagebox::OkCancelButtons);
		base_form.apply_ok_btn->setShortcut(QKeySequence("Ctrl+Return"));

		if(results_tbw->selectedItems().size() == 1)
			col_data_edit_wgt->setData(results_tbw->currentItem()->text());

		GeneralConfigWidget::restoreWidgetGeometry(&base_form, col_data_edit_wgt->metaObject()->className());
		base_form.exec();
		GeneralConfigWidget::saveWidgetGeometry(&base_form, col_data_edit_wgt->metaObject()->className());

		if(base_form.result() == QDialog::Accepted)
		{
			QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();

			for(auto range : sel_ranges)
			{
				for(int row = range.topRow(); row <= range.bottomRow(); row++)
				{
					for(int col = range.leftColumn(); col <= range.rightColumn(); col++)
					{
						results_tbw->item(row, col)->setText(col_data_edit_wgt->getData());
					}
				}
			}
		}
	}

	void createDropShadow(QWidget *wgt, int x_offset, int y_offset, int radius, const QColor &color)
	{
		QGraphicsDropShadowEffect *shadow=nullptr;

		shadow=new QGraphicsDropShadowEffect(wgt);
		shadow->setXOffset(x_offset);
		shadow->setYOffset(y_offset);
		shadow->setBlurRadius(radius);
		shadow->setColor(color);
		wgt->setGraphicsEffect(shadow);
	}

	void handleFileDialogSatate(QFileDialog *file_dlg, bool save_state)
	{
		if(!file_dlg)
			return;

		try
		{
			QString filename = GlobalAttributes::getConfigurationsPath() +
												 GlobalAttributes::DirSeparator +
												 GlobalAttributes::FileDialogConf +
												 GlobalAttributes::ConfigurationExt;

			QSettings settings(filename, QSettings::NativeFormat);

			if(save_state)
			{
				settings.setValue("geometry", file_dlg->saveGeometry());
				settings.setValue("state", file_dlg->saveState());
				settings.sync();
			}
			else
			{
				file_dlg->restoreGeometry(settings.value("geometry").toByteArray());
				file_dlg->restoreState(settings.value("state").toByteArray());
			}
		}
		catch(Exception &){}
	}

	void saveFileDialogState(QFileDialog *file_dlg)
	{
		handleFileDialogSatate(file_dlg, true);
	}

	void restoreFileDialogState(QFileDialog *file_dlg)
	{
		handleFileDialogSatate(file_dlg, false);
	}

	void updateObjectsTable(QTableView *table_vw, const std::vector<BaseObject *> &objects, const QString &search_attr)
	{
		if(!table_vw)
			return;

		// Scheduling the destruction of the current table view model
		if(table_vw->model())
		{
			table_vw->model()->deleteLater();
			table_vw->setModel(nullptr);
		}

		if(objects.empty())
			return;

		table_vw->setUpdatesEnabled(false);
		table_vw->setSortingEnabled(false);

		// Create a proxy model for sorting purposes
		QSortFilterProxyModel *proxy_model = new QSortFilterProxyModel(table_vw);
		ObjectsListModel *model = new ObjectsListModel(objects, search_attr, proxy_model);

		proxy_model->setSourceModel(model);
		table_vw->setModel(proxy_model);

		table_vw->resizeColumnsToContents();
		table_vw->resizeRowsToContents();
		table_vw->sortByColumn(0, Qt::AscendingOrder);
		table_vw->setUpdatesEnabled(true);
		table_vw->setSortingEnabled(true);
	}

	void updateObjectsTable(QTableView *table_vw, const std::vector<attribs_map> &attribs)
	{
		if(!table_vw)
			return;

		// Scheduling the destruction of the current table view model
		if(table_vw->model())
		{
			table_vw->model()->deleteLater();
			table_vw->setModel(nullptr);
		}

		if(attribs.empty())
			return;

		table_vw->setUpdatesEnabled(false);
		table_vw->setSortingEnabled(false);

						// Create a proxy model for sorting purposes
		QSortFilterProxyModel *proxy_model = new QSortFilterProxyModel(table_vw);
		ObjectsListModel *model = new ObjectsListModel(attribs, proxy_model);

		proxy_model->setSourceModel(model);
		table_vw->setModel(proxy_model);

		table_vw->resizeColumnsToContents();
		table_vw->resizeRowsToContents();
		table_vw->sortByColumn(0, Qt::AscendingOrder);
		table_vw->setUpdatesEnabled(true);
		table_vw->setSortingEnabled(true);
	}

	#warning "Performance bottleneck here!"
	#warning "Function performs pooly with large set of data!"
	#warning "Create a version that updates a QTableView!"
	void updateObjectsTable(QTableWidget *tab_wgt, std::vector<BaseObject *> &objs, const QString &search_attr, bool checkable_items)
	{
		if(!tab_wgt || tab_wgt->columnCount() == 0)
			return;

		unsigned lin_idx = 0;
		QTableWidgetItem *tab_item=nullptr;
		BaseObject *parent_obj=nullptr;
		QFont fnt;
		QString str_aux;
		bool new_row = false;

		tab_wgt->setUpdatesEnabled(false);
		tab_wgt->setSortingEnabled(false);

		for(auto &obj : objs)
		{
			if(obj->getObjectType()==ObjectType::BaseRelationship)
				str_aux = "tv";
			else
				str_aux.clear();

			new_row = false;

			if(static_cast<int>(lin_idx) >= tab_wgt->rowCount())
			{
				tab_wgt->insertRow(lin_idx);
				new_row = true;
			}

			//First column: Object name
			tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 0));
			tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(obj)));
			fnt=tab_item->font();

			tab_item->setText(obj->getName());
			tab_item->setIcon(QIcon(GuiUtilsNs::getIconPath(BaseObject::getSchemaName(obj->getObjectType()) + str_aux)));
			if(new_row) tab_wgt->setItem(lin_idx, 0, tab_item);
			if(checkable_items)	tab_item->setCheckState(Qt::Checked);

			if(obj->isProtected() || obj->isSystemObject())
			{
				fnt.setItalic(true);
				tab_item->setForeground(ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor));
			}
			else if(dynamic_cast<TableObject *>(obj) &&
					dynamic_cast<TableObject *>(obj)->isAddedByRelationship())
			{
				fnt.setItalic(true);
				tab_item->setForeground(ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::RelAddedItemAltFgColor));
			}
			else
				fnt.setItalic(false);

			fnt.setStrikeOut(obj->isSQLDisabled() && !obj->isSystemObject());
			tab_item->setFont(fnt);
			fnt.setStrikeOut(false);

			//Second column: Object type
			if(tab_wgt->columnCount() > 1)
			{
				fnt.setItalic(true);
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 1));
				tab_item->setFont(fnt);
				tab_item->setText(obj->getTypeName());
				if(new_row) tab_wgt->setItem(lin_idx, 1, tab_item);
			}

			//Third column: Object id
			if(tab_wgt->columnCount() > 2)
			{
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 2));
				tab_item->setText(QString::number(obj->getObjectId()));
				if(new_row) tab_wgt->setItem(lin_idx, 2, tab_item);
			}

			//Fourth column: Parent object name
			if(tab_wgt->columnCount() > 3)
			{
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 3));

				if(dynamic_cast<TableObject *>(obj))
					parent_obj=dynamic_cast<TableObject *>(obj)->getParentTable();
				else if(obj->getSchema())
					parent_obj=obj->getSchema();
				else if(dynamic_cast<Permission *>(obj))
					parent_obj=dynamic_cast<Permission *>(obj)->getObject();
				else
					parent_obj=obj->getDatabase();

				tab_item->setText(parent_obj ? parent_obj->getName() : "-");
				tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(reinterpret_cast<void *>(parent_obj)));
				if(new_row) tab_wgt->setItem(lin_idx, 3, tab_item);

				if(parent_obj)
				{
					if(parent_obj->isProtected() || parent_obj->isSystemObject())
					{
						fnt.setItalic(true);
						tab_item->setForeground(ObjectsTableWidget::getTableItemColor(ObjectsTableWidget::ProtItemAltFgColor));
					}
					else
						fnt.setItalic(false);

					tab_item->setFont(fnt);
					tab_item->setIcon(QIcon(GuiUtilsNs::getIconPath(parent_obj->getObjectType())));
				}
			}

			//Fifth column: Parent object type
			if(tab_wgt->columnCount() > 4)
			{
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 4));
				fnt.setItalic(true);
				tab_item->setFont(fnt);
				tab_item->setText(parent_obj ? parent_obj->getTypeName() : "-");
				if(new_row) tab_wgt->setItem(lin_idx, 4, tab_item);
			}

			//Sixth column: object comment
			if(tab_wgt->columnCount() > 5)
			{
				attribs_map search_attribs = obj->getSearchAttributes();
				tab_item=(new_row ? new QTableWidgetItem : tab_wgt->item(lin_idx, 5));
				fnt.setItalic(false);
				tab_item->setFont(fnt);

				if(search_attr != Attributes::Name &&
					 search_attr != Attributes::Schema &&
					 search_attr != Attributes::Comment)
					tab_item->setText(search_attribs[search_attr]);
				else
					tab_item->setText(obj->getComment());

				if(new_row) tab_wgt->setItem(lin_idx, 5, tab_item);
			}

			lin_idx++;
		}

		if(static_cast<int>(objs.size()) != tab_wgt->rowCount())
			tab_wgt->setRowCount(objs.size());

		tab_wgt->setUpdatesEnabled(true);
		tab_wgt->setSortingEnabled(true);
		tab_wgt->resizeColumnsToContents();
		tab_wgt->resizeRowsToContents();
	}

	QStringList selectFiles(const QString &title, QFileDialog::FileMode file_mode, QFileDialog::AcceptMode accept_mode,
													const QStringList &name_filters, const QStringList &mime_filters, const QString &default_suffix,
													const QString &selected_file)
	{
		QFileDialog file_dlg;

		file_dlg.setWindowIcon(QIcon(getIconPath("pgmodeler_logo")));
		file_dlg.setWindowTitle(title);
		file_dlg.setDefaultSuffix(default_suffix);
		file_dlg.selectFile(selected_file);

		if(!name_filters.isEmpty())
		 file_dlg.setNameFilters(name_filters);

		if(!mime_filters.isEmpty())
			file_dlg.setMimeTypeFilters(mime_filters);

		file_dlg.setFileMode(file_mode);
		file_dlg.setAcceptMode(accept_mode);
		file_dlg.setModal(true);

		GuiUtilsNs::restoreFileDialogState(&file_dlg);
		file_dlg.exec();
		GuiUtilsNs::saveFileDialogState(&file_dlg);

		if(file_dlg.result() == QDialog::Accepted)
			return file_dlg.selectedFiles();

		return QStringList();
	}

	bool selectAndSaveFile(const QByteArray &buffer, const QString &title, QFileDialog::FileMode file_mode,
												 const QStringList &name_filters, const QStringList &mime_filters,
												 const QString &default_suffix, const QString &selected_file)
	{
		if(file_mode != QFileDialog::ExistingFile &&
			 file_mode != QFileDialog::AnyFile)
			return false;

		try
		{
			QStringList sel_files = selectFiles(title, file_mode, QFileDialog::AcceptSave,
																					name_filters, mime_filters, default_suffix,
																					selected_file);

			if(!sel_files.isEmpty())
			{
				UtilsNs::saveFile(sel_files.at(0), buffer);
				return true;
			}

			return false;
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
		}
	}

	bool selectAndLoadFile(QByteArray &buffer,
												 const QString &title, QFileDialog::FileMode file_mode, const QStringList &name_filters,
												 const QStringList &mime_filters, const QString &selected_file)
	{
		buffer.clear();

		if(file_mode != QFileDialog::ExistingFile &&
			 file_mode != QFileDialog::AnyFile)
			return false;

		try
		{
			QStringList sel_files = selectFiles(title, file_mode, QFileDialog::AcceptOpen,
																					name_filters, mime_filters, "", selected_file);

			if(!sel_files.isEmpty())
			{
				buffer = UtilsNs::loadFile(sel_files.at(0));
				return true;
			}

			return false;
		}
		catch(Exception &e)
		{
			throw Exception(e.getErrorMessage(), __PRETTY_FUNCTION__, __FILE__, __LINE__, &e);
		}
	}

	void updateDropShadows(QWidgetList widgets)
	{
		QColor color(0, 0, 0, 80);
		int radius = 6, x = 1, y = 1;
		QGraphicsDropShadowEffect *shadow = nullptr;
		QString class_name = "QToolButton";

		if(AppearanceConfigWidget::getUiThemeId() == Attributes::Light ||
			 AppearanceConfigWidget::getUiThemeId() == Attributes::InkSaver)
		{
			radius = 1;
			color.setRgb(225, 225, 225);
			color.setAlpha(255);
		}

		for(auto &wgt : widgets)
		{
			if(wgt->metaObject()->className() == class_name && wgt->graphicsEffect())
			{
				shadow = qobject_cast<QGraphicsDropShadowEffect *>(wgt->graphicsEffect());
				shadow->setColor(color);
				shadow->setOffset(x, y);
				shadow->setBlurRadius(radius);
			}
		}
	}
}
