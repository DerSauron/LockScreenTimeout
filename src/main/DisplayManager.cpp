/*
 * Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#include "DisplayManager.h"

#include "Logging.h"
#include <QTimer>

namespace {

constexpr int WATCHDOG_INTERVAL = 10000;
constexpr Dpms::Timeouts OFF_TIMEOUTS{10};

} // namespace

DisplayManager::DisplayManager(Dpms* dpms, QObject* parent) :
    QObject(parent),
    dpms_(dpms),
    watchdog_(new QTimer(this)),
    dpmsState_{}
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
            saveDpmsState();
            adjustDpmsForLockedState(false);
            watchdog_->start();
            break;

        case Session::State::Unlocked:
            watchdog_->stop();
            restoreDpmsState();
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

void DisplayManager::saveDpmsState()
{
    dpmsState_.enabled = dpms_->isEnabled();
    dpmsState_.timeouts = dpms_->timeouts();

    LOG(DEBUG) << "Remember current dpms state: enabled:" << dpmsState_.enabled << " standby:" << dpmsState_.timeouts.standby;
}

void DisplayManager::restoreDpmsState()
{
    LOG(DEBUG) << "Restore previouse dpms state";

    dpms_->setTimeouts(dpmsState_.timeouts);
    dpms_->setEnabled(dpmsState_.enabled);
}

void DisplayManager::adjustDpmsForLockedState(bool immediateSwitchOff)
{
    LOG(DEBUG) << "Set short dpms timeouts";

    dpms_->setTimeouts(OFF_TIMEOUTS);
    dpms_->setEnabled(true);

    if (immediateSwitchOff)
        dpms_->forceState(Dpms::State::Off);
}
