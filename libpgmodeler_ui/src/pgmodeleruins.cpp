#include "pgmodeleruins.h"
#include "messagebox.h"
#include "databasemodel.h"
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include "numberedtexteditor.h"
#include <QScreen>
#include <QDesktopWidget>
#include "baseform.h"
#include "bulkdataeditwidget.h"

namespace PgModelerUiNs {

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

			txt_height = output_lst->fontMetrics().height() * text.count(QString("<br/>"));

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
				vector<TableObject *> *objects=dynamic_cast<PhysicalTable *>(object)->getObjectList(ObjectType::Constraint);

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
			vector<BaseObject *> refs;
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
		QStringList start_tags={ QString("<strong>"), QString("<em>(") },
				end_tags={ QString("</strong>"), QString(")</em>") };
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

		fmt_msg.replace(QString("\n"), QString("<br/>"));

		return fmt_msg;
	}

	void configureWidgetFont(QWidget *widget, unsigned factor_id)
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
		vector<Exception> list;
		vector<Exception>::reverse_iterator itr, itr_end;
		QString text;
		int idx=0;
		QTreeWidgetItem *item=nullptr, *child_item=nullptr;

		if(!exceptions_trw)
			return;

		e.getExceptionsList(list);
		itr = list.rbegin();
		itr_end = list.rend();

		//for(Exception &ex : list)
		while(itr != itr_end)
		{			
			text=QString("[%1] - %2").arg(idx).arg(itr->getMethod());
			item=createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("funcao")), root, false, true);

			text=QString("%1 (%2)").arg(itr->getFile()).arg(itr->getLine());
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("codigofonte")), item, false, true);

			text=QString("%1 (%2)").arg(Exception::getErrorCode(itr->getErrorCode())).arg(enum_cast(itr->getErrorCode()));
			createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("msgbox_alerta")), item, false, true);

			child_item=createOutputTreeItem(exceptions_trw, itr->getErrorMessage(), QPixmap(getIconPath("msgbox_erro")), item, false, true);
			exceptions_trw->itemWidget(child_item, 0)->setStyleSheet(QString("color: #ff0000;"));

			if(!itr->getExtraInfo().isEmpty())
			{
				child_item=createOutputTreeItem(exceptions_trw, itr->getExtraInfo(), QPixmap(getIconPath("msgbox_info")), item, false, true);
				exceptions_trw->itemWidget(child_item, 0)->setStyleSheet(QString("color: #000080;"));
			}

			idx++; itr++;

			/* If we have a stack bigger than 30 items we just ignore the rest in order to avoid
			 * the production or reduntant/useless information on the exception message box */
			if(static_cast<unsigned>(idx) >= Exception::MaximumStackSize)
			{
				text = QT_TR_NOOP("Another %1 error(s) were suppressed due to stacktrace size limits.");
				text = text.arg(list.size() - idx);
				createOutputTreeItem(exceptions_trw, text, QPixmap(getIconPath("msgbox_alerta")), item, false, false);
				break;
			}
		}
	}

	QString getIconPath(const QString &icon)
	{
		return QString(":/icones/icones/%1.png").arg(icon);
	}

	QString getIconPath(ObjectType obj_type)
	{
		return getIconPath(BaseObject::getSchemaName(obj_type));
	}

	void resizeDialog(QWidget *widget)
	{
		QSize min_size=widget->minimumSize();
		int max_h = 0, curr_w =0, curr_h = 0,
				screen_id = qApp->desktop()->screenNumber(qApp->activeWindow());
		QScreen *screen=qApp->screens().at(screen_id);
		double dpi_factor = 0;
		double pixel_ratio = 0;

		dpi_factor = screen->logicalDotsPerInch() / 96.0;
    pixel_ratio = screen->devicePixelRatio();

		//If the dpi_factor is unchanged (1) we keep the dialog original dimension
		if(dpi_factor <= 1.01)
			return;

		max_h = screen->size().height() * 0.70;

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

		curr_w *= dpi_factor * pixel_ratio;
		curr_h *= dpi_factor * pixel_ratio;

		if(curr_w > screen->size().width())
			curr_w = screen->size().width() * 0.80;

		if(curr_h > screen->size().height())
			curr_h = screen->size().height() * 0.80;

		widget->setMinimumSize(widget->minimumSize());
		widget->resize(curr_w, curr_h);
		widget->adjustSize();
	}

	void bulkDataEdit(QTableWidget *results_tbw)
	{
		if(!results_tbw)
			return;

		BaseForm base_frm;
		BulkDataEditWidget *bulkedit_wgt = new BulkDataEditWidget;

		base_frm.setMainWidget(bulkedit_wgt);
		base_frm.setButtonConfiguration(Messagebox::OkCancelButtons);

		if(base_frm.exec() == QDialog::Accepted)
		{
			QList<QTableWidgetSelectionRange> sel_ranges=results_tbw->selectedRanges();

			for(auto range : sel_ranges)
			{
				for(int row = range.topRow(); row <= range.bottomRow(); row++)
				{
					for(int col = range.leftColumn(); col <= range.rightColumn(); col++)
					{
						results_tbw->item(row, col)->setText(bulkedit_wgt->value_edt->toPlainText());
					}
				}
			}
		}
	}

	void createDropShadow(QToolButton *btn, int x_offset, int y_offset, int radius)
	{
		QGraphicsDropShadowEffect *shadow=nullptr;

		shadow=new QGraphicsDropShadowEffect(btn);
		shadow->setXOffset(x_offset);
		shadow->setYOffset(y_offset);
		shadow->setBlurRadius(radius);
		shadow->setColor(QColor(0,0,0, 100));
		btn->setGraphicsEffect(shadow);
	}
}
