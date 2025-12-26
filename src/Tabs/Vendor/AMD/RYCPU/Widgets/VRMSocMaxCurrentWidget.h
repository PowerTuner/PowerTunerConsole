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

#include "../Include/RADJSliderWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::AMD {
    class VRMSocMaxCurrentWidget final: public RADJSliderWidget {
    public:
        explicit VRMSocMaxCurrentWidget(const bool hasReadFeature): RADJSliderWidget("VRM SoC Maximum Current Limit",
                                                "Amps",
                                                [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); },
                                                hasReadFeature) {}

        void setData(const PWTS::DaemonPacket &packet) override {
            setEnabled(packet.amdData->vrmSocMaxCurrent.isValid());

            if (!isEnabled()) {
                UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
                return;
            }

            if (!enableChk.isNull()) {
                const QSignalBlocker sblock {enableChk};

                enableChk->setChecked(packet.hasProfileData ? !packet.amdData->vrmSocMaxCurrent.isIgnored() : enableChecked);
            }

            slider->setValue(packet.amdData->vrmSocMaxCurrent.getValue());
        }

        void setDataForPacket(const PWTS::ClientPacket &packet) const override {
            if (!isEnabled())
                return;

            packet.amdData->vrmSocMaxCurrent.setValue(slider->getValue(), true, !enableChk.isNull() && !enableChk->isChecked());
        }
    };
}
