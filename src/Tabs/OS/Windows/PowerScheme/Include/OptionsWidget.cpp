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
#include "OptionsWidget.h"

namespace PWT::CUI::WIN {
    OptionsWidget::OptionsWidget(const PWTS::WIN::PowerSchemeSettingData &data, const QString &guid): PowerSettingWidget(data.name, data.groupTitle, guid) {
        options = new ConsoleSelect(data.name, data.description);

        options->addOptions(data.options);

        lyt->addWidget(options);

        QObject::connect(options, &ConsoleSelect::selectionChanged, this, &OptionsWidget::onSettingValueChanged);
    }

    void OptionsWidget::setOptions(const QList<QString> &optionList) const {
        options->clear();
        options->addOptions(optionList);
    }

    void OptionsWidget::onModeChanged(const int idx) const {
        const QSignalBlocker sblock {options};

        if (idx == 0)
            options->setCurrentIndex(settings.value(currentScheme).ac);
        else
            options->setCurrentIndex(settings.value(currentScheme).dc);
    }
}
