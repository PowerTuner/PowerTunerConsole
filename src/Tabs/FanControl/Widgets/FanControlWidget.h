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

#include "FanCurveWidget.h"
#include "../../Widgets/ConsoleSelect.h"
#include "pwtShared/Include/Feature.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::CUI::FANS {
    class FanControlWidget final: public QWidget {
        Q_OBJECT

    private:
        QVBoxLayout *lyt = nullptr;
        ConsoleSelect *mode = nullptr;
        ConsoleButton *addPointBtn = nullptr;
        QList<FanCurveWidget *> curveWidgetList;
        QString fanID;

        void addTripPoint();

    public:
        FanControlWidget(const QSet<PWTS::Feature> &features, const QString &id);

        [[nodiscard]] QString getFanID() const { return fanID; }

        void setData(const PWTS::DaemonPacket &packet);
        [[nodiscard]] PWTS::FanData getData() const;

    private slots:
        void onModeChanged(int idx) const;
        void onAddPointBtnClicked();
        void onRemovePoint(const QWidget *wdg);

    signals:
        void addPointToFocusList(const QWidget *wdg);
        void removePoint(const QWidget *wdg);
    };
}
