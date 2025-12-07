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

#include "PkgRaplLimitWidget.h"
#include "../../../../../Widgets/ConsoleCheckbox.h"
#include "../../../../../Widgets/ConsoleSliderUnit.h"

namespace PWT::CUI::INTEL {
    class PkgRaplLimitIVBWidget final: public PkgRaplLimitWidget {
        Q_OBJECT

    private:
        ConsoleCheckbox *lock = nullptr;
        ConsoleSliderUnit *pl1 = nullptr;
        ConsoleSliderUnit *pl1Time = nullptr;
        ConsoleSliderUnit *pl2 = nullptr;
        ConsoleSliderUnit *pl2Time = nullptr;
        ConsoleCheckbox *pl1Enable = nullptr;
        ConsoleCheckbox *pl2Enable = nullptr;
        ConsoleCheckbox *pl1Clamp = nullptr;
        ConsoleCheckbox *pl2Clamp = nullptr;

    public:
        PkgRaplLimitIVBWidget();

        void setRanges(const PWTS::MinMax &range) const override;
        void setData(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(const PWTS::ClientPacket &packet) const override;

    private slots:
        void onPl1EnableChanged(Qt::CheckState state) const;
        void onPl2EnableChanged(Qt::CheckState state) const;
    };
}
