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

#include "gqbcorewidget.h"
#include "pgmodeleruins.h"

GqbCoreWidget::GqbCoreWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	connect(insert_btn, SIGNAL(clicked(bool)), this, SLOT(insertObjects(void)));
	connect(hide_tb, SIGNAL(clicked(void)), this, SLOT(hide(void)));
	connect(reset_btn, SIGNAL(clicked(bool)), this, SLOT(resetQuery(void)));
	connect(show_sql_btn, SIGNAL(clicked(bool)), this, SLOT(produceSQL(void)));

	this->setModel(nullptr);
	this->installEventFilter(this);
}

bool GqbCoreWidget::eventFilter(QObject *object, QEvent *event)
{
	QKeyEvent *k_event=dynamic_cast<QKeyEvent *>(event);

	//Show sql when user presses enter/return on the pattern field
	if(event->type() == QEvent::KeyPress &&
			(k_event->key()==Qt::Key_Return || k_event->key()==Qt::Key_Enter))
	{
		show_sql_btn->click();
		return(true);
	}
	else
		return(QWidget::eventFilter(object, event));
}

void GqbCoreWidget::hide(void)
{
	QWidget::hide();
	emit s_visibilityChanged(false);
}

void GqbCoreWidget::setModel(ModelWidget *model_wgt)
{
	bool enable=model_wgt!=nullptr;
	this->model_wgt=model_wgt;
	this->resetQuery();
	insert_btn->setEnabled(enable);
	reset_btn->setEnabled(enable);
	show_sql_btn->setEnabled(enable);
	tab_wgt->setEnabled(enable);
}

void GqbCoreWidget::resetQuery(void)
{
	tab_wgt->setColumnCount(0);
	ord_query_data.clear();
	ord_query_rels.clear();
}

