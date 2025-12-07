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

#include "PowerProfileWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::AMD {
    PowerProfileWidget::PowerProfileWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        powerProfile = new ConsoleSelect("Power profile", "Hidden option to improve power efficiency. Behavior depends on CPU, device and manufacturer");

        powerProfile->addOptions({"power saving", "max performance", "don't set"});

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(powerProfile);

        setLayout(lyt);
        setEnabled(false);
    }

    void PowerProfileWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.amdData->powerProfile.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const int val = packet.amdData->powerProfile.getValue();

        if (val == 0)
            powerProfile->setCurrentIndex(0);
        else if (val == 1)
            powerProfile->setCurrentIndex(1);
        else
            powerProfile->setCurrentIndex(2);
    }

    void PowerProfileWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        const int idx = powerProfile->getCurrentIndex();

        if (idx == 0)
            packet.amdData->powerProfile.setValue(0, true);
        else if (idx == 1)
            packet.amdData->powerProfile.setValue(1, true);
        else
            packet.amdData->powerProfile.setValue(-1, true);
    }
}
