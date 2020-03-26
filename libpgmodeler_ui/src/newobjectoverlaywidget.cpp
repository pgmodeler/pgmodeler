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

#include "newobjectoverlaywidget.h"
#include "modelwidget.h"

NewObjectOverlayWidget::NewObjectOverlayWidget(ModelWidget *parent): QWidget(parent)
{
	Ui_NewObjectOverlayWidget::setupUi(this);

	QToolButton *button=nullptr;
	ObjectType obj_type;
	QString shortcut;
	int action_idx=0;
	QList<QAction *> rel_actions=parent->rels_menu->actions();
	map<QToolButton *, tuple<QString, ObjectType>>  obj_shortcuts={
										{ aggregate_tb,    std::make_tuple(tr("A"), ObjectType::Aggregate)    },
										{ cast_tb,         std::make_tuple(tr("G"), ObjectType::Cast)         },
										{ eventtrigger_tb, std::make_tuple(tr("K"), ObjectType::EventTrigger)},
										{ collation_tb,    std::make_tuple(tr("H"), ObjectType::Collation)    },
										{ conversion_tb,   std::make_tuple(tr("J"), ObjectType::Conversion)   },
										{ domain_tb,       std::make_tuple(tr("D"), ObjectType::Domain)       },
										{ extension_tb,    std::make_tuple(tr("E"), ObjectType::Extension)    },
										{ function_tb,     std::make_tuple(tr("F"), ObjectType::Function)     },
										{ language_tb,     std::make_tuple(tr("L"), ObjectType::Language)     },
										{ opclass_tb,      std::make_tuple(tr("O"), ObjectType::OpClass)      },
										{ operator_tb,     std::make_tuple(tr("U"), ObjectType::Operator)     },
										{ opfamily_tb,     std::make_tuple(tr("I"), ObjectType::OpFamily)     },
										{ role_tb,         std::make_tuple(tr("R"), ObjectType::Role)         },
										{ schema_tb,       std::make_tuple(tr("S"), ObjectType::Schema)       },
										{ sequence_tb,     std::make_tuple(tr("Q"), ObjectType::Sequence)     },
										{ table_tb,        std::make_tuple(tr("T"), ObjectType::Table)        },
										{ tablespace_tb,   std::make_tuple(tr("P"), ObjectType::Tablespace)   },
										{ textbox_tb,      std::make_tuple(tr("M"), ObjectType::Textbox)      },
										{ type_tb,         std::make_tuple(tr("Y"), ObjectType::Type)         },
										{ view_tb,         std::make_tuple(tr("W"), ObjectType::View)         },
										{ tag_tb,          std::make_tuple(tr("9"), ObjectType::Tag)          },
										{ constraint_tb,   std::make_tuple(tr("Z"), ObjectType::Constraint)   },
										{ index_tb,        std::make_tuple(tr("X"), ObjectType::Index)        },
										{ column_tb,       std::make_tuple(tr("C"), ObjectType::Column)       },
										{ rule_tb,         std::make_tuple(tr("V"), ObjectType::Rule)         },
										{ trigger_tb,      std::make_tuple(tr("B"), ObjectType::Trigger)      },
										{ policy_tb,       std::make_tuple(tr("9"), ObjectType::Policy)       },
										{ genericsql_tb,   std::make_tuple(tr("8"), ObjectType::GenericSql)   },
										{ fdw_tb,          std::make_tuple(tr("7"), ObjectType::ForeignDataWrapper) },
										{ server_tb,       std::make_tuple(tr("6"), ObjectType::ForeignServer) },
										{ user_mapping_tb, std::make_tuple(tr("5"), ObjectType::UserMapping) },
										{ foreigntable_tb, std::make_tuple(tr("4"), ObjectType::ForeignTable) }};

	map<QToolButton *, tuple<QString, int>> rel_shortcuts={
										{ rel11_tb,  std::make_tuple(tr("1"), 0) },
										{ rel1n_tb,  std::make_tuple(tr("2"), 1) },
										{ relnn_tb,  std::make_tuple(tr("3"), 2) },
										{ reldep_tb, std::make_tuple(tr("5"), 3) },
										{ relgen_tb, std::make_tuple(tr("4"), 4) },
										{ relpart_tb, std::make_tuple(tr("6"), 5) }};

	vector<QToolButton *> permission_btns={db_sch_perms_tb, tab_perms_tb };

	for(auto &itr : obj_shortcuts)
	{
		button=itr.first;
		shortcut=std::get<0>(itr.second);
		obj_type=std::get<1>(itr.second);

		button->setText(shortcut + QString(": ") + button->text());
		button->setShortcut(QKeySequence(shortcut));
		connect(button, SIGNAL(clicked()), this, SLOT(hide()));
		connect(button, SIGNAL(clicked()), parent->actions_new_objects[obj_type], SLOT(trigger()));
	}

	for(auto &itr : rel_shortcuts)
	{
		button=itr.first;
		shortcut=std::get<0>(itr.second);
		action_idx=std::get<1>(itr.second);

		button->setText(shortcut + QString(": ") + button->text());
		button->setShortcut(QKeySequence(shortcut));

		connect(button, SIGNAL(clicked()), this, SLOT(hide()));
		if(action_idx < rel_actions.size())
			connect(button, SIGNAL(clicked()), rel_actions[action_idx], SLOT(trigger()));
	}

	shortcut=tr("0");
	for(auto &itr : permission_btns)
	{
		button=itr;
		button->setText(shortcut + QString(": ") + button->text());
		button->setShortcut(QKeySequence(shortcut));

		connect(button, SIGNAL(clicked()), this, SLOT(hide()));
		connect(button, SIGNAL(clicked()), parent->action_edit_perms, SLOT(trigger()));
	}
}

void NewObjectOverlayWidget::setSelectedObjects(vector<BaseObject *> &sel_objs)
{
	ObjectType obj_type=ObjectType::BaseObject;

	if(sel_objs.size()==1)
		obj_type=sel_objs.at(0)->getObjectType();
	else if(sel_objs.empty())
		obj_type=ObjectType::Database;

	db_objs_grp->setVisible(obj_type==ObjectType::Database);
	sch_objs_grp->setVisible(obj_type==ObjectType::Database || obj_type==ObjectType::Schema);

	tab_objs_grp->setVisible(BaseTable::isBaseTable(obj_type) || obj_type==ObjectType::Relationship);
	column_tb->setVisible(obj_type!=ObjectType::View);
	constraint_tb->setVisible(obj_type!=ObjectType::View);
	index_tb->setVisible(obj_type==ObjectType::Table || obj_type==ObjectType::View);
	rule_tb->setVisible(obj_type==ObjectType::Table || obj_type==ObjectType::View);
	trigger_tb->setVisible(BaseTable::isBaseTable(obj_type));
	policy_tb->setVisible(obj_type==ObjectType::Table);
	tab_perms_tb->setVisible(obj_type==ObjectType::Table || obj_type==ObjectType::View);
	rels_grp->setVisible((sel_objs.size()==1 && sel_objs.at(0)->getObjectType()==ObjectType::Table) ||
											 (sel_objs.size()==2 && sel_objs.at(0)->getObjectType()==ObjectType::Table && sel_objs.at(1)->getObjectType()==ObjectType::Table));

	overlay_frm->adjustSize();
	this->adjustSize();
}
