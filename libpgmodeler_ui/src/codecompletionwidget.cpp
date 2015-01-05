/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "codecompletionwidget.h"
#include "generalconfigwidget.h"

CodeCompletionWidget::CodeCompletionWidget(QTextEdit *code_field_txt) :	QWidget(dynamic_cast<QWidget *>(code_field_txt))
{
	if(!code_field_txt)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	parent_wgt=new QWidget(this);
	parent_wgt->setWindowFlags(Qt::Popup);

	name_list=new QListWidget(parent_wgt);
	name_list->setSpacing(2);
	name_list->setIconSize(QSize(16,16));

	persistent_chk=new QCheckBox(parent_wgt);
	persistent_chk->setText(trUtf8("Make persistent"));
	persistent_chk->setToolTip(trUtf8("Makes the widget closable only by ESC key or mouse click on other controls."));
	persistent_chk->setFocusPolicy(Qt::NoFocus);

	QVBoxLayout *vbox=new QVBoxLayout(parent_wgt);
	vbox->addWidget(name_list);
	vbox->addWidget(persistent_chk);
	vbox->setContentsMargins(4,4,4,4);
	vbox->setSpacing(6);
	parent_wgt->setLayout(vbox);

	QFont font=name_list->font();
	font.setPointSizeF(8);
	name_list->setFont(font);

	this->code_field_txt=code_field_txt;
	auto_triggered=false;

	db_model=nullptr;
	setQualifyingLevel(nullptr);

	connect(name_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectItem(void)));
  connect(name_list, SIGNAL(currentRowChanged(int)), this, SLOT(showItemTooltip(void)));

	this->setVisible(false);
}

bool CodeCompletionWidget::eventFilter(QObject *object, QEvent *event)
{
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

	if(k_event && k_event->type()==QEvent::KeyPress)
	{
		if(object==code_field_txt)
		{
			//Filters the trigger char and shows up the code completion only if there is a valid database model in use
			if(QChar(k_event->key())==completion_trigger && db_model)
			{
				if(name_list->isVisible())
					this->selectItem();

				auto_triggered=true;
				this->show();
			}
			//Filters the Crtl+Space to trigger the code completion
			else if(k_event->key()==Qt::Key_Space && (k_event->modifiers()==Qt::ControlModifier || k_event->modifiers()==Qt::MetaModifier))
			{
				this->show();
				return(true);
			}
			else if(k_event->key()==Qt::Key_Space || k_event->key()==Qt::Key_Backspace || k_event->key()==Qt::Key_Delete)
			{
				if(persistent_chk->isChecked() && this->parent_wgt->isVisible())
					this->show();
				else
					this->close();
			}
		}
		else if(object==name_list)
		{
			if(k_event->key()==Qt::Key_Escape)
			{
				this->close();
				return(true);
			}
			//Filters the ENTER/RETURN press to close the code completion widget select the name
			else if(k_event->key()==Qt::Key_Enter || k_event->key()==Qt::Key_Return)
			{
				this->selectItem();

				if(persistent_chk->isChecked())
				{
					//Forcing the line break on the code field when holding Control key and hit return/enter
					if(k_event->modifiers()==Qt::ControlModifier)
					{
						QTextCursor cursor=code_field_txt->textCursor();
						code_field_txt->insertPlainText(QChar(QChar::LineFeed));
						cursor.movePosition(QTextCursor::Down);
						code_field_txt->setTextCursor(cursor);
					}

					this->show();
				}

				return(true);
			}
			//Filters other key press and redirects to the code input field
			else if(k_event->key()!=Qt::Key_Up && k_event->key()!=Qt::Key_Down &&
							k_event->key()!=Qt::Key_PageUp && k_event->key()!=Qt::Key_PageDown &&
							k_event->key()!=Qt::Key_Home && k_event->key()!=Qt::Key_End &&
							k_event->modifiers()!=Qt::AltModifier)
			{

				QCoreApplication::sendEvent(code_field_txt, k_event);
				this->updateList();
				return(true);
			}
		}
	}

	return(QWidget::eventFilter(object, event));
}

