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

#include "codecompletionwidget.h"
#include "settings/generalconfigwidget.h"
#include "guiutilsns.h"
#include "settings/snippetsconfigwidget.h"

const QStringList CodeCompletionWidget::dml_keywords = {
	/* Insert here the keywords that need have their position determined
	 * in order to call retriveColumnNames() and retrieveObjectsName().
	 * New keywords here need a new entry in DmlKeywordId enum */
	"select", "insert", "update", "delete",
	"truncate", "from", "join", "into", "as",
	"set", "table", "only", "where",

	/* Insert new keywords after this point if their position in the SQL command
	 * is not important but they are need to do some extra checkings */
	"inner", "outer", "left", "right",	"full"
};

const QString CodeCompletionWidget::special_chars("(),*;");

CodeCompletionWidget::CodeCompletionWidget(QPlainTextEdit *code_field_txt, bool enable_snippets) :	QWidget(dynamic_cast<QWidget *>(code_field_txt))
{
	if(!code_field_txt)
		throw Exception(ErrorCode::AsgNotAllocattedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	resetKeywordsPos();

	this->enable_snippets = enable_snippets;
	popup_timer.setInterval(300);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	completion_wgt=new QWidget(this);
	completion_wgt->setWindowFlags(Qt::Dialog);
	completion_wgt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	completion_wgt->setMinimumSize(200, 200);
	completion_wgt->setMaximumHeight(300);

	always_on_top_chk=new QCheckBox(completion_wgt);
	always_on_top_chk->setText(tr("&Always on top"));
	always_on_top_chk->setToolTip(tr("The widget will be always displayed while typing. It can be closable only by ESC key or when focus changes to another widget."));
	always_on_top_chk->setFocusPolicy(Qt::NoFocus);

	name_list=new QListWidget(completion_wgt);
	name_list->setSpacing(2);
	name_list->setIconSize(QSize(22, 22));
	name_list->setSortingEnabled(false);
	name_list->setSizeAdjustPolicy(QListWidget::AdjustToContents);
	name_list->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	name_list->setMaximumHeight(completion_wgt->maximumHeight() - always_on_top_chk->height() - GuiUtilsNs::LtSpacing);

	QVBoxLayout *vbox=new QVBoxLayout(completion_wgt);
	vbox->addWidget(name_list);
	vbox->addWidget(always_on_top_chk);
	vbox->setContentsMargins(GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin,GuiUtilsNs::LtMargin);
	vbox->setSpacing(GuiUtilsNs::LtSpacing);
	completion_wgt->setLayout(vbox);

	GuiUtilsNs::configureWidgetFont(name_list, GuiUtilsNs::MediumFontFactor);

	this->code_field_txt=code_field_txt;
	auto_triggered=false;

	db_model=nullptr;
	setQualifyingLevel(nullptr);

	connect(name_list, &QListWidget::itemDoubleClicked, this, &CodeCompletionWidget::selectItem);
	connect(name_list, &QListWidget::currentRowChanged, this, &CodeCompletionWidget::showItemTooltip);

	connect(&popup_timer, &QTimer::timeout, this, [this](){
		if(qualifying_level < 2)
		{
			auto_triggered=true;
			this->show();
		}
	});

	this->setVisible(false);

	if(enable_snippets)
		connect(this, &CodeCompletionWidget::s_wordSelected, this, &CodeCompletionWidget::handleSelectedWord);
}

void CodeCompletionWidget::handleSelectedWord(QString word)
{
	if(SnippetsConfigWidget::isSnippetExists(word))
	{
		QTextCursor tc=code_field_txt->textCursor();
		tc.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
		tc.removeSelectedText();
		tc.insertText(SnippetsConfigWidget::getParsedSnippet(word));
	}
}

bool CodeCompletionWidget::eventFilter(QObject *object, QEvent *event)
{
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

	if(k_event && k_event->type()==QEvent::KeyPress)
	{
		if(object==code_field_txt)
		{
			//Filters the trigger char and shows up the code completion only if there is a valid database model in use
			if(k_event->key() == completion_trigger.unicode() && (db_model || catalog.isConnectionValid()))
			{
				/* If the completion widget is not visible start the timer to give the user
				a small delay in order to type another character. If no char is typed the completion is triggered */
				if(!completion_wgt->isVisible() && !popup_timer.isActive())
					popup_timer.start();

				if(name_list->isVisible())
				{
					this->selectItem();
					this->show();
				}
			}
			else
			{
				popup_timer.stop();

				//Filters the Crtl+Space to trigger the code completion
				if(k_event->key()==Qt::Key_Space && (k_event->modifiers()==Qt::ControlModifier || k_event->modifiers()==Qt::MetaModifier))
				{
					setQualifyingLevel(nullptr);
					this->show();
					return true;
				}
				else if(k_event->key()==Qt::Key_Space || k_event->key()==Qt::Key_Backspace || k_event->key()==Qt::Key_Delete)
				{
					QTextCursor tc=code_field_txt->textCursor();
					tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

					/* Avoiding deleting text using backspace or delete if the current char is the completion trigger (.).
						 This will block the cursor and cause the list to stay in the current qualifying level */
					if(completion_wgt->isVisible() &&
						 (k_event->key()==Qt::Key_Backspace || k_event->key()==Qt::Key_Delete) &&
						 tc.selectedText().contains(completion_trigger))
					{
						event->ignore();
						return true;
					}
					else if(k_event->key()==Qt::Key_Space)
					{
						setQualifyingLevel(nullptr);

						if(!always_on_top_chk->isChecked())
							this->close();
					}

					if(always_on_top_chk->isChecked())
						this->show();
				}
			}
		}
		else if(object==name_list)
		{
			if(k_event->key()==Qt::Key_Escape)
			{
				this->close();
				return true;
			}
			//Filters the ENTER/RETURN press to close the code completion widget select the name
			else if(k_event->key()==Qt::Key_Enter || k_event->key()==Qt::Key_Return)
			{
				if(!always_on_top_chk->isChecked())
					this->selectItem();
				else
				{
					//Forcing the line break on the code field when holding Control key and hit return/enter
					if(k_event->modifiers()==Qt::ControlModifier)
					{
						QTextCursor cursor=code_field_txt->textCursor();
						code_field_txt->insertPlainText(QChar(QChar::LineFeed));
						cursor.movePosition(QTextCursor::Down);
						code_field_txt->setTextCursor(cursor);
					}
					else
						this->selectItem();

					this->show();
				}

				return true;
			}
			//Filters other key press and redirects to the code input field
			else if(k_event->key()!=Qt::Key_Up && k_event->key()!=Qt::Key_Down &&
					k_event->key()!=Qt::Key_PageUp && k_event->key()!=Qt::Key_PageDown &&
					k_event->key()!=Qt::Key_Home && k_event->key()!=Qt::Key_End &&
					k_event->modifiers()!=Qt::AltModifier)
			{

				QCoreApplication::sendEvent(code_field_txt, k_event);
				this->updateList();
				return true;
			}
		}

		name_list->adjustSize();
		adjustSize();
	}

	return QWidget::eventFilter(object, event);
}

void CodeCompletionWidget::configureCompletion(DatabaseModel *db_model, SyntaxHighlighter *syntax_hl, const QString &keywords_grp)
{
	std::map<QString, attribs_map> confs=GeneralConfigWidget::getConfigurationParams();

	name_list->clear();
	word.clear();
	setQualifyingLevel(nullptr);
	resetKeywordsPos();
	auto_triggered=false;
	this->db_model=db_model;

	if(confs[Attributes::Configuration][Attributes::CodeCompletion]==Attributes::True)
	{
		code_field_txt->installEventFilter(this);
		name_list->installEventFilter(this);

		if(syntax_hl && keywords.isEmpty())
		{
			//Get the keywords from the highlighter
			std::vector<QRegularExpression> exprs=syntax_hl->getExpressions(keywords_grp);

			while(!exprs.empty())
			{
				/* Since keywords are exact match patterns in the form \A(?:keyword)\z"
				 * we need to remove from the pattern the initial and final regexp operators in
				 * order to use only the word itself */
				keywords.push_front(exprs.back().pattern().remove("\\A(?:").remove(")\\z"));
				exprs.pop_back();
			}

			completion_trigger=syntax_hl->getCompletionTrigger();
		}
		else
			completion_trigger=QChar('.');

		if(enable_snippets)
		{
			clearCustomItems();
			insertCustomItems(SnippetsConfigWidget::getAllSnippetsAttribute(Attributes::Id),
												SnippetsConfigWidget::getAllSnippetsAttribute(Attributes::Label),
												QPixmap(GuiUtilsNs::getIconPath("codesnippet")));
		}
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
		insertCustomItem(names[i], (i < tooltips.size() ? tooltips[i] : ""), icon);
	}
}

void CodeCompletionWidget::insertCustomItems(const QStringList &names, const QString &tooltip, ObjectType obj_type)
{
	for(auto &name : names)
		insertCustomItem(name, tooltip, QPixmap(GuiUtilsNs::getIconPath(obj_type)));
}

void CodeCompletionWidget::clearCustomItems()
{
	custom_items.clear();
}

void CodeCompletionWidget::setConnection(Connection conn)
{
	// When setting a connection, we disable the lookup in the database model
	db_model = nullptr;
	catalog.closeConnection();
	catalog.setConnection(conn);
}

void CodeCompletionWidget::populateNameList(std::vector<BaseObject *> &objects, QString filter)
{
	QListWidgetItem *item=nullptr;
	QString obj_name;
	ObjectType obj_type;
	QRegularExpression regexp(QRegularExpression::wildcardToRegularExpression(filter.remove('"') + QString("*")),
														QRegularExpression::CaseInsensitiveOption);

	name_list->clear();

	for(auto &obj : objects)
	{
		obj_type = obj->getObjectType();
		obj_name.clear();

		//Formatting the object name according to the object type
		if(obj_type == ObjectType::Function)
		{
			dynamic_cast<Function *>(obj)->createSignature(false);
			obj_name = dynamic_cast<Function *>(obj)->getSignature();
		}
		else if(obj_type == ObjectType::Operator)
			obj_name = dynamic_cast<Operator *>(obj)->getSignature(false);
		else
			obj_name += obj->getName(false, false);

		//The object will be inserted if its name matches the filter or there is no filter set
		if(filter.isEmpty() || regexp.match(obj_name).hasMatch())
		{
			item= new QListWidgetItem(QPixmap(GuiUtilsNs::getIconPath(obj->getSchemaName())), obj_name);
			item->setToolTip(QString("%1 (%2)").arg(obj->getName(true)).arg(obj->getTypeName()));
			item->setData(Qt::UserRole, QVariant::fromValue<void *>(obj));
			item->setToolTip(BaseObject::getTypeName(obj_type));
			name_list->addItem(item);
		}
	}

	name_list->sortItems();
}

void CodeCompletionWidget::show()
{
	prev_txt_cur = code_field_txt->textCursor();
	updateList();
	completion_wgt->show();	
	popup_timer.stop();
	completion_wgt->adjustSize();
	adjustSize();

	QTimer::singleShot(500, this, [this](){
		showItemTooltip();
	});
}

void CodeCompletionWidget::setQualifyingLevel(BaseObject *obj)
{
	if(!obj)
		qualifying_level=-1;
	else if(obj->getObjectType()==ObjectType::Schema)
		qualifying_level=0;
	else if(BaseTable::isBaseTable(obj->getObjectType()))
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

void CodeCompletionWidget::resetKeywordsPos()
{
	for(unsigned id = Select; id <= Where; id++)
		dml_kwords_pos[id] = -1;
}

bool CodeCompletionWidget::retrieveColumnNames()
{
	QTextCursor tc = code_field_txt->textCursor();
	int cur_pos = tc.position();
	QStringList tab_names;
	QListWidgetItem *item = nullptr;
	attribs_map filter, attribs;
	QString sch_name, tab_name, curr_word;
	QStringList aux_names;
	bool cols_added = false;

	// If a table alias is being referenced we use the name of the table aliased
	if(word == completion_trigger)
	{
		tc = code_field_txt->textCursor();
		tc.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);
		tc.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor, 2);
		curr_word = tc.selectedText();
		curr_word.remove(completion_trigger);
		curr_word = curr_word.trimmed();

		if(curr_word.isEmpty() || dml_keywords.contains(curr_word, Qt::CaseInsensitive))
			return false;

		/* If the word is an registered table alias then we will use the related table
		 * to retrieve the columns */
		if(tab_aliases.count(curr_word))
			tab_names.append(tab_aliases[curr_word]);

		curr_word.clear();
	}
	else
		curr_word = word;

	curr_word.remove(',');
	curr_word.remove('"');

	// If no table name was retrieved from aliases names
	if(word != completion_trigger)
	{
		// Retrieving the table name between FROM ... JOIN/WHERE
		if(((dml_kwords_pos[Select] >= 0 && dml_kwords_pos[From] >= 0 &&
				 cur_pos > dml_kwords_pos[Select] && cur_pos < dml_kwords_pos[From]) ||
			 (dml_kwords_pos[Select] >= 0 &&
				dml_kwords_pos[Where] >= 0 && cur_pos > dml_kwords_pos[Where])))
		{
			tab_names = getTableNames(dml_kwords_pos[From],
																dml_kwords_pos[Join] >= 0 ? dml_kwords_pos[Join] : dml_kwords_pos[Where]);
		}
		// Retrieving the table name after DELETE FROM ...
		else if((dml_kwords_pos[Delete] >= 0 && dml_kwords_pos[From] >= 0 &&
						 cur_pos > dml_kwords_pos[From]))
		{
			tab_names = getTableNames(dml_kwords_pos[From], dml_kwords_pos[Where]);
		}
		// Retrieving the table name between UPDATE ... SET
		else if((dml_kwords_pos[Update] >= 0 && dml_kwords_pos[Set] >= 0 &&
						 cur_pos > dml_kwords_pos[Set]))
		{
			tab_names = getTableNames(dml_kwords_pos[Update], dml_kwords_pos[Set]);
		}
	}

	for(auto &name : tab_names)
	{
		aux_names = name.split(completion_trigger);
		sch_name = aux_names[0].trimmed();
		tab_name = aux_names[1].trimmed();
		catalog.setQueryFilter(Catalog::ListAllObjects);

		if(!tab_name.isEmpty())
			filter[Attributes::NameFilter] = QString("^(%1)").arg(curr_word);

		attribs = catalog.getObjectsNames(ObjectType::Column, sch_name, tab_name, filter);

		for(auto &attr : attribs)
		{
			cols_added = true;
			item = new QListWidgetItem(QIcon(GuiUtilsNs::getIconPath(ObjectType::Column)), attr.second);
			item->setToolTip(tr("Object: <em>%1</em><br/>Table: %2")
											 .arg(BaseObject::getTypeName(ObjectType::Column),
														QString("<strong>%1</strong>.%2").arg(sch_name, tab_name)));
			name_list->addItem(item);
		}
	}

	name_list->sortItems();
	return cols_added;
}

