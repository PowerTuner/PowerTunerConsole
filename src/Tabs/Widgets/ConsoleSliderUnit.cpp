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
#include "ConsoleSliderUnit.h"

namespace PWT::CUI {
    ConsoleSliderUnit::ConsoleSliderUnit(const QString &unit, const std::function<void(QLabel *, int)> &unitValueCallback, const bool hasLabel, const QString &title, const QString &help): ConsoleSliderBase(help) {
        QHBoxLayout *unitLyt = new QHBoxLayout();

        unitLabel = new QLabel(unit);
        unitValue = new QLabel("0");
        unitValueCB = unitValueCallback;

        if (hasLabel) {
            label = new QLabel("0");

            unitLyt->addWidget(label);
        }

        unitLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        unitLyt->addStretch();
        unitLyt->addWidget(unitValue);
        unitLyt->addWidget(unitLabel);

        if (!title.isEmpty()) {
            titleLbl = new QLabel(title);

            lyt->insertWidget(0, titleLbl);
            lyt->addLayout(unitLyt);

        } else {
            lyt->insertLayout(0, unitLyt);
        }
    }

    QString ConsoleSliderUnit::getFocusedTTS() const {
        return tr("%1, formatted value: %2 %3").arg(ConsoleSliderBase::getFocusedTTS(), unitValue->text(), unitLabel->text());
    }

    void ConsoleSliderUnit::setUnitLabel(const QString &unit) const {
        unitLabel->setText(unit);
    }

    void ConsoleSliderUnit::setUnitValue(const QString &text) const {
        unitValue->setText(text);
    }

    void ConsoleSliderUnit::setUnitValue(const int value) const {
        unitValue->setNum(value);
    }

    void ConsoleSliderUnit::setUnitValue(const double value) const {
        unitValue->setNum(value);
    }

    void ConsoleSliderUnit::sendTTS(const int value) {
        if (hasTTSEnabled())
            emit sayStoppableTTS(tr("%1, formatted value: %2 %3").arg(value).arg(unitValue->text(), unitLabel->text()));
    }

    void ConsoleSliderUnit::onSliderChanged(const int v) {
        unitValueCB(unitValue, v);
        ConsoleSliderBase::onSliderChanged(v);
    }

    void ConsoleSliderUnit::onSpinEditChanged(const int v) {
        unitValueCB(unitValue, v);
        ConsoleSliderBase::onSpinEditChanged(v);
    }
}
