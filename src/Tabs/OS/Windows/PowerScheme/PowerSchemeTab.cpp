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
#include "PowerSchemeTab.h"

namespace PWT::CUI::WIN {
	PowerSchemeTab::PowerSchemeTab(): AddonTab() {
		QHBoxLayout *pageBtnLyt = new QHBoxLayout();

		switchViewBtn = new ConsoleButton("Show options");
        schemeSelectInput = new SchemeSelectInput();
		schemeSettingsWidget = new SchemeSettingsWidget();
        schemeOptions = new SchemesOptionsWidget();
        searchBox = new ConsoleLineEdit();
		modeAll = new ConsoleSelect("Mode (All)");
		activeSchemeLbl = new ConsoleLabel("Active scheme", "-");
		prevPageBtn = new ConsoleButton("< Previous");
		nextPageBtn = new ConsoleButton("Next >");

        searchBox->setPlaceholder("Search setting name or group");
		modeAll->addOptions({"", "Plugged In", "Battery"});
		schemeOptions->setVisible(false);
		disablePageButtons();

		pageBtnLyt->addWidget(prevPageBtn);
		pageBtnLyt->addWidget(nextPageBtn);
		scrollWidgLyt->insertWidget(nextInsertIdx(), activeSchemeLbl);
		scrollWidgLyt->insertWidget(nextInsertIdx(), schemeSelectInput);
        scrollWidgLyt->insertWidget(nextInsertIdx(), searchBox);
		scrollWidgLyt->insertWidget(nextInsertIdx(), modeAll);
		scrollWidgLyt->insertWidget(nextInsertIdx(), switchViewBtn);
		scrollWidgLyt->insertLayout(nextInsertIdx(), pageBtnLyt);
		scrollWidgLyt->insertWidget(nextInsertIdx(), schemeOptions);
		scrollWidgLyt->insertWidget(nextInsertIdx(), schemeSettingsWidget);

		QObject::connect(schemeSelectInput, &SchemeSelectInput::schemeSelected, this, &PowerSchemeTab::onSchemeSelected);
		QObject::connect(switchViewBtn, &ConsoleButton::clicked, this, &PowerSchemeTab::onSwitchViewBtnClicked);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::setSchemeActive, this, &PowerSchemeTab::onSetSchemeActive);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::createDuplicate, this, &PowerSchemeTab::onDuplicateScheme);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::setDeleteFlag, this, &PowerSchemeTab::onSetDeleteFlag);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::setResetFlag, this, &PowerSchemeTab::onSetResetFlag);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::deleteScheme, this, &PowerSchemeTab::onDeleteScheme);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::copySettings, this, &PowerSchemeTab::onCopySettings);
		QObject::connect(modeAll, &ConsoleSelect::selectionChanged, this, &PowerSchemeTab::onModeAllChanged);
		QObject::connect(searchBox, &ConsoleLineEdit::returnPressed, this, &PowerSchemeTab::onSearchSetting);
		QObject::connect(prevPageBtn, &ConsoleButton::clicked, this, &PowerSchemeTab::onPrevBtnClicked);
		QObject::connect(nextPageBtn, &ConsoleButton::clicked, this, &PowerSchemeTab::onNextBtnClicked);
	}

	void PowerSchemeTab::refreshTab(const PWTS::DaemonPacket &packet) {
		const QSharedPointer<PWTS::WIN::WindowsData> wdata = packet.windowsData;

		schemeSettingsWidget->disableSettingWidgets();
		schemeSelectInput->clearInput();
		schemeOptions->clearWidget();
		schemesCache.clear();

		for (const auto &[schemeGuid, schemeData]: wdata->schemes.asKeyValueRange()) {
			const QString selectLbl = makeSelectLabel(schemeGuid, schemeData.friendlyName);

			for (const auto &[settingGuid, setting]: schemeData.settings.asKeyValueRange()) {
				schemeSettingsWidget->addSetting(settingGuid,  wdata->schemeOptionsData[settingGuid]); // create/enable setting
				schemeSettingsWidget->addData(schemeGuid, settingGuid, setting);
			}

			if (wdata->activeScheme == schemeGuid) [[unlikely]]
				activeSchemeLbl->setText(selectLbl);

			schemeSelectInput->addScheme(selectLbl);
			schemeOptions->addScheme(selectLbl);
			schemesCache.insert(selectLbl, {
				.friendlyName = schemeData.friendlyName,
				.guid = schemeGuid,
				.deleteFlag = schemeData.deleteFlag,
				.resetFlag = schemeData.resetFlag
			});
		}

		schemeOptions->refreshWidget(packet);
		schemeSelectInput->restoreSelectedScheme();
		schemeSettingsWidget->nextPage(searchBox->getText());
		schemeSettingsWidget->showPage();
		enablePageButtons();
		setupFocusableWidgets();
	}

	void PowerSchemeTab::setDataForPacket(PWTS::ClientPacket &packet) const {
		QMap<QString, PWTS::WIN::PowerScheme> schemes;

		for (const auto &[selectLbl, schemeData]: schemesCache.asKeyValueRange()) {
			const PWTS::WIN::PowerScheme pscheme {
				.settings = schemeSettingsWidget->getWidgetsData(schemeData.guid),
				.friendlyName = schemeData.friendlyName,
				.deleteFlag = schemeData.deleteFlag,
				.resetFlag = schemeData.resetFlag
			};

			schemes.insert(schemeData.guid, pscheme);
		}

		packet.windowsData->schemes = schemes;
		packet.windowsData->activeScheme = schemesCache[activeSchemeLbl->getText()].guid;

		schemeOptions->setDataForPacket(packet);
	}

	QString PowerSchemeTab::makeSelectLabel(const QString &schemeGuid, const QString &friendlyName) const {
		return QString("%1 %2").arg(schemeGuid, friendlyName);
	}

    void PowerSchemeTab::hideSettings() const {
		schemeSettingsWidget->hidePage();
		setOptionsVisible(false);
		schemeOptions->setCurrentSettingsVisible(false);
    }

	void PowerSchemeTab::setOptionsVisible(const bool visible) const {
		schemeOptions->setVisible(visible);
		schemeSettingsWidget->setVisible(!visible);
		prevPageBtn->setVisible(!visible);
		nextPageBtn->setVisible(!visible);
		switchViewBtn->setText(visible ? QStringLiteral("Show scheme settings") : QStringLiteral("Show options"));
	}

	void PowerSchemeTab::setOptionsEnabled(const bool enable) const {
		switchViewBtn->setEnabled(enable);
		schemeOptions->setEnabled(enable);
	}

	void PowerSchemeTab::disablePageButtons() const {
		prevPageBtn->setEnabled(false);
		nextPageBtn->setEnabled(false);
	}

	void PowerSchemeTab::enablePageButtons() const {
		const bool hasPrevPage = schemeSettingsWidget->hasPreviousPage();
		const bool hasNextPage = schemeSettingsWidget->hasNextPage();

		prevPageBtn->setEnabled(hasPrevPage);
		nextPageBtn->setEnabled(hasNextPage);
	}

	void PowerSchemeTab::onSchemeSelected(const QString &scheme) const {
		if (scheme.isEmpty()) {
			hideSettings();
			return;
		}

		const SchemeCacheData &selected = schemesCache[scheme];

		schemeSettingsWidget->switchScheme(selected.guid);
		schemeOptions->switchScheme(scheme, scheme == activeSchemeLbl->getText(), selected.deleteFlag, selected.resetFlag);
		schemeOptions->setCurrentSettingsVisible(true);
		scrollToTop();
	}

	void PowerSchemeTab::onSwitchViewBtnClicked() const {
		setOptionsVisible(!schemeOptions->isVisible());
	}

	void PowerSchemeTab::onPrevBtnClicked() const {
		disablePageButtons();
		schemeSettingsWidget->previousPage(searchBox->getText());
		scrollToTop();
		enablePageButtons();
		prevPageBtn->setFocus(Qt::TabFocusReason);
	}

	void PowerSchemeTab::onNextBtnClicked() const {
		disablePageButtons();
		schemeSettingsWidget->nextPage(searchBox->getText());
		scrollToTop();
		enablePageButtons();
		nextPageBtn->setFocus(Qt::TabFocusReason);
	}

    void PowerSchemeTab::onModeAllChanged(const int idx) const {
        if (idx == 0)
            return;

        schemeSettingsWidget->setModeAll(idx - 1);
    }

	void PowerSchemeTab::onSetSchemeActive() const {
		activeSchemeLbl->setText(schemeSelectInput->getCurrentScheme());
	}

	void PowerSchemeTab::onDuplicateScheme(const QString &baseScheme, const QString &duplicateName) {
		setOptionsEnabled(false);

		const QString baseGuid = schemesCache[baseScheme].guid;
		const QString tmpGuid = QString("{guid-tbd-%1}").arg(++guidTbdID);
		const QString comboLbl = makeSelectLabel(tmpGuid, duplicateName);

		schemeSettingsWidget->duplicateSettings(baseGuid, tmpGuid);
		schemeSelectInput->addScheme(comboLbl);
		schemeOptions->addDuplicatedScheme(comboLbl);

		schemesCache.insert(comboLbl, {
			.friendlyName = duplicateName,
			.guid = tmpGuid
		});

		setOptionsEnabled(true);
	}

	void PowerSchemeTab::onSetDeleteFlag(const bool flagged) const {
		const QString scheme = schemeSelectInput->getCurrentScheme();

		if (scheme.isEmpty())
			return;

		schemesCache[scheme].deleteFlag = flagged;
	}

	void PowerSchemeTab::onSetResetFlag(const bool flagged) const {
		const QString scheme = schemeSelectInput->getCurrentScheme();

		if (scheme.isEmpty())
			return;

		schemesCache[scheme].resetFlag = flagged;
	}

	void PowerSchemeTab::onDeleteScheme() const {
		const QString current = schemeSelectInput->getCurrentScheme();

		schemeOptions->removeScheme(current);
		schemeSettingsWidget->deleteScheme(schemesCache[current].guid);
		schemeSelectInput->deleteCurrentScheme();
		schemesCache.remove(current);
	}

    void PowerSchemeTab::onSearchSetting() const {
        schemeSettingsWidget->filterVisibleSettings(searchBox->getText());
		enablePageButtons();
        scrollViewTop();
    }

	void PowerSchemeTab::onCopySettings(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) const {
		setOptionsEnabled(false);

		const QString fromGuid = schemesCache[fromScheme].guid;
		const QString toGuid = schemesCache[toScheme].guid;

		schemeSettingsWidget->copySchemeValues(fromGuid, fromMode, toGuid, toMode);

		setOptionsEnabled(true);
	}
}
