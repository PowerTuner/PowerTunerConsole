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
#include "HWPRequestWidget.h"

namespace PWT::CUI::INTEL {
    HWPRequestWidget::HWPRequestWidget(const bool hasEPP, const bool hasActivityWindow, const bool hasPackageControl, const bool hasValidBits) {
        const auto mhzUnitVCB = [](QLabel *unitV, const int v) { unitV->setNum(v*100); };
        QVBoxLayout *lyt = new QVBoxLayout();

        minPerf = new ConsoleSliderUnit("MHz", mhzUnitVCB, true, "Minimum performance");
        maxPerf = new ConsoleSliderUnit("MHz", mhzUnitVCB, true, "Maximum performance");
        desiredPerf = new ConsoleSliderUnit("MHz", mhzUnitVCB, true, "Desired performance");

        minPerf->setPageStep(1);
        maxPerf->setPageStep(1);
        desiredPerf->setLabel("Auto");
        desiredPerf->setPageStep(1);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(minPerf);
        lyt->addWidget(maxPerf);
        lyt->addWidget(desiredPerf);

        if (hasActivityWindow) {
            const auto acwUnitVCB = [](QLabel *unitV, const int v) { unitV->setNum(static_cast<float>(v) / 1000); };

            acw = new ConsoleSliderUnit("ms", acwUnitVCB, true, "Activity window");

            acw->setLabel("Auto");
            acw->setRange(0, 127 * 1000);
            acw->setPageStep(1000);

            lyt->addWidget(acw);

            QObject::connect(acw, &ConsoleSliderUnit::valueChanged, this, &HWPRequestWidget::onAcwChanged);
        }

        if (hasEPP) {
            epp = new ConsoleSliderLevels("Performance", "Power saving", "Energy performance preference");

            epp->setStaticLabel("0 (0 %)");
            epp->setRange(0, 255);
            epp->setPageStep(10);

            lyt->addWidget(epp);

            QObject::connect(epp, &ConsoleSliderLevels::valueChanged, this, &HWPRequestWidget::onEppChanged);
        }

        if (hasValidBits) {
            minValid = new ConsoleCheckbox("Minimum valid");
            maxValid = new ConsoleCheckbox("Maximum valid");
            desiredValid = new ConsoleCheckbox("Desired valid");

            lyt->addWidget(minValid);
            lyt->addWidget(maxValid);
            lyt->addWidget(desiredValid);

            if (hasActivityWindow) {
                activeWindowValid = new ConsoleCheckbox("Activity window valid");
                lyt->addWidget(activeWindowValid);
            }

            if (hasEPP) {
                eppValid = new ConsoleCheckbox("EPP valid");
                lyt->addWidget(eppValid);
            }
        }

        if (hasPackageControl) {
            pkgControl = new ConsoleCheckbox("Package control");
            lyt->addWidget(pkgControl);
        }

        setLayout(lyt);

        QObject::connect(desiredPerf, &ConsoleSliderUnit::valueChanged, this, &HWPRequestWidget::onDesiredPerfChanged);
    }

    void HWPRequestWidget::setRequestPkgData(const PWTS::Intel::HWPRequestPkg &data) const {
        maxPerf->setValue(data.max);
        minPerf->setValue(data.min);
        desiredPerf->setValue(data.desired);

        if (!acw.isNull())
            acw->setValue(data.acw);

        if (!epp.isNull())
            epp->setValue(data.epp);
    }

    void HWPRequestWidget::setRequestData(const PWTS::Intel::HWPRequest &data) const {
        setRequestPkgData(data.requestPkg);

        if (!minValid.isNull())
            minValid->setChecked(data.minValid);

        if (!maxValid.isNull())
            maxValid->setChecked(data.maxValid);

        if (!desiredValid.isNull())
            desiredValid->setChecked(data.desiredValid);

        if (!activeWindowValid.isNull())
            activeWindowValid->setChecked(data.acwValid);

        if (!eppValid.isNull())
            eppValid->setChecked(data.eppValid);

        if (!pkgControl.isNull())
            pkgControl->setChecked(data.packageControl);
    }

    PWTS::Intel::HWPRequestPkg HWPRequestWidget::getRequestPkgData() const {
        return {
            .min = minPerf->getValue(),
            .max = maxPerf->getValue(),
            .desired = desiredPerf->getValue(),
            .epp = !epp.isNull() ? epp->getValue() : 0,
            .acw = !acw.isNull() ? acw->getValue() : 0
        };
    }

    PWTS::Intel::HWPRequest HWPRequestWidget::getRequestData() const {
        return {
            .requestPkg = getRequestPkgData(),
            .packageControl = !pkgControl.isNull() && pkgControl->isChecked(),
            .acwValid = !activeWindowValid.isNull() && activeWindowValid->isChecked(),
            .eppValid = !eppValid.isNull() && eppValid->isChecked(),
            .desiredValid = !desiredValid.isNull() && desiredValid->isChecked(),
            .maxValid = !maxValid.isNull() && maxValid->isChecked(),
            .minValid = !minValid.isNull() && minValid->isChecked()
        };
    }

    void HWPRequestWidget::setRanges(const PWTS::Intel::HWPCapabilities &caps) const {
        minPerf->setRange(caps.lowestPerf, caps.highestPerf);
        maxPerf->setRange(caps.lowestPerf, caps.highestPerf);
        desiredPerf->setRange(caps.lowestPerf, caps.highestPerf);
    }

    void HWPRequestWidget::onDesiredPerfChanged(const int v) const {
        if (v != 0)
            return;

        desiredPerf->setLabel("Auto");

        if (!acw.isNull())
            acw->setValue(0);
    }

    void HWPRequestWidget::onEppChanged(const int v) const {
        const int perc = qRound((static_cast<float>(v) / static_cast<float>(epp->getMaximum())) * 100);

        epp->setLabel(QString("%1 (%2 %)").arg(v).arg(perc));
    }

    void HWPRequestWidget::onAcwChanged(const int v) const {
        if (v == 0)
            acw->setLabel("Auto");
    }
}
