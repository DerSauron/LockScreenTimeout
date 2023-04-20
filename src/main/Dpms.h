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


    struct Timeouts
    {
        constexpr Timeouts() :
            standby{0},
            suspend{0},
            off{0}
        {
        }

        constexpr Timeouts(uint16_t ti) :
            standby{ti},
            suspend{ti},
            off{ti}
        {
        }

        constexpr Timeouts(uint16_t st, uint16_t su, uint16_t of) :
            standby{st},
            suspend{su},
            off{of}
        {
        }

        uint16_t standby;
        uint16_t suspend;
        uint16_t off;
    };

public:
    static std::unique_ptr<Dpms> create();

    virtual ~Dpms() = default;

    virtual bool available() const = 0;

    virtual Timeouts timeouts() const = 0;
    virtual void setTimeouts(const Timeouts& timeout) = 0;

    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;

    virtual State state() const = 0;
    virtual void forceState(State state) const = 0;
};
