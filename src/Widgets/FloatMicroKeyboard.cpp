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
#include <QHBoxLayout>

#include "FloatMicroKeyboard.h"
#include "../Classes/ConsoleSettings.h"

namespace PWT::CUI {
    FloatMicroKeyboard::FloatMicroKeyboard() {
        const QSharedPointer<ConsoleSettings> settings = ConsoleSettings::getInstance();
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *frameLyt = new QHBoxLayout();
        QLabel *leftAct = new QLabel("<");
        QLabel *rightAct = new QLabel(">");
        QFrame *frame = new QFrame();
        QFont actFont = leftAct->font();
        QFont inptFont;

        hasTTS = settings->getEnableTTS();
        selectedKey = 0;
        microKb = new QLabel(kbKeys[0].data());
        inptFont = microKb->font();

        inptFont.setBold(true);
        inptFont.setPointSize(26);
        actFont.setBold(true);
        actFont.setPointSize(24);
        microKb->setFont(inptFont);
        microKb->setAlignment(Qt::AlignCenter);
        leftAct->setFont(actFont);
        rightAct->setFont(actFont);
        frame->setLayout(frameLyt);
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(2);
        frame->setMidLineWidth(2);

        frameLyt->addWidget(leftAct);
        frameLyt->addStretch();
        frameLyt->addWidget(microKb);
        frameLyt->addStretch();
        frameLyt->addWidget(rightAct);
        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(frame);

        setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Tool);
        setAttribute(Qt::WA_MacAlwaysShowToolWindow);
        setFixedSize(150, 100);
        setAutoFillBackground(true);
        setLayout(lyt);
    }

    void FloatMicroKeyboard::showEvent(QShowEvent *event) {
        if (hasTTS)
            emit sayTTS(tr("micro keyboard is shown"), TTSID::MICRO_KEYBOARD);
    }

    void FloatMicroKeyboard::saySelectedKey(const std::string_view &key) {
        if (!hasTTS) [[likely]]
            return;

        if (key[0] == '[')
            emit sayStoppableTTS(tr("space"));
        else if (key == "RET")
            emit sayStoppableTTS(tr("return"));
        else
            emit sayStoppableTTS(key.data());
    }

    int FloatMicroKeyboard::prevKey(const int step) {
        if ((selectedKey - step) < 0)
            selectedKey = kbKeys.size() - qAbs(selectedKey - step);
        else
            selectedKey -= step;

        return selectedKey;
    }

    int FloatMicroKeyboard::nextKey(const int step) {
        if ((selectedKey + step) >= kbKeys.size())
            selectedKey = (selectedKey + step) - kbKeys.size();
        else
            selectedKey += step;

        return selectedKey;
    }

    void FloatMicroKeyboard::processGamepadButtonEvent(const GamepadButton button, const GamepadMap &mapping) {
        if (button == mapping.leftGP || button == mapping.fastLeftGP) {
            const int step = button == mapping.leftGP ? 1 : 5;
            const std::string_view sel = kbKeys[prevKey(step)];

            if (upperCase)
                microKb->setText(QString(sel.data()).toUpper());
            else
                microKb->setText(sel.data());

            saySelectedKey(sel);

        } else if (button == mapping.rightGP || button == mapping.fastRightGP) {
            const int step = button == mapping.rightGP ? 1 : 5;
            const std::string_view sel = kbKeys[nextKey(step)];

            if (upperCase)
                microKb->setText(QString(sel.data()).toUpper());
            else
                microKb->setText(sel.data());

            saySelectedKey(sel);

        } else if (button == mapping.scrollUpGP) {
            move(x(), 0);

        } else if (button == mapping.scrollDownGP) {
            move(x(), parentWidget()->height() - height());

        } else if (button == mapping.scrollLeftGP) {
            move(0, y());

        } else if (button == mapping.scrollRightGP) {
            move(parentWidget()->width() - width(), y());

        } else if (button == mapping.inputModeGP) {
            upperCase = !upperCase;

            if (upperCase) {
                microKb->setText(QString(kbKeys[selectedKey].data()).toUpper());

                if (hasTTS)
                    emit sayStoppableTTS(tr("uppercase"));

            } else {
                microKb->setText(kbKeys[selectedKey].data());

                if (hasTTS)
                    emit sayStoppableTTS(tr("lowercase"));
            }

        } else if (button == mapping.clickGP) {
            if (kbKeys[selectedKey][0] == '[') // space
                emit writeKey(' ');
            else if (kbKeys[selectedKey] == "RET")
                emit writeKey(EnterKey);
            else if (upperCase)
                emit writeKey(QChar::fromLatin1(kbKeys[selectedKey][0]).toUpper().toLatin1());
            else
                emit writeKey(kbKeys[selectedKey][0]);

        } else if (button == mapping.prevTabGP) { // simulate backspace
            emit writeKey(BackspaceKey);

            if (hasTTS)
                emit sayStoppableTTS(tr("backspace key"));

        } else if (button == mapping.nextTabGP) { // clear text input
            emit writeKey(DeleteKey);

            if (hasTTS)
                emit sayStoppableTTS(tr("delete key"));

        } else if (button == mapping.backGP) {
            if (hasTTS)
                emit sayTTS(tr("closing micro keyboard"), TTSID::MICRO_KEYBOARD);

            emit closeKeyboard();
        }
    }
}
