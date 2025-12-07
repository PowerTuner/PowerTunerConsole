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
#include "SliderClockWidget.h"

namespace PWT::CUI::AMD {
    SliderClockWidget::SliderClockWidget(const QString &label, const bool enableDontSet) {
        QVBoxLayout *lyt = new QVBoxLayout();

        slider = new ConsoleSliderUnit("MHz", [](QLabel *unitV, const int v) { unitV->setNum(v); });

        slider->setStaticLabel(label);
        slider->setPageStep(100);

        if (enableDontSet) {
            dontSet = new ConsoleCheckbox("Don't apply this setting");

            lyt->addWidget(dontSet);

            QObject::connect(dontSet, &ConsoleCheckbox::checkStateChanged, this, &SliderClockWidget::onDontSetChanged);
        }

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(slider);

        setLayout(lyt);
        setEnabled(false);
    }

    void SliderClockWidget::setRange(const PWTS::MinMax &range) const {
        slider->setRange(range.min, range.max);
    }

    void SliderClockWidget::onDontSetChanged(const Qt::CheckState state) const {
        slider->setEnabled(state == Qt::Unchecked);
    }
}
