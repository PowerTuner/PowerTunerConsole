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

#include <QElapsedTimer>
#include <QHash>

#include "../external/SDL/include/SDL3/SDL_gamepad.h"
#include "../Include/GamepadButton.h"

namespace PWT::CUI {
    class GamepadWorker final: public QObject {
        Q_OBJECT

    private:
        struct AxisState final {
            int leftX = 0;
            int leftY = 0;
            int rightX = 0;
            int rightY = 0;
        };

        struct Gamepad final {
            SDL_Gamepad *pad = nullptr;
            int deadzone = 8000;
            QString id;
        };

        static constexpr int holdTimeout = 500;
        static constexpr int holdAxisTimeout = 100;
        static constexpr int repeatTimeout = 50;
        static constexpr int repeatAxisTimeout = 30;
        QList<std::tuple<uint32_t, uint8_t, uint32_t>> ignoreList;
        QHash<SDL_JoystickID, Gamepad> sdlGamepadMap;
        GamepadButton heldButton = GamepadButton::UNKNOWN;
        SDL_JoystickID activeID = 0;
        bool allGamepadsMode = false;
        bool hasFocus = false;
        QElapsedTimer etimer;
        AxisState axisState;
        int heldTimeout;
        int heldRepeatTimeout;
        QString heldID;

        [[nodiscard]] QString getGamepadLabel(SDL_JoystickID id);
        [[nodiscard]] QString getGamepadID(SDL_JoystickID id) const;
        [[nodiscard]] bool isDuplicateGamepadID(const QString &gamepadID) const;
        [[nodiscard]] SDL_JoystickID getJoystickID(const QString &gamepadID) const;
        void setHeldButton(GamepadButton button, int timeout, int repeatTimeout);
        [[nodiscard]] bool isDeadzone(int axis, int deadzone) const;
        [[nodiscard]] bool isComboPressed(SDL_Gamepad *pad, SDL_GamepadButton btn1, SDL_GamepadButton btn2, SDL_GamepadButton btn3 = SDL_GAMEPAD_BUTTON_INVALID) const;
        [[nodiscard]] bool ignoreEvent(uint32_t evtType, uint8_t id, uint32_t button);
        [[nodiscard]] bool tryButtonCombo(SDL_JoystickID id);

    public:
        ~GamepadWorker() override;

    public slots:
        void startSDLEventsThread();
        void setAllGamepadsMode(bool enable);
        void setActiveID(const QString &gamepadID);
        void setDeadzone(const QString &gamepadID, int deadzone);
        void setGamepadsDeadzoneData(const QList<std::pair<QString, int>> &data);
        void focusChange(bool focused);

    signals:
        void logMessageSent(const QString &msg);
        void gamepadConnected(const QString &gamepadLbl, const QString &gamepadID);
        void gamepadDisconnected(const QString &gamepadID);
        void gamepadButton(const QString &id, PWT::GamepadButton button);
    };
}
