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
#include <QVBoxLayout>

#include "DaemonWidget.h"
#include "../../../ConsoleStyle.h"

namespace PWT::CUI {
    DaemonWidget::DaemonWidget(const QString &name, const QString &adr, const quint16 port) {
        QHBoxLayout *lyt = new QHBoxLayout();
        QVBoxLayout *btnLyt = new QVBoxLayout();
        QPalette pal = ConsoleStyle::getPalette();

        daemonLbl = new QLabel(QString("%1 [%2:%3]").arg(name.toUpper(), adr, QString::number(port)));
        dname = name;
        dadr = adr;
        dport = port;
        connectBtn = new ConsoleButton("Connect");
        deleteBtn = new ConsoleButton("Delete");

        daemonLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        btnLyt->addWidget(connectBtn);
        btnLyt->addWidget(deleteBtn);
        lyt->addWidget(daemonLbl);
        lyt->addLayout(btnLyt);

        setLayout(lyt);
        pal.setColor(QPalette::Window, QColor(17, 19, 22));
        setPalette(pal);
        setAutoFillBackground(true);

        QObject::connect(connectBtn, &ConsoleButton::clicked, this, &DaemonWidget::connectBtnClicked);
        QObject::connect(connectBtn, &ConsoleButton::focusChangeExternalIn, this, &DaemonWidget::onDaemonButtonFocusIn);
        QObject::connect(deleteBtn, &ConsoleButton::clicked, this, &DaemonWidget::deleteBtnClicked);
        QObject::connect(deleteBtn, &ConsoleButton::focusChangeExternalIn, this, &DaemonWidget::onDaemonButtonFocusIn);
    }

    void DaemonWidget::setConnectionResult(const bool connected) {
        connectBtn->setText(connected ? "Disconnect" : "Connect");
        connectBtn->setEnabled(true);
        connectBtn->setFocus(Qt::TabFocusReason);
        daemonConnected = connected;
    }

    void DaemonWidget::connectBtnClicked() {
        connectBtn->setEnabled(false);

        if (!daemonConnected) {
            connectBtn->setText("Connecting..");
            emit connectDaemon(dadr, dport);

        } else {
            connectBtn->setText("Disconnecting..");
            emit disconnectDaemon();
        }
    }

    void DaemonWidget::deleteBtnClicked() {
        connectBtn->setEnabled(false);
        deleteBtn->setEnabled(false);
        emit deleteDaemon();
    }

    void DaemonWidget::onDaemonButtonFocusIn(const ConsoleWidget *widget) {
        if (!widget->hasTTSEnabled()) [[likely]]
            return;

        emit sayStoppableTTS(QString("%1: %2").arg(widget->getFocusedTTS(), daemonLbl->text()));
    }
}
