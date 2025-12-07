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

#include "PKGCSTConfigControlCU1CoreWidget.h"

namespace PWT::CUI::INTEL {
    PKGCSTConfigControlCU1CoreWidget::PKGCSTConfigControlCU1CoreWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        pkgCStateLimit = new ConsoleSelect("Package C-State Limit");
        maxCoreCState = new ConsoleSelect("Max core C-State");
        ioMwaitRedirectionEnable = new ConsoleCheckbox("I/O MWAIT redirection enable");
        cfgLock = new ConsoleCheckbox("CFG lock");
        c3StateAutodemotionEnable = new ConsoleCheckbox("C3 state auto-demotion enable");
        c1StateAutodemotionEnable = new ConsoleCheckbox("C1 state auto-demotion enable");
        c3UndemotionEnable = new ConsoleCheckbox("C3 un-demotion enable");
        c1UndemotionEnable = new ConsoleCheckbox("C1 un-demotion enable");
        pkgcAutodemotionEnable = new ConsoleCheckbox("Package C-State auto-demotion enable");
        pkgcUndemotionEnable = new ConsoleCheckbox("Package C-State un-demotion enable");
        timedMwaitEnable = new ConsoleCheckbox("Timed MWAIT enable");

        pkgCStateLimit->addOption("C0/C1 (no package C-sate support)", 0b0000);
        pkgCStateLimit->addOption("C2", 0b0001);
        pkgCStateLimit->addOption("C3", 0b0010);
        pkgCStateLimit->addOption("C6", 0b0011);
        pkgCStateLimit->addOption("C7", 0b0100);
        pkgCStateLimit->addOption("C7s", 0b0101);
        pkgCStateLimit->addOption("C8", 0b0110);
        pkgCStateLimit->addOption("C9", 0b0111);
        pkgCStateLimit->addOption("C10", 0b1000);
        maxCoreCState->addOption("C1", 0b0001);
        maxCoreCState->addOption("C3", 0b0010);
        maxCoreCState->addOption("C6", 0b0011);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(pkgCStateLimit);
        lyt->addWidget(maxCoreCState);
        lyt->addWidget(ioMwaitRedirectionEnable);
        lyt->addWidget(cfgLock);
        lyt->addWidget(c3StateAutodemotionEnable);
        lyt->addWidget(c1StateAutodemotionEnable);
        lyt->addWidget(c3UndemotionEnable);
        lyt->addWidget(c1UndemotionEnable);
        lyt->addWidget(pkgcAutodemotionEnable);
        lyt->addWidget(pkgcUndemotionEnable);
        lyt->addWidget(timedMwaitEnable);

        setLayout(lyt);

        QObject::connect(cfgLock, &ConsoleCheckbox::checkStateChanged, this, &PKGCSTConfigControlCU1CoreWidget::onCfgLockStateChanged);
    }

    void PKGCSTConfigControlCU1CoreWidget::setData(const PWTS::Intel::PkgCstConfigControl &data, const bool isBiosLocked) const {
        const QSignalBlocker sblock {cfgLock};

        pkgCStateLimit->setCurrentIndex(pkgCStateLimit->indexOf(data.packageCStateLimit));
        maxCoreCState->setCurrentIndex(maxCoreCState->indexOf(data.maxCoreCState));
        ioMwaitRedirectionEnable->setChecked(data.ioMwaitRedirectionEnable);
        cfgLock->setChecked(data.cfgLock);
        c3StateAutodemotionEnable->setChecked(data.c3StateAutodemotionEnable);
        c1StateAutodemotionEnable->setChecked(data.c1StateAutodemotionEnable);
        c3UndemotionEnable->setChecked(data.c3UndemotionEnable);
        c1UndemotionEnable->setChecked(data.c1UndemotionEnable);
        pkgcAutodemotionEnable->setChecked(data.pkgcAutodemotionEnable);
        pkgcUndemotionEnable->setChecked(data.pkgcUndemotionEnable);
        timedMwaitEnable->setChecked(data.timedMwaitEnable);
        applyCFGLock(isBiosLocked);
    }

    PWTS::Intel::PkgCstConfigControl PKGCSTConfigControlCU1CoreWidget::getData() const {
        return {
            .packageCStateLimit = static_cast<short>(pkgCStateLimit->getCurrentData().toInt()),
            .maxCoreCState = static_cast<short>(maxCoreCState->getCurrentData().toInt()),
            .ioMwaitRedirectionEnable = ioMwaitRedirectionEnable->isChecked(),
            .cfgLock = cfgLock->isChecked(),
            .c3StateAutodemotionEnable = c3StateAutodemotionEnable->isChecked(),
            .c1StateAutodemotionEnable = c1StateAutodemotionEnable->isChecked(),
            .c3UndemotionEnable = c3UndemotionEnable->isChecked(),
            .c1UndemotionEnable = c1UndemotionEnable->isChecked(),
            .pkgcAutodemotionEnable = pkgcAutodemotionEnable->isChecked(),
            .pkgcUndemotionEnable = pkgcUndemotionEnable->isChecked(),
            .timedMwaitEnable = timedMwaitEnable->isChecked()
        };
    }

    void PKGCSTConfigControlCU1CoreWidget::applyCFGLock(const bool biosLock) const {
        const bool unlocked = !cfgLock->isChecked() && !biosLock;

        cfgLock->setEnabled(!biosLock);
        pkgCStateLimit->setEnabled(unlocked);
        maxCoreCState->setEnabled(unlocked);
        ioMwaitRedirectionEnable->setEnabled(unlocked);
    }

    void PKGCSTConfigControlCU1CoreWidget::onCfgLockStateChanged(const Qt::CheckState state) const {
        const bool locked = state == Qt::Checked;

        pkgCStateLimit->setEnabled(!locked);
        maxCoreCState->setEnabled(!locked);
        ioMwaitRedirectionEnable->setEnabled(!locked);
    }
}
