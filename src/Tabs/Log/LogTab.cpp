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
#include <QDateTime>

#include "LogTab.h"

namespace PWT::CUI {
    LogTab::LogTab() {
        logs = new ConsoleLogText();
        clearBtn = new ConsoleButton("Clear");

        scrollWidgLyt->insertWidget(nextInsertIdx(), logs);
        scrollWidgLyt->insertWidget(nextInsertIdx(), clearBtn);
        setupFocusableWidgets();

        QObject::connect(clearBtn, &ConsoleButton::clicked, logs, &ConsoleLogText::clearLog);
    }

    void LogTab::writeLog(const QString &msg) const {
        logs->append(QString("%1 %2").arg(QDateTime::currentDateTime().toString("[ddd MMMM d yyyy hh:mm:ss] "), msg));
    }
}
