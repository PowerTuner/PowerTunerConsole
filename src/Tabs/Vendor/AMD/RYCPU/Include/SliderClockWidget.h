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

#include "../../../../Widgets/ConsoleSliderUnit.h"
#include "../../../../Widgets/ConsoleCheckbox.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::CUI::AMD {
    class SliderClockWidget: public QWidget {
        Q_OBJECT

    protected:
        ConsoleSliderUnit *slider = nullptr;
        QPointer<ConsoleCheckbox> dontSet;

        int getValue() const { return !dontSet.isNull() && dontSet->isChecked() ? -1 : slider->getValue(); }

    public:
        explicit SliderClockWidget(const QString &label, bool enableDontSet = false);

        virtual void setData(const PWTS::DaemonPacket &packet) = 0;
        virtual void setDataForPacket(const PWTS::ClientPacket &packet) const = 0;

        void setRange(const PWTS::MinMax &range) const;

    private slots:
        void onDontSetChanged(Qt::CheckState state) const;
    };
}
