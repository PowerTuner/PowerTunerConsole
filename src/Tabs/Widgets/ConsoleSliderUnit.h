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

#include <functional>

#include "ConsoleSliderBase.h"

namespace PWT::CUI {
    class ConsoleSliderUnit final: public ConsoleSliderBase {
        Q_OBJECT

    private:
        QLabel *unitLabel = nullptr;
        QLabel *unitValue = nullptr;
        std::function<void(QLabel *, int)> unitValueCB;

    protected:
        void sendTTS(int value) override;

    public:
        ConsoleSliderUnit(const QString &unit, const std::function<void(QLabel *, int)> &unitValueCallback, bool hasLabel = true, const QString &title = "", const QString &help = "");

        [[nodiscard]] QString getFocusedTTS() const override;
        void setUnitLabel(const QString &unit) const;
        void setUnitValue(const QString &text) const;
        void setUnitValue(int value) const;
        void setUnitValue(double value) const;

    protected slots:
        void onSliderChanged(int v) override;
        void onSpinEditChanged(int v) override;
    };
}