void GqbCoreWidget::insertObjects(void)
{
	if(!model_wgt)
		return;

	this->resetQuery();

	//Get the selected scene items ordered in two containers : one for data, one for relations.
	for(const auto& item: model_wgt->getScene()->selectedItems())
	{
		auto obj=dynamic_cast<BaseObjectView *>(item);
		switch (obj->getSourceObject()->getObjectType())
		{
			case ObjectType::Table:
			case ObjectType::View:
			case ObjectType::Column:
				ord_query_data.insert(static_cast<int>(obj->getSelectionOrder()), obj);
				break;

			case ObjectType::Relationship:
			case ObjectType::BaseRelationship:
				ord_query_rels.insert(static_cast<int>(obj->getSelectionOrder()), obj);
				break;

			default:
				break;
		}
	}

	//Iterate over the data container to populate the query table.
	QMap<int, BaseObjectView *>::iterator map_itr = ord_query_data.begin();
	tab_wgt->setUpdatesEnabled(false);
	tab_wgt->setSortingEnabled(false);

	int ctr=0;
	for(map_itr=ord_query_data.begin() ; map_itr!=ord_query_data.end() ; map_itr++)
	{
		QTableWidgetItem *tab_item=nullptr;
		bool new_col = false;

		if(ctr >= tab_wgt->columnCount())
		{
		  tab_wgt->insertColumn(ctr);
		  new_col = true;
		}

		if(new_col)
		{
			auto * w = new QWidget;
			auto *chb_wgt=new QCheckBox;
			auto * l = new QHBoxLayout(w);
			l->addWidget(chb_wgt);
			l->setAlignment(Qt::AlignCenter);
			l->setContentsMargins(0,0,0,0);
			chb_wgt->setToolTip("Include this column in the select clause");
			tab_wgt->setCellWidget(0,ctr,w);
		}
		qobject_cast<QCheckBox *>(tab_wgt->cellWidget(0,ctr)->children().last())->setCheckState(Qt::Checked);

		// [three lines] x [Table/View or Column]
		if(map_itr.value()->getSourceObject()->getObjectType()==ObjectType::Table ||
				map_itr.value()->getSourceObject()->getObjectType()==ObjectType::View  )
		{
			//First line: Schema
			tab_item=(new_col ? new QTableWidgetItem : tab_wgt->item(tW_Schema, ctr));
			tab_item->setText(map_itr.value()->getSourceObject()->getSchema()->getName());
			if(new_col) tab_wgt->setItem(tW_Schema, ctr, tab_item);

			//Second line: table/view
			tab_item=(new_col ? new QTableWidgetItem : tab_wgt->item(tW_Table, ctr));
			tab_item->setText(map_itr.value()->getSourceObject()->getName());
			tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(
								  dynamic_cast<BaseTable *>(map_itr.value()->getSourceObject())));
			if(new_col) tab_wgt->setItem(tW_Table, ctr, tab_item);

			//Third line: column
			tab_item=(new_col ? new QTableWidgetItem : tab_wgt->item(tW_Column, ctr));
			tab_item->setText("*");
			if(new_col) tab_wgt->setItem(tW_Column, ctr, tab_item);
		}

		if(map_itr.value()->getSourceObject()->getObjectType()==ObjectType::Column)
		{
			//First line: Schema
			tab_item=(new_col ? new QTableWidgetItem : tab_wgt->item(tW_Schema, ctr));
			tab_item->setText(dynamic_cast<TableObject *>(map_itr.value()->getSourceObject())->getParentTable()->getSchema()->getName());
			if(new_col) tab_wgt->setItem(tW_Schema, ctr, tab_item);

			//Second line: table/view
			tab_item=(new_col ? new QTableWidgetItem : tab_wgt->item(tW_Table, ctr));
			tab_item->setText(dynamic_cast<TableObject *>(map_itr.value()->getSourceObject())->getParentTable()->getName());
			tab_item->setData(Qt::UserRole, QVariant::fromValue<void *>(
									  dynamic_cast<BaseTable *>(
									  dynamic_cast<TableObject *>(map_itr.value()->getSourceObject())->getParentTable())));
			if(new_col) tab_wgt->setItem(tW_Table, ctr, tab_item);

			//Third line: column
			tab_item=(new_col ? new QTableWidgetItem : tab_wgt->item(tW_Column, ctr));
			tab_item->setText(map_itr.value()->getSourceObject()->getName());
			if(new_col) tab_wgt->setItem(tW_Column, ctr, tab_item);
		}

		tab_item=(new_col ? new QTableWidgetItem : tab_wgt->item(tW_Alias, ctr));
		if(new_col)
		{
			tab_wgt->setItem(tW_Alias, ctr, tab_item);
			auto currentFlags = tab_wgt->item(tW_Alias,ctr)->flags();
			tab_wgt->item(tW_Alias,ctr)->setFlags(currentFlags & (~Qt::ItemIsEditable));
		}

		if(new_col)
		{
			auto * w = new QWidget;
			auto *chb_wgt=new QCheckBox;
			auto * l = new QHBoxLayout(w);
			l->addWidget(chb_wgt);
			l->setAlignment(Qt::AlignCenter);
			l->setContentsMargins(0,0,0,0);
			chb_wgt->setToolTip("Include this column in the group clause");
			tab_wgt->setCellWidget(tW_Group,ctr,w);
		}
		qobject_cast<QCheckBox *>(tab_wgt->cellWidget(tW_Group,ctr)->children().last())->setCheckState(Qt::Unchecked);

		if(new_col)
		{
			auto * w = new QWidget;
			auto *cb_wgt=new QComboBox;
			cb_wgt->insertItem(0,"");
			cb_wgt->insertItem(1,"ASC");
			cb_wgt->insertItem(2,"DESC");
			auto * l = new QHBoxLayout(w);
			l->addWidget(cb_wgt);
			l->setContentsMargins(0,0,0,0);
			tab_wgt->setCellWidget(tW_Order,ctr,w);
		}

		//Set the editable/enabled flag/states accordingly.
		if(tab_wgt->item(tW_Column,ctr)->text()=="*")
		{
			if(new_col)
			{
				tab_item=new QTableWidgetItem;
				tab_wgt->setItem(tW_Where, ctr, tab_item);
			}
			auto currentFlags = tab_wgt->item(tW_Where,ctr)->flags();
			tab_wgt->item(tW_Where,ctr)->setFlags(currentFlags & (~Qt::ItemIsEditable));
			if(new_col)
			{
				tab_item=new QTableWidgetItem;
				tab_wgt->setItem(tW_Having, ctr, tab_item);
			}
			currentFlags = tab_wgt->item(tW_Having,ctr)->flags();
			tab_wgt->item(tW_Having,ctr)->setFlags(currentFlags & (~Qt::ItemIsEditable));

			qobject_cast<QWidget *>(tab_wgt->cellWidget(tW_Group,ctr))->setEnabled(false);
			qobject_cast<QWidget *>(tab_wgt->cellWidget(tW_Order,ctr))->setEnabled(false);
		}
		else if(tab_wgt->item(tW_Column,ctr)->text()!="*" && !new_col)
		{
			auto currentFlags = tab_wgt->item(tW_Where,ctr)->flags();
			tab_wgt->item(tW_Where,ctr)->setFlags(currentFlags & (Qt::ItemIsEditable));
			currentFlags = tab_wgt->item(tW_Having,ctr)->flags();
			tab_wgt->item(tW_Having,ctr)->setFlags(currentFlags & (Qt::ItemIsEditable));

			qobject_cast<QWidget *>(tab_wgt->cellWidget(tW_Group,ctr))->setEnabled(true);
			qobject_cast<QWidget *>(tab_wgt->cellWidget(tW_Order,ctr))->setEnabled(true);
		}
		else if(new_col)
		{
			auto currentFlags = tab_wgt->item(tW_Schema,ctr)->flags();
			tab_wgt->item(tW_Schema,ctr)->setFlags(currentFlags & (~Qt::ItemIsEditable));
			currentFlags = tab_wgt->item(tW_Table,ctr)->flags();
			tab_wgt->item(tW_Table,ctr)->setFlags(currentFlags & (~Qt::ItemIsEditable));
			currentFlags = tab_wgt->item(tW_Column,ctr)->flags();
			tab_wgt->item(tW_Column,ctr)->setFlags(currentFlags & (~Qt::ItemIsEditable));
		}

		ctr+=1;
	}

	if(static_cast<int>(ord_query_data.size() != tab_wgt->columnCount()))
		tab_wgt->setColumnCount(ord_query_data.size());
	tab_wgt->setUpdatesEnabled(true);
	tab_wgt->setSortingEnabled(false);

}

