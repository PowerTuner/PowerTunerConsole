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

#include "PStateWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::AMD {
    PStateWidget::PStateWidget(const int threadCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        pstateAll = new ConsoleSelect("All CPUs");
        cpuSelect = new ConsoleSelect("CPU");
        pstate = new ConsoleSelect("CPU PState");

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addOption(QString::number(i));

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(pstateAll);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(pstate);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(pstateAll, &ConsoleSelect::selectionChanged, this, &PStateWidget::onPStateAllChanged);
        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &PStateWidget::onCPUSelectChanged);
        QObject::connect(pstate, &ConsoleSelect::selectionChanged, this, &PStateWidget::onPStateChanged);
    }

    void PStateWidget::setData(const PWTS::DaemonPacket &packet) {
        const QSignalBlocker allSblock {pstateAll};
        const QSignalBlocker pstateBlock {pstate};

        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.amdData->threadData.size() && packet.amdData->pstateCurrentLimit.isValid());
        pstateAll->clear();
        pstate->clear();
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::AMD::PStateCurrentLimit limits = packet.amdData->pstateCurrentLimit.getValue();

        pstateAll->addOption("");

        for (int p=limits.curPStateLimit; p<=limits.pstateMaxValue; ++p) {
            const QString ps = QString::number(p);

            pstateAll->addOption(ps);
            pstate->addOption(ps);
        }

        for (const PWTS::AMD::AMDThreadData &thd: packet.amdData->threadData) {
            data.append({
                .valid = thd.pstateCmd.isValid(),
                .pstate = thd.pstateCmd.getValue()
            });
        }

        onCPUSelectChanged(cpuSelect->getCurrentIndex());
    }

    void PStateWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.amdData->threadData[i].pstateCmd.setValue(data[i].pstate, true);
        }
    }

    void PStateWidget::onPStateAllChanged(const int idx) {
        if (idx == 0)
            return;

        const QSignalBlocker sblock {pstate};

        for (WData &wd: data) {
            if (wd.valid)
                wd.pstate = idx - 1;
        }

        if (data[cpuSelect->getCurrentIndex()].valid)
            pstate->setCurrentIndex(data[cpuSelect->getCurrentIndex()].pstate);
    }

    void PStateWidget::onCPUSelectChanged(const int idx) const {
        const QSignalBlocker allSblock {pstateAll};
        const QSignalBlocker boostSblock {pstate};

        pstate->setEnabled(data[idx].valid);
        pstateAll->setCurrentIndex(0);

        if (data[idx].valid)
            pstate->setCurrentIndex(data[idx].pstate);
    }

    void PStateWidget::onPStateChanged(const int idx) {
        const QSignalBlocker sblock {pstateAll};

        pstateAll->setCurrentIndex(0);

        data[cpuSelect->getCurrentIndex()].pstate = idx;
    }
}
