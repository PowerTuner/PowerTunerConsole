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

#include "GamepadButton.h"

namespace PWT {
    struct GamepadMap final {
        GamepadButton prevTabGP = GamepadButton::L1;
        GamepadButton nextTabGP = GamepadButton::R1;
        GamepadButton leftGP = GamepadButton::DPAD_LEFT;
        GamepadButton fastLeftGP = GamepadButton::L2;
        GamepadButton rightGP = GamepadButton::DPAD_RIGTH;
        GamepadButton fastRightGP = GamepadButton::R2;
        GamepadButton upGP = GamepadButton::DPAD_UP;
        GamepadButton downGP = GamepadButton::DPAD_DOWN;
        GamepadButton clickGP = GamepadButton::SOUTH;
        GamepadButton backGP = GamepadButton::EAST;
        GamepadButton reloadGP = GamepadButton::START;
        GamepadButton applyGP = GamepadButton::SELECT;
        GamepadButton inputModeGP = GamepadButton::NORTH;
        GamepadButton scrollUpGP = GamepadButton::ANALOG_LEFT_UP;
        GamepadButton scrollDownGP = GamepadButton::ANALOG_LEFT_DOWN;
        GamepadButton scrollLeftGP = GamepadButton::ANALOG_LEFT_LEFT;
        GamepadButton scrollRightGP = GamepadButton::ANALOG_LEFT_RIGHT;
        GamepadButton scrollViewUpGP = GamepadButton::ANALOG_RIGHT_UP;
        GamepadButton scrollViewDownGP = GamepadButton::ANALOG_RIGHT_DOWN;
        GamepadButton scrollViewTopGP = GamepadButton::ANALOG_RIGHT_LEFT;
        GamepadButton scrollViewBottomGP = GamepadButton::ANALOG_RIGHT_RIGHT;
        GamepadButton appComboGP = GamepadButton::COMBO_L1_DOWN_WEST;
        int deadzone = 8;

        friend QDataStream &operator<<(QDataStream &ds, const GamepadMap &data) {
            ds << data.prevTabGP <<
                data.nextTabGP <<
                data.leftGP <<
                data.fastLeftGP <<
                data.rightGP <<
                data.fastRightGP <<
                data.upGP <<
                data.downGP <<
                data.clickGP <<
                data.backGP <<
                data.reloadGP <<
                data.applyGP <<
                data.inputModeGP <<
                data.scrollUpGP <<
                data.scrollDownGP <<
                data.scrollLeftGP <<
                data.scrollRightGP <<
                data.scrollViewUpGP <<
                data.scrollViewDownGP <<
                data.scrollViewTopGP <<
                data.scrollViewBottomGP <<
                data.appComboGP <<
                data.deadzone;

            return ds;
        }

        friend QDataStream &operator>>(QDataStream &ds, GamepadMap &data) {
            ds >> data.prevTabGP >>
                data.nextTabGP >>
                data.leftGP >>
                data.fastLeftGP >>
                data.rightGP >>
                data.fastRightGP >>
                data.upGP >>
                data.downGP >>
                data.clickGP >>
                data.backGP >>
                data.reloadGP >>
                data.applyGP >>
                data.inputModeGP >>
                data.scrollUpGP >>
                data.scrollDownGP >>
                data.scrollLeftGP >>
                data.scrollRightGP >>
                data.scrollViewUpGP >>
                data.scrollViewDownGP >>
                data.scrollViewTopGP >>
                data.scrollViewBottomGP >>
                data.appComboGP >>
                data.deadzone;

            return ds;
        }
    };
}
