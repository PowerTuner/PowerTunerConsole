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

#include <QVBoxLayout>

#include "pwtShared/Include/Packets/DaemonPacket.h"
#include "pwtShared/Include/CPU/CpuInfo.h"
#include "../../Widgets/ConsoleLabel.h"

namespace PWT::CUI {
    class CPUInfoWidget final: public QWidget {
    private:
        static constexpr char defaultStr[] = "Unknown";
        QVBoxLayout *lyt = nullptr;
        ConsoleLabel *cpu = nullptr;
        ConsoleLabel *codename = nullptr;
        ConsoleLabel *techNode = nullptr;
        ConsoleLabel *vendor = nullptr;
        ConsoleLabel *family = nullptr;
        ConsoleLabel *extFamily = nullptr;
        ConsoleLabel *model = nullptr;
        ConsoleLabel *extModel = nullptr;
        ConsoleLabel *coreCount = nullptr;
        ConsoleLabel *threadCount = nullptr;
        ConsoleLabel *stepping = nullptr;
        ConsoleLabel *l1Data = nullptr;
        ConsoleLabel *l1ICache = nullptr;
        ConsoleLabel *l2 = nullptr;
        ConsoleLabel *l3 = nullptr;
        QHash<QString, ConsoleLabel *> addonInfoList;

    public:
        CPUInfoWidget();

        void init(const PWTS::CpuInfo &cpuInfo);
        void setData(const PWTS::DaemonPacket &packet) const;
        void clearWidget();
    };
}
