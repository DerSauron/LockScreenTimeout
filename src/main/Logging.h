/*
 * Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <QDebug>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LST)

#define _LOGGER_CONFIG nospace().noquote()

#define LOG_DEBUG() qCDebug(LST)._LOGGER_CONFIG
#define LOG_INFO() qCInfo(LST)._LOGGER_CONFIG
#define LOG_WARN() qCWarning(LST)._LOGGER_CONFIG
#define LOG_ERROR() qCCritical(LST)._LOGGER_CONFIG
#define LOG(type) LOG_##type()
