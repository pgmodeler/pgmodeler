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
																	{ aggregate_tb,    std::make_tuple(trUtf8("A"), ObjAggregate)    },
																	{ cast_tb,         std::make_tuple(trUtf8("G"), ObjCast)         },
																	{ eventtrigger_tb, std::make_tuple(trUtf8("K"), ObjEventTrigger)},
																	{ collation_tb,    std::make_tuple(trUtf8("H"), ObjCollation)    },
																	{ conversion_tb,   std::make_tuple(trUtf8("J"), ObjConversion)   },
																	{ domain_tb,       std::make_tuple(trUtf8("D"), ObjDomain)       },
																	{ extension_tb,    std::make_tuple(trUtf8("E"), ObjExtension)    },
																	{ function_tb,     std::make_tuple(trUtf8("F"), ObjFunction)     },
																	{ language_tb,     std::make_tuple(trUtf8("L"), ObjLanguage)     },
																	{ opclass_tb,      std::make_tuple(trUtf8("O"), ObjOpClass)      },
																	{ operator_tb,     std::make_tuple(trUtf8("U"), ObjOperator)     },
																	{ opfamily_tb,     std::make_tuple(trUtf8("I"), ObjOpFamily)     },
																	{ role_tb,         std::make_tuple(trUtf8("R"), ObjRole)         },
																	{ schema_tb,       std::make_tuple(trUtf8("S"), ObjSchema)       },
																	{ sequence_tb,     std::make_tuple(trUtf8("Q"), ObjSequence)     },
																	{ table_tb,        std::make_tuple(trUtf8("T"), ObjTable)        },
																	{ tablespace_tb,   std::make_tuple(trUtf8("P"), ObjTablespace)   },
																	{ textbox_tb,      std::make_tuple(trUtf8("M"), ObjTextbox)      },
																	{ type_tb,         std::make_tuple(trUtf8("Y"), ObjType)         },
																	{ view_tb,         std::make_tuple(trUtf8("W"), ObjView)         },
																	{ tag_tb,          std::make_tuple(trUtf8("9"), ObjTag)          },
																	{ constraint_tb,   std::make_tuple(trUtf8("Z"), ObjConstraint)   },
																	{ index_tb,        std::make_tuple(trUtf8("X"), ObjIndex)        },
																	{ column_tb,       std::make_tuple(trUtf8("C"), ObjColumn)       },
																	{ rule_tb,         std::make_tuple(trUtf8("V"), ObjRule)         },
																	{ trigger_tb,      std::make_tuple(trUtf8("B"), ObjTrigger)      },
																	{ policy_tb,       std::make_tuple(trUtf8("9"), ObjPolicy)       },
																	{ genericsql_tb,   std::make_tuple(trUtf8("8"), ObjGenericSQL)  } };

	map<QToolButton *, tuple<QString, int>> rel_shortcuts={ { rel11_tb,  std::make_tuple(trUtf8("1"), 0) },
															{ rel1n_tb,  std::make_tuple(trUtf8("2"), 1) },
															{ relnn_tb,  std::make_tuple(trUtf8("3"), 2) },
															{ reldep_tb, std::make_tuple(trUtf8("5"), 3) },
															{ relgen_tb, std::make_tuple(trUtf8("4"), 4) },
															{ relpart_tb, std::make_tuple(trUtf8("5"), 5) }};

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
	ObjectType obj_type=ObjBaseObject;

	if(sel_objs.size()==1)
		obj_type=sel_objs.at(0)->getObjectType();
	else if(sel_objs.empty())
		obj_type=ObjDatabase;

    db_objs_grp->setVisible(obj_type==ObjDatabase);
    sch_objs_grp->setVisible(obj_type==ObjDatabase || obj_type==ObjSchema);

    tab_objs_grp->setVisible(obj_type==ObjTable || obj_type==ObjView || obj_type==ObjRelationship);
	column_tb->setDisabled(obj_type==ObjView);
	constraint_tb->setDisabled(obj_type==ObjView);
	index_tb->setVisible(obj_type==ObjTable || obj_type==ObjView);
	rule_tb->setVisible(obj_type==ObjTable || obj_type==ObjView);
	trigger_tb->setVisible(obj_type==ObjTable || obj_type==ObjView);
    policy_tb->setVisible(obj_type==ObjTable || obj_type==ObjView);
	tab_perms_tb->setVisible(obj_type==ObjTable || obj_type==ObjView);
    rels_grp->setVisible((sel_objs.size()==1 && sel_objs.at(0)->getObjectType()==ObjTable) ||
                         (sel_objs.size()==2 && sel_objs.at(0)->getObjectType()==ObjTable && sel_objs.at(1)->getObjectType()==ObjTable));

	overlay_frm->adjustSize();
	this->adjustSize();
}
