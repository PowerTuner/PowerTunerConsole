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

#include <QTextEdit>

#include "../../Widgets/ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleLogText final: public ConsoleWidget {
        Q_OBJECT

    private:
        QTextEdit *logs = nullptr;

    protected:
        [[nodiscard]] QString getFocusedTTS() const override;

    public:
        ConsoleLogText();

        void scrollUpAction() override;
        void scrollDownAction() override;
        void scrollLeftAction() override;
        void scrollRightAction() override;
        void append(const QString &msg) const;

    public slots:
        void clearLog() const;
    };
}
