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
#include "TurboPowerCurrentWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    TurboPowerCurrentWidget::TurboPowerCurrentWidget() {
        const auto sliderUnitVCB = [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); };
        QVBoxLayout *lyt = new QVBoxLayout();

        tdp = new ConsoleSliderUnit("Watts", sliderUnitVCB);
        tdpEnable = new ConsoleCheckbox("Enable TDP limit override");
        tdc = new ConsoleSliderUnit("Amps", sliderUnitVCB);
        tdcEnable = new ConsoleCheckbox("Enable TDC limit overrride");

        tdp->setStaticLabel("TDP Limit");
        tdp->setPageStep(100);
        tdc->setStaticLabel("TDC Limit");
        tdc->setPageStep(100);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(tdp);
        lyt->addWidget(tdpEnable);
        lyt->addWidget(tdc);
        lyt->addWidget(tdcEnable);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(tdpEnable, &ConsoleCheckbox::checkStateChanged, this, &TurboPowerCurrentWidget::onTDPEnableChanged);
        QObject::connect(tdcEnable, &ConsoleCheckbox::checkStateChanged, this, &TurboPowerCurrentWidget::onTDCEnableChanged);
    }

    void TurboPowerCurrentWidget::setRanges(const PWTS::MinMax &tdpRange, const PWTS::MinMax &tdcRange) const {
        tdp->setRange(tdpRange.min, tdpRange.max);
        tdc->setRange(tdcRange.min, tdcRange.max);
    }

    void TurboPowerCurrentWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->turboPowerCurrentLimit.isValid());

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::TurboPowerCurrentLimit data = packet.intelData->turboPowerCurrentLimit.getValue();

        tdp->setValue(data.tdpLimit);
        tdpEnable->setChecked(data.tdpLimitOverride);
        tdc->setValue(data.tdcLimit);
        tdcEnable->setChecked(data.tdcLimitOverride);
    }

    void TurboPowerCurrentWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->turboPowerCurrentLimit.setValue({
            .tdpLimit = tdp->getValue(),
            .tdpLimitOverride = tdpEnable->isChecked(),
            .tdcLimit = tdc->getValue(),
            .tdcLimitOverride = tdcEnable->isChecked(),
        }, true);
    }

    void TurboPowerCurrentWidget::onTDPEnableChanged(const Qt::CheckState state) const {
        tdp->setEnabled(state == Qt::Checked);
    }

    void TurboPowerCurrentWidget::onTDCEnableChanged(const Qt::CheckState state) const {
        tdc->setEnabled(state == Qt::Checked);
    }
}
