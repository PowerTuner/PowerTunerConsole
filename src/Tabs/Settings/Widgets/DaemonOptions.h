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

#include "pwtShared/DaemonSettings.h"
#include "../../Widgets/ConsoleSlider.h"
#include "../../Widgets/ConsoleCheckbox.h"
#include "../../Widgets/ConsoleSelect.h"
#include "../../Widgets/ConsoleButton.h"
#include "../../Widgets/ConsoleLineEdit.h"
#include "../../Widgets/ConsoleSpinBox.h"

namespace PWT::CUI {
    class DaemonOptions final: public QWidget {
        Q_OBJECT

    private:
        QScopedPointer<PWTS::DaemonSettings> daemonSettings;
        ConsoleSelect *daemonStartProfile = nullptr;
        ConsoleSelect *daemonBatProfile = nullptr;
        ConsoleSelect *daemonPSupplyProfile = nullptr;
        ConsoleSlider *daemonApplyInterval = nullptr;
        ConsoleLineEdit *daemonListenAdr = nullptr;
        ConsoleSpinBox *daemonTcpPort = nullptr;
        ConsoleSpinBox *daemonUdpPort = nullptr;
        ConsoleCheckbox *daemonApplyOnWake = nullptr;
        ConsoleCheckbox *daemonIgnoreBatEvt = nullptr;
        ConsoleSelect *daemonLogLevel = nullptr;
        ConsoleSlider *daemonMaxLogFiles = nullptr;
        ConsoleButton *saveDaemonSettBtn = nullptr;
        ConsoleButton *reloadDaemonSettBtn = nullptr;

    public:
        DaemonOptions();

        void updateDaemonSettings(const QByteArray &data) const;
        void updateProfileList(const QList<QString> &list) const;
        void clearWidget() const;

    private slots:
        void onDaemonApplyIntervalChanged(int v) const;
        void onReloadDaemonSettingsClicked();
        void onSaveDaemonSettingsClicked();

    signals:
        void reloadDaemonSettings();
        void applyDaemonSettings(const QByteArray &data);
    };
}
