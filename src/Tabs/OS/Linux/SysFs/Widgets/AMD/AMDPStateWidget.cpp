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

#include "AMDPStateWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI {
    AMDPStateWidget::AMDPStateWidget(const int threadCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        cpuSelect = new ConsoleSelect("CPU");
        pstateStatus = new ConsoleSelect("Status");
        eppAll = new ConsoleSelect("All CPUs");
        epp = new ConsoleSelect("Energy Performance Preference");

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addOption(QString::number(i));

        pstateStatus->addOptions({"", "disable", "active", "passive", "guided"});

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(pstateStatus);
        lyt->addWidget(eppAll);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(epp);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(pstateStatus, &ConsoleSelect::textChanged, this, &AMDPStateWidget::onStatusChanged);
        QObject::connect(eppAll, &ConsoleSelect::textChanged, this, &AMDPStateWidget::onEPPAllChanged);
        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &AMDPStateWidget::onCPUChanged);
        QObject::connect(epp, &ConsoleSelect::textChanged, this, &AMDPStateWidget::onEPPChanged);
    }

    void AMDPStateWidget::setData(const PWTS::DaemonPacket &packet) {
        const QList<PWTS::LNX::AMD::LinuxAMDThreadData> &thdData = packet.linuxAmdData->threadData;
        const QSignalBlocker eppAllBlock {eppAll};
        const QSignalBlocker statusBlock {pstateStatus};

        setEnabled(cpuSelect->count() && cpuSelect->count() == thdData.size() && packet.linuxAmdData->pstateStatus.isValid());
        eppAll->clear();
        epp->clear();
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("invalid data from packet"));
            return;
        }

        pstateStatus->setCurrentIndex(pstateStatus->indexOf(packet.linuxAmdData->pstateStatus.getValue()));
        eppAll->addOption("");

        for (int i=0,l=thdData.size(); i<l; ++i) {
            const QList<PWTS::LNX::LinuxThreadData> &lthd = packet.linuxData->threadData;
            const bool isCpuOn = lthd[i].cpuOnlineStatus.isValid() && lthd[i].cpuOnlineStatus.getValue() == 1;
            const QList<QString> availEpp = thdData[i].pstateData.getValue().eppAvailablePrefs;

            if (thdData[i].pstateData.isValid()) {
                for (const QString &epp: availEpp) {
                    if (eppAll->indexOf(epp) == -1)
                        eppAll->addOption(epp);
                }
            }

            data.append({
                .valid = isCpuOn && thdData[i].pstateData.isValid() && thdData[i].epp.isValid(),
                .availableEPP = availEpp,
                .epp = thdData[i].epp.getValue()
            });
        }

        onCPUChanged(cpuSelect->getCurrentIndex());
        onStatusChanged(pstateStatus->getCurrentText());
    }

    void AMDPStateWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxAmdData->pstateStatus.setValue(pstateStatus->getCurrentText(), isEnabled());

        for (int i=0,l=data.size(); i<l; ++i) {
            if (!data[i].valid)
                continue;

            packet.linuxAmdData->threadData[i].epp.setValue(data[i].epp, true);
        }
    }

    void AMDPStateWidget::onStatusChanged(const QString &text) const {
        const bool hasEPP = text == "active";

        cpuSelect->setEnabled(hasEPP);
        eppAll->setEnabled(hasEPP);
        epp->setEnabled(hasEPP);
    }

    void AMDPStateWidget::onEPPAllChanged(const QString &text) {
        if (text.isEmpty())
            return;

        const QSignalBlocker sblock {epp};
        const int eppIdx = epp->indexOf(text);

        eppAll->setEnabled(false);

        for (WData &wd: data) {
            if (wd.availableEPP.contains(text))
                wd.epp = text;
        }

        if (data[cpuSelect->getCurrentIndex()].valid && eppIdx != -1)
            epp->setCurrentIndex(eppIdx);

        eppAll->setEnabled(true);
        eppAll->setFocus(Qt::TabFocusReason);
    }

    void AMDPStateWidget::onCPUChanged(const int idx) const {
        const QSignalBlocker eppBlock {epp};
        const QSignalBlocker eppAllBlock {eppAll};

        eppAll->setCurrentIndex(0);
        epp->clear();

        if (!data[idx].valid)
            return;

        epp->addOptions(data[idx].availableEPP);
        epp->setCurrentIndex(epp->indexOf(data[idx].epp));
    }

    void AMDPStateWidget::onEPPChanged(const QString &text) {
        const QSignalBlocker eppAllBlock {eppAll};

        eppAll->setCurrentIndex(0);

        data[cpuSelect->getCurrentIndex()].epp = text;
    }
}
