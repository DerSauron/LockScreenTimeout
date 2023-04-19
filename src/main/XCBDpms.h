/*
 * Copyright (c) 2021-2023 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "Dpms.h"
#include <QtGlobal>

class XCBDpms : public Dpms
{
public:
    XCBDpms();
    ~XCBDpms() override;

    bool available() const override;

    uint16_t timeout() const override;
    void setTimeout(uint16_t timeout) override;

    bool isEnabled() const override;
    void setEnabled(bool enabled) override;

    Dpms::State state() const override;
    void forceState(Dpms::State state) const override;

private:
    Q_DISABLE_COPY_MOVE(XCBDpms)
};
