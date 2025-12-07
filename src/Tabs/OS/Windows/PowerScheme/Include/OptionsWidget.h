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

#include "PowerSettingWidget.h"
#include "pwtShared/Include/OS/Windows/PowerSchemeSettingData.h"

namespace PWT::CUI::WIN {
    class OptionsWidget final: public PowerSettingWidget {
        Q_OBJECT

    private:
        ConsoleSelect *options = nullptr;

    protected:
        void onModeChanged(int idx) const override;

    public:
        OptionsWidget(const PWTS::WIN::PowerSchemeSettingData &data, const QString &guid);

        void setOptions(const QList<QString> &optionList) const;
    };
}
