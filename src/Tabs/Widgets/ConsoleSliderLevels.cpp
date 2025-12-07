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
#include "ConsoleSliderLevels.h"

namespace PWT::CUI {
    ConsoleSliderLevels::ConsoleSliderLevels(const QString &left, const QString &right, const QString &title, const QString &help): ConsoleSliderBase(help) {
        QHBoxLayout *lvlLyt = new QHBoxLayout();
        QHBoxLayout *headLyt = new QHBoxLayout();
        QLabel *leftLbl = new QLabel(left);
        QLabel *rightLbl = new QLabel(right);

        titleLbl = new QLabel(title);
        label = new QLabel("0");

        headLyt->addWidget(titleLbl);
        headLyt->addStretch();
        headLyt->addWidget(label);
        lvlLyt->addWidget(leftLbl);
        lvlLyt->addStretch();
        lvlLyt->addWidget(rightLbl);
        lyt->insertLayout(0, headLyt);
        lyt->addLayout(lvlLyt);
    }
}
