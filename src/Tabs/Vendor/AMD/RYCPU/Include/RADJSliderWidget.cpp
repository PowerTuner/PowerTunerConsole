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
#include "RADJSliderWidget.h"

namespace PWT::CUI::AMD {
    RADJSliderWidget::RADJSliderWidget(const QString &label, const QString &unit, const std::function<void(QLabel *,int)> &unitVCallback, const bool hasReadFeature) {
        QVBoxLayout *lyt = new QVBoxLayout();

        slider = new ConsoleSliderUnit(unit, unitVCallback);

        slider->setStaticLabel(label);

        if (!hasReadFeature) { // cannot read its value from table, make this an optional setting
            enableChk = new ConsoleCheckbox("Enable setting", "If unchecked, this setting is ignored and wont be applied");

            lyt->addWidget(enableChk);

            QObject::connect(enableChk, &ConsoleCheckbox::checkStateChanged, this, &RADJSliderWidget::onEnableStateChanged);
        }

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(slider);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(slider, &ConsoleSliderUnit::valueChanged, this, &RADJSliderWidget::onSliderValueChanged);
    }

    void RADJSliderWidget::setRange(const PWTS::MinMax &range) const {
        const QSignalBlocker sblock {slider};

        slider->setRange(range.min, range.max);
    }

    void RADJSliderWidget::onSliderValueChanged(const int v) {
        emit sliderValueChanged(v);
    }

    void RADJSliderWidget::onEnableStateChanged(const Qt::CheckState state) {
        enableChecked = state == Qt::Checked;
    }
}
