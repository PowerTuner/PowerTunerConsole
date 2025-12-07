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

#include "../Include/SliderLimitWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::AMD {
    class FastLimitWidget final: public SliderLimitWidget {
    public:
        FastLimitWidget(): SliderLimitWidget("Actual Power Limit",
                                        "Watts",
                                        [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); }) {}

        void setData(const PWTS::DaemonPacket &packet) override {
            setEnabled(packet.amdData->fastLimit.isValid());

            if (!isEnabled()) {
                UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
                return;
            }

            const int val = packet.amdData->fastLimit.getValue();

            if (val >= 0)
                slider->setValue(val);
        }

        void setDataForPacket(const PWTS::ClientPacket &packet) const override {
            if (!isEnabled())
                return;

            packet.amdData->fastLimit.setValue(slider->getValue(), true);
        }
    };
}
