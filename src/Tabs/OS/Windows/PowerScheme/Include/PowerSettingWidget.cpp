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
#include "PowerSettingWidget.h"
#include "../../../../Widgets/ConsoleLabel.h"
#include "../../../../Widgets/SectionWidget.h"

namespace PWT::CUI::WIN {
	PowerSettingWidget::PowerSettingWidget(const QString &title, const QString &groupTitle, const QString &guid) {
		lyt = new QVBoxLayout();
		mode = new ConsoleSelect(QStringLiteral("Mode"));
		settingGroupTitle = groupTitle.isEmpty() ? QStringLiteral("No group") : groupTitle;
		settingTitle = title;

		mode->addOptions({QStringLiteral("Plugged In"), QStringLiteral("Battery")});
		lyt->setContentsMargins(0, 0, 0, 0);
		lyt->addWidget(new SectionWidget(title));
		lyt->addWidget(new ConsoleLabel("GUID", guid));
		lyt->addWidget(new ConsoleLabel("Group", settingGroupTitle));
		lyt->addWidget(mode);

		setLayout(lyt);

		QObject::connect(mode, &ConsoleSelect::selectionChanged, this, &PowerSettingWidget::onModeChanged);
	}

	void PowerSettingWidget::setData(const QString &scheme, const PWTS::WIN::PowerSchemeSetting &setting) {
		settingGroupGuid = setting.groupGuid;
		settings.insert(scheme, setting.value);
	}

	void PowerSettingWidget::switchScheme(const QString &scheme) {
		currentScheme = scheme;

		onModeChanged(mode->getCurrentIndex());
	}

	void PowerSettingWidget::duplicateScheme(const QString &current, const QString &duplicate) {
		settings.insert(duplicate, settings.value(current));
	}

	void PowerSettingWidget::deleteScheme(const QString &scheme) {
		settings.remove(scheme);
	}

    void PowerSettingWidget::setMode(const int idx) const {
        mode->setCurrentIndex(idx);
    }

    void PowerSettingWidget::copyValue(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) {
        if (toMode == 0)
            settings[toScheme].ac = fromMode == 0 ? settings[fromScheme].ac : settings[fromScheme].dc;
        else if (toMode == 1)
            settings[toScheme].dc = fromMode == 0 ? settings[fromScheme].ac : settings[fromScheme].dc;
        else
            settings[toScheme] = settings[fromScheme];

        if (fromScheme == toScheme) // refresh ui
            onModeChanged(mode->getCurrentIndex());
    }

	void PowerSettingWidget::onSettingValueChanged(const int v) {
		PWTS::WIN::PowerSettingValue sett = settings.value(currentScheme);

		if (mode->getCurrentIndex() == 0)
			sett.ac = v;
		else
			sett.dc = v;

		settings.insert(currentScheme, sett);
	}
}
