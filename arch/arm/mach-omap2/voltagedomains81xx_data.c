/*
 * TI81XX voltage domain data
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/init.h>

#include "voltage.h"

#if 0
static struct voltagedomain ti81xx_voltdm_mpu = {
	.name		= "mpu",
};
#endif

/* let's assume there's only one voltage domain in the SoC */
static struct voltagedomain ti81xx_voltdm_core = {
	.name		= "core",
};

#if 0
static struct voltagedomain ti81xx_voltdm_rtc = {
	.name		= "rtc",
};
#endif

static struct voltagedomain *voltagedomains_ti81xx[] __initdata = {
	&ti81xx_voltdm_core,
#if 0
	&ti81xx_voltdm_mpu,
	&ti81xx_voltdm_rtc,
#endif
	NULL,
};

void __init ti81xx_voltagedomains_init(void)
{
	voltdm_init(voltagedomains_ti81xx);
}
