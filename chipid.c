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
#include <string.h>
#include "chipid.h"
#include "comm.h"
#include "utils.h"

static const struct _chip _chips_samx7[] = {
	{ "SAME70Q21A", 0xa1020e00, 0x00000002, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAME70Q21B", 0xa1020e01, 0x00000002, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAME70Q20", 0xa1020c00, 0x00000002, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAME70Q19", 0xa10d0a00, 0x00000002, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAME70N21A", 0xa1020e00, 0x00000001, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAME70N21B", 0xa1020e01, 0x00000001, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAME70N20A", 0xa1020c00, 0x00000001, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAME70N20B", 0xa1020c01, 0x00000001, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAME70N19", 0xa10d0a00, 0x00000001, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAME70J21", 0xa1020e00, 0x00000000, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAME70J20", 0xa1020c00, 0x00000000, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAME70J19", 0xa10d0a00, 0x00000000, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMS70Q21A", 0xa1120e00, 0x00000002, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMS70Q21B", 0xa1120e01, 0x00000002, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMS70N21A", 0xa1120e00, 0x00000001, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMS70N21B", 0xa1120e01, 0x00000001, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMS70Q20", 0xa1120c00, 0x00000002, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMS70Q19", 0xa11d0a00, 0x00000002, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMS70N21A", 0xa1120e00, 0x00000001, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMS70N21B", 0xa1120e01, 0x00000001, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMS70N20A", 0xa1120c00, 0x00000001, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMS70N20B", 0xa1120c01, 0x00000001, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMS70N19", 0xa11d0a00, 0x00000001, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMS70J21", 0xa1120e00, 0x00000000, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMS70J20", 0xa1120c00, 0x00000000, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMS70J19", 0xa11d0a00, 0x00000000, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMV71Q21", 0xa1220e00, 0x00000002, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMV71Q20", 0xa1220c00, 0x00000002, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMV71Q19", 0xa12d0a00, 0x00000002, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMV71N21", 0xa1220e00, 0x00000001, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMV71N20", 0xa1220c00, 0x00000001, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMV71N19", 0xa12d0a00, 0x00000001, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMV71J21", 0xa1220e00, 0x00000000, 0x400e0c00, 0x00400000, 2048, 9, 0xa5000005 },
	{ "SAMV71J20", 0xa1220c00, 0x00000000, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMV71J19", 0xa12d0a00, 0x00000000, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMV70Q20", 0xa1320c00, 0x00000002, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMV70Q19", 0xa13d0a00, 0x00000002, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMV70N20A", 0xa1320c00, 0x00000001, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMV70N20B", 0xa1320c01, 0x00000001, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMV70N19", 0xa13d0a00, 0x00000001, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
	{ "SAMV70J20", 0xa1320c00, 0x00000000, 0x400e0c00, 0x00400000, 1024, 9, 0xa5000005 },
	{ "SAMV70J19", 0xa13d0a00, 0x00000000, 0x400e0c00, 0x00400000,  512, 9, 0xa5000005 },
};

static const struct _chip _chips_sam4[] = {
	{ "SAM4E8CA" , 0xa3cc0ce0, 0x00120209, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
	{ "SAM4E8CB" , 0xa3cc0ce1, 0x00120209, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
	{ "SAM4E8EA" , 0xa3cc0ce0, 0x00120208, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
	{ "SAM4E8EB" , 0xa3cc0ce1, 0x00120208, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
};

static const struct _chip _chips_sam4s[] = {
	{ "SAM4SA16C (Rev A)" , 0x28A70CE0, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4SA16C (Rev B)" , 0x28A70CE1, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4SA16B (Rev A)" , 0x28970CE0, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4SA16B (Rev B)" , 0x28970CE1, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4S16B (Rev A)"  , 0x289C0CE0, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4S16B (Rev B)"  , 0x289C0CE1, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4S16C (Rev A)"  , 0x28AC0CE0, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4S16C (Rev B)"  , 0x28AC0CE1, 0x00000000, 0x400e0a00, 0x00400000,  1024,	2, 0xa5000005 },
	{ "SAM4S8B (Rev A)"   , 0x289C0AE0, 0x00000000, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
	{ "SAM4S8B (Rev B)"   , 0x289C0AE1, 0x00000000, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
	{ "SAM4S8C (Rev A)"   , 0x28AC0AE0, 0x00000000, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
	{ "SAM4S8C (Rev B)"   , 0x28AC0AE1, 0x00000000, 0x400e0a00, 0x00400000,  512,	2, 0xa5000005 },
	{ "SAM4S4C (Rev A)"   , 0x28AB09E0, 0x00000000, 0x400e0a00, 0x00400000,  256,	2, 0xa5000005 },
	{ "SAM4S4C (Rev B)"   , 0x28AB09E1, 0x00000000, 0x400e0a00, 0x00400000,  256,	2, 0xa5000005 },
	{ "SAM4S4B (Rev A)"   , 0x289B09E0, 0x00000000, 0x400e0a00, 0x00400000,  256,	2, 0xa5000005 },
	{ "SAM4S4B (Rev B)"   , 0x289B09E1, 0x00000000, 0x400e0a00, 0x00400000,  256,	2, 0xa5000005 },
	{ "SAM4S4A (Rev A)"   , 0x288B09E0, 0x00000000, 0x400e0a00, 0x00400000,  256,	2, 0xa5000005 },
	{ "SAM4S4A (Rev B)"   , 0x288B09E1, 0x00000000, 0x400e0a00, 0x00400000,  256,	2, 0xa5000005 },
	{ "SAM4S2C (Rev A)"   , 0x28AB07E0, 0x00000000, 0x400e0a00, 0x00400000,  128,	2, 0xa5000005 },
	{ "SAM4S2C (Rev B)"   , 0x28AB07E1, 0x00000000, 0x400e0a00, 0x00400000,  128,	2, 0xa5000005 },
	{ "SAM4S2B (Rev A)"   , 0x289B07E0, 0x00000000, 0x400e0a00, 0x00400000,  128,	2, 0xa5000005 },
	{ "SAM4S2B (Rev B)"   , 0x289B07E1, 0x00000000, 0x400e0a00, 0x00400000,  128,	2, 0xa5000005 },
	{ "SAM4S2A (Rev A)"   , 0x288B07E0, 0x00000000, 0x400e0a00, 0x00400000,  128,	2, 0xa5000005 },
	{ "SAM4S2A (Rev B)"   , 0x288B07E1, 0x00000000, 0x400e0a00, 0x00400000,  128,	2, 0xa5000005 },
};

static const struct _pio_regs _pio_regs_sam4s[] = {
	{ (1 << 11), 0x400E0E60, 0x400E0E64, 0x400E0E68, 0x400E0E90, 0x400E0E94, 0x400E0E98, 0x400E0E3C },
	{ (1 << 12), 0x400E1060, 0x400E1064, 0x400E1068, 0x400E1090, 0x400E1094, 0x400E1098, 0x400E103C },
	{ (1 << 13), 0x400E1260, 0x400E1264, 0x400E1268, 0x400E1290, 0x400E1294, 0x400E1298, 0x400E123C },
};

static const struct _chip_serie _chip_series[] = {
	{
		.name       = "samx7",
		.cidr_reg   = 0x400e0940,
		.exid_reg   = 0x400e0944,
		.rstccr_reg = 0x400e1800,
		.nb_chips   = ARRAY_SIZE(_chips_samx7),
		.chips      = _chips_samx7,
	},
	{
		.name       = "sam4",
		.cidr_reg   = 0x400e0740,
		.exid_reg   = 0x400e0744,
		.rstccr_reg = 0x400e1800,
		.nb_chips   = ARRAY_SIZE(_chips_sam4),
		.chips      = _chips_sam4,
	},
	{
		.name         = "sam4s",
		.cidr_reg     = 0x400e0740,
		.exid_reg     = 0x400e0744,
		.rstccr_reg   = 0x400e1400,
		.pmcpcer0_reg = 0x400E0410,
		.pmcpcdr0_reg = 0x400E0414,
		.pmcpcsr0_reg = 0x400E0418,
		.nb_chips     = ARRAY_SIZE(_chips_sam4s),
		.chips	      = _chips_sam4s,
	    .nb_pio_regs  = ARRAY_SIZE(_pio_regs_sam4s),
		.pio_regs     = _pio_regs_sam4s,
	},
};

const struct _chip_serie* chipid_get_serie(const char* name)
{
	for (int i = 0; i < ARRAY_SIZE(_chip_series); i++)
		if (!strcmp(_chip_series[i].name, name))
			return &_chip_series[i];
	return NULL;
}

bool chipid_check_serie(serial_port_handle_t fd, const struct _chip_serie* serie, const struct _chip** chip)
{
	// Read chip identifiers (CIDR/EXID)
	uint32_t cidr, exid;
	if (!samba_read_word(fd, serie->cidr_reg, &cidr))
		return false;
	if (!samba_read_word(fd, serie->exid_reg, &exid))
		return false;

	printf("Testing series %s\nRead CIDR %08x EXID %08x\n", serie->name, cidr, exid);

	// Identify chip and read its flash infos
	for (int i = 0; i < serie->nb_chips; i++) {
		if (serie->chips[i].cidr == cidr && serie->chips[i].exid == exid) {
			*chip = &serie->chips[i];
			return true;
		}
	}

	return false;
}

const struct _chip_serie* chipid_identity_serie(serial_port_handle_t fd, const struct _chip** chip)
{
	for (int i = 0; i < ARRAY_SIZE(_chip_series); i++)
		if (chipid_check_serie(fd, &_chip_series[i], chip))
			return &_chip_series[i];
	return NULL;
}


// _chip_series muss extern definiert sein, z. B. in einer anderen Quelldatei.
extern const struct _chip_serie _chip_series[];

uint32_t supported_chips_checksum(void) {
    uint32_t checksum = 0;

    for (size_t i = 0; i < ARRAY_SIZE(_chip_series); i++) {
        const struct _chip_serie *serie = &_chip_series[i];

		uint32_t series_checksum = 0;

        // Verarbeite den String der Serienbezeichnung
        if (serie->name) {
            const char *ptr = serie->name;
            while (*ptr) {
                series_checksum += (uint32_t)*ptr++;
            }
        }

        // Addiere numerische Felder der Serie
        series_checksum += serie->cidr_reg;
        series_checksum += serie->exid_reg;
        series_checksum += serie->rstccr_reg;
        series_checksum += serie->pmcpcer0_reg;
        series_checksum += serie->pmcpcdr0_reg;
        series_checksum += serie->pmcpcsr0_reg;
        series_checksum += serie->nb_chips;

        // Iteriere über die unterstützten Chips dieser Serie
        for (size_t j = 0; j < serie->nb_chips; j++) {
            const struct _chip *chip = &serie->chips[j];

            // Verarbeite den String der Chipbezeichnung
            if (chip->name) {
                const char *ptr = chip->name;
                while (*ptr) {
                    series_checksum += (uint32_t)*ptr++;
                }
            }

            // Addiere die restlichen numerischen Felder
            series_checksum += chip->cidr;
            series_checksum += chip->exid;
            series_checksum += chip->eefc_base;
            series_checksum += chip->flash_addr;
            series_checksum += chip->flash_size;
            series_checksum += chip->gpnvm;
            series_checksum += chip->rstccr;
        }

        // Iteriere über die PIO Register, wenn vorhanden
        if (NULL != serie->pio_regs) {
            for (size_t j = 0; j < serie->nb_pio_regs; j++) {
                const struct _pio_regs* pio_regs = &serie->pio_regs[j];

                // Addiere die numerischen Felder
                series_checksum += pio_regs->peripheral_instance_id;
                series_checksum += pio_regs->pio_pudr_reg;
                series_checksum += pio_regs->pio_puer_reg;
                series_checksum += pio_regs->pio_pusr_reg;
                series_checksum += pio_regs->pio_ppddr_reg;
                series_checksum += pio_regs->pio_ppder_reg;
                series_checksum += pio_regs->pio_ppdsr_reg;
                series_checksum += pio_regs->pio_pdsr_reg;
            }
        }

        printf("Processor Family: %s; Supported Devices: %u; Checksum 0x%08x\n", serie->name, serie->nb_chips, series_checksum);
        checksum += series_checksum;
    }

    //printf("Checksum: 0x%08x\n", checksum);
	return checksum;
}