void CodeCompletionWidget::configureCompletion(DatabaseModel *db_model, SyntaxHighlighter *syntax_hl, const QString &keywords_grp, bool persistent)
{
  map<QString, attribs_map> confs=GeneralConfigWidget::getConfigurationParams();

  name_list->clear();
  word.clear();
  setQualifyingLevel(nullptr);
  auto_triggered=false;
  this->db_model=db_model;

  persistent_chk->setVisible(db_model==nullptr);
  persistent_chk->setChecked(persistent);

  if(confs[ParsersAttributes::CONFIGURATION][ParsersAttributes::CODE_COMPLETION]==ParsersAttributes::_TRUE_)
  {
    code_field_txt->installEventFilter(this);
    name_list->installEventFilter(this);

    if(syntax_hl && keywords.isEmpty())
    {
      //Get the keywords from the highlighter
      vector<QRegExp> exprs=syntax_hl->getExpressions(keywords_grp);

      while(!exprs.empty())
      {
        keywords.push_front(exprs.back().pattern());
        exprs.pop_back();
      }

      completion_trigger=syntax_hl->getCompletionTrigger();
    }
    else
      completion_trigger=QChar('.');
  }
  else
  {
    code_field_txt->removeEventFilter(this);
    name_list->removeEventFilter(this);
  }
}

void CodeCompletionWidget::insertCustomItem(const QString &name, const QString &tooltip, const QPixmap &icon)
{
	if(!name.isEmpty())
  {
    QString item_name=name.simplified();
    custom_items[item_name]=icon;
    custom_items_tips[item_name]=tooltip;
  }
}

void CodeCompletionWidget::insertCustomItems(const QStringList &names, const QStringList &tooltips, const QPixmap &icon)
{
  for(int i=0; i < names.size(); i++)
  {
    insertCustomItem(names[i],
                     (i < tooltips.size() ? tooltips[i] : ""),
                     icon);

  }
}

void CodeCompletionWidget::clearCustomItems(void)
{
	custom_items.clear();
}

void CodeCompletionWidget::populateNameList(vector<BaseObject *> &objects, QString filter)
{
	QListWidgetItem *item=nullptr;
	QString obj_name;
  ObjectType obj_type;
	QRegExp regexp(filter.remove("\"") + "*", Qt::CaseInsensitive, QRegExp::Wildcard);

	name_list->clear();
	for(unsigned i=0; i < objects.size(); i++)
	{
    obj_type=objects[i]->getObjectType();
		obj_name.clear();

		//Formatting the object name according to the object type
    if(obj_type==OBJ_FUNCTION)
		{
			dynamic_cast<Function *>(objects[i])->createSignature(false);
			obj_name=dynamic_cast<Function *>(objects[i])->getSignature();
		}
    else if(obj_type==OBJ_OPERATOR)
			obj_name=dynamic_cast<Operator *>(objects[i])->getSignature(false);
		else
			obj_name+=objects[i]->getName(false, false);

		//The object will be inserted if its name matches the filter or there is no filter set
		if(filter.isEmpty() || regexp.exactMatch(obj_name))
		{
			item=new QListWidgetItem(QPixmap(QString(":/icones/icones/") + objects[i]->getSchemaName() + QString(".png")), obj_name);
			item->setToolTip(QString("%1 (%2)").arg(objects[i]->getName(true)).arg(objects[i]->getTypeName()));
			item->setData(Qt::UserRole, QVariant::fromValue<void *>(objects[i]));
      item->setToolTip(BaseObject::getTypeName(obj_type));
			name_list->addItem(item);
		}
	}
}

void CodeCompletionWidget::show(void)
{
	prev_txt_cur=code_field_txt->textCursor();
	this->updateList();
	parent_wgt->show();
  this->showItemTooltip();
}

void CodeCompletionWidget::setQualifyingLevel(BaseObject *obj)
{
	if(!obj)
		qualifying_level=-1;
	else if(obj->getObjectType()==OBJ_SCHEMA)
		qualifying_level=0;
	else if(obj->getObjectType()==OBJ_TABLE ||
					obj->getObjectType()==OBJ_VIEW)
		qualifying_level=1;
	else
		qualifying_level=2;

	if(qualifying_level < 0)
	{
		sel_objects={ nullptr, nullptr, nullptr };
	}
	else
	{
		sel_objects[qualifying_level]=obj;
		lvl_cur=code_field_txt->textCursor();
	}
}

