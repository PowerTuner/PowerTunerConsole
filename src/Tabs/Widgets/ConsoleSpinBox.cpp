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
#include <QHBoxLayout>

#include "ConsoleSpinBox.h"

namespace PWT::CUI {
    ConsoleSpinBox::ConsoleSpinBox(const QString &label) {
        QHBoxLayout *lyt = new QHBoxLayout();

        titleLbl = new QLabel(label);
        spinBox = new QSpinBox();

        titleLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        titleLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        spinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        lyt->addWidget(titleLbl);
        lyt->addWidget(spinBox);

        setLayout(lyt);

        QObject::connect(spinBox, &QSpinBox::valueChanged, this, &ConsoleSpinBox::onValueChanged);
    }

    void ConsoleSpinBox::leftAction() {
        spinBox->setValue(spinBox->value() - 1);
    }

    void ConsoleSpinBox::fastLeftAction() {
        spinBox->setValue(spinBox->value() - 10);
    }

    void ConsoleSpinBox::rightAction() {
        spinBox->setValue(spinBox->value() + 1);
    }

    void ConsoleSpinBox::fastRightAction() {
        spinBox->setValue(spinBox->value() + 10);
    }

    void ConsoleSpinBox::clickAction(const bool isGamepad) {
        spinBox->setFocus(Qt::TabFocusReason);

        if (hasTTSEnabled())
            emit sayStoppableTTS(tr("numeric input focused"));
    }

    void ConsoleSpinBox::backAction() {
        spinBox->parentWidget()->setFocus(Qt::TabFocusReason);
    }

    QString ConsoleSpinBox::getFocusedTTS() const {
        return tr("%1 numeric input set to %2").arg(titleLbl->text(), spinBox->text());
    }

    void ConsoleSpinBox::onValueChanged(const int v) {
        if (hasTTSEnabled())
            emit sayStoppableTTS(QString::number(v));
    }
}
