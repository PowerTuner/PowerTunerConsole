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
#include "RYCPUTab.h"
#include "../../../Widgets/SectionWidget.h"
#include "pwtClientCommon/InputRanges/InputRanges.h"

namespace PWT::CUI::AMD {
    RYCPUTab::RYCPUTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::AMD_RY_FAST_LIMIT_W)) {
            fastLimitWidget = new FastLimitWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Package Power Tracking Fast Limit (PPT Fast)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), fastLimitWidget);
            QObject::connect(fastLimitWidget, &FastLimitWidget::sliderValueChanged, this, &RYCPUTab::onFastLimitChanged);
        }

        if (features.contains(PWTS::Feature::AMD_RY_SLOW_LIMIT_W)) {
            slowLimitWidget = new SlowLimitWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Package Power Tracking Slow Limit (PPT Slow)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), slowLimitWidget);
            QObject::connect(slowLimitWidget, &SlowLimitWidget::sliderValueChanged, this, &RYCPUTab::onSlowLimitChanged);
        }

        if (features.contains(PWTS::Feature::AMD_RY_STAPM_LIMIT_W)) {
            stapmLimitWidget = new StapmLimitWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Skin Temperature-Aware Power Management Limit (STAPM)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), stapmLimitWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_APU_SLOW_W)) {
            apuSlowLimitWidget = new APUSlowLimitWidget(features.contains(PWTS::Feature::AMD_RY_APU_SLOW_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Package Power Tracking APU Slow Limit (PPT APU)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), apuSlowLimitWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_TCTL_TEMP_W)) {
            tctlTempWidget = new TctlTempWidget(features.contains(PWTS::Feature::AMD_RY_TCTL_TEMP_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("T Control Temperature (TCTL)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), tctlTempWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_APU_SKIN_TEMP_W)) {
            apuSkinTempWidget = new APUSkinTempWidget(features.contains(PWTS::Feature::AMD_RY_APU_SKIN_TEMP_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Smart Temperature Tracking APU (STT APU)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), apuSkinTempWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_DGPU_SKIN_TEMP_W)) {
            dgpuSkinTempWidget = new DGPUSkinTempWidget(features.contains(PWTS::Feature::AMD_RY_DGPU_SKIN_TEMP_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Smart Temperature Tracking dGPU (STT dGPU)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), dgpuSkinTempWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_CURRENT_W)) {
            vrmCurrentWidget = new VRMCurrentWidget(features.contains(PWTS::Feature::AMD_RY_VRM_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Thermal Design Current Limit (TDC VDD)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmCurrentWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_SOC_CURRENT_W)) {
            vrmSocCurrentWidget = new VRMSocCurrentWidget(features.contains(PWTS::Feature::AMD_RY_VRM_SOC_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Thermal Design Current Limit (TDC SoC)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmSocCurrentWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_MAX_CURRENT_W)) {
            vrmMaxCurrentWidget = new VRMMaxCurrentWidget(features.contains(PWTS::Feature::AMD_RY_VRM_MAX_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Electrical Design Current Limit (EDC VDD)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmMaxCurrentWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_SOC_MAX_CURRENT_W)) {
            vrmSocMaxCurrentWidget = new VRMSocMaxCurrentWidget(features.contains(PWTS::Feature::AMD_RY_VRM_SOC_MAX_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Electrical Design Current Limit (EDC SoC)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmSocMaxCurrentWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_CO_ALL_W)) {
            curveOptimizerAllWidget = new CurveOptimizerAllWidget(false);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Curve Optimizer (All Cores)"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), curveOptimizerAllWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_CO_PER_W)) {
            curveOptimizerCoreWidget = new CurveOptimizerCoreWidget(packet.cpuInfo.numCores);
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Curve Optimizer"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), curveOptimizerCoreWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_STATIC_GFX_CLK_W)) {
            staticGfxClkWidget = new StaticGfxClkWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Force static GPU clock"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), staticGfxClkWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_MIN_GFX_CLOCK_W)) {
            minGfxClockWidget = new MinGfxClockWidget(features.contains(PWTS::Feature::AMD_RY_MIN_GFX_CLOCK_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Minimum GFX Clock"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), minGfxClockWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_MAX_GFX_CLOCK_W)) {
            maxGfxClockWidget = new MaxGfxClockWidget(features.contains(PWTS::Feature::AMD_RY_MAX_GFX_CLOCK_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Maximum GFX Clock"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), maxGfxClockWidget);
        }

        if (features.contains(PWTS::Feature::AMD_RY_POWER_PROFILE_W)) {
            powerProfileWidget = new PowerProfileWidget();
            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Power Profile"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), powerProfileWidget);
        }
    }

    void RYCPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const QSharedPointer<UI::InputRanges> inputRanges = UI::InputRanges::getInstance();

        if (!stapmLimitWidget.isNull()) {
            stapmLimitWidget->setRange(inputRanges->getRADJPl());
            stapmLimitWidget->setData(packet);
        }

        if (!slowLimitWidget.isNull()) {
            const QSignalBlocker sblock {slowLimitWidget};

            slowLimitWidget->setRange(inputRanges->getRADJPl());
            slowLimitWidget->setData(packet);

            if (packet.amdData->slowLimit.isValid())
                onSlowLimitChanged(packet.amdData->slowLimit.getValue());
        }

        if (!fastLimitWidget.isNull()) {
            const QSignalBlocker sblock {fastLimitWidget};

            fastLimitWidget->setRange(inputRanges->getRADJPl());
            fastLimitWidget->setData(packet);

            if (packet.amdData->fastLimit.isValid())
                onFastLimitChanged(packet.amdData->fastLimit.getValue());
        }

        if (!tctlTempWidget.isNull()) {
            tctlTempWidget->setRange(inputRanges->getRADJTctl());
            tctlTempWidget->setData(packet);
        }

        if (!apuSlowLimitWidget.isNull()) {
            apuSlowLimitWidget->setRange(inputRanges->getRADJAPUSlow());
            apuSlowLimitWidget->setData(packet);
        }

        if (!apuSkinTempWidget.isNull()) {
            apuSkinTempWidget->setRange(inputRanges->getRADJAPUSkinTemp());
            apuSkinTempWidget->setData(packet);
        }

        if (!dgpuSkinTempWidget.isNull()) {
            dgpuSkinTempWidget->setRange(inputRanges->getRADJDGPUSkinTemp());
            dgpuSkinTempWidget->setData(packet);
        }

        if (!vrmCurrentWidget.isNull()) {
            vrmCurrentWidget->setRange(inputRanges->getRADJVrmCurrent());
            vrmCurrentWidget->setData(packet);
        }

        if (!vrmSocCurrentWidget.isNull()) {
            vrmSocCurrentWidget->setRange(inputRanges->getRADJVrmSocCurrent());
            vrmSocCurrentWidget->setData(packet);
        }

        if (!vrmMaxCurrentWidget.isNull()) {
            vrmMaxCurrentWidget->setRange(inputRanges->getRADJVrmCurrent());
            vrmMaxCurrentWidget->setData(packet);
        }

        if (!vrmSocMaxCurrentWidget.isNull()) {
            vrmSocMaxCurrentWidget->setRange(inputRanges->getRADJVrmSocCurrent());
            vrmSocMaxCurrentWidget->setData(packet);
        }

        if (!curveOptimizerAllWidget.isNull()) {
            curveOptimizerAllWidget->setRange(inputRanges->getRADJCO());
            curveOptimizerAllWidget->setData(packet);
        }

        if (!curveOptimizerCoreWidget.isNull()) {
            curveOptimizerCoreWidget->setRange(inputRanges->getRADJCO());
            curveOptimizerCoreWidget->setData(packet);
        }

        if (!staticGfxClkWidget.isNull()) {
            staticGfxClkWidget->setRange(inputRanges->getRADJGfxClock());
            staticGfxClkWidget->setData(packet);
        }

        if (!minGfxClockWidget.isNull()) {
            minGfxClockWidget->setRange(inputRanges->getRADJGfxClock());
            minGfxClockWidget->setData(packet);
        }

        if (!maxGfxClockWidget.isNull()) {
            maxGfxClockWidget->setRange(inputRanges->getRADJGfxClock());
            maxGfxClockWidget->setData(packet);
        }

        if (!powerProfileWidget.isNull())
            powerProfileWidget->setData(packet);

        setupFocusableWidgets();
    }

    void RYCPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!stapmLimitWidget.isNull())
            stapmLimitWidget->setDataForPacket(packet);

        if (!fastLimitWidget.isNull())
            fastLimitWidget->setDataForPacket(packet);

        if (!slowLimitWidget.isNull())
            slowLimitWidget->setDataForPacket(packet);

        if (!tctlTempWidget.isNull())
            tctlTempWidget->setDataForPacket(packet);

        if (!apuSlowLimitWidget.isNull())
            apuSlowLimitWidget->setDataForPacket(packet);

        if (!apuSkinTempWidget.isNull())
            apuSkinTempWidget->setDataForPacket(packet);

        if (!dgpuSkinTempWidget.isNull())
            dgpuSkinTempWidget->setDataForPacket(packet);

        if (!vrmCurrentWidget.isNull())
            vrmCurrentWidget->setDataForPacket(packet);

        if (!vrmSocCurrentWidget.isNull())
            vrmSocCurrentWidget->setDataForPacket(packet);

        if (!vrmMaxCurrentWidget.isNull())
            vrmMaxCurrentWidget->setDataForPacket(packet);

        if (!vrmSocMaxCurrentWidget.isNull())
            vrmSocMaxCurrentWidget->setDataForPacket(packet);

        if (!curveOptimizerAllWidget.isNull())
            curveOptimizerAllWidget->setDataForPacket(packet);

        if (!curveOptimizerCoreWidget.isNull())
            curveOptimizerCoreWidget->setDataForPacket(packet);

        if (!staticGfxClkWidget.isNull())
             staticGfxClkWidget->setDataForPacket(packet);

        if (!minGfxClockWidget.isNull())
            minGfxClockWidget->setDataForPacket(packet);

        if (!maxGfxClockWidget.isNull())
            maxGfxClockWidget->setDataForPacket(packet);

        if (!powerProfileWidget.isNull())
            powerProfileWidget->setDataForPacket(packet);
    }

    void RYCPUTab::onFastLimitChanged(const int v) const {
        slowLimitWidget->setMaximum(v);
    }

    void RYCPUTab::onSlowLimitChanged(const int v) const {
        stapmLimitWidget->setMaximum(v);
    }
}
