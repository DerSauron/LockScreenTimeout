/*
 * Copyright (c) 2021-2023 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "Dpms.h"
#include "SessionState.h"
#include <QObject>

class QTimer;

class DisplayManager : public QObject
{
    Q_OBJECT

public:
    DisplayManager(Dpms* dpms, QObject* parent = nullptr);
    ~DisplayManager() override;

public slots:
    void onSessionStateChanged(Session::State state);

private slots:
    void onWatchdogTimeout();

private:
    void saveDpmsState();
    void restoreDpmsState();

    void adjustDpmsForLockedState(bool immediateSwitchOff);

private:
    struct DpmsState
    {
        bool enabled{};
        Dpms::Timeouts timeouts{};
    };

    Dpms* dpms_;
    QTimer* watchdog_;
    DpmsState dpmsState_;

    Q_DISABLE_COPY_MOVE(DisplayManager)
};
