/*
 * Copyright (c) 2021-2023 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "SessionState.h"
#include <QObject>

class Dpms;
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
    void switchOff(bool immediate);
    void switchOn();

private:
    Dpms* dpms_;
    QTimer* watchdog_;
    bool dpmsWasEnabled_;

    Q_DISABLE_COPY_MOVE(DisplayManager)
};
