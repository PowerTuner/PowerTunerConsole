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

#include "SectionItemWidget.h"

namespace PWT::CUI {
    SectionItemWidget::SectionItemWidget(const QString &title, const QString &subTitle) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QPalette pal = palette();
        QFont wfont = font();

        itmTitle = new QLabel(title);

        itmTitle->setAlignment(Qt::AlignLeft);
        itmTitle->setMinimumHeight(20);
        pal.setColor(QPalette::WindowText, QColor(210, 210, 220));
        pal.setColor(QPalette::Window, QColor(17, 19, 22));
        wfont.setPointSize(11);
        setPalette(pal);
        setFont(wfont);
        setAutoFillBackground(true);

        lyt->addWidget(itmTitle);

        if (!subTitle.isEmpty()) {
            itmSubTitle = new QLabel(subTitle);

            QFont font = itmSubTitle->font();

            font.setItalic(true);
            font.setPointSize(10);
            itmSubTitle->setAlignment(Qt::AlignLeft);
            itmSubTitle->setFont(font);

            lyt->addWidget(itmSubTitle);
            lyt->setSpacing(0);
            lyt->setContentsMargins(8, 1, 8, 1);
        }

        setLayout(lyt);
    }

    QString SectionItemWidget::getFocusedTTS() const {
        QString tts = tr("item title: %1").arg(itmTitle->text());

        if (itmSubTitle != nullptr)
            tts.append(tr(", subtitle: %2").arg(itmSubTitle->text()));

        return tts;
    }
}
