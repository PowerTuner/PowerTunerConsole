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
#include <QHBoxLayout>

#include "ConsoleSelect.h"

namespace PWT::CUI {
    ConsoleSelect::ConsoleSelect(const QString &label, const QString &helpTx) {
        QHBoxLayout *lyt = new QHBoxLayout();

        title = new QLabel(label);
        valueLbl = new QLabel();

        title->setAlignment(Qt::AlignLeft);
        valueLbl->setAlignment(Qt::AlignRight);

        lyt->addWidget(title);
        lyt->addWidget(valueLbl);

        if (!helpTx.isEmpty()) {
            helpLbl = new QLabel(helpTx);

            QVBoxLayout *hlyt = new QVBoxLayout();
            QFont font = helpLbl->font();

            font.setPointSize(10);
            font.setStyle(QFont::StyleItalic);
            font.setWeight(QFont::Normal);
            helpLbl->setFont(font);
            helpLbl->setAlignment(Qt::AlignLeft);
            helpLbl->setWordWrap(true);

            hlyt->addLayout(lyt);
            hlyt->addSpacing(4);
            hlyt->addWidget(helpLbl);
            setLayout(hlyt);

        } else {
            setLayout(lyt);
        }
    }

    int ConsoleSelect::indexOf(const QString &value) const {
        int i = 0;

        for (const OptionItem &opt: options) {
            if (opt.text == value)
                return i;

            ++i;
        }

        return -1;
    }

    int ConsoleSelect::indexOf(const QVariant &data) const {
        int i = 0;

        for (const OptionItem &opt: options) {
            if (opt.data == data)
                return i;

            ++i;
        }

        return -1;
    }

    void ConsoleSelect::setCurrentIndex(const int index) {
        if (options.isEmpty() || idx == index)
            return;

        if (index < 0 || index >= options.size()) {
            idx = -1;
            setValueLbl("");
            emit textChanged("");

        } else {
            idx = index;
            setValueLbl(options[idx].text);
            emit textChanged(options[idx].text);
        }

        emit selectionChanged(idx);
    }

    void ConsoleSelect::setOptionText(const int index, const QString &text) {
        if (options.isEmpty() || index < 0 || index >= options.size())
            return;

        options[index].text = text;
    }

    void ConsoleSelect::addOption(const QString &option, const QVariant &data) {
        options.append({option, data});

        if (options.size() == 1) {
            setValueLbl(options[0].text);
            emit selectionChanged(0);
            emit textChanged(options[0].text);
        }
    }

    void ConsoleSelect::addOptions(const QList<QString> &list) {
        const qsizetype oldSz = options.size();

        for (const QString &opt: list)
            options.append({opt, QVariant()});

        if (oldSz == 0 && options.size() > 0) {
            setValueLbl(options[0].text);
            emit selectionChanged(0);
            emit textChanged(options[0].text);
        }
    }

    void ConsoleSelect::removeAt(const int index) {
        if (index < 0 || index >= options.size())
            return;

        options.removeAt(index);

        if (options.isEmpty()) {
            idx = 0;
            setValueLbl("");
            return;

        } else if (index > idx) {
            return;
        }

        // if removed item was last one
        if (idx == options.size())
            --idx;

        setValueLbl(options[idx].text);
        emit selectionChanged(idx);
        emit textChanged(options[idx].text);
    }

    void ConsoleSelect::clear() {
        idx = 0;

        options.clear();
        setValueLbl("");
    }

    void ConsoleSelect::leftAction() {
        if (options.isEmpty()) {
            if (hasTTSEnabled())
                emit sayStoppableTTS(tr("no options"));

            return;
        }

        --idx;

        if (idx < 0)
            idx = options.size() - 1;

        const QString opt = options[idx].text;

        setValueLbl(opt);
        emit selectionChanged(idx);
        emit textChanged(opt);

        if (hasTTSEnabled())
            emit sayStoppableTTS(opt.isEmpty() ? tr("empty") : opt);
    }

    void ConsoleSelect::rightAction() {
        if (options.isEmpty()) {
            if (hasTTSEnabled())
                emit sayStoppableTTS(tr("no options"));

            return;
        }

        ++idx;

        if (idx >= options.size())
            idx = 0;

        const QString opt = options[idx].text;

        setValueLbl(opt);
        emit selectionChanged(idx);
        emit textChanged(opt);

        if (hasTTSEnabled())
            emit sayStoppableTTS(opt.isEmpty() ? tr("empty") : opt);
    }

    QString ConsoleSelect::getFocusedTTS() const {
        const QString val = valueLbl->text();

        return QString("%1: %2 %3").arg(
            title->text(),
            val.isEmpty() ? tr("empty") : val,
            helpLbl == nullptr ? "" : QString(", help: %1").arg(helpLbl->text())
        );
    }
}
