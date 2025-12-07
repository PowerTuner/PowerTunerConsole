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
#include <QVBoxLayout>

#include "ConsoleButton.h"

namespace PWT::CUI {
    ConsoleButton::ConsoleButton(const QString &label) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QPalette pal;

        btn = new QPushButton(label);
        pal = btn->palette();

        btn->setFocusPolicy(Qt::NoFocus);
        pal.setColor(QPalette::ButtonText, textEnabledColor);
        btn->setPalette(pal);
        setSelected(false);

        lyt->addWidget(btn);
        lyt->setContentsMargins(0, 0, 0, 0);
        setLayout(lyt);
    }

    void ConsoleButton::setSelected(const bool selected) const {
        QPalette pal = btn->palette();

        pal.setColor(QPalette::Button, selected ? buttonFocusColor : buttonNoFocusColor);
        btn->setPalette(pal);
    }

    void ConsoleButton::clickAction(const bool isGamepad) {
        btn->animateClick();
        emit clicked();
    }

    void ConsoleButton::focusInEvent(QFocusEvent *event) {
        setSelected(true);
        ConsoleWidget::focusInEvent(event);
    }

    void ConsoleButton::focusOutEvent(QFocusEvent *event) {
        setSelected(false);
        ConsoleWidget::focusOutEvent(event);
    }

    void ConsoleButton::changeEvent(QEvent *event) {
        switch (event->type()) {
            case QEvent::EnabledChange: {
                QPalette pal = btn->palette();

                pal.setColor(QPalette::ButtonText, btn->isEnabled() ? textEnabledColor : textDisabledColor);
                btn->setPalette(pal);
            }
                break;
            default:
                break;
        }

        ConsoleWidget::changeEvent(event);
    }

    QString ConsoleButton::getFocusedTTS() const {
        return tr("%1 button").arg(btn->text());
    }
}