bool CodeCompletionWidget::retrieveObjectNames()
{
	attribs_map attribs, filter;
	QListWidgetItem *item = nullptr;
	QString curr_word = word, obj_name;
	QTextCursor tc = code_field_txt->textCursor();
	bool retrieved = false;

	while(!curr_word.isEmpty())
	{
		tc.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);
		tc.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
		curr_word = tc.selectedText();

		if(curr_word == "," || dml_keywords.contains(curr_word, Qt::CaseInsensitive))
			break;

		obj_name.prepend(curr_word);
		tc.movePosition(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
	}

	if(obj_name == completion_trigger)
		return false;

	QStringList names = obj_name.split(completion_trigger);
	QList<ObjectType> obj_types;
	QString sch_name, aux_name;

	if(names.size() == 1)
		obj_types.append(ObjectType::Schema);
	else if(names.size() == 2)
	{
		obj_types.append({ ObjectType::Table,
											 ObjectType::ForeignTable,
											 ObjectType::View,
											 ObjectType::Aggregate,
											 ObjectType::Function,
											 ObjectType::Procedure });
		sch_name = names[0];
		obj_name = names[1];
	}

	for(auto &obj_type : obj_types)
	{
		catalog.setQueryFilter(Catalog::ListAllObjects);

		if(!obj_name.isEmpty() && obj_name != completion_trigger)
			filter[Attributes::NameFilter] = QString("^(%1)").arg(obj_name);

		attribs = catalog.getObjectsNames(obj_type, sch_name, "", filter);

		for(auto &attr : attribs)
		{
			aux_name = attr.second;

			// Removing parameter names from functions/procedures/aggregates
			if(obj_type == ObjectType::Function ||
				 obj_type == ObjectType::Procedure ||
				 obj_type == ObjectType::Aggregate)
				aux_name.remove(QRegularExpression("(\\()(.*)(\\))"));

			name_list->addItem(aux_name);
			item = name_list->item(name_list->count() - 1);
			item->setIcon(QIcon(GuiUtilsNs::getIconPath(obj_type)));

			if(obj_type != ObjectType::Schema)
			{
				item->setToolTip(tr("Object: <em>%1</em><br/>Signature: %2")
												 .arg(BaseObject::getTypeName(obj_type),
															QString("<strong>%1</strong>.%2").arg(sch_name, attr.second)));
			}
			else
				item->setToolTip(tr("Object: <em>%1</em>").arg(BaseObject::getTypeName(obj_type)));

			retrieved = true;
		}
	}

	return retrieved;
}

