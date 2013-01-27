#include "pgmodelerplugin.h"

PgModelerPlugin::PgModelerPlugin(void)
{
 QGridLayout *gridLayout=NULL;
 QSpacerItem *verticalSpacer=NULL;
 QFont font;
 QWidget *widget=NULL;

 plugin_info_frm=new FormBasico(NULL, Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
 plugin_info_frm->definirBotoes(MessageBox::OK_BUTTON);
 plugin_info_frm->connect(plugin_info_frm->aplicar_ok_btn, SIGNAL(clicked(void)), plugin_info_frm, SLOT(close(void)));

 gridLayout=new QGridLayout(plugin_info_frm);

 widget=new QWidget(plugin_info_frm);
 plugin_info_frm->widgetgeral_wgt->insertWidget(0, widget);
 plugin_info_frm->widgetgeral_wgt->setCurrentIndex(0);
 plugin_info_frm->setWindowTitle(QT_TRANSLATE_NOOP("PgModelerPlugin", "Plugin Information"));

 gridLayout->setHorizontalSpacing(10);
 gridLayout->setVerticalSpacing(6);
 gridLayout->setContentsMargins(6, 6, 6, 6);

 icon_lbl = new QLabel(widget);
 icon_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
 icon_lbl->setMinimumSize(QSize(32, 32));
 icon_lbl->setMaximumSize(QSize(32, 32));
 gridLayout->addWidget(icon_lbl, 0, 0, 2, 1);

 title_lbl = new QLabel(widget);
 title_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
 font.setPointSize(12);
 font.setBold(true);
 font.setItalic(true);
 font.setWeight(75);
 title_lbl->setFont(font);
 gridLayout->addWidget(title_lbl, 0, 1, 1, 1);

 author_lbl = new QLabel(widget);
 author_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
 gridLayout->addWidget(author_lbl, 1, 1, 2, 1);

 verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);
 gridLayout->addItem(verticalSpacer, 2, 0, 2, 1);

 version_lbl = new QLabel(widget);
 version_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
 gridLayout->addWidget(version_lbl, 3, 1, 1, 1);

 description_lbl = new QLabel(widget);
 description_lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
 description_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
 gridLayout->addWidget(description_lbl, 4, 0, 1, 2);

 widget->setLayout(gridLayout);

 plugin_info_frm->setMinimumSize(400, 250);
 plugin_info_frm->resize(plugin_info_frm->minimumSize());
}

PgModelerPlugin::~PgModelerPlugin(void)
{
 delete(plugin_info_frm);
}

void PgModelerPlugin::configurePluginInfo(const QString &title, const QString &version, const QString &author,
                                          const QString &description, const QString &ico_filename)
{
 QPixmap ico;

 title_lbl->setText(title);
 version_lbl->setText(QString(QT_TRANSLATE_NOOP("PgModelerPlugin", "Version: %1")).arg(version));
 author_lbl->setText(QString(QT_TRANSLATE_NOOP("PgModelerPlugin","Author: %1")).arg(author));
 description_lbl->setText(description);

 ico.load(ico_filename);
 icon_lbl->setPixmap(ico);
}
