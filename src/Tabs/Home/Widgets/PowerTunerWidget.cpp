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

#include "PowerTunerWidget.h"
#include "../../../version.h"
#include "pwtClientService/ClientService.h"
#include "pwtShared/Utils.h"
#include "pwtClientCommon/CommonUtils.h"

namespace PWT::CUI {
    PowerTunerWidget::PowerTunerWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();

        daemonVer = new ConsoleLabel("Daemon version", "0");
        daemonPwtsVer = new ConsoleLabel("Daemon libPWTShared version", "0");

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(new ConsoleLabel("Console client version", QString("%1.%2").arg(CLIENT_VER_MAJOR).arg(CLIENT_VER_MINOR)));
        lyt->addWidget(new ConsoleLabel("libClientCommon version", QString("%1.%2").arg(getCommonMajorVersion()).arg(getCommonMinorVersion())));
        lyt->addWidget(new ConsoleLabel("libPWTService version", QString("%1.%2").arg(PWTCS::ClientService::getMajorVersion()).arg(PWTCS::ClientService::getMinorVersion())));
        lyt->addWidget(new ConsoleLabel("libPWTShared version", QString("%1.%2").arg(PWTS::getLibMajorVersion()).arg(PWTS::getLibMinorVersion())));
        lyt->addWidget(daemonVer);
        lyt->addWidget(daemonPwtsVer);

        setLayout(lyt);
    }

    void PowerTunerWidget::setDaemonVersion(const int major, const int minor) const {
        daemonVer->setText(QString("%1.%2").arg(major).arg(minor));
    }

    void PowerTunerWidget::setDaemonPwtsVersion(const int major, const int minor) const {
        daemonPwtsVer->setText(QString("%1.%2").arg(major).arg(minor));
    }

    void PowerTunerWidget::clearWidget() const {
        daemonVer->setNum(0);
        daemonPwtsVer->setNum(0);
    }
}