void CodeCompletionWidget::extractTableNames()
{
	QTextCursor tc = code_field_txt->textCursor();
	QString curr_word, tab_name, alias;
	bool extract_alias = false, tab_name_extracted = false,
			parse_next = false;
	int prev_pos = -1;

	tab_aliases.clear();
	tab_names_pos.clear();
	tc.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

	while(!tc.atEnd())
	{
		tc.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
		curr_word = tc.selectedText();
		curr_word.remove('"');
		tc.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

		/* Aliases can appear in the following forms:
		 * SELECT/DELETE ... FROM tabname [AS] alias ...
		 * JOIN tabname [AS] alias
		 * UPDATE tabname [AS] alias */
		if(curr_word.compare("from", Qt::CaseInsensitive) == 0 ||
			 curr_word.compare("join", Qt::CaseInsensitive) == 0 ||
			 curr_word.compare("into", Qt::CaseInsensitive) == 0 ||
			 curr_word.compare("update", Qt::CaseInsensitive) == 0 ||
			 (parse_next && !dml_keywords.contains(curr_word, Qt::CaseInsensitive)))
		{
			if(parse_next)
			{
				tc.setPosition(prev_pos);
				//tc.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);
			}
			else
				tc.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);

			extract_alias = tab_name_extracted = parse_next = false;
			tab_name.clear();
			alias.clear();

			while(!tc.atEnd())
			{
				tc.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
				curr_word = tc.selectedText().trimmed();
				curr_word.remove('"');
				tc.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

				if(curr_word.isEmpty() ||
					 (curr_word.compare("as", Qt::CaseInsensitive) != 0 &&
						(dml_keywords.contains(curr_word, Qt::CaseInsensitive))))
					continue;

				if(!extract_alias && !curr_word.isEmpty() &&
					 (curr_word.compare("as", Qt::CaseInsensitive) == 0 || tab_name_extracted))
					extract_alias = true;

				if(!extract_alias && !special_chars.contains(curr_word))
				{
					if(tab_name.endsWith(completion_trigger))
						tab_name_extracted = true;

					tab_name.append(curr_word);

					// Register the table name and position
					if(tab_name_extracted)
						tab_names_pos[tc.position() - tab_name.length()] = tab_name;
				}
				else
				{
					if(extract_alias &&	 !special_chars.contains(curr_word) &&
						 curr_word.compare("as", Qt::CaseInsensitive) != 0)
					{
						alias.append(curr_word);
						tab_aliases[alias] = tab_name;
					}

					if(special_chars.contains(curr_word) || !alias.isEmpty())
					{
						prev_pos = tc.position();
						parse_next = true;
						break;
					}
				}
			}
		}
	}
}

