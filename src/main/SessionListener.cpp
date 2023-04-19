/*
 * Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#include "SessionListener.h"

#include "Logging.h"
#include <QDBusConnection>
#include <QDBusError>

namespace {

const QString SCRSVR_DBUS_SERVICE = QStringLiteral("org.freedesktop.ScreenSaver");
const QString SCRSVR_DBUS_PATH = QStringLiteral("/ScreenSaver");
const QString SCRSVR_DBUS_INTERFACE = QStringLiteral("org.freedesktop.ScreenSaver");
const QString SCRSVR_DBUS_SIGNAL = QStringLiteral("ActiveChanged");

} // namespace

SessionListener::SessionListener(QObject* parent) :
    QObject(parent)
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.isConnected())
    {
        LOG(ERROR) << "Could not get a connection to the dbus session bus: "
                   << qPrintable(QDBusError::errorString(sessionBus.lastError().type()))
                   << ". No state changes will be recorded";
        return;
    }

    if (!sessionBus.connect(SCRSVR_DBUS_SERVICE, SCRSVR_DBUS_PATH, SCRSVR_DBUS_INTERFACE, SCRSVR_DBUS_SIGNAL, this,
                            SLOT(onScrsvrActiveChanged(bool))))
    {
        LOG(ERROR) << "Could not connect to dbus screen saver service: "
                   << qPrintable(QDBusError::errorString(sessionBus.lastError().type()))
                   << ". No state changes will be recorded";
        return;
    }
}

SessionListener::~SessionListener()
{
}

void SessionListener::onScrsvrActiveChanged(bool active)
{
    Session::State state = active ? Session::State::Locked : Session::State::Unlocked;

    LOG(INFO) << "Session changed state to " << state;

    emit sessionStateChanged(state);
}
