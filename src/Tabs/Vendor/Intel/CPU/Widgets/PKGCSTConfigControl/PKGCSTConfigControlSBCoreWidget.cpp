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

#include "PKGCSTConfigControlSBCoreWidget.h"

namespace PWT::CUI::INTEL {
    PKGCSTConfigControlSBCoreWidget::PKGCSTConfigControlSBCoreWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        pkgCStateLimit = new ConsoleSelect("Package C-State Limit");
        ioMwaitRedirectionEnable = new ConsoleCheckbox("I/O MWAIT redirection enable");
        cfgLock = new ConsoleCheckbox("CFG lock");
        c3StateAutodemotionEnable = new ConsoleCheckbox("C3 state auto-demotion enable");
        c1StateAutodemotionEnable = new ConsoleCheckbox("C1 state auto-demotion enable");
        c3UndemotionEnable = new ConsoleCheckbox("C3 un-demotion enable");
        c1UndemotionEnable = new ConsoleCheckbox("C1 un-demotion enable");

        pkgCStateLimit->addOption("C0/C1 (no package C-sate support)", 0b000);
        pkgCStateLimit->addOption("C2", 0b001);
        pkgCStateLimit->addOption("C6 no retention", 0b010);
        pkgCStateLimit->addOption("C6 retention", 0b011);
        pkgCStateLimit->addOption("C7", 0b100);
        pkgCStateLimit->addOption("C7s", 0b101);
        pkgCStateLimit->addOption("No package C-state limit", 0b111);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(pkgCStateLimit);
        lyt->addWidget(ioMwaitRedirectionEnable);
        lyt->addWidget(cfgLock);
        lyt->addWidget(c3StateAutodemotionEnable);
        lyt->addWidget(c1StateAutodemotionEnable);
        lyt->addWidget(c3UndemotionEnable);
        lyt->addWidget(c1UndemotionEnable);

        setLayout(lyt);

        QObject::connect(cfgLock, &ConsoleCheckbox::checkStateChanged, this, &PKGCSTConfigControlSBCoreWidget::onCfgLockStateChanged);
    }

    void PKGCSTConfigControlSBCoreWidget::setData(const PWTS::Intel::PkgCstConfigControl &data, const bool isBiosLocked) const {
        const QSignalBlocker sblock {cfgLock};

        pkgCStateLimit->setCurrentIndex(pkgCStateLimit->indexOf(data.packageCStateLimit));
        ioMwaitRedirectionEnable->setChecked(data.ioMwaitRedirectionEnable);
        cfgLock->setChecked(data.cfgLock);
        c3StateAutodemotionEnable->setChecked(data.c3StateAutodemotionEnable);
        c1StateAutodemotionEnable->setChecked(data.c1StateAutodemotionEnable);
        c3UndemotionEnable->setChecked(data.c3UndemotionEnable);
        c1UndemotionEnable->setChecked(data.c1UndemotionEnable);
        applyCFGLock(isBiosLocked);
    }

    PWTS::Intel::PkgCstConfigControl PKGCSTConfigControlSBCoreWidget::getData() const {
        return {
            .packageCStateLimit = static_cast<short>(pkgCStateLimit->getCurrentData().toInt()),
            .ioMwaitRedirectionEnable = ioMwaitRedirectionEnable->isChecked(),
            .cfgLock = cfgLock->isChecked(),
            .c3StateAutodemotionEnable = c3StateAutodemotionEnable->isChecked(),
            .c1StateAutodemotionEnable = c1StateAutodemotionEnable->isChecked(),
            .c3UndemotionEnable = c3UndemotionEnable->isChecked(),
            .c1UndemotionEnable = c1UndemotionEnable->isChecked()
        };
    }

    void PKGCSTConfigControlSBCoreWidget::applyCFGLock(const bool biosLock) const {
        const bool unlocked = !cfgLock->isChecked() && !biosLock;

        cfgLock->setEnabled(!biosLock);
        pkgCStateLimit->setEnabled(unlocked);
        ioMwaitRedirectionEnable->setEnabled(unlocked);
    }

    void PKGCSTConfigControlSBCoreWidget::onCfgLockStateChanged(const Qt::CheckState state) const {
        const bool locked = state == Qt::Checked;

        pkgCStateLimit->setEnabled(!locked);
        ioMwaitRedirectionEnable->setEnabled(!locked);
    }
}
