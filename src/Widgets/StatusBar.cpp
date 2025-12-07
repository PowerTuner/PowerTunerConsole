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
#include <QHBoxLayout>

#include "StatusBar.h"

namespace PWT::CUI {
    StatusBar::StatusBar() {
        QHBoxLayout *lyt = new QHBoxLayout();
        QPalette cmdPal, connPal;
        QFont wfont = font();

        cmdStatus = new QLabel();
        connectionStatus = new QLabel();
        cmdPal = cmdStatus->palette();
        connPal = connectionStatus->palette();

        wfont.setPointSize(10);
        wfont.setWeight(QFont::Normal);
        setFont(wfont);
        cmdPal.setColor(QPalette::WindowText, Qt::red);
        connPal.setColor(QPalette::WindowText, Qt::red);
        cmdStatus->setPalette(cmdPal);
        connectionStatus->setPalette(connPal);
        setDaemonConnected(false);

        lyt->setAlignment(Qt::AlignRight);
        lyt->addWidget(new QLabel("GPLv3  |  PowerTuner Console - kylon  |"));
        lyt->addWidget(cmdStatus);
        lyt->addWidget(new QLabel("|"));
        lyt->addWidget(connectionStatus);

        setLayout(lyt);
    }

    void StatusBar::setCMDStatus(const CMDStatus status) const {
        QPalette pal = cmdStatus->palette();
        QColor color;

        switch (status) {
            case CMDStatus::Pending: {
                color = QColor(214, 61, 0);
                cmdStatus->setText("Command pending");
            }
                break;
            case CMDStatus::Ready: {
                color = Qt::green;
                cmdStatus->setText("Command ready");
            }
                break;
            default: {
                color = Qt::red;
                cmdStatus->setText("Command disconnected");
            }
                break;
        }

        pal.setColor(QPalette::WindowText, color);
        cmdStatus->setPalette(pal);
    }

    void StatusBar::setDaemonConnected(const bool connected) const {
        QPalette pal = connectionStatus->palette();
        QColor color;

        if (connected) {
            color = Qt::green;
            connectionStatus->setText("Daemon connected");

        } else {
            color = Qt::red;
            setCMDStatus(CMDStatus::Disconnected);
            connectionStatus->setText("Daemon disconnected");
        }

        pal.setColor(QPalette::WindowText, color);
        connectionStatus->setPalette(pal);
    }
}
