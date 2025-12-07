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
#include "HomeTab.h"

namespace PWT::CUI {
    HomeTab::HomeTab() {
        sysInfo = new SystemInfoWidget();
        cpuInfo = new CPUInfoWidget();
        powerTuner = new PowerTunerWidget();

        scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("PowerTuner"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), powerTuner);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("System"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), sysInfo);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("CPU"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), cpuInfo);
        setupFocusableWidgets();
    }

    void HomeTab::init(const PWTS::DeviceInfoPacket &packet) {
        powerTuner->setDaemonVersion(packet.daemonMajorVersion, packet.daemonMinorVersion);
        powerTuner->setDaemonPwtsVersion(packet.daemonPwtsMajorVersion, packet.daemonPwtsMinorVersion);
        sysInfo->init(packet.cpuInfo, packet.sysInfo);
        cpuInfo->init(packet.cpuInfo);

        if (!packet.gpusInfo.isEmpty()) {
            if (gpuInfo.isNull()) {
                gpuInfo = new GPUInfoWidget();
                gpuSectionTitle = new SectionWidget("GPU");
                scrollWidgLyt->insertWidget(nextInsertIdx(), gpuSectionTitle);
                scrollWidgLyt->insertWidget(nextInsertIdx(), gpuInfo);
            }

            gpuInfo->setData(packet.gpusInfo);
        }
    }

    void HomeTab::refreshTab(const PWTS::DaemonPacket &packet) const {
        cpuInfo->setData(packet);
        sysInfo->setData(packet);
        setupFocusableWidgets();
    }

    void HomeTab::clearWidget() {
        if (!gpuInfo.isNull()) {
            deleteWidget(gpuSectionTitle);
            deleteWidget(gpuInfo);
        }

        powerTuner->clearWidget();
        cpuInfo->clearWidget();
        sysInfo->clearWidget();
        setupFocusableWidgets();
    }
}
