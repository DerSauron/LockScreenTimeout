/*
 * Copyright (c) 2021-2023 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "SessionState.h"
#include <QObject>

class XDisplay;

class SessionListener : public QObject
{
    Q_OBJECT

public:
    SessionListener(QObject* parent = nullptr);
    ~SessionListener() override;

signals:
    void sessionStateChanged(Session::State state);

private slots:
    void onScrsvrActiveChanged(bool active);

private:
    Q_DISABLE_COPY_MOVE(SessionListener)
};
