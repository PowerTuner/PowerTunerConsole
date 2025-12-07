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

#include "../../../Include/AddonTab.h"
#include "../../../Widgets/ConsoleLabel.h"
#include "Widgets/SchemeSelectInput.h"
#include "Widgets/SchemesOptionsWidget.h"
#include "Widgets/SchemeSettingsWidget.h"

namespace PWT::CUI::WIN {
	class PowerSchemeTab final: public AddonTab {
		Q_OBJECT

	private:
		struct SchemeCacheData final {
			QString friendlyName;
			QString guid;
			bool deleteFlag;
			bool resetFlag;
		};

		ConsoleButton *switchViewBtn = nullptr;
        SchemeSelectInput *schemeSelectInput = nullptr;
		SchemesOptionsWidget *schemeOptions = nullptr;
		SchemeSettingsWidget *schemeSettingsWidget = nullptr;
		ConsoleLabel *activeSchemeLbl = nullptr;
		ConsoleLineEdit *searchBox = nullptr;
        ConsoleSelect *modeAll = nullptr;
		ConsoleButton *prevPageBtn = nullptr;
		ConsoleButton *nextPageBtn = nullptr;
		mutable QHash<QString, SchemeCacheData> schemesCache;
		int guidTbdID = 0;

		[[nodiscard]] QString makeSelectLabel(const QString &schemeGuid, const QString &friendlyName) const;
        void hideSettings() const;
		void setOptionsVisible(bool visible) const;
		void setOptionsEnabled(bool enable) const;
		void disablePageButtons() const;
		void enablePageButtons() const;

	public:
		PowerSchemeTab();

		void refreshTab(const PWTS::DaemonPacket &packet) override;
		void setDataForPacket(PWTS::ClientPacket &packet) const override;

	private slots:
		void onSchemeSelected(const QString &scheme) const;
		void onSwitchViewBtnClicked() const;
		void onPrevBtnClicked() const;
		void onNextBtnClicked() const;
		void onSetSchemeActive() const;
		void onDuplicateScheme(const QString &baseScheme, const QString &duplicateName);
		void onSetDeleteFlag(bool flagged) const;
		void onSetResetFlag(bool flagged) const;
		void onDeleteScheme() const;
		void onSearchSetting() const;
		void onModeAllChanged(int idx) const;
		void onCopySettings(const QString &fromScheme, int fromMode, const QString &toScheme, int toMode) const;
	};
}
