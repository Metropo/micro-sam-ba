/*
 * Copyright (c) 2015-2016, Atmel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#ifndef PIO_H_
#define PIO_H_

#include <stdint.h>

#include "common_types.h"

typedef enum e_PIO {
    PIOA,
    PIOB,
    PIOC,
    PIOD,
    PIOE,
    INVALID_PIO
} PIO;

struct _pio_regs {
    const uint32_t peripheral_instance_id; // Instance ID of Peripheral Identifier
    const uint32_t pio_pudr_reg;	       // PIO Pull-Up Disable Register
    const uint32_t pio_puer_reg;	       // PIO Pull-Up Enable Register
    const uint32_t pio_pusr_reg;	       // PIO Pull-Up Status Register
    const uint32_t pio_ppddr_reg;	       // PIO Pad Pull-Down Disable Register
    const uint32_t pio_ppder_reg;	       // PIO Pad Pull-Down Enable Register
    const uint32_t pio_ppdsr_reg;	       // PIO Pad Pull-Down Status Register
    const uint32_t pio_pdsr_reg;	       // PIO Pin Data Status Register
};

// fix incompatible-pointer-types
struct _chip_serie;

bool reading_pio_pin_data(serial_port_handle_t fd, const struct _chip_serie* chip_serie, char* pio_port, uint32_t bit_pattern_pull_up, uint32_t bit_pattern_pull_down, uint32_t bit_pattern_floating);

#endif /* PIO_H_ */
