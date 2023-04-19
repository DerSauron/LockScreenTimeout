/*
 * Copyright (c) 2021-2023 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <cinttypes>
#include <memory>

class Dpms
{
public:
    enum class State
    {
        On,
        Standby,
        Suspend,
        Off,
    };

public:
    static std::unique_ptr<Dpms> create();

    virtual ~Dpms() = default;

    virtual bool available() const = 0;

    virtual uint16_t timeout() const = 0;
    virtual void setTimeout(uint16_t timeout) = 0;

    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;

    virtual State state() const = 0;
    virtual void forceState(State state) const = 0;
};
