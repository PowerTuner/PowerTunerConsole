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
#include <QHBoxLayout>

#include "SchemeSelectInput.h"

namespace PWT::CUI::WIN {
	SchemeSelectInput::SchemeSelectInput() {
		QHBoxLayout *lyt = new QHBoxLayout();

		schemeSelect = new ConsoleSelect("Scheme");

		lyt->setContentsMargins(0, 0, 0, 0);
		lyt->addWidget(schemeSelect);

		setLayout(lyt);

		QObject::connect(schemeSelect, &ConsoleSelect::textChanged, this, &SchemeSelectInput::onSchemeChanged);
	}

	void SchemeSelectInput::clearInput() const {
		const QSignalBlocker sblock {schemeSelect};

		schemeSelect->clear();
	}

	void SchemeSelectInput::addScheme(const QString &label) const {
		const QSignalBlocker sblock {schemeSelect};

		schemeSelect->addOption(label);
	}

	void SchemeSelectInput::restoreSelectedScheme() {
		const QSignalBlocker sblock {schemeSelect};
		const int prevIdx = schemeSelect->indexOf(prevSelected);

		schemeSelect->setCurrentIndex(prevIdx == -1 ? 0 : prevIdx);
		onSchemeChanged(schemeSelect->getCurrentText());
	}

	void SchemeSelectInput::onSchemeChanged(const QString &scheme) {
		emit schemeSelected(scheme);
		prevSelected = scheme;
	}
}
