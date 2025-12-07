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
#include "BlockQueueSchedulerContainer.h"

namespace PWT::CUI::LNX {
    BlockQueueSchedulerContainer::BlockQueueSchedulerContainer() {
        lyt = new QVBoxLayout();

        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
    }

    void BlockQueueSchedulerContainer::clearDevices() {
        while (lyt->count()) {
            const QLayoutItem *itm = lyt->takeAt(0);

            itm->widget()->deleteLater();
            delete itm;
        }

        blockDevList.clear();
    }

    void BlockQueueSchedulerContainer::setData(const PWTS::DaemonPacket &packet) {
        clearDevices();

        for (const auto &[dev, data]: packet.linuxData->blockDevicesQueSched.asKeyValueRange()) {
            BlockQueueSchedulerWidget *wdg = new BlockQueueSchedulerWidget(dev, data);

            lyt->addWidget(wdg);
            blockDevList.append(wdg);

            QObject::connect(wdg, &BlockQueueSchedulerWidget::deleteDevice, this, &BlockQueueSchedulerContainer::onDeleteDevice);
        }
    }

    void BlockQueueSchedulerContainer::setDataForPacket(const PWTS::ClientPacket &packet) const {
        for (const BlockQueueSchedulerWidget *wdg: blockDevList)
            packet.linuxData->blockDevicesQueSched.insert(wdg->getDevice(), {.scheduler = wdg->getScheduler()});
    }

    void BlockQueueSchedulerContainer::onDeleteDevice() {
        const BlockQueueSchedulerWidget *dev = qobject_cast<BlockQueueSchedulerWidget *>(QObject::sender());

        if (dev == nullptr)
            return;

        const int idx = lyt->indexOf(dev);

        if (idx == -1)
            return;

        const QLayoutItem *itm = lyt->takeAt(idx);

        emit deleteDevice(itm);
        blockDevList.remove(idx);
    }
}
