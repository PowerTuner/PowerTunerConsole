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

#include "DaemonOptions.h"
#include "../../Widgets/SectionWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI {
    DaemonOptions::DaemonOptions() {
        QVBoxLayout *lyt = new QVBoxLayout();

        daemonSettings.reset(new PWTS::DaemonSettings);

        daemonStartProfile = new ConsoleSelect("Daemon start profile");
        daemonBatProfile = new ConsoleSelect("Battery profile");
        daemonPSupplyProfile = new ConsoleSelect("Power supply profile");
        daemonApplyInterval = new ConsoleSlider("Re-apply interval");
        daemonListenAdr = new ConsoleLineEdit("Listen address");
        daemonTcpPort = new ConsoleSpinBox("TCP port");
        daemonUdpPort = new ConsoleSpinBox("UDP port");
        daemonLogLevel = new ConsoleSelect("Log level");
        daemonMaxLogFiles = new ConsoleSlider("Max log files");
        daemonApplyOnWake = new ConsoleCheckbox("Re-apply on wake from sleep", "Re-apply latest settings on wake from sleep");
        daemonIgnoreBatEvt = new ConsoleCheckbox("Ignore battery events", "Do not automatically switch profile on battery status change");
        saveDaemonSettBtn = new ConsoleButton("Save");
        reloadDaemonSettBtn = new ConsoleButton("Reload");

        daemonApplyInterval->setRange(PWTS::DaemonSettings::MinApplyInterval, PWTS::DaemonSettings::MaxApplyInterval);
        onDaemonApplyIntervalChanged(0);
        daemonListenAdr->setPlaceholder("any (address|localhost|any)");
        daemonTcpPort->setRange(1000, UINT16_MAX);
        daemonUdpPort->setRange(1000, UINT16_MAX);
        daemonLogLevel->addOptions({"Info", "Warning", "Error", "Service", "None"});
        daemonMaxLogFiles->setRange(1, 40);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new SectionWidget("Daemon settings", SectionWidget::Size::Big));
        lyt->addWidget(daemonStartProfile);
        lyt->addWidget(daemonBatProfile);
        lyt->addWidget(daemonPSupplyProfile);
        lyt->addWidget(daemonApplyInterval);
        lyt->addWidget(daemonListenAdr);
        lyt->addWidget(daemonTcpPort);
        lyt->addWidget(daemonUdpPort);
        lyt->addWidget(daemonLogLevel);
        lyt->addWidget(daemonMaxLogFiles);
        lyt->addWidget(daemonApplyOnWake);
        lyt->addWidget(daemonIgnoreBatEvt);
        lyt->addWidget(saveDaemonSettBtn);
        lyt->addWidget(reloadDaemonSettBtn);

        setLayout(lyt);
        setEnabled(false);

        QObject::connect(daemonApplyInterval, &ConsoleSlider::valueChanged, this, &DaemonOptions::onDaemonApplyIntervalChanged);
        QObject::connect(saveDaemonSettBtn, &ConsoleButton::clicked, this, &DaemonOptions::onSaveDaemonSettingsClicked);
        QObject::connect(reloadDaemonSettBtn, &ConsoleButton::clicked, this, &DaemonOptions::onReloadDaemonSettingsClicked);
    }

    void DaemonOptions::updateDaemonSettings(const QByteArray &data) const {
        if (!daemonSettings->load(data)) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Console: failed to load daemon settings, using defaults"));
            return;
        }

        daemonStartProfile->setCurrentIndex(daemonStartProfile->indexOf(daemonSettings->getOnStartProfile()));
        daemonBatProfile->setCurrentIndex(daemonBatProfile->indexOf(daemonSettings->getOnBatteryProfile()));
        daemonPSupplyProfile->setCurrentIndex(daemonPSupplyProfile->indexOf(daemonSettings->getOnPowerSupplyProfile()));
        daemonApplyInterval->setValue(daemonSettings->getApplyInterval());
        daemonListenAdr->setText(daemonSettings->getAddress());
        daemonTcpPort->setValue(daemonSettings->getSocketTcpPort());
        daemonUdpPort->setValue(daemonSettings->getSocketUdpPort());
        daemonApplyOnWake->setChecked(daemonSettings->getApplyOnWakeFromSleep());
        daemonIgnoreBatEvt->setChecked(daemonSettings->getIgnoreBatteryEvent());
        daemonLogLevel->setCurrentIndex(static_cast<int>(daemonSettings->getLogLevel()));
        daemonMaxLogFiles->setValue(daemonSettings->getMaxLogFiles());
    }

    void DaemonOptions::updateProfileList(const QList<QString> &list) const {
        for (ConsoleSelect *sel: {daemonStartProfile, daemonBatProfile, daemonPSupplyProfile}) {
            sel->clear();
            sel->addOption("");
            sel->addOptions(list);
        }

        daemonStartProfile->setCurrentIndex(daemonStartProfile->indexOf(daemonSettings->getOnStartProfile()));
        daemonBatProfile->setCurrentIndex(daemonBatProfile->indexOf(daemonSettings->getOnBatteryProfile()));
        daemonPSupplyProfile->setCurrentIndex(daemonPSupplyProfile->indexOf(daemonSettings->getOnPowerSupplyProfile()));
    }

    void DaemonOptions::clearWidget() const {
        daemonStartProfile->clear();
        daemonBatProfile->clear();
        daemonPSupplyProfile->clear();
        daemonApplyInterval->setValue(0);
        daemonListenAdr->clear();
        daemonTcpPort->setValue(PWTS::DaemonSettings::DefaultTCPPort);
        daemonUdpPort->setValue(PWTS::DaemonSettings::DefaultUDPPort);
        daemonApplyOnWake->setChecked(false);
        daemonIgnoreBatEvt->setChecked(false);
        daemonLogLevel->setCurrentIndex(2);
        daemonMaxLogFiles->setValue(5);
    }

    void DaemonOptions::onDaemonApplyIntervalChanged(const int v) const {
        const int val = v > 59 ? (v / 60) : v;
        const QString unit = v > 59 ? "min":"sec";
        const QString time = v == 0 ? "Once":"Every";

        daemonApplyInterval->setLabel(QString("%1 [%2 %3]").arg(time).arg(val).arg(unit));
    }

    void DaemonOptions::onReloadDaemonSettingsClicked() {
        emit reloadDaemonSettings();
    }

    void DaemonOptions::onSaveDaemonSettingsClicked() {
        daemonSettings->setOnStartProfile(daemonStartProfile->getCurrentText());
        daemonSettings->setOnBatteryProfile(daemonBatProfile->getCurrentText());
        daemonSettings->setOnPowerSupplyProfile(daemonPSupplyProfile->getCurrentText());
        daemonSettings->setApplyInterval(daemonApplyInterval->getValue());
        daemonSettings->setAddress(daemonListenAdr->getText());
        daemonSettings->setSocketTcpPort(daemonTcpPort->getValue());
        daemonSettings->setSocketUdpPort(daemonUdpPort->getValue());
        daemonSettings->setApplyOnWakeFromSleep(daemonApplyOnWake->isChecked());
        daemonSettings->setIgnoreBatteryEvent(daemonIgnoreBatEvt->isChecked());
        daemonSettings->setLogLevel(static_cast<PWTS::LogLevel>(daemonLogLevel->getCurrentIndex()));
        daemonSettings->setMaxLogFiles(daemonMaxLogFiles->getValue());
        emit applyDaemonSettings(daemonSettings->getData());
    }
}
