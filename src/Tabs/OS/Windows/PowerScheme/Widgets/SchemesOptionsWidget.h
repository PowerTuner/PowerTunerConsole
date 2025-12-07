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

#include "SchemeOptionsWidgets/CopySchemeSettingsWidget.h"
#include "SchemeOptionsWidgets/DuplicateSchemeWidget.h"
#include "SchemeOptionsWidgets/MiscGlobalOptionsWidget.h"
#include "SchemeOptionsWidgets/SchemeActionsWidget.h"
#include "../../../../Widgets/SectionWidget.h"
#include "pwtShared/Include/Packets/ClientPacket.h"

namespace PWT::CUI::WIN {
    class SchemesOptionsWidget final: public QWidget {
        Q_OBJECT

    private:
        SectionWidget *currentSection = nullptr;
        MiscGlobalOptionsWidget *miscGlobalOptsWidget = nullptr;
        CopySchemeSettingsWidget *copySchemeSettingsWidget = nullptr;
        DuplicateSchemeWidget *duplicateSchemeWidget = nullptr;
        SchemeActionsWidget *schemeActionsWidget = nullptr;

    public:
        SchemesOptionsWidget();

        void refreshWidget(const PWTS::DaemonPacket &packet) const;
        void setDataForPacket(const PWTS::ClientPacket &packet) const;
        void setCurrentSettingsVisible(bool visible) const;
        void clearWidget() const;
        void addScheme(const QString &label) const;
        void addDuplicatedScheme(const QString &label) const;
        void switchScheme(const QString &scheme, bool isActive, bool deleteFlag, bool resetFlag) const;
        void removeScheme(const QString &label) const;

    private slots:
        void onSetSchemeActive();
        void onCreateDuplicate(const QString &baseScheme, const QString &duplicateName);
        void onDeleteScheme();
        void onSetDeleteFlag(bool flagged);
        void onSetResetFlag(bool flagged);
        void onCopySettings(const QString &fromScheme, int fromMode, const QString &toScheme, int toMode);

    signals:
        void setSchemeActive();
        void createDuplicate(const QString &baseScheme, const QString &duplicateName);
        void deleteScheme();
        void setDeleteFlag(bool flagged);
        void setResetFlag(bool flagged);
        void copySettings(const QString &fromScheme, int fromMode, const QString &toScheme, int toMode);
    };
}
