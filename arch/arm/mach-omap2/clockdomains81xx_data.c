/*
 * AM33XX Clock Domain data.
 *
 * Copyright (C) 2011-2012 Texas Instruments Incorporated - http://www.ti.com/
 * Vaibhav Hiremath <hvaibhav@ti.com>
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
#include <linux/io.h>

#include "clockdomain.h"
#include "cm.h"
#include "cm81xx.h"
#include "cm-regbits-33xx.h"	/* has the bit fields for cm
registers. doesn't seem to be used anywhere? */

static struct clockdomain gem_ti81xx_clkdm = {
	.name		= "gem_clkdm",
	.pwrdm		= { .name = "active_pwrdm" },
	.cm_inst	= TI81XX_CM_ACTIVE_MOD,
	.clkdm_offs	= TI81XX_CM_ACTIVE_GEM_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain hdvpss_ti81xx_clkdm = {
	.name		= "hdvpss_clkdm",
	.pwrdm		= { .name = "active_pwrdm" },
	.cm_inst	= TI81XX_CM_ACTIVE_MOD,
	.clkdm_offs	= TI81XX_CM_ACTIVE_HDVPSS_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain hdmi_ti81xx_clkdm = {
	.name		= "hdmi_clkdm",
	.pwrdm		= { .name = "active_pwrdm" },
	.cm_inst	= TI81XX_CM_ACTIVE_MOD,
	.clkdm_offs	= TI81XX_CM_ACTIVE_HDMI_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain l3med_ti81xx_clkdm = {
	.name		= "l3med_default_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.clkdm_offs	= TI81XX_CM_DEFAULT_L3_MED_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain l3fast_ti81xx_clkdm = {
	.name		= "l3fast_default_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.clkdm_offs	= TI81XX_CM_DEFAULT_L3_FAST_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain tppss_ti81xx_clkdm = {
	.name		= "tppss_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.clkdm_offs	= TI81XX_CM_DEFAULT_TPPSS_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain pci_ti81xx_clkdm = {
	.name		= "pci_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.clkdm_offs	= TI81XX_CM_DEFAULT_PCI_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain l3slow_ti81xx_clkdm = {
	.name		= "l3slow_default_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.clkdm_offs	= TI81XX_CM_DEFAULT_TPPSS_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain ducati_ti81xx_clkdm = {
	.name		= "ducati_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI81XX_CM_DEFAULT_MOD,
	.clkdm_offs	= TI81XX_CM_DEFAULT_DUCATI_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain ivahd0_ti81xx_clkdm = {
	.name		= "ivahd0_clkdm",
	.pwrdm		= { .name = "ivahd0_pwrdm" },
	.cm_inst	= TI81XX_CM_IVAHD0_MOD,
	.clkdm_offs	= TI81XX_CM_IVAHD0_IVAHD_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain ivahd1_ti81xx_clkdm = {
	.name		= "ivahd1_clkdm",
	.pwrdm		= { .name = "ivahd1_pwrdm" },
	.cm_inst	= TI81XX_CM_IVAHD1_MOD,
	.clkdm_offs	= TI81XX_CM_IVAHD1_IVAHD_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain ivahd2_ti81xx_clkdm = {
	.name		= "ivahd2_clkdm",
	.pwrdm		= { .name = "ivahd2_pwrdm" },
	.cm_inst	= TI81XX_CM_IVAHD2_MOD,
	.clkdm_offs	= TI81XX_CM_IVAHD2_IVAHD_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain sgx_ti81xx_clkdm = {
	.name		= "sgx_clkdm",
	.pwrdm		= { .name = "sgx_pwrdm" },
	.cm_inst	= TI81XX_CM_SGX_MOD,
	.clkdm_offs	= TI81XX_CM_SGX_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain l3slow_alwon_ti81xx_clkdm = {
	.name		= "l3slow_alwon_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_SLOW_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain ethernet_ti81xx_clkdm = {
	.name		= "ethernet_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_ETHERNET_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain l3med_alwon_ti81xx_clkdm = {
	.name		= "l3med_alwon_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_MED_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain mmu_ti81xx_clkdm = {
	.name		= "mmu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_MMU_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain mmucfg_ti81xx_clkdm = {
	.name		= "mmucfg_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_MMUCFG_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain ocmc0_ti81xx_clkdm = {
	.name		= "ocmc0_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_OCMC_0_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain ocmc1_ti81xx_clkdm = {
	.name		= "ocmc1_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_OCMC_1_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain mpu_ti81xx_clkdm = {
	.name		= "mpu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_MPU_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain sysclk4_ti81xx_clkdm = {
	.name		= "sysclk4_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_SYSCLK4_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain sysclk5_ti81xx_clkdm = {
	.name		= "sysclk5_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_SYSCLK5_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain sysclk6_ti81xx_clkdm = {
	.name		= "sysclk6_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_SYSCLK6_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain rtc_ti81xx_clkdm = {
	.name		= "rtc_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_RTC_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain l3fast_alwon_ti81xx_clkdm = {
	.name		= "l3fast_alwon_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_FAST_CLKSTCTRL_OFFSET,
	.flags		= CLKDM_CAN_SWSUP,
};

static struct clockdomain *clockdomains_ti81xx[] __initdata = {
	&gem_ti81xx_clkdm,
	&hdvpss_ti81xx_clkdm,
	&hdmi_ti81xx_clkdm,
	&l3med_ti81xx_clkdm,
	&l3fast_ti81xx_clkdm,
	&tppss_ti81xx_clkdm,
	&pci_ti81xx_clkdm,
	&l3slow_ti81xx_clkdm,
	&ducati_ti81xx_clkdm,
	&ivahd0_ti81xx_clkdm,
	&ivahd1_ti81xx_clkdm,
	&ivahd2_ti81xx_clkdm,
	&sgx_ti81xx_clkdm,
	&l3slow_alwon_ti81xx_clkdm,
	&ethernet_ti81xx_clkdm,
	&l3med_alwon_ti81xx_clkdm,
	&mmu_ti81xx_clkdm,
	&mmucfg_ti81xx_clkdm,
	&ocmc0_ti81xx_clkdm,
	&ocmc1_ti81xx_clkdm,
	&mpu_ti81xx_clkdm,
	&sysclk4_ti81xx_clkdm,
	&sysclk5_ti81xx_clkdm,
	&sysclk6_ti81xx_clkdm,
	&rtc_ti81xx_clkdm,
	&l3fast_alwon_ti81xx_clkdm,
	NULL,
};

void __init ti81xx_clockdomains_init(void)
{
#if 0
	clkdm_register_platform_funcs(&ti81xx_clkdm_operations);
#endif
	clkdm_register_platform_funcs(&am33xx_clkdm_operations);
	clkdm_register_clkdms(clockdomains_ti81xx);
	clkdm_complete_init();
}
