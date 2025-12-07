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
#include "Widgets/StapmLimitWidget.h"
#include "Widgets/FastLimitWidget.h"
#include "Widgets/SlowLimitWidget.h"
#include "Widgets/TctlTempWidget.h"
#include "Widgets/APUSlowLimitWidget.h"
#include "Widgets/APUSkinTempWidget.h"
#include "Widgets/DGPUSkinTempWidget.h"
#include "Widgets/VRMCurrentWidget.h"
#include "Widgets/VRMSocCurrentWidget.h"
#include "Widgets/VRMMaxCurrentWidget.h"
#include "Widgets/VRMSocMaxCurrentWidget.h"
#include "Widgets/CurveOptimizerAllWidget.h"
#include "Widgets/CurveOptimizerCoreWidget.h"
#include "Widgets/StaticGfxClkWidget.h"
#include "Widgets/MinGfxClockWidget.h"
#include "Widgets/MaxGfxClockWidget.h"
#include "Widgets/PowerProfileWidget.h"

namespace PWT::CUI::AMD {
    class RYCPUTab final: public AddonTab {
        Q_OBJECT

    private:
        QPointer<FastLimitWidget> fastLimitWidget;
        QPointer<SlowLimitWidget> slowLimitWidget;
        QPointer<StapmLimitWidget> stapmLimitWidget;
        QPointer<TctlTempWidget> tctlTempWidget;
        QPointer<APUSlowLimitWidget> apuSlowLimitWidget;
        QPointer<APUSkinTempWidget> apuSkinTempWidget;
        QPointer<DGPUSkinTempWidget> dgpuSkinTempWidget;
        QPointer<VRMCurrentWidget> vrmCurrentWidget;
        QPointer<VRMSocCurrentWidget> vrmSocCurrentWidget;
        QPointer<VRMMaxCurrentWidget> vrmMaxCurrentWidget;
        QPointer<VRMSocMaxCurrentWidget> vrmSocMaxCurrentWidget;
        QPointer<CurveOptimizerAllWidget> curveOptimizerAllWidget;
        QPointer<CurveOptimizerCoreWidget> curveOptimizerCoreWidget;
        QPointer<StaticGfxClkWidget> staticGfxClkWidget;
        QPointer<MinGfxClockWidget> minGfxClockWidget;
        QPointer<MaxGfxClockWidget> maxGfxClockWidget;
        QPointer<PowerProfileWidget> powerProfileWidget;

    public:
        explicit RYCPUTab(const PWTS::DeviceInfoPacket &packet);

        void refreshTab(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(PWTS::ClientPacket &packet) const override;

    private slots:
        void onFastLimitChanged(int v) const;
        void onSlowLimitChanged(int v) const;
    };
}
