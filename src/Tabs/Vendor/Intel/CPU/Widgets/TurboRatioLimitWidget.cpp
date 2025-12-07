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
#include "TurboRatioLimitWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    TurboRatioLimitWidget::TurboRatioLimitWidget(const int coreCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        for (int i=0; i<coreCount && i<8; ++i) {
            ConsoleSlider *slider = new ConsoleSlider(QString("Maximum ratio limit for %1 cores").arg(i + 1));

            slider->setRange(1, 255);
            slider->setPageStep(10);

            lyt->addWidget(slider);
            turboLimitList.append(slider);
        }

        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
        setEnabled(false);
    }

    void TurboRatioLimitWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->turboRatioLimit.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::TurboRatioLimit data = packet.intelData->turboRatioLimit.getValue();
        const QList<int> ratioLimits {
            data.maxRatioLimit1C,
            data.maxRatioLimit2C,
            data.maxRatioLimit3C,
            data.maxRatioLimit4C,
            data.maxRatioLimit5C,
            data.maxRatioLimit6C,
            data.maxRatioLimit7C,
            data.maxRatioLimit8C
        };

        for (int i=0,l=turboLimitList.size(); i<l; ++i)
            turboLimitList[i]->setValue(ratioLimits[i]);
    }

    void TurboRatioLimitWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        QList<int> ratioLimits(8, 0);

        for (int i=0,l=turboLimitList.size(); i<l; ++i)
            ratioLimits[i] = turboLimitList[i]->getValue();

        packet.intelData->turboRatioLimit.setValue({
            .maxRatioLimit1C = ratioLimits[0],
            .maxRatioLimit2C = ratioLimits[1],
            .maxRatioLimit3C = ratioLimits[2],
            .maxRatioLimit4C = ratioLimits[3],
            .maxRatioLimit5C = ratioLimits[4],
            .maxRatioLimit6C = ratioLimits[5],
            .maxRatioLimit7C = ratioLimits[6],
            .maxRatioLimit8C = ratioLimits[7]
        }, true);
    }
}