void CodeCompletionWidget::updateList(void)
{
	QListWidgetItem *item=nullptr;
	QString pattern;
	QStringList list;
	vector<BaseObject *> objects;
	vector<ObjectType> types=BaseObject::getObjectTypes(false, 	{ OBJ_TEXTBOX, OBJ_RELATIONSHIP, BASE_RELATIONSHIP });
	QTextCursor tc;

	name_list->clear();
	word.clear();
	new_txt_cur=tc=code_field_txt->textCursor();

	/* Try to move the cursor to the previous char in order to check if the user is
	calling the completion without an attached word */
	tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

	if(!tc.selectedText().trimmed().isEmpty() && new_txt_cur.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor))
	{
		//Move the cursor right before the trigger char in order to get the complete word
		code_field_txt->setTextCursor(new_txt_cur);
		word=code_field_txt->textCursor().selectedText();
		word.remove("\"");

		//Case the completion was triggered using the trigger char
		if(db_model && (auto_triggered || completion_trigger==word))
		{
			/* The completion will try to find a schema, table or view that matches the word,
			if the serach returns one item the completion will start/continue an qualifying level */
			new_txt_cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
			code_field_txt->setTextCursor(new_txt_cur);
			word=code_field_txt->textCursor().selectedText();
			word.remove(completion_trigger);
			word.remove("\"");

			objects=db_model->findObjects(word, { OBJ_SCHEMA, OBJ_TABLE, OBJ_VIEW }, false, false, false, true);

			if(objects.size()==1)
				setQualifyingLevel(objects[0]);
		}

		code_field_txt->setTextCursor(prev_txt_cur);
	}

	if(!word.isEmpty() && !auto_triggered)
		pattern="(^" + word.simplified() + ")";
	else if(auto_triggered)
		pattern=word;

	if(db_model)
	{
		//Negative qualifying level means that user called the completion before a space (empty word)
		if(qualifying_level < 0)
			//The default behavior for this is to search all the objects on the model
			objects=db_model->findObjects(pattern, types, false, false, !auto_triggered, auto_triggered);
		else
		{
			//Searching objects according to qualifying level.

			//Level 0 indicates that user selected a schema, so all objects of the schema are retrieved
			if(qualifying_level==0)
				objects=db_model->getObjects(sel_objects[qualifying_level]);
			//Level 1 indicates that user selected a table or view, so all child objects are retrieved
			else if(qualifying_level==1)
				objects=dynamic_cast<BaseTable *>(sel_objects[qualifying_level])->getObjects();

			/* If the typed word is equal to the current level object's name clear the order in order
			to avoid listing the same object */
			if(word==sel_objects[qualifying_level]->getName())
				word.clear();
		}

		populateNameList(objects, word);
	}

	/* List the keywords if the qualifying level is negative or the
	completion wasn't triggered using the special char */
	if(qualifying_level < 0 && !auto_triggered)
	{
		QRegExp regexp(pattern, Qt::CaseInsensitive);

		list=keywords.filter(regexp);
		for(int i=0; i < list.size(); i++)
		{
			item=new QListWidgetItem(QPixmap(":/icones/icones/keyword.png"), list[i]);
      item->setToolTip(trUtf8("SQL Keyword"));
			name_list->addItem(item);
		}

		name_list->sortItems();

		//If there are custom items, they wiill be placed at the very beggining of the list
		if(!custom_items.empty())
		{
			QStringList list;
			int row=0;
      QListWidgetItem *item=nullptr;

			for(auto itr : custom_items)
			{
				if(itr.first.contains(regexp))
					list.push_back(itr.first);
			}

			list.sort();
      for(auto item_name : list)
      {
        item=new QListWidgetItem(custom_items[item_name], item_name);
        item->setToolTip(custom_items_tips[item_name]);
        name_list->insertItem(row++, item);
      }
		}
	}

	if(name_list->count()==0)
	{
		name_list->addItem(trUtf8("(no items found.)"));
		name_list->item(0)->setFlags(Qt::NoItemFlags);
    QToolTip::hideText();
	}
	else
		name_list->setItemSelected(name_list->item(0), true);

	//Sets the list position right below of text cursor
	parent_wgt->move(code_field_txt->mapToGlobal(code_field_txt->cursorRect().topLeft() + QPoint(0,20)));
	name_list->setFocus();
}

