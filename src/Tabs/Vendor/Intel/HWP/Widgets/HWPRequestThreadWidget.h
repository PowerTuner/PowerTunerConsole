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

#include "HWPRequestWidget.h"
#include "../../../../Widgets/ConsoleSelect.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"
#include "pwtShared/Include/Feature.h"

namespace PWT::CUI::INTEL {
    class HWPRequestThreadWidget final: public QWidget {
        Q_OBJECT

    private:
        struct WData final {
            bool valid;
            PWTS::Intel::HWPCapabilities caps;
            PWTS::Intel::HWPRequest request;
        };

        HWPRequestWidget *hwpRequestWidget = nullptr;
        ConsoleSelect *cpuSelect = nullptr;
        ConsoleCheckbox *applyToAll = nullptr;
        int prevCpuSelect = 0;
        QList<WData> data;

        void updateHwpReqWidget(int idx) const;

    public:
        explicit HWPRequestThreadWidget(const QSet<PWTS::Feature> &cpuFeatures, int threadCount);

        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet, bool isIgnored);

    private slots:
        void onApplyToAllStateChanged(Qt::CheckState state) const;
        void onCpuSelectChanged(int idx);
    };
}
