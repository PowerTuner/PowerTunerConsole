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
#include "IntelGPUTab.h"
#include "../../../Widgets/SectionWidget.h"

namespace PWT::CUI::LNX::Intel {
    IntelGPUTab::IntelGPUTab(const PWTS::DeviceInfoPacket &packet, const int index) {
        const QSet<PWTS::Feature> &features = packet.features.gpus.value(index).second;

        if (features.contains(PWTS::Feature::INTEL_GPU_RPS_FREQ_SYSFS)) {
            gpuFrequencyWidget = new GPUFrequencyWidget(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Frequency"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), gpuFrequencyWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_GPU_BOOST_SYSFS)) {
            gpuBoostWidget = new GPUBoostWidget(index);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Boost Frequency"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), gpuBoostWidget);
        }
    }

    void IntelGPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!gpuFrequencyWidget.isNull())
            gpuFrequencyWidget->setData(packet);

        if (!gpuBoostWidget.isNull())
            gpuBoostWidget->setData(packet);

        setupFocusableWidgets();
    }

    void IntelGPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!gpuFrequencyWidget.isNull())
            gpuFrequencyWidget->setDataForPacket(packet);

        if (!gpuBoostWidget.isNull())
            gpuBoostWidget->setDataForPacket(packet);
    }
}
