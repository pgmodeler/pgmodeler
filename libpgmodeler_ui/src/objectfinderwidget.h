#ifndef OBJECT_FINDER_WIDGET_H
#define OBJECT_FINDER_WIDGET_H

#include "ui_objectfinderwidget.h"
#include <QWidget>

class ObjectFinderWidget : public QWidget, public Ui::ObjectFinderWidget {
	private:
		Q_OBJECT

	public:
		ObjectFinderWidget(QWidget *parent = 0);
		
	signals:
		void s_visibilityChanged(bool);
		
	public slots:
		void hide(void);
		
};

#endif
