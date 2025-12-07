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

#include "MiscPowerManagementWidget.h"
#include "../../../../Widgets/ConsoleButton.h"
#include "../../../../Widgets/SectionItemWidget.h"

namespace PWT::CUI::LNX {
    MiscPowerManagementWidget::MiscPowerManagementWidget(const PWTS::LNX::MiscPMDevice &device) {
        QVBoxLayout *lyt = new QVBoxLayout();
        ConsoleButton *deleteBtn = new ConsoleButton("remove");

        mode = new ConsoleSelect("runtime");
        devPath = device.control;
        name = device.name;

        mode->addOptions({"auto", "on"});
        mode->setCurrentIndex(mode->indexOf(device.controlValue));

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new SectionItemWidget(device.name, device.control));
        lyt->addWidget(mode);
        lyt->addWidget(deleteBtn);

        setLayout(lyt);

        QObject::connect(deleteBtn, &ConsoleButton::clicked, this, &MiscPowerManagementWidget::onDeleteDevice);
    }

    void MiscPowerManagementWidget::onDeleteDevice() {
        emit deleteDevice();
    }
}
