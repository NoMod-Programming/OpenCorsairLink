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
#include "../../lowlevel/asetek.h"
#include "../../device.h"
#include "../../driver.h"
#include "../../print.h"
#include "core.h"

int corsairlink_asetek_pump_mode(struct corsair_device_info *dev, struct libusb_device_handle *handle,
            uint8_t *pump_mode)
{
    int rr;
    uint8_t response[32];
    uint8_t commands[32] ;
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = PumpMode;

    if (*(pump_mode) == PERFORMANCE)
        commands[1] = Asetek_Performance;
    else if (*(pump_mode) == QUIET)
        commands[1] = Asetek_Quiet;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 2);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    return rr;
}

int corsairlink_asetek_pump_speed(struct corsair_device_info *dev,
            struct libusb_device_handle *handle,
            uint16_t *speed, uint16_t *maxspeed)
{
    int rr;
    uint8_t response[32];
    uint8_t commands[32] ;
    memset(response, 0, sizeof(response));
    memset(commands, 0, sizeof(commands));

    commands[0] = 0x20;

    rr = dev->driver->write(handle, dev->write_endpoint, commands, 32);
    rr = dev->driver->read(handle, dev->read_endpoint, response, 32);

    msg_debug("%02X %02X\n", response[8], response[9]);
    *(speed) = (response[8]<<8) + response[9];

    return rr;
}
