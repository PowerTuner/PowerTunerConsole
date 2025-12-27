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
#include <QScrollBar>

#include "ConsoleTab.h"
#include "../Widgets/ModalYesNo.h"

namespace PWT::CUI {
    ConsoleTab::ConsoleTab() {
        QVBoxLayout *lyt = new QVBoxLayout();

        scrollArea = new QScrollArea();
        scrollWidgLyt = new QVBoxLayout();
        consoleSettings = ConsoleSettings::getInstance();

        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);
        scrollWidgLyt->addStretch();
        scrollArea->widget()->setLayout(scrollWidgLyt);
        scrollArea->viewport()->installEventFilter(this);
        QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

        lyt->addWidget(scrollArea);
        setLayout(lyt);
    }

    void ConsoleTab::setWidgetFocus() const {
        ConsoleBaseWidget *cw = tabWidgetList[focusIdx];

        cw->setFocus(Qt::TabFocusReason);

        if (focusIdx <= 1)
            scrollToTop();
        else if (!isWidgetFullyVisible(cw))
            scrollArea->ensureWidgetVisible(cw);
    }

    void ConsoleTab::unfocusSelectedWidget() const {
        if (focusIdx < 0 || focusIdx >= tabWidgetList.size())
            return;

        tabWidgetList[focusIdx]->clearFocus();
    }

    void ConsoleTab::setupConsoleWidgetEvents(const ConsoleBaseWidget *bw) const {
        const ConsoleWidget *cw = qobject_cast<const ConsoleWidget *>(bw);

        // base events
        QObject::disconnect(bw, &ConsoleBaseWidget::sayTTS, this, &ConsoleTab::onWidgetSayTTS);
        QObject::disconnect(bw, &ConsoleBaseWidget::sayStoppableTTS, this, &ConsoleTab::onWidgetSayStoppableTTS);
        QObject::connect(bw, &ConsoleBaseWidget::sayTTS, this, &ConsoleTab::onWidgetSayTTS);
        QObject::connect(bw, &ConsoleBaseWidget::sayStoppableTTS, this, &ConsoleTab::onWidgetSayStoppableTTS);

        if (cw == nullptr) // not a console widget
            return;

        // console widget events
        QObject::disconnect(cw, &ConsoleWidget::focusChangeExternalIn, this, &ConsoleTab::onWidgetFocusExternalIn);
        QObject::disconnect(cw, &ConsoleWidget::openMicroKeyboard, this, &ConsoleTab::onWidgetOpenMicroKeyboard);
        QObject::connect(cw, &ConsoleWidget::focusChangeExternalIn, this, &ConsoleTab::onWidgetFocusExternalIn);
        QObject::connect(cw, &ConsoleWidget::openMicroKeyboard, this, &ConsoleTab::onWidgetOpenMicroKeyboard);
    }

    void ConsoleTab::collectConsoleWidgets(const QLayout *lyt) const {
        if (lyt == nullptr)
            return;

        for (int i=0,l=lyt->count(); i<l; ++i) {
            QLayoutItem *itm = lyt->itemAt(i);
            QWidget *w = itm->widget();

            if (itm->isEmpty() && w != nullptr && w->layout() != nullptr && w->layout()->isEmpty())
                continue;

            if (w == nullptr) {
                collectConsoleWidgets(itm->layout());

            } else {
                collectConsoleWidgets(w->layout());

                ConsoleBaseWidget *cw = qobject_cast<ConsoleBaseWidget *>(w);

                if (cw != nullptr) {
                    tabWidgetList.append(cw);
                    setupConsoleWidgetEvents(cw);
                }
            }
        }
    }

    bool ConsoleTab::isWidgetFullyVisible(const QWidget *widget) const {
        return !widget->visibleRegion().isEmpty() && widget->visibleRegion().boundingRect().height() == widget->height();
    }

    bool ConsoleTab::isSkipWidgetFocus(const ConsoleBaseWidget *bw) const {
        return !consoleSettings->getEnableTTS() && qobject_cast<const ConsoleWidget *>(bw) == nullptr;
    }

    void ConsoleTab::scrollToTop() const {
        scrollArea->verticalScrollBar()->setValue(0);
    }

    void ConsoleTab::deleteWidget(QWidget *widget) {
        scrollWidgLyt->removeWidget(widget);
        --insertIdx;
        widget->deleteLater();
    }

    void ConsoleTab::setupFocusableWidgets() const {
        tabWidgetList.clear();
        collectConsoleWidgets(scrollArea->widget()->layout());
    }

    void ConsoleTab::removeWidgetChildrenFromFocusList(const QWidget *widget) const {
        tabWidgetList.removeIf([widget](const QWidget *cw)->bool {
            return cw->parentWidget() == widget;
        });
    }

    bool ConsoleTab::askUserYesNo(const QString &title, const QString &msg) {
        activeModal.reset(new ModalYesNo(title, msg, this));

        QObject::connect(activeModal.get(), &ModalYesNo::sayTTS, this, &ConsoleTab::onWidgetSayTTS);
        QObject::connect(activeModal.get(), &ModalYesNo::sayStoppableTTS, this, &ConsoleTab::onWidgetSayStoppableTTS);

        const bool res = activeModal->exec() == QDialog::DialogCode::Accepted;

        activeModal.reset();
        return res;
    }

    bool ConsoleTab::hasLostFocus() const {
        for (const ConsoleBaseWidget *cw: tabWidgetList) {
            if (isSkipWidgetFocus(cw))
                continue;

            if (cw->hasFocus())
                return false;
        }

        return true;
    }

    void ConsoleTab::focusFirstValidWidget() const {
        focusIdx = 0;

        // try first enabled and visibile
        for (const ConsoleBaseWidget *cw: tabWidgetList) {
            if (isSkipWidgetFocus(cw) || !cw->isEnabled() || !isWidgetFullyVisible(cw)) {
                ++focusIdx;
                continue;
            }

            setWidgetFocus();
            return;
        }

        focusIdx = 0;

        // try first enabled
        for (const ConsoleBaseWidget *cw: tabWidgetList) {
            if (isSkipWidgetFocus(cw) || !cw->isEnabled()) {
                ++focusIdx;
                continue;
            }

            setWidgetFocus();
            return;
        }

        // no suitable widget found, reset idx
        focusIdx = -1;
    }

    void ConsoleTab::focusWidget(const bool next) const {
        for (int i=(focusIdx + (next ? 1:-1)),l=tabWidgetList.size(); i>=0 && i<l; i += (next ? 1:-1)) {
            const ConsoleBaseWidget *cw = tabWidgetList[i];

            if (isSkipWidgetFocus(cw) || !cw->isEnabled() || !cw->isVisible())
                continue;

            focusIdx = i;
            setWidgetFocus();
            break;
        }
    }

    void ConsoleTab::scrollViewUp() const {
        scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() - 30);
        unfocusSelectedWidget();
    }

    void ConsoleTab::scrollViewDown() const {
        scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() + 30);
        unfocusSelectedWidget();
    }

    void ConsoleTab::scrollViewTop() const {
        scrollArea->verticalScrollBar()->setValue(0);
        unfocusSelectedWidget();
    }

    void ConsoleTab::scrollViewBottom() const {
        scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
        unfocusSelectedWidget();
    }

    void ConsoleTab::sendGamepadButtonEvent(const GamepadButton button, const GamepadMap &mapping) const {
        if (focusIdx >= tabWidgetList.size() || focusIdx < 0)
            return;

        ConsoleWidget *cw = qobject_cast<ConsoleWidget *>(tabWidgetList[focusIdx]);

        if (cw != nullptr)
            cw->processGamepadButtonEvent(button, mapping);
    }

    void ConsoleTab::sendGamepadButtonEventForModal(const GamepadButton button, const GamepadMap &mapping) const {
        if (activeModal.isNull())
            return;

        activeModal->processGamepadButtonEvent(button, mapping);
    }

    void ConsoleTab::sendGamepadButtonEventForMicroKeyboard(const GamepadButton button, const GamepadMap &mapping) const {
        if (floatMicroKb.isNull())
            return;

        floatMicroKb->processGamepadButtonEvent(button, mapping);
    }

    bool ConsoleTab::eventFilter(QObject *watched, QEvent *event) {
        switch (event->type()) {
            case QEvent::Scroll: {
                QScrollEvent *sev = dynamic_cast<QScrollEvent *>(event);

                if (sev->scrollState() == QScrollEvent::ScrollStarted)
                    unfocusSelectedWidget();
            }
                break;
            default:
                break;
        }

        return QObject::eventFilter(watched, event);
    }

    void ConsoleTab::onWidgetFocusExternalIn(const ConsoleWidget *widget) const {
        focusIdx = 0;

        for (const ConsoleBaseWidget *cw: tabWidgetList) {
            if (cw != widget)
                ++focusIdx;
            else
                break;
        }
    }

    void ConsoleTab::onWidgetOpenMicroKeyboard() {
        floatMicroKb.reset(new FloatMicroKeyboard);

        QObject::connect(floatMicroKb.get(), &FloatMicroKeyboard::writeKey, this, &ConsoleTab::onMicroKeyboardWriteKey);
        QObject::connect(floatMicroKb.get(), &FloatMicroKeyboard::closeKeyboard, this, &ConsoleTab::onMicroKeyboardClose);
        QObject::connect(floatMicroKb.get(), &FloatMicroKeyboard::sayTTS, this, &ConsoleTab::onWidgetSayTTS);
        QObject::connect(floatMicroKb.get(), &FloatMicroKeyboard::sayStoppableTTS, this, &ConsoleTab::onWidgetSayStoppableTTS);

        floatMicroKb->setParent(this);
        floatMicroKb->move(this->width() - floatMicroKb->width(), this->height() - floatMicroKb->height());
        floatMicroKb->show();
        floatMicroKb->raise();
    }

    void ConsoleTab::onMicroKeyboardWriteKey(const char c) const {
        ConsoleWidget *cw = qobject_cast<ConsoleWidget *>(tabWidgetList[focusIdx]);

        if (cw != nullptr)
            cw->writeMicroKeyboardKey(c);
    }

    void ConsoleTab::onMicroKeyboardClose() {
        floatMicroKb.reset();
        tabWidgetList[focusIdx]->setFocus(Qt::TabFocusReason);
    }

    void ConsoleTab::onWidgetSayTTS(const QString &msg, const TTSID id) {
        emit sayTTS(msg, id);
    }

    void ConsoleTab::onWidgetSayStoppableTTS(const QString &msg) {
        emit sayStoppableTTS(msg);
    }
}