void GqbCoreWidget::produceSQL(void)
{
	QString select_cl="SELECT ", from_cl="FROM ", where_cl="WHERE ", group_cl="GROUP BY ", having_cl="HAVING ",
			order_cl="ORDER BY ", result;
	msg=nullptr;

	if(tab_wgt->columnCount()==0)
	{
		result=("SELECT v_schema.table.Hello FROM world!");
		emit s_gqbSqlRequested(result);
		return;
	}

	//'FROM' clause
	if(tab_wgt->columnCount()==1)
	{
		from_cl+= tab_wgt->item(tW_Schema,0)->text() + "." + tab_wgt->item(tW_Table,0)->text() + "\n";
	}

	else
	{
		join_in_where=false;
		QVector < QPair< BaseTable *, QVector < QPair<Column *, Column *> > > > path;
		QVector < QPair<Column *, Column *> >::iterator col_itr;

		path=getQueryPath();

		if(join_in_where)
			msg+="At least one <strong>cycle</strong> has been found in the join path : "
				 "'<strong>join</strong> will happen <strong>in where</strong>'.<br/><br/>";

		QVector < QPair< BaseTable *, QVector < QPair<Column *, Column *> > > >::iterator path_itr;
		for (path_itr=path.begin(); path_itr!=path.end(); path_itr++)
		{
			if(path.empty())
				break;

			if(path_itr==path.begin())
				from_cl+=path.front().first->getSchema()->getName() + "." + path.front().first->getName();
			else
			{

				if (join_in_where)
				{
					if(path_itr->first!=nullptr)
						from_cl+=", " + path_itr->first->getSchema()->getName() + "." + path_itr->first->getName();

					for(col_itr=path_itr->second.begin(); col_itr!=path_itr->second.end(); col_itr++)
					{
						//TODO manage disambiguation of schema/table/column names globally
						if(col_itr->first->getName()!=col_itr->second->getName())
							where_cl+=(where_cl=="WHERE "? "" : "\n\t\tAND ") +
									col_itr->first->getName() + "=" + col_itr->second->getName();
						else if(col_itr->first->getParentTable()->getName()!=col_itr->second->getParentTable()->getName())
							where_cl+=(where_cl=="WHERE "? "" : "\n\t\tAND ") +
							  col_itr->first->getParentTable()->getName() + "." + col_itr->first->getName() + "=" +
							  col_itr->second->getParentTable()->getName() + "." + col_itr->second->getName();
					}
				}
				else
				{
					from_cl+="\nJOIN " + path_itr->first->getSchema()->getName() + "." + path_itr->first->getName() + "\n";

					for(col_itr=path_itr->second.begin(); col_itr!=path_itr->second.end(); col_itr++)
					{
						from_cl+=(col_itr==path_itr->second.begin() ? "\t\tON " : "\t\tAND ");

						if(col_itr->first->getName()!=col_itr->second->getName())
							from_cl+=col_itr->first->getName() + "=" + col_itr->second->getName();
						else if(col_itr->first->getParentTable()->getName()!=col_itr->second->getParentTable()->getName())
							from_cl+=
							  col_itr->first->getParentTable()->getName() + "." + col_itr->first->getName() + "=" +
							  col_itr->second->getParentTable()->getName() + "." + col_itr->second->getName();
					}
				}
			}
		}

		if(!path.empty())
			from_cl+=(!disconnected_vertices.empty()?",\n":"\n");
		if(!disconnected_vertices.empty())
		{
			if(required_vertices.size()!=1)
				msg+="A valid join <strong>path</strong> has <strong>not</strong> been <strong>found</strong> for some tables :<br/>"
					 "these tables will be joined with a <strong>sheer cartesian product</strong>!<br/><br/>";

			for(int k=0;k<disconnected_vertices.size();k++)
			{
				from_cl+=disconnected_vertices[k]->getName() + (k==disconnected_vertices.size()-1 ? "\n": ", ");
			}
		}
	}

	for (int i=0; i<tab_wgt->columnCount();i++)
	{
		//'SELECT' clause
		if(qobject_cast<QCheckBox *>(tab_wgt->cellWidget(tW_Selection,i)->children().last())->checkState()==Qt::Checked)
				select_cl+= tab_wgt->item(tW_Table,i)->text() + "." +
						   tab_wgt->item(tW_Column,i)->text() +
						   (i==tab_wgt->columnCount()-1?"\n":",\n\t\t");

		//'WHERE' clause
		if (tab_wgt->item(tW_Where,i) && tab_wgt->item(tW_Where,i)->text()!="")
				where_cl+= (where_cl=="WHERE "? "" : "\n\t\tAND ") + tab_wgt->item(tW_Column,i)->text() +
							   tab_wgt->item(tW_Where,i)->text();
		//'GROUP BY' clause
		if(qobject_cast<QCheckBox *>(tab_wgt->cellWidget(6,i)->children().last())->checkState()==Qt::Checked)
				group_cl+= (group_cl=="GROUP BY "? "" : ",\n\t\t") + tab_wgt->item(tW_Column,i)->text();
		//'HAVING' clause
		if (tab_wgt->item(7,i) && tab_wgt->item(7,i)->text()!="")
				having_cl+= (having_cl=="HAVING "? "" : " AND\n\t\t") + tab_wgt->item(tW_Column,i)->text() +
							   tab_wgt->item(7,i)->text();
		//'ORDER BY' clause
		switch(qobject_cast<QComboBox *>(tab_wgt->cellWidget(8,i)->children().last())->currentIndex())
		{
			case 1:
				order_cl+= (order_cl=="ORDER BY "? "" : ",\n\t\t") + tab_wgt->item(tW_Column,i)->text() + " ASC";
				break;
			case 2:
				order_cl+= (order_cl=="ORDER BY "? "" : ",\n\t\t") + tab_wgt->item(tW_Column,i)->text() + " DESC";
				break;
			default:
				break;
		}
	}

	//Clean the dust in the optional clauses
	where_cl=="WHERE "      ? where_cl=nullptr  : where_cl+="\n";
	group_cl=="GROUP BY "   ? group_cl=nullptr  : group_cl+="\n";
	having_cl=="HAVING "    ? having_cl=nullptr : having_cl+="\n";
	order_cl=="ORDER BY "   ? order_cl=nullptr  : order_cl+="\n";

	//Agglomerate the result
		result = select_cl +
				 from_cl +
				 where_cl +
				 group_cl +
				 having_cl +
				 order_cl +
				 ";";

	if(msg!=nullptr)
	{
		Messagebox msg_box;
		msg_box.show(msg.toUtf8(), Messagebox::AlertIcon, Messagebox::OkButton);
	}
	emit s_gqbSqlRequested(result);
}

