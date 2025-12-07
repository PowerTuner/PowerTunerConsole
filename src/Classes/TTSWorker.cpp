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
#include "TTSWorker.h"

namespace PWT::CUI {
    void TTSWorker::startTTS() {
        tts.reset(new QTextToSpeech);
        sayTimer.reset(new QTimer);
        sayTimer->setSingleShot(true);
        sayTimer->setInterval(800);

#ifdef _WIN32
        tts->setEngine("sapi"); // winrt is broken on all my devices, qt bug?
#endif

        QObject::connect(tts.get(), &QTextToSpeech::stateChanged, this, &TTSWorker::onStateChanged);
        QObject::connect(sayTimer.get(), &QTimer::timeout, this, &TTSWorker::onSayTimerTimeOut);
    }

    void TTSWorker::pauseStoppableTTS(const bool pause) {
        paused = pause;
    }

    void TTSWorker::enqueueMessage(const QString &msg, const TTSID id) {
        if (paused && id == TTSID::STOPPABLE) [[unlikely]]
            return;

        const TTSMsg tmsg = {
            .id = id,
            .msg = msg
        };

        msgQue.removeOne(tmsg); // only keep most recent message for an id
        msgQue.enqueue(tmsg);
    }

    void TTSWorker::sayQueuedMessage() {
        if (msgQue.isEmpty())
            return;

        const TTSMsg tmsg = msgQue.dequeue();

        playingStoppable = tmsg.id == TTSID::STOPPABLE;
        tts->say(tmsg.msg);
    }

    void TTSWorker::say(const QString &msg, const TTSID id) {
        if (tts->state() == QTextToSpeech::Error) [[unlikely]]
            return;

        sayTimer->stop();
        enqueueMessage(msg, id);
        sayTimer->start();
    }

    void TTSWorker::sayStoppable(const QString &msg) {
        if (tts->state() == QTextToSpeech::Error) [[unlikely]]
            return;

        sayTimer->stop();
        enqueueMessage(msg, TTSID::STOPPABLE);
        sayTimer->start();
    }

    void TTSWorker::onStateChanged(const QTextToSpeech::State state) {
        if (state != QTextToSpeech::Ready)
            return;

        sayQueuedMessage();
    }

    void TTSWorker::onError([[maybe_unused]] const QTextToSpeech::ErrorReason reason, const QString &error) {
        emit logMessageSent(error);
    }

    void TTSWorker::onSayTimerTimeOut() {
        if (msgQue.isEmpty())
            return;

        if (tts->state() != QTextToSpeech::Ready) {
            if (!playingStoppable)
                sayTimer->start();
            else
                tts->stop(); // this will trigger state change

        } else {
            sayQueuedMessage();
        }
    }
}
