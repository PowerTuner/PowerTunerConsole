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
#include "FanCurveWidget.h"
#include "../../Widgets/SectionWidget.h"

namespace PWT::CUI::FANS {
    FanCurveWidget::FanCurveWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        temperature = new ConsoleSlider("Temperature (°C)");
        speedPerc = new ConsoleSlider("Speed (%)");
        removeBtn = new ConsoleButton("remove");

        temperature->setRange(0, 100);
        speedPerc->setRange(0, 100);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new SectionWidget("Trip Point"));
        lyt->addWidget(temperature);
        lyt->addWidget(speedPerc);
        lyt->addWidget(removeBtn);

        setLayout(lyt);

        QObject::connect(removeBtn, &ConsoleButton::clicked, this, &FanCurveWidget::onRemoveBtnClicked);
    }

    void FanCurveWidget::setData(const int temp, const int speed) const {
        temperature->setValue(temp);
        speedPerc->setValue(speed);
    }

    void FanCurveWidget::onRemoveBtnClicked() {
        emit removePoint(this);
    }
}
