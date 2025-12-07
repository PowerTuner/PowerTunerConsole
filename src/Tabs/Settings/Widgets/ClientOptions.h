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

#include "../../../Classes/ConsoleSettings.h"
#include "../../Widgets/ConsoleSelect.h"
#include "../../Widgets/ConsoleButton.h"
#include "../../Widgets/ConsoleSlider.h"
#include "../../Widgets/ConsoleCheckbox.h"

namespace PWT::CUI {
    class ClientOptions final: public QWidget {
        Q_OBJECT

    private:
        const QMap<Qt::Key, QString> keyMap = {
            {Qt::Key_A, "A"},
            {Qt::Key_B, "B"},
            {Qt::Key_C, "C"},
            {Qt::Key_D, "D"},
            {Qt::Key_E, "E"},
            {Qt::Key_F, "F"},
            {Qt::Key_G, "G"},
            {Qt::Key_H, "H"},
            {Qt::Key_I, "I"},
            {Qt::Key_J, "J"},
            {Qt::Key_K, "K"},
            {Qt::Key_L, "L"},
            {Qt::Key_M, "M"},
            {Qt::Key_N, "N"},
            {Qt::Key_O, "O"},
            {Qt::Key_P, "P"},
            {Qt::Key_Q, "Q"},
            {Qt::Key_R, "R"},
            {Qt::Key_S, "S"},
            {Qt::Key_T, "T"},
            {Qt::Key_U, "U"},
            {Qt::Key_V, "V"},
            {Qt::Key_W, "W"},
            {Qt::Key_X, "X"},
            {Qt::Key_Y, "Y"},
            {Qt::Key_Z, "Z"},
            {Qt::Key_0, "0"},
            {Qt::Key_1, "1"},
            {Qt::Key_2, "2"},
            {Qt::Key_3, "3"},
            {Qt::Key_4, "4"},
            {Qt::Key_5, "5"},
            {Qt::Key_6, "6"},
            {Qt::Key_7, "7"},
            {Qt::Key_8, "8"},
            {Qt::Key_9, "9"},
            {Qt::Key_Up, "UP"},
            {Qt::Key_Down, "DOWN"},
            {Qt::Key_Left, "LEFT"},
            {Qt::Key_Right, "RIGHT"},
            {Qt::Key_Escape, "ESC"},
            {Qt::Key_Space, "SPACE"},
            {Qt::Key_F1, "F1"},
            {Qt::Key_F2, "F2"},
            {Qt::Key_F3, "F3"},
            {Qt::Key_F4, "F4"},
            {Qt::Key_F5, "F5"},
            {Qt::Key_F6, "F6"},
            {Qt::Key_F7, "F7"},
            {Qt::Key_F8, "F8"},
            {Qt::Key_F9, "F9"},
            {Qt::Key_F10, "F10"},
            {Qt::Key_F11, "F11"},
            {Qt::Key_F12, "F12"},
            {Qt::Key_Less, "<"},
            {Qt::Key_Greater, ">"},
            {Qt::Key_PageUp, "PAGE UP"},
            {Qt::Key_PageDown, "PAGE DOWN"}
        };
        const QMap<GamepadButton, QString> buttonMap = {
            {GamepadButton::DPAD_UP, "DPAD UP"},
            {GamepadButton::DPAD_DOWN, "DPAD DOWN"},
            {GamepadButton::DPAD_LEFT, "DPAD LEFT"},
            {GamepadButton::DPAD_RIGTH, "DPAD RIGHT"},
            {GamepadButton::SOUTH, "A (XBox) / X (PS) / B (Nintendo)"},
            {GamepadButton::EAST, "B (XBox) / O (PS) / A (Nintendo)"},
            {GamepadButton::WEST, "X (XBox) / [] (PS) / Y (Nintendo)"},
            {GamepadButton::NORTH, "Y (XBox) / /\\ (PS) / X (Nintendo)"},
            {GamepadButton::L1, "L1"},
            {GamepadButton::L2, "L2"},
            {GamepadButton::L3, "L3"},
            {GamepadButton::R1, "R1"},
            {GamepadButton::R2, "R2"},
            {GamepadButton::R3, "R3"},
            {GamepadButton::SELECT, "SELECT"},
            {GamepadButton::START, "START"},
            {GamepadButton::HOME, "HOME"},
            {GamepadButton::MISC, "MISC (Additional button, eg. share/capture/mic. button)"},
            {GamepadButton::UP_LEFT_PADDLE, "UPPER LEFT PADDLE"},
            {GamepadButton::LOW_LEFT_PADDLE, "LOWER LEFT PADDLE"},
            {GamepadButton::UP_RIGHT_PADDLE, "UPPER RIGHT PADDLE"},
            {GamepadButton::LOW_RIGHT_PADDLE, "LOWER RIGHT PADDLE"},
            {GamepadButton::ANALOG_LEFT_UP, "LEFT ANALOG UP"},
            {GamepadButton::ANALOG_LEFT_DOWN, "LEFT ANALOG DOWN"},
            {GamepadButton::ANALOG_LEFT_LEFT, "LEFT ANALOG LEFT"},
            {GamepadButton::ANALOG_LEFT_RIGHT, "LEFT ANALOG RIGHT"},
            {GamepadButton::ANALOG_RIGHT_UP, "RIGHT ANALOG UP"},
            {GamepadButton::ANALOG_RIGHT_DOWN, "RIGHT ANALOG DOWN"},
            {GamepadButton::ANALOG_RIGHT_LEFT, "RIGHT ANALOG LEFT"},
            {GamepadButton::ANALOG_RIGHT_RIGHT, "RIGHT ANALOG RIGHT"}
        };
        const QMap<GamepadButton, QString> comboMap = {
            {GamepadButton::UNKNOWN, "DISABLED"},
            {GamepadButton::COMBO_L3_R3, "L3 + R3"},
            {GamepadButton::COMBO_START_SELECT, "START + SELECT"},
            {GamepadButton::COMBO_L1_DOWN_WEST, "L1 + DPAD DOWN + X (XBox) / [] (PS) / Y (Nintendo)"},
            {GamepadButton::COMBO_L1_SELECT, "L1 + SELECT"},
            {GamepadButton::COMBO_L1_START, "L1 + START"},
            {GamepadButton::COMBO_L3_START, "L3 + START"},
            {GamepadButton::COMBO_R3_SELECT, "R3 + SELECT"}
        };
        QSharedPointer<ConsoleSettings> consoleSettings;
        QHash<QString, GamepadMap> gamepadMapsCache;
        QList<QString> gamepadIDCacheList;
        ConsoleSelect *appQuitBehaviour = nullptr;
        ConsoleSelect *floatIconSize = nullptr;
        ConsoleSelect *autoConnectDaemon = nullptr;
        ConsoleCheckbox *enableTTS = nullptr;
        ConsoleCheckbox *rememberWindowState = nullptr;
        ConsoleCheckbox *runAtStartup = nullptr;
        ConsoleSelect *prevTabKB = nullptr;
        ConsoleSelect *nextTabKB = nullptr;
        ConsoleSelect *leftKB = nullptr;
        ConsoleSelect *fastLeftKB = nullptr;
        ConsoleSelect *rightKB = nullptr;
        ConsoleSelect *fastRightKB = nullptr;
        ConsoleSelect *upKB = nullptr;
        ConsoleSelect *downKB = nullptr;
        ConsoleSelect *clickKB = nullptr;
        ConsoleSelect *backKB = nullptr;
        ConsoleSelect *reloadKB = nullptr;
        ConsoleSelect *applyKB = nullptr;
        ConsoleSelect *inputModeKB = nullptr;
        ConsoleSelect *scrollUpKB = nullptr;
        ConsoleSelect *scrollDownKB = nullptr;
        ConsoleSelect *scrollLeftKB = nullptr;
        ConsoleSelect *scrollRightKB = nullptr;
        ConsoleSelect *scrollViewUpKB = nullptr;
        ConsoleSelect *scrollViewDownKB = nullptr;
        ConsoleSelect *scrollViewTopKB = nullptr;
        ConsoleSelect *scrollViewBottomKB = nullptr;
        ConsoleSelect *gamepad = nullptr;
        ConsoleCheckbox *allGamepadsUIControl = nullptr;
        ConsoleSelect *prevTabGP = nullptr;
        ConsoleSelect *nextTabGP = nullptr;
        ConsoleSelect *leftGP = nullptr;
        ConsoleSelect *fastLeftGP = nullptr;
        ConsoleSelect *rightGP = nullptr;
        ConsoleSelect *fastRightGP = nullptr;
        ConsoleSelect *upGP = nullptr;
        ConsoleSelect *downGP = nullptr;
        ConsoleSelect *clickGP = nullptr;
        ConsoleSelect *backGP = nullptr;
        ConsoleSelect *reloadGP = nullptr;
        ConsoleSelect *applyGP = nullptr;
        ConsoleSelect *inputModeGP = nullptr;
        ConsoleSelect *scrollUpGP = nullptr;
        ConsoleSelect *scrollDownGP = nullptr;
        ConsoleSelect *scrollLeftGP = nullptr;
        ConsoleSelect *scrollRightGP = nullptr;
        ConsoleSelect *scrollViewUpGP = nullptr;
        ConsoleSelect *scrollViewDownGP = nullptr;
        ConsoleSelect *scrollViewTopGP = nullptr;
        ConsoleSelect *scrollViewBottomGP = nullptr;
        ConsoleSelect *appCombo = nullptr;
        ConsoleSlider *deadzone = nullptr;
        ConsoleButton *reloadMappingsBtn = nullptr;
        ConsoleButton *saveBnt = nullptr;
        ConsoleButton *resetDefaultBtn = nullptr;
        int prevGamepadIdx = -1;

        void loadGamepadMappings(const QString &gamepadID) const;
        void updateGamepadMappings(const QString &gamepadID);
        void enableGamepadSettings(bool hasGamepads) const;

    public:
        ClientOptions();

        void initAutoConnectDaemonList() const;
        void loadSettings() const;
        void addGamepad(const QString &gamepadName, const QString &gamepadID);
        void removeGamepad(const QString &gamepadID);
        [[nodiscard]] QString getFirstGamepadID() const;

    private slots:
        void onAppQuitBehaviourChanged(int idx) const;
        void onGamepadChanged(int idx);
        void onReloadGamepadMappingsBtnClicked();
        void onSaveBtnClicked();
        void onResetDefaultBtnClicked();

    signals:
        void reloadGamepadMappings();
        void clientSettingsSaved();
    };
}
