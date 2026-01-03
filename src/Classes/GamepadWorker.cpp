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
#include <QCoreApplication>
#include <QThread>

#include "../external/SDL/include/SDL3/SDL_init.h"
#include "../external/SDL/include/SDL3/SDL_hints.h"
#include "../external/SDL/include/SDL3/SDL_events.h"
#include "GamepadWorker.h"

namespace PWT::CUI {
    GamepadWorker::~GamepadWorker() {
        for (auto [jid, gpad]: sdlGamepadMap.asKeyValueRange())
            SDL_CloseGamepad(gpad.pad);

        SDL_Quit();
    }

    QString GamepadWorker::getGamepadLabel(const SDL_JoystickID id) {
        const char *name = SDL_GetGamepadNameForID(id);
        QString label;

        if (name != nullptr) {
            label = name;

        } else {
            const SDL_GamepadType gtype = SDL_GetGamepadTypeForID(id);
            const char *typeStr = SDL_GetGamepadStringForType(gtype);

            if (gtype == SDL_GAMEPAD_TYPE_UNKNOWN || typeStr == nullptr) {
                emit logMessageSent(QStringLiteral("onGamepadConnected: unknown gamepad type, skip.."));
                return "";
            }

            label = typeStr;
        }

        return QString("%1 (%2)").arg(label).arg(id);
    }

    QString GamepadWorker::getGamepadID(const SDL_JoystickID id) const {
        char buff[34] = {0};
        int guidNo = 0;
        QString gamepadID;

        SDL_GUIDToString(SDL_GetGamepadGUIDForID(id), buff, 34);

        while (true) {
            gamepadID = QString("%1%2").arg(guidNo++).arg(buff);

            if (!isDuplicateGamepadID(gamepadID))
                break;
        }

        return gamepadID;
    }

    bool GamepadWorker::isDuplicateGamepadID(const QString &gamepadID) const {
        for (const auto &[jid, gpad]: sdlGamepadMap.asKeyValueRange()) {
            if (gpad.id == gamepadID)
                return true;
        }

        return false;
    }

    SDL_JoystickID GamepadWorker::getJoystickID(const QString &gamepadID) const {
        if (gamepadID.isEmpty())
            return 0;

        for (const auto &[jid, gpad]: sdlGamepadMap.asKeyValueRange()) {
            if (gpad.id == gamepadID)
                return jid;
        }

        return 0;
    }

    void GamepadWorker::setHeldButton(const GamepadButton button, const int timeout, const int repeatTimeout) {
        heldButton = button;
        heldTimeout = timeout;
        heldRepeatTimeout = repeatTimeout;
        etimer.start();
    }

    bool GamepadWorker::isDeadzone(const int axis, const int deadzone) const {
        return axis >= -deadzone && axis <= deadzone;
    }

    bool GamepadWorker::isComboPressed(SDL_Gamepad *pad, const SDL_GamepadButton btn1, const SDL_GamepadButton btn2, const SDL_GamepadButton btn3) const {
        bool res = SDL_GetGamepadButton(pad, btn1) && SDL_GetGamepadButton(pad, btn2);

        if (btn3 != SDL_GAMEPAD_BUTTON_INVALID)
            res = res && SDL_GetGamepadButton(pad, btn3);

        return res;
    }

    bool GamepadWorker::ignoreEvent(const uint32_t evtType, const uint8_t id, const uint32_t button) {
        if (ignoreList.isEmpty()) [[likely]]
            return false;

        for (const auto &[type, joyID, btn]: ignoreList) {
            if (type != evtType || joyID != id || btn != button)
                continue;

            ignoreList.removeIf([&evtType, &id, &button](const std::tuple<uint32_t, uint8_t, uint32_t> &itm)->bool {
                return std::get<0>(itm) == evtType && std::get<1>(itm) == id && std::get<2>(itm) == button;
            });
            return true;
        }

        return false;
    }

