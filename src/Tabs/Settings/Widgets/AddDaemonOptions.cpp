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

#include "AddDaemonOptions.h"
#include "pwtShared/DaemonSettings.h"
#include "pwtShared/Utils.h"
#include "../../Widgets/SectionWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI {
    AddDaemonOptions::AddDaemonOptions() {
        QVBoxLayout *lyt = new QVBoxLayout();

        consoleSettings = ConsoleSettings::getInstance();
        addDaemonName = new ConsoleLineEdit("Name");
        addDaemonAdr = new ConsoleLineEdit("Address");
        addDaemonPort = new ConsoleSpinBox("TCP port");
        addDaemonBtn = new ConsoleButton("Add daemon");

        addDaemonPort->setRange(1000, UINT16_MAX);
        addDaemonPort->setValue(PWTS::DaemonSettings::DefaultTCPPort);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new SectionWidget("Add daemon", SectionWidget::Size::Big));
        lyt->addWidget(addDaemonName);
        lyt->addWidget(addDaemonAdr);
        lyt->addWidget(addDaemonPort);
        lyt->addWidget(addDaemonBtn);

        setLayout(lyt);

        QObject::connect(addDaemonBtn, &ConsoleButton::clicked, this, &AddDaemonOptions::onAddDaemonClicked);
    }

    void AddDaemonOptions::onAddDaemonClicked() {
        const QString dname = addDaemonName->getText();
        const QString dadr = addDaemonAdr->getText();
        const int dport = addDaemonPort->getValue();

        if (dname.isEmpty()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Add daemon: name is empty"));
            return;

        } else if (dadr.isEmpty() || QHostAddress(dadr).isNull()) {
            UI::UILogger::getInstance()->writeLog(QString("Add daemon: address '%1' is not valid").arg(dadr));
            return;

        } else if (!PWTS::isValidPort(dport)) {
            UI::UILogger::getInstance()->writeLog(QString("Add daemon: port '%1' is not valid").arg(dport));
            return;
        }

        addDaemonName->clear();
        addDaemonAdr->clear();
        addDaemonPort->setValue(PWTS::DaemonSettings::DefaultTCPPort);
        emit daemonAdded(Daemon(dname, dadr, dport));
    }
}
