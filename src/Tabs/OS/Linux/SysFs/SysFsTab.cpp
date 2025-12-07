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
#include "SysFsTab.h"
#include "../../../Widgets/SectionWidget.h"

namespace PWT::CUI::LNX {
    SysFsTab::SysFsTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::CPU_PARK_SYSFS)) {
            cpuParkWidget = new CPUParkWidget(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("CPU parking"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuParkWidget);
        }

        if (features.contains(PWTS::Feature::CPU_SMT_SYSFS)) {
            cpuSmtWidget = new CPUSMTWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Simultaneous Multi-Threading / Hyper-Threading"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuSmtWidget);
        }

        if (features.contains(PWTS::Feature::CPUFREQ_SYSFS)) {
            cpuScalingGovernorWidget = new CPUScalingGovernorWidget(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("CPU scaling governor"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuScalingGovernorWidget);

            cpuFrequencyWidget = new CPUFrequencyWidget(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("CPU frequency"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuFrequencyWidget);
        }

        if (features.contains(PWTS::Feature::CPUIDLE_GOV_SYSFS)) {
            cpuIdleGovernorWidget = new CPUIdleGovernorWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("CPU idle governor"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuIdleGovernorWidget);
        }

#ifdef WITH_AMD
        if (features.contains(PWTS::Feature::AMD_PSTATE_SYSFS)) {
            amdPStateWidget = new AMDPStateWidget(packet.cpuInfo.numLogicalCpus);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("AMD PState scaling driver"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), amdPStateWidget);
        }
#endif

        blockQueueSchedulerContainer = new BlockQueueSchedulerContainer();
        scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Block device queue scheduler", SectionWidget::Size::Big));
        scrollWidgLyt->insertWidget(nextInsertIdx(), blockQueueSchedulerContainer);
        QObject::connect(blockQueueSchedulerContainer, &BlockQueueSchedulerContainer::deleteDevice, this, &SysFsTab::onWidgetDeleted);

        miscPowerManagementContainer = new MiscPowerManagementContainer();
        scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Misc power management", SectionWidget::Size::Big));
        scrollWidgLyt->insertWidget(nextInsertIdx(), miscPowerManagementContainer);
        QObject::connect(miscPowerManagementContainer, &MiscPowerManagementContainer::deleteDevice, this, &SysFsTab::onWidgetDeleted);
    }

    void SysFsTab::refreshTab(const PWTS::DaemonPacket &packet) {
        if (!cpuParkWidget.isNull())
            cpuParkWidget->setData(packet);

        if (!cpuSmtWidget.isNull())
            cpuSmtWidget->setData(packet);

        if (!cpuScalingGovernorWidget.isNull())
            cpuScalingGovernorWidget->setData(packet);

        if (!cpuFrequencyWidget.isNull())
            cpuFrequencyWidget->setData(packet);

        if (!cpuIdleGovernorWidget.isNull())
            cpuIdleGovernorWidget->setData(packet);

#ifdef WITH_AMD
        if (!amdPStateWidget.isNull())
            amdPStateWidget->setData(packet);
#endif

        blockQueueSchedulerContainer->setData(packet);
        miscPowerManagementContainer->setData(packet);

        setupFocusableWidgets();
    }

    void SysFsTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!cpuParkWidget.isNull())
            cpuParkWidget->setDataForPacket(packet);

        if (!cpuSmtWidget.isNull())
            cpuSmtWidget->setDataForPacket(packet);

        if (!cpuScalingGovernorWidget.isNull())
            cpuScalingGovernorWidget->setDataForPacket(packet);

        if (!cpuFrequencyWidget.isNull())
            cpuFrequencyWidget->setDataForPacket(packet);

        if (!cpuIdleGovernorWidget.isNull())
            cpuIdleGovernorWidget->setDataForPacket(packet);

#ifdef WITH_AMD
        if (!amdPStateWidget.isNull())
            amdPStateWidget->setDataForPacket(packet);
#endif

        blockQueueSchedulerContainer->setDataForPacket(packet);
        miscPowerManagementContainer->setDataForPacket(packet);
    }

    void SysFsTab::onWidgetDeleted(const QLayoutItem *itm) const {
        removeWidgetChildrenFromFocusList(itm->widget());
        itm->widget()->deleteLater();
        delete itm;
    }
}
