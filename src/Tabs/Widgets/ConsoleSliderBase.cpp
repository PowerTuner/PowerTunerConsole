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
#include "ConsoleSliderBase.h"

namespace PWT::CUI {
    ConsoleSliderBase::ConsoleSliderBase(const QString &help) {
        QPalette sliderPal;
        QPalette spinEditPal;

        lyt = new QVBoxLayout();
        slider = new QSlider(Qt::Horizontal);
        spinEdit = new QSpinBox();
        sliderPal = slider->palette();
        spinEditPal = spinEdit->palette();

        slider->setFixedHeight(35);
        slider->setFocusPolicy(Qt::NoFocus);
        sliderPal.setColor(QPalette::Window, grooveNoFocusColor); // groove
        sliderPal.setColor(QPalette::Button, QColor(193, 194, 199)); // handle
        slider->setPalette(sliderPal);
        spinEdit->setVisible(false);
        spinEditPal.setColor(QPalette::Base, QColor(26, 26, 30));
        spinEdit->setPalette(spinEditPal);

        lyt->addWidget(slider);
        lyt->addWidget(spinEdit);

        if (!help.isEmpty()) {
            helpLbl = new QLabel(help);

            QFont font = helpLbl->font();

            font.setPointSize(10);
            font.setStyle(QFont::StyleItalic);
            font.setWeight(QFont::Normal);
            helpLbl->setFont(font);
            helpLbl->setAlignment(Qt::AlignLeft);
            helpLbl->setWordWrap(true);

            lyt->addSpacing(4);
            lyt->addWidget(helpLbl);
        }

        setLayout(lyt);

        QObject::connect(slider, &QSlider::valueChanged, this, &ConsoleSliderBase::onSliderChanged);
        QObject::connect(spinEdit, &QSpinBox::valueChanged, this, &ConsoleSliderBase::onSpinEditChanged);
    }

    void ConsoleSliderBase::leftAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->value() - 1);
    }

    void ConsoleSliderBase::fastLeftAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->value() - slider->pageStep());
    }

    void ConsoleSliderBase::rightAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->value() + 1);
    }

    void ConsoleSliderBase::fastRightAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->value() + slider->pageStep());
    }

    void ConsoleSliderBase::scrollUpAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->minimum());
    }

    void ConsoleSliderBase::scrollDownAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->maximum());
    }

    void ConsoleSliderBase::scrollLeftAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->value() - slider->pageStep() * 2);
    }

    void ConsoleSliderBase::scrollRightAction() {
        if (!isSlider)
            return;

        slider->setValue(slider->value() + slider->pageStep() * 2);
    }

    void ConsoleSliderBase::clickAction(const bool isGamepad) {
        if (!isSlider)
            spinEdit->setFocus(Qt::TabFocusReason);
    }

    void ConsoleSliderBase::backAction() {
        if (!isSlider)
            spinEdit->parentWidget()->setFocus(Qt::TabFocusReason);
    }

    void ConsoleSliderBase::inputModeAction() {
        if (isSlider) {
            const QSignalBlocker sblock {spinEdit};

            slider->setVisible(false);
            spinEdit->setVisible(true);
            spinEdit->setValue(currentValue);
            isSlider = false;

            if (hasTTSEnabled())
                emit sayStoppableTTS(tr("switched to numeric input"));

        } else {
            const QSignalBlocker sblock {slider};

            spinEdit->setVisible(false);
            slider->setVisible(true);
            slider->setValue(currentValue);
            isSlider = true;

            if (hasTTSEnabled())
                emit sayStoppableTTS(tr("switched to slider input"));
        }
    }

    void ConsoleSliderBase::focusInEvent(QFocusEvent *event) {
        QPalette pal = slider->palette();

        pal.setColor(QPalette::Window, grooveFocusColor);
        slider->setPalette(pal);
        ConsoleWidget::focusInEvent(event);
    }

    void ConsoleSliderBase::focusOutEvent(QFocusEvent *event) {
        QPalette pal = slider->palette();

        pal.setColor(QPalette::Window, grooveNoFocusColor);
        slider->setPalette(pal);
        ConsoleWidget::focusOutEvent(event);
    }

    void ConsoleSliderBase::setRange(const int min, const int max) const {
        slider->setRange(min, max);
        spinEdit->setRange(min, max);
    }

    void ConsoleSliderBase::setStaticLabel(const QString &text) const {
        staticLabel = true;
        label->setText(text);
    }

    void ConsoleSliderBase::setValue(const int value) const {
        if (isSlider)
            slider->setValue(value);
        else
            spinEdit->setValue(value);
    }

    void ConsoleSliderBase::sendTTS(const int value) {
        if (hasTTSEnabled())
            emit sayStoppableTTS(QString::number(value));
    }

    QString ConsoleSliderBase::getFocusedTTS() const {
        return tr("%1 %2 set to %3 %4")
            .arg(titleLbl.isNull() ? (staticLabel ? label->text():"") : titleLbl->text())
            .arg(isSlider ? "slider" : "numeric input")
            .arg(isSlider ? slider->value() : spinEdit->value())
            .arg(helpLbl.isNull() ? "" : tr(", help: %1").arg(helpLbl->text()));
    }

    void ConsoleSliderBase::onSliderChanged(const int v) {
        if (!label.isNull() && !staticLabel)
            label->setNum(v);

        currentValue = v;
        emit valueChanged(v);
        sendTTS(v);
    }

    void ConsoleSliderBase::onSpinEditChanged(const int v) {
        if (v < slider->minimum() || v > slider->maximum())
            return;

        if (!label.isNull() && !staticLabel)
            label->setNum(v);

        currentValue = v;
        emit valueChanged(v);
        sendTTS(v);
    }
}
