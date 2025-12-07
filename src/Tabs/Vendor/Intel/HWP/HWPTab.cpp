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
#include "HWPTab.h"
#include "../../../Widgets/SectionWidget.h"

namespace PWT::CUI::INTEL {
    HWPTab::HWPTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        enableHWP = new ConsoleCheckbox("HWP Enable");

        scrollWidgLyt->insertWidget(nextInsertIdx(), enableHWP);

        if (features.contains(PWTS::Feature::INTEL_HWP_CTL)) {
            hwpPkgCtlPolarity = new ConsoleCheckbox("HWP Pkg Ctl Polarity enable");
            scrollWidgLyt->insertWidget(nextInsertIdx(), hwpPkgCtlPolarity);
        }

        hwpRequestThreadWidget = new HWPRequestThreadWidget(features, packet.cpuInfo.numLogicalCpus);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Power Managment Control Hints (Thread)"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), hwpRequestThreadWidget);

        if (features.contains(PWTS::Feature::INTEL_HWP_REQ_PKG)) {
            hwpRequestPkgWidget = new HWPRequestPkgWidget(features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Power Management Control Hints (Package)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), hwpRequestPkgWidget);
        }
    }

    void HWPTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const int hwpEnable = packet.intelData->hwpEnable.getValue();
        const bool hwpEnableValid = packet.intelData->hwpEnable.isValid();

        enableHWP->setEnabled(hwpEnableValid && hwpEnable == 0);
        enableHWP->setChecked(hwpEnableValid && hwpEnable == 1);
        hwpRequestThreadWidget->setData(packet);

        if (!hwpRequestPkgWidget.isNull())
            hwpRequestPkgWidget->setData(packet);

        if (!hwpPkgCtlPolarity.isNull()) {
            hwpPkgCtlPolarity->setEnabled(packet.intelData->hwpPkgCtlPolarity.isValid());

            if (hwpPkgCtlPolarity->isEnabled())
                hwpPkgCtlPolarity->setChecked(packet.intelData->hwpPkgCtlPolarity.getValue());
        }

        setupFocusableWidgets();
    }

    void HWPTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        packet.intelData->hwpEnable.setValue(enableHWP->isChecked(), enableHWP->isEnabled());
        hwpRequestThreadWidget->setDataForPacket(packet);

        if (!hwpRequestPkgWidget.isNull())
            hwpRequestPkgWidget->setDataForPacket(packet);

        if (!hwpPkgCtlPolarity.isNull())
            packet.intelData->hwpPkgCtlPolarity.setValue(hwpPkgCtlPolarity->isChecked(), hwpPkgCtlPolarity->isEnabled());
    }
}
