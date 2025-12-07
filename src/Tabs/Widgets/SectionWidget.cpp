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

#include "SectionWidget.h"

namespace PWT::CUI {
    SectionWidget::SectionWidget(const QString &title, const Size sectionSize) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QPalette pal = palette();
        QFont wfont = font();

        sectionTitle = new QLabel(title);

        sectionTitle->setAlignment(Qt::AlignCenter);
        sectionTitle->setMinimumHeight(static_cast<int>(sectionSize));
        pal.setColor(QPalette::WindowText, QColor(210, 210, 220));
        pal.setColor(QPalette::Window, QColor(17, 19, 22));
        wfont.setPointSize(sectionSize == Size::Default ? 12 : 14);
        setPalette(pal);
        setFont(wfont);
        setAutoFillBackground(true);

        lyt->addWidget(sectionTitle);

        setLayout(lyt);
    }

    QString SectionWidget::getFocusedTTS() const {
        return tr("section title: %1").arg(sectionTitle->text());
    }
}
