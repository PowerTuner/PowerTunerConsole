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
#include <QStandardPaths>
#include <QDir>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ConsoleStyle.h"
#include "../version.h"
#include "external/SDL/include/SDL3/SDL_init.h"
#include "external/SDL/include/SDL3/SDL_hints.h"
#include "pwtShared/Utils.h"
#include "pwtClientCommon/CommonUtils.h"
#include "pwtClientCommon/UILogger.h"
#include "Tabs/OS/Linux/SysFs/SysFsTab.h"
#include "Tabs/OS/Linux/IntelGPU/IntelGPUTab.h"
#include "Tabs/OS/Linux/AMDGPU/AMDGPUTab.h"
#include "Tabs/OS/Windows/PowerScheme/PowerSchemeTab.h"
#include "Tabs/FanControl/FanControlTab.h"
#ifdef WITH_INTEL
#include "Tabs/Vendor/Intel/CPU/CPUTab.h"
#include "Tabs/Vendor/Intel/HWP/HWPTab.h"
#include "Tabs/Vendor/Intel/MCHBAR/MCHBARTab.h"
#endif
#ifdef WITH_AMD
#include "Tabs/Vendor/AMD/RYCPU/RYCPUTab.h"
#include "Tabs/Vendor/AMD/CPU/CPUTab.h"
#endif

