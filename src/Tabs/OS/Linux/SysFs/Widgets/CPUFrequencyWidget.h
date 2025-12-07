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
#include "../../../../Widgets/ConsoleSelect.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::CUI::LNX {
    class CPUFrequencyWidget final: public QWidget {
        Q_OBJECT

    private:
        struct WData final {
            bool valid;
            PWTS::LNX::CPUFrequencyLimits limits;
            PWTS::MinMax freq;
        };

        ConsoleSliderUnit *minAll = nullptr;
        ConsoleSliderUnit *maxAll = nullptr;
        ConsoleSelect *cpuSelect = nullptr;
        ConsoleSliderUnit *minFreq = nullptr;
        ConsoleSliderUnit *maxFreq = nullptr;
        QList<WData> data;

        void resetAllSliders() const;
        void setRelatedCPUs(bool minimum, int freq);

    public:
        explicit CPUFrequencyWidget(int threadCount);

        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet) const;

    private slots:
        void onMinAllChanged(int v);
        void onMaxAllChanged(int v);
        void onMinFreqChanged(int v);
        void onMaxFreqChanged(int v);
        void onCPUSelectChanged(int idx);
    };
}
