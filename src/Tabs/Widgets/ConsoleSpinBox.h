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
#pragma once

#include <QSpinBox>
#include <QLabel>

#include "ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleSpinBox final: public ConsoleWidget {
        Q_OBJECT

    private:
        QLabel *titleLbl = nullptr;
        QSpinBox *spinBox = nullptr;

    public:
        explicit ConsoleSpinBox(const QString &label);

        void setRange(const int min, const int max) const { spinBox->setRange(min, max); }
        [[nodiscard]] int getValue() const { return spinBox->value(); }
        void setValue(const int value) const { spinBox->setValue(value); }
        void clear() const { spinBox->clear(); }

        [[nodiscard]] QString getFocusedTTS() const override;
        void leftAction() override;
        void fastLeftAction() override;
        void rightAction() override;
        void fastRightAction() override;
        void clickAction(bool isGamepad) override;
        void backAction() override;

    private slots:
        void onValueChanged(int v);
    };
}
