#include "taskprogresswidget.h"

TaskProgressWidget::TaskProgressWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
	setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

void TaskProgressWidget::addIcon(unsigned id, const QIcon &ico)
{
	icons[id]=ico;
}

void TaskProgressWidget::updateProgress(int progress, const QString &text, unsigned icon_id)
{
	if(progress > progress_pb->maximum())
		progress=progress_pb->maximum();

	progress_pb->setValue(progress);
	text_lbl->setText(text);

	if(icons.count(icon_id))
		icon_lbl->setPixmap(icons[icon_id].pixmap(QSize(32,32)));
	else
		icon_lbl->clear();

	this->repaint();
}

void TaskProgressWidget::close(void)
{
	progress_pb->setValue(0);
	text_lbl->clear();
	icon_lbl->clear();
	QWidget::close();
}