void CodeCompletionWidget::selectItem(void)
{
	if(!name_list->selectedItems().isEmpty())
	{
		QListWidgetItem *item=name_list->selectedItems().at(0);
		BaseObject *object=nullptr;
		QTextCursor tc;

		if(qualifying_level < 0)
			code_field_txt->setTextCursor(new_txt_cur);

		//If the selected item is a object (data not null)
		if(!item->data(Qt::UserRole).isNull())
		{
			//Retrieve the object
			object=reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>());

			/* Move the cursor to the start of the word because all the chars will be replaced
			with the object name */
			prev_txt_cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
			tc=prev_txt_cur;
			tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

			if(tc.selectedText().contains("\""))
				prev_txt_cur=tc;

			code_field_txt->setTextCursor(prev_txt_cur);
			insertObjectName(object);
			setQualifyingLevel(object);
		}
		else
		{
			code_field_txt->insertPlainText(item->text() + " ");
			setQualifyingLevel(nullptr);
		}

    emit s_wordSelected(item->text());
	}
	else
		setQualifyingLevel(nullptr);

	name_list->clearSelection();

	if(!persistent_chk->isChecked())
		this->close();

  auto_triggered=false;
}

void CodeCompletionWidget::showItemTooltip(void)
{
  QListWidgetItem *item=name_list->currentItem();

  if(item)
  {
    QPoint pos=name_list->mapToGlobal(QPoint(name_list->width(), name_list->geometry().top()));
    QToolTip::showText(pos, item->toolTip());
  }
}

void CodeCompletionWidget::close(void)
{
	setQualifyingLevel(nullptr);
	name_list->clearSelection();
	parent_wgt->close();
	auto_triggered=false;
}

void CodeCompletionWidget::insertObjectName(BaseObject *obj)
{
	bool sch_qualified=!sel_objects[0],
			 modify_name=QApplication::keyboardModifiers()==Qt::AltModifier;
	QString name=obj->getName(true, sch_qualified);
	ObjectType obj_type=obj->getObjectType();
	int move_cnt=0;


	if(modify_name &&
		 (obj_type==OBJ_TABLE || TableObject::isTableObject(obj_type)))
	{
		if(obj_type==OBJ_TABLE)
		{
			Table *tab=dynamic_cast<Table *>(obj);

			name+="(";
			for(unsigned i=0; i < tab->getColumnCount(); i++)
				name+=tab->getColumn(i)->getName(true) + ",";

			name.remove(name.size()-1, 1);
			name+=")";
		}
		else
		{
			if(sel_objects[0])
				move_cnt=2;
			else
				move_cnt=3;

			lvl_cur.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor, move_cnt);
			code_field_txt->setTextCursor(lvl_cur);
		}
	}
	else if(obj_type==OBJ_FUNCTION)
	{
		Function *func=dynamic_cast<Function *>(obj);
		func->createSignature(true, sch_qualified);
		name=func->getSignature();
	}
	else if(obj_type==OBJ_CAST)
	{
		name.replace(",", " AS ");
	}
	else if(obj_type==OBJ_AGGREGATE)
	{
		Aggregate *agg;
		agg=dynamic_cast<Aggregate *>(obj);
		name+="(";

		if(agg->getDataTypeCount()==0)
			name+="*";
		else
		{
			for(unsigned i=0; i < agg->getDataTypeCount(); i++)
				name+=~agg->getDataType(i) + ",";
			name.remove(name.size()-1, 1);
		}

		name+=")";
	}

	code_field_txt->insertPlainText(name);
}
