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
#include "SystemInfoWidget.h"

namespace PWT::CUI {
    SystemInfoWidget::SystemInfoWidget() {
        lyt = new QVBoxLayout();

        product = new ConsoleLabel("Product", defaultStr);
        distribution = new ConsoleLabel("Distribution", defaultStr);
        kernel = new ConsoleLabel("Kernel", defaultStr);
        ram = new ConsoleLabel("RAM", defaultStr);
        arch = new ConsoleLabel("Architecture", defaultStr);
        profile = new ConsoleLabel("PowerTuner Profile", "-");

        setDefaultLabels();
        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
    }

    void SystemInfoWidget::setDefaultLabels() const {
        lyt->addWidget(product);
        lyt->addWidget(distribution);
        lyt->addWidget(kernel);
        lyt->addWidget(ram);
        lyt->addWidget(arch);
        lyt->addWidget(profile);
    }

    void SystemInfoWidget::clear() const {
        while (lyt->count()) {
            const QLayoutItem *itm = lyt->takeAt(0);

            delete itm;
        }
    }

    void SystemInfoWidget::init(const PWTS::CpuInfo &cpuInfo, const PWTS::SystemInfo &sysInfo) {
        const bool isLinux = sysInfo.osType == PWTS::OSType::Linux;

        osType = sysInfo.osType;
        logicalCPUCount = cpuInfo.numLogicalCpus;

        product->setText(sysInfo.product);
        distribution->setText(sysInfo.distribution);
        kernel->setText(sysInfo.kernel);
        ram->setText(sysInfo.availableRam);
        arch->setText(sysInfo.arch);

        if (!sysInfo.manufacturer.isEmpty())
            addonInfoList.insert("manuf", new ConsoleLabel("Manufacturer", sysInfo.manufacturer));

        if (!sysInfo.biosVendor.isEmpty())
            addonInfoList.insert("biosvnd", new ConsoleLabel("Bios Vendor", sysInfo.biosVendor));

        if (!sysInfo.biosVersion.isEmpty())
            addonInfoList.insert("biosver", new ConsoleLabel("Bios Version", sysInfo.biosVersion));

        if (!sysInfo.biosDate.isEmpty())
            addonInfoList.insert("biosd", new ConsoleLabel("Bios Date", sysInfo.biosDate));

        if (!sysInfo.ecVersion.isEmpty())
            addonInfoList.insert("ecv", new ConsoleLabel("EC Version", sysInfo.ecVersion));

        if (isLinux || osType == PWTS::OSType::Windows) {
            addonInfoList.insert("onlinec", new ConsoleLabel("Online CPUs", defaultStr));
            addonInfoList.insert("offlinec", new ConsoleLabel("Offline CPUs", defaultStr));
        }

        if (isLinux)
            addonInfoList.insert("swap", new ConsoleLabel("Swap", "0"));

        setLabels();
    }

    void SystemInfoWidget::setLabels() const {
        clear();

        lyt->addWidget(product);

        if (addonInfoList.contains("manuf"))
            lyt->addWidget(addonInfoList["manuf"]);

        lyt->addWidget(distribution);
        lyt->addWidget(kernel);
        lyt->addWidget(arch);

        if (addonInfoList.contains("biosvnd"))
            lyt->addWidget(addonInfoList["biosvnd"]);

        if (addonInfoList.contains("biosver"))
            lyt->addWidget(addonInfoList["biosver"]);

        if (addonInfoList.contains("biosd"))
            lyt->addWidget(addonInfoList["biosd"]);

        if (addonInfoList.contains("ecv"))
            lyt->addWidget(addonInfoList["ecv"]);

        lyt->addWidget(ram);

        if (addonInfoList.contains("swap"))
            lyt->addWidget(addonInfoList["swap"]);

        if (addonInfoList.contains("onlinec"))
            lyt->addWidget(addonInfoList["onlinec"]);

        if (addonInfoList.contains("offlinec"))
            lyt->addWidget(addonInfoList["offlinec"]);

        lyt->addWidget(profile);
    }

    void SystemInfoWidget::setData(const PWTS::DaemonPacket &packet) {
        const bool isLinux = osType == PWTS::OSType::Linux;

        profile->setText(packet.activeProfile.isEmpty() ? "-" : packet.activeProfile);

        if (isLinux || osType == PWTS::OSType::Windows) {
            const int onlineCpus = packet.dynSysInfo.onlineCPUCount;

            addonInfoList["onlinec"]->setNum(onlineCpus);
            addonInfoList["offlinec"]->setNum(onlineCpus > 0 && onlineCpus <= logicalCPUCount ? (logicalCPUCount - onlineCpus) : 0);
        }

        if (isLinux)
            addonInfoList["swap"]->setText(packet.dynSysInfo.swapMemory);
    }

    void SystemInfoWidget::clearWidget() {
        osType = PWTS::OSType::Unknown;
        logicalCPUCount = 0;

        clear();

        for (ConsoleLabel *label: addonInfoList) {
            lyt->removeWidget(label);
            delete label;
        }

        product->setText(defaultStr);
        distribution->setText(defaultStr);
        kernel->setText(defaultStr);
        ram->setText(defaultStr);
        arch->setText(defaultStr);
        profile->setText("-");

        addonInfoList.clear();
        setDefaultLabels();
    }
}