QStringList CodeCompletionWidget::getTableNames(int start_pos, int stop_pos)
{
	if(start_pos < 0)
		return QStringList();

	QStringList names;

	for(auto &itr : tab_names_pos)
	{
		if(stop_pos >= 0 && itr.first > stop_pos)
			break;

		if(itr.first >= start_pos)
			names.append(itr.second);
	}

	names.removeDuplicates();
	return names;
}

bool CodeCompletionWidget::updateObjectsList()
{
	QTextCursor orig_tc, tc;
	QStringList dml_cmds,	dml_clauses;
	unsigned kw_id = Select;
	int found_kw_id = -1;
	QTextDocument::FindFlags find_flags[2] = { (QTextDocument::FindWholeWords |
																							QTextDocument::FindBackward),

																						 QTextDocument::FindWholeWords };

	dml_cmds = dml_keywords.mid(Select, 5);
	dml_clauses = dml_keywords.mid(From);
	orig_tc = tc = code_field_txt->textCursor();

	for(auto &cmd : dml_cmds)
	{
		/* Finding the postion of the DML command start (SELECT, INSERT, UPDATE, DELETE, TRUNCATE).
		 * This will be the starting point of the search for the other keywords */
		for(auto &flag : find_flags)
		{
			code_field_txt->setTextCursor(tc);

			if(code_field_txt->find(cmd, flag))
			{
				dml_kwords_pos[kw_id] = code_field_txt->textCursor().position();
				found_kw_id = kw_id;
			}
			else
				dml_kwords_pos[kw_id] = -1;

			if(dml_kwords_pos[kw_id] >= 0)
				break;
		}

		code_field_txt->setTextCursor(tc);
		kw_id++;
	}

	// If none of the dml command start are found, abort the completion
	if(found_kw_id < 0)
		return false;

	// Move the cursor right after the select keyword
	tc.setPosition(dml_kwords_pos[found_kw_id] + 1);
	code_field_txt->setTextCursor(tc);
	kw_id = From;

	// Finding the position of the FROM/JOIN/WHERE keywords
	for(auto &kw : dml_clauses)
	{
		if(code_field_txt->find(kw, QTextDocument::FindWholeWords))
			dml_kwords_pos[kw_id++] = code_field_txt->textCursor().position();
		else
			dml_kwords_pos[kw_id++] = -1;

		code_field_txt->setTextCursor(tc);
	}

	int cur_pos = orig_tc.position();
	code_field_txt->setTextCursor(orig_tc);

	if(cur_pos < 0)
		return false;

	try
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);

		bool cols_retrieved = false, objs_retrieved = false;

		name_list->clear();
		extractTableNames();
		cols_retrieved = retrieveColumnNames();

		if(!cols_retrieved)
			objs_retrieved = retrieveObjectNames();

		QApplication::restoreOverrideCursor();

		return cols_retrieved || objs_retrieved;
	}
	catch(Exception &e)
	{
		QApplication::restoreOverrideCursor();
		QTextStream out(stdout);
		out << e.getExceptionsText() << Qt::endl;
		return false;
	}
}

