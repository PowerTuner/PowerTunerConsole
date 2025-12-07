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

#include "ModalYesNo.h"

namespace PWT::CUI {
    ModalYesNo::ModalYesNo(const QString &title, const QString &msg, QWidget *parent): Modal(parent, title) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QFont msgFont;

        message = new QLabel(msg);
        msgFont = message->font();
        okBtn = new ConsoleButton("Yes");
        cancelBtn = new ConsoleButton("No");

        msgFont.setBold(true);
        msgFont.setPointSize(12);
        message->setFont(msgFont);
        message->setAlignment(Qt::AlignCenter);

        lyt->addSpacing(4);
        lyt->addWidget(message);
        lyt->addStretch();
        lyt->addWidget(okBtn);
        lyt->addWidget(cancelBtn);

        setLayout(lyt);

        QObject::connect(okBtn, &ConsoleButton::clicked, this, &QDialog::accept);
        QObject::connect(okBtn, &ConsoleButton::sayStoppableTTS, this, &ModalYesNo::onSayStoppableTTS);
        QObject::connect(cancelBtn, &ConsoleButton::clicked, this, &QDialog::reject);
        QObject::connect(cancelBtn, &ConsoleButton::sayStoppableTTS, this, &ModalYesNo::onSayStoppableTTS);
    }

    void ModalYesNo::focusInEvent(QFocusEvent *event) {
        if (settings->getEnableTTS())
            emit sayTTS(tr("dialog open, please choose an option: %1").arg(message->text()), TTSID::OPEN_DIALOG);
    }

    void ModalYesNo::keyReleaseEvent(QKeyEvent *evt) {
        const Qt::Key ekey = static_cast<Qt::Key>(evt->key());

        if (ekey == settings->getUpKB() && !evt->isAutoRepeat()) {
            okBtn->setFocus(Qt::TabFocusReason);

            if (settings->getEnableTTS())
                emit sayStoppableTTS(okBtn->getFocusedTTS());

        } else if (ekey == settings->getDownKB() && !evt->isAutoRepeat()) {
            cancelBtn->setFocus(Qt::TabFocusReason);

            if (settings->getEnableTTS())
                emit sayStoppableTTS(cancelBtn->getFocusedTTS());

        } else if (ekey == settings->getClickKB() && !evt->isAutoRepeat()) {
            if (okBtn->hasFocus())
                okBtn->clickAction(false);
            else if (cancelBtn->hasFocus())
                cancelBtn->clickAction(false);

        } else if (ekey == settings->getBackKB() && !evt->isAutoRepeat()) {
            cancelBtn->clickAction(false);
        }
    }

    void ModalYesNo::processGamepadButtonEvent(const GamepadButton button, const GamepadMap &mapping) {
        if (button == mapping.upGP) {
            okBtn->setFocus(Qt::TabFocusReason);

            if (settings->getEnableTTS())
                emit sayStoppableTTS(okBtn->getFocusedTTS());

        } else if (button == mapping.downGP) {
            cancelBtn->setFocus(Qt::TabFocusReason);

            if (settings->getEnableTTS())
                emit sayStoppableTTS(cancelBtn->getFocusedTTS());

        } else if (button == mapping.clickGP) {
            if (okBtn->hasFocus())
                okBtn->clickAction(true);
            else if (cancelBtn->hasFocus())
                cancelBtn->clickAction(true);

        } else if (button == mapping.backGP) {
            cancelBtn->clickAction(true);
        }
    }

    void ModalYesNo::onSayStoppableTTS(const QString &msg) {
        emit sayStoppableTTS(msg);
    }
}
