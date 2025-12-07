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

#include "../../../Classes/ConsoleSettings.h"
#include "../../Widgets/ConsoleButton.h"
#include "../../Widgets/ConsoleLineEdit.h"
#include "../../Widgets/ConsoleSpinBox.h"

namespace PWT::CUI {
    class AddDaemonOptions final: public QWidget {
        Q_OBJECT

    private:
        QSharedPointer<ConsoleSettings> consoleSettings;
        ConsoleLineEdit *addDaemonName = nullptr;
        ConsoleLineEdit *addDaemonAdr = nullptr;
        ConsoleSpinBox *addDaemonPort = nullptr;
        ConsoleButton *addDaemonBtn = nullptr;

    public:
        AddDaemonOptions();

    private slots:
        void onAddDaemonClicked();

    signals:
        void daemonAdded(const PWT::Daemon &daemon);
    };
}
