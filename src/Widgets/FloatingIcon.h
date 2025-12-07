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

#include <QWidget>

#include "../Include/FloatIconSize.h"

namespace PWT::CUI {
    class FloatingIcon final: public QWidget {
        Q_OBJECT

    private:
        QPointF oldMousePos;
        QPointF pressMousePos;

    protected:
        void mousePressEvent(QMouseEvent *e) override;
        void mouseReleaseEvent(QMouseEvent *e) override;
        void mouseMoveEvent(QMouseEvent *e) override;

    public:
        FloatingIcon(const QPointF &pos, FloatIconSize size);

        void showIcon();
        void setSize(FloatIconSize size);

    signals:
        void showMainWindow();
        void positionChanged(const QPointF &pos);
        void quitApplication();
    };
}
