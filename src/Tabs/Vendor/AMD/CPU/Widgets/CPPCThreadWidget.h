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

#include "../../../../Widgets/ConsoleSliderLevels.h"
#include "pwtShared/Include/CPU/AMD/CPPCCapability1.h"
#include "pwtShared/Include/CPU/AMD/CPPCRequest.h"

namespace PWT::CUI::AMD {
    class CPPCThreadWidget final: public QWidget {
        Q_OBJECT

    private:
        ConsoleSliderLevels *maxPerf = nullptr;
        ConsoleSliderLevels *minPerf = nullptr;
        ConsoleSliderLevels *desPerf = nullptr;
        ConsoleSliderLevels *epp = nullptr;

    public:
        CPPCThreadWidget();

        void setRanges(const PWTS::AMD::CPPCCapability1 &cap1) const;
        void setData(const PWTS::AMD::CPPCRequest &data) const;
        PWTS::AMD::CPPCRequest getData() const;

    private slots:
        void onMaxPerfChanged(int v) const;
        void onDesPerfChanged(int v) const;
        void onEppChanged(int v) const;
    };
}