void CodeCompletionWidget::updateList()
{
	QListWidgetItem *item=nullptr;
	QString pattern;
	std::vector<BaseObject *> objects;
	std::vector<ObjectType> types=BaseObject::getObjectTypes(false, 	{ ObjectType::Textbox,
																																			ObjectType::Relationship,
																																			ObjectType::BaseRelationship });
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
		word.remove('"');

		//Case the completion was triggered using the trigger char
		if(db_model && (auto_triggered || completion_trigger==word))
		{
			/* The completion will try to find a schema, table or view that matches the word,
			if the serach returns one item the completion will start/continue an qualifying level */
			new_txt_cur.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor);
			code_field_txt->setTextCursor(new_txt_cur);
			word=code_field_txt->textCursor().selectedText();
			word.remove(completion_trigger);
			word.remove('"');

			objects=db_model->findObjects(word, { ObjectType::Schema, ObjectType::Table, ObjectType::ForeignTable, ObjectType::View }, false, false, true);

			if(objects.size()==1)
				setQualifyingLevel(objects[0]);
		}

		code_field_txt->setTextCursor(prev_txt_cur);
	}

	if(!word.isEmpty() && !auto_triggered)
		pattern = "(^" + word.simplified() + ")";
	else if(auto_triggered)
		pattern=word;

	pattern.replace("*", "\\*");

	if(db_model)
	{
		//Negative qualifying level means that user called the completion before a space (empty word)
		if(qualifying_level < 0)
			//The default behavior for this is to search all the objects on the model
			objects=db_model->findObjects(pattern, types, false, !auto_triggered, auto_triggered);
		else
		{
			QString left_word;

			//Searching objects according to qualifying level.
			tc=code_field_txt->textCursor();
			tc.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor);

			/* Retrieving the word at the left in order to compare it to the object's name at the current qualifying level,
		 if the word does not matches the object then children objects will not be retrieved */
			if(tc.selectedText().contains('\"'))
			{
				tc.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor);
				left_word=tc.selectedText();
				left_word.remove('"');
			}
			else
				left_word=tc.selectedText();

			//Level 0 indicates that user selected a schema, so all objects of the schema are retrieved
			if(qualifying_level==0 /*&& left_word==sel_objects[qualifying_level]->getName()*/)
				objects=db_model->getObjects(sel_objects[qualifying_level]);

			/* Level 1 indicates that user selected a table or view, so all child objects are retrieved.
		 If the current level is 1 and the table/view name isn't present then the children will not be listed */
			else if(qualifying_level==1 /*&& left_word==sel_objects[qualifying_level]->getName()*/)
				objects=dynamic_cast<BaseTable *>(sel_objects[qualifying_level])->getObjects();

			/* If the current qualifying level and current word does retrieve any object as a fallback
		 we try to find any object in the model and reset the qualifying level */
			else
			{
				objects=db_model->findObjects(pattern, types, false, !auto_triggered, auto_triggered);
				setQualifyingLevel(nullptr);
			}

			/* If the typed word is equal to the current level object's name clear the order in order
			to avoid listing the same object */
			if(qualifying_level >=0 && word==sel_objects[qualifying_level]->getName())
				word.clear();
		}

		populateNameList(objects, word);
	}

	// Retrieving object names from the database if a valid connection is configured
	bool db_objs_retrieved = false;

	if(catalog.isConnectionValid())
		db_objs_retrieved = updateObjectsList();

	/* List the keywords if no object was retrived from databas or
	 * the qualifying level is negative or the completion wasn't triggered
	 * using the special char */
	if(!db_objs_retrieved && qualifying_level < 0 && !auto_triggered)
	{
		QRegularExpression regexp(pattern, QRegularExpression::CaseInsensitiveOption);

		//If there are custom items, they wiill be placed at the very beggining of the list
		if(!custom_items.empty())
		{
			QStringList list;
			QListWidgetItem *item=nullptr;

			for(auto &itr : custom_items)
			{
				if(itr.first.contains(regexp))
					list.push_back(itr.first);
			}

			list.sort();
			for(auto &item_name : list)
			{
				item=new QListWidgetItem(custom_items[item_name], item_name);
				item->setToolTip(custom_items_tips[item_name]);
				name_list->addItem(item);
			}
		}

		for(auto &kw : keywords.filter(regexp))
		{
			item=new QListWidgetItem(QPixmap(GuiUtilsNs::getIconPath("keyword")), kw);
			item->setToolTip(tr("SQL Keyword"));
			name_list->addItem(item);
		}
	}

	if(name_list->count()==0)
	{
		name_list->addItem(tr("(no items found)"));
		name_list->item(0)->setFlags(Qt::NoItemFlags);
		QToolTip::hideText();
	}
	else
		name_list->item(0)->setSelected(true);

	//Sets the list position right below of text cursor
	completion_wgt->move(code_field_txt->viewport()->mapToGlobal(code_field_txt->cursorRect().bottomLeft()));
	name_list->scrollToTop();
	name_list->setFocus();
}

