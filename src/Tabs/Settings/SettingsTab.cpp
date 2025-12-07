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
#include "SettingsTab.h"

namespace PWT::CUI {
    SettingsTab::SettingsTab() {
        clientOptions = new ClientOptions();
        addDaemonOptions = new AddDaemonOptions();
        daemonOptions = new DaemonOptions();

        scrollWidgLyt->insertWidget(nextInsertIdx(), clientOptions);
        scrollWidgLyt->insertWidget(nextInsertIdx(), addDaemonOptions);
        scrollWidgLyt->insertWidget(nextInsertIdx(), daemonOptions);

        setupFocusableWidgets();

        QObject::connect(clientOptions, &ClientOptions::reloadGamepadMappings, this, &SettingsTab::onReloadGamepadMappings);
        QObject::connect(clientOptions, &ClientOptions::clientSettingsSaved, this, &SettingsTab::onSaveClientSettings);
        QObject::connect(addDaemonOptions, &AddDaemonOptions::daemonAdded, this, &SettingsTab::onDaemonAdded);
        QObject::connect(daemonOptions, &DaemonOptions::reloadDaemonSettings, this, &SettingsTab::onReloadDaemonSettings);
        QObject::connect(daemonOptions, &DaemonOptions::applyDaemonSettings, this, &SettingsTab::onSaveDaemonSettings);
    }

    void SettingsTab::init(const PWTS::DeviceInfoPacket &packet) const {
        daemonOptions->updateDaemonSettings(packet.daemonSettings);
        daemonOptions->setEnabled(true);
    }

    void SettingsTab::refreshTab(const PWTS::DaemonPacket &packet) const {
        daemonOptions->updateProfileList(packet.profilesList);
    }

    void SettingsTab::updateProfileList(const QList<QString> &list) const {
        daemonOptions->updateProfileList(list);
    }

    void SettingsTab::updateDaemonSettings(const QByteArray &data) const {
        daemonOptions->updateDaemonSettings(data);
    }

    void SettingsTab::addGamepad(const QString &gamepadName, const QString &gamepadID) const {
        clientOptions->addGamepad(gamepadName, gamepadID);
    }

    void SettingsTab::removeGamepad(const QString &gamepadID) const {
        clientOptions->removeGamepad(gamepadID);
    }

    QString SettingsTab::getFirstGamepadID() const {
        return clientOptions->getFirstGamepadID();
    }

    void SettingsTab::initAutoConnectDaemonList() const {
        clientOptions->initAutoConnectDaemonList();
    }

    void SettingsTab::loadConsoleSettings() const {
        clientOptions->initAutoConnectDaemonList();
        clientOptions->loadSettings();
    }

    void SettingsTab::setDaemonOptionsEnabled(const bool enable) const {
        daemonOptions->setEnabled(enable);
    }

    void SettingsTab::clearWidget() const {
        daemonOptions->clearWidget();
        daemonOptions->setEnabled(false);
    }

    void SettingsTab::onReloadGamepadMappings() {
        emit reloadGamepadMappings();
    }

    void SettingsTab::onSaveClientSettings() {
        emit appSettingsSaved();
    }

    void SettingsTab::onDaemonAdded(const Daemon &daemon) {
        emit daemonAdded(daemon);
    }

    void SettingsTab::onReloadDaemonSettings() {
        emit reloadDaemonSettings();
    }

    void SettingsTab::onSaveDaemonSettings(const QByteArray &data) {
        emit applyDaemonSettings(data);
    }
}
