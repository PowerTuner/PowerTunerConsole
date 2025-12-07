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

#include "../../../../Widgets/ConsoleSelect.h"
#include "../../../../Widgets/ConsoleSliderUnit.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::CUI::LNX {
    class CPUParkWidget final: public QWidget {
        Q_OBJECT

    private:
        struct WData final {
            bool valid;
            int status;
        };

        ConsoleSliderUnit *cpuParkSlider = nullptr;
        ConsoleSelect *cpuSelect = nullptr;
        ConsoleSelect *cpuMode = nullptr;
        int onlineCount = 0;
        QList<WData> data;

        [[nodiscard]] int getOnlinePercent() const { return qFloor(static_cast<float>(onlineCount) / static_cast<float>(data.size()) * 100); }

    public:
        explicit CPUParkWidget(int threadCount);

        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet) const;

    private slots:
        void onCPUSelectChanged(int idx) const;
        void onCPUModeChanged(int idx);
        void onParkSliderChanged(int v);
    };
}
