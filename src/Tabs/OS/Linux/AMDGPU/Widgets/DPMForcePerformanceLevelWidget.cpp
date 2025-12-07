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
#include "DPMForcePerformanceLevelWidget.h"
#include "../../../../Widgets/SectionWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::LNX::AMD {
    DPMForcePerformanceLevelWidget::DPMForcePerformanceLevelWidget(const int idx) {
        QVBoxLayout *lyt = new QVBoxLayout();

        index = idx;
        dpmPerfLevel = new ConsoleSelect("DPM performance level");
        resetPowerLevels = new ConsoleCheckbox("reset to default");
        engineClockSclkWidget = new EngineClockSCLKWidget(idx);

        dpmPerfLevel->addOptions({"auto", "low", "high", "manual", "profile_standard", "profile_min_sclk", "profile_min_mclk", "profile_peak"});

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(resetPowerLevels);
        lyt->addWidget(dpmPerfLevel);
        lyt->addWidget(new SectionWidget("Engine Clock (OD SCLK)"));
        lyt->addWidget(engineClockSclkWidget);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(dpmPerfLevel, &ConsoleSelect::textChanged, this, &DPMForcePerformanceLevelWidget::onDpmPerfLevelChanged);
    }

    void DPMForcePerformanceLevelWidget::setData(const PWTS::DaemonPacket &packet) {
        const bool isValid = packet.linuxData->amdGpuData.contains(index) &&
                             packet.linuxData->amdGpuData[index].odRanges.isValid() &&
                             packet.linuxData->amdGpuData[index].dpmForcePerfLevel.isValid();

        setEnabled(isValid);

        if (!isValid) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::LNX::AMD::GPUDPMForcePerfLevel dpmPerfLvl = packet.linuxData->amdGpuData[index].dpmForcePerfLevel.getValue();
        const QSignalBlocker sblock {dpmPerfLevel};

        resetPowerLevels->setChecked(false);
        dpmPerfLevel->setCurrentIndex(dpmPerfLevel->indexOf(dpmPerfLvl.level));
        engineClockSclkWidget->setData(packet);
        onDpmPerfLevelChanged(dpmPerfLevel->getCurrentText());
    }

    void DPMForcePerformanceLevelWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxData->amdGpuData[index].dpmForcePerfLevel.setValue({
            .level = dpmPerfLevel->getCurrentText(),
            .sclk = engineClockSclkWidget->getData(),
            .reset = resetPowerLevels->isChecked()
        }, isEnabled());
    }

    void DPMForcePerformanceLevelWidget::onDpmPerfLevelChanged(const QString &text) const {
        const bool isManual = text == "manual";

        engineClockSclkWidget->setEnabled(isManual);
        resetPowerLevels->setEnabled(isManual);
    }
}
