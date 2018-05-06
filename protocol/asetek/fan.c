/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017,2018  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>
#include "lowlevel/asetek.h"
#include "device.h"
#include "driver.h"
#include "print.h"
#include "protocol/asetek.h"

int corsairlink_asetek_fan_count(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t *fan_count)
{
    int rr = 0;
    // undefined, return device value from device.c
    *(fan_count) = dev->fan_control_count;
    return rr;
}

int corsairlink_asetek_fan_print_mode(uint8_t mode, uint16_t data,
            char *modestr, uint8_t modestr_size)
{

    int rr = 0;
    // undefined, return hex value of mode
    snprintf(modestr,modestr_size, "Mode 0x%02X",mode);
    return rr;
}

int corsairlink_asetek_fan_mode(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t selector, uint8_t *fan_mode, uint16_t *fan_data)
{
    int rr;
    struct fan_table curve;
    if ( *(fan_mode) == PERFORMANCE) {
        ASETEK_FAN_TABLE_EXTREME(curve);
    }
    else if ( *(fan_mode) == QUIET) {
        ASETEK_FAN_TABLE_QUIET(curve);
    }
    rr = dev->driver->fan.custom(dev, handle, selector, &curve);

    return rr;
}

int corsairlink_asetek_fan_curve(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t selector, struct fan_table *fan)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = FanCurve;
    commands[1] = UnknownFanCurve;

    commands[2] = fan->t1;
    commands[3] = fan->t2;
    commands[4] = fan->t3;
    commands[5] = fan->t4;
    commands[6] = fan->t5;
    commands[7] = fan->t6;

    commands[8] = fan->s1;
    commands[9] = fan->s2;
    commands[10] = fan->s3;
    commands[11] = fan->s4;
    commands[12] = fan->s5;
    commands[13] = fan->s6;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 14);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    return rr;
}

int corsairlink_asetek_fan_speed(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint8_t selector, uint16_t *speed, uint16_t *maxspeed)
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x20;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 32);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    msg_debug("%02X %02X\n", response[0], response[1]);
    *(speed) = (response[0]<<8) + response[1];

    return rr;
}
