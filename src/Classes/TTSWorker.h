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

#include <QTextToSpeech>
#include <QTimer>
#include <QQueue>

#include "../Include/TTSID.h"

namespace PWT::CUI {
    class TTSWorker final: public QObject {
        Q_OBJECT

    private:
        struct TTSMsg final {
            TTSID id;
            QString msg;

            friend bool operator==(const TTSMsg &a, const TTSMsg &b) { return a.id == b.id; }
        };

        QScopedPointer<QTextToSpeech> tts;
        QScopedPointer<QTimer> sayTimer;
        QQueue<TTSMsg> msgQue;
        bool playingStoppable = false;
        bool paused = false;

        void enqueueMessage(const QString &msg, TTSID id);
        void sayQueuedMessage();

    private slots:
        void onStateChanged(QTextToSpeech::State state);
        void onError(QTextToSpeech::ErrorReason reason, const QString &error);
        void onSayTimerTimeOut();

    public slots:
        void startTTS();
        void pauseStoppableTTS(bool pause);
        void say(const QString &msg, TTSID id);
        void sayStoppable(const QString &msg);

    signals:
        void logMessageSent(const QString &msg);
    };
}
