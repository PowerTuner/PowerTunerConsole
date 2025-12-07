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

#include "../../Widgets/ConsoleSlider.h"
#include "../../Widgets/ConsoleButton.h"

namespace PWT::CUI::FANS {
    class FanCurveWidget final: public QWidget {
        Q_OBJECT

    private:
        ConsoleSlider *temperature = nullptr;
        ConsoleSlider *speedPerc = nullptr;
        ConsoleButton *removeBtn = nullptr;

    public:
        FanCurveWidget();

        void disableRemoveBtn() const { removeBtn->setEnabled(false); }
        [[nodiscard]] int getTemperature() const { return temperature->getValue(); }
        [[nodiscard]] int getSpeed() const { return speedPerc->getValue(); }

        void setData(int temp, int speed) const;

    private slots:
        void onRemoveBtnClicked();

    signals:
        void removePoint(const QWidget *wdg);
    };
}
