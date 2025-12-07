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

#include <QLabel>

#include "ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleLabel final: public ConsoleWidget {
    private:
        QLabel *title = nullptr;
        QLabel *labelTx = nullptr;

    public:
        ConsoleLabel(const QString &label, const QString &text);

        [[nodiscard]] QString getText() const { return labelTx->text(); }
        void setText(const QString &text) const { labelTx->setText(text); }
        void setNum(const int value) const { labelTx->setNum(value); }

        [[nodiscard]] QString getFocusedTTS() const override;
    };
}
