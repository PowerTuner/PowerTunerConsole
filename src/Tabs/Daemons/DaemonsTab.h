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
#pragma once

#include "../ConsoleTab.h"
#include "../../Include/Daemon.h"
#include "Widgets/DaemonWidget.h"

namespace PWT::CUI {
    class DaemonsTab final: public ConsoleTab {
        Q_OBJECT

    private:
        QList<DaemonWidget *> widgetList;
        mutable DaemonWidget *activeDaemonWdg = nullptr;

        void _addDaemon(const Daemon &daemon);

    public:
        void addDaemon(const Daemon &daemon);
        void loadDaemons(const QList<Daemon> &daemonList);
        void setActiveDaemon(const QString &adr, quint16 port) const;
        void setDaemonAsDisconnected() const;
        void setDaemonAsConnected() const;

    private slots:
        void onDaemonConnectionRequest(const QString &adr, quint16 port);
        void onDaemonDisconnectionRequest();
        void onDaemonDeletionRequest();
        void onSayStoppableTTS(const QString &msg);

    signals:
        void daemonConnectionRequest(const QString &adr, quint16 port);
        void daemonDisconnectionRequest();
        void daemonDeleted(const PWT::Daemon &data);
    };
}
