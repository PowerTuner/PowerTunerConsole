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

#include "MiscGlobalOptionsWidget.h"

namespace PWT::CUI::WIN {
    MiscGlobalOptionsWidget::MiscGlobalOptionsWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        resetSchemes = new ConsoleCheckbox("Restore default power schemes", "Reset power schemes to Windows defaults before applying settings");
        replaceDefaults = new ConsoleCheckbox("Replace default power schemes", "Replace all Windows default power schemes with current schemes, so that reset to defaults will reset to custom scheme settings");

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(resetSchemes);
        lyt->addWidget(replaceDefaults);

        setLayout(lyt);
    }

    void MiscGlobalOptionsWidget::refreshWidget(const PWTS::DaemonPacket &packet) const {
        resetSchemes->setChecked(packet.windowsData->resetSchemesDefault);
        replaceDefaults->setChecked(packet.windowsData->replaceDefaultSchemes);
    }
}
