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

#include "PKGCSTConfigControlCoreWidget.h"
#include "../../../../../Widgets/ConsoleSelect.h"
#include "../../../../../Widgets/ConsoleCheckbox.h"

namespace PWT::CUI::INTEL {
    class PKGCSTConfigControlSBCoreWidget final: public PKGCSTConfigControlCoreWidget {
        Q_OBJECT

    private:
        ConsoleSelect *pkgCStateLimit = nullptr;
        ConsoleCheckbox *ioMwaitRedirectionEnable = nullptr;
        ConsoleCheckbox *cfgLock = nullptr;
        ConsoleCheckbox *c3StateAutodemotionEnable = nullptr;
        ConsoleCheckbox *c1StateAutodemotionEnable = nullptr;
        ConsoleCheckbox *c3UndemotionEnable = nullptr;
        ConsoleCheckbox *c1UndemotionEnable = nullptr;

    public:
        PKGCSTConfigControlSBCoreWidget();

        void setData(const PWTS::Intel::PkgCstConfigControl &data, bool isBiosLocked) const override;
        PWTS::Intel::PkgCstConfigControl getData() const override;
        void applyCFGLock(bool biosLock) const;

    private slots:
        void onCfgLockStateChanged(Qt::CheckState state) const;
    };
}