void CodeCompletionWidget::selectItem()
{
	if(!name_list->selectedItems().isEmpty())
	{
		QListWidgetItem *item=name_list->selectedItems().at(0);

		if(qualifying_level < 0)
			code_field_txt->setTextCursor(new_txt_cur);

		//If the selected item is a object (data not null)
		if(!item->data(Qt::UserRole).isNull())
		{
			BaseObject *object=nullptr;
			QTextCursor tc;

			//Try to retrieve the object's reference
			object=reinterpret_cast<BaseObject *>(item->data(Qt::UserRole).value<void *>());

			/* Move the cursor to the start of the word because all the chars will be replaced
			with the object name */
			prev_txt_cur.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

			tc=prev_txt_cur;
			tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

			/* An small workaround to correctly write the object name in the current
			qualifying level without remove the parent's name. This happens only when
			the completion is marked as persistent */
			if(always_on_top_chk->isChecked())
			{
				if(tc.selectedText().startsWith(completion_trigger))
				{
					prev_txt_cur.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);

					if(!tc.selectedText().endsWith(completion_trigger))
						prev_txt_cur.insertText(completion_trigger);
				}
				else if(qualifying_level >= 0 && !tc.selectedText().endsWith(completion_trigger))
				{
					prev_txt_cur.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);
					prev_txt_cur.insertText(completion_trigger);
				}
			}
			else if(tc.selectedText().contains('"'))
				prev_txt_cur=tc;

			code_field_txt->setTextCursor(prev_txt_cur);
			insertObjectName(object);
			setQualifyingLevel(object);
		}
		// If we are selecting items from the database catalog
		else if(catalog.isConnectionValid())
		{
			QTextCursor tc = code_field_txt->textCursor();

			// If the word is not empty we replace it by the selected item in the list
			if(!word.isEmpty() && word != completion_trigger && word != ",")
				tc.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
			// If the current word is the completion trigger or a comma we preserve the char
			else if(word == completion_trigger || word == ",")
				tc.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);

			code_field_txt->setTextCursor(tc);
			code_field_txt->insertPlainText(BaseObject::formatName(item->text()));
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
	auto_triggered=false;

	if(!always_on_top_chk->isChecked())
		this->close();
}

