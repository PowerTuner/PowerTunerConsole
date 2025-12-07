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

#include "CopySchemeSettingsWidget.h"

namespace PWT::CUI::WIN {
    CopySchemeSettingsWidget::CopySchemeSettingsWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        copyFromScheme = new ConsoleSelect("From scheme");
        copyFromMode = new ConsoleSelect("From mode");
        copyToScheme = new ConsoleSelect("To scheme");
        copyToMode = new ConsoleSelect("To mode");
        copyBtn = new ConsoleButton("Copy settings");

        copyFromMode->addOptions({"", "Plugged In", "Battery"});
        copyToMode->addOptions({"", "Plugged In", "Battery", "All"});

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(copyFromScheme);
        lyt->addWidget(copyFromMode);
        lyt->addWidget(copyToScheme);
        lyt->addWidget(copyToMode);
        lyt->addWidget(copyBtn);

        setLayout(lyt);

        QObject::connect(copyBtn, &ConsoleButton::clicked, this, &CopySchemeSettingsWidget::onCopySettingsClicked);
    }

    void CopySchemeSettingsWidget::clearWidget() const {
        copyFromScheme->clear();
        copyToScheme->clear();
        copyFromScheme->addOption("");
        copyToScheme->addOption("");
    }

    void CopySchemeSettingsWidget::addScheme(const QString &label) const {
        copyFromScheme->addOption(label);
        copyToScheme->addOption(label);
    }

    void CopySchemeSettingsWidget::removeScheme(const QString &label) const {
        const int idx = copyFromScheme->indexOf(label);

        if (idx != -1) {
            copyFromScheme->removeAt(idx);
            copyToScheme->removeAt(idx);
        }
    }

    void CopySchemeSettingsWidget::onCopySettingsClicked() {
        if (copyFromScheme->getCurrentIndex() == 0 || copyFromMode->getCurrentIndex() == 0 || copyToScheme->getCurrentIndex() == 0 || copyToMode->getCurrentIndex() == 0)
            return;

        emit copySettings(copyFromScheme->getCurrentText(), copyFromMode->getCurrentIndex() - 1, copyToScheme->getCurrentText(), copyToMode->getCurrentIndex() - 1);
        copyFromScheme->setCurrentIndex(0);
        copyFromMode->setCurrentIndex(0);
        copyToScheme->setCurrentIndex(0);
        copyToMode->setCurrentIndex(0);
    }
}
