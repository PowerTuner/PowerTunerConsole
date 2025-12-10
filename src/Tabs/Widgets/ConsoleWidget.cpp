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
#include "ConsoleWidget.h"

namespace PWT::CUI {
    ConsoleWidget::ConsoleWidget() {
        QPalette pal = palette();

        bakWindowTextColor = pal.color(QPalette::WindowText);

        pal.setColor(QPalette::Base, QColor(23, 25, 29));

        setPalette(pal);
        setAutoFillBackground(true);
        setFocusPolicy(Qt::NoFocus);
        setSelected(false);
    }

    void ConsoleWidget::setSelected(const bool selected) {
        QPalette widgetPal = palette();

        widgetPal.setColor(QPalette::Window, selected ? selectedBgColor : unselectedBgColor);
        setPalette(widgetPal);
    }

    void ConsoleWidget::changeEvent(QEvent *event) {
        switch (event->type()) {
            case QEvent::EnabledChange: {
                QPalette pal = palette();

                pal.setColor(QPalette::WindowText, isEnabled() ? bakWindowTextColor : textDisabledColor);
                setPalette(pal);
            }
                break;
            default:
                break;
        }

        QWidget::changeEvent(event);
    }

    void ConsoleWidget::keyReleaseEvent(QKeyEvent *evt) {
        const Qt::Key ekey = static_cast<Qt::Key>(evt->key());

        if (ekey == consoleSettings->getClickKB() && !evt->isAutoRepeat())
            clickAction(false);
        else if (ekey == consoleSettings->getBackKB() && !evt->isAutoRepeat())
            backAction();
        else if (ekey == consoleSettings->getInputModeKB() && !evt->isAutoRepeat())
            inputModeAction();
        else
            QWidget::keyPressEvent(evt);
    }

    void ConsoleWidget::keyPressEvent(QKeyEvent *evt) {
        const Qt::Key ekey = static_cast<Qt::Key>(evt->key());

        if (ekey == consoleSettings->getLeftKB())
            leftAction();
        else if (ekey == consoleSettings->getFastLeftKB())
            fastLeftAction();
        else if (ekey == consoleSettings->getRightKB())
            rightAction();
        else if (ekey == consoleSettings->getFastRightKB())
            fastRightAction();
        else if (ekey == consoleSettings->getScrollUpKB())
            scrollUpAction();
        else if (ekey == consoleSettings->getScrollDownKB())
            scrollDownAction();
        else if (ekey == consoleSettings->getScrollLeftKB())
            scrollLeftAction();
        else if (ekey == consoleSettings->getScrollRightKB())
            scrollRightAction();
        else
            QWidget::keyPressEvent(evt);
    }

    void ConsoleWidget::focusInEvent(QFocusEvent *event) {
        if (hasTTSEnabled())
            emit sayStoppableTTS(getFocusedTTS());

        setSelected(true);
        emit focusChangeExternalIn(this);
    }

    void ConsoleWidget::focusOutEvent(QFocusEvent *event) {
        setSelected(false);
    }

    void ConsoleWidget::processGamepadButtonEvent(const GamepadButton button, const GamepadMap &mapping) {
        if (button == mapping.leftGP)
            leftAction();
        if (button == mapping.fastLeftGP)
            fastLeftAction();
        else if (button == mapping.rightGP)
            rightAction();
        else if (button == mapping.fastRightGP)
            fastRightAction();
        else if (button == mapping.clickGP)
            clickAction(true);
        else if (button == mapping.backGP)
            backAction();
        else if (button == mapping.inputModeGP)
            inputModeAction();
        else if (button == mapping.scrollUpGP)
            scrollUpAction();
        else if (button == mapping.scrollDownGP)
            scrollDownAction();
        else if (button == mapping.scrollLeftGP)
            scrollLeftAction();
        else if (button == mapping.scrollRightGP)
            scrollRightAction();
    }
}
