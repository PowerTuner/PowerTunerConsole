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
#include "CPUInfoWidget.h"

namespace PWT::CUI {
    CPUInfoWidget::CPUInfoWidget() {
        lyt = new QVBoxLayout();

        cpu = new ConsoleLabel("CPU", defaultStr);
        codename = new ConsoleLabel("Codename", defaultStr);
        techNode = new ConsoleLabel("Technology node", defaultStr);
        vendor = new ConsoleLabel("Vendor", defaultStr);
        family = new ConsoleLabel("Family", defaultStr);
        extFamily = new ConsoleLabel("Ext. family", defaultStr);
        model = new ConsoleLabel("Model", defaultStr);
        extModel = new ConsoleLabel("Ext. model", defaultStr);
        coreCount = new ConsoleLabel("Core count", defaultStr);
        threadCount = new ConsoleLabel("Thread count", defaultStr);
        stepping = new ConsoleLabel("Stepping", defaultStr);
        l1Data = new ConsoleLabel("L1 data cache", defaultStr);
        l1ICache = new ConsoleLabel("L1 instruction cache", defaultStr);
        l2 = new ConsoleLabel("L2 cache", defaultStr);
        l3 = new ConsoleLabel("L3 cache", defaultStr);

        lyt->setContentsMargins(0, 0, 0, 0);

        lyt->addWidget(cpu);
        lyt->addWidget(codename);
        lyt->addWidget(techNode);
        lyt->addWidget(vendor);
        lyt->addWidget(family);
        lyt->addWidget(extFamily);
        lyt->addWidget(model);
        lyt->addWidget(extModel);
        lyt->addWidget(coreCount);
        lyt->addWidget(threadCount);
        lyt->addWidget(stepping);
        lyt->addWidget(l1Data);
        lyt->addWidget(l1ICache);
        lyt->addWidget(l2);
        lyt->addWidget(l3);

        setLayout(lyt);
    }

    void CPUInfoWidget::init(const PWTS::CpuInfo &cpuInfo) {
        cpu->setText(cpuInfo.brand);
        codename->setText(cpuInfo.codename);
        techNode->setText(cpuInfo.techNode);
        vendor->setText(cpuInfo.vendorString);
        family->setText(QString::number(cpuInfo.family, 16).prepend("0x"));
        extFamily->setText(QString::number(cpuInfo.extFamily, 16).prepend("0x"));
        model->setText(QString::number(cpuInfo.model, 16).prepend("0x"));
        extModel->setText(QString::number(cpuInfo.extModel, 16).prepend("0x"));
        coreCount->setNum(cpuInfo.numCores);
        threadCount->setNum(cpuInfo.numLogicalCpus);
        stepping->setNum(cpuInfo.stepping);
        l1Data->setText(cpuInfo.l1DCache);
        l1ICache->setText(cpuInfo.l1ICache);
        l2->setText(cpuInfo.l2Cache);
        l3->setText(cpuInfo.l3Cache);

        if (!cpuInfo.l4Cache.isEmpty()) {
            ConsoleLabel *l4 = new ConsoleLabel("L4 cache", cpuInfo.l4Cache);

            addonInfoList.insert("l4", l4);
            lyt->addWidget(l4);
        }

        for (int i=0,l=cpuInfo.numLogicalCpus; i<l; ++i) {
            ConsoleLabel *ucode = new ConsoleLabel(QString("CPU%1 microcode").arg(i), defaultStr);

            addonInfoList.insert(QString("ucode%1").arg(i), ucode);
            lyt->addWidget(ucode);
        }
    }

    void CPUInfoWidget::setData(const PWTS::DaemonPacket &packet) const {
        const PWTS::DynamicSystemInfo &dynInfo = packet.dynSysInfo;

        for (int i=0,l=dynInfo.cpuMicrocode.size(); i<l; ++i)
            addonInfoList.value(QString("ucode%1").arg(i))->setText(dynInfo.cpuMicrocode[i]);
    }

    void CPUInfoWidget::clearWidget() {
        cpu->setText(defaultStr);
        codename->setText(defaultStr);
        techNode->setText(defaultStr);
        vendor->setText(defaultStr);
        family->setText(defaultStr);
        extFamily->setText(defaultStr);
        model->setText(defaultStr);
        extModel->setText(defaultStr);
        coreCount->setText(defaultStr);
        threadCount->setText(defaultStr);
        stepping->setText(defaultStr);
        l1Data->setText(defaultStr);
        l1ICache->setText(defaultStr);
        l2->setText(defaultStr);
        l3->setText(defaultStr);

        for (ConsoleLabel *label: addonInfoList) {
            lyt->removeWidget(label);
            delete label;
        }

        addonInfoList.clear();
    }
}
