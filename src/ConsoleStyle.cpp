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
#include <QPainter>
#include <QStyleOption>
#include <QScreen>

#include "ConsoleStyle.h"

namespace PWT::CUI {
    QPalette ConsoleStyle::getPalette() const {
        const QColor mainTextColor = QColor(250,250,250);

        return {
            mainTextColor,
            QColor(39, 42, 50),
            QColor(39, 40, 58),
            QColor(17, 17, 24),
            Qt::cyan,
            mainTextColor,
            Qt::green,
            QColor(23, 25, 29),
            QColor(15, 16, 20)
        };
    }

    constexpr qreal ConsoleStyle::getDefaultDPI() const {
#ifdef Q_OS_DARWIN
        return 72;
#else
        return 96;
#endif
    }

    qreal ConsoleStyle::getDPI(const QStyleOption *option) const {
#ifndef Q_OS_DARWIN
            // Prioritize the application override, except for on macOS where
            // we have historically not supported the AA_Use96Dpi flag.
            if (QCoreApplication::testAttribute(Qt::AA_Use96Dpi))
                return 96;
#endif
            // Expect that QStyleOption::QFontMetrics::QFont has the correct DPI set
            if (option)
                return option->fontMetrics.fontDpi();

            // Fall back to historical Qt behavior: hardocded 72 DPI on mac,
            // primary screen DPI on other platforms.
#ifdef Q_OS_DARWIN
            return 72;
#else
        if (const QScreen *screen = QGuiApplication::primaryScreen())
            return qRound(screen->logicalDotsPerInchX());

        // PI has not been initialised, or it is being initialised. Give a default dpi
        return 100;
#endif
    }

    qreal ConsoleStyle::dpiScaled(const qreal value, const QStyleOption *option) const {
        return value * getDPI(option) / getDefaultDPI();
    }

    int ConsoleStyle::pixelMetric(const QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *w) const {
        switch (m) {
            case QStyle::PM_SliderControlThickness:
                return static_cast<int>(dpiScaled(25, opt));
            case QStyle::PM_SliderTickmarkOffset:
                return static_cast<int>(dpiScaled(opt->rect.height() / 2, opt));
            case QStyle::PM_SliderLength:
                return static_cast<int>(dpiScaled(28, opt));
            case PM_IndicatorWidth:
            case PM_IndicatorHeight: {
                if (std::strncmp(w->metaObject()->className(), "QCheckBox", classCheckboxLen) == 0)
                    return static_cast<int>(dpiScaled(26, opt));

                return SuperStyle::pixelMetric(m, opt, w);
            }
            case PM_CheckBoxLabelSpacing:
                return static_cast<int>(dpiScaled(10, opt));
            default:
                return SuperStyle::pixelMetric(m, opt, w);
        }
    }

