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

#include <QDataStream>
#include <QString>

namespace PWT {
    struct [[nodiscard]] Daemon final {
        QString name;
        QString adr;
        quint16 port;

        Daemon() = default;

        Daemon(const QString &name, const QString &adr, const quint16 port) {
            this->name = name;
            this->adr = adr;
            this->port = port;
        }

        friend QDataStream &operator<<(QDataStream &ds, const Daemon &df) {
            ds << df.name <<
               df.adr <<
               df.port;

            return ds;
        }

        friend QDataStream &operator>>(QDataStream &ds, Daemon &df) {
            ds >> df.name >>
               df.adr >>
               df.port;

            return ds;
        }

        friend bool operator==(const Daemon &a, const Daemon &b) {
            return a.name == b.name && a.adr == b.adr && a.port == b.port;
        }
    };
}
