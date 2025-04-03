#ifndef TIPOFDAYWIDGET_H
#define TIPOFDAYWIDGET_H

#include <QWidget>
#include "ui_tipofdayform.h"

class TipOfDayForm : public QWidget, public Ui::TipOfDayForm {
	Q_OBJECT

	public:
		explicit TipOfDayForm(QWidget *parent = nullptr);
		~TipOfDayForm();
};

#endif // TIPOFDAYWIDGET_H