    void ConsoleStyle::drawPrimitive(const QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const {
        if (w == nullptr) {
            SuperStyle::drawPrimitive(pe, opt, p, w);
            return;
        }

        switch (pe) {
            case QStyle::PE_FrameLineEdit:
                break;
            case QStyle::PE_Frame:
            case QStyle::PE_FrameTabWidget: {
                const char *className = w->metaObject()->className();

                if (std::strncmp(className, "QTabWidget", classTabWidgetLen) == 0 ||
                    std::strncmp(className, "QScrollArea", classScrollAreaLen) == 0)
                    break;

                SuperStyle::drawPrimitive(pe, opt, p, w);
            }
                break;
            case QStyle::PE_FrameFocusRect: {
                const char *className = w->metaObject()->className();

                if (std::strncmp(className, "QTabBar", classTabBarLen) == 0) {
                    p->fillRect(w->rect().x(), w->rect().height() - 2, w->rect().width(), 2, tabFocusColor);
                    break;

                } else if (std::strncmp(className, "QPushButton", classPushButtonLen) == 0) {
                    break;
                }

                SuperStyle::drawPrimitive(pe, opt, p, w);
            }
                break;
            case QStyle::PE_IndicatorCheckBox: {
                const QBrush oldBrush = p->brush();

                if (opt->state & State_On)
                    p->setBrush(opt->palette.color(QPalette::Accent));
                else
                    p->setBrush(opt->palette.color(QPalette::Window));

                p->drawRect(opt->rect);
                p->setBrush(oldBrush);
            }
                break;
            default:
                SuperStyle::drawPrimitive(pe, opt, p, w);
                break;
        }
    }

    void ConsoleStyle::drawControl(const QStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w) const {
        switch (ce) {
            case CE_TabBarTabShape: {
                const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(opt);

                if (tab == nullptr)
                    break;

                p->fillRect(opt->rect.adjusted(0, 0, 0, 0), tab->state & State_Selected ? tabSelectedColor : tabUnselectedColor);
            }
                break;
            case CE_CheckBox: {
                const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(opt);

                if (btn == nullptr)
                    break;

                QStyleOptionButton subOpt = *btn;

                subOpt.rect = subElementRect(SE_CheckBoxIndicator, btn, w);
                drawPrimitive(PE_IndicatorCheckBox,&subOpt, p, w);
                subOpt.rect = subElementRect(SE_CheckBoxContents, btn, w);
                proxy()->drawControl(CE_CheckBoxLabel, &subOpt, p, w);
            }
                break;
            default:
                SuperStyle::drawControl(ce, opt, p, w);
                break;
        }
    }

    void ConsoleStyle::drawComplexControl(const QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *w) const {
        switch (cc) {
            case CC_Slider: {
                const QRect groove = subControlRect(CC_Slider, opt, SC_SliderGroove, w);
                const QRect handle = subControlRect(CC_Slider, opt, SC_SliderHandle, w);

                if ((opt->subControls & SC_SliderGroove) && groove.isValid()) {
                    const QBrush oldBrush = p->brush();
                    const QPen oldPen = p->pen();

                    p->setPen(opt->palette.color(QPalette::Window));
                    p->setBrush(opt->palette.color(QPalette::Window));
                    p->drawRect(0, (w->height()/2) - (groove.height()/2), groove.width(), groove.height());
                    p->setPen(oldPen);
                    p->setBrush(oldBrush);
                }

                if ((opt->subControls & SC_SliderHandle) ) {
                    const QBrush oldBrush = p->brush();
                    const QPen oldPen = p->pen();

                    p->setPen(opt->palette.color(QPalette::Button));
                    p->setBrush(opt->palette.color(QPalette::Button));
                    p->drawRect(handle);
                    p->setPen(oldPen);
                    p->setBrush(oldBrush);
                }
            }
                break;
            default:
                SuperStyle::drawComplexControl(cc, opt, p, w);
                break;
        }
    }

    QRect ConsoleStyle::subElementRect(const QStyle::SubElement sr, const QStyleOption *opt, const QWidget *w) const {
        switch (sr) {
            case QStyle::SE_SliderFocusRect:
                return {};
            case SE_CheckBoxContents: {
                // Deal with the logical first, then convert it back to screen coords.
                const QRect ir = visualRect(opt->direction, opt->rect, subElementRect(SE_CheckBoxIndicator, opt, w));
                const int spacing = pixelMetric(PM_CheckBoxLabelSpacing, opt, w);
                const QRect rect {ir.right() + spacing, opt->rect.y(), opt->rect.width() - ir.width() - spacing, opt->rect.height()};

                return visualRect(opt->direction, opt->rect, rect);
            }
            default:
                return SuperStyle::subElementRect(sr, opt, w);
        }
    }

    QRect ConsoleStyle::subControlRect(const QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *w) const {
        switch (cc) {
            case CC_Slider: {
                const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(opt);

                if (slider == nullptr)
                    return SuperStyle::subControlRect(cc, opt, sc, w);

                const int thickness = pixelMetric(PM_SliderControlThickness, slider, w);
                const int halfThickness = thickness / 2;
                const int tickOffset = pixelMetric(PM_SliderTickmarkOffset, slider, w) - halfThickness;
                QRect ret;

                switch (sc) {
                    case SC_SliderHandle: {
                        const int len = pixelMetric(PM_SliderLength, slider, w);
                        const int sliderPos = sliderPositionFromValue(slider->minimum,
                                                                        slider->maximum,
                                                                        slider->sliderPosition,
                                                                        slider->rect.width() - len,
                                                                        slider->upsideDown);

                        ret.setRect(slider->rect.x() + sliderPos, slider->rect.y() + tickOffset, len, thickness);
                    }
                        break;
                    case SC_SliderGroove:
                        ret.setRect(slider->rect.x(), slider->rect.y() + tickOffset, slider->rect.width(), halfThickness);
                        break;
                    default:
                        break;
                }

                return visualRect(slider->direction, slider->rect, ret);
            }
            default:
                return SuperStyle::subControlRect(cc, opt, sc, w);
        }
    }
}
