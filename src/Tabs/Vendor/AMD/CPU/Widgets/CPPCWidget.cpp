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
#include "CPPCWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::AMD {
    CPPCWidget::CPPCWidget(const QSet<PWTS::Feature> &cpuFeatures, const int threadCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        enable = new ConsoleCheckbox("CPPC enable");
        cpuSelect = new ConsoleSelect("CPU");
        applyToAll = new ConsoleCheckbox("Apply to all CPUs");
        cppcThreadWidget = new CPPCThreadWidget();

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addOption(QString::number(i));

        if (cpuFeatures.contains(PWTS::Feature::AMD_PSTATE_SYSFS) || cpuFeatures.contains(PWTS::Feature::PWR_SCHEME_GROUP)) {
            preferOSSettng = new ConsoleCheckbox("Prefer OS setting", "If checked, this option is ignored and the OS setting will be applied instead, for example SysFs in Linux or power scheme option in Windows.");

            preferOSSettng->setChecked(true);
            lyt->addWidget(preferOSSettng);

            QObject::connect(preferOSSettng, &ConsoleCheckbox::checkStateChanged, this, &CPPCWidget::onPreferOSStateChanged);
        }

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(enable);
        lyt->addWidget(applyToAll);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(cppcThreadWidget);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(applyToAll, &ConsoleCheckbox::checkStateChanged, this, &CPPCWidget::onApplyToAllChanged);
        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &CPPCWidget::onCpuSelectChanged);
    }

    void CPPCWidget::updateCPPCWidget(const int idx) const {
        cppcThreadWidget->setEnabled(data[idx].valid);

        if (!cppcThreadWidget->isEnabled())
            return;

        cppcThreadWidget->setRanges(data[idx].caps1);
        cppcThreadWidget->setData(data[idx].request);
    }

    void CPPCWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.amdData->threadData.size());
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const int enableBitValid = packet.amdData->cppcEnableBit.isValid();
        const int enableBit = packet.amdData->cppcEnableBit.getValue();

        enable->setEnabled(enableBitValid);
        enable->setChecked(enableBitValid && enableBit == 1);

        if (!preferOSSettng.isNull()) {
            const QSignalBlocker sblock {preferOSSettng};

            preferOSSettng->setChecked(packet.hasProfileData ? packet.amdData->cppcEnableBit.isIgnored() : preferOSChecked);
        }

        for (const PWTS::AMD::AMDThreadData &thd: packet.amdData->threadData) {
            data.append({
                .valid = thd.cppcCapability1.isValid() && thd.cppcRequest.isValid(),
                .caps1 = thd.cppcCapability1.getValue(),
                .request = thd.cppcRequest.getValue()
            });
        }

        updateCPPCWidget(cpuSelect->getCurrentIndex());
    }

    void CPPCWidget::setDataForPacket(const PWTS::ClientPacket &packet) {
        if (!isEnabled())
            return;

        const PWTS::AMD::CPPCRequest reqData = cppcThreadWidget->getData();
        const bool isIgnored = !preferOSSettng.isNull() && preferOSSettng->isChecked();

        packet.amdData->cppcEnableBit.setValue(enable->isChecked(), enable->isEnabled(), isIgnored);

        if (applyToAll->isChecked()) {
            for (WData &wd: data) {
                if (!wd.valid)
                    continue;

                wd.request = reqData;

                // fix bounds
                wd.request.maxPerf = qBound(wd.caps1.lowestPerf, wd.request.maxPerf, wd.caps1.highestPerf);
                wd.request.minPerf = qBound(wd.caps1.lowestPerf, wd.request.minPerf, wd.caps1.highestPerf);
                wd.request.desPerf = wd.request.desPerf > 0 ? qBound(wd.caps1.lowestPerf, wd.request.desPerf, wd.caps1.highestPerf) : 0;
            }
        } else {
            data[cpuSelect->getCurrentIndex()].request = reqData;
        }

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.amdData->threadData[i].cppcRequest.setValue(data[i].request, true, isIgnored);
        }
    }

    void CPPCWidget::onApplyToAllChanged(const Qt::CheckState state) const {
        cpuSelect->setEnabled(state != Qt::Checked);
    }

    void CPPCWidget::onCpuSelectChanged(const int idx) {
        if (data[prevCpuSelect].valid)
            data[prevCpuSelect].request = cppcThreadWidget->getData();

        prevCpuSelect = idx;

        updateCPPCWidget(idx);
    }

    void CPPCWidget::onPreferOSStateChanged(const Qt::CheckState state) {
        preferOSChecked = state == Qt::Checked;
    }
}
