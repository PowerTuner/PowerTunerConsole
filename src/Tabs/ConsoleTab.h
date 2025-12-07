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

#include <QScrollArea>
#include <QScroller>
#include <QVBoxLayout>

#include "Widgets/ConsoleWidget.h"
#include "../Widgets/Modal.h"
#include "../Widgets/FloatMicroKeyboard.h"

namespace PWT::CUI {
    class ConsoleTab: public QWidget {
        Q_OBJECT

    private:
        QSharedPointer<ConsoleSettings> consoleSettings;
        mutable QList<ConsoleBaseWidget *> tabWidgetList;
        QScopedPointer<Modal> activeModal;
        QScopedPointer<FloatMicroKeyboard> floatMicroKb;
        QScrollArea *scrollArea = nullptr;
        mutable int focusIdx = -1;
        int insertIdx = 0;

        void setWidgetFocus() const;
        void unfocusSelectedWidget() const;
        void setupConsoleWidgetEvents(const ConsoleBaseWidget *bw) const;
        [[nodiscard]] bool isWidgetFullyVisible(const QWidget *widget) const;
        [[nodiscard]] bool isSkipWidgetFocus(const ConsoleBaseWidget *bw) const;

    protected:
        QVBoxLayout *scrollWidgLyt = nullptr;

        [[nodiscard]] int nextInsertIdx() { return insertIdx++; }
        void changeEvent(QEvent *event) override { floatMicroKb.reset(); }
        void focusOutEvent(QFocusEvent *event) override { floatMicroKb.reset(); }
        void hideEvent(QHideEvent *event) override { floatMicroKb.reset(); }
        void resizeEvent(QResizeEvent *event) override { floatMicroKb.reset(); }

        bool eventFilter(QObject *watched, QEvent *event) override;
        void collectConsoleWidgets(const QLayout *lyt) const;
        void scrollToTop() const;
        void deleteWidget(QWidget *widget);
        void setupFocusableWidgets() const;
        void removeWidgetChildrenFromFocusList(const QWidget *widget) const;
        [[nodiscard]] bool askUserYesNo(const QString &title, const QString &msg);

    public:
        ConsoleTab();

        [[nodiscard]] bool hasModal() const { return !activeModal.isNull(); }
        [[nodiscard]] bool hasMicroKeyboard() const { return !floatMicroKb.isNull(); }

        [[nodiscard]] bool hasLostFocus() const;
        void focusFirstValidWidget() const;
        void focusWidget(bool next) const;
        void scrollViewUp() const;
        void scrollViewDown() const;
        void scrollViewTop() const;
        void scrollViewBottom() const;
        void sendGamepadButtonEvent(GamepadButton button, const GamepadMap &mapping) const;
        void sendGamepadButtonEventForModal(GamepadButton button, const GamepadMap &mapping) const;
        void sendGamepadButtonEventForMicroKeyboard(GamepadButton button, const GamepadMap &mapping) const;

    protected slots:
        void onWidgetFocusExternalIn(const PWT::CUI::ConsoleWidget *widget) const;
        void onWidgetOpenMicroKeyboard();
        void onMicroKeyboardWriteKey(char c) const;
        void onMicroKeyboardClose();
        void onWidgetSayTTS(const QString &msg, TTSID id);
        void onWidgetSayStoppableTTS(const QString &msg);

    signals:
        void sayTTS(const QString &msg, TTSID id);
        void sayStoppableTTS(const QString &msg);
    };
}
