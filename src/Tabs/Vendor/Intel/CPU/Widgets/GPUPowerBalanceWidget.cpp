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
#include "GPUPowerBalanceWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    GPUPowerBalanceWidget::GPUPowerBalanceWidget() {
        QHBoxLayout *lyt = new QHBoxLayout();

        gpu = new ConsoleSliderLevels("Low", "High", "GPU Priority");

        gpu->setRange(0, 31);
        gpu->setPageStep(5);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(gpu);

        setLayout(lyt);
        setEnabled(false);
    }

    void GPUPowerBalanceWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->pp1Priority.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        gpu->setValue(packet.intelData->pp1Priority.getValue());
    }

    void GPUPowerBalanceWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->pp1Priority.setValue(gpu->getValue(), true);
    }
}
