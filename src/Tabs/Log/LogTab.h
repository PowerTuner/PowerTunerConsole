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
#include "Widgets/ConsoleLogText.h"
#include "../Widgets/ConsoleButton.h"

namespace PWT::CUI {
    class LogTab final: public ConsoleTab {
    private:
        ConsoleLogText *logs = nullptr;
        ConsoleButton *clearBtn = nullptr;

    public:
        LogTab();

        void writeLog(const QString &msg) const;
    };
}
