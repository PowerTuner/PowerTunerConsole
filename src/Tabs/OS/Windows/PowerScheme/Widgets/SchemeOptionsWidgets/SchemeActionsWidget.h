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

#include "../../../../../Widgets/ConsoleCheckbox.h"
#include "../../../../../Widgets/ConsoleButton.h"

namespace PWT::CUI::WIN {
    class SchemeActionsWidget final: public QWidget {
        Q_OBJECT

    private:
		ConsoleCheckbox *setActive = nullptr;
        ConsoleCheckbox *deleteFlag = nullptr;
        ConsoleCheckbox *resetFlag = nullptr;
        ConsoleButton *deleteBtn = nullptr;

    public:
        SchemeActionsWidget();

        void switchScheme(const QString &scheme, bool isActive, bool deleteFlagged, bool resetFlagged) const;

    private slots:
        void onSetActiveChanged(Qt::CheckState state);
        void onDeleteFlagChanged(Qt::CheckState state);
        void onResetFlagChanged(Qt::CheckState state);
        void onDeleteBtnClicked();

    signals:
        void setSchemeActive();
        void setDeleteFlag(bool flagged);
        void setResetFlag(bool flagged);
        void deleteScheme();
    };
}
