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
#include <QApplication>
#include <QCommandLineParser>

#include "mainwindow.h"
#include "ConsoleStyle.h"

int main(int argc, char *argv[]) {
    PWT::CUI::ConsoleStyle *appStyle = new PWT::CUI::ConsoleStyle();
    const QCommandLineOption startupOp {"s", "start minimized"};
    QCommandLineParser parser;

    QApplication::setStyle(appStyle);
    QApplication::setPalette(appStyle->getPalette());
    QApplication::setQuitOnLastWindowClosed(false);

    QApplication a(argc, argv);
    MainWindow w;

    parser.addOption(startupOp);
    a.installEventFilter(&w);
    parser.process(a);
    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);

    if (parser.isSet("s")) {
        QCloseEvent evt {};

        w.closeEvent(&evt);
        w.onAppStateChanged(Qt::ApplicationHidden);

    } else {
        w.show();
    }

    QObject::connect(&a, &QApplication::applicationStateChanged, &w, &MainWindow::onAppStateChanged);

    return a.exec();
}
