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

#include "../../../../Widgets/ConsoleSelect.h"
#include "../../../../Widgets/ConsoleCheckbox.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"
#include "pwtShared/Include/Feature.h"
#include "CPPCThreadWidget.h"

namespace PWT::CUI::AMD {
    class CPPCWidget final: public QWidget {
        Q_OBJECT

    private:
        struct WData final {
            bool valid;
            PWTS::AMD::CPPCCapability1 caps1;
            PWTS::AMD::CPPCRequest request;
        };

        ConsoleCheckbox *enable = nullptr;
        ConsoleSelect *cpuSelect = nullptr;
        ConsoleCheckbox *applyToAll = nullptr;
        CPPCThreadWidget *cppcThreadWidget = nullptr;
        QPointer<ConsoleCheckbox> preferOSSettng;
        int prevCpuSelect = 0;
        bool preferOSChecked = true;
        QList<WData> data;

        void updateCPPCWidget(int idx) const;

    public:
        explicit CPPCWidget(const QSet<PWTS::Feature> &cpuFeatures, int threadCount);

        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet);

    private slots:
        void onApplyToAllChanged(Qt::CheckState state) const;
        void onCpuSelectChanged(int idx);
        void onPreferOSStateChanged(Qt::CheckState state);
    };
}
