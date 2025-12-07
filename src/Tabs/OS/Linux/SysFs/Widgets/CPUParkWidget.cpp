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
#include "CPUParkWidget.h"
#include "../../../../Widgets/ConsoleSliderLevels.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::LNX {
    CPUParkWidget::CPUParkWidget(const int threadCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        cpuParkSlider = new ConsoleSliderUnit("%", [](QLabel *unitV, const int v) { unitV->setNum(v); }, false);
        cpuSelect = new ConsoleSelect("CPU");
        cpuMode = new ConsoleSelect("Status");

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addOption(QString::number(i));

        cpuParkSlider->setRange(0, 100);
        cpuMode->addOptions({"Disable", "Enable"});

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(cpuParkSlider);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(cpuMode);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &CPUParkWidget::onCPUSelectChanged);
        QObject::connect(cpuMode, &ConsoleSelect::selectionChanged, this, &CPUParkWidget::onCPUModeChanged);
        QObject::connect(cpuParkSlider, &ConsoleSliderUnit::valueChanged, this, &CPUParkWidget::onParkSliderChanged);
    }

    void CPUParkWidget::setData(const PWTS::DaemonPacket &packet) {
        const QList<PWTS::LNX::LinuxThreadData> &thdData = packet.linuxData->threadData;

        setEnabled(cpuSelect->count() && cpuSelect->count() == thdData.size());
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("invalid data from packet"));
            return;
        }

        const QSignalBlocker sblock {cpuParkSlider};
        int i = 0;
        int perc;

        onlineCount = 0;

        for (const PWTS::LNX::LinuxThreadData &thd: thdData) {
            const bool isLogic = thd.coreID.isValid() && thd.coreID.getValue() != i;

            cpuSelect->setOptionText(i, isLogic ? QString("%1 [%2]").arg(i).arg(thd.coreID.getValue()) : QString::number(i));

            if (thd.cpuOnlineStatus.isValid() && thd.cpuOnlineStatus.getValue() == 1)
                ++onlineCount;

            data.append({
                .valid = thd.cpuOnlineStatus.isValid() && thd.cpuLogicalOffAvailable.isValid() && thd.cpuLogicalOffAvailable.getValue(),
                .status = thd.cpuOnlineStatus.getValue()
            });
            ++i;
        }

        perc = getOnlinePercent();

        cpuParkSlider->setValue(perc);
        cpuParkSlider->setUnitValue(perc);
        onCPUSelectChanged(cpuSelect->getCurrentIndex());
    }

    void CPUParkWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (int i=0,l=data.size(); i<l; ++i) {
            if (!data[i].valid)
                continue;

            packet.linuxData->threadData[i].cpuOnlineStatus.setValue(data[i].status, true);
        }
    }

    void CPUParkWidget::onCPUSelectChanged(const int idx) const {
        const QSignalBlocker sblock {cpuMode};

        cpuMode->setEnabled(data[idx].valid);
        cpuMode->setCurrentIndex(data[idx].valid ? data[idx].status : 1); // show enabled when status is unknown
    }

    void CPUParkWidget::onCPUModeChanged(const int idx) {
        const QSignalBlocker sblock {cpuParkSlider};
        int perc;

        onlineCount += idx ? 1 : -1;
        perc = getOnlinePercent();
        data[cpuSelect->getCurrentIndex()].status = idx;

        cpuParkSlider->setValue(perc);
        cpuParkSlider->setUnitValue(perc);
    }

    void CPUParkWidget::onParkSliderChanged(const int v) {
        const QSignalBlocker sblock {cpuMode};
        int newOnlineC = qFloor(static_cast<float>(v * data.size()) / 100.f);

        for (int i=0,l=data.size(); i<l; ++i) {
            const int enable = newOnlineC > 0;

            if (!data[i].valid || data[i].status == enable) {
                --newOnlineC;
                continue;
            }

            data[i].status = enable;
            onlineCount += enable ? 1 : -1;
            --newOnlineC;
        }

        cpuMode->setCurrentIndex(data[cpuSelect->getCurrentIndex()].status);
    }
}