void CodeCompletionWidget::showItemTooltip()
{
	QListWidgetItem *item = name_list->currentItem();

	if(item)
	{
		QPoint pos = name_list->mapToGlobal(QPoint(name_list->width(), name_list->geometry().top()));
		QToolTip::showText(pos, item->toolTip());
	}
}

void CodeCompletionWidget::close()
{
	name_list->clearSelection();
	completion_wgt->close();
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
			(PhysicalTable::isPhysicalTable(obj_type) || TableObject::isTableObject(obj_type)))
	{
		if(PhysicalTable::isPhysicalTable(obj_type))
		{
			PhysicalTable *tab=dynamic_cast<PhysicalTable *>(obj);

			name+=QString("(");
			for(unsigned i=0; i < tab->getColumnCount(); i++)
				name+=tab->getColumn(i)->getName(true) + QString(",");

			name.remove(name.size()-1, 1);
			name+=QString(")");
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
	else if(obj_type==ObjectType::Function)
	{
		Function *func=dynamic_cast<Function *>(obj);
		func->createSignature(true, sch_qualified);
		name=func->getSignature();
	}
	else if(obj_type==ObjectType::Cast)
	{
		name.replace(',', QLatin1String(" AS "));
	}
	else if(obj_type==ObjectType::Aggregate)
	{
		Aggregate *agg;
		agg=dynamic_cast<Aggregate *>(obj);
		name+=QString("(");

		if(agg->getDataTypeCount()==0)
			name+='*';
		else
		{
			for(unsigned i=0; i < agg->getDataTypeCount(); i++)
				name+=~agg->getDataType(i) + ',';
			name.remove(name.size()-1, 1);
		}

		name+=')';
	}

	code_field_txt->insertPlainText(name);
}
