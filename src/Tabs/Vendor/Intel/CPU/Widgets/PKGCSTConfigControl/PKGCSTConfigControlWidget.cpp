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

#include "PKGCSTConfigControlWidget.h"
#include "PKGCSTConfigControlSBCoreWidget.h"
#include "PKGCSTConfigControlCU1CoreWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::INTEL {
    PKGCSTConfigControlWidget::PKGCSTConfigControlWidget(const int coreCount, const QSet<PWTS::Feature> &features) {
        QVBoxLayout *lyt = new QVBoxLayout();

        cpu = new ConsoleSelect("Core");
        applyToAll = new ConsoleCheckbox("Apply to all cores");

        for (int i=0; i<coreCount; ++i)
            cpu->addOption(QString::number(i));

        initPkgCstConfigControlWidget(features);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(applyToAll);
        lyt->addWidget(cpu);
        lyt->addWidget(pkgCstConfigControlCoreWidget);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(applyToAll, &ConsoleCheckbox::checkStateChanged, this, &PKGCSTConfigControlWidget::onApplyToAllChanged);
        QObject::connect(cpu, &ConsoleSelect::selectionChanged, this, &PKGCSTConfigControlWidget::onCpuSelectChanged);
    }

    void PKGCSTConfigControlWidget::initPkgCstConfigControlWidget(const QSet<PWTS::Feature> &features) {
        if (features.contains(PWTS::Feature::INTEL_PKG_CST_CONFIG_CONTROL_SB))
            pkgCstConfigControlCoreWidget = new PKGCSTConfigControlSBCoreWidget();
        else if (features.contains(PWTS::Feature::INTEL_PKG_CST_CONFIG_CONTROL_CU1))
            pkgCstConfigControlCoreWidget = new PKGCSTConfigControlCU1CoreWidget();
    }

    void PKGCSTConfigControlWidget::updatePkgCstConfigControlWidget(const int idx) const {
        pkgCstConfigControlCoreWidget->setEnabled(data[idx].valid);

        if (!data[idx].valid)
            return;

        pkgCstConfigControlCoreWidget->setData(data[idx].cstConfig, data[idx].cfgLock);
    }

    void PKGCSTConfigControlWidget::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(cpu->count() && cpu->count() == packet.intelData->coreData.size());
        data.clear();

        if (!isEnabled()) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        for (const PWTS::Intel::IntelCoreData &core: packet.intelData->coreData) {
            const PWTS::Intel::PkgCstConfigControl cfg = core.pkgCstConfigControl.getValue();

            data.append({
                .valid = core.pkgCstConfigControl.isValid(),
                .cfgLock = cfg.cfgLock,
                .cstConfig = cfg
            });
        }

        updatePkgCstConfigControlWidget(cpu->getCurrentIndex());
    }

    void PKGCSTConfigControlWidget::setDataForPacket(const PWTS::ClientPacket &packet) {
        if (!isEnabled())
            return;

        const PWTS::Intel::PkgCstConfigControl cstConfigData = pkgCstConfigControlCoreWidget->getData();

        if (applyToAll->isChecked()) {
            for (WData &wd: data) {
                if (!wd.valid)
                    continue;

                wd.cstConfig = cstConfigData;
            }
        } else if (data[cpu->getCurrentIndex()].valid) {
            data[cpu->getCurrentIndex()].cstConfig = cstConfigData;
        }

        for (int i=0,l=data.size(); i<l; ++i) {
            if (data[i].valid)
                packet.intelData->coreData[i].pkgCstConfigControl.setValue(data[i].cstConfig, true);
        }
    }

    void PKGCSTConfigControlWidget::onApplyToAllChanged(const Qt::CheckState state) const {
        cpu->setEnabled(state != Qt::Checked);
    }

    void PKGCSTConfigControlWidget::onCpuSelectChanged(const int idx) {
        if (data[prevCpuSelect].valid)
            data[prevCpuSelect].cstConfig = pkgCstConfigControlCoreWidget->getData();

        prevCpuSelect = idx;

        updatePkgCstConfigControlWidget(idx);
    }
}
