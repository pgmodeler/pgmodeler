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

#include "textoutputwidget.h"

TextOutputWidget::TextOutputWidget(QWidget *parent): QWidget(parent)
{
	QGridLayout *grid = new QGridLayout(this);
	grid->setContentsMargins(4,4,4,4);

	save_tb = new QToolButton(this);
	save_tb->setText(tr("Save"));
	save_tb->setIcon(QPixmap(PgModelerUiNs::getIconPath("salvar")));
	save_tb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	save_tb->setIconSize(QSize(22, 22));
	save_tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	output_txt = new NumberedTextEditor(this);
	output_txt->setReadOnly(true);

	output_hl = new SyntaxHighlighter(output_txt);
	output_hl->loadConfiguration(GlobalAttributes::getSQLHighlightConfPath());

	find_wgt = new FindReplaceWidget(output_txt, this);
	find_wgt->hide_tb->setVisible(false);

	grid->addWidget(output_txt, 0, 0, 1, 3);
	grid->addWidget(find_wgt, 1, 0);
	grid->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding), 1, 1);
	grid->addWidget(save_tb, 1, 2);
}

void TextOutputWidget::setOutputWidget(QTreeWidget *output_trw)
{
	output_txt->setPlainText(generateTreeTextOutput(output_trw));
}

QString TextOutputWidget::generateTextOutput(QTreeWidget *output_trw)
{
	return generateTreeTextOutput(output_trw);
}

QString TextOutputWidget::generateTreeTextOutput(QTreeWidget *output_trw)
{
	if(!output_trw)
		return "";

	QString output;
	QTreeWidgetItem *item = nullptr;

	for(int idx = 0; idx < output_trw->topLevelItemCount(); idx ++)
	{
		item = output_trw->topLevelItem(idx);
		generateItemTextOutput(item, output, 0);
		output += '\n';
	}

	return output;
}

void TextOutputWidget::generateItemTextOutput(QTreeWidgetItem *item, QString &output, int level)
{
	if(!item)	return;

	QLabel *label = dynamic_cast<QLabel *>(item->treeWidget()->itemWidget(item, 0));
	QString text,
			filler = "\n" + QString("").fill(' ', level * 2);

	text = item->text(0);

	if(label && text.isEmpty())
		text = label->text();

	text.replace(QRegExp("(\\<)(\\/)?(br|strong|em)(\\/)?(\\>)"), "");
	text.prepend(level == 0 ? "=> " : "\n");

	text.replace("\n", filler);
	output += text;

	for(int child = 0; child < item->childCount(); child++)
	{
		generateItemTextOutput(item->child(child), output, level + 1);
	}
}

