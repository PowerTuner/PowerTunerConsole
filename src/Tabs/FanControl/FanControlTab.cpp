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
#include "FanControlTab.h"
#include "../Widgets/SectionWidget.h"

namespace PWT::CUI::FANS {
    FanControlTab::FanControlTab(const PWTS::DeviceInfoPacket &packet) {
        for (const auto &[fanID, label]: packet.fanLabels.asKeyValueRange()) {
            FanControlWidget *fanControl = new FanControlWidget(packet.features.fans[fanID], fanID);

            scrollWidgLyt->insertWidget(nextInsertIdx(), new SectionWidget(label, SectionWidget::Size::Big));
            scrollWidgLyt->insertWidget(nextInsertIdx(), fanControl);
            fanControlList.append(fanControl);

            QObject::connect(fanControl, &FanControlWidget::addPointToFocusList, this, &FanControlTab::onAddPointToFocusList);
            QObject::connect(fanControl, &FanControlWidget::removePoint, this, &FanControlTab::onRemovePoint);
        }
    }

    void FanControlTab::refreshTab(const PWTS::DaemonPacket &packet) {
        for (FanControlWidget *wdg: fanControlList)
            wdg->setData(packet);

        setupFocusableWidgets();
    }

    void FanControlTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        for (const FanControlWidget *wdg: fanControlList)
            packet.fanData.insert(wdg->getFanID(), wdg->getData());
    }

    void FanControlTab::onAddPointToFocusList(const QWidget *wdg) const {
        collectConsoleWidgets(wdg->layout());
    }

    void FanControlTab::onRemovePoint(const QWidget *wdg) const {
        removeWidgetChildrenFromFocusList(wdg);
        delete wdg;
    }
}
