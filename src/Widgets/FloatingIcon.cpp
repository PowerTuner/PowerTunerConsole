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
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QLabel>

#include "FloatingIcon.h"

namespace PWT::CUI {
    FloatingIcon::FloatingIcon(const QPointF &pos, const FloatIconSize size) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *icon = new QLabel();

        icon->setAlignment(Qt::AlignCenter);
        icon->setPixmap(QPixmap(":/ico/pwt"));
        icon->setScaledContents(true);
        setMinimumWidth(25);
        setMinimumHeight(25);
        setSize(size);
        setWindowFlags(windowFlags() |
                        Qt::WindowStaysOnTopHint |
                        Qt::FramelessWindowHint |
                        Qt::Tool |
                        Qt::WindowDoesNotAcceptFocus |
                        Qt::BypassWindowManagerHint);
        setAttribute(Qt::WA_MacAlwaysShowToolWindow);
        move(pos.x(), pos.y());
        hide();

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(icon);

        setLayout(lyt);
    }

    void FloatingIcon::showIcon() {
        show();
        raise();
    }

    void FloatingIcon::setSize(const FloatIconSize size) {
        switch (size) {
            case FloatIconSize::Tiny:
                resize(minimumWidth(), minimumHeight());
                break;
            case FloatIconSize::Small:
                resize(minimumWidth() + 10, minimumHeight() + 10);
                break;
            case FloatIconSize::Medium:
                resize(minimumWidth() + 15, minimumHeight() + 15);
                break;
            case FloatIconSize::Big:
                resize(minimumWidth() + 20, minimumHeight() + 20);
                break;
            case FloatIconSize::Large:
                resize(minimumWidth() + 25, minimumHeight() + 25);
                break;
            default:
                break;
        }
    }

    void FloatingIcon::mousePressEvent(QMouseEvent *e) {
        oldMousePos = e->globalPosition();
        pressMousePos = oldMousePos;
    }

    void FloatingIcon::mouseReleaseEvent(QMouseEvent *e) {
        const QPointF pos = e->globalPosition();

        if (pressMousePos.x() == pos.x() && pressMousePos.y() == pos.y()) {
            if (e->button() == Qt::LeftButton) {
                emit showMainWindow();
                hide();

            } else if (e->button() == Qt::RightButton) {
                emit quitApplication();
            }
        } else {
            emit positionChanged(pos);
        }
    }

    void FloatingIcon::mouseMoveEvent(QMouseEvent *e) {
        if (!e->button() != Qt::LeftButton)
            return;

        const QPointF delta = e->globalPosition() - oldMousePos;

        move(x() + delta.x(), y() + delta.y());
        e->accept();

        oldMousePos = e->globalPosition();
    }
}
