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
#include "HWPRequestThreadWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    HWPRequestThreadWidget::HWPRequestThreadWidget(const QSet<PWTS::Feature> &cpuFeatures, int threadCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        applyToAll = new ConsoleCheckbox("Apply to all CPUs");
        cpuSelect = new ConsoleSelect("CPU");
        hwpRequestWidget = new HWPRequestWidget(
            cpuFeatures.contains(PWTS::Feature::INTEL_HWP_EPP),
            cpuFeatures.contains(PWTS::Feature::INTEL_HWP_ACT_WIND),
            cpuFeatures.contains(PWTS::Feature::INTEL_HWP_REQ_PKG),
            cpuFeatures.contains(PWTS::Feature::INTEL_HWP_VALID_BITS)
        );

        for (int i=0; i<threadCount; ++i)
            cpuSelect->addOption(QString::number(i));

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(applyToAll);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(hwpRequestWidget);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &HWPRequestThreadWidget::onCpuSelectChanged);
        QObject::connect(applyToAll, &ConsoleCheckbox::checkStateChanged, this, &HWPRequestThreadWidget::onApplyToAllStateChanged);
    }

    void HWPRequestThreadWidget::updateHwpReqWidget(const int idx) const {
        hwpRequestWidget->setEnabled(data[idx].valid);

        if (!data[idx].valid)
            return;

        hwpRequestWidget->setRanges(data[idx].caps);
        hwpRequestWidget->setRequestData(data[idx].request);
    }

    void HWPRequestThreadWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.intelData->threadData.size());
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (const PWTS::Intel::IntelThreadData &thd: packet.intelData->threadData) {
            data.append({
                .valid = thd.hwpCapapabilities.isValid() && thd.hwpRequest.isValid(),
                .caps = thd.hwpCapapabilities.getValue(),
                .request = thd.hwpRequest.getValue()
            });
        }

        updateHwpReqWidget(cpuSelect->getCurrentIndex());
    }

    void HWPRequestThreadWidget::setDataForPacket(const PWTS::ClientPacket &packet) {
        if (!isEnabled())
            return;

        const PWTS::Intel::HWPRequest reqData = hwpRequestWidget->getRequestData();

        if (applyToAll->isChecked()) {
            for (WData &wd: data) {
                if (!wd.valid)
                    continue;

                wd.request = reqData;

                // fix bounds
                wd.request.requestPkg.max = qBound(wd.caps.lowestPerf, wd.request.requestPkg.max, wd.caps.highestPerf);
                wd.request.requestPkg.min = qBound(wd.caps.lowestPerf, wd.request.requestPkg.min, wd.caps.highestPerf);
                wd.request.requestPkg.desired = wd.request.requestPkg.desired > 0 ? qBound(wd.caps.lowestPerf, wd.request.requestPkg.desired, wd.caps.highestPerf) : 0;
            }
        } else if (data[cpuSelect->getCurrentIndex()].valid) {
            data[cpuSelect->getCurrentIndex()].request = reqData;
        }

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.intelData->threadData[i].hwpRequest.setValue(data[i].request, true);
        }
    }

    void HWPRequestThreadWidget::onCpuSelectChanged(const int idx) {
        if (data[prevCpuSelect].valid)
            data[prevCpuSelect].request = hwpRequestWidget->getRequestData();

        prevCpuSelect = idx;

        updateHwpReqWidget(idx);
    }

    void HWPRequestThreadWidget::onApplyToAllStateChanged(const Qt::CheckState state) const {
        cpuSelect->setEnabled(state != Qt::Checked);
    }
}
