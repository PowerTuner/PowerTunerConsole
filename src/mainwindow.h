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
#pragma once

#include <QMainWindow>
#include <QScopedPointer>

#include "pwtClientService/ClientService.h"
#include "pwtClientCommon/InputRanges/InputRanges.h"
#include "Classes/GamepadWorker.h"
#include "Classes/TTSWorker.h"
#include "Tabs/Include/AddonTab.h"
#include "Widgets/StatusBar.h"
#include "Widgets/TrayIcon.h"
#include "Widgets/FloatingIcon.h"
#include "Tabs/Home/HomeTab.h"
#include "Tabs/Log/LogTab.h"
#include "Tabs/Daemons/DaemonsTab.h"
#include "Tabs/Settings/SettingsTab.h"
#include "Tabs/Profiles/ProfilesTab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final: public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QSharedPointer<PWT::ConsoleSettings> consoleSettings;
    QScopedPointer<PWTCS::ClientService> service;
    QSharedPointer<PWT::UI::InputRanges> inputRanges;
    mutable QPointer<PWT::CUI::ConsoleTab> currentTab;
    QPointer<PWT::CUI::TrayIcon> trayIcon;
    QPointer<PWT::CUI::FloatingIcon> floatIcon;
    QList<QPointer<PWT::CUI::AddonTab>> addonTabList;
    QScopedPointer<QTimer> windowEventTimer;
    QString appDataPath;
    PWT::GamepadMap activeGamepadMap;
    QString activeGamepad;
    QString prevGamepadID;
    QThread gamepadThread;
    QPointer<QThread> ttsThread;
    PWT::CUI::GamepadWorker *gamepadWorker = nullptr;
    PWT::CUI::TTSWorker *ttsWorker = nullptr;
    PWT::CUI::StatusBar *statusBar = nullptr;
    PWT::CUI::HomeTab *homeTab = nullptr;
    PWT::CUI::LogTab *logTab = nullptr;
    PWT::CUI::DaemonsTab *daemonsTab = nullptr;
    PWT::CUI::SettingsTab *settingsTab = nullptr;
    PWT::CUI::ProfilesTab *profilesTab = nullptr;
    QTabWidget *tabWidget = nullptr;
    mutable bool locked = true;
    bool appFocused = false;
    bool appMaximized = false;
    PWTS::CPUVendor cpuVendor = PWTS::CPUVendor::Unknown;
    PWTS::OSType osType = PWTS::OSType::Unknown;
    int cpuCoreCount = 0;
    int cpuThreadCount = 0;
    int insertTabIdx = 1;
    int unreadLogs = 0;

    [[nodiscard]] int nextTabInsertIdx() { return insertTabIdx++; }

    void printErrors(const QSet<PWTS::DError> &errors);
    void loadConsoleSettings();
    void loadControllerDB();
    void initTrayIcon();
    void initFloatIcon();
    void initTTS();
    void disableTTS();
    void quitApp() const;
    void setActiveGamepad(const QString &gamepadID);
    void initMain(const PWTS::DeviceInfoPacket &packet);
    void initLinuxIntelGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, int index, const QString &marketName);
    void initLinuxAMDGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, int index, const QString &marketName);
    void initLinuxTabs(const PWTS::DeviceInfoPacket &packet);
    void initWindowsTabs(const PWTS::DeviceInfoPacket &packet);
    void initFanControlTab(const PWTS::DeviceInfoPacket &packet);
#ifdef WITH_INTEL
    void initIntelTabs(const PWTS::DeviceInfoPacket &packet);
#endif
#ifdef WITH_AMD
    void initAMDTabs(const PWTS::DeviceInfoPacket &packet);
#endif
    void deleteAddonTabs();
    void upDownAction(bool isDown) const;
    void prevNextAction(bool isNext) const;
    void connectServiceSignals() const;
    void disconnectServiceSignals() const;
    void completeServiceDisconnection();
    void lockUI(bool lock) const;
    void refreshTabsData(const PWTS::DaemonPacket &packet) const;
    PWTS::ClientPacket createClientPacket() const;
    void reloadFromDaemon() const;
    void applyCurrentSettings() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void closeEvent(QCloseEvent *e) override;

private slots:
    void onLogMessageSent(const QString &msg);
    void onReloadGamepadMappings();
    void onConsoleSettingsSaved();
    void onShowMainWindow();
    void onWindowEventTimerTimeout() const;
    void onQuitApp() const;
    void onFloatIconPositionChanged(const QPointF &pos) const;
    void onDaemonAdded(const PWT::Daemon &daemon) const;
    void onTabChanged(int idx);
    void onDaemonConnectionRequested(const QString &adr, quint16 port);
    void onDaemonDisconnectionRequested();
    void onDaemonDeleted(const PWT::Daemon &data);
    void onApplyDaemonSettings(const QByteArray &data) const;
    void onReloadDaemonSettings() const;
    void onServiceError();
    void onServiceCommandFailed();
    void onServiceConnected();
    void onServiceDisconnected();
    void onServiceDeviceInfoPacketReceived(const PWTS::DeviceInfoPacket &packet);
    void onServiceDaemonPacketReceived(const PWTS::DaemonPacket &packet);
    void onServiceSettingsApplied(const QSet<PWTS::DError> &errors);
    void onServiceDaemonSettingsApplied(bool diskSaveResult);
    void onServiceDaemonSettingsReceived(const QByteArray &data) const;
    void onServiceBatteryStatusChanged(const QSet<PWTS::DError> &errors, const QString &profile);
    void onServiceWakeFromSleepEvent(const QSet<PWTS::DError> &errors);
    void onServiceApplyTimerTick(const QSet<PWTS::DError> &errors);
    void onServiceProfileWritten(bool result);
    void onServiceProfileDeleted(bool result);
    void onServiceProfileApplied(const QSet<PWTS::DError> &errors, const QString &name);
    void onProfileListReceived(const QList<QString> &profileList);
    void onProfileCreated(const QString &name) const;
    void onProfileDeleted(const QString &name) const;
    void onProfileSaved(const QString &name) const;
    void onProfileLoaded(const QString &name) const;
    void onProfileApplied(const QString &name) const;
    void onGamepadConnected(const QString &gamepadLbl, const QString &gamepadID);
    void onGamepadDisconnected(const QString &gamepadID);
    void onGamepadButton(const QString &gamepadID, PWT::GamepadButton button);
    void onSayTTS(const QString &msg, PWT::CUI::TTSID id);
    void onSayStoppableTTS(const QString &msg);

public slots:
    void onAppStateChanged(Qt::ApplicationState state);

signals:
    void activeGamepadSet(const QString &gamepadID);
    void allGamepadsModeChange(bool enable);
    void deadzoneUpdated(const QString &gamepadID, int deadzone);
    void deadzoneDataUpdated(const QList<std::pair<QString, int>> &data);
    void focusChanged(bool focused);
    void pauseStoppableTTS(bool pause);
    void sayTTS(const QString &msg, PWT::CUI::TTSID id);
    void sayStoppableTTS(const QString &msg);
};
