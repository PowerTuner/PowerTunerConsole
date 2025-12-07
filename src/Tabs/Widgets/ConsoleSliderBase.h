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

#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QPointer>
#include <QVBoxLayout>

#include "ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleSliderBase: public ConsoleWidget {
        Q_OBJECT

    private:
        const QColor grooveNoFocusColor = QColor(108, 109, 127);
        const QColor grooveFocusColor = QColor(38, 43, 54);
        QSlider *slider = nullptr;
        QSpinBox *spinEdit = nullptr;
        bool isSlider = true;
        mutable bool staticLabel = false;
        int currentValue = 0;

    protected:
        QVBoxLayout *lyt = nullptr;
        QPointer<QLabel> titleLbl;
        QPointer<QLabel> label;
        QPointer<QLabel> helpLbl;

        void focusInEvent(QFocusEvent *event) override;
        void focusOutEvent(QFocusEvent *event) override;
        virtual void sendTTS(int value);

    public:
        explicit ConsoleSliderBase(const QString &help);

        [[nodiscard]] int getValue() const { return currentValue; }
        [[nodiscard]] int getMinumum() const { return slider->minimum(); }
        [[nodiscard]] int getMaximum() const { return slider->maximum(); }
        void setMaximum(const int max) const { slider->setMaximum(max); }
        void setPageStep(const int step) const { slider->setPageStep(step); }
        void setSingleStep(const int step) const { slider->setSingleStep(step); }
        void setLabel(const QString &text) const { label->setText(text); }

        [[nodiscard]] QString getFocusedTTS() const override;
        void leftAction() override;
        void fastLeftAction() override;
        void rightAction() override;
        void fastRightAction() override;
        void scrollUpAction() override;
        void scrollDownAction() override;
        void scrollLeftAction() override;
        void scrollRightAction() override;
        void clickAction(bool isGamepad) override;
        void backAction() override;
        void inputModeAction() override;
        void setRange(int min, int max) const;
        virtual void setStaticLabel(const QString &text) const;
        void setValue(int value) const;

    protected slots:
        virtual void onSliderChanged(int v);
        virtual void onSpinEditChanged(int v);

    signals:
        void valueChanged(int v);
    };
}