QVector < QPair< BaseTable *, QVector < QPair<Column *, Column *> > > >
	GqbCoreWidget::getQueryPath(void)
{
	//The path returned consists of a nested container : first 'FROM' table,
	//and following 'JOIN' clauses, which are a table and its 'ON' statements : a vector of pairs of columns.
	QVector < QPair< BaseTable *, QVector < QPair<Column *, Column *> > > >  result;

	//another stack container to store the pairs of 'ON' statements.
	QVector < QPair<Column *, Column *> > col_vec;

	//temporary-stored JOIN tables during the iteration
	BaseTable *join_tb;

	//Custom-Depth-First-Search heap containers
	required_vertices.clear();
	visited_vertices.clear();
	disconnected_vertices.clear();
	dfs_rels.clear();
	dfs_result.clear();

	//Set root vertex at first table in the gqb widget :
	//For now the DFS is done from only one root.
	BaseTable * root_vertex = reinterpret_cast<BaseTable *>(tab_wgt->item(tW_Table,0)->data(Qt::UserRole).value<void *>());

	//Populate the list of required (selected - graphically not in tablewidget) vertices (tables)
	bool already_in;
	for (int i=0; i<tab_wgt->columnCount();i++)
	{
		already_in=false;
		for (auto const &req_item: required_vertices)
		{
			if(req_item==reinterpret_cast<BaseTable *>(tab_wgt->item(tW_Table,i)->data(Qt::UserRole).value<void *>()))
			{
					already_in=true;
					break;
			}
		}
		if(!already_in)
		{
			required_vertices.push_back(reinterpret_cast<BaseTable *>(tab_wgt->item(tW_Table,i)->data(Qt::UserRole).value<void *>()));
		}
	}

	// Populate the relation container used for DFS : a vector of pairs (visited flag + rel)
	for(const auto &rel_itr:ord_query_rels)
	{
		auto * base_rel = dynamic_cast<BaseRelationship *>(rel_itr->getSourceObject());
		if( base_rel->getRelTypeAttribute()==Attributes::RelationshipFk ||
			base_rel->getRelTypeAttribute()==Attributes::Relationship11 ||
			base_rel->getRelTypeAttribute()==Attributes::Relationship1n )
		{
				dfs_rels.append(qMakePair(false,
									 dynamic_cast<BaseRelationship *>(rel_itr->getSourceObject())));
		}

		else
		{
			msg+="Relationships with <strong>types</strong> other than fk, 1-to-1 and 1-to-many are "
				 "<strong>discarded</strong> for now!<br/><br/>";
		}
	}

	customDFS(root_vertex);

	for (const auto &vertex:required_vertices)
	{
		if(visited_vertices.indexOf(vertex)==-1)
			disconnected_vertices.append(vertex);
	}

	QVector <QPair<short, BaseRelationship *>>::iterator dfs_res_itr;
	for(dfs_res_itr=dfs_result.begin();dfs_res_itr!=dfs_result.end(); dfs_res_itr++)
	{
		col_vec.clear();

		if(dfs_res_itr==dfs_result.begin())
		{
			//Add only in first entry the first table - this is the 'FROM' part, no 'ON' cols.
			col_vec.push_back(qMakePair(nullptr,nullptr));
			result.push_back(qMakePair(
				root_vertex,
				col_vec));
			col_vec.clear();
		}

		switch(dfs_res_itr->first)
		{
			case DFS_flowRel:
			join_tb= dfs_res_itr->second->getTable(BaseRelationship::DstTable);
				break;
			case DFS_reverseRel:
				join_tb= dfs_res_itr->second->getTable(BaseRelationship::SrcTable);
				break;
			default:
				join_tb=nullptr;
		}

		auto src_cols=dfs_res_itr->second->reference_fk->getColumns(Constraint::SourceCols);
		auto ref_cols=dfs_res_itr->second->reference_fk->getColumns(Constraint::ReferencedCols);
		for(unsigned long i=0;i<src_cols.size();i++)
		{
			col_vec.append(dfs_res_itr->first%2==0?
						(qMakePair(src_cols[i], ref_cols[i])) :
						(qMakePair(ref_cols[i], src_cols[i])));
		}
		result.append(qMakePair(
				join_tb,
				col_vec));
	}
	if(result.empty())
		disconnected_vertices.push_front(root_vertex);

	return result;
}

