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
#include "SliderLimitWidget.h"

namespace PWT::CUI::AMD {
    SliderLimitWidget::SliderLimitWidget(const QString &label, const QString &unit, const std::function<void(QLabel *,int)> &unitVCallback) {
        QVBoxLayout *lyt = new QVBoxLayout();

        slider = new ConsoleSliderUnit(unit, unitVCallback);

        slider->setStaticLabel(label);
        slider->setPageStep(100);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(slider);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(slider, &ConsoleSliderUnit::valueChanged, this, &SliderLimitWidget::onSliderValueChanged);
    }

    void SliderLimitWidget::setRange(const PWTS::MinMax &range) const {
        const QSignalBlocker sblock {slider};

        slider->setRange(range.min, range.max);
    }

    void SliderLimitWidget::onSliderValueChanged(const int v) {
        emit sliderValueChanged(v);
    }
}
