/*
 * TI81XX Power domain data
 *
 * Copyright (C) 2011-2012 Texas Instruments Incorporated - http://www.ti.com/
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

#include "powerdomain.h"
#include "prcm-common.h"
#include "prm-regbits-33xx.h"
#include "prm33xx.h"
#include "prm81xx.h"

static struct powerdomain sgx_pwrdm = {
	.name			= "sgx_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_SGX_MOD,
	.pwrstctrl_offs		= TI81XX_PM_SGX_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_SGX_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_ON,
	.banks			= 1,
	.mem_on_mask		= {
		[0]		= AM33XX_GFX_MEM_ONSTATE_MASK,	/* gfx_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= AM33XX_GFX_MEM_STATEST_MASK,	/* gfx_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* gfx_mem */
	},
};

static struct powerdomain ivahd2_pwrdm = {
	.name			= "ivahd2_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_IVAHD2_MOD,
	.pwrstctrl_offs		= TI81XX_PM_IVAHD2_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_IVAHD2_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_ON,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.logicretstate_mask	= AM33XX_LOGICRETSTATE_MASK,
	.banks			= 1,
	.mem_on_mask		= {
		[0]		= AM33XX_GFX_MEM_ONSTATE_MASK,	/* gfx_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= AM33XX_GFX_MEM_STATEST_MASK,	/* gfx_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* gfx_mem */
	},
};

static struct powerdomain ivahd1_pwrdm = {
	.name			= "ivahd1_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_IVAHD1_MOD,
	.pwrstctrl_offs		= TI81XX_PM_IVAHD1_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_IVAHD1_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_ON,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.logicretstate_mask	= AM33XX_LOGICRETSTATE_MASK,
	.banks			= 1,
	.mem_on_mask		= {
		[0]		= AM33XX_GFX_MEM_ONSTATE_MASK,	/* gfx_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= AM33XX_GFX_MEM_STATEST_MASK,	/* gfx_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* gfx_mem */
	},
};

static struct powerdomain ivahd0_pwrdm = {
	.name			= "ivahd0_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_IVAHD0_MOD,
	.pwrstctrl_offs		= TI81XX_PM_IVAHD0_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_IVAHD0_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_ON,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.logicretstate_mask	= AM33XX_LOGICRETSTATE_MASK,
	.banks			= 1,
	.mem_on_mask		= {
		[0]		= AM33XX_GFX_MEM_ONSTATE_MASK,	/* gfx_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= AM33XX_GFX_MEM_STATEST_MASK,	/* gfx_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* gfx_mem */
	},
};

static struct powerdomain active_pwrdm = {
	.name			= "active_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_ACTIVE_MOD,
	.pwrstctrl_offs		= TI81XX_PM_ACTIVE_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_ACTIVE_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_ON,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.logicretstate_mask	= AM33XX_LOGICRETSTATE_MASK,
	.banks			= 1,
	.mem_on_mask		= {
		[0]		= AM33XX_GFX_MEM_ONSTATE_MASK,	/* gfx_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= AM33XX_GFX_MEM_STATEST_MASK,	/* gfx_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* gfx_mem */
	},
};

static struct powerdomain default_pwrdm = {
	.name			= "default_pwrdm",
	.voltdm			= { .name = "core" },
	.prcm_offs		= TI81XX_PRM_DEFAULT_MOD,
	.pwrstctrl_offs		= TI81XX_PM_DEFAULT_PWRSTCTRL_OFFSET,
	.pwrstst_offs		= TI81XX_PM_DEFAULT_PWRSTST_OFFSET,
	.pwrsts			= PWRSTS_OFF_ON,
	.flags			= PWRDM_HAS_LOWPOWERSTATECHANGE,
	.logicretstate_mask	= AM33XX_LOGICRETSTATE_MASK,
	.banks			= 1,
	.mem_on_mask		= {
		[0]		= AM33XX_GFX_MEM_ONSTATE_MASK,	/* gfx_mem */
	},
	.mem_pwrst_mask		= {
		[0]		= AM33XX_GFX_MEM_STATEST_MASK,	/* gfx_mem */
	},
	.pwrsts_mem_on		= {
		[0]		= PWRSTS_ON,		/* gfx_mem */
	},
};

static struct powerdomain alwon_pwrdm = {
	.name			= "alwon_pwrdm",
	.voltdm			= { .name = "core" },
#if 0
	.pwrstctrl_offs		= AM33XX_PM_ALWON_PWRSTCTRL_OFFSET,
#endif
	.pwrsts			= PWRSTS_ON,
};

static struct powerdomain *powerdomains_ti81xx[] __initdata = {
	&alwon_pwrdm,
	&default_pwrdm,
	&active_pwrdm,
	&ivahd0_pwrdm,
	&ivahd1_pwrdm,
	&ivahd2_pwrdm,
	&sgx_pwrdm,
	NULL,
};

void __init ti81xx_powerdomains_init(void)
{
#if 0
	pwrdm_register_platform_funcs(&ti81xx_pwrdm_operations);
#endif
	/* looks like we should be able to reuse the pwrmd ops... but
	 * double-check whether that's really the case
	 */
	pwrdm_register_platform_funcs(&am33xx_pwrdm_operations);
	pwrdm_register_pwrdms(powerdomains_ti81xx);
	pwrdm_complete_init();
}
