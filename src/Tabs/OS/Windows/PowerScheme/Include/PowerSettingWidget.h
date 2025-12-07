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

#include <QVBoxLayout>

#include "../../../../Widgets/ConsoleSelect.h"
#include "pwtShared/Include/OS/Windows/PowerSchemeSetting.h"

namespace PWT::CUI::WIN {
    class PowerSettingWidget: public QWidget {
        Q_OBJECT

    private:
        ConsoleSelect *mode = nullptr;

    protected:
        QVBoxLayout *lyt = nullptr;
        QHash<QString, PWTS::WIN::PowerSettingValue> settings;
        QString settingGroupGuid;
        QString settingGroupTitle;
        QString settingTitle;
        QString currentScheme;

    public:
        PowerSettingWidget(const QString &title, const QString &groupTitle, const QString &guid);

		void clearData() { settings.clear(); }
        [[nodiscard]] QString getGroupGUID() const { return settingGroupGuid; }
        [[nodiscard]] QStringView getGroupTitle() const { return settingGroupTitle; }
        [[nodiscard]] QStringView getTitle() const { return settingTitle; }
        PWTS::WIN::PowerSettingValue getData(const QString &scheme) const { return settings.value(scheme); }

        void setData(const QString &scheme, const PWTS::WIN::PowerSchemeSetting &setting);
        void switchScheme(const QString &scheme);
        void duplicateScheme(const QString &current, const QString &duplicate);
        void deleteScheme(const QString &scheme);
        void setMode(int idx) const;
        void copyValue(const QString &fromScheme, int fromMode, const QString &toScheme, int toMode);

    protected slots:
        virtual void onModeChanged(int idx) const = 0;

        void onSettingValueChanged(int v);
    };
}