void GqbCoreWidget::customDFS(BaseTable * current_vertex)
{
	//This function is recursive so it would need to be reentrant=stack-only,
	//but we manipulate data cross-calls :
	//we store meaningful containers in heap (class attributes) not in stack.
	if (visited_vertices.indexOf(current_vertex)!=-1)
	{
		join_in_where=true;
		dfs_result.back().first+=2;
		return;
	}

	visited_vertices.push_back(current_vertex);

	QVector<QPair<bool, BaseRelationship *>>::iterator dfs_rel_itr;
	for (dfs_rel_itr=dfs_rels.begin();dfs_rel_itr!=dfs_rels.end();dfs_rel_itr++)
	{
		if ((dfs_rel_itr->second->getTable(BaseRelationship::SrcTable)==current_vertex ||
			 dfs_rel_itr->second->getTable(BaseRelationship::DstTable)==current_vertex) &&
			 !dfs_rel_itr->first /* this rel has not yet been traversed */)
		{
				dfs_result.append(qMakePair(
						static_cast<short>(dfs_rel_itr->second->getTable(BaseRelationship::SrcTable)!=current_vertex),
						dfs_rel_itr->second));
				dfs_rel_itr->first=true;
				customDFS( (dfs_result.back().first%2==0 ?
								 dfs_rel_itr->second->getTable(BaseRelationship::DstTable) :
								 dfs_rel_itr->second->getTable(BaseRelationship::SrcTable)) );
		}
	}
}
