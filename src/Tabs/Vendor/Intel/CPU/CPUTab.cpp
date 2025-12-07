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
#include "pwtClientCommon/InputRanges/InputRanges.h"
#include "Widgets/PowerCtl/PowerCtlNHLMWidget.h"
#include "Widgets/PowerCtl/PowerCtlSBWidget.h"
#include "Widgets/PowerCtl/PowerCtlCU1Widget.h"
#include "Widgets/MiscPowerManagement/MiscPowerManagementNHLMWidget.h"

namespace PWT::CUI::INTEL {
    CPUTab::CPUTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::INTEL_PKG_POWER_LIMIT)) {
            powerLimitWidget = new PowerLimitWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Power limits"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), powerLimitWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_VR_CURRENT_CFG)) {
            vrCurrentConfigWidget = new VRCurrentConfigWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("VR Current Config"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrCurrentConfigWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_PP1_CURRENT_CFG)) {
            pp1ConfigWidget = new PP1ConfigWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Graphics Current Config (PP1)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), pp1ConfigWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_TURBO_POWER_CURRENT_LIMIT_RW)) {
            turboPowerCurrentWidget = new TurboPowerCurrentWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Turbo Power Current Limit"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), turboPowerCurrentWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_CPU_POWER_BALANCE)) {
            cpuPowerBalanceWidget = new CPUPowerBalanceWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("CPU Power Balance"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), cpuPowerBalanceWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_GPU_POWER_BALANCE)) {
            gpuPowerBalanceWidget = new GPUPowerBalanceWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("GPU Power Balance"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), gpuPowerBalanceWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_ENERGY_PERF_BIAS) && !features.contains(PWTS::Feature::INTEL_HWP_EPP)) {
            energyPerformanceBiasWidget = new EnergyPerformanceBiasWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Performance Energy Bias Hint"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), energyPerformanceBiasWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_TURBO_RATIO_LIMIT_RW)) {
            turboRatioLimitWidget = new TurboRatioLimitWidget(packet.cpuInfo.numCores);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Turbo Ratio Limit"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), turboRatioLimitWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_IA32_MISC_ENABLE_GROUP)) {
            miscProcessorFeaturesWidget = new MiscProcessorFeaturesWidget(features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Misc. Processor Features"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), miscProcessorFeaturesWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_MISC_PWR_MGMT)) {
            if (features.contains(PWTS::Feature::INTEL_MISC_PWR_MGMT_NHLM))
                miscPowerManagementWidget = new MiscPowerManagementNHLMWidget();

            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Misc. Power Management"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), miscPowerManagementWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_POWER_CTL)) {
            if (features.contains(PWTS::Feature::INTEL_POWER_CTL_NHLM))
                powerCtlWidget = new PowerCtlNHLMWidget();
            else if (features.contains(PWTS::Feature::INTEL_POWER_CTL_SB))
                powerCtlWidget = new PowerCtlSBWidget();
            else if (features.contains(PWTS::Feature::INTEL_POWER_CTL_CU1))
                powerCtlWidget = new PowerCtlCU1Widget();

            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Power Control"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), powerCtlWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_UNDERVOLT_GROUP)) {
            fivrControlWidget = new FIVRControlWidget(features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Undervolt"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), fivrControlWidget);
        }

        if (features.contains(PWTS::Feature::INTEL_PKG_CST_CONFIG_CONTROL)) {
            pkgCstConfigControlWidget = new PKGCSTConfigControlWidget(packet.cpuInfo.numCores, features);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("C-State Configuration Control"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), pkgCstConfigControlWidget);
        }
    }

    void CPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const QSharedPointer<UI::InputRanges> inputRanges = UI::InputRanges::getInstance();

        if (!powerLimitWidget.isNull()) {
            powerLimitWidget->setRanges(inputRanges->getIntelPl());
            powerLimitWidget->setData(packet);
        }

        if (!vrCurrentConfigWidget.isNull()) {
            vrCurrentConfigWidget->setRange(inputRanges->getIntelPl4());
            vrCurrentConfigWidget->setData(packet);
        }

        if (!pp1ConfigWidget.isNull()) {
            pp1ConfigWidget->setRange(inputRanges->getIntelPP1());
            pp1ConfigWidget->setData(packet);
        }

        if (!turboPowerCurrentWidget.isNull()) {
            turboPowerCurrentWidget->setRanges(inputRanges->getIntelTurboPwrCurrentTDP(), inputRanges->getIntelTurboPwrCurrentTDC());
            turboPowerCurrentWidget->setData(packet);
        }

        if (!cpuPowerBalanceWidget.isNull())
            cpuPowerBalanceWidget->setData(packet);

        if (!gpuPowerBalanceWidget.isNull())
            gpuPowerBalanceWidget->setData(packet);

        if (!energyPerformanceBiasWidget.isNull())
            energyPerformanceBiasWidget->setData(packet);

        if (!turboRatioLimitWidget.isNull())
            turboRatioLimitWidget->setData(packet);

        if (!miscProcessorFeaturesWidget.isNull())
            miscProcessorFeaturesWidget->setData(packet);

        if (!miscPowerManagementWidget.isNull())
            miscPowerManagementWidget->setData(packet);

        if (!powerCtlWidget.isNull())
            powerCtlWidget->setData(packet);

        if (!fivrControlWidget.isNull()) {
            fivrControlWidget->setRanges(inputRanges->getIntelFIVR());
            fivrControlWidget->setData(packet);
        }

        if (!pkgCstConfigControlWidget.isNull())
            pkgCstConfigControlWidget->setData(packet);

        setupFocusableWidgets();
    }

    void CPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!powerLimitWidget.isNull())
            powerLimitWidget->setDataForPacket(packet);

        if (!vrCurrentConfigWidget.isNull())
            vrCurrentConfigWidget->setDataForPacket(packet);

        if (!pp1ConfigWidget.isNull())
            pp1ConfigWidget->setDataForPacket(packet);

        if (!turboPowerCurrentWidget.isNull())
            turboPowerCurrentWidget->setDataForPacket(packet);

        if (!cpuPowerBalanceWidget.isNull())
            cpuPowerBalanceWidget->setDataForPacket(packet);

        if (!gpuPowerBalanceWidget.isNull())
            gpuPowerBalanceWidget->setDataForPacket(packet);

        if (!energyPerformanceBiasWidget.isNull())
            energyPerformanceBiasWidget->setDataForPacket(packet);

        if (!turboRatioLimitWidget.isNull())
            turboRatioLimitWidget->setDataForPacket(packet);

        if (!miscProcessorFeaturesWidget.isNull())
            miscProcessorFeaturesWidget->setDataForPacket(packet);

        if (!miscPowerManagementWidget.isNull())
            miscPowerManagementWidget->setDataForPacket(packet);

        if (!powerCtlWidget.isNull())
            powerCtlWidget->setDataForPacket(packet);

        if (!fivrControlWidget.isNull())
            fivrControlWidget->setDataForPacket(packet);

        if (!pkgCstConfigControlWidget.isNull())
            pkgCstConfigControlWidget->setDataForPacket(packet);
    }
}
