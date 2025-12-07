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
#include <QVBoxLayout>

#include "CPUIdleGovernorWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::LNX {
    CPUIdleGovernorWidget::CPUIdleGovernorWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        idleGovernor = new ConsoleSelect("CPU Idle Governor");

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(idleGovernor);

        setLayout(lyt);
        setEnabled(false);
    }

    void CPUIdleGovernorWidget::setData(const PWTS::DaemonPacket &packet) {
        const PWTS::ROData<QList<QString>> availGovs = packet.linuxData->cpuIdleAvailableGovernors;
        const PWTS::RWData<QString> governor = packet.linuxData->cpuIdleGovernor;

        setEnabled(availGovs.isValid() && governor.isValid());
        idleGovernor->clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        idleGovernor->addOptions(availGovs.getValue());
        idleGovernor->setCurrentIndex(idleGovernor->indexOf(governor.getValue()));
    }

    void CPUIdleGovernorWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.linuxData->cpuIdleGovernor.setValue(idleGovernor->getCurrentText(), true);
    }
}
