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
																	{ aggregate_tb,    std::make_tuple(trUtf8("A"), OBJ_AGGREGATE)    },
																	{ cast_tb,         std::make_tuple(trUtf8("G"), OBJ_CAST)         },
																	{ eventtrigger_tb, std::make_tuple(trUtf8("K"), OBJ_EVENT_TRIGGER)},
																	{ collation_tb,    std::make_tuple(trUtf8("H"), OBJ_COLLATION)    },
																	{ conversion_tb,   std::make_tuple(trUtf8("J"), OBJ_CONVERSION)   },
																	{ domain_tb,       std::make_tuple(trUtf8("D"), OBJ_DOMAIN)       },
																	{ extension_tb,    std::make_tuple(trUtf8("E"), OBJ_EXTENSION)    },
																	{ function_tb,     std::make_tuple(trUtf8("F"), OBJ_FUNCTION)     },
																	{ language_tb,     std::make_tuple(trUtf8("L"), OBJ_LANGUAGE)     },
																	{ opclass_tb,      std::make_tuple(trUtf8("O"), OBJ_OPCLASS)      },
																	{ operator_tb,     std::make_tuple(trUtf8("U"), OBJ_OPERATOR)     },
																	{ opfamily_tb,     std::make_tuple(trUtf8("I"), OBJ_OPFAMILY)     },
																	{ role_tb,         std::make_tuple(trUtf8("R"), OBJ_ROLE)         },
																	{ schema_tb,       std::make_tuple(trUtf8("S"), OBJ_SCHEMA)       },
																	{ sequence_tb,     std::make_tuple(trUtf8("Q"), OBJ_SEQUENCE)     },
																	{ table_tb,        std::make_tuple(trUtf8("T"), OBJ_TABLE)        },
																	{ tablespace_tb,   std::make_tuple(trUtf8("P"), OBJ_TABLESPACE)   },
																	{ textbox_tb,      std::make_tuple(trUtf8("M"), OBJ_TEXTBOX)      },
																	{ type_tb,         std::make_tuple(trUtf8("Y"), OBJ_TYPE)         },
																	{ view_tb,         std::make_tuple(trUtf8("W"), OBJ_VIEW)         },
																	{ tag_tb,          std::make_tuple(trUtf8("9"), OBJ_TAG)          },
																	{ constraint_tb,   std::make_tuple(trUtf8("Z"), OBJ_CONSTRAINT)   },
																	{ index_tb,        std::make_tuple(trUtf8("X"), OBJ_INDEX)        },
																	{ column_tb,       std::make_tuple(trUtf8("C"), OBJ_COLUMN)       },
																	{ rule_tb,         std::make_tuple(trUtf8("V"), OBJ_RULE)         },
																	{ trigger_tb,      std::make_tuple(trUtf8("B"), OBJ_TRIGGER)      },
																	{ genericsql_tb,   std::make_tuple(trUtf8("8"), OBJ_GENERIC_SQL)  } };

	map<QToolButton *, tuple<QString, int>> rel_shortcuts={ { rel11_tb,  std::make_tuple(trUtf8("1"), 0) },
															{ rel1n_tb,  std::make_tuple(trUtf8("2"), 1) },
															{ relnn_tb,  std::make_tuple(trUtf8("3"), 2) },
															{ reldep_tb, std::make_tuple(trUtf8("5"), 3) },
															{ relgen_tb, std::make_tuple(trUtf8("4"), 4) } };

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

	shortcut=trUtf8("0");
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
	ObjectType obj_type=BASE_OBJECT;

	if(sel_objs.size()==1)
		obj_type=sel_objs.at(0)->getObjectType();
	else if(sel_objs.empty())
		obj_type=OBJ_DATABASE;

	db_objs_btns_wgt->setVisible(obj_type==OBJ_DATABASE);
	sch_objs_btns_wgt->setVisible(obj_type==OBJ_DATABASE || obj_type==OBJ_SCHEMA);

	tab_objs_btns_wgt->setVisible(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW || obj_type==OBJ_RELATIONSHIP);
	column_tb->setDisabled(obj_type==OBJ_VIEW);
	constraint_tb->setDisabled(obj_type==OBJ_VIEW);
	index_tb->setVisible(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW);
	rule_tb->setVisible(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW);
	trigger_tb->setVisible(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW);
	tab_perms_tb->setVisible(obj_type==OBJ_TABLE || obj_type==OBJ_VIEW);
	rel_btns_wgt->setVisible((sel_objs.size()==1 && sel_objs.at(0)->getObjectType()==OBJ_TABLE) ||
													 (sel_objs.size()==2 && sel_objs.at(0)->getObjectType()==OBJ_TABLE && sel_objs.at(1)->getObjectType()==OBJ_TABLE));

	overlay_frm->adjustSize();
	this->adjustSize();
}
