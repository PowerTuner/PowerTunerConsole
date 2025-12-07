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
#include <QCryptographicHash>

#include "ConsoleSettings.h"
#include "pwtShared/Utils.h"
#include "pwtShared/DaemonSettings.h"

namespace PWT {
    QSharedPointer<ConsoleSettings> ConsoleSettings::getInstance() {
        if (instance.isNull())
            instance.reset(new ConsoleSettings);

        return instance;
    }

    void ConsoleSettings::setAppDataPath(const QString &appDataPath) {
        if (!appDataPath.isEmpty())
            path = QString("%1/powertunerconsole.pwt").arg(appDataPath);
    }

    Daemon ConsoleSettings::defaultDaemon() const {
        return {"Local", "127.0.0.1", PWTS::DaemonSettings::DefaultTCPPort};
    }

    void ConsoleSettings::load() {
        const QByteArray fdata = PWTS::loadFile(path);

        if (fdata.isEmpty()) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Failed to load console settings, using defaults");
            return;
        }

        QDataStream ds(fdata);
        QString fsignature;
        QByteArray checksum;
        QByteArray settingsData;
        CData cdata;

        ds >> fsignature;
        if (fsignature != signature) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Invalid console settings signature, using defaults");
            return;
        }

        ds >> checksum >> settingsData;
        if (checksum != QCryptographicHash::hash(settingsData, QCryptographicHash::Sha256)) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Invalid console settings checksum, using defaults");
            return;
        }

        if (!PWTS::unpackData<ConsoleSettings::CData>(settingsData, cdata)) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Failed to unpack console settings, using defaults");
            return;
        }

        if (cdata.daemonList.isEmpty())
            cdata.daemonList.append(defaultDaemon());

        _data = cdata;
    }

    void ConsoleSettings::save(const bool quiet) {
        if (path.isEmpty()) {
            emit logMessageSent(QStringLiteral("ConsoleSettings: no disk path, cannot save to disk"));
            return;
        }

        QByteArray data, cdata;
        QDataStream ds(&data, QIODevice::WriteOnly);

        if (!PWTS::packData<ConsoleSettings::CData>(_data, cdata)) {
            emit logMessageSent("ConsoleSettings: failed to pack data, save failed");
            return;
        }

        ds << signature <<
            QCryptographicHash::hash(cdata, QCryptographicHash::Sha256) <<
            cdata;

        if (!PWTS::writeFile(path, data))
            emit logMessageSent("Failed to save console settings");
        else if (!quiet)
            emit logMessageSent(QStringLiteral("Console settings saved successfully"));
    }

    void ConsoleSettings::reset() {
        CData data {};

        data.daemonList = _data.daemonList;
        _data = data;
    }

    QList<std::pair<QString, int>> ConsoleSettings::getGamepadsDeadzoneData() const {
        QList<std::pair<QString, int>> data;

        for (const auto &[gid, gmap]: _data.gamepadMappings.asKeyValueRange())
            data.append(std::make_pair(gid, gmap.deadzone));

        return data;
    }

    QDataStream &operator<<(QDataStream &ds, const ConsoleSettings::CData &cdata) {
        ds << cdata.version <<
            cdata.daemonList <<
            cdata.appQuitBehaviour <<
            cdata.prevTabKB <<
            cdata.nextTabKB <<
            cdata.leftKB <<
            cdata.fastLeftKB <<
            cdata.rightKB <<
            cdata.fastRightKB <<
            cdata.upKB <<
            cdata.downKB <<
            cdata.clickKB <<
            cdata.backKB <<
            cdata.reloadKB <<
            cdata.applyKB <<
            cdata.inputModeKB <<
            cdata.scrollUpKB <<
            cdata.scrollDownKB <<
            cdata.scrollLeftKB <<
            cdata.scrollRightKB <<
            cdata.scrollViewUpKB <<
            cdata.scrollViewDownKB <<
            cdata.scrollViewTopKB <<
            cdata.scrollViewBottomKB <<
            cdata.gamepadMappings <<
            cdata.preferredGamepadID <<
            cdata.allGamepadsUIControl <<
            cdata.floatIconPosition <<
            cdata.floatIconSize <<
            cdata.rememberWindowState <<
            cdata.windowState <<
            cdata.autoConnectDaemonName <<
            cdata.enableTTS;

        return ds;
    }

    QDataStream &operator>>(QDataStream &ds, ConsoleSettings::CData &cdata) {
        ds >> cdata.version >>
            cdata.daemonList >>
            cdata.appQuitBehaviour >>
            cdata.prevTabKB >>
            cdata.nextTabKB >>
            cdata.leftKB >>
            cdata.fastLeftKB >>
            cdata.rightKB >>
            cdata.fastRightKB >>
            cdata.upKB >>
            cdata.downKB >>
            cdata.clickKB >>
            cdata.backKB >>
            cdata.reloadKB >>
            cdata.applyKB >>
            cdata.inputModeKB >>
            cdata.scrollUpKB >>
            cdata.scrollDownKB >>
            cdata.scrollLeftKB >>
            cdata.scrollRightKB >>
            cdata.scrollViewUpKB >>
            cdata.scrollViewDownKB >>
            cdata.scrollViewTopKB >>
            cdata.scrollViewBottomKB >>
            cdata.gamepadMappings >>
            cdata.preferredGamepadID >>
            cdata.allGamepadsUIControl >>
            cdata.floatIconPosition >>
            cdata.floatIconSize >>
            cdata.rememberWindowState >>
            cdata.windowState >>
            cdata.autoConnectDaemonName >>
            cdata.enableTTS;

        return ds;
    }
}
