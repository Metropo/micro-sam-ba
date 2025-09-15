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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "pio.h"
#include "comm.h"
#include "chipid.h"

static char* string_upper(char* str)
{
    char* tmp = str;

    while (*tmp) {
        *tmp = toupper((unsigned char)*tmp);
        tmp++;
    }

    return str;
}

bool reading_pio_pin_data(serial_port_handle_t fd, const struct _chip_serie* chip_serie, char* pio_port, uint32_t bit_pattern_pull_up, uint32_t bit_pattern_pull_down, uint32_t bit_pattern_floating)
{
    PIO	pio                = INVALID_PIO;
    uint32_t read_word     = 0;

    if (!fd || !chip_serie || !pio_port) {
        fprintf(stderr, "Error: null pointer input.\n");
        return false;
    }

    if (!chip_serie->pmcpcer0_reg || !chip_serie->pmcpcdr0_reg || !chip_serie->pmcpcsr0_reg || !chip_serie->pio_regs) {
        fprintf(stderr, "Error: Reading PIO Pin Data not defined for this MCU Series.\n");
        return false;
    }

    if (4 != strlen(pio_port)) {
        fprintf(stderr, "Error: PIO input does not match expected length. Expected one of following: PIOA, PIOB, PIOC (, PIOD or PIOE).\n");
        return false;
    }

    // Convert to uppercase for easier compare
    pio_port = string_upper(pio_port);

    if (strncmp("PIO", pio_port, 3)) {
        fprintf(stderr, "Error: PIO input does not start with PIO. Expected one of following: PIOA, PIOB, PIOC (, PIOD or PIOE).\n");
        return false;
    }

    if (!('A' <= pio_port[3] && pio_port[3] <= 'E')) {
        fprintf(stderr, "Error: The selected PIO Port does not exist.\n");
        return false;
    }

    pio = pio_port[3] - 'A';

    if (!(pio < chip_serie->nb_pio_regs))
    {
        fprintf(stderr, "Error: This device does not feature the selected PIO Port.\n");
        return false;
    }

    const struct _pio_regs* pio_regs = &(chip_serie->pio_regs[pio]);

    if (!pio_regs->peripheral_instance_id
        || !pio_regs->pio_pudr_reg
        || !pio_regs->pio_puer_reg
        || !pio_regs->pio_pusr_reg
        || !pio_regs->pio_ppddr_reg
        || !pio_regs->pio_ppder_reg
        || !pio_regs->pio_ppdsr_reg)
    {
        fprintf(stderr, "Error: One register of the selected PIO Port is not defined.\n");
        return false;
    }

    /*
    Procedure:
    1) Enable peripheral clock
    2) Reset pull-up, pull-down
    3) Set pull-up, pull-down
    4) Return pin data status register
    5) Disable peripheral clock
    */

    /* PMC Peripheral Clock Enable Register 0 */
    // Set
    if (!samba_write_word(fd, chip_serie->pmcpcer0_reg, pio_regs->peripheral_instance_id)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Read Status
    if (!samba_read_word(fd, chip_serie->pmcpcsr0_reg, &read_word)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Check if set
    if (!(read_word & pio_regs->peripheral_instance_id)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    /* Disable and enable pull-ups and pull-downs */
    // Disable Pull-up
    if (!samba_write_word(fd, pio_regs->pio_pudr_reg, bit_pattern_pull_down | bit_pattern_floating)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Disable Pull-down
    if (!samba_write_word(fd, pio_regs->pio_ppddr_reg, bit_pattern_pull_up | bit_pattern_floating)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Enable Pull-up
    if (!samba_write_word(fd, pio_regs->pio_puer_reg, bit_pattern_pull_up)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Enable Pull-down
    if (!samba_write_word(fd, pio_regs->pio_ppder_reg, bit_pattern_pull_down)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    /* PIO Pull-Up Status Register */
    // Read Status
    if (!samba_read_word(fd, pio_regs->pio_pusr_reg, &read_word)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Check if set
    // Invert because of: 0: Pull-up resistor is enabled on the I/O line
    if ((~read_word & bit_pattern_pull_up) != bit_pattern_pull_up) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    /* PIO Pull-Down Status Register */
    // Read Status
    if (!samba_read_word(fd, pio_regs->pio_ppdsr_reg, &read_word)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Check if set
    // Invert because of: 0: Pull-down resistor is enabled on the I/O line
    if ((~read_word & bit_pattern_pull_down) != bit_pattern_pull_down) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    /* PIO Pin Data Status Register */
    if (!samba_read_word(fd, pio_regs->pio_pdsr_reg, &read_word)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    /* Output PIO Pin Data Status Register */
    printf("PIO Pin Data Status Register Value: 0x%08X\n", read_word);

    /* PMC Peripheral Clock Disable Register 0 */
    // Set
    if (!samba_write_word(fd, chip_serie->pmcpcdr0_reg, pio_regs->peripheral_instance_id)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Read Status
    if (!samba_read_word(fd, chip_serie->pmcpcsr0_reg, &read_word)) {
    fprintf(stderr, "Error on line: %d.\n", __LINE__);
    return false;
    }

    // Check if still set
    if (read_word & pio_regs->peripheral_instance_id) {
        fprintf(stderr, "Error on line: %d.\n", __LINE__);
        return false;
    }

    return true;
}
