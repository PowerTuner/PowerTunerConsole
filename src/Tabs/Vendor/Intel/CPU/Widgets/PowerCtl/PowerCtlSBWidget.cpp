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

#include "PowerCtlSBWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    PowerCtlSBWidget::PowerCtlSBWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        prochot = new ConsoleCheckbox("Bi-Directional PROCHOT");
        c1e = new ConsoleCheckbox("C1 Enhanced (C1E)");
        disableEfficiencyOptimization = new ConsoleCheckbox("Disable energy efficiency optimization");
        disableRaceToHalt = new ConsoleCheckbox("Disable race to halt optimization");

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(prochot);
        lyt->addWidget(c1e);
        lyt->addWidget(disableEfficiencyOptimization);
        lyt->addWidget(disableRaceToHalt);

        setLayout(lyt);
        setEnabled(false);
    }

    void PowerCtlSBWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->powerCtl.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::PowerCtl data = packet.intelData->powerCtl.getValue();

        prochot->setChecked(data.bdProcHot);
        c1e->setChecked(data.c1eEnable);
        disableEfficiencyOptimization->setChecked(data.disableEnergyEfficiencyOpt);
        disableRaceToHalt->setChecked(data.disableRaceToHaltOpt);
    }

    void PowerCtlSBWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->powerCtl.setValue({
            .bdProcHot = prochot->isChecked(),
            .c1eEnable = c1e->isChecked(),
            .disableEnergyEfficiencyOpt = disableEfficiencyOptimization->isChecked(),
            .disableRaceToHaltOpt = disableRaceToHalt->isChecked()
        }, true);
    }
}