MainWindow::~MainWindow() {
    gamepadThread.quit();
    gamepadThread.wait();
    SDL_Quit();
    disableTTS();

    if (!trayIcon.isNull())
        delete trayIcon;

    if (!floatIcon.isNull())
        delete floatIcon;

    delete ui;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(QString("PowerTuner Console %1.%2").arg(CLIENT_VER_MAJOR).arg(CLIENT_VER_MINOR));
    setWindowIcon(QIcon(":/ico/pwt"));

    const QString globalDataPath = QString("%1/PowerTuner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    QVBoxLayout *lyt = new QVBoxLayout();
    QFont appFont = font();
    const QDir qdir;

    appDataPath = QString("%1/console").arg(globalDataPath);

    if (!qdir.exists(appDataPath) && !qdir.mkpath(appDataPath))
        appDataPath.clear();

    appFont.setPointSize(12);
    appFont.setWeight(QFont::Medium);
    setFont(appFont);

    service.reset(new PWTCS::ClientService);
    windowEventTimer.reset(new QTimer);

    consoleSettings = PWT::ConsoleSettings::getInstance();
    inputRanges = PWT::UI::InputRanges::getInstance();
    statusBar = new PWT::CUI::StatusBar();
    tabWidget = new QTabWidget();
    homeTab = new PWT::CUI::HomeTab();
    logTab = new PWT::CUI::LogTab();
    daemonsTab = new PWT::CUI::DaemonsTab();
    settingsTab = new PWT::CUI::SettingsTab();
    profilesTab = new PWT::CUI::ProfilesTab();

    consoleSettings->setAppDataPath(appDataPath);
    inputRanges->setAppDataPath(globalDataPath);
    tabWidget->setPalette(PWT::CUI::ConsoleStyle::getPalette());
    tabWidget->addTab(homeTab, "Home");
    tabWidget->addTab(logTab, "Log");
    tabWidget->addTab(daemonsTab, "Daemons");
    tabWidget->addTab(settingsTab, "Settings");
    tabWidget->addTab(profilesTab, "Profiles");
    tabWidget->setCurrentIndex(0);
    tabWidget->setFocusPolicy(Qt::NoFocus);
    ui->centralwidget->setFocusPolicy(Qt::NoFocus);
    onTabChanged(0);
    SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "0");
    windowEventTimer->setSingleShot(true);
    windowEventTimer->setInterval(650);

    lyt->addWidget(tabWidget);
    lyt->addWidget(statusBar);
    lyt->setContentsMargins(0, 0, 0, 0);
    ui->centralwidget->setLayout(lyt);

    if (!appDataPath.isEmpty())
        onLogMessageSent(QString("Console client data path: %1").arg(appDataPath));

    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        onLogMessageSent("Failed to init SDL gamepad subsystem");

    } else {
        gamepadWorker = new PWT::CUI::GamepadWorker();

        gamepadWorker->moveToThread(&gamepadThread);

        QObject::connect(&gamepadThread, &QThread::finished, gamepadWorker, &QObject::deleteLater);
        QObject::connect(&gamepadThread, &QThread::started, gamepadWorker, &PWT::CUI::GamepadWorker::startSDLEventsThread);
        QObject::connect(gamepadWorker, &PWT::CUI::GamepadWorker::logMessageSent, this, &MainWindow::onLogMessageSent);
        QObject::connect(gamepadWorker, &PWT::CUI::GamepadWorker::gamepadConnected, this, &MainWindow::onGamepadConnected);
        QObject::connect(gamepadWorker, &PWT::CUI::GamepadWorker::gamepadDisconnected, this, &MainWindow::onGamepadDisconnected);
        QObject::connect(gamepadWorker, &PWT::CUI::GamepadWorker::gamepadButton, this, &MainWindow::onGamepadButton);
        QObject::connect(windowEventTimer.get(), &QTimer::timeout, this, &MainWindow::onWindowEventTimerTimeout);
        QObject::connect(this, &MainWindow::activeGamepadSet, gamepadWorker, &PWT::CUI::GamepadWorker::setActiveID);
        QObject::connect(this, &MainWindow::allGamepadsModeChange, gamepadWorker, &PWT::CUI::GamepadWorker::setAllGamepadsMode);
        QObject::connect(this, &MainWindow::deadzoneUpdated, gamepadWorker, &PWT::CUI::GamepadWorker::setDeadzone);
        QObject::connect(this, &MainWindow::deadzoneDataUpdated, gamepadWorker, &PWT::CUI::GamepadWorker::setGamepadsDeadzoneData);
        QObject::connect(this, &MainWindow::focusChanged, gamepadWorker, &PWT::CUI::GamepadWorker::focusChange);

        loadControllerDB();
        gamepadThread.start();
    }

    QObject::connect(PWT::UI::UILogger::getInstance().get(), &PWT::UI::UILogger::logWritten, this, &MainWindow::onLogMessageSent);
    QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    QObject::connect(homeTab, &PWT::CUI::HomeTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    QObject::connect(daemonsTab, &PWT::CUI::DaemonsTab::daemonConnectionRequest, this, &MainWindow::onDaemonConnectionRequested);
    QObject::connect(daemonsTab, &PWT::CUI::DaemonsTab::daemonDisconnectionRequest, this, &MainWindow::onDaemonDisconnectionRequested);
    QObject::connect(daemonsTab, &PWT::CUI::DaemonsTab::daemonDeleted, this, &MainWindow::onDaemonDeleted);
    QObject::connect(daemonsTab, &PWT::CUI::DaemonsTab::sayTTS, this, &MainWindow::onSayTTS);
    QObject::connect(daemonsTab, &PWT::CUI::DaemonsTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    QObject::connect(logTab, &PWT::CUI::LogTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    QObject::connect(settingsTab, &PWT::CUI::SettingsTab::reloadGamepadMappings, this, &MainWindow::onReloadGamepadMappings);
    QObject::connect(settingsTab, &PWT::CUI::SettingsTab::appSettingsSaved, this, &MainWindow::onConsoleSettingsSaved);
    QObject::connect(settingsTab, &PWT::CUI::SettingsTab::daemonAdded, this, &MainWindow::onDaemonAdded);
    QObject::connect(settingsTab, &PWT::CUI::SettingsTab::applyDaemonSettings, this, &MainWindow::onApplyDaemonSettings);
    QObject::connect(settingsTab, &PWT::CUI::SettingsTab::reloadDaemonSettings, this, &MainWindow::onReloadDaemonSettings);
    QObject::connect(settingsTab, &PWT::CUI::SettingsTab::sayTTS, this, &MainWindow::onSayTTS);
    QObject::connect(settingsTab, &PWT::CUI::SettingsTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    QObject::connect(profilesTab, &PWT::CUI::ProfilesTab::profileLoaded, this, &MainWindow::onProfileLoaded);
    QObject::connect(profilesTab, &PWT::CUI::ProfilesTab::profileApplied, this, &MainWindow::onProfileApplied);
    QObject::connect(profilesTab, &PWT::CUI::ProfilesTab::profileSaved, this, &MainWindow::onProfileSaved);
    QObject::connect(profilesTab, &PWT::CUI::ProfilesTab::profileDeleted, this, &MainWindow::onProfileDeleted);
    QObject::connect(profilesTab, &PWT::CUI::ProfilesTab::profileCreated, this, &MainWindow::onProfileCreated);
    QObject::connect(profilesTab, &PWT::CUI::ProfilesTab::sayTTS, this, &MainWindow::onSayTTS);
    QObject::connect(profilesTab, &PWT::CUI::ProfilesTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    QObject::connect(service.get(), &PWTCS::ClientService::logMessageSent, this, &MainWindow::onLogMessageSent);
    QObject::connect(service.get(), &PWTCS::ClientService::serviceConnected, this, &MainWindow::onServiceConnected);
    QObject::connect(service.get(), &PWTCS::ClientService::serviceError, this, &MainWindow::onServiceError);
    QObject::connect(service.get(), &PWTCS::ClientService::serviceDisconnected, this, &MainWindow::onServiceDisconnected);
    QObject::connect(consoleSettings.get(), &PWT::ConsoleSettings::logMessageSent, this, &MainWindow::onLogMessageSent);
    QObject::connect(inputRanges.get(), &PWT::UI::InputRanges::logMessageSent, this, &MainWindow::onLogMessageSent);

    loadConsoleSettings();
}

void MainWindow::loadConsoleSettings() {
    consoleSettings->load();
    daemonsTab->loadDaemons(consoleSettings->getDaemonList());
    settingsTab->loadConsoleSettings();
    initTrayIcon();
    initFloatIcon();
    emit allGamepadsModeChange(consoleSettings->getAllGamepadsUIControl());

    if (consoleSettings->getEnableTTS())
        initTTS();

    if (consoleSettings->getRememberWindowState()) {
        const PWT::UI::PWTWindowState wstate = consoleSettings->getWindowState();
        const QPoint pos = mapToGlobal(wstate.position);

        if (wstate.windowState != Qt::WindowNoState)
            setWindowState(wstate.windowState);

        if (pos != QPoint(-1, -1))
            move(pos);

        if (wstate.width >= minimumWidth() && wstate.height >= minimumHeight())
            resize(wstate.width, wstate.height);
    }

    if (!consoleSettings->getAutoConnectDaemonName().isEmpty()) {
        const QString name = consoleSettings->getAutoConnectDaemonName();

        for (const PWT::Daemon &d: consoleSettings->getDaemonList()) {
            if (d.name != name)
                continue;

            daemonsTab->setActiveDaemon(d.adr, d.port);
            onDaemonConnectionRequested(d.adr, d.port);
            break;
        }
    }
}

void MainWindow::loadControllerDB() {
    if (appDataPath.isEmpty()) [[unlikely]]
        return;

    const QString controllerDBPath {QString("%1/sdl_gamepad_db.txt").arg(appDataPath)};
    QFile controllerDB {controllerDBPath};
    int count;

    if (!controllerDB.exists()) {
        QFile builtinDB {":/sdl/db"};

        if (!controllerDB.open(QFile::WriteOnly | QFile::Text) || !builtinDB.open(QFile::ReadOnly | QFile::Text)) {
            onLogMessageSent("Failed to load gamepad mappings db");
            return;
        }

        if (controllerDB.write(builtinDB.readAll()) == -1) {
            onLogMessageSent("Failed to copy builtin gamepad mappings db");
            return;
        }

        controllerDB.flush();
        controllerDB.close();
        builtinDB.close();
    }

    count = SDL_AddGamepadMappingsFromFile(controllerDBPath.toStdString().c_str());

    if (count == -1)
        onLogMessageSent("Failed to load gamepad mappings");
    else
        onLogMessageSent(QString("%1 SDL gamepad mappings loaded").arg(count));
}

void MainWindow::printErrors(const QSet<PWTS::DError> &errors) {
    for (const PWTS::DError &e: errors)
        onLogMessageSent(PWTS::getErrorStr(e));
}

void MainWindow::initTrayIcon() {
    if (!trayIcon.isNull() || consoleSettings->getAppQuitBehaviour() != PWT::AppQuitBehaviour::CloseToTray)
        return;

    trayIcon = new PWT::CUI::TrayIcon();

    QObject::connect(trayIcon, &PWT::CUI::TrayIcon::showMainWindow, this, &MainWindow::onShowMainWindow);
    QObject::connect(trayIcon, &PWT::CUI::TrayIcon::quitApplication, this, &MainWindow::onQuitApp);
}

void MainWindow::initFloatIcon() {
    if (!floatIcon.isNull() || consoleSettings->getAppQuitBehaviour() != PWT::AppQuitBehaviour::FloatingIcon)
        return;

    floatIcon = new PWT::CUI::FloatingIcon(consoleSettings->getFloatIconPosition(), consoleSettings->getFloatIconSize());

    QObject::connect(floatIcon, &PWT::CUI::FloatingIcon::showMainWindow, this, &MainWindow::onShowMainWindow);
    QObject::connect(floatIcon, &PWT::CUI::FloatingIcon::quitApplication, this, &MainWindow::onQuitApp);
    QObject::connect(floatIcon, &PWT::CUI::FloatingIcon::positionChanged, this, &MainWindow::onFloatIconPositionChanged);
}

void MainWindow::initTTS() {
    if (!ttsThread.isNull())
        return;

    ttsThread = new QThread();
    ttsWorker = new PWT::CUI::TTSWorker();

    ttsWorker->moveToThread(ttsThread);

    QObject::connect(ttsThread, &QThread::finished, ttsWorker, &QObject::deleteLater);
    QObject::connect(ttsThread, &QThread::started, ttsWorker, &PWT::CUI::TTSWorker::startTTS);
    QObject::connect(ttsWorker, &PWT::CUI::TTSWorker::logMessageSent, this, &MainWindow::onLogMessageSent);
    QObject::connect(this, &MainWindow::pauseStoppableTTS, ttsWorker, &PWT::CUI::TTSWorker::pauseStoppableTTS);
    QObject::connect(this, &MainWindow::sayTTS, ttsWorker, &PWT::CUI::TTSWorker::say);
    QObject::connect(this, &MainWindow::sayStoppableTTS, ttsWorker, &PWT::CUI::TTSWorker::sayStoppable);

    ttsThread->start();
    emit sayTTS("powertuner console", PWT::CUI::TTSID::TTS_INIT);
}

void MainWindow::disableTTS() {
    if (ttsThread.isNull())
        return;

    ttsThread->quit();
    ttsThread->wait();
    delete ttsThread;

    ttsThread = nullptr;
}

void MainWindow::quitApp() const {
    SDL_Event sdlEvt {.type = SDL_EVENT_QUIT};

    SDL_PushEvent(&sdlEvt);
    QApplication::quit();
}

void MainWindow::setActiveGamepad(const QString &gamepadID) {
    activeGamepad = gamepadID;
    activeGamepadMap = consoleSettings->getGamepadMap(gamepadID);
    emit activeGamepadSet(gamepadID);
}

void MainWindow::initMain(const PWTS::DeviceInfoPacket &packet) {
    cpuVendor = packet.cpuInfo.vendor;
    osType = packet.sysInfo.osType;
    cpuCoreCount = packet.cpuInfo.numCores;
    cpuThreadCount = packet.cpuInfo.numLogicalCpus;

    onLogMessageSent(QString("App data location: %1").arg(packet.daemonDataPath));
    inputRanges->load(packet.sysInfo.product, packet.cpuInfo.brand);
    homeTab->init(packet);
    settingsTab->init(packet);

    switch (osType) {
        case PWTS::OSType::Linux:
            initLinuxTabs(packet);
            break;
        case PWTS::OSType::Windows:
            initWindowsTabs(packet);
            break;
        default:
            break;
    }

    switch (cpuVendor) {
#ifdef WITH_INTEL
        case PWTS::CPUVendor::Intel:
            initIntelTabs(packet);
            break;
#endif
#ifdef WITH_AMD
        case PWTS::CPUVendor::AMD:
            initAMDTabs(packet);
            break;
#endif
        default:
            break;
    }

    initFanControlTab(packet);
}

void MainWindow::initLinuxIntelGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, const int index, const QString &marketName) {
    if (features.contains(PWTS::Feature::INTEL_GPU_SYSFS_GROUP)) {
        const QString tabName = QString("%1 [%2]").arg(marketName.isEmpty() ? "Intel GPU" : marketName).arg(index);
        const QPointer<PWT::CUI::LNX::Intel::IntelGPUTab> intelGpuTab = new PWT::CUI::LNX::Intel::IntelGPUTab(packet, index);

        tabWidget->insertTab(nextTabInsertIdx(), intelGpuTab, tabName);
        addonTabList.append(intelGpuTab);

        QObject::connect(intelGpuTab, &PWT::CUI::LNX::Intel::IntelGPUTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(intelGpuTab, &PWT::CUI::LNX::Intel::IntelGPUTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }
}

void MainWindow::initLinuxAMDGPUTab(const PWTS::DeviceInfoPacket &packet, const QSet<PWTS::Feature> &features, const int index, const QString &marketName) {
    if (features.contains(PWTS::Feature::AMD_GPU_SYSFS_GROUP)) {
        const QString tabName = QString("%1 [%2]").arg(marketName.isEmpty() ? "AMD GPU" : marketName).arg(index);
        const QPointer<PWT::CUI::LNX::AMD::AMDGPUTab> amdGpuTab = new PWT::CUI::LNX::AMD::AMDGPUTab(packet, index);

        tabWidget->insertTab(nextTabInsertIdx(), amdGpuTab, tabName);
        addonTabList.append(amdGpuTab);

        QObject::connect(amdGpuTab, &PWT::CUI::LNX::AMD::AMDGPUTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(amdGpuTab, &PWT::CUI::LNX::AMD::AMDGPUTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }
}

void MainWindow::initLinuxTabs(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.cpu.contains(PWTS::Feature::SYSFS_GROUP)) {
        const QPointer<PWT::CUI::LNX::SysFsTab> sysFsTab = new PWT::CUI::LNX::SysFsTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), sysFsTab, "SysFs");
        addonTabList.append(sysFsTab);

        QObject::connect(sysFsTab, &PWT::CUI::LNX::SysFsTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(sysFsTab, &PWT::CUI::LNX::SysFsTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }

    for (const auto [gpuIndex, gpuFeatures]: packet.features.gpus.asKeyValueRange()) {
        const PWTS::GpuInfo &info = packet.gpusInfo[gpuIndex];
        const QString marketName = PWT::getGPUMarketName(info.vendor, info.deviceID.sliced(2), info.revisionID.sliced(2));

        switch (gpuFeatures.first) {
            case PWTS::GPUVendor::Intel:
                initLinuxIntelGPUTab(packet, gpuFeatures.second, gpuIndex, marketName);
                break;
            case PWTS::GPUVendor::AMD:
                initLinuxAMDGPUTab(packet, gpuFeatures.second, gpuIndex, marketName);
                break;
            default:
                break;
        }
    }
}

void MainWindow::initWindowsTabs(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.cpu.contains(PWTS::Feature::PWR_SCHEME_GROUP)) {
        const QPointer<PWT::CUI::WIN::PowerSchemeTab> powerSchemeTab = new PWT::CUI::WIN::PowerSchemeTab();

        tabWidget->insertTab(nextTabInsertIdx(), powerSchemeTab, "Power Scheme");
        addonTabList.append(powerSchemeTab);

        QObject::connect(powerSchemeTab, &PWT::CUI::WIN::PowerSchemeTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(powerSchemeTab, &PWT::CUI::WIN::PowerSchemeTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }
}

void MainWindow::initFanControlTab(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.fans.isEmpty())
        return;

    const QPointer<PWT::CUI::FANS::FanControlTab> fanControlTab = new PWT::CUI::FANS::FanControlTab(packet);

    tabWidget->insertTab(nextTabInsertIdx(), fanControlTab, "Fan Control");
    addonTabList.append(fanControlTab);

    QObject::connect(fanControlTab, &PWT::CUI::FANS::FanControlTab::sayTTS, this, &MainWindow::onSayTTS);
    QObject::connect(fanControlTab, &PWT::CUI::FANS::FanControlTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
}

#ifdef WITH_INTEL
void MainWindow::initIntelTabs(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.cpu.contains(PWTS::Feature::INTEL_CPU_GROUP)) {
        const QPointer<PWT::CUI::INTEL::CPUTab> CPUTab = new PWT::CUI::INTEL::CPUTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), CPUTab, "CPU");
        addonTabList.append(CPUTab);

        QObject::connect(CPUTab, &PWT::CUI::INTEL::CPUTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(CPUTab, &PWT::CUI::INTEL::CPUTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }

    if (packet.features.cpu.contains(PWTS::Feature::INTEL_HWP_GROUP)) {
        const QPointer<PWT::CUI::INTEL::HWPTab> HWPTab = new PWT::CUI::INTEL::HWPTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), HWPTab, "HWP");
        addonTabList.append(HWPTab);

        QObject::connect(HWPTab, &PWT::CUI::INTEL::HWPTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(HWPTab, &PWT::CUI::INTEL::HWPTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }

    if (packet.features.cpu.contains(PWTS::Feature::INTEL_MCHBAR_GROUP)) {
        const QPointer<PWT::CUI::INTEL::MCHBARTab> mchbarTab = new PWT::CUI::INTEL::MCHBARTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), mchbarTab, "MCHBAR");
        addonTabList.append(mchbarTab);

        QObject::connect(mchbarTab, &PWT::CUI::INTEL::MCHBARTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(mchbarTab, &PWT::CUI::INTEL::MCHBARTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }
}
#endif
#ifdef WITH_AMD
void MainWindow::initAMDTabs(const PWTS::DeviceInfoPacket &packet) {
    if (packet.features.cpu.contains(PWTS::Feature::AMD_CPU_RY_GROUP)) {
        const QPointer<PWT::CUI::AMD::RYCPUTab> ryTab = new PWT::CUI::AMD::RYCPUTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), ryTab, "RyzenAdj");
        addonTabList.append(ryTab);

        QObject::connect(ryTab, &PWT::CUI::AMD::RYCPUTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(ryTab, &PWT::CUI::AMD::RYCPUTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }

    if (packet.features.cpu.contains(PWTS::Feature::AMD_CPU_GROUP)) {
        const QPointer<PWT::CUI::AMD::CPUTab> CPUTab = new PWT::CUI::AMD::CPUTab(packet);

        tabWidget->insertTab(nextTabInsertIdx(), CPUTab, "CPU");
        addonTabList.append(CPUTab);

        QObject::connect(CPUTab, &PWT::CUI::AMD::CPUTab::sayTTS, this, &MainWindow::onSayTTS);
        QObject::connect(CPUTab, &PWT::CUI::AMD::CPUTab::sayStoppableTTS, this, &MainWindow::onSayStoppableTTS);
    }
}
#endif

void MainWindow::deleteAddonTabs() {
    for (const QPointer<PWT::CUI::AddonTab> &tab: addonTabList) {
        tabWidget->removeTab(tabWidget->indexOf(tab));
        tab->deleteLater();
    }

    addonTabList.clear();
    insertTabIdx = tabWidget->indexOf(homeTab) + 1;
}

void MainWindow::upDownAction(const bool isDown) const {
    if (currentTab->hasLostFocus())
        currentTab->focusFirstValidWidget();
    else
        currentTab->focusWidget(isDown);
}

void MainWindow::prevNextAction(const bool isNext) const {
    int idx = tabWidget->currentIndex();

    if ((isNext && (idx+1) >= tabWidget->count()) || (!isNext && (idx-1) < 0))
        return;

    idx += isNext ? 1 : -1;
    tabWidget->setCurrentIndex(idx);
}

void MainWindow::connectServiceSignals() const {
    QObject::connect(service.get(), &PWTCS::ClientService::commandFailed, this, &MainWindow::onServiceCommandFailed);
    QObject::connect(service.get(), &PWTCS::ClientService::deviceInfoPacketReceived, this, &MainWindow::onServiceDeviceInfoPacketReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::daemonPacketReceived, this, &MainWindow::onServiceDaemonPacketReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::settingsApplied, this, &MainWindow::onServiceSettingsApplied);
    QObject::connect(service.get(), &PWTCS::ClientService::profileListReceived, this, &MainWindow::onProfileListReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::profileWritten, this, &MainWindow::onServiceProfileWritten);
    QObject::connect(service.get(), &PWTCS::ClientService::profileDeleted, this, &MainWindow::onServiceProfileDeleted);
    QObject::connect(service.get(), &PWTCS::ClientService::profileApplied, this, &MainWindow::onServiceProfileApplied);
    QObject::connect(service.get(), &PWTCS::ClientService::daemonSettingsReceived, this, &MainWindow::onServiceDaemonSettingsReceived);
    QObject::connect(service.get(), &PWTCS::ClientService::daemonSettingsApplied, this, &MainWindow::onServiceDaemonSettingsApplied);
    QObject::connect(service.get(), &PWTCS::ClientService::batteryStatusChanged, this, &MainWindow::onServiceBatteryStatusChanged);
    QObject::connect(service.get(), &PWTCS::ClientService::wakeFromSleepEvent, this, &MainWindow::onServiceWakeFromSleepEvent);
    QObject::connect(service.get(), &PWTCS::ClientService::applyTimerTick, this, &MainWindow::onServiceApplyTimerTick);
}

void MainWindow::disconnectServiceSignals() const {
    QObject::disconnect(service.get(), &PWTCS::ClientService::commandFailed, this, &MainWindow::onServiceCommandFailed);
    QObject::disconnect(service.get(), &PWTCS::ClientService::deviceInfoPacketReceived, this, &MainWindow::onServiceDeviceInfoPacketReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::daemonPacketReceived, this, &MainWindow::onServiceDaemonPacketReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::settingsApplied, this, &MainWindow::onServiceSettingsApplied);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileListReceived, this, &MainWindow::onProfileListReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileWritten, this, &MainWindow::onServiceProfileWritten);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileDeleted, this, &MainWindow::onServiceProfileDeleted);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileApplied, this, &MainWindow::onServiceProfileApplied);
    QObject::disconnect(service.get(), &PWTCS::ClientService::profileApplied, this, &MainWindow::onServiceProfileApplied);
    QObject::disconnect(service.get(), &PWTCS::ClientService::daemonSettingsReceived, this, &MainWindow::onServiceDaemonSettingsReceived);
    QObject::disconnect(service.get(), &PWTCS::ClientService::daemonSettingsApplied, this, &MainWindow::onServiceDaemonSettingsApplied);
    QObject::disconnect(service.get(), &PWTCS::ClientService::batteryStatusChanged, this, &MainWindow::onServiceBatteryStatusChanged);
    QObject::disconnect(service.get(), &PWTCS::ClientService::wakeFromSleepEvent, this, &MainWindow::onServiceWakeFromSleepEvent);
    QObject::disconnect(service.get(), &PWTCS::ClientService::applyTimerTick, this, &MainWindow::onServiceApplyTimerTick);
}

void MainWindow::completeServiceDisconnection() {
    disconnectServiceSignals();
    homeTab->clearWidget();
    settingsTab->clearWidget();
    profilesTab->clearWidget();
    deleteAddonTabs();
    statusBar->setDaemonConnected(false);
}

void MainWindow::lockUI(const bool lock) const {
    locked = lock;
    profilesTab->setEnabled(!lock);
    settingsTab->setDaemonOptionsEnabled(!lock);
}

void MainWindow::refreshTabsData(const PWTS::DaemonPacket &packet) const {
    homeTab->refreshTab(packet);
    settingsTab->refreshTab(packet);
    profilesTab->updateProfileList(packet.profilesList);

    for (const QPointer<PWT::CUI::AddonTab> &tab: addonTabList)
        tab->refreshTab(packet);
}

PWTS::ClientPacket MainWindow::createClientPacket() const {
    PWTS::ClientPacket packet;

    packet.os = osType;
    packet.vendor = cpuVendor;

    switch (osType) {
        case PWTS::OSType::Linux: {
            packet.linuxData = QSharedPointer<PWTS::LNX::LinuxData>::create();
            packet.linuxData->threadData.fill({}, cpuThreadCount);
        }
            break;
        case PWTS::OSType::Windows: {
            packet.windowsData = QSharedPointer<PWTS::WIN::WindowsData>::create();
        }
            break;
        default:
            break;
    }

    switch (cpuVendor) {
#ifdef WITH_INTEL
        case PWTS::CPUVendor::Intel: {
            packet.intelData = QSharedPointer<PWTS::Intel::IntelData>::create();
            packet.intelData->coreData.fill({}, cpuCoreCount);
            packet.intelData->threadData.fill({}, cpuThreadCount);
        }
            break;
#endif
#ifdef WITH_AMD
        case PWTS::CPUVendor::AMD: {
            packet.amdData = QSharedPointer<PWTS::AMD::AMDData>::create();
            packet.amdData->coreData.fill({}, cpuCoreCount);
            packet.amdData->threadData.fill({}, cpuThreadCount);

            switch (osType) {
                case PWTS::OSType::Linux: {
                    packet.linuxAmdData = QSharedPointer<PWTS::LNX::AMD::LinuxAMDData>::create();
                    packet.linuxAmdData->threadData.fill({}, cpuThreadCount);
                }
                    break;
                default:
                    break;
            }
        }
#endif
        default:
            break;
    }

    for (const QPointer<PWT::CUI::AddonTab> &tab: addonTabList)
        tab->setDataForPacket(packet);

    return packet;
}

void MainWindow::reloadFromDaemon() const {
    lockUI(true);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
    service->sendGetDaemonPacketRequest();
}

void MainWindow::applyCurrentSettings() const {
    lockUI(true);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
    service->sendApplySettingsRequest(createClientPacket());
}

void MainWindow::closeEvent(QCloseEvent *e) {
    switch (consoleSettings->getAppQuitBehaviour()) {
        case PWT::AppQuitBehaviour::Quit:
            quitApp();
            break;
        case PWT::AppQuitBehaviour::CloseToTray: {
            appMaximized = windowState() == Qt::WindowMaximized;

            e->ignore();
            hide();
            trayIcon->show();

            if (!ttsThread.isNull())
                emit sayTTS(tr("minimized to tray bar"), PWT::CUI::TTSID::MINIMIZED_CONSOLE);
        }
            break;
        case PWT::AppQuitBehaviour::FloatingIcon: {
            appMaximized = windowState() == Qt::WindowMaximized;

            e->ignore();
            hide();
            floatIcon->show();

            if (!ttsThread.isNull())
                emit sayTTS(tr("minimized to floating icon"), PWT::CUI::TTSID::MINIMIZED_CONSOLE);
        }
            break;
        default:
            break;
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease: {
            if (currentTab.isNull())
                break;

            const QKeyEvent *keyEvt = dynamic_cast<QKeyEvent *>(event);
            const Qt::Key ekey = static_cast<Qt::Key>(keyEvt->key());

            if (event->type() == QEvent::KeyPress) {
                if (ekey == consoleSettings->getUpKB() || ekey == consoleSettings->getDownKB() || ekey == Qt::Key_Tab) {
                    upDownAction(ekey == consoleSettings->getDownKB() || ekey == Qt::Key_Tab);
                    return true;

                } else if (ekey == consoleSettings->getPrevTabKB() || ekey == consoleSettings->getNextTabKB()) {
                    prevNextAction(ekey == consoleSettings->getNextTabKB());
                    return true;

                } else if (ekey == consoleSettings->getScrollViewUpKB()) {
                    currentTab->scrollViewUp();
                    return true;

                } else if (ekey == consoleSettings->getScrollViewDownKB()) {
                    currentTab->scrollViewDown();
                    return true;

                } else if (ekey == consoleSettings->getScrollViewTopKB()) {
                    currentTab->scrollViewTop();
                    return true;

                } else if (ekey == consoleSettings->getScrollViewBottomKB()) {
                    currentTab->scrollViewBottom();
                    return true;
                }
            } else if (!keyEvt->isAutoRepeat()) {
                if (ekey == consoleSettings->getReloadKB()) {
                    if (locked) {
                        emit sayTTS(tr("cannot refresh values, daemon disconnected or busy"), PWT::CUI::TTSID::DAEMON_CMD_FAIL);
                        break;
                    }

                    reloadFromDaemon();
                    return true;

                } else if (ekey == consoleSettings->getAppyKB()) {
                    if (locked) {
                        emit sayTTS(tr("cannot apply settings, daemon disconnected or busy"), PWT::CUI::TTSID::DAEMON_CMD_FAIL);
                        break;
                    }

                    applyCurrentSettings();
                    return true;
                }
            }
        }
            break;
        case QEvent::WindowStateChange:
        case QEvent::Move:
        case QEvent::Resize: {
            if (!consoleSettings->getRememberWindowState())
                break;

            windowEventTimer->start();
        }
            break;
        default:
            break;
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::onAppStateChanged(const Qt::ApplicationState state) {
    emit focusChanged(state == Qt::ApplicationActive);

    appFocused = state == Qt::ApplicationActive;
}

void MainWindow::onLogMessageSent(const QString &msg) {
    const int logTabIdx = tabWidget->indexOf(logTab);

    if (tabWidget->currentIndex() != logTabIdx) [[likely]]
        tabWidget->setTabText(logTabIdx, QString("Log (%1)").arg(++unreadLogs));

    logTab->writeLog(msg);

    if (!ttsThread.isNull())
        emit sayTTS(tr("logs page updated"), PWT::CUI::TTSID::NEW_UI_LOGS);
}

void MainWindow::onReloadGamepadMappings() {
    loadControllerDB();
}

void MainWindow::onConsoleSettingsSaved() {
    if (consoleSettings->getAppQuitBehaviour() == PWT::AppQuitBehaviour::CloseToTray)
        initTrayIcon();
    else if (!trayIcon.isNull())
        delete trayIcon;

    if (consoleSettings->getAppQuitBehaviour() == PWT::AppQuitBehaviour::FloatingIcon) {
        initFloatIcon();
        floatIcon->setSize(consoleSettings->getFloatIconSize());

    } else if (!floatIcon.isNull()) {
        delete floatIcon;
    }

    if (consoleSettings->getEnableTTS())
        initTTS();
    else
        disableTTS();

    emit allGamepadsModeChange(consoleSettings->getAllGamepadsUIControl());

    if (consoleSettings->getAllGamepadsUIControl()) {
        activeGamepadMap = consoleSettings->getGamepadMap(prevGamepadID); // refresh mappings
        emit deadzoneDataUpdated(consoleSettings->getGamepadsDeadzoneData());

    } else {
        const QString prefGamepad = consoleSettings->getPreferredGamepadID();

        if (!prefGamepad.isEmpty()) {
            activeGamepadMap = consoleSettings->getGamepadMap(prefGamepad);
            emit deadzoneUpdated(prefGamepad, activeGamepadMap.deadzone);
        }

        setActiveGamepad(prefGamepad);
    }

    if (!ttsThread.isNull())
        emit sayTTS(tr("client settings saved"), PWT::CUI::TTSID::CLIENT_SETT_SAVED);
}

void MainWindow::onDaemonAdded(const PWT::Daemon &daemon) const {
    consoleSettings->addDaemon(daemon);
    consoleSettings->save();
    daemonsTab->addDaemon(daemon);
    settingsTab->initAutoConnectDaemonList();
}

void MainWindow::onTabChanged(const int idx) {
    currentTab = qobject_cast<PWT::CUI::ConsoleTab *>(tabWidget->currentWidget());

    if (unreadLogs > 0) {
        const int logTabIdx = tabWidget->indexOf(logTab);

        if (idx == logTabIdx) {
            tabWidget->setTabText(logTabIdx, "Log");
            unreadLogs = 0;
        }
    }

    if (!ttsThread.isNull())
        emit sayTTS(tr("page %1").arg(tabWidget->tabText(idx)), PWT::CUI::TTSID::TAB_PAGE);
}

void MainWindow::onDaemonConnectionRequested(const QString &adr, const quint16 port) {
    lockUI(true);

    if (service->isConnected()) {
        service->disconnectFromDaemon();
        completeServiceDisconnection();
    }

    service->connectToDaemon(adr, port);
}

void MainWindow::onDaemonDisconnectionRequested() {
    lockUI(true);
    service->disconnectFromDaemon();
    daemonsTab->setDaemonAsDisconnected();
    completeServiceDisconnection();

    if (!ttsThread.isNull())
        emit sayTTS(tr("daemon disconnected"), PWT::CUI::TTSID::DAEMON_DISCONNECTED);
}

void MainWindow::onDaemonDeleted(const PWT::Daemon &data) {
    consoleSettings->removeDaemon(data);
    consoleSettings->save();
    settingsTab->initAutoConnectDaemonList();

    if (!ttsThread.isNull())
        emit sayTTS(tr("%1 daemon has been deleted").arg(data.name), PWT::CUI::TTSID::DAEMON_DELETED);
}

void MainWindow::onApplyDaemonSettings(const QByteArray &data) const {
    service->sendApplyDaemonSettingsRequest(data);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
    lockUI(true);
}

void MainWindow::onReloadDaemonSettings() const {
    service->sendGetDaemonSettingsRequest();
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
    lockUI(true);
}

void MainWindow::onServiceError() {
    lockUI(true);
    daemonsTab->setDaemonAsDisconnected();
    completeServiceDisconnection();

    if (!ttsThread.isNull())
        emit sayTTS(tr("service error, daemon disconnected"), PWT::CUI::TTSID::SERVICE_ERROR);
}

void MainWindow::onServiceCommandFailed() {
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Ready);
    lockUI(false);

    if (!ttsThread.isNull())
        emit sayTTS(tr("daemon command failed"), PWT::CUI::TTSID::DAEMON_CMD_FAIL);
}

void MainWindow::onServiceDisconnected() {
    lockUI(true);
    daemonsTab->setDaemonAsDisconnected();
    completeServiceDisconnection();

    if (!ttsThread.isNull())
        emit sayTTS(tr("daemon disconnected"), PWT::CUI::TTSID::DAEMON_DISCONNECTED);
}

void MainWindow::onServiceConnected() {
    daemonsTab->setDaemonAsConnected();
    connectServiceSignals();
    service->sendGetDeviceInfoPacketRequest();
    statusBar->setDaemonConnected(true);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
    onLogMessageSent(QString("[%1:%2] Daemon connection successful").arg(service->getDaemonAddress()).arg(service->getDaemonPort()));
}

void MainWindow::onServiceDeviceInfoPacketReceived(const PWTS::DeviceInfoPacket &packet) {
    if (!ttsThread.isNull())
        emit pauseStoppableTTS(true);

    initMain(packet);
    service->sendGetDaemonPacketRequest();
}

void MainWindow::onServiceDaemonPacketReceived(const PWTS::DaemonPacket &packet) {
    inputRanges->refresh();
    refreshTabsData(packet);
    printErrors(packet.errors);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Ready);
    lockUI(false);

    if (!ttsThread.isNull()) {
        emit sayTTS(tr("application data has been refreshed"), PWT::CUI::TTSID::DPACKET_RECEIVED);
        emit pauseStoppableTTS(false);
    }
}

void MainWindow::onServiceSettingsApplied(const QSet<PWTS::DError> &errors) {
    printErrors(errors);
    homeTab->unsetAppliedProfile();
    reloadFromDaemon();
}

void MainWindow::onServiceDaemonSettingsApplied(const bool diskSaveResult) {
    onLogMessageSent(diskSaveResult ? QStringLiteral("Daemon settings saved successfully") : QStringLiteral("Failed to write new daemon settings to disk"));
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceDaemonSettingsReceived(const QByteArray &data) const {
    settingsTab->updateDaemonSettings(data);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceBatteryStatusChanged(const QSet<PWTS::DError> &errors, const QString &profile) {
    if (errors.isEmpty()) {
        onLogMessageSent(QString("Battery status changed: applied \"%1\" profile").arg(profile));
        homeTab->setAppliedProfile(profile);

    } else {
        onLogMessageSent(QString("Battery status changed: failed to apply all \"%1\" profile settings").arg(profile));
        onLogMessageSent(QString("== '%1' error log start ==").arg(profile));
        printErrors(errors);
        onLogMessageSent(QString("== '%1' error log end ==").arg(profile));
        homeTab->unsetAppliedProfile();
    }

    reloadFromDaemon();
}

void MainWindow::onServiceWakeFromSleepEvent(const QSet<PWTS::DError> &errors) {
    onLogMessageSent(QStringLiteral("Wake from sleep event"));

    if (errors.isEmpty()) {
        onLogMessageSent(QStringLiteral("Settings re-applied successfully"));

    } else {
        onLogMessageSent(QStringLiteral("== Wake from sleep error log start =="));
        printErrors(errors);
        onLogMessageSent(QStringLiteral("== Wake from sleep error log end =="));
    }

    reloadFromDaemon();
}

void MainWindow::onServiceApplyTimerTick(const QSet<PWTS::DError> &errors) {
    printErrors(errors);
}

void MainWindow::onServiceProfileWritten(const bool result) {
    onLogMessageSent(result ? QStringLiteral("Profile saved successfully") : QStringLiteral("Failed to write profile to disk"));
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceProfileDeleted(const bool result) {
    onLogMessageSent(result ? QStringLiteral("Profile deleted successfully") : QStringLiteral("Failed to delete profile"));
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Ready);
    lockUI(false);
}

void MainWindow::onServiceProfileApplied(const QSet<PWTS::DError> &errors, const QString &name) {
    if (errors.isEmpty()) {
        homeTab->setAppliedProfile(name);
        onLogMessageSent(QString("\"%1\" profile applied successfully").arg(name));

    } else {
        onLogMessageSent(QString("Failed to apply all \"%1\" profile settings").arg(name));
        onLogMessageSent(QString("== '%1' error log start ==").arg(name));
        printErrors(errors);
        onLogMessageSent(QString("== '%1' error log end ==").arg(name));
    }

    reloadFromDaemon();
}

void MainWindow::onProfileListReceived(const QList<QString> &profileList) {
    lockUI(true);
    settingsTab->updateProfileList(profileList);
    profilesTab->updateProfileList(profileList);
    onLogMessageSent(QStringLiteral("Updated profile list received from daemon"));
    lockUI(false);
}

void MainWindow::onProfileCreated(const QString &name) const {
    lockUI(true);
    service->sendWriteProfileRequest(name, createClientPacket());
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileDeleted(const QString &name) const {
    lockUI(true);
    service->sendDeleteProfileRequest(name);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileSaved(const QString &name) const {
    lockUI(true);
    service->sendWriteProfileRequest(name, createClientPacket());
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileLoaded(const QString &name) const {
    lockUI(true);
    service->sendLoadProfileRequest(name);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onProfileApplied(const QString &name) const {
    lockUI(true);
    service->sendApplyProfileRequest(name);
    statusBar->setCMDStatus(PWT::CUI::StatusBar::CMDStatus::Pending);
}

void MainWindow::onShowMainWindow() {
    if (appMaximized)
        showMaximized();
    else
        showNormal();

    raise();
    activateWindow();

    if (!ttsThread.isNull())
        emit sayTTS(tr("powertuner is shown"), PWT::CUI::TTSID::SHOW_CONSOLE);
}

void MainWindow::onWindowEventTimerTimeout() const {
    consoleSettings->setWindowState({
        .windowState = windowState(),
        .position = pos(),
        .width = width(),
        .height = height()
    });
    consoleSettings->save(true);
}

void MainWindow::onQuitApp() const {
    quitApp();
}

void MainWindow::onFloatIconPositionChanged(const QPointF &pos) const {
    consoleSettings->setFloatIconPosition(pos);
    consoleSettings->save();
}

void MainWindow::onGamepadConnected(const QString &gamepadLbl, const QString &gamepadID) {
    if (!ttsThread.isNull())
        emit pauseStoppableTTS(true);

    if (activeGamepad.isEmpty() || gamepadID == consoleSettings->getPreferredGamepadID())
        setActiveGamepad(gamepadID);

    settingsTab->addGamepad(gamepadLbl, gamepadID);

    if (consoleSettings->getAllGamepadsUIControl())
        emit deadzoneUpdated(gamepadID, consoleSettings->getGamepadMap(gamepadID).deadzone);

    if (!ttsThread.isNull()) {
        emit sayTTS(tr("gamepad connected"), PWT::CUI::TTSID::PAD_CONNECTED);
        emit pauseStoppableTTS(false);
    }
}

void MainWindow::onGamepadDisconnected(const QString &gamepadID) {
    settingsTab->removeGamepad(gamepadID);

    if (gamepadID == activeGamepad)
        setActiveGamepad(settingsTab->getFirstGamepadID());

    if (!ttsThread.isNull())
        emit sayTTS(tr("gamepad disconnected"), PWT::CUI::TTSID::PAD_DISCONNECTED);
}

void MainWindow::onGamepadButton(const QString &gamepadID, const PWT::GamepadButton button) {
    if ((!consoleSettings->getAllGamepadsUIControl() && gamepadID != activeGamepad) || currentTab.isNull())
        return;

    if (consoleSettings->getAllGamepadsUIControl() && prevGamepadID != gamepadID) {
        prevGamepadID = gamepadID;
        activeGamepadMap = consoleSettings->getGamepadMap(gamepadID);
    }

    if (currentTab->hasModal()) {
        currentTab->sendGamepadButtonEventForModal(button, activeGamepadMap);

    } else if (button == activeGamepadMap.appComboGP) {
        if (!appFocused) {
            if (consoleSettings->getAppQuitBehaviour() == PWT::AppQuitBehaviour::CloseToTray && trayIcon->isVisible())
                trayIcon->hide();
            else if (consoleSettings->getAppQuitBehaviour() == PWT::AppQuitBehaviour::FloatingIcon && floatIcon->isVisible())
                floatIcon->hide();

            onShowMainWindow();

        } else {
            close();
        }
    } else if (currentTab->hasMicroKeyboard()) {
        currentTab->sendGamepadButtonEventForMicroKeyboard(button, activeGamepadMap);

    } else if (button == activeGamepadMap.upGP || button == activeGamepadMap.downGP) {
        upDownAction(button == activeGamepadMap.downGP);

    } else if (button == activeGamepadMap.prevTabGP || button == activeGamepadMap.nextTabGP) {
        prevNextAction(button == activeGamepadMap.nextTabGP);

    } else if (button == activeGamepadMap.scrollViewUpGP) {
        currentTab->scrollViewUp();

    } else if (button == activeGamepadMap.scrollViewDownGP) {
        currentTab->scrollViewDown();

    } else if (button == activeGamepadMap.scrollViewTopGP) {
        currentTab->scrollViewTop();

    } else if (button == activeGamepadMap.scrollViewBottomGP) {
        currentTab->scrollViewBottom();

    } else if (button == activeGamepadMap.reloadGP) {
        if (locked)
            emit sayTTS(tr("cannot refresh values, daemon disconnected or busy"), PWT::CUI::TTSID::DAEMON_CMD_FAIL);
        else
            reloadFromDaemon();

    } else if (button == activeGamepadMap.applyGP) {
        if (locked)
            emit sayTTS(tr("cannot apply settings, daemon disconnected or busy"), PWT::CUI::TTSID::DAEMON_CMD_FAIL);
        else
            applyCurrentSettings();

    } else {
        currentTab->sendGamepadButtonEvent(button, activeGamepadMap);
    }
}

void MainWindow::onSayTTS(const QString &msg, const PWT::CUI::TTSID id) {
    emit sayTTS(msg, id);
}

void MainWindow::onSayStoppableTTS(const QString &msg) {
    emit sayStoppableTTS(msg);
}
