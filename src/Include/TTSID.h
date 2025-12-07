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

namespace PWT::CUI {
    enum struct TTSID: int {
        STOPPABLE,
        TTS_INIT,
        TAB_PAGE,
        PAD_CONNECTED,
        PAD_DISCONNECTED,
        SHOW_CONSOLE,
        DPACKET_RECEIVED,
        DAEMON_CONNECTED,
        DAEMON_DISCONNECTED,
        DAEMON_CMD_FAIL,
        SERVICE_ERROR,
        SETTINGS_APPLIED,
        DAEMON_DELETED,
        NEW_UI_LOGS,
        CLIENT_SETT_SAVED,
        MINIMIZED_CONSOLE,
        OPEN_DIALOG,
        MICRO_KEYBOARD
    };
}
