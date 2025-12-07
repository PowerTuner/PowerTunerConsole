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
#include "CPUTab.h"
#include "../../../Widgets/SectionWidget.h"

namespace PWT::CUI::AMD {
    CPUTab::CPUTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::AMD_HWPSTATE)) {
            pstateWidget = new PStateWidget(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("P-State Control"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), pstateWidget);
        }

        if (features.contains(PWTS::Feature::AMD_CORE_PERFORMANCE_BOOST)) {
            corePerformanceBoostWidget = new CorePerformanceBoostWidget(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Core Performance Boost"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), corePerformanceBoostWidget);
        }

        if (features.contains(PWTS::Feature::AMD_CPPC)) {
            cppcGBox = new CPPCWidget(features, packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("CPPC Request"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), cppcGBox);
        }
    }

    void CPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!pstateWidget.isNull())
            pstateWidget->setData(packet);

        if (!corePerformanceBoostWidget.isNull())
            corePerformanceBoostWidget->setData(packet);

        if (!cppcGBox.isNull())
            cppcGBox->setData(packet);

        setupFocusableWidgets();
    }

    void CPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!pstateWidget.isNull())
            pstateWidget->setDataForPacket(packet);

        if (!corePerformanceBoostWidget.isNull())
            corePerformanceBoostWidget->setDataForPacket(packet);

        if (!cppcGBox.isNull())
            cppcGBox->setDataForPacket(packet);
    }
}
