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

#include "BlockQueueSchedulerWidget.h"
#include "../../../../Widgets/ConsoleButton.h"
#include "../../../../Widgets/SectionItemWidget.h"

namespace PWT::CUI::LNX {
    BlockQueueSchedulerWidget::BlockQueueSchedulerWidget(const QString &device, const PWTS::LNX::BlockDeviceQueSched &data) {
        QVBoxLayout *lyt = new QVBoxLayout();
        ConsoleButton *deleteBtn = new ConsoleButton("remove");

        scheduler = new ConsoleSelect("scheduler");
        deviceLabel = data.name;
        blockDev = device;

        scheduler->addOptions(data.availableQueueSchedulers);
        scheduler->setCurrentIndex(scheduler->indexOf(data.scheduler));

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new SectionItemWidget(deviceLabel, QString("device: %2").arg(blockDev)));
        lyt->addWidget(scheduler);
        lyt->addWidget(deleteBtn);

        setLayout(lyt);

        QObject::connect(deleteBtn, &ConsoleButton::clicked, this, &BlockQueueSchedulerWidget::onDeleteDevice);
    }

    QString BlockQueueSchedulerWidget::getDevice() const {
        return blockDev;
    }

    QString BlockQueueSchedulerWidget::getScheduler() const {
        return scheduler->getCurrentText();
    }

    void BlockQueueSchedulerWidget::onDeleteDevice() {
        emit deleteDevice();
    }
}
