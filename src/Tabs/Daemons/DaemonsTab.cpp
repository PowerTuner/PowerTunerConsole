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
#include "DaemonsTab.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI {
    void DaemonsTab::loadDaemons(const QList<Daemon> &daemonList) {
        for (const Daemon &daemon: daemonList)
            _addDaemon(daemon);

        setupFocusableWidgets();
    }

    void DaemonsTab::_addDaemon(const Daemon &daemon) {
        DaemonWidget *dwidget = new DaemonWidget(daemon.name, daemon.adr, daemon.port);

        widgetList.append(dwidget);
        scrollWidgLyt->insertWidget(nextInsertIdx(), dwidget);

        QObject::connect(dwidget, &DaemonWidget::connectDaemon, this, &DaemonsTab::onDaemonConnectionRequest);
        QObject::connect(dwidget, &DaemonWidget::disconnectDaemon, this, &DaemonsTab::onDaemonDisconnectionRequest);
        QObject::connect(dwidget, &DaemonWidget::deleteDaemon, this, &DaemonsTab::onDaemonDeletionRequest);
        QObject::connect(dwidget, &DaemonWidget::sayStoppableTTS, this, &DaemonsTab::onSayStoppableTTS);
    }

    void DaemonsTab::addDaemon(const Daemon &daemon) {
        _addDaemon(daemon);
        setupFocusableWidgets();
    }

    void DaemonsTab::setActiveDaemon(const QString &adr, const quint16 port) const {
        for (DaemonWidget *dw: widgetList) {
            if (dw->getAddress() != adr && dw->getPort() != port)
                continue;

            activeDaemonWdg = dw;
            break;
        }
    }

    void DaemonsTab::setDaemonAsDisconnected() const {
        if (activeDaemonWdg == nullptr)
            return;

        activeDaemonWdg->setConnectionResult(false);
        activeDaemonWdg = nullptr;
    }

    void DaemonsTab::setDaemonAsConnected() const {
        if (activeDaemonWdg == nullptr) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid object"));
            return;
        }

        activeDaemonWdg->setConnectionResult(true);
    }

    void DaemonsTab::onDaemonConnectionRequest(const QString &adr, const quint16 port) {
        DaemonWidget *dw = qobject_cast<DaemonWidget *>(QObject::sender());

        if (dw == nullptr) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid object"));
            return;
        }

        if (activeDaemonWdg != nullptr)
            activeDaemonWdg->setConnectionResult(false);

        activeDaemonWdg = dw;
        emit daemonConnectionRequest(adr, port);
    }

    void DaemonsTab::onDaemonDisconnectionRequest() {
        emit daemonDisconnectionRequest();
    }

    void DaemonsTab::onDaemonDeletionRequest() {
        DaemonWidget *dw = qobject_cast<DaemonWidget *>(QObject::sender());

        if (dw == nullptr) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid object"));
            return;
        }

        if (dw == activeDaemonWdg) {
            activeDaemonWdg = nullptr;
            emit daemonDisconnectionRequest();
        }

        emit daemonDeleted({dw->getName(), dw->getAddress(), dw->getPort()});
        removeWidgetChildrenFromFocusList(dw);
        widgetList.removeAt(widgetList.indexOf(dw));
        deleteWidget(dw);
        dw->deleteLater();
    }

    void DaemonsTab::onSayStoppableTTS(const QString &msg) {
        emit sayStoppableTTS(msg);
    }
}
