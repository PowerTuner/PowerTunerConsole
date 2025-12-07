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
#include "SchemeSettingsWidget.h"
#include "../Include/MinMaxSliderWidget.h"
#include "../Include/OptionsWidget.h"

namespace PWT::CUI::WIN {
    SchemeSettingsWidget::SchemeSettingsWidget() {
        lyt = new QVBoxLayout();

        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
    }

    void SchemeSettingsWidget::disableSettingWidgets() {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            setting->enabled = false;
            setting->widget->setVisible(false);
            setting->widget->clearData();
        }

        startPageIdx = 0;
        endPageIdx = 0;
        pageItmList.clear();
    }

    void SchemeSettingsWidget::addSetting(const QString &guid, const PWTS::WIN::PowerSchemeSettingData &schemeSetting) {
        if (settingWidgMap.contains(guid)) {
            if (settingWidgMap[guid]->enabled)
                return;

            if (schemeSetting.isRangeDefined)
                qobject_cast<MinMaxSliderWidget *>(settingWidgMap[guid]->widget)->setLimits(schemeSetting);
            else
                qobject_cast<OptionsWidget *>(settingWidgMap[guid]->widget)->setOptions(schemeSetting.options);

            settingWidgMap[guid]->enabled = true;
            return;
        }

        const QSharedPointer<SettingItm> itm = QSharedPointer<SettingItm>::create();

        if (schemeSetting.isRangeDefined)
            itm->widget = new MinMaxSliderWidget(schemeSetting, guid);
        else
            itm->widget = new OptionsWidget(schemeSetting, guid);

        itm->widget->setVisible(false);
        settingWidgMap.insert(guid, itm);
        settingItmList.append(itm);
        lyt->addWidget(itm->widget);
    }

    void SchemeSettingsWidget::addData(const QString &scheme, const QString &settingGuid, const PWTS::WIN::PowerSchemeSetting &setting) const {
        settingWidgMap[settingGuid]->widget->setData(scheme, setting);
    }

    QMap<QString, PWTS::WIN::PowerSchemeSetting> SchemeSettingsWidget::getWidgetsData(const QString &scheme) const {
        QMap<QString, PWTS::WIN::PowerSchemeSetting> data;

        for (const auto &[settingGuid, itm]: settingWidgMap.asKeyValueRange()) {
            if (itm->enabled) [[likely]]
                data.insert(settingGuid, {.groupGuid = itm->widget->getGroupGUID(), .value = itm->widget->getData(scheme)});
        }

        return data;
    }

    bool SchemeSettingsWidget::isSettingVisible(const QString &filter, const PowerSettingWidget *wdg) const {
        return filter.isEmpty() || wdg->getTitle().contains(filter, Qt::CaseInsensitive) || wdg->getGroupTitle().contains(filter, Qt::CaseInsensitive);
    }

    void SchemeSettingsWidget::preparePreviousPage(const QString &optionsFilter) const {
        for (int i=(startPageIdx-1),j=0; i>=0 && j<pageSize; --i,--startPageIdx) {
            const QSharedPointer<SettingItm> itm = settingItmList[i];

            if (!itm->enabled || !isSettingVisible(optionsFilter, itm->widget))
                continue;

            pageItmList.append(itm);
            ++j;
        }
    }

    void SchemeSettingsWidget::prepareNextPage(const QString &optionsFilter) const {
        for (int i=startPageIdx,j=0,l=settingItmList.size(); i<l && j<pageSize; ++i,++endPageIdx) {
            const QSharedPointer<SettingItm> itm = settingItmList[i];

            if (!itm->enabled || !isSettingVisible(optionsFilter, itm->widget))
                continue;

            pageItmList.append(itm);
            ++j;
        }
    }

    void SchemeSettingsWidget::switchScheme(const QString &schemeGuid) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->widget->switchScheme(schemeGuid);
        }
    }

    void SchemeSettingsWidget::hidePage() const {
        for (const QSharedPointer<SettingItm> &itm: pageItmList)
            itm->widget->setVisible(false);

        pageItmList.clear();
    }

    void SchemeSettingsWidget::showPage() const {
        for (const QSharedPointer<SettingItm> &itm: pageItmList)
            itm->widget->setVisible(true);
    }

    void SchemeSettingsWidget::previousPage(const QString &optionsFilter) const {
        if (startPageIdx <= 0)
            return;

        endPageIdx = startPageIdx;

        hidePage();
        preparePreviousPage(optionsFilter);
        showPage();
    }

    void SchemeSettingsWidget::nextPage(const QString &optionsFilter) const {
        if (endPageIdx >= settingItmList.size())
            return;

        startPageIdx = endPageIdx;

        hidePage();
        prepareNextPage(optionsFilter);
        showPage();
    }

    void SchemeSettingsWidget::duplicateSettings(const QString &current, const QString &duplicate) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->widget->duplicateScheme(current, duplicate);
        }
    }

    void SchemeSettingsWidget::deleteScheme(const QString &scheme) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->widget->deleteScheme(scheme);
        }
    }

    void SchemeSettingsWidget::filterVisibleSettings(const QString &filter) const {
        startPageIdx = endPageIdx = 0;

        hidePage();
        prepareNextPage(filter);
        showPage();
    }

    void SchemeSettingsWidget::setModeAll(const int idx) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->widget->setMode(idx);
        }
    }

    void SchemeSettingsWidget::copySchemeValues(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->widget->copyValue(fromScheme, fromMode, toScheme, toMode);
        }
    }
}
