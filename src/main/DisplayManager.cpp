/*
 * Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#include "DisplayManager.h"

#include "Dpms.h"
#include <QTimer>

namespace {

constexpr int WATCHDOG_INTERVAL = 10000;
constexpr int OFF_TIMEOUT = 10000;

} // namespace

DisplayManager::DisplayManager(Dpms* dpms, QObject* parent) :
    QObject(parent),
    dpms_(dpms),
    watchdog_(new QTimer(this)),
    dpmsWasEnabled_{false}
{
    watchdog_->setSingleShot(false);
    watchdog_->setInterval(WATCHDOG_INTERVAL);
    connect(watchdog_, &QTimer::timeout, this, &DisplayManager::onWatchdogTimeout);
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::onSessionStateChanged(Session::State state)
{
    switch (state)
    {
        case Session::State::Locked:
            dpmsWasEnabled_ = dpms_->isEnabled();
            switchOff(false);
            watchdog_->start();
            break;

        case Session::State::Unlocked:
            watchdog_->stop();
            switchOn();
            break;
    }
}

void DisplayManager::onWatchdogTimeout()
{
    if (dpms_->state() != Dpms::State::Off)
    {
        switchOff(true);
    }
}

void DisplayManager::switchOff(bool immediate)
{
    if (!dpms_->isEnabled())
        dpms_->setEnabled(true);
    dpms_->setTimeout(OFF_TIMEOUT);
    if (immediate)
        dpms_->forceState(Dpms::State::Off);
}

void DisplayManager::switchOn()
{
    dpms_->setTimeout(0);
    if (dpmsWasEnabled_)
        dpms_->setEnabled(false);
}
