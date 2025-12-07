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

#include <QKeyEvent>

#include "ConsoleBaseWidget.h"

namespace PWT::CUI {
    class ConsoleWidget: public ConsoleBaseWidget {
        Q_OBJECT

    private:
        const QColor unselectedBgColor = QColor(39, 42, 50);
        const QColor selectedBgColor = QColor(68, 79, 106);

        void setSelected(bool selected);

    protected:
        const QColor textDisabledColor = QColor(130, 130, 130);
        QColor bakWindowTextColor;

        void changeEvent(QEvent *event) override;
        void keyReleaseEvent(QKeyEvent *evt) final;
        void keyPressEvent(QKeyEvent *evt) final;
        void focusInEvent(QFocusEvent *event) override;
        void focusOutEvent(QFocusEvent *event) override;

    public:
        ConsoleWidget();

        virtual void leftAction() {}
        virtual void fastLeftAction() {}
        virtual void rightAction() {}
        virtual void fastRightAction() {}
        virtual void scrollUpAction() {}
        virtual void scrollDownAction() {}
        virtual void scrollLeftAction() {}
        virtual void scrollRightAction() {}
        virtual void clickAction(const bool isGamepad) {}
        virtual void backAction() {}
        virtual void inputModeAction() {}
        virtual void writeMicroKeyboardKey(const char c) {}

        void processGamepadButtonEvent(GamepadButton button, const GamepadMap &mapping);

    signals:
        void focusChangeExternalIn(const PWT::CUI::ConsoleWidget *widget);
        void openMicroKeyboard();
    };
}
