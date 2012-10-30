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

#ifndef __ASSEMBLER__
struct wkup_m3_context {
	struct device		*dev;
	struct firmware		*firmware;
	struct omap_mbox	*mbox;
	void __iomem		*code;
	int			resume_addr;
	int			ipc_data1;
	int			ipc_data2;
	int			sleep_mode;
	u8			state;
};

#ifdef CONFIG_SUSPEND
static void am33xx_ipc_cmd(void);
static void am33xx_m3_state_machine_reset(void);
#else
static inline void am33xx_ipc_cmd(void) {}
static inline void am33xx_m3_state_machine_reset(void) {}
#endif /* CONFIG_SUSPEND */

extern void __iomem *am33xx_get_emif_base(void);
int wkup_mbox_msg(struct notifier_block *self, unsigned long len, void *msg);
#endif

#define	IPC_CMD_DS0			0x3
#define IPC_CMD_RESET                   0xe
#define DS_IPC_DEFAULT			0xffffffff

#define IPC_RESP_SHIFT			16
#define IPC_RESP_MASK			(0xffff << 16)

#define M3_STATE_UNKNOWN		0
#define M3_STATE_RESET			1
#define M3_STATE_INITED			2
#define M3_STATE_MSG_FOR_LP		3
#define M3_STATE_MSG_FOR_RESET		4

#define AM33XX_OCMC_END			0x40310000
#define AM33XX_EMIF_BASE		0x4C000000

/*
 * This a subset of registers defined in drivers/memory/emif.h
 * Move that to include/linux/?
 */
#define EMIF_MODULE_ID_AND_REVISION			0x0000
#define EMIF_STATUS					0x0004
#define EMIF_SDRAM_CONFIG				0x0008
#define EMIF_SDRAM_CONFIG_2				0x000c
#define EMIF_SDRAM_REFRESH_CONTROL			0x0010
#define EMIF_SDRAM_REFRESH_CTRL_SHDW			0x0014
#define EMIF_SDRAM_TIMING_1				0x0018
#define EMIF_SDRAM_TIMING_1_SHDW			0x001c
#define EMIF_SDRAM_TIMING_2				0x0020
#define EMIF_SDRAM_TIMING_2_SHDW			0x0024
#define EMIF_SDRAM_TIMING_3				0x0028
#define EMIF_SDRAM_TIMING_3_SHDW			0x002c
#define EMIF_LPDDR2_NVM_TIMING				0x0030
#define EMIF_LPDDR2_NVM_TIMING_SHDW			0x0034
#define EMIF_POWER_MANAGEMENT_CONTROL			0x0038
#define EMIF_POWER_MANAGEMENT_CTRL_SHDW			0x003c
#define EMIF_PERFORMANCE_COUNTER_1			0x0080
#define EMIF_PERFORMANCE_COUNTER_2			0x0084
#define EMIF_PERFORMANCE_COUNTER_CONFIG			0x0088
#define EMIF_PERFORMANCE_COUNTER_MASTER_REGION_SELECT	0x008c
#define EMIF_PERFORMANCE_COUNTER_TIME			0x0090
#define EMIF_MISC_REG					0x0094
#define EMIF_DLL_CALIB_CTRL				0x0098
#define EMIF_DLL_CALIB_CTRL_SHDW			0x009c
#define EMIF_SYSTEM_OCP_INTERRUPT_RAW_STATUS		0x00a4
#define EMIF_SYSTEM_OCP_INTERRUPT_STATUS		0x00ac
#define EMIF_SYSTEM_OCP_INTERRUPT_ENABLE_SET		0x00b4
#define EMIF_SYSTEM_OCP_INTERRUPT_ENABLE_CLEAR		0x00bc
#define EMIF_SDRAM_OUTPUT_IMPEDANCE_CALIBRATION_CONFIG	0x00c8
#define EMIF_READ_WRITE_LEVELING_RAMP_WINDOW		0x00d4
#define EMIF_READ_WRITE_LEVELING_RAMP_CONTROL		0x00d8
#define EMIF_READ_WRITE_LEVELING_CONTROL		0x00dc
#define EMIF_DDR_PHY_CTRL_1				0x00e4
#define EMIF_DDR_PHY_CTRL_1_SHDW			0x00e8

#endif
