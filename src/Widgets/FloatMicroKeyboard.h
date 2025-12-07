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

#include <QLabel>

#include "../Include/GamepadButton.h"
#include "../Include/GamepadMap.h"
#include "../Include/TTSID.h"

namespace PWT::CUI {
    class FloatMicroKeyboard final: public QWidget {
        Q_OBJECT

    private:
        static constexpr std::array<std::string_view, 41> kbKeys = {{
            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
            "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "_", ".", "-", "[  ]", "RET"
        }};
        static constexpr int scrollJump = 10;
        QLabel *microKb = nullptr;
        bool upperCase = false;
        bool hasTTS = false;
        int selectedKey;

        [[nodiscard]] int prevKey(int step = 1);
        [[nodiscard]] int nextKey(int step = 1);
        void saySelectedKey(const std::string_view &key);

    protected:
        void showEvent(QShowEvent *event) override;

    public:
        static constexpr char DeleteKey = '\0';
        static constexpr char BackspaceKey = '\b';
        static constexpr char EnterKey = '\r';

        FloatMicroKeyboard();

        void processGamepadButtonEvent(GamepadButton button, const GamepadMap &mapping);

    signals:
        void writeKey(char c);
        void closeKeyboard();
        void sayTTS(const QString &msg, TTSID id);
        void sayStoppableTTS(const QString &msg);
    };
}