    bool GamepadWorker::tryButtonCombo(const SDL_JoystickID id) {
        const QString gamepadID = sdlGamepadMap[id].id;
        SDL_Gamepad *pad = sdlGamepadMap[id].pad;

        if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_LEFT_STICK, SDL_GAMEPAD_BUTTON_RIGHT_STICK)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_L3_R3);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_LEFT_STICK));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_RIGHT_STICK));
            return true;

        } else if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_START, SDL_GAMEPAD_BUTTON_BACK)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_START_SELECT);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_START));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_BACK));
            return true;

        } else if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_LEFT_STICK, SDL_GAMEPAD_BUTTON_START)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_L3_START);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_LEFT_STICK));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_START));
            return true;

        } else if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_RIGHT_STICK, SDL_GAMEPAD_BUTTON_BACK)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_R3_SELECT);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_RIGHT_STICK));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_BACK));
            return true;

        } else if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_BUTTON_BACK)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_L1_SELECT);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_BACK));
            return true;

        } else if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_BUTTON_START)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_L1_START);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_START));
            return true;

        } else if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_WEST)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_L1_DOWN_WEST);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_DPAD_DOWN));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_WEST));
            return true;

        } else if (isComboPressed(pad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_BUTTON_BACK, SDL_GAMEPAD_BUTTON_WEST)) {
            heldButton = GamepadButton::UNKNOWN;

            emit gamepadButton(gamepadID, GamepadButton::COMBO_L1_SELECT_WEST);
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_BACK));
            ignoreList.append(std::make_tuple(SDL_EVENT_GAMEPAD_BUTTON_UP, id, SDL_GAMEPAD_BUTTON_WEST));
            return true;
        }

        return false;
    }

    void GamepadWorker::startSDLEventsThread() {
        SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "0");

        if (!SDL_Init(SDL_INIT_GAMEPAD)) {
            emit logMessageSent("Failed to init SDL gamepad subsystem");
            emit initFail();
            return;
        }

        while (true) {
            SDL_Event evt;
            const bool hasEvent = SDL_PollEvent(&evt);

            if (hasEvent && evt.type == SDL_EVENT_QUIT) [[unlikely]]
                break;

            if (heldButton != GamepadButton::UNKNOWN && etimer.hasExpired(heldTimeout)) [[unlikely]] {
                heldTimeout = heldRepeatTimeout;
                emit gamepadButton(heldID, heldButton);
                etimer.start();
            }

            QCoreApplication::processEvents();

            if (!hasEvent) [[likely]] {
                QThread::msleep(15);
                continue;
            }

            switch (evt.type) {
                case SDL_EVENT_GAMEPAD_ADDED: {
                    // sdl may send multiple events, skip if existing
                    if (sdlGamepadMap.contains(evt.gdevice.which)) [[unlikely]]
                        break;

                    const SDL_JoystickID id = evt.gdevice.which;
                    const QString gamepadLbl = getGamepadLabel(id);
                    const QString gamepadID = getGamepadID(id);
                    SDL_Gamepad *pad = nullptr;

                    if (gamepadLbl.isEmpty()) [[unlikely]]
                        break;

                    if (allGamepadsMode) {
                        pad = SDL_OpenGamepad(id);

                        if (pad == nullptr) {
                            emit logMessageSent(QString("Gamepad connection error: %1").arg(SDL_GetError()));
                            SDL_ClearError();
                            break;
                        }
                    }

                    sdlGamepadMap.insert(id, {
                        .pad = pad,
                        .id = gamepadID,
                    });

                    emit gamepadConnected(gamepadLbl, gamepadID);
                }
                    break;
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    if (!sdlGamepadMap.contains(evt.gdevice.which)) [[unlikely]]
                        break;

                    const Gamepad gpad = sdlGamepadMap.take(evt.gdevice.which);

                    SDL_CloseGamepad(gpad.pad);
                    emit gamepadDisconnected(gpad.id);
                }
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                    const SDL_JoystickID jid = evt.gbutton.which;

                    if (ignoreEvent(evt.type, jid, evt.gbutton.button) || !hasFocus) {
                        heldButton = GamepadButton::UNKNOWN;
                        break;
                    }

                    const QString id = sdlGamepadMap[jid].id;

                    switch (evt.gbutton.button) {
                        case SDL_GAMEPAD_BUTTON_SOUTH:
                            emit gamepadButton(id, GamepadButton::SOUTH);
                            break;
                        case SDL_GAMEPAD_BUTTON_EAST:
                            emit gamepadButton(id, GamepadButton::EAST);
                            break;
                        case SDL_GAMEPAD_BUTTON_WEST:
                            emit gamepadButton(id, GamepadButton::WEST);
                            break;
                        case SDL_GAMEPAD_BUTTON_NORTH:
                            emit gamepadButton(id, GamepadButton::NORTH);
                            break;
                        case SDL_GAMEPAD_BUTTON_START:
                            emit gamepadButton(id, GamepadButton::START);
                            break;
                        case SDL_GAMEPAD_BUTTON_BACK:
                            emit gamepadButton(id, GamepadButton::SELECT);
                            break;
                        case SDL_GAMEPAD_BUTTON_GUIDE:
                            emit gamepadButton(id, GamepadButton::HOME);
                            break;
                        case SDL_GAMEPAD_BUTTON_MISC1:
                            emit gamepadButton(id, GamepadButton::MISC);
                            break;
                        case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1:
                            emit gamepadButton(id, GamepadButton::UP_LEFT_PADDLE);
                            break;
                        case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2:
                            emit gamepadButton(id, GamepadButton::LOW_LEFT_PADDLE);
                            break;
                        case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1:
                            emit gamepadButton(id, GamepadButton::UP_RIGHT_PADDLE);
                            break;
                        case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2:
                            emit gamepadButton(id, GamepadButton::LOW_RIGHT_PADDLE);
                            break;
                        case SDL_GAMEPAD_BUTTON_LEFT_STICK:
                            emit gamepadButton(id, GamepadButton::L3);
                            break;
                        case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
                            emit gamepadButton(id, GamepadButton::R3);
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_UP:
                        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
                        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
                        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
                        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
                        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
                            heldButton = GamepadButton::UNKNOWN;
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                    const SDL_JoystickID jid = evt.gbutton.which;

                    if (tryButtonCombo(jid) || !hasFocus)
                        break;

                    const QString id = sdlGamepadMap[jid].id;

                    heldID = id;

                    switch (evt.gbutton.button) {
                        case SDL_GAMEPAD_BUTTON_DPAD_UP: {
                            emit gamepadButton(id, GamepadButton::DPAD_UP);
                            setHeldButton(GamepadButton::DPAD_UP, holdTimeout, repeatTimeout);
                        }
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_DOWN: {
                            emit gamepadButton(id, GamepadButton::DPAD_DOWN);
                            setHeldButton(GamepadButton::DPAD_DOWN, holdTimeout, repeatTimeout);
                        }
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_LEFT: {
                            emit gamepadButton(id, GamepadButton::DPAD_LEFT);
                            setHeldButton(GamepadButton::DPAD_LEFT, holdTimeout, repeatTimeout);
                        }
                            break;
                        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: {
                            emit gamepadButton(id, GamepadButton::DPAD_RIGTH);
                            setHeldButton(GamepadButton::DPAD_RIGTH, holdTimeout, repeatTimeout);
                        }
                            break;
                        case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: {
                            emit gamepadButton(id, GamepadButton::L1);
                            setHeldButton(GamepadButton::L1, holdTimeout, repeatTimeout);
                        }
                            break;
                        case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: {
                            emit gamepadButton(id, GamepadButton::R1);
                            setHeldButton(GamepadButton::R1, holdTimeout, repeatTimeout);
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                    if (!hasFocus)
                        break;

                    const SDL_JoystickID jid = evt.gaxis.which;
                    const QString id = sdlGamepadMap[jid].id;

                    heldID = id;

                    switch (evt.gaxis.axis) {
                        case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: {
                            if (evt.gaxis.value < SDL_JOYSTICK_AXIS_MAX) {
                                heldButton = GamepadButton::UNKNOWN;

                            } else if (evt.gaxis.value == SDL_JOYSTICK_AXIS_MAX) {
                                emit gamepadButton(id, GamepadButton::L2);
                                setHeldButton(GamepadButton::L2, holdAxisTimeout, repeatAxisTimeout);
                            }
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: {
                            if (evt.gaxis.value < SDL_JOYSTICK_AXIS_MAX) {
                                heldButton = GamepadButton::UNKNOWN;

                            } else if (evt.gaxis.value == SDL_JOYSTICK_AXIS_MAX) {
                                emit gamepadButton(id, GamepadButton::R2);
                                setHeldButton(GamepadButton::R2, holdAxisTimeout, repeatAxisTimeout);
                            }
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_LEFTX: {
                            const int deadzone = sdlGamepadMap[jid].deadzone;
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value, deadzone);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.leftX, deadzone);

                            axisState.leftX = std::abs(evt.gaxis.value);

                            if (axisState.leftY > axisState.leftX || hasDrift)
                                break;

                            if (insideDeadzone) {
                                heldButton = GamepadButton::UNKNOWN;

                            } else if (evt.gaxis.value > deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_LEFT_RIGHT);
                                setHeldButton(GamepadButton::ANALOG_LEFT_RIGHT, holdAxisTimeout, repeatAxisTimeout);

                            } else if (evt.gaxis.value < -deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_LEFT_LEFT);
                                setHeldButton(GamepadButton::ANALOG_LEFT_LEFT, holdAxisTimeout, repeatAxisTimeout);
                            }
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_LEFTY: {
                            const int deadzone = sdlGamepadMap[jid].deadzone;
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value, deadzone);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.leftY, deadzone);

                            axisState.leftY = std::abs(evt.gaxis.value);

                            if (axisState.leftX > axisState.leftY || hasDrift)
                                break;

                            if (insideDeadzone) {
                                heldButton = GamepadButton::UNKNOWN;

                            } else if (evt.gaxis.value > deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_LEFT_DOWN);
                                setHeldButton(GamepadButton::ANALOG_LEFT_DOWN, holdAxisTimeout, repeatAxisTimeout);

                            } else if (evt.gaxis.value < -deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_LEFT_UP);
                                setHeldButton(GamepadButton::ANALOG_LEFT_UP, holdAxisTimeout, repeatAxisTimeout);
                            }
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_RIGHTX: {
                            const int deadzone = sdlGamepadMap[jid].deadzone;
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value, deadzone);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.rightX, deadzone);

                            axisState.rightX = std::abs(evt.gaxis.value);

                            if (axisState.rightY > axisState.rightX || hasDrift)
                                break;

                            if (insideDeadzone) {
                                heldButton = GamepadButton::UNKNOWN;

                            } else if (evt.gaxis.value > deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_RIGHT_RIGHT);
                                setHeldButton(GamepadButton::ANALOG_RIGHT_RIGHT, holdAxisTimeout, repeatAxisTimeout);

                            } else if (evt.gaxis.value < -deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_RIGHT_LEFT);
                                setHeldButton(GamepadButton::ANALOG_RIGHT_LEFT, holdAxisTimeout, repeatAxisTimeout);
                            }
                        }
                            break;
                        case SDL_GAMEPAD_AXIS_RIGHTY: {
                            const int deadzone = sdlGamepadMap[jid].deadzone;
                            const bool insideDeadzone = isDeadzone(evt.gaxis.value, deadzone);
                            const bool hasDrift = insideDeadzone && isDeadzone(axisState.rightY, deadzone);

                            axisState.rightY = std::abs(evt.gaxis.value);

                            if (axisState.rightX > axisState.rightY || hasDrift)
                                break;

                            if (insideDeadzone) {
                                heldButton = GamepadButton::UNKNOWN;

                            } else if (evt.gaxis.value > deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_RIGHT_DOWN);
                                setHeldButton(GamepadButton::ANALOG_RIGHT_DOWN, holdAxisTimeout, repeatAxisTimeout);

                            } else if (evt.gaxis.value < -deadzone) {
                                emit gamepadButton(id, GamepadButton::ANALOG_RIGHT_UP);
                                setHeldButton(GamepadButton::ANALOG_RIGHT_UP, holdAxisTimeout, repeatAxisTimeout);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }

    void GamepadWorker::setAllGamepadsMode(const bool enable) {
        allGamepadsMode = enable;

        for (const auto &[jid, gpad]: sdlGamepadMap.asKeyValueRange()) {
            if (allGamepadsMode) {
                gpad.pad = SDL_OpenGamepad(jid);

                if (gpad.pad == nullptr) {
                    emit logMessageSent(QString("AllGamepadsMode: failed to open gamepad: %1").arg(SDL_GetError()));
                    SDL_ClearError();
                }
            } else {
                SDL_CloseGamepad(gpad.pad);
                gpad.pad = nullptr;
            }
        }
    }

    void GamepadWorker::setActiveID(const QString &gamepadID) {
        if (!allGamepadsMode && activeID != 0) {
            Gamepad gpad = sdlGamepadMap[activeID];

            if (gpad.pad != nullptr) {
                SDL_CloseGamepad(gpad.pad);
                gpad.pad = nullptr;
            }
        }

        const SDL_JoystickID jid = getJoystickID(gamepadID);

        if (jid == 0) {
            emit logMessageSent(QStringLiteral("GamepadWorker::setActiveID: failed to set active gamepad"));
            activeID = 0;
            return;
        }

        if (!allGamepadsMode) {
            sdlGamepadMap[jid].pad = SDL_OpenGamepad(jid);

            if (sdlGamepadMap[jid].pad == nullptr) {
                emit logMessageSent(QString("Failed to open gamepad: %1").arg(SDL_GetError()));
                SDL_ClearError();
                activeID = 0;
                return;
            }
        }

        activeID = jid;
    }

    void GamepadWorker::setDeadzone(const QString &gamepadID, const int deadzone) {
        const SDL_JoystickID id = getJoystickID(gamepadID);

        if (id == 0)
            return;

        sdlGamepadMap[id].deadzone = deadzone * 1000;
    }

    void GamepadWorker::setGamepadsDeadzoneData(const QList<std::pair<QString, int>> &data) {
        for (const auto &[id, dz]: data) {
            SDL_JoystickID jid = getJoystickID(id);

            if (jid == 0)
                continue;

            sdlGamepadMap[jid].deadzone = dz * 1000;
        }
    }

    void GamepadWorker::focusChange(const bool focused) {
        heldButton = GamepadButton::UNKNOWN;
        hasFocus = focused;
    }
}
