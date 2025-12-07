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

#include "MiscProcessorFeaturesWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    MiscProcessorFeaturesWidget::MiscProcessorFeaturesWidget(const QSet<PWTS::Feature> &cpuFeatures) {
        const bool hasEnhancedSpeedStep = cpuFeatures.contains(PWTS::Feature::INTEL_ENHANCED_SPEEDSTEP);
        const bool hasTurboBoost = cpuFeatures.contains(PWTS::Feature::INTEL_TURBO_BOOST);
        QVBoxLayout *lyt = new QVBoxLayout();

        if (hasEnhancedSpeedStep) {
            enhancedSpeedStep = new ConsoleCheckbox("Enhanced SpeedStep");
            lyt->addWidget(enhancedSpeedStep);
        }

        if (hasTurboBoost) {
            disableTurboBoost = new ConsoleCheckbox("Disable Turbo Boost");
            lyt->addWidget(disableTurboBoost);
        }

        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
        setEnabled(false);
    }

    void MiscProcessorFeaturesWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->miscProcFeatures.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::MiscProcFeatures data = packet.intelData->miscProcFeatures.getValue();

        if (!enhancedSpeedStep.isNull())
            enhancedSpeedStep->setChecked(data.enhancedSpeedStep);

        if (!disableTurboBoost.isNull())
            disableTurboBoost->setChecked(data.disableTurboMode);
    }

    void MiscProcessorFeaturesWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->miscProcFeatures.setValue({
            .enhancedSpeedStep = !enhancedSpeedStep.isNull() && enhancedSpeedStep->isChecked(),
            .disableTurboMode = !disableTurboBoost.isNull() && disableTurboBoost->isChecked()
        }, true);
    }
}
