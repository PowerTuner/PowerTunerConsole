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

#include "CorePerformanceBoostWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::AMD {
    CorePerformanceBoostWidget::CorePerformanceBoostWidget(const int threadCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        boostAll = new ConsoleSelect("All CPUs");
        cpuSelect = new ConsoleSelect("CPU");
        cpuBoost = new ConsoleSelect("CPU boost");

        boostAll->addOptions({"", "Enabled", "Disabled"});
        cpuBoost->addOptions({"Enabled", "Disabled"});

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addOption(QString::number(i));

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(boostAll);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(cpuBoost);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(boostAll, &ConsoleSelect::selectionChanged, this, &CorePerformanceBoostWidget::onBoostAllChanged);
        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &CorePerformanceBoostWidget::onCPUSelectChanged);
        QObject::connect(cpuBoost, &ConsoleSelect::selectionChanged, this, &CorePerformanceBoostWidget::onBoostChanged);
    }

    void CorePerformanceBoostWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.amdData->threadData.size());
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (const PWTS::AMD::AMDThreadData &thd: packet.amdData->threadData) {
            data.append({
                .valid = thd.corePerfBoost.isValid(),
                .disable = thd.corePerfBoost.getValue()
            });
        }

        onCPUSelectChanged(cpuSelect->getCurrentIndex());
    }

    void CorePerformanceBoostWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.amdData->threadData[i].corePerfBoost.setValue(data[i].disable, true);
        }
    }

    void CorePerformanceBoostWidget::onBoostAllChanged(const int idx) {
        if (idx == 0)
            return;

        const QSignalBlocker sblock {cpuBoost};

        for (WData &wd: data) {
            if (wd.valid)
                wd.disable = idx - 1;
        }

        if (data[cpuSelect->getCurrentIndex()].valid)
            cpuBoost->setCurrentIndex(data[cpuSelect->getCurrentIndex()].disable);
    }

    void CorePerformanceBoostWidget::onCPUSelectChanged(const int idx) const {
        const QSignalBlocker allSblock {boostAll};
        const QSignalBlocker boostSblock {cpuBoost};

        cpuBoost->setEnabled(data[idx].valid);
        boostAll->setCurrentIndex(0);

        if (data[idx].valid)
            cpuBoost->setCurrentIndex(data[idx].disable);
    }

    void CorePerformanceBoostWidget::onBoostChanged(const int idx) {
        const QSignalBlocker sblock {boostAll};

        boostAll->setCurrentIndex(0);

        data[cpuSelect->getCurrentIndex()].disable = idx;
    }
}
