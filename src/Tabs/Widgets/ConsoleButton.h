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

#include <QPushButton>

#include "ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleButton final: public ConsoleWidget {
        Q_OBJECT

    private:
        const QColor buttonFocusColor = QColor(68, 79, 106);
        const QColor buttonNoFocusColor = QColor(39, 42, 50);
        const QColor textEnabledColor = QColor(250, 250, 250);
        QPushButton *btn = nullptr;

        void setSelected(bool selected) const;

    protected:
        void changeEvent(QEvent *event) override;
        void focusInEvent(QFocusEvent *event) override;
        void focusOutEvent(QFocusEvent *event) override;

    public:
        explicit ConsoleButton(const QString &label);

        void setText(const QString &text) const { btn->setText(text); }

        [[nodiscard]] QString getFocusedTTS() const override;
        void clickAction(bool isGamepad) override;

    signals:
        void clicked();
    };
}
