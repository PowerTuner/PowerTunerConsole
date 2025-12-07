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
#include "pwtShared/Include/Packets/DeviceInfoPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"
#include "Widgets/ClientOptions.h"
#include "Widgets/AddDaemonOptions.h"
#include "Widgets/DaemonOptions.h"

namespace PWT::CUI {
    class SettingsTab final: public ConsoleTab {
        Q_OBJECT

    private:
        ClientOptions *clientOptions = nullptr;
        AddDaemonOptions *addDaemonOptions = nullptr;
        DaemonOptions *daemonOptions = nullptr;

    public:
        SettingsTab();

        void init(const PWTS::DeviceInfoPacket &packet) const;
        void refreshTab(const PWTS::DaemonPacket &packet) const;
        void updateProfileList(const QList<QString> &list) const;
        void updateDaemonSettings(const QByteArray &data) const;
        void addGamepad(const QString &gamepadName, const QString &gamepadID) const;
        void removeGamepad(const QString &gamepadID) const;
        [[nodiscard]] QString getFirstGamepadID() const;
        void initAutoConnectDaemonList() const;
        void loadConsoleSettings() const;
        void setDaemonOptionsEnabled(bool enable) const;
        void clearWidget() const;

    private slots:
        void onReloadGamepadMappings();
        void onSaveClientSettings();
        void onDaemonAdded(const PWT::Daemon &daemon);
        void onReloadDaemonSettings();
        void onSaveDaemonSettings(const QByteArray &data);

    signals:
        void reloadGamepadMappings();
        void appSettingsSaved();
        void daemonAdded(const PWT::Daemon &daemon);
        void reloadDaemonSettings();
        void applyDaemonSettings(const QByteArray &data);
    };
}
