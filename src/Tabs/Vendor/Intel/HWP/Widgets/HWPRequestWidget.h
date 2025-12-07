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

#include "../../../../Widgets/ConsoleSliderUnit.h"
#include "../../../../Widgets/ConsoleSliderLevels.h"
#include "../../../../Widgets/ConsoleCheckbox.h"
#include "pwtShared/Include/CPU/Intel/HWPRequest.h"
#include "pwtShared/Include/CPU/Intel/HWPRequestPkg.h"
#include "pwtShared/Include/CPU/Intel/HWPCapabilities.h"

namespace PWT::CUI::INTEL {
    class HWPRequestWidget final: public QWidget {
        Q_OBJECT

    private:
        ConsoleSliderUnit *minPerf = nullptr;
        ConsoleSliderUnit *maxPerf = nullptr;
        ConsoleSliderUnit *desiredPerf = nullptr;
        QPointer<ConsoleSliderLevels> epp;
        QPointer<ConsoleSliderUnit> acw;
        QPointer<ConsoleCheckbox> pkgControl;
        QPointer<ConsoleCheckbox> activeWindowValid;
        QPointer<ConsoleCheckbox> eppValid;
        QPointer<ConsoleCheckbox> desiredValid;
        QPointer<ConsoleCheckbox> maxValid;
        QPointer<ConsoleCheckbox> minValid;

    public:
        HWPRequestWidget(bool hasEPP, bool hasActivityWindow, bool hasPackageControl, bool hasValidBits);

        void setRequestPkgData(const PWTS::Intel::HWPRequestPkg &data) const;
        void setRequestData(const PWTS::Intel::HWPRequest &data) const;
        PWTS::Intel::HWPRequestPkg getRequestPkgData() const;
        PWTS::Intel::HWPRequest getRequestData() const;
        void setRanges(const PWTS::Intel::HWPCapabilities &caps) const;

    private slots:
        void onDesiredPerfChanged(int v) const;
        void onEppChanged(int v) const;
        void onAcwChanged(int v) const;
    };
}
