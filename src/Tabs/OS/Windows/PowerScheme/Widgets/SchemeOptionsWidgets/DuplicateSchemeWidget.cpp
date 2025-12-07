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

#include "DuplicateSchemeWidget.h"

namespace PWT::CUI::WIN {
    DuplicateSchemeWidget::DuplicateSchemeWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        schemesCombo = new ConsoleSelect("Base scheme");
        duplicateName = new ConsoleLineEdit("Duplicate name");
        duplicateBtn = new ConsoleButton("Duplicate scheme");

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(schemesCombo);
        lyt->addWidget(duplicateName);
        lyt->addWidget(duplicateBtn);

        setLayout(lyt);

        QObject::connect(duplicateBtn, &ConsoleButton::clicked, this, &DuplicateSchemeWidget::onDuplicateClicked);
    }

    void DuplicateSchemeWidget::clearWidget() const {
        duplicateName->clear();
        schemesCombo->clear();
        schemesCombo->addOption("");
    }

    void DuplicateSchemeWidget::addScheme(const QString &label) const {
        schemesCombo->addOption(label);
    }

    void DuplicateSchemeWidget::removeScheme(const QString &label) const {
        schemesCombo->removeAt(schemesCombo->indexOf(label));
    }

    void DuplicateSchemeWidget::onDuplicateClicked() {
        const QString name = duplicateName->getText();

        if (name.isEmpty() || schemesCombo->getCurrentIndex() == 0)
            return;

        emit createDuplicate(schemesCombo->getCurrentText(), name);
        duplicateName->clear();
        schemesCombo->setCurrentIndex(0);
    }
}
