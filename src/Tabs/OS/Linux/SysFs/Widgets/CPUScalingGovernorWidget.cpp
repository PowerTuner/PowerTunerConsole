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

#include "CPUScalingGovernorWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI {
    CPUScalingGovernorWidget::CPUScalingGovernorWidget(const int threadCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        governorAll = new ConsoleSelect("All CPUs");
        cpuSelect = new ConsoleSelect("CPU");
        governor = new ConsoleSelect("Scaling governor");

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addOption(QString::number(i));

        lyt->addWidget(governorAll);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(governor);
        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(governorAll, &ConsoleSelect::textChanged, this, &CPUScalingGovernorWidget::onGovernorAllChanged);
        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &CPUScalingGovernorWidget::onCPUSelectChanged);
        QObject::connect(governor, &ConsoleSelect::textChanged, this, &CPUScalingGovernorWidget::onGovernorChanged);
    }

    void CPUScalingGovernorWidget::setData(const PWTS::DaemonPacket &packet) {
        const QList<PWTS::LNX::LinuxThreadData> &thdData = packet.linuxData->threadData;
        const QSignalBlocker sblock {governorAll};

        setEnabled(cpuSelect->count() && cpuSelect->count() == thdData.size());
        governorAll->clear();
        governor->clear();
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("invalid data from packet"));
            return;
        }

        governorAll->addOption("");

        for (const PWTS::LNX::LinuxThreadData &thd: thdData) {
            const PWTS::LNX::CPUScalingAvailableGovernors availGovs = thd.scalingAvailableGovernors.getValue();
            const bool isCpuOn = thd.cpuOnlineStatus.isValid() && thd.cpuOnlineStatus.getValue() == 1;

            if (thd.scalingAvailableGovernors.isValid()) {
                for (const QString &gov: availGovs.availableGovernors) {
                    if (governorAll->indexOf(gov) == -1)
                        governorAll->addOption(gov);
                }
            }

            data.append({
                .valid = isCpuOn && thd.scalingAvailableGovernors.isValid() && thd.scalingGovernor.isValid(),
                .availableGovs = availGovs,
                .governor = thd.scalingGovernor.getValue()
            });
        }

        onCPUSelectChanged(cpuSelect->getCurrentIndex());
    }

    void CPUScalingGovernorWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (int i=0,l=data.size(); i<l; ++i) {
            if (!data[i].valid)
                continue;

            packet.linuxData->threadData[i].scalingGovernor.setValue(data[i].governor, true);
        }
    }

    void CPUScalingGovernorWidget::onGovernorAllChanged(const QString &text) {
        if (text.isEmpty())
            return;

        const QSignalBlocker sblock {governor};
        const int govIdx = governor->indexOf(text);

        governorAll->setEnabled(false);

        for (WData &wd: data) {
            if (wd.availableGovs.availableGovernors.contains(text))
                wd.governor = text;
        }

        if (data[cpuSelect->getCurrentIndex()].valid && govIdx != -1)
            governor->setCurrentIndex(govIdx);

        governorAll->setEnabled(true);
        governorAll->setFocus(Qt::TabFocusReason);
    }

    void CPUScalingGovernorWidget::onCPUSelectChanged(const int idx) const {
        const QSignalBlocker sblockAll {governorAll};
        const QSignalBlocker sblock {governor};

        governorAll->setCurrentIndex(0);
        governor->clear();

        if (!data[idx].valid)
            return;

        governor->addOptions(data[idx].availableGovs.availableGovernors);
        governor->setCurrentIndex(governor->indexOf(data[idx].governor));
    }

    void CPUScalingGovernorWidget::onGovernorChanged(const QString &text) {
        const QSignalBlocker sblock {governorAll};

        data[cpuSelect->getCurrentIndex()].governor = text;

        governorAll->setCurrentIndex(0);

        for (const QString &relatedIdx: data[cpuSelect->getCurrentIndex()].availableGovs.relatedCPUs) {
            bool res;
            const int cpuIdx = relatedIdx.toInt(&res);

            if (!res || !data[cpuIdx].valid)
                continue;

            data[cpuIdx].governor = text;
        }
    }
}
