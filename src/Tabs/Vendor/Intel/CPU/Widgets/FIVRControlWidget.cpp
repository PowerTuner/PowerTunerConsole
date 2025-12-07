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
#include "FIVRControlWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    FIVRControlWidget::FIVRControlWidget(const QSet<PWTS::Feature> &cpuFeatures) {
        const auto sliderUnitVCB = [](QLabel *unitV, const int v) { unitV->setNum(v); };
        const bool hasCPUUV = cpuFeatures.contains(PWTS::Feature::INTEL_UNDERVOLT_CPU);
        const bool hasGPUUV = cpuFeatures.contains(PWTS::Feature::INTEL_UNDERVOLT_GPU);
        const bool hasCacheUV = cpuFeatures.contains(PWTS::Feature::INTEL_UNDERVOLT_CACHE);
        const bool hasSysAgentUV = cpuFeatures.contains(PWTS::Feature::INTEL_UNDERVOLT_SYSAGENT);
        const bool hasUnsliceUV = cpuFeatures.contains(PWTS::Feature::INTEL_UNDERVOLT_UNSLICE);
        QVBoxLayout *lyt = new QVBoxLayout();

        if (hasCPUUV) {
            cpu = new ConsoleSliderUnit("mv", sliderUnitVCB);
            cpu->setStaticLabel("CPU");
            cpu->setPageStep(10);
            lyt->addWidget(cpu);
        }

        if (hasGPUUV) {
            gpu = new ConsoleSliderUnit("mv", sliderUnitVCB);
            gpu->setStaticLabel("GPU");
            gpu->setPageStep(10);
            lyt->addWidget(gpu);
        }

        if (hasCacheUV) {
            cache = new ConsoleSliderUnit("mv", sliderUnitVCB);
            cache->setStaticLabel("Cache");
            cache->setPageStep(10);
            lyt->addWidget(cache);
        }

        if (hasSysAgentUV) {
            sysAgent = new ConsoleSliderUnit("mv", sliderUnitVCB);
            sysAgent->setStaticLabel("System Agent");
            sysAgent->setPageStep(10);
            lyt->addWidget(sysAgent);
        }

        if (hasUnsliceUV) {
            unslice = new ConsoleSliderUnit("mv", sliderUnitVCB);
            unslice->setStaticLabel("Unslice");
            unslice->setPageStep(10);
            lyt->addWidget(unslice);
        }

        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
        setEnabled(false);
    }

    void FIVRControlWidget::setRanges(const PWTS::MinMax &range) const {
        if (!cpu.isNull())
            cpu->setRange(range.min, range.max);

        if (!gpu.isNull())
            gpu->setRange(range.min, range.max);

        if (!cache.isNull())
            cache->setRange(range.min, range.max);

        if (!unslice.isNull())
            unslice->setRange(range.min, range.max);

        if (!sysAgent.isNull())
            sysAgent->setRange(range.min, range.max);
    }

    void FIVRControlWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->undervoltData.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::FIVRControlUV data = packet.intelData->undervoltData.getValue();

        if (!cpu.isNull())
            cpu->setValue(data.cpu);

        if (!gpu.isNull())
            gpu->setValue(data.gpu);

        if (!cache.isNull())
            cache->setValue(data.cpuCache);

        if (!unslice.isNull())
            unslice->setValue(data.unslice);

        if (!sysAgent.isNull())
            sysAgent->setValue(data.sa);
    }

    void FIVRControlWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->undervoltData.setValue({
            .cpu = !cpu.isNull() ? cpu->getValue() : 0,
            .gpu = !gpu.isNull() ? gpu->getValue() : 0,
            .cpuCache = !cache.isNull() ? cache->getValue() : 0,
            .unslice = !unslice.isNull() ? unslice->getValue() : 0,
            .sa = !sysAgent.isNull() ? sysAgent->getValue() : 0
        }, true);
    }
}
