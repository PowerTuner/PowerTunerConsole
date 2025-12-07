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

#include <QLineEdit>
#include <QLabel>

#include "ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleLineEdit final: public ConsoleWidget {
        Q_OBJECT

    private:
        QLabel *lineEditLbl = nullptr;
        QLineEdit *lineEdit = nullptr;

    public:
        explicit ConsoleLineEdit(const QString &label = "");

        [[nodiscard]] QString getText() const { return lineEdit->text(); }
        void setText(const QString &text) const { lineEdit->setText(text); }
        void setPlaceholder(const QString &plh) const { lineEdit->setPlaceholderText(plh); }
        void clear() const { lineEdit->clear(); }

        [[nodiscard]] QString getFocusedTTS() const override;
        void clickAction(bool isGamepad) override;
        void backAction() override;
        void writeMicroKeyboardKey(char c) override;

    private slots:
        void onReturnPressed() { emit returnPressed(); }

    signals:
        void returnPressed();
    };
}
