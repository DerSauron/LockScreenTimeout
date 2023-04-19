/*
 * Copyright (c) 2021-2023 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#include "Dpms.h"

#include "XCBDpms.h"
//#include "WaylandDpms.h"
#include <QGuiApplication>
#include <QX11Info>

std::unique_ptr<Dpms> Dpms::create()
{
    if (QX11Info::isPlatformX11())
    {
        return std::make_unique<XCBDpms>();
    }
    else if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), Qt::CaseInsensitive))
    {
        return {/*new WaylandDpms()*/};
    }
    return {};
}
