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

#include <QCheckBox>
#include <QLabel>

#include "ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleCheckbox final: public ConsoleWidget {
        Q_OBJECT

    private:
        QLabel *checkLbl = nullptr;
        QLabel *helpLbl = nullptr;
        QCheckBox *checkbx = nullptr;

    public:
        explicit ConsoleCheckbox(const QString &label, const QString &help = "");

        void setChecked(const bool checked) const { checkbx->setChecked(checked); }
        [[nodiscard]] bool isChecked() const { return checkbx->isChecked(); }

        [[nodiscard]] QString getFocusedTTS() const override;
        void clickAction(bool isGamepad) override;

    private slots:
        void onCheckStateChanged(Qt::CheckState state);

    signals:
        void checkStateChanged(Qt::CheckState state);
    };
}
