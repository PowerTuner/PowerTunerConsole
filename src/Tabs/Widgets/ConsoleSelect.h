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

#include <QLabel>

#include "ConsoleWidget.h"

namespace PWT::CUI {
    class ConsoleSelect final: public ConsoleWidget {
        Q_OBJECT

    private:
        struct OptionItem final {
            QString text;
            QVariant data;
        };

        QList<OptionItem> options;
        QLabel *title = nullptr;
        QLabel *valueLbl = nullptr;
        QLabel *helpLbl = nullptr;
        int idx = 0;

        void setValueLbl(const QString &text) const { valueLbl->setText(text); }

    public:
        explicit ConsoleSelect(const QString &label, const QString &helpTx = "");

        [[nodiscard]] QString getCurrentText() const { return options.isEmpty() ? "" : options[idx].text; }
        [[nodiscard]] QVariant getCurrentData() const { return options.isEmpty() ? QVariant() : options[idx].data; }
        [[nodiscard]] int getCurrentIndex() const { return idx; }
        [[nodiscard]] QString getText(const int optIdx) const { return optIdx >= 0 && optIdx < options.size() ? options[optIdx].text : ""; }
        [[nodiscard]] QVariant getData(const int optIdx) const { return optIdx >= 0 && optIdx < options.size() ? options[optIdx].data : QVariant(); }
        [[nodiscard]] bool isEmpty() const { return options.isEmpty(); }
        [[nodiscard]] int count() const { return options.size(); }

        [[nodiscard]] QString getFocusedTTS() const override;
        [[nodiscard]] int indexOf(const QString &value) const;
        [[nodiscard]] int indexOf(const QVariant &data) const;
        void setCurrentIndex(int index);
        void setOptionText(int index, const QString &text);
        void addOption(const QString &option, const QVariant &data = {});
        void addOptions(const QList<QString> &list);
        void removeAt(int index);
        void clear();
        void leftAction() override;
        void rightAction() override;

    signals:
        void selectionChanged(int idx);
        void textChanged(const QString &text);
    };
}
