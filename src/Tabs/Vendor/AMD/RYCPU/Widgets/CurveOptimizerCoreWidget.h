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
#include "../../../../Widgets/ConsoleCheckbox.h"
#include "../../../../Widgets/ConsoleSelect.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::CUI::AMD {
    class CurveOptimizerCoreWidget final: public QWidget {
        Q_OBJECT

    private:
        struct WData final {
            bool valid;
            int co;
        };

        ConsoleCheckbox *applyToAll = nullptr;
        ConsoleCheckbox *enableChk = nullptr;
        ConsoleSelect *cpuSelect = nullptr;
        ConsoleSliderUnit *co = nullptr;
        QList<WData> data;
        bool enableChecked = false;
        int prevCpuSelected = 0;

        void updateCOSlider(int idx) const;

    public:
        explicit CurveOptimizerCoreWidget(int coreCount);

        void setRange(const PWTS::MinMax &range) const;
        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet);

    private slots:
        void onApplyToAllStateChanged(Qt::CheckState state) const;
        void onCpuSelectChanged(int idx);
        void onEnableStateChanged(Qt::CheckState state);
    };
}
