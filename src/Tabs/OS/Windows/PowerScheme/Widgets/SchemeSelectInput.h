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

#include "../../../../Widgets/ConsoleSelect.h"

namespace PWT::CUI::WIN {
	class SchemeSelectInput final: public QWidget {
		Q_OBJECT

	private:
		ConsoleSelect *schemeSelect = nullptr;
		QString prevSelected;

	public:
		SchemeSelectInput();

		[[nodiscard]] QString getCurrentScheme() const { return schemeSelect->getCurrentText(); }
		[[nodiscard]] bool hasSchemes() const { return schemeSelect->count(); }
		void deleteCurrentScheme() const { schemeSelect->removeAt(schemeSelect->getCurrentIndex()); }

		void clearInput() const;
		void addScheme(const QString &label) const;
		void restoreSelectedScheme();

	private slots:
		void onSchemeChanged(const QString &scheme);

	signals:
		void schemeSelected(const QString &scheme);
	};
}
