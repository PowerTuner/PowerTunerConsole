/*
 * This file is part of PowerTunerConsole.
 * Copyright (C) 2025 kylon
 *
 * PowerTunerConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PowerTunerConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QApplication>

#include "TrayIcon.h"

namespace PWT::CUI {
    TrayIcon::TrayIcon() {
        QAction *showApp = new QAction("Open PowerTuner Console");

        tray = new QSystemTrayIcon(QIcon(":/ico/pwt"));
        contextMenu = new QMenu("PowerTuner Console Menu");
        closeApp = new QAction("Exit");

        contextMenu->addAction(showApp);
        contextMenu->addAction(closeApp);
        tray->setContextMenu(contextMenu);

        QObject::connect(tray, &QSystemTrayIcon::activated, this, &TrayIcon::onTrayActivated);
        QObject::connect(showApp, &QAction::triggered, this, &TrayIcon::onTrayShowApp);
        QObject::connect(closeApp, &QAction::triggered, this, &TrayIcon::quitApplication);
    }

    TrayIcon::~TrayIcon() {
        contextMenu->clear();
        delete contextMenu;
        delete tray;
    }

    void TrayIcon::onTrayActivated(const QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
            case QSystemTrayIcon::Trigger:
                onTrayShowApp();
                break;
            default:
                break;
        }
    }

    void TrayIcon::onTrayShowApp() {
        tray->hide();
        emit showMainWindow();
    }

    void TrayIcon::onTrayExitApp() {
        emit quitApplication();
    }
}
