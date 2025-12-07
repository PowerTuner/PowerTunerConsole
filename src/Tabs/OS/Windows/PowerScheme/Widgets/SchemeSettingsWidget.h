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

#include "../Include/PowerSettingWidget.h"
#include "pwtShared/Include/OS/Windows/PowerSchemeSettingData.h"

namespace PWT::CUI::WIN {
    class SchemeSettingsWidget final: public QWidget {
    private:
        struct SettingItm final {
            bool enabled = true;
            PowerSettingWidget *widget = nullptr;
        };

        static constexpr int pageSize = 30;
        QVBoxLayout *lyt = nullptr;
        mutable int startPageIdx = 0;
        mutable int endPageIdx = 0;
        QHash<QString, QSharedPointer<SettingItm>> settingWidgMap;
        QList<QSharedPointer<SettingItm>> settingItmList;
        mutable QList<QSharedPointer<SettingItm>> pageItmList;

        [[nodiscard]] bool isSettingVisible(const QString &filter, const PowerSettingWidget *wdg) const;
        void preparePreviousPage(const QString &optionsFilter) const;
        void prepareNextPage(const QString &optionsFilter) const;

    public:
        SchemeSettingsWidget();

        [[nodiscard]] bool hasPreviousPage() const { return startPageIdx > 0; }
        [[nodiscard]] bool hasNextPage() const { return endPageIdx < settingItmList.size(); }

        void disableSettingWidgets();
        void addSetting(const QString &guid, const PWTS::WIN::PowerSchemeSettingData &schemeSetting);
        void addData(const QString &scheme, const QString &settingGuid, const PWTS::WIN::PowerSchemeSetting &setting) const;
        [[nodiscard]] QMap<QString, PWTS::WIN::PowerSchemeSetting> getWidgetsData(const QString &scheme) const;
        void switchScheme(const QString &schemeGuid) const;
        void hidePage() const;
        void showPage() const;
        void previousPage(const QString &optionsFilter) const;
        void nextPage(const QString &optionsFilter) const;
        void duplicateSettings(const QString &current, const QString &duplicate) const;
        void deleteScheme(const QString &scheme) const;
        void filterVisibleSettings(const QString &filter) const;
        void setModeAll(int idx) const;
        void copySchemeValues(const QString &fromScheme, int fromMode, const QString &toScheme, int toMode) const;
    };
}
