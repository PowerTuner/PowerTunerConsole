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
#include "MCHBARTab.h"
#include "../../../Widgets/SectionWidget.h"
#include "pwtClientCommon/InputRanges/InputRanges.h"
#include "Widgets/PkgRaplLimit/PkgRaplLimitIVBWidget.h"
#include "Widgets/PkgRaplLimit/PkgRaplLimitTGLWidget.h"

namespace PWT::CUI::INTEL {
    MCHBARTab::MCHBARTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::INTEL_MCHBAR_PKG_RAPL_LIMIT)) {
            if (features.contains(PWTS::Feature::INTEL_MCHBAR_PKG_RAPL_LIMIT_IVB))
                pkgRaplLimitWidget = new PkgRaplLimitIVBWidget();
            else if (features.contains(PWTS::Feature::INTEL_MCHBAR_PKG_RAPL_LIMIT_TGL))
                pkgRaplLimitWidget = new PkgRaplLimitTGLWidget();

            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget("RAPL power limits"));
            scrollWidgLyt->insertWidget(nextInsertIdx(), pkgRaplLimitWidget);
        }
    }

    void MCHBARTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const QSharedPointer<UI::InputRanges> inputRanges = UI::InputRanges::getInstance();

        if (!pkgRaplLimitWidget.isNull()) {
            pkgRaplLimitWidget->setRanges(inputRanges->getIntelPl());
            pkgRaplLimitWidget->setData(packet);
        }

        setupFocusableWidgets();
    }

    void MCHBARTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!pkgRaplLimitWidget.isNull())
            pkgRaplLimitWidget->setDataForPacket(packet);
    }
}
