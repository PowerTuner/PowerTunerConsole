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

#include "ConsoleLineEdit.h"
#include "../../Widgets/FloatMicroKeyboard.h"

namespace PWT::CUI {
    ConsoleLineEdit::ConsoleLineEdit(const QString &label) {
        QHBoxLayout *lyt = new QHBoxLayout();

        lineEdit = new QLineEdit();

        if (!label.isEmpty()) {
            lineEditLbl = new QLabel(label);

            lineEditLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            lineEditLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            lyt->addWidget(lineEditLbl);
        }

        lyt->addWidget(lineEdit);

        setLayout(lyt);

        QObject::connect(lineEdit, &QLineEdit::returnPressed, this, &ConsoleLineEdit::returnPressed);
    }

    void ConsoleLineEdit::clickAction(const bool isGamepad) {
        lineEdit->setFocus(Qt::TabFocusReason);

        if (hasTTSEnabled())
            emit sayStoppableTTS(tr("text input focused"));

        if (isGamepad)
            emit openMicroKeyboard();
    }

    void ConsoleLineEdit::backAction() {
        lineEdit->parentWidget()->setFocus(Qt::TabFocusReason);
    }

    void ConsoleLineEdit::writeMicroKeyboardKey(const char c) {
        if (c == FloatMicroKeyboard::BackspaceKey)
            lineEdit->setText(lineEdit->text().removeLast());
        else if (c == FloatMicroKeyboard::DeleteKey)
            lineEdit->clear();
        else if (c == FloatMicroKeyboard::EnterKey)
            emit returnPressed();
        else
            lineEdit->setText(lineEdit->text().append(c));
    }

    QString ConsoleLineEdit::getFocusedTTS() const {
        const QString txt = lineEdit->text();

        return tr("%1 text input set to %2").arg(
            lineEditLbl == nullptr ? "" : lineEditLbl->text(),
            txt.isEmpty() ? "empty" : txt
        );
    }
}
