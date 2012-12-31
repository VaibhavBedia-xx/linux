/*
 * AM33XX Power Management Routines
 *
 * Copyright (C) 2012 Texas Instruments Inc.
 * Vaibhav Bedia <vaibhav.bedia@ti.com>
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
#ifndef __ARCH_ARM_MACH_OMAP2_PM33XX_H
#define __ARCH_ARM_MACH_OMAP2_PM33XX_H

#include "control.h"

#ifndef __ASSEMBLER__

/*
 * This enum is used to index the array passed to suspend routine with
 * parameters that vary across DDR2 and DDR3 sleep sequence.
 *
 * Since these are used to load into registers by suspend code,
 * entries here must always be in sync with the suspend code
 * in arm/mach-omap2/sleep33xx.S
 */
enum suspend_cfg_params {
	EMIF_ADDR_VIRT = 0,
	MEMORY_TYPE,
	SUSPEND_CFG_PARAMS_END /* Must always be the last entry */
};

struct wkup_m3_context {
	struct am33xx_ipc_data	ipc_data;
	struct device		*dev;
	struct firmware		*firmware;
	struct mailbox		*mbox;
	void __iomem		*code;
	u8			state;
	u32			version;
};

#ifdef CONFIG_SUSPEND
static void am33xx_m3_state_machine_reset(void);
#else
static inline void am33xx_m3_state_machine_reset(void) {}
#endif /* CONFIG_SUSPEND */

extern void __iomem *am33xx_get_emif_base(void);
#endif

#define	IPC_CMD_DS0			0x3
#define IPC_CMD_RESET                   0xe
#define DS_IPC_DEFAULT			0xffffffff
#define M3_VERSION_UNKNOWN		0x0000ffff
#define M3_BASELINE_VERSION		0x20

#define IPC_RESP_SHIFT			16
#define IPC_RESP_MASK			(0xffff << 16)

#define M3_STATE_UNKNOWN		0
#define M3_STATE_RESET			1
#define M3_STATE_INITED			2
#define M3_STATE_MSG_FOR_LP		3
#define M3_STATE_MSG_FOR_RESET		4

#define AM33XX_OCMC_END			0x40310000
#define AM33XX_EMIF_BASE		0x4C000000

#define MEM_TYPE_DDR2		2

#endif
