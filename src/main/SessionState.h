/*
 * Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <QMetaEnum>

namespace Session {
Q_NAMESPACE

enum class State
{
    Unlocked,
    Locked,
};
Q_ENUM_NS(State)

} // namespace Session
