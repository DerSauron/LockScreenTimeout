/*
 * Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#include "XCBDpms.h"

#include "Logging.h"
#include <QX11Info>
#include <xcb/dpms.h>
#include <memory>

namespace {

template<typename T>
auto makeAutoCPointer(T* ptr)
{
    return std::unique_ptr<T, decltype(std::free)*>{ptr, std::free};
}

auto dpmsInfo()
{
    auto* c = QX11Info::connection();
    return makeAutoCPointer<xcb_dpms_info_reply_t>(xcb_dpms_info_reply(c, xcb_dpms_info(c), nullptr));
}

auto dpmsCapable()
{
    auto* c = QX11Info::connection();
    return makeAutoCPointer<xcb_dpms_capable_reply_t>(xcb_dpms_capable_reply(c, xcb_dpms_capable(c), nullptr));
}

auto dpmsTimeouts()
{
    auto* c = QX11Info::connection();
    return makeAutoCPointer<xcb_dpms_get_timeouts_reply_t>(xcb_dpms_get_timeouts_reply(c, xcb_dpms_get_timeouts(c), nullptr));
}

} // namespace

XCBDpms::XCBDpms()
{
}

XCBDpms::~XCBDpms()
{
}

bool XCBDpms::available() const
{
    xcb_connection_t* connection = QX11Info::connection();

    xcb_prefetch_extension_data(connection, &xcb_dpms_id);
    auto* extension = xcb_get_extension_data(connection, &xcb_dpms_id);
    if (!extension || !extension->present)
    {
        LOG(ERROR) << "DPMS extension not available";
        return false;
    }

    auto capable = dpmsCapable();
    return capable && capable->capable;
}

Dpms::Timeouts XCBDpms::timeouts() const
{
    auto timeouts = dpmsTimeouts();
    if (!timeouts)
        return 0;
    return {timeouts->standby_timeout, timeouts->suspend_timeout, timeouts->off_timeout};
}

void XCBDpms::setTimeouts(const Timeouts& timeouts)
{
    auto* c = QX11Info::connection();
    xcb_dpms_set_timeouts_checked(c, timeouts.standby, timeouts.suspend, timeouts.off);
}

bool XCBDpms::isEnabled() const
{
    auto info = dpmsInfo();
    return info && info->state;
}

void XCBDpms::setEnabled(bool enabled)
{
    if (enabled)
        xcb_dpms_enable_checked(QX11Info::connection());
    else
        xcb_dpms_disable_checked(QX11Info::connection());
}

Dpms::State XCBDpms::state() const
{
    auto info = dpmsInfo();

    if (info && info->state)
    {
        switch (info->power_level)
        {
            case XCB_DPMS_DPMS_MODE_ON:
                return State::On;
            case XCB_DPMS_DPMS_MODE_STANDBY:
                return State::Standby;
            case XCB_DPMS_DPMS_MODE_SUSPEND:
                return State::Suspend;
            case XCB_DPMS_DPMS_MODE_OFF:
                return State::Off;
        }
    }
    return State::Off;
}

void XCBDpms::forceState(Dpms::State state) const
{
    uint16_t level = 0;
    switch (state)
    {
        case State::On:
            level = XCB_DPMS_DPMS_MODE_ON;
            break;
        case State::Standby:
            level = XCB_DPMS_DPMS_MODE_STANDBY;
            break;
        case State::Suspend:
            level = XCB_DPMS_DPMS_MODE_SUSPEND;
            break;
        case State::Off:
            level = XCB_DPMS_DPMS_MODE_OFF;
            break;
    }

    xcb_dpms_force_level(QX11Info::connection(), level);
}
