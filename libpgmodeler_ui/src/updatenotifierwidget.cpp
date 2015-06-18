/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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

#include "updatenotifierwidget.h"
#include <iostream>

UpdateNotifierWidget::UpdateNotifierWidget(QWidget *parent) : QWidget(parent)
{
  setupUi(this);
  setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);

  show_no_upd_msg=false;
  update_chk_reply=nullptr;
  old_pos=QPoint(-1,-1);
  frame->installEventFilter(this);

	QGraphicsDropShadowEffect * drop_shadow=new QGraphicsDropShadowEffect(this);
	drop_shadow->setOffset(5,5);
	drop_shadow->setBlurRadius(30);
	this->setGraphicsEffect(drop_shadow);

  get_binary_menu=new QMenu(this);
  action_recover=get_binary_menu->addAction(trUtf8("Recover a package"));
  action_purchase=get_binary_menu->addAction(trUtf8("Purchase a new package"));
  get_binary_tb->setMenu(get_binary_menu);

  connect(&update_chk_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleUpdateChecked(QNetworkReply*)));

  //C++11 lambda slots
  connect(action_purchase, &QAction::triggered, this, [=](){ activateLink(GlobalAttributes::PGMODELER_PURCHASE_URL); });
  connect(action_recover, &QAction::triggered, this, [=](){ activateLink(GlobalAttributes::PGMODELER_RECOVER_URL); });
  connect(get_source_tb, &QToolButton::clicked, this, [=](){ activateLink(GlobalAttributes::PGMODELER_SRC_URL); });

	connect(hide_tb, &QToolButton::clicked, this,
					[=](){
						this->close();
						emit s_visibilityChanged(false);
					});
}

bool UpdateNotifierWidget::eventFilter(QObject *obj, QEvent *event)
{
  if(obj==frame && (event->type()==QEvent::MouseMove || event->type()==QEvent::MouseButtonPress))
  {
    QMouseEvent *m_event=dynamic_cast<QMouseEvent *>(event);

    if(event->type()==QEvent::MouseButtonPress)
      old_pos=QPoint(-1,-1);
    else
    {
      //Resizing the widget
      if(m_event->buttons()==Qt::LeftButton)
      {
        if(m_event->pos().x() >= this->minimumWidth() - 20 ||
           m_event->pos().y() >= this->minimumHeight()- 20)
        {
          int w, h;

          if(old_pos.x() >= 0)
          {
            //Calculates the width and height based upon the delta between the points
            w=this->width() + (m_event->pos().x() - old_pos.x());
            h=this->height() + (m_event->pos().y() - old_pos.y());
            this->setGeometry(this->pos().x(), this->pos().y(), w, h);
          }
        }

        old_pos=m_event->pos();
      }
    }
  }

  return(QWidget::eventFilter(obj, event));
}

void UpdateNotifierWidget::activateLink(const QString &link)
{
  QDesktopServices::openUrl(QUrl(link));
  this->close();
  emit s_visibilityChanged(false);
}

void UpdateNotifierWidget::checkForUpdate(void)
{
	QUrl url(GlobalAttributes::PGMODELER_UPD_CHECK_URL + GlobalAttributes::PGMODELER_VERSION);
	QNetworkRequest req(url);

	show_no_upd_msg=(dynamic_cast<QAction *>(sender())!=nullptr);
	update_chk_reply=update_chk_manager.get(req);
}

void UpdateNotifierWidget::handleUpdateChecked(QNetworkReply *reply)
{
  Messagebox msg_box;

  if(reply->error()!=QNetworkReply::NoError)
  {
    msg_box.show(trUtf8("Failed to check updates"),
                 trUtf8("The update notifier failed to check for new versions! Please, verify your internet connectivity and try again! Connection error returned: <strong>%1</strong>.").arg(reply->errorString()),
                 Messagebox::ERROR_ICON, Messagebox::OK_BUTTON);
  }
  else
  {
    unsigned http_status=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();

    /* In case of [301 - Move permanently] there is the need to make a new request to
       reach the final destination */
    if(http_status==301)
    {
      QString url=reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
      QNetworkRequest req(url);
      update_chk_reply=update_chk_manager.get(req);
    }
    else
    {
      //In case of [200 - OK] updates the contents with the retrieved json
      if(http_status==200)
      {
        QJsonDocument json_doc=QJsonDocument::fromJson(reply->readAll());
        QJsonObject json_obj=json_doc.object();
        QString version=json_obj.value(ParsersAttributes::NEW_VERSION).toString(),
                changelog=json_obj.value(ParsersAttributes::CHANGELOG).toString(),
                date=json_obj.value(ParsersAttributes::DATE).toString();
        bool upd_found=(!version.isEmpty() && version!=ParsersAttributes::_FALSE_);

        if(upd_found)
        {
          ver_num_lbl->setText(version);
          changelog_txt->setText(changelog);
          ver_date_lbl->setText(date);
        }
        else if(show_no_upd_msg)
        {
          msg_box.show(trUtf8("No updates found"),
                       trUtf8("You are running the most recent pgModeler version! No update needed."),
                       Messagebox::INFO_ICON, Messagebox::OK_BUTTON);
        }

        emit s_updateAvailable(upd_found);
      }
      else
      {
        msg_box.show(trUtf8("Failed to check updates"),
                     trUtf8("The update notifier failed to check for new versions! A HTTP status code was returned: <strong>%1</strong>").arg(http_status),
                     Messagebox::ERROR_ICON, Messagebox::OK_BUTTON);
      }

      delete(update_chk_reply);
      update_chk_reply=nullptr;
    }
  }
}


