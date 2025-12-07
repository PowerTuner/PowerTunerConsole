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

#include "../../../../Widgets/ConsoleSliderUnit.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::CUI::LNX::AMD {
    class EngineClockSCLKWidget final: public QWidget {
    private:
        ConsoleSliderUnit *min = nullptr;
        ConsoleSliderUnit *max = nullptr;
        int index;

    public:
        explicit EngineClockSCLKWidget(int idx);

        void setData(const PWTS::DaemonPacket &packet) const;
        [[nodiscard]] PWTS::MinMax getData() const;
    };
}
