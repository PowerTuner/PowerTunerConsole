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
#include "VRCurrentConfigWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    VRCurrentConfigWidget::VRCurrentConfigWidget() {
        const auto sliderUnitValCB = [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); };
        QVBoxLayout *lyt = new QVBoxLayout();

        lock = new ConsoleCheckbox("Lock until device reboot");
        pl4 = new ConsoleSliderUnit("Watts", sliderUnitValCB);

        pl4->setStaticLabel("Power limit 4");
        pl4->setPageStep(100);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(lock);
        lyt->addWidget(pl4);

        setLayout(lyt);
        setEnabled(false);
    }

    void VRCurrentConfigWidget::setRange(const PWTS::MinMax &range) const {
        pl4->setRange(range.min, range.max);
    }

    void VRCurrentConfigWidget::setData(const PWTS::DaemonPacket &packet) {
        const PWTS::Intel::VRCurrentConfig data = packet.intelData->vrCurrentCfg.getValue();
        const bool isValid = packet.intelData->vrCurrentCfg.isValid();

        setEnabled(isValid && !data.lock);

        if (!isValid) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        lock->setChecked(data.lock);
        pl4->setValue(data.pl4);
    }

    void VRCurrentConfigWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->vrCurrentCfg.setValue({
            .pl4 = pl4->getValue(),
            .lock = lock->isChecked()
        }, true);
    }
}
