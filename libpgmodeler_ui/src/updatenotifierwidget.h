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

/**
\ingroup libpgmodeler_ui
\class UpdateNotifierWidget
\brief Implements a basic update notifier shown as a popup widget on main window.

 ATTENTION: This widget make requests to the url defined in GlobalAttributes::PGMODELER_UPD_CHECK_URL
 and expects that a JSON data is returned. In order to work correctly the returned JSON must be as follow:

 1) When there is a new version:

  { "new-version":"[version]",
    "date":"[release_date]",
    "changelog":"[change_list]" }

 2) When there is no new version:

   {"new-version":"false"}

 It's recommended to produce this JSON based upon the CHANGELOG.md file since all needed information is there.
*/

#ifndef UPDATE_NOTIFIER_WIDGET_H
#define UPDATE_NOTIFIER_WIDGET_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "ui_updatenotifierwidget.h"
#include "messagebox.h"
#include "globalattributes.h"
#include "parsersattributes.h"

class LIBPGMODELER_UI_EXPORT UpdateNotifierWidget: public QWidget, public Ui::UpdateNotifierWidget {
	private:
		Q_OBJECT

  //! brief Object responsible to make request to server
  QNetworkAccessManager update_chk_manager;

  //! brief This pointer stores the reply after the update manager make a call to server
  QNetworkReply *update_chk_reply;

  //! brief Stores the old mouse position to simulate the widget resizing
  QPoint old_pos;

  /*! brief This attribute indicates wether a message informing that no updates are available
      must be shown. Generally, this attribute is true when the user call the checkForUpdate()
      from a QAction */
  bool show_no_upd_msg;

  QMenu *get_binary_menu;

  QAction *action_purchase, *action_recover;

  //! brief This event filter is used to resize the widget when the mouse is over the notifier
  bool eventFilter(QObject *obj, QEvent *event);

  void activateLink(const QString &link);

	public:
    UpdateNotifierWidget(QWidget * parent = 0);

	public slots:
    void checkForUpdate(void);

	private slots:
    void handleUpdateChecked(QNetworkReply *reply);

  signals:
    void s_updateAvailable(bool value);
    void s_visibilityChanged(bool visible);
};

#endif
