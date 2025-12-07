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
#include "CPUPowerBalanceWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    CPUPowerBalanceWidget::CPUPowerBalanceWidget() {
        QHBoxLayout *lyt = new QHBoxLayout();

        cpu = new ConsoleSliderLevels("Low", "High", "CPU Priority");

        cpu->setRange(0, 31);
        cpu->setPageStep(5);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(cpu);

        setLayout(lyt);
        setEnabled(false);
    }

    void CPUPowerBalanceWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->pp0Priority.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        cpu->setValue(packet.intelData->pp0Priority.getValue());
    }

    void CPUPowerBalanceWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->pp0Priority.setValue(cpu->getValue(), true);
    }
}
