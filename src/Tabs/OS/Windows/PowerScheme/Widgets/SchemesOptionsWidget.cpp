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

#include "SchemesOptionsWidget.h"

namespace PWT::CUI::WIN {
    SchemesOptionsWidget::SchemesOptionsWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        currentSection = new SectionWidget("Current scheme options", SectionWidget::Size::Big);
        miscGlobalOptsWidget = new MiscGlobalOptionsWidget();
        copySchemeSettingsWidget = new CopySchemeSettingsWidget();
        duplicateSchemeWidget = new DuplicateSchemeWidget();
        schemeActionsWidget = new SchemeActionsWidget();

        setCurrentSettingsVisible(false);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new SectionWidget("Global options", SectionWidget::Size::Big));
        lyt->addWidget(new SectionWidget("Misc. options"));
        lyt->addWidget(miscGlobalOptsWidget);
        lyt->addWidget(new SectionWidget("Copy scheme settings"));
        lyt->addWidget(copySchemeSettingsWidget);
        lyt->addWidget(new SectionWidget("Duplicate scheme"));
        lyt->addWidget(duplicateSchemeWidget);
        lyt->addWidget(currentSection);
        lyt->addWidget(new SectionWidget("Scheme actions"));
        lyt->addWidget(schemeActionsWidget);

        setLayout(lyt);

        QObject::connect(copySchemeSettingsWidget, &CopySchemeSettingsWidget::copySettings, this, &SchemesOptionsWidget::onCopySettings);
        QObject::connect(duplicateSchemeWidget, &DuplicateSchemeWidget::createDuplicate, this, &SchemesOptionsWidget::onCreateDuplicate);
        QObject::connect(schemeActionsWidget, &SchemeActionsWidget::setSchemeActive, this, &SchemesOptionsWidget::onSetSchemeActive);
        QObject::connect(schemeActionsWidget, &SchemeActionsWidget::setDeleteFlag, this, &SchemesOptionsWidget::onSetDeleteFlag);
        QObject::connect(schemeActionsWidget, &SchemeActionsWidget::setResetFlag, this, &SchemesOptionsWidget::onSetResetFlag);
        QObject::connect(schemeActionsWidget, &SchemeActionsWidget::deleteScheme, this, &SchemesOptionsWidget::onDeleteScheme);
    }

    void SchemesOptionsWidget::refreshWidget(const PWTS::DaemonPacket &packet) const {
        miscGlobalOptsWidget->refreshWidget(packet);
    }

    void SchemesOptionsWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.windowsData->resetSchemesDefault = miscGlobalOptsWidget->getResetSchemes();
        packet.windowsData->replaceDefaultSchemes = miscGlobalOptsWidget->getReplaceDefaultSchemes();
    }

    void SchemesOptionsWidget::setCurrentSettingsVisible(const bool visible) const {
        currentSection->setVisible(visible);
        schemeActionsWidget->setVisible(visible);
    }

    void SchemesOptionsWidget::clearWidget() const {
        copySchemeSettingsWidget->clearWidget();
        duplicateSchemeWidget->clearWidget();
    }

    void SchemesOptionsWidget::addScheme(const QString &label) const {
        copySchemeSettingsWidget->addScheme(label);
        duplicateSchemeWidget->addScheme(label);
    }

    void SchemesOptionsWidget::addDuplicatedScheme(const QString &label) const {
        copySchemeSettingsWidget->addScheme(label);
    }

    void SchemesOptionsWidget::switchScheme(const QString &scheme, const bool isActive, const bool deleteFlag, const bool resetFlag) const {
        schemeActionsWidget->switchScheme(scheme, isActive, deleteFlag, resetFlag);
    }

    void SchemesOptionsWidget::removeScheme(const QString &label) const {
        copySchemeSettingsWidget->removeScheme(label);
        duplicateSchemeWidget->removeScheme(label);
    }

    void SchemesOptionsWidget::onSetSchemeActive() {
        emit setSchemeActive();
    }

    void SchemesOptionsWidget::onCreateDuplicate(const QString &baseScheme, const QString &duplicateName) {
        emit createDuplicate(baseScheme, duplicateName);
    }

    void SchemesOptionsWidget::onDeleteScheme() {
        emit deleteScheme();
    }

    void SchemesOptionsWidget::onSetDeleteFlag(const bool flagged) {
        emit setDeleteFlag(flagged);
    }

    void SchemesOptionsWidget::onSetResetFlag(const bool flagged) {
        emit setResetFlag(flagged);
    }

    void SchemesOptionsWidget::onCopySettings(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) {
        emit copySettings(fromScheme, fromMode, toScheme, toMode);
    }
}
