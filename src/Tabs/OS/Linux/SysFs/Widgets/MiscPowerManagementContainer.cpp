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
#include "MiscPowerManagementContainer.h"

namespace PWT::CUI::LNX {
    MiscPowerManagementContainer::MiscPowerManagementContainer() {
        lyt = new QVBoxLayout();

        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
    }

    void MiscPowerManagementContainer::clearDevices() {
        while (lyt->count()) {
            const QLayoutItem *itm = lyt->takeAt(0);

            itm->widget()->deleteLater();
            delete itm;
        }

        pmDevList.clear();
    }

    void MiscPowerManagementContainer::setData(const PWTS::DaemonPacket &packet) {
        clearDevices();

        for (const PWTS::LNX::MiscPMDevice &dev: packet.linuxData->miscPMDevices) {
            MiscPowerManagementWidget *wdg = new MiscPowerManagementWidget(dev);

            lyt->addWidget(wdg);
            pmDevList.append(wdg);

            QObject::connect(wdg, &MiscPowerManagementWidget::deleteDevice, this, &MiscPowerManagementContainer::onDeleteDevice);
        }
    }

    void MiscPowerManagementContainer::setDataForPacket(const PWTS::ClientPacket &packet) const {
        QList<PWTS::LNX::MiscPMDevice> data;

        for (const MiscPowerManagementWidget *dev: pmDevList) {
            packet.linuxData->miscPMDevices.append({
                .name = dev->getName(),
                .control = dev->getPath(),
                .controlValue = dev->getMode()
            });
        }
    }

    void MiscPowerManagementContainer::onDeleteDevice() {
        const MiscPowerManagementWidget *dev = qobject_cast<MiscPowerManagementWidget *>(QObject::sender());

        if (dev == nullptr)
            return;

        const int idx = lyt->indexOf(dev);

        if (idx == -1)
            return;

        const QLayoutItem *itm = lyt->takeAt(idx);

        emit deleteDevice(itm);
        pmDevList.remove(idx);
    }
}
