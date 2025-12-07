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

#include <QDialog>

#include "../Classes/ConsoleSettings.h"
#include "../Include/TTSID.h"

namespace PWT::CUI {
    class Modal: public QDialog {
        Q_OBJECT

    protected:
        QSharedPointer<ConsoleSettings> settings;

    public:
        Modal(QWidget *parent, const QString &title): QDialog(parent, Qt::Dialog) {
            settings = ConsoleSettings::getInstance();

            setMinimumWidth(250);
            setMinimumHeight(150);
            setWindowTitle(title);
        }

        virtual void processGamepadButtonEvent(GamepadButton button, const GamepadMap &mapping) = 0;

    signals:
        void sayTTS(const QString &msg, TTSID id);
        void sayStoppableTTS(const QString &msg);
    };
}
