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

#include <QSharedPointer>
#include <QHash>

#include "../Include/Daemon.h"
#include "../Include/GamepadMap.h"
#include "../Include/AppQuitBehaviour.h"
#include "../Include/FloatIconSize.h"
#include "pwtClientCommon/Include/PWTWindowState.h"

namespace PWT {
    class ConsoleSettings final: public QObject {
        Q_OBJECT

    private:
        struct CData final {
            int version = 1;
            QList<Daemon> daemonList {};
            AppQuitBehaviour appQuitBehaviour = AppQuitBehaviour::Quit;
            Qt::Key prevTabKB = Qt::Key_F1;
            Qt::Key nextTabKB = Qt::Key_F2;
            Qt::Key leftKB = Qt::Key_Left;
            Qt::Key fastLeftKB = Qt::Key_Less;
            Qt::Key rightKB = Qt::Key_Right;
            Qt::Key fastRightKB = Qt::Key_Greater;
            Qt::Key upKB = Qt::Key_Up;
            Qt::Key downKB = Qt::Key_Down;
            Qt::Key clickKB = Qt::Key_X;
            Qt::Key backKB = Qt::Key_Escape;
            Qt::Key reloadKB = Qt::Key_F5;
            Qt::Key applyKB = Qt::Key_F6;
            Qt::Key inputModeKB = Qt::Key_T;
            Qt::Key scrollUpKB = Qt::Key_J;
            Qt::Key scrollDownKB = Qt::Key_K;
            Qt::Key scrollLeftKB = Qt::Key_N;
            Qt::Key scrollRightKB = Qt::Key_M;
            Qt::Key scrollViewUpKB = Qt::Key_PageUp;
            Qt::Key scrollViewDownKB = Qt::Key_PageDown;
            Qt::Key scrollViewTopKB = Qt::Key_F3;
            Qt::Key scrollViewBottomKB = Qt::Key_F4;
            QHash<QString, GamepadMap> gamepadMappings {};
            QString preferredGamepadID {};
            bool allGamepadsUIControl = true;
            QPointF floatIconPosition {0, 0};
            FloatIconSize floatIconSize = FloatIconSize::Small;
            bool rememberWindowState = true;
            UI::PWTWindowState windowState {};
            QString autoConnectDaemonName;
            bool enableTTS = true;
        };

        static inline const QString signature {"PWTCS"};
        static inline QSharedPointer<ConsoleSettings> instance;
        QString path;
        CData _data;

        friend QDataStream &operator<<(QDataStream &ds, const CData &cdata);
        friend QDataStream &operator>>(QDataStream &ds, CData &cdata);

        ConsoleSettings() = default;

        Daemon defaultDaemon() const;

    public:
        ConsoleSettings(const ConsoleSettings &) = delete;
        ConsoleSettings &operator=(const ConsoleSettings &) = delete;

