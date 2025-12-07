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
#pragma once

#include "../ConsoleTab.h"
#include "../Widgets/ConsoleSelect.h"
#include "../Widgets/ConsoleButton.h"
#include "../Widgets/ConsoleLineEdit.h"

namespace PWT::CUI {
    class ProfilesTab final: public ConsoleTab {
        Q_OBJECT

    private:
        ConsoleSelect *profileList = nullptr;
        ConsoleButton *saveProfileBtn = nullptr;
        ConsoleButton *loadProfileBtn = nullptr;
        ConsoleButton *applyProfileBtn = nullptr;
        ConsoleButton *deleteProfileBtn = nullptr;
        ConsoleButton *createProfileBtn = nullptr;
        ConsoleLineEdit *newProfileName = nullptr;

    public:
        ProfilesTab();

        void setEnabled(bool enable) const;
        void updateProfileList(const QList<QString> &list) const;
        void clearWidget() const;

    private slots:
        void onProfileSaveClicked();
        void onProfileLoadClicked();
        void onProfileApplyClicked();
        void onProfileDeleteClicked();
        void onProfileCreateClicked();

    signals:
        void profileSaved(const QString &name);
        void profileLoaded(const QString &name);
        void profileApplied(const QString &name);
        void profileDeleted(const QString &name);
        void profileCreated(const QString &name);
    };
}
