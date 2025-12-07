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

#include "ConsoleLabel.h"

namespace PWT::CUI {
    ConsoleLabel::ConsoleLabel(const QString &label, const QString &text) {
        QHBoxLayout *lyt = new QHBoxLayout();

        title = new QLabel(label);
        labelTx = new QLabel(text);

        title->setAlignment(Qt::AlignLeft);
        labelTx->setAlignment(Qt::AlignRight);
        lyt->addWidget(title);
        lyt->addWidget(labelTx);

        setLayout(lyt);
    }

    QString ConsoleLabel::getFocusedTTS() const {
        return QString("%1: %2").arg(title->text(), labelTx->text());
    }
}
