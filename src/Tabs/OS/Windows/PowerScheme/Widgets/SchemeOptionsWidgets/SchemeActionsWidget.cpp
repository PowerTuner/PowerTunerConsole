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

#include "SchemeActionsWidget.h"

namespace PWT::CUI::WIN {
    SchemeActionsWidget::SchemeActionsWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        setActive = new ConsoleCheckbox("Set active");
        deleteFlag = new ConsoleCheckbox("Flag for deletion");
        resetFlag = new ConsoleCheckbox("Reset values to defaults", "Reset scheme to defaults from default user scheme stored in HKEY_USERS\\.Default.");
        deleteBtn = new ConsoleButton("Delete scheme");

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(setActive);
        lyt->addWidget(deleteFlag);
        lyt->addWidget(resetFlag);
        lyt->addWidget(deleteBtn);

        setLayout(lyt);

        QObject::connect(setActive, &ConsoleCheckbox::checkStateChanged, this, &SchemeActionsWidget::onSetActiveChanged);
        QObject::connect(deleteFlag, &ConsoleCheckbox::checkStateChanged, this, &SchemeActionsWidget::onDeleteFlagChanged);
        QObject::connect(resetFlag, &ConsoleCheckbox::checkStateChanged, this, &SchemeActionsWidget::onResetFlagChanged);
        QObject::connect(deleteBtn, &ConsoleButton::clicked, this, &SchemeActionsWidget::onDeleteBtnClicked);
    }

    void SchemeActionsWidget::switchScheme(const QString &scheme, const bool isActive, const bool deleteFlagged, const bool resetFlagged) const {
        const bool isTBD = scheme.startsWith("{guid-tbd");
        const QSignalBlocker sblockAct {setActive};
        const QSignalBlocker sblockDelFlag {deleteFlag};

        setActive->setChecked(isActive);
        setActive->setEnabled(!isActive && !deleteFlagged);
        deleteFlag->setChecked(deleteFlagged);
        deleteFlag->setVisible(!isActive && !isTBD);
        resetFlag->setChecked(resetFlagged);
        resetFlag->setVisible(!isTBD);
        deleteBtn->setVisible(!isActive && isTBD);
    }

    void SchemeActionsWidget::onDeleteFlagChanged(const Qt::CheckState state) {
        const bool flagged = state == Qt::Checked;

        setActive->setEnabled(!flagged);
        emit setDeleteFlag(flagged);
    }

    void SchemeActionsWidget::onResetFlagChanged(const Qt::CheckState state) {
        emit setResetFlag(state == Qt::Checked);
    }

    void SchemeActionsWidget::onSetActiveChanged(const Qt::CheckState state) {
        if (state != Qt::Checked)
            return;

        setActive->setEnabled(false);
        deleteFlag->setVisible(false);
        deleteBtn->setVisible(false);
        emit setSchemeActive();
    }

    void SchemeActionsWidget::onDeleteBtnClicked() {
        emit deleteScheme();
    }
}