        [[nodiscard]] QList<Daemon> getDaemonList() const { return _data.daemonList; }
        [[nodiscard]] AppQuitBehaviour getAppQuitBehaviour() const { return _data.appQuitBehaviour; }
        [[nodiscard]] Qt::Key getPrevTabKB() const { return _data.prevTabKB; }
        [[nodiscard]] Qt::Key getNextTabKB() const { return _data.nextTabKB; }
        [[nodiscard]] Qt::Key getLeftKB() const { return _data.leftKB; }
        [[nodiscard]] Qt::Key getFastLeftKB() const { return _data.fastLeftKB; }
        [[nodiscard]] Qt::Key getRightKB() const { return _data.rightKB; }
        [[nodiscard]] Qt::Key getFastRightKB() const { return _data.fastRightKB; }
        [[nodiscard]] Qt::Key getUpKB() const { return _data.upKB; }
        [[nodiscard]] Qt::Key getDownKB() const { return _data.downKB; }
        [[nodiscard]] Qt::Key getClickKB() const { return _data.clickKB; }
        [[nodiscard]] Qt::Key getBackKB() const { return _data.backKB; }
        [[nodiscard]] Qt::Key getReloadKB() const { return _data.reloadKB; }
        [[nodiscard]] Qt::Key getAppyKB() const { return _data.applyKB; }
        [[nodiscard]] Qt::Key getInputModeKB() const { return _data.inputModeKB; }
        [[nodiscard]] Qt::Key getScrollUpKB() const { return _data.scrollUpKB; }
        [[nodiscard]] Qt::Key getScrollDownKB() const { return _data.scrollDownKB; }
        [[nodiscard]] Qt::Key getScrollLeftKB() const { return _data.scrollLeftKB; }
        [[nodiscard]] Qt::Key getScrollRightKB() const { return _data.scrollRightKB; }
        [[nodiscard]] Qt::Key getScrollViewUpKB() const { return _data.scrollViewUpKB; }
        [[nodiscard]] Qt::Key getScrollViewDownKB() const { return _data.scrollViewDownKB; }
        [[nodiscard]] Qt::Key getScrollViewTopKB() const { return _data.scrollViewTopKB; }
        [[nodiscard]] Qt::Key getScrollViewBottomKB() const { return _data.scrollViewBottomKB; }
        [[nodiscard]] GamepadMap getGamepadMap(const QString &gamepadID) const { return _data.gamepadMappings.value(gamepadID, {}); }
        [[nodiscard]] QString getPreferredGamepadID() const { return _data.preferredGamepadID; }
        [[nodiscard]] bool getAllGamepadsUIControl() const { return _data.allGamepadsUIControl; }
        [[nodiscard]] QPointF getFloatIconPosition() const { return _data.floatIconPosition; }
        [[nodiscard]] FloatIconSize getFloatIconSize() const { return _data.floatIconSize; }
        [[nodiscard]] bool getRememberWindowState() const { return _data.rememberWindowState; }
        [[nodiscard]] UI::PWTWindowState getWindowState() const { return _data.windowState; }
        [[nodiscard]] QString getAutoConnectDaemonName() const { return _data.autoConnectDaemonName; }
        [[nodiscard]] bool getEnableTTS() const { return _data.enableTTS; }
        void setAppQuitBehaviour(const AppQuitBehaviour action) { _data.appQuitBehaviour = action; }
        void setPrevTabKB(const Qt::Key kbKey) { _data.prevTabKB = kbKey; }
        void setNextTabKB(const Qt::Key kbKey) { _data.nextTabKB = kbKey; }
        void setLeftKB(const Qt::Key kbKey) { _data.leftKB = kbKey; }
        void setFastLeftKB(const Qt::Key kbKey) { _data.fastLeftKB = kbKey; }
        void setRightKB(const Qt::Key kbKey) { _data.rightKB = kbKey; }
        void setFastRightKB(const Qt::Key kbKey) { _data.fastRightKB = kbKey; }
        void setUpKB(const Qt::Key kbKey) { _data.upKB = kbKey; }
        void setDownKB(const Qt::Key kbKey) { _data.downKB = kbKey; }
        void setClickKB(const Qt::Key kbKey) { _data.clickKB = kbKey; }
        void setBackKB(const Qt::Key kbKey) { _data.backKB = kbKey; }
        void setReloadKB(const Qt::Key kbKey) { _data.reloadKB = kbKey; }
        void setApplyKB(const Qt::Key kbKey) { _data.applyKB = kbKey; }
        void setInputModeKB(const Qt::Key kbKey) { _data.inputModeKB = kbKey; }
        void setScrollUpKB(const Qt::Key kbKey) { _data.scrollUpKB = kbKey; }
        void setScrollDownKB(const Qt::Key kbKey) { _data.scrollDownKB = kbKey; }
        void setScrollLeftKB(const Qt::Key kbKey) { _data.scrollLeftKB = kbKey; }
        void setScrollRightKB(const Qt::Key kbKey) { _data.scrollRightKB = kbKey; }
        void setScrollViewUpKB(const Qt::Key kbKey) { _data.scrollViewUpKB = kbKey; }
        void setScrollViewDownKB(const Qt::Key kbKey) { _data.scrollViewDownKB = kbKey; }
        void setScrollViewTopKB(const Qt::Key kbKey) { _data.scrollViewTopKB = kbKey; }
        void setScrollViewBottomKB(const Qt::Key kbKey) { _data.scrollViewBottomKB = kbKey; }
        void setGamepadMap(const QString &gamepadID, const GamepadMap &map) { _data.gamepadMappings.insert(gamepadID, map); }
        void setPreferredGamepadID(const QString &id) { _data.preferredGamepadID = id; }
        void setAllGamepadsUIControl(const bool enable) { _data.allGamepadsUIControl = enable; }
        void setFloatIconPosition(const QPointF &pos) { _data.floatIconPosition = pos; }
        void setFloatIconSize(const FloatIconSize size) { _data.floatIconSize = size; }
        void setRememberWindowState(const bool enable) { _data.rememberWindowState = enable; }
        void setWindowState(const UI::PWTWindowState &state) { _data.windowState = state; }
        void setAutoConnectDaemonName(const QString &name) { _data.autoConnectDaemonName = name; }
        void setEnableTTS(const bool enable) { _data.enableTTS = enable; }
        void addDaemon(const Daemon &dbox) { _data.daemonList.append(dbox); }
        void removeDaemon(const Daemon &dbox) { _data.daemonList.removeOne(dbox); }

        [[nodiscard]] static QSharedPointer<ConsoleSettings> getInstance();
        void setAppDataPath(const QString &appDataPath);
        void load();
        void save(bool quiet = false);
        void reset();
        [[nodiscard]] QList<std::pair<QString, int>> getGamepadsDeadzoneData() const;

    signals:
        void logMessageSent(const QString &msg);
    };
}
