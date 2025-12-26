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
#include "CurveOptimizerCoreWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::AMD {
    CurveOptimizerCoreWidget::CurveOptimizerCoreWidget(const int coreCount) {
        QVBoxLayout *lyt = new QVBoxLayout();

        enableChk = new ConsoleCheckbox("Enable setting", "If unchecked, this setting is ignored and wont be applied");
        applyToAll = new ConsoleCheckbox("Apply to all cores");
        cpuSelect = new ConsoleSelect("Core");
        co = new ConsoleSliderUnit("", [](QLabel *unitV, const int v) { unitV->setNum(v); });

        for (int i=0; i<coreCount; ++i)
            cpuSelect->addOption(QString::number(i));

        co->setStaticLabel("Offset");
        co->setPageStep(10);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(enableChk);
        lyt->addWidget(applyToAll);
        lyt->addWidget(cpuSelect);
        lyt->addWidget(co);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(applyToAll, &ConsoleCheckbox::checkStateChanged, this, &CurveOptimizerCoreWidget::onApplyToAllStateChanged);
        QObject::connect(cpuSelect, &ConsoleSelect::selectionChanged, this, &CurveOptimizerCoreWidget::onCpuSelectChanged);
        QObject::connect(enableChk, &ConsoleCheckbox::checkStateChanged, this, &CurveOptimizerCoreWidget::onEnableStateChanged);
    }

    void CurveOptimizerCoreWidget::updateCOSlider(const int idx) const {
        co->setEnabled(data[idx].valid);
        co->setValue(data[idx].co);
    }

    void CurveOptimizerCoreWidget::setRange(const PWTS::MinMax &range) const {
        co->setRange(range.min, range.max);
    }

    void CurveOptimizerCoreWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpuSelect->count() && cpuSelect->count() == packet.amdData->coreData.size());
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const QSignalBlocker sblock {enableChk};

        for (const PWTS::AMD::AMDCoreData &cdt: packet.amdData->coreData) {
            data.append({
                .valid = cdt.curveOptimizer.isValid(),
                .co = cdt.curveOptimizer.getValue()
            });
        }

        updateCOSlider(cpuSelect->getCurrentIndex());
        enableChk->setChecked(packet.hasProfileData ? !packet.amdData->coreData[0].curveOptimizer.isIgnored() : enableChecked);
    }

    void CurveOptimizerCoreWidget::setDataForPacket(const PWTS::ClientPacket &packet) {
        if (!isEnabled())
            return;

        const int curve = co->getValue();
        const bool isIgnored = !enableChk->isChecked();

        if (applyToAll->isChecked()) {
            for (WData &wd: data) {
                if (wd.valid)
                    wd.co = curve;
            }
        } else {
            data[cpuSelect->getCurrentIndex()].co = curve;
        }

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.amdData->coreData[i].curveOptimizer.setValue(data[i].co, true, isIgnored);
        }
    }

    void CurveOptimizerCoreWidget::onApplyToAllStateChanged(const Qt::CheckState state) const {
        cpuSelect->setEnabled(state != Qt::Checked);
    }

    void CurveOptimizerCoreWidget::onCpuSelectChanged(const int idx) {
        if (data[prevCpuSelected].valid)
            data[prevCpuSelected].co = co->getValue();

        prevCpuSelected = idx;

        updateCOSlider(idx);
    }

    void CurveOptimizerCoreWidget::onEnableStateChanged(const Qt::CheckState state) {
        enableChecked = state == Qt::Checked;
    }
}
