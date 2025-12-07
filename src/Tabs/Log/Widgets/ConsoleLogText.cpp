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
#include <QScrollBar>
#include <QScroller>

#include "ConsoleLogText.h"

namespace PWT::CUI {
    ConsoleLogText::ConsoleLogText() {
        QVBoxLayout *lyt = new QVBoxLayout();

        logs = new QTextEdit();

        logs->setFocusPolicy(Qt::NoFocus);
        logs->setWordWrapMode(QTextOption::NoWrap);
        logs->setReadOnly(true);
        logs->setAcceptRichText(true);
        logs->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
        QScroller::grabGesture(logs->viewport(), QScroller::LeftMouseButtonGesture);

        lyt->addWidget(logs);
        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
    }

    void ConsoleLogText::scrollUpAction() {
        logs->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
    }

    void ConsoleLogText::scrollDownAction() {
        logs->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
    }

    void ConsoleLogText::scrollLeftAction() {
        logs->horizontalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
    }

    void ConsoleLogText::scrollRightAction() {
        logs->horizontalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
    }

    void ConsoleLogText::append(const QString &msg) const {
        logs->append(msg);
    }

    void ConsoleLogText::clearLog() const {
        logs->clear();
    }

    QString ConsoleLogText::getFocusedTTS() const {
        return tr("log messages: %1").arg(logs->toPlainText());
    }
}
