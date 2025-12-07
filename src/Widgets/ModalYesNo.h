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

#include "Modal.h"
#include "../Tabs/Widgets/ConsoleButton.h"

namespace PWT::CUI {
    class ModalYesNo final: public Modal {
        Q_OBJECT

    private:
        QLabel *message = nullptr;
        ConsoleButton *okBtn = nullptr;
        ConsoleButton *cancelBtn = nullptr;

    protected:
        void keyReleaseEvent(QKeyEvent *evt) override;
        void focusInEvent(QFocusEvent *event) override;

    public:
        ModalYesNo(const QString &title, const QString &msg, QWidget *parent);

        void processGamepadButtonEvent(GamepadButton button, const GamepadMap &mapping) override;

    private slots:
        void onSayStoppableTTS(const QString &msg);
    };
}
