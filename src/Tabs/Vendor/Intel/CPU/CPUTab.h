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

#include "../../../Include/AddonTab.h"
#include "pwtShared/Include/Packets/DeviceInfoPacket.h"
#include "Widgets/PowerLimitWidget.h"
#include "Widgets/VRCurrentConfigWidget.h"
#include "Widgets/PP1ConfigWidget.h"
#include "Widgets/TurboPowerCurrentWidget.h"
#include "Widgets/CPUPowerBalanceWidget.h"
#include "Widgets/GPUPowerBalanceWidget.h"
#include "Widgets/EnergyPerformanceBiasWidget.h"
#include "Widgets/TurboRatioLimitWidget.h"
#include "Widgets/MiscProcessorFeaturesWidget.h"
#include "Widgets/MiscPowerManagement/MiscPowerManagementWidget.h"
#include "Widgets/PowerCtl/PowerCtlWidget.h"
#include "Widgets/FIVRControlWidget.h"
#include "Widgets/PKGCSTConfigControl/PKGCSTConfigControlWidget.h"

namespace PWT::CUI::INTEL {
    class CPUTab final: public AddonTab {
    private:
        QPointer<PowerLimitWidget> powerLimitWidget;
        QPointer<VRCurrentConfigWidget> vrCurrentConfigWidget;
        QPointer<PP1ConfigWidget> pp1ConfigWidget;
        QPointer<TurboPowerCurrentWidget> turboPowerCurrentWidget;
        QPointer<CPUPowerBalanceWidget> cpuPowerBalanceWidget;
        QPointer<GPUPowerBalanceWidget> gpuPowerBalanceWidget;
        QPointer<EnergyPerformanceBiasWidget> energyPerformanceBiasWidget;
        QPointer<TurboRatioLimitWidget> turboRatioLimitWidget;
        QPointer<MiscProcessorFeaturesWidget> miscProcessorFeaturesWidget;
        QPointer<MiscPowerManagementWidget> miscPowerManagementWidget;
        QPointer<PowerCtlWidget> powerCtlWidget;
        QPointer<FIVRControlWidget> fivrControlWidget;
        QPointer<PKGCSTConfigControlWidget> pkgCstConfigControlWidget;

    public:
        explicit CPUTab(const PWTS::DeviceInfoPacket &packet);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;
    };
}
