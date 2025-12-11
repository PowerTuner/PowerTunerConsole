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

#include <QProxyStyle>

namespace PWT::CUI {
    using SuperStyle = QProxyStyle; // debug helper

    class ConsoleStyle final: public SuperStyle {
        Q_OBJECT

    private:
        static constexpr std::size_t classTabWidgetLen = std::string_view("QTabWidget").length();
        static constexpr std::size_t classScrollAreaLen = std::string_view("QScrollArea").length();
        static constexpr std::size_t classTabBarLen = std::string_view("QTabBar").length();
        static constexpr std::size_t classCheckboxLen = std::string_view("QCheckBox").length();
        static constexpr std::size_t classPushButtonLen = std::string_view("QPushButton").length();
        const QColor tabFocusColor = QColor(15, 16, 20);
        const QColor tabSelectedColor = QColor(68, 79, 106);
        const QColor tabUnselectedColor = QColor(39, 42, 50);

        // from qt6/src/widgets/styles/qstylehelper.cpp
        [[nodiscard]] constexpr qreal getDefaultDPI() const;
        [[nodiscard]] qreal getDPI(const QStyleOption *option) const;
        [[nodiscard]] qreal dpiScaled(qreal value, const QStyleOption *option) const;

    public:
        [[nodiscard]] static QPalette getPalette();
        int pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *w) const override;
        void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const override;
        void drawControl(ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w) const override;
        QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *w) const override;
        void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *w) const override;
        QRect subElementRect(SubElement sr, const QStyleOption *opt, const QWidget *w) const override;
    };
}
