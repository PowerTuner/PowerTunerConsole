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
#include "FanControlWidget.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::CUI::FANS {
    FanControlWidget::FanControlWidget(const QSet<PWTS::Feature> &features, const QString &id) {
        FanCurveWidget *curveWdg = new FanCurveWidget();

        lyt = new QVBoxLayout();
        mode = new ConsoleSelect("Mode");
        fanID = id;

        mode->addOptions({"Auto", "Manual"});
        curveWdg->disableRemoveBtn();
        curveWidgetList.append(curveWdg);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(mode);

        if (features.contains(PWTS::Feature::FAN_CURVE)) {
            addPointBtn = new ConsoleButton("Add trip point");

            lyt->addWidget(addPointBtn);

            QObject::connect(addPointBtn, &ConsoleButton::clicked, this, &FanControlWidget::onAddPointBtnClicked);
        }

        lyt->addWidget(curveWdg);

        setLayout(lyt);
        onModeChanged(mode->getCurrentIndex());
        setEnabled(false);

        QObject::connect(mode, &ConsoleSelect::selectionChanged, this, &FanControlWidget::onModeChanged);
    }

    void FanControlWidget::addTripPoint() {
        FanCurveWidget *wdg = new FanCurveWidget();

        lyt->addWidget(wdg);
        curveWidgetList.append(wdg);
        wdg->setEnabled(mode->getCurrentIndex() == 1);

        QObject::connect(wdg, &FanCurveWidget::removePoint, this, &FanControlWidget::onRemovePoint);
    }

    void FanControlWidget::setData(const PWTS::DaemonPacket &packet) {
        const bool isValid = packet.fanData.contains(fanID) && packet.fanData[fanID].mode.isValid();

        setEnabled(isValid);

        if (!isValid) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        mode->setCurrentIndex(packet.fanData[fanID].mode.getValue());

        for (int i=0,l=packet.fanData[fanID].curve.size(); i<l; ++i) {
            const auto [temp, speed] = packet.fanData[fanID].curve[i];

            if (i >= curveWidgetList.size())
                addTripPoint();

            curveWidgetList[i]->setData(temp, speed);
        }

        // remove unused trip points, if any
        for (int i=0,l=(curveWidgetList.size() - packet.fanData[fanID].curve.size()); i<l; ++i) {
            if (curveWidgetList.size() == 1) { // do not delete the last one
                curveWidgetList.last()->setData(0, 0);
                break;
            }

            FanCurveWidget *wdg = curveWidgetList.takeLast();

            lyt->removeWidget(wdg);
            delete wdg;
        }
    }

    PWTS::FanData FanControlWidget::getData() const {
        PWTS::FanData data;

        data.mode.setValue(mode->getCurrentIndex(), isEnabled());

        for (const FanCurveWidget *curve: curveWidgetList)
            data.curve.append(std::make_pair(curve->getTemperature(), curve->getSpeed()));

        return data;
    }

    void FanControlWidget::onModeChanged(const int idx) const {
        const bool isManual = idx != 0;

        addPointBtn->setEnabled(isManual);

        for (FanCurveWidget *wdg: curveWidgetList)
            wdg->setEnabled(isManual);
    }

    void FanControlWidget::onAddPointBtnClicked() {
        addTripPoint();
        emit addPointToFocusList(curveWidgetList.last());
    }

    void FanControlWidget::onRemovePoint(const QWidget *wdg) {
        const int idx = curveWidgetList.indexOf(wdg);

        if (idx == -1) {
            UI::UILogger::getInstance()->writeLog(QStringLiteral("failed to delete trip point"));
            return;
        }

        FanCurveWidget *curveWdg = curveWidgetList.takeAt(idx);

        lyt->removeWidget(curveWdg);
        emit removePoint(curveWdg);
    }
}
