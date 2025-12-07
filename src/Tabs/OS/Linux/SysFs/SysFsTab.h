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

#include "../../../Include/AddonTab.h"
#include "pwtShared/Include/Packets/DeviceInfoPacket.h"
#include "Widgets/CPUParkWidget.h"
#include "Widgets/CPUSMTWidget.h"
#include "Widgets/CPUScalingGovernorWidget.h"
#include "Widgets/CPUFrequencyWidget.h"
#include "Widgets/CPUIdleGovernorWidget.h"
#include "Widgets/BlockQueueSchedulerContainer.h"
#include "Widgets/MiscPowerManagementContainer.h"
#ifdef WITH_AMD
#include "Widgets/AMD/AMDPStateWidget.h"
#endif

namespace PWT::CUI::LNX {
    class SysFsTab final: public AddonTab {
        Q_OBJECT

    private:
        BlockQueueSchedulerContainer *blockQueueSchedulerContainer = nullptr;
        MiscPowerManagementContainer *miscPowerManagementContainer = nullptr;
        QPointer<CPUParkWidget> cpuParkWidget;
        QPointer<CPUSMTWidget> cpuSmtWidget;
        QPointer<CPUScalingGovernorWidget> cpuScalingGovernorWidget;
        QPointer<CPUFrequencyWidget> cpuFrequencyWidget;
        QPointer<CPUIdleGovernorWidget> cpuIdleGovernorWidget;
#ifdef WITH_AMD
        QPointer<AMDPStateWidget> amdPStateWidget;
#endif

    public:
        explicit SysFsTab(const PWTS::DeviceInfoPacket &packet);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;

    private slots:
        void onWidgetDeleted(const QLayoutItem *itm) const;
    };
}
