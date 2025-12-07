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
#include "EnergyPerformanceBiasWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    EnergyPerformanceBiasWidget::EnergyPerformanceBiasWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        energyBias = new ConsoleSliderLevels("Performance", "Power saving", "Power Policy Preference");

        energyBias->setRange(0, 15);
        energyBias->setPageStep(5);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(energyBias);

        setLayout(lyt);
        setEnabled(false);
    }

    void EnergyPerformanceBiasWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->energyPerfBias.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        energyBias->setValue(packet.intelData->energyPerfBias.getValue());
    }

    void EnergyPerformanceBiasWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->energyPerfBias.setValue(energyBias->getValue(), true);
    }
}
