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

#include "ConsoleCheckbox.h"

namespace PWT::CUI {
    ConsoleCheckbox::ConsoleCheckbox(const QString &label, const QString &help) {
        QHBoxLayout *lyt = new QHBoxLayout();
        QPalette pal;

        checkLbl = new QLabel(label);
        checkbx = new QCheckBox();
        pal = checkbx->palette();

        checkbx->setFocusPolicy(Qt::NoFocus);
        checkbx->setMinimumHeight(30);
        checkbx->setMinimumWidth(30);
        pal.setColor(QPalette::Window, QColor(173, 173, 188)); // check
        pal.setColor(QPalette::Accent, QColor(20, 24, 33)); // indicator
        checkbx->setPalette(pal);

        checkLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lyt->addWidget(checkLbl);
        lyt->addStretch();
        lyt->addWidget(checkbx);

        if (!help.isEmpty()) {
            helpLbl = new QLabel(help);

            QVBoxLayout *hlyt = new QVBoxLayout();
            QFont font = helpLbl->font();

            font.setPointSize(10);
            font.setStyle(QFont::StyleItalic);
            font.setWeight(QFont::Normal);
            helpLbl->setFont(font);
            helpLbl->setAlignment(Qt::AlignLeft);
            helpLbl->setWordWrap(true);

            hlyt->addLayout(lyt);
            hlyt->addWidget(helpLbl);
            setLayout(hlyt);

        } else {
            setLayout(lyt);
        }

        QObject::connect(checkbx, &QCheckBox::checkStateChanged, this, &ConsoleCheckbox::onCheckStateChanged);
    }

    void ConsoleCheckbox::clickAction(const bool isGamepad) {
        checkbx->setChecked(!checkbx->isChecked());
    }

    QString ConsoleCheckbox::getFocusedTTS() const {
        return QString("%1: %2 %3").arg(
            checkLbl->text(),
            checkbx->isChecked() ? "checked":"unchecked",
            helpLbl == nullptr ? "" :  QString(", help: %1").arg(helpLbl->text())
        );
    }

    void ConsoleCheckbox::onCheckStateChanged(const Qt::CheckState state) {
        emit checkStateChanged(state);

        if (hasTTSEnabled())
            emit sayStoppableTTS(tr(state == Qt::Checked ? "checked":"unchecked"));
    }
}
