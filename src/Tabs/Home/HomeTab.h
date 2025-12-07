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

#include <QPointer>

#include "../ConsoleTab.h"
#include "pwtShared/Include/Packets/DeviceInfoPacket.h"
#include "Widgets/SystemInfoWidget.h"
#include "Widgets/CPUInfoWidget.h"
#include "Widgets/GPUInfoWidget.h"
#include "Widgets/PowerTunerWidget.h"
#include "../Widgets/SectionWidget.h"

namespace PWT::CUI {
    class HomeTab final: public ConsoleTab {
    private:
        SystemInfoWidget *sysInfo = nullptr;
        CPUInfoWidget *cpuInfo = nullptr;
        PowerTunerWidget *powerTuner = nullptr;
        QPointer<SectionWidget> gpuSectionTitle;
        QPointer<GPUInfoWidget> gpuInfo;

    public:
        HomeTab();

        [[nodiscard]] QString getActiveProfile() const { return sysInfo->getActiveProfile(); }
        void setAppliedProfile(const QString &name) const { sysInfo->setAppliedProfile(name); }
        void unsetAppliedProfile() const { sysInfo->unsetAppliedProfile(); }

        void init(const PWTS::DeviceInfoPacket &packet);
        void refreshTab(const PWTS::DaemonPacket &packet) const;
        void clearWidget();
    };
}
