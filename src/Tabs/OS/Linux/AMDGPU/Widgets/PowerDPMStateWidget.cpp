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

#include "PowerDPMStateWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::LNX::AMD {
    PowerDPMStateWidget::PowerDPMStateWidget(const int idx) {
        QVBoxLayout *lyt = new QVBoxLayout();

        index = idx;
        stateCombo = new ConsoleSelect("DPM state");

        stateCombo->addOptions({"battery", "balanced", "performance"});

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(stateCombo);

        setLayout(lyt);
        setEnabled(false);
    }

    void PowerDPMStateWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.linuxData->amdGpuData.contains(index) && packet.linuxData->amdGpuData[index].powerDpmState.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        stateCombo->setCurrentIndex(stateCombo->indexOf(packet.linuxData->amdGpuData[index].powerDpmState.getValue()));
    }

    void PowerDPMStateWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxData->amdGpuData[index].powerDpmState.setValue(stateCombo->getCurrentText(), isEnabled());
    }
}
