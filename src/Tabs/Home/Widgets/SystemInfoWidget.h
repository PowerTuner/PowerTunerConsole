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
#include "pwtShared/Include/SystemInfo.h"
#include "pwtShared/Include/CPU/CpuInfo.h"
#include "../../Widgets/ConsoleLabel.h"

namespace PWT::CUI {
    class SystemInfoWidget final: public QWidget {
    private:
        static constexpr char defaultStr[] = "Unknown";
        PWTS::OSType osType = PWTS::OSType::Unknown;
        int logicalCPUCount = 0;
        QVBoxLayout *lyt = nullptr;
        ConsoleLabel *product = nullptr;
        ConsoleLabel *distribution = nullptr;
        ConsoleLabel *kernel = nullptr;
        ConsoleLabel *ram = nullptr;
        ConsoleLabel *arch = nullptr;
        ConsoleLabel *profile = nullptr;
        QHash<QString, ConsoleLabel *> addonInfoList;

        void setDefaultLabels() const;
        void clear() const;
        void setLabels() const;

    public:
        SystemInfoWidget();

        [[nodiscard]] QString getActiveProfile() const { return profile->getText(); }
        void setAppliedProfile(const QString &name) const { profile->setText(name.isEmpty() ? "-" : name); }
        void unsetAppliedProfile() const { profile->setText("-"); }

        void init(const PWTS::CpuInfo &cpuInfo, const PWTS::SystemInfo &sysInfo);
        void setData(const PWTS::DaemonPacket &packet);
        void clearWidget();
    };
}
