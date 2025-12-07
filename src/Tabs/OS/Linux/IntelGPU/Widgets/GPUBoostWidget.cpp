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
#include "GPUBoostWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::LNX::Intel {
    GPUBoostWidget::GPUBoostWidget(const int idx) {
        QVBoxLayout *lyt = new QVBoxLayout();

        index = idx;
        boost = new ConsoleSliderUnit("MHz", [](QLabel *unitV, const int v) { unitV->setNum(v); }, false);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(boost);

        setLayout(lyt);
        setEnabled(false);
    }

    void GPUBoostWidget::setData(const PWTS::DaemonPacket &packet) {
        const bool isValid = packet.linuxData->intelGpuData.contains(index) &&
                             packet.linuxData->intelGpuData[index].rpsLimits.isValid() &&
                             packet.linuxData->intelGpuData[index].boostFrequency.isValid();

        setEnabled(isValid);

        if (!isValid) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::LNX::Intel::GPURPSLimits limits = packet.linuxData->intelGpuData[index].rpsLimits.getValue();

        boost->setRange(limits.rpn, limits.rp0);
        boost->setValue(packet.linuxData->intelGpuData[index].boostFrequency.getValue());
    }

    void GPUBoostWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxData->intelGpuData[index].boostFrequency.setValue(boost->getValue(), isEnabled());
    }
}
