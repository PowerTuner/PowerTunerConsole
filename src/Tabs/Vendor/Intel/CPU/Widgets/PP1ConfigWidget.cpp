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
#include "PP1ConfigWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    PP1ConfigWidget::PP1ConfigWidget() {
        const auto sliderUnitValCB = [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); };
        QVBoxLayout *lyt = new QVBoxLayout();

        lock = new ConsoleCheckbox("Lock until device reboot");
        pp1 = new ConsoleSliderUnit("Watts", sliderUnitValCB);

        pp1->setStaticLabel("Power limit");
        pp1->setPageStep(100);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(lock);
        lyt->addWidget(pp1);

        setLayout(lyt);
        setEnabled(false);
    }

    void PP1ConfigWidget::setRange(const PWTS::MinMax &range) const {
        pp1->setRange(range.min, range.max);
    }

    void PP1ConfigWidget::setData(const PWTS::DaemonPacket &packet) {
        const bool isValid = packet.intelData->pp1CurrentCfg.isValid();
        const PWTS::Intel::PP1CurrentConfig data = packet.intelData->pp1CurrentCfg.getValue();

        setEnabled(isValid && !data.lock);

        if (!isValid) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        lock->setChecked(data.lock);
        pp1->setValue(data.limit);
    }

    void PP1ConfigWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->pp1CurrentCfg.setValue({
            .limit = pp1->getValue(),
            .lock = lock->isChecked()
        }, true);
    }
}
