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
#include "AMDGPUTab.h"
#include "../../../Widgets/SectionWidget.h"

namespace PWT::CUI::LNX::AMD {
    AMDGPUTab::AMDGPUTab(const PWTS::DeviceInfoPacket &packet, const int index) {
        const QSet<PWTS::Feature> &features = packet.features.gpus.value(index).second;

        if (features.contains(PWTS::Feature::AMD_GPU_DPM_FORCE_PERF_LEVEL_SYSFS)) {
            dpmForcePerfLevelWidget = new DPMForcePerformanceLevelWidget(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Power DPM Force Performance Level"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), dpmForcePerfLevelWidget);
        }

        if (features.contains(PWTS::Feature::AMD_GPU_POWER_DPM_STATE_SYSFS)) {
            powerDpmStateWidget = new PowerDPMStateWidget(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Power DPM State"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), powerDpmStateWidget);
        }
    }

    void AMDGPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!dpmForcePerfLevelWidget.isNull())
            dpmForcePerfLevelWidget->setData(packet);

        if (!powerDpmStateWidget.isNull())
            powerDpmStateWidget->setData(packet);

        setupFocusableWidgets();
    }

    void AMDGPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!dpmForcePerfLevelWidget.isNull())
            dpmForcePerfLevelWidget->setDataForPacket(packet);

        if (!powerDpmStateWidget.isNull())
            powerDpmStateWidget->setDataForPacket(packet);
    }
}
