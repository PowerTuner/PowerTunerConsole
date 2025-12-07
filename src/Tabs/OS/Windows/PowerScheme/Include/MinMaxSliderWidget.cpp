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
#include "MinMaxSliderWidget.h"

namespace PWT::CUI::WIN {
    MinMaxSliderWidget::MinMaxSliderWidget(const PWTS::WIN::PowerSchemeSettingData &data, const QString &guid): PowerSettingWidget(data.name, data.groupTitle, guid) {
        slider = new ConsoleSliderUnit(data.valueUnit, [](QLabel *unitV, const int v) { unitV->setNum(v); }, false, "", data.description);

        setLimits(data);

        lyt->addWidget(slider);

        QObject::connect(slider, &ConsoleSliderUnit::valueChanged, this, &MinMaxSliderWidget::onSliderChanged);
    }

    int MinMaxSliderWidget::getPageStep(const int valueMax) const {
        if (valueMax >= 1000)
            return 100;
        else if (valueMax >= 100)
            return 10;

        return 5;
    }

    void MinMaxSliderWidget::setLimits(const PWTS::WIN::PowerSchemeSettingData &data) const {
        slider->setRange(data.range.min, data.range.max);
        slider->setSingleStep(data.valueIncrement);
        slider->setPageStep(getPageStep(data.range.max));
    }

    void MinMaxSliderWidget::onSliderChanged(const int v) {
        onSettingValueChanged(v);
    }

    void MinMaxSliderWidget::onModeChanged(const int idx) const {
        const QSignalBlocker sblock {slider};

        if (idx == 0)
            slider->setValue(settings.value(currentScheme).ac);
        else
            slider->setValue(settings.value(currentScheme).dc);
    }
}
