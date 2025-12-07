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
#include "ClientOptions.h"
#include "../../Widgets/SectionWidget.h"
#include "pwtClientCommon/CommonUtils.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI {
    ClientOptions::ClientOptions() {
        const QList<QString> keyOpts = keyMap.values();
        const QList<QString> buttonOpts = buttonMap.values();
        QVBoxLayout *lyt = new QVBoxLayout();

        consoleSettings = ConsoleSettings::getInstance();
        appQuitBehaviour = new ConsoleSelect("Application quit behaviour");
        floatIconSize = new ConsoleSelect("Floating icon size");
        autoConnectDaemon = new ConsoleSelect("Connect daemon on start");
        enableTTS = new ConsoleCheckbox("Enable Text To Speech (TTS)");
        rememberWindowState = new ConsoleCheckbox("Remember window state and position");
        runAtStartup = new ConsoleCheckbox("Run at startup");
        prevTabKB = new ConsoleSelect("Previous tab");
        nextTabKB = new ConsoleSelect("Next tab");
        leftKB = new ConsoleSelect("Left");
        fastLeftKB = new ConsoleSelect("Fast left");
        rightKB = new ConsoleSelect("Right");
        fastRightKB = new ConsoleSelect("Fast Right");
        upKB = new ConsoleSelect("Up");
        downKB = new ConsoleSelect("Down");
        clickKB = new ConsoleSelect("Click");
        backKB = new ConsoleSelect("Back");
        reloadKB = new ConsoleSelect("Reload values from daemon");
        applyKB = new ConsoleSelect("Apply current settings");
        inputModeKB = new ConsoleSelect("Input mode switch");
        scrollUpKB = new ConsoleSelect("Scroll up");
        scrollDownKB = new ConsoleSelect("Scroll down");
        scrollLeftKB = new ConsoleSelect("Scroll left");
        scrollRightKB = new ConsoleSelect("Scroll right");
        scrollViewUpKB = new ConsoleSelect("Scroll page up");
        scrollViewDownKB = new ConsoleSelect("Scroll page down");
        scrollViewTopKB = new ConsoleSelect("Scroll page to top");
        scrollViewBottomKB = new ConsoleSelect("Scroll page to bottom");
        gamepad = new ConsoleSelect("Gamepad");
        allGamepadsUIControl = new ConsoleCheckbox("All gamepads control UI", "All connected gamepads can control the UI, each with its own mapping");
        prevTabGP = new ConsoleSelect("Previous tab");
        nextTabGP = new ConsoleSelect("Next tab");
        leftGP = new ConsoleSelect("Left");
        fastLeftGP = new ConsoleSelect("Fast left");
        rightGP = new ConsoleSelect("Right");
        fastRightGP = new ConsoleSelect("Fast right");
        upGP = new ConsoleSelect("Up");
        downGP = new ConsoleSelect("Down");
        clickGP = new ConsoleSelect("Click");
        backGP = new ConsoleSelect("Back");
        reloadGP = new ConsoleSelect("Reload values from daemon");
        applyGP = new ConsoleSelect("Apply current settings");
        inputModeGP = new ConsoleSelect("Input mode switch");
        scrollUpGP = new ConsoleSelect("Scroll up");
        scrollDownGP = new ConsoleSelect("Scroll down");
        scrollLeftGP = new ConsoleSelect("Scroll left");
        scrollRightGP = new ConsoleSelect("Scroll right");
        scrollViewUpGP = new ConsoleSelect("Scroll page up");
        scrollViewDownGP = new ConsoleSelect("Scroll page down");
        scrollViewTopGP = new ConsoleSelect("Scroll page to top");
        scrollViewBottomGP = new ConsoleSelect("Scroll page to bottom");
        appCombo = new ConsoleSelect("Application button combo");
        deadzone = new ConsoleSlider("Deadzone");
        reloadMappingsBtn = new ConsoleButton("Reload gamepad mappings");
        saveBnt = new ConsoleButton("Save");
        resetDefaultBtn = new ConsoleButton("Reset to defaults");

        appQuitBehaviour->addOptions({"Quit application", "Close to tray", "Floating icon"});
        floatIconSize->addOptions({"Tiny", "Small", "Medium", "Big", "Large"});
        prevTabKB->addOptions(keyOpts);
        nextTabKB->addOptions(keyOpts);
        leftKB->addOptions(keyOpts);
        fastLeftKB->addOptions(keyOpts);
        rightKB->addOptions(keyOpts);
        fastRightKB->addOptions(keyOpts);
        upKB->addOptions(keyOpts);
        downKB->addOptions(keyOpts);
        clickKB->addOptions(keyOpts);
        backKB->addOptions(keyOpts);
        reloadKB->addOptions(keyOpts);
        applyKB->addOptions(keyOpts);
        inputModeKB->addOptions(keyOpts);
        scrollUpKB->addOptions(keyOpts);
        scrollDownKB->addOptions(keyOpts);
        scrollLeftKB->addOptions(keyOpts);
        scrollRightKB->addOptions(keyOpts);
        scrollViewUpKB->addOptions(keyOpts);
        scrollViewDownKB->addOptions(keyOpts);
        scrollViewTopKB->addOptions(keyOpts);
        scrollViewBottomKB->addOptions(keyOpts);
        prevTabGP->addOptions(buttonOpts);
        nextTabGP->addOptions(buttonOpts);
        leftGP->addOptions(buttonOpts);
        fastLeftGP->addOptions(buttonOpts);
        rightGP->addOptions(buttonOpts);
        fastRightGP->addOptions(buttonOpts);
        upGP->addOptions(buttonOpts);
        downGP->addOptions(buttonOpts);
        clickGP->addOptions(buttonOpts);
        backGP->addOptions(buttonOpts);
        reloadGP->addOptions(buttonOpts);
        applyGP->addOptions(buttonOpts);
        inputModeGP->addOptions(buttonOpts);
        scrollUpGP->addOptions(buttonOpts);
        scrollDownGP->addOptions(buttonOpts);
        scrollLeftGP->addOptions(buttonOpts);
        scrollRightGP->addOptions(buttonOpts);
        scrollViewUpGP->addOptions(buttonOpts);
        scrollViewDownGP->addOptions(buttonOpts);
        scrollViewTopGP->addOptions(buttonOpts);
        scrollViewBottomGP->addOptions(buttonOpts);
        appCombo->addOptions(comboMap.values());
        deadzone->setRange(1, 25);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new SectionWidget("Application settings", SectionWidget::Size::Big));
        lyt->addWidget(appQuitBehaviour);
        lyt->addWidget(floatIconSize);
        lyt->addWidget(autoConnectDaemon);
        lyt->addWidget(enableTTS);
        lyt->addWidget(rememberWindowState);
        lyt->addWidget(runAtStartup);
        lyt->addWidget(new SectionWidget("Keyboard"));
        lyt->addWidget(prevTabKB);
        lyt->addWidget(nextTabKB);
        lyt->addWidget(leftKB);
        lyt->addWidget(fastLeftKB);
        lyt->addWidget(rightKB);
        lyt->addWidget(fastRightKB);
        lyt->addWidget(upKB);
        lyt->addWidget(downKB);
        lyt->addWidget(clickKB);
        lyt->addWidget(backKB);
        lyt->addWidget(reloadKB);
        lyt->addWidget(applyKB);
        lyt->addWidget(inputModeKB);
        lyt->addWidget(scrollUpKB);
        lyt->addWidget(scrollDownKB);
        lyt->addWidget(scrollLeftKB);
        lyt->addWidget(scrollRightKB);
        lyt->addWidget(scrollViewUpKB);
        lyt->addWidget(scrollViewDownKB);
        lyt->addWidget(scrollViewTopKB);
        lyt->addWidget(scrollViewBottomKB);
        lyt->addWidget(new SectionWidget("Gamepad"));
        lyt->addWidget(gamepad);
        lyt->addWidget(allGamepadsUIControl);
        lyt->addWidget(prevTabGP);
        lyt->addWidget(nextTabGP);
        lyt->addWidget(leftGP);
        lyt->addWidget(fastLeftGP);
        lyt->addWidget(rightGP);
        lyt->addWidget(fastRightGP);
        lyt->addWidget(upGP);
        lyt->addWidget(downGP);
        lyt->addWidget(clickGP);
        lyt->addWidget(backGP);
        lyt->addWidget(reloadGP);
        lyt->addWidget(applyGP);
        lyt->addWidget(inputModeGP);
        lyt->addWidget(scrollUpGP);
        lyt->addWidget(scrollDownGP);
        lyt->addWidget(scrollLeftGP);
        lyt->addWidget(scrollRightGP);
        lyt->addWidget(scrollViewUpGP);
        lyt->addWidget(scrollViewDownGP);
        lyt->addWidget(scrollViewTopGP);
        lyt->addWidget(scrollViewBottomGP);
        lyt->addWidget(appCombo);
        lyt->addWidget(deadzone);
        lyt->addWidget(reloadMappingsBtn);
        lyt->addWidget(saveBnt);
        lyt->addWidget(resetDefaultBtn);

        setLayout(lyt);
        enableGamepadSettings(false);

        QObject::connect(appQuitBehaviour, &ConsoleSelect::selectionChanged, this, &ClientOptions::onAppQuitBehaviourChanged);
        QObject::connect(gamepad, &ConsoleSelect::selectionChanged, this, &ClientOptions::onGamepadChanged);
        QObject::connect(reloadMappingsBtn, &ConsoleButton::clicked, this, &ClientOptions::onReloadGamepadMappingsBtnClicked);
        QObject::connect(saveBnt, &ConsoleButton::clicked, this, &ClientOptions::onSaveBtnClicked);
        QObject::connect(resetDefaultBtn, &ConsoleButton::clicked, this, &ClientOptions::onResetDefaultBtnClicked);
    }

    void ClientOptions::loadGamepadMappings(const QString &gamepadID) const {
        const GamepadMap gmap = gamepadMapsCache[gamepadID];

        prevTabGP->setCurrentIndex(prevTabGP->indexOf(buttonMap.value(gmap.prevTabGP)));
        nextTabGP->setCurrentIndex(nextTabGP->indexOf(buttonMap.value(gmap.nextTabGP)));
        leftGP->setCurrentIndex(leftGP->indexOf(buttonMap.value(gmap.leftGP)));
        fastLeftGP->setCurrentIndex(fastLeftGP->indexOf(buttonMap.value(gmap.fastLeftGP)));
        rightGP->setCurrentIndex(rightGP->indexOf(buttonMap.value(gmap.rightGP)));
        fastRightGP->setCurrentIndex(fastRightGP->indexOf(buttonMap.value(gmap.fastRightGP)));
        upGP->setCurrentIndex(upGP->indexOf(buttonMap.value(gmap.upGP)));
        downGP->setCurrentIndex(downGP->indexOf(buttonMap.value(gmap.downGP)));
        clickGP->setCurrentIndex(clickGP->indexOf(buttonMap.value(gmap.clickGP)));
        backGP->setCurrentIndex(backGP->indexOf(buttonMap.value(gmap.backGP)));
        reloadGP->setCurrentIndex(reloadGP->indexOf(buttonMap.value(gmap.reloadGP)));
        applyGP->setCurrentIndex(applyGP->indexOf(buttonMap.value(gmap.applyGP)));
        inputModeGP->setCurrentIndex(inputModeGP->indexOf(buttonMap.value(gmap.inputModeGP)));
        scrollUpGP->setCurrentIndex(scrollUpGP->indexOf(buttonMap.value(gmap.scrollUpGP)));
        scrollDownGP->setCurrentIndex(scrollDownGP->indexOf(buttonMap.value(gmap.scrollDownGP)));
        scrollLeftGP->setCurrentIndex(scrollLeftGP->indexOf(buttonMap.value(gmap.scrollLeftGP)));
        scrollRightGP->setCurrentIndex(scrollRightGP->indexOf(buttonMap.value(gmap.scrollRightGP)));
        scrollViewUpGP->setCurrentIndex(scrollViewUpGP->indexOf(buttonMap.value(gmap.scrollViewUpGP)));
        scrollViewDownGP->setCurrentIndex(scrollViewDownGP->indexOf(buttonMap.value(gmap.scrollViewDownGP)));
        scrollViewTopGP->setCurrentIndex(scrollViewTopGP->indexOf(buttonMap.value(gmap.scrollViewTopGP)));
        scrollViewBottomGP->setCurrentIndex(scrollViewBottomGP->indexOf(buttonMap.value(gmap.scrollViewBottomGP)));
        appCombo->setCurrentIndex(appCombo->indexOf(comboMap.value(gmap.appComboGP)));
        deadzone->setValue(gmap.deadzone);
    }

    void ClientOptions::updateGamepadMappings(const QString &gamepadID) {
        const GamepadMap gmap = {
            .prevTabGP = buttonMap.key(prevTabGP->getCurrentText()),
            .nextTabGP = buttonMap.key(nextTabGP->getCurrentText()),
            .leftGP = buttonMap.key(leftGP->getCurrentText()),
            .fastLeftGP = buttonMap.key(fastLeftGP->getCurrentText()),
            .rightGP = buttonMap.key(rightGP->getCurrentText()),
            .fastRightGP = buttonMap.key(fastRightGP->getCurrentText()),
            .upGP = buttonMap.key(upGP->getCurrentText()),
            .downGP = buttonMap.key(downGP->getCurrentText()),
            .clickGP = buttonMap.key(clickGP->getCurrentText()),
            .backGP = buttonMap.key(backGP->getCurrentText()),
            .reloadGP = buttonMap.key(reloadGP->getCurrentText()),
            .applyGP = buttonMap.key(applyGP->getCurrentText()),
            .inputModeGP = buttonMap.key(inputModeGP->getCurrentText()),
            .scrollUpGP = buttonMap.key(scrollUpGP->getCurrentText()),
            .scrollDownGP = buttonMap.key(scrollDownGP->getCurrentText()),
            .scrollLeftGP = buttonMap.key(scrollLeftGP->getCurrentText()),
            .scrollRightGP = buttonMap.key(scrollRightGP->getCurrentText()),
            .scrollViewUpGP = buttonMap.key(scrollViewUpGP->getCurrentText()),
            .scrollViewDownGP = buttonMap.key(scrollViewDownGP->getCurrentText()),
            .scrollViewTopGP = buttonMap.key(scrollViewTopGP->getCurrentText()),
            .scrollViewBottomGP = buttonMap.key(scrollViewBottomGP->getCurrentText()),
            .appComboGP = comboMap.key(appCombo->getCurrentText()),
            .deadzone = deadzone->getValue()
        };

        gamepadMapsCache.insert(gamepadID, gmap);
    }

    void ClientOptions::enableGamepadSettings(const bool hasGamepads) const {
        allGamepadsUIControl->setEnabled(hasGamepads);
        prevTabGP->setEnabled(hasGamepads);
        nextTabGP->setEnabled(hasGamepads);
        leftGP->setEnabled(hasGamepads);
        fastLeftGP->setEnabled(hasGamepads);
        rightGP->setEnabled(hasGamepads);
        fastRightGP->setEnabled(hasGamepads);
        upGP->setEnabled(hasGamepads);
        downGP->setEnabled(hasGamepads);
        clickGP->setEnabled(hasGamepads);
        backGP->setEnabled(hasGamepads);
        reloadGP->setEnabled(hasGamepads);
        applyGP->setEnabled(hasGamepads);
        inputModeGP->setEnabled(hasGamepads);
        scrollUpGP->setEnabled(hasGamepads);
        scrollDownGP->setEnabled(hasGamepads);
        scrollLeftGP->setEnabled(hasGamepads);
        scrollRightGP->setEnabled(hasGamepads);
        scrollViewUpGP->setEnabled(hasGamepads);
        scrollViewDownGP->setEnabled(hasGamepads);
        scrollViewTopGP->setEnabled(hasGamepads);
        scrollViewBottomGP->setEnabled(hasGamepads);
        appCombo->setEnabled(hasGamepads);
        deadzone->setEnabled(hasGamepads);
        reloadMappingsBtn->setEnabled(hasGamepads);
    }

    void ClientOptions::initAutoConnectDaemonList() const {
        autoConnectDaemon->clear();
        autoConnectDaemon->addOption("");

        for (const Daemon &d: consoleSettings->getDaemonList())
            autoConnectDaemon->addOption(d.name);
    }

    void ClientOptions::loadSettings() const {
        const QSignalBlocker appQBBlock {appQuitBehaviour};

        appQuitBehaviour->setCurrentIndex(static_cast<int>(consoleSettings->getAppQuitBehaviour()));
        floatIconSize->setCurrentIndex(static_cast<int>(consoleSettings->getFloatIconSize()));
        autoConnectDaemon->setCurrentIndex(autoConnectDaemon->indexOf(consoleSettings->getAutoConnectDaemonName()));
        enableTTS->setChecked(consoleSettings->getEnableTTS());
        runAtStartup->setChecked(getStartupUtil("powertunerconsole")->isSet());
        rememberWindowState->setChecked(consoleSettings->getRememberWindowState());
        prevTabKB->setCurrentIndex(prevTabKB->indexOf(keyMap.value(consoleSettings->getPrevTabKB())));
        nextTabKB->setCurrentIndex(nextTabKB->indexOf(keyMap.value(consoleSettings->getNextTabKB())));
        leftKB->setCurrentIndex(leftKB->indexOf(keyMap.value(consoleSettings->getLeftKB())));
        fastLeftKB->setCurrentIndex(fastLeftKB->indexOf(keyMap.value(consoleSettings->getFastLeftKB())));
        rightKB->setCurrentIndex(rightKB->indexOf(keyMap.value(consoleSettings->getRightKB())));
        fastRightKB->setCurrentIndex(fastRightKB->indexOf(keyMap.value(consoleSettings->getFastRightKB())));
        upKB->setCurrentIndex(upKB->indexOf(keyMap.value(consoleSettings->getUpKB())));
        downKB->setCurrentIndex(downKB->indexOf(keyMap.value(consoleSettings->getDownKB())));
        clickKB->setCurrentIndex(clickKB->indexOf(keyMap.value(consoleSettings->getClickKB())));
        backKB->setCurrentIndex(backKB->indexOf(keyMap.value(consoleSettings->getBackKB())));
        reloadKB->setCurrentIndex(reloadKB->indexOf(keyMap.value(consoleSettings->getReloadKB())));
        applyKB->setCurrentIndex(applyKB->indexOf(keyMap.value(consoleSettings->getAppyKB())));
        inputModeKB->setCurrentIndex(inputModeKB->indexOf(keyMap.value(consoleSettings->getInputModeKB())));
        scrollUpKB->setCurrentIndex(scrollUpKB->indexOf(keyMap.value(consoleSettings->getScrollUpKB())));
        scrollDownKB->setCurrentIndex(scrollDownKB->indexOf(keyMap.value(consoleSettings->getScrollDownKB())));
        scrollLeftKB->setCurrentIndex(scrollLeftKB->indexOf(keyMap.value(consoleSettings->getScrollLeftKB())));
        scrollRightKB->setCurrentIndex(scrollRightKB->indexOf(keyMap.value(consoleSettings->getScrollRightKB())));
        scrollViewUpKB->setCurrentIndex(scrollViewUpKB->indexOf(keyMap.value(consoleSettings->getScrollViewUpKB())));
        scrollViewDownKB->setCurrentIndex(scrollViewDownKB->indexOf(keyMap.value(consoleSettings->getScrollViewDownKB())));
        scrollViewTopKB->setCurrentIndex(scrollViewTopKB->indexOf(keyMap.value(consoleSettings->getScrollViewTopKB())));
        scrollViewBottomKB->setCurrentIndex(scrollViewBottomKB->indexOf(keyMap.value(consoleSettings->getScrollViewBottomKB())));
        allGamepadsUIControl->setChecked(consoleSettings->getAllGamepadsUIControl());

        onAppQuitBehaviourChanged(appQuitBehaviour->getCurrentIndex());
    }

    void ClientOptions::addGamepad(const QString &gamepadName, const QString &gamepadID) {
        const QSignalBlocker gamepadBlock {gamepad};

        gamepad->addOption(gamepadName);
        gamepadMapsCache.insert(gamepadID, consoleSettings->getGamepadMap(gamepadID));
        gamepadIDCacheList.append(gamepadID);

        if (gamepad->count() == 1) {
            enableGamepadSettings(true);
            onGamepadChanged(gamepad->getCurrentIndex());

        } else if (gamepadID == consoleSettings->getPreferredGamepadID()) {
            const int idx = gamepad->count() - 1;

            gamepad->setCurrentIndex(idx);
            onGamepadChanged(idx);
        }
    }

    void ClientOptions::removeGamepad(const QString &gamepadID) {
        const QSignalBlocker gamepadBlock {gamepad};
        const int rmIdx = gamepadIDCacheList.indexOf(gamepadID);
        const int curIdx = gamepad->getCurrentIndex();

        gamepad->removeAt(rmIdx);
        gamepadIDCacheList.removeAt(rmIdx);
        gamepadMapsCache.remove(gamepadID);

        if (gamepad->count() == 0) {
            enableGamepadSettings(false);
            prevGamepadIdx = -1;

        } else if (rmIdx == curIdx) {
            prevGamepadIdx = -1;
            gamepad->setCurrentIndex(0);
            onGamepadChanged(0);

        } else if (rmIdx < curIdx) {
            prevGamepadIdx = gamepad->getCurrentIndex();
        }
    }

    QString ClientOptions::getFirstGamepadID() const {
        return gamepadIDCacheList.isEmpty() ? "" : gamepadIDCacheList[0];
    }

    void ClientOptions::onAppQuitBehaviourChanged(const int idx) const {
        const AppQuitBehaviour quitBhv = static_cast<AppQuitBehaviour>(idx);
        const bool isQuit = quitBhv == AppQuitBehaviour::Quit;

        floatIconSize->setEnabled(quitBhv == AppQuitBehaviour::FloatingIcon);
        runAtStartup->setEnabled(!isQuit);
        runAtStartup->setChecked(!isQuit && runAtStartup->isChecked());
    }

    void ClientOptions::onGamepadChanged(const int idx) {
        if (idx == prevGamepadIdx || gamepad->count() == 0)
            return;

        if (prevGamepadIdx != -1)
            updateGamepadMappings(gamepadIDCacheList[prevGamepadIdx]);

        loadGamepadMappings(gamepadIDCacheList[idx]);
        prevGamepadIdx = idx;
    }

    void ClientOptions::onReloadGamepadMappingsBtnClicked() {
        emit reloadGamepadMappings();
    }

    void ClientOptions::onSaveBtnClicked() {
        saveBnt->setEnabled(false);

        const std::unique_ptr<StartupUtil> startupUtil = getStartupUtil("powertunerconsole");
        QString gamepadID = "";

        if (gamepad->count() > 0) {
            gamepadID = gamepadIDCacheList[gamepad->getCurrentIndex()];
            updateGamepadMappings(gamepadID); // refresh selected gamepad mapping cache
        }

        consoleSettings->setAppQuitBehaviour(static_cast<AppQuitBehaviour>(appQuitBehaviour->getCurrentIndex()));
        consoleSettings->setFloatIconSize(static_cast<FloatIconSize>(floatIconSize->getCurrentIndex()));
        consoleSettings->setAutoConnectDaemonName(autoConnectDaemon->getCurrentText());
        consoleSettings->setEnableTTS(enableTTS->isChecked());
        consoleSettings->setRememberWindowState(rememberWindowState->isChecked());
        consoleSettings->setPrevTabKB(keyMap.key(prevTabKB->getCurrentText()));
        consoleSettings->setNextTabKB(keyMap.key(nextTabKB->getCurrentText()));
        consoleSettings->setUpKB(keyMap.key(upKB->getCurrentText()));
        consoleSettings->setDownKB(keyMap.key(downKB->getCurrentText()));
        consoleSettings->setLeftKB(keyMap.key(leftKB->getCurrentText()));
        consoleSettings->setFastLeftKB(keyMap.key(fastLeftKB->getCurrentText()));
        consoleSettings->setRightKB(keyMap.key(rightKB->getCurrentText()));
        consoleSettings->setFastRightKB(keyMap.key(fastRightKB->getCurrentText()));
        consoleSettings->setClickKB(keyMap.key(clickKB->getCurrentText()));
        consoleSettings->setBackKB(keyMap.key(backKB->getCurrentText()));
        consoleSettings->setReloadKB(keyMap.key(reloadKB->getCurrentText()));
        consoleSettings->setApplyKB(keyMap.key(applyKB->getCurrentText()));
        consoleSettings->setInputModeKB(keyMap.key(inputModeKB->getCurrentText()));
        consoleSettings->setScrollUpKB(keyMap.key(scrollUpKB->getCurrentText()));
        consoleSettings->setScrollDownKB(keyMap.key(scrollDownKB->getCurrentText()));
        consoleSettings->setScrollLeftKB(keyMap.key(scrollLeftKB->getCurrentText()));
        consoleSettings->setScrollRightKB(keyMap.key(scrollRightKB->getCurrentText()));
        consoleSettings->setScrollViewUpKB(keyMap.key(scrollViewUpKB->getCurrentText()));
        consoleSettings->setScrollViewDownKB(keyMap.key(scrollViewDownKB->getCurrentText()));
        consoleSettings->setScrollViewTopKB(keyMap.key(scrollViewTopKB->getCurrentText()));
        consoleSettings->setScrollViewBottomKB(keyMap.key(scrollViewBottomKB->getCurrentText()));
        consoleSettings->setAllGamepadsUIControl(allGamepadsUIControl->isChecked());
        consoleSettings->setPreferredGamepadID(gamepadID);

        // save all mappings
        for (const auto &[gid, gmap]: gamepadMapsCache.asKeyValueRange())
            consoleSettings->setGamepadMap(gid, gmap);

        if (runAtStartup->isChecked()) {
            if (!startupUtil->setup())
                UI::UILogger::getInstance()->writeLog(QStringLiteral("Failed to create startup file"));

        } else {
            if (!startupUtil->unset())
                UI::UILogger::getInstance()->writeLog(QStringLiteral("Failed to remove from startup"));
        }

        consoleSettings->save();
        emit clientSettingsSaved();
        saveBnt->setEnabled(true);
        saveBnt->setFocus(Qt::TabFocusReason);
    }

    void ClientOptions::onResetDefaultBtnClicked() {
        prevGamepadIdx = -1;

        consoleSettings->reset();
        loadSettings();

        if (gamepad->count()) {
            // reset mappings in cache
            for (auto [gid, gdata]: gamepadMapsCache.asKeyValueRange())
                gdata = {};

            gamepad->setCurrentIndex(0);
            consoleSettings->setPreferredGamepadID(gamepadIDCacheList[0]); // auto select first one
            onGamepadChanged(0);
        }

        consoleSettings->save();
        emit clientSettingsSaved();
    }
}
