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

#include <QObject>

namespace PWT {
    Q_NAMESPACE

    enum GamepadButton {
        SOUTH = 0,
        EAST,
        WEST,
        NORTH,
        DPAD_UP,
        DPAD_DOWN,
        DPAD_LEFT,
        DPAD_RIGTH,
        SELECT,
        START,
        HOME,
        MISC,
        UP_LEFT_PADDLE,
        LOW_LEFT_PADDLE,
        UP_RIGHT_PADDLE,
        LOW_RIGHT_PADDLE,
        L1,
        L2,
        L3,
        R1,
        R2,
        R3,
        ANALOG_LEFT_UP,
        ANALOG_LEFT_DOWN,
        ANALOG_LEFT_LEFT,
        ANALOG_LEFT_RIGHT,
        ANALOG_RIGHT_UP,
        ANALOG_RIGHT_DOWN,
        ANALOG_RIGHT_LEFT,
        ANALOG_RIGHT_RIGHT,

        // combos
        COMBO_L3_R3,
        COMBO_START_SELECT,
        COMBO_L3_START,
        COMBO_R3_SELECT,
        COMBO_L1_SELECT,
        COMBO_L1_START,
        COMBO_L1_DOWN_WEST,
        COMBO_L1_SELECT_WEST,

        UNKNOWN
    };

    Q_ENUM_NS(GamepadButton)
}
