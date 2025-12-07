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
#include "ProfilesTab.h"
#include "../Widgets/SectionWidget.h"

namespace PWT::CUI {
    ProfilesTab::ProfilesTab() {
        profileList = new ConsoleSelect("Profile");
        loadProfileBtn = new ConsoleButton("Load");
        applyProfileBtn = new ConsoleButton("Apply");
        saveProfileBtn = new ConsoleButton("Save");
        deleteProfileBtn = new ConsoleButton("Delete");
        createProfileBtn = new ConsoleButton("Create");
        newProfileName = new ConsoleLineEdit("Name");

        scrollWidgLyt->insertWidget(nextInsertIdx(), profileList);
        scrollWidgLyt->insertWidget(nextInsertIdx(), loadProfileBtn);
        scrollWidgLyt->insertWidget(nextInsertIdx(), applyProfileBtn);
        scrollWidgLyt->insertWidget(nextInsertIdx(), saveProfileBtn);
        scrollWidgLyt->insertWidget(nextInsertIdx(), deleteProfileBtn);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("Create"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), newProfileName);
        scrollWidgLyt->insertWidget(nextInsertIdx(), createProfileBtn);

        QObject::connect(loadProfileBtn, &ConsoleButton::clicked, this, &ProfilesTab::onProfileLoadClicked);
        QObject::connect(applyProfileBtn, &ConsoleButton::clicked, this, &ProfilesTab::onProfileApplyClicked);
        QObject::connect(deleteProfileBtn, &ConsoleButton::clicked, this, &ProfilesTab::onProfileDeleteClicked);
        QObject::connect(saveProfileBtn, &ConsoleButton::clicked, this, &ProfilesTab::onProfileSaveClicked);
        QObject::connect(createProfileBtn, &ConsoleButton::clicked, this, &ProfilesTab::onProfileCreateClicked);

        setEnabled(false);
        setupFocusableWidgets();
    }

    void ProfilesTab::setEnabled(const bool enable) const {
        profileList->setEnabled(enable);
        loadProfileBtn->setEnabled(enable);
        applyProfileBtn->setEnabled(enable);
        saveProfileBtn->setEnabled(enable);
        deleteProfileBtn->setEnabled(enable);
        newProfileName->setEnabled(enable);
        createProfileBtn->setEnabled(enable);
    }

    void ProfilesTab::updateProfileList(const QList<QString> &list) const {
        profileList->clear();
        profileList->addOption("");
        profileList->addOptions(list);
    }

    void ProfilesTab::clearWidget() const {
        profileList->clear();
    }

    void ProfilesTab::onProfileSaveClicked() {
        const QString name = profileList->getCurrentText();

        if (name.isEmpty() || !askUserYesNo(QStringLiteral("Save profile"), QString("Save current settings to \"%1\" profile?").arg(name)))
            return;

        emit profileSaved(name);
    }

    void ProfilesTab::onProfileLoadClicked() {
        const QString name = profileList->getCurrentText();

        if (name.isEmpty())
            return;

        emit profileLoaded(name);
    }

    void ProfilesTab::onProfileApplyClicked() {
        const QString name = profileList->getCurrentText();

        if (name.isEmpty())
            return;

        emit profileApplied(name);
    }

    void ProfilesTab::onProfileDeleteClicked() {
        const QString name = profileList->getCurrentText();

        if (name.isEmpty() || !askUserYesNo(QStringLiteral("Delete profile"), QString("\"%1\" profile will be deleted").arg(name)))
            return;

        emit profileDeleted(name);
    }

    void ProfilesTab::onProfileCreateClicked() {
        const QString name = newProfileName->getText();

        if (
            name.isEmpty() ||
            !askUserYesNo(QStringLiteral("Create profile"), QString("Save current settings to \"%1\" profile?").arg(name)) ||
            (profileList->indexOf(name) != -1 && !askUserYesNo(QStringLiteral("Create profile"), QString("\"%1\" profile already exists, overwrite?").arg(name)))
        )
            return;

        newProfileName->clear();
        emit profileCreated(name);
    }
}
