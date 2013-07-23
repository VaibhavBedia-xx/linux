/*
 * Low level suspend code for AM33XX SoCs
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
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

#include <linux/linkage.h>
#include <linux/ti_emif.h>
#include <asm/memory.h>
#include <asm/assembler.h>

#include "cm33xx.h"
#include "cm43xx.h"
#include "pm33xx.h"
#include "prm33xx.h"
#include "prm43xx.h"

#include "common.h"
#include "omap-secure.h"
#include "omap44xx.h"
#include <asm/hardware/cache-l2x0.h>

	.text
	.align 3

/*
 * This routine is executed from internal RAM and expects some
 * parameters to be passed in r0 _strictly_ in following order:
 * 1) emif_addr_virt - ioremapped EMIF address
 * 2) mem_type - 2 -> DDR2, 3-> DDR3
 * 3) dram_sync_word - uncached word in SDRAM
 * 4) l2_virt_base - L2CC ioremapped address for AM437x
 *
 * The code loads these values taking r0 value as reference to
 * the array in registers starting from r0, i.e emif_addr_virt
 * goes to r1, mem_type goes to r2 and and so on. These are
 * then saved into memory locations before proceeding with the
 * sleep sequence and hence registers r0, r1 etc can still be
 * used in the rest of the sleep code.
 */

ENTRY(am33xx_do_wfi)
	stmfd	sp!, {r4 - r11, lr}	@ save registers on stack

	ldm	r0, {r1-r5}		@ gather values passed

	/* Save the values passed */
	str	r1, emif_addr_virt
	str	r2, mem_type
	str	r3, dram_sync_word
	str	r4, l2_base_virt
	str	r5, cpu_id

	ldr	r0, scratchpad_virt
	mov	r1, #0x1
	str	r1, [r0]

	/*
	 * Flush all data from the L1 data cache before disabling
	 * SCTLR.C bit.
	 */
	ldr	r1, kernel_flush
	mov	lr, pc
	bx	r1

	/*
	 * Clear the SCTLR.C bit to prevent further data cache
	 * allocation. Clearing SCTLR.C would make all the data accesses
	 * strongly ordered and would not hit the cache.
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #(1 << 2)	@ Disable the C bit
	mcr	p15, 0, r0, c1, c0, 0
	isb
	dsb

	/*
	 * Invalidate L1 data cache. Even though only invalidate is
	 * necessary exported flush API is used here. Doing clean
	 * on already clean cache would be almost NOP.
	 */
	ldr	r1, kernel_flush
	blx	r1
	/*
	 * The kernel doesn't interwork: v7_flush_dcache_all in particluar will
	 * always return in Thumb state when CONFIG_THUMB2_KERNEL is enabled.
	 * This sequence switches back to ARM.  Note that .align may insert a
	 * nop: bx pc needs to be word-aligned in order to work.
	 */
 THUMB(	.thumb		)
 THUMB(	.align		)
 THUMB(	bx	pc	)
 THUMB(	nop		)
	.arm

	ldr	r0, scratchpad_virt
	mov	r1, #0x2
	str	r1, [r0]

	ldr	r0, cpu_id
	cmp	r0, #0x1
	beq	skip_l2_invalidate

#ifdef CONFIG_CACHE_L2X0
	/*
	 * Clean and invalidate the L2 cache.
	 * TODO: Unlike OMAP4 can we use common cache-l2x0.c functions?
	 */
#ifdef CONFIG_PL310_ERRATA_727915
	mov	r0, #0x03
	mov	r12, #OMAP4_MON_L2X0_DBG_CTRL_INDEX
	dsb
	smc	#0
	dsb
#endif
	ldr	r0, scratchpad_virt
	mov	r1, #0x3
	str	r1, [r0]

	ldr	r0, l2_base_virt
	mov	r2, r0
	ldr	r0, [r2, #L2X0_AUX_CTRL]
	str	r0, l2_aux_ctrl_val
	ldr	r0, [r2, #L2X0_PREFETCH_CTRL]
	str	r0, l2_prefetch_ctrl_val

	ldr	r0, l2_val
	str	r0, [r2, #L2X0_CLEAN_INV_WAY]
wait:
	ldr	r0, [r2, #L2X0_CLEAN_INV_WAY]
	ldr	r1, l2_val
	ands	r0, r0, r1
	bne	wait
#ifdef CONFIG_PL310_ERRATA_727915
	mov	r0, #0x00
	mov	r12, #OMAP4_MON_L2X0_DBG_CTRL_INDEX
	dsb
	smc	#0
	dsb
#endif
l2x_sync:
	ldr	r0, l2_base_virt
	mov	r2, r0
	mov	r0, #0x0
	str	r0, [r2, #L2X0_CACHE_SYNC]
sync:
	ldr	r0, [r2, #L2X0_CACHE_SYNC]
	ands	r0, r0, #0x1
	bne	sync
#endif
skip_l2_invalidate:

	ldr	r0, emif_addr_virt
	/* Save EMIF configuration */
	ldr	r1, [r0, #EMIF_SDRAM_CONFIG]
	str	r1, emif_sdcfg_val
	ldr	r1, [r0, #EMIF_SDRAM_REFRESH_CONTROL]
	str	r1, emif_ref_ctrl_val
	ldr	r1, [r0, #EMIF_SDRAM_TIMING_1]
	str	r1, emif_timing1_val
	ldr	r1, [r0, #EMIF_SDRAM_TIMING_2]
	str	r1, emif_timing2_val
	ldr	r1, [r0, #EMIF_SDRAM_TIMING_3]
	str	r1, emif_timing3_val
	ldr	r1, [r0, #EMIF_POWER_MANAGEMENT_CONTROL]
	str	r1, emif_pmcr_val
	ldr	r1, [r0, #EMIF_POWER_MANAGEMENT_CTRL_SHDW]
	str	r1, emif_pmcr_shdw_val
	ldr	r1, [r0, #EMIF_SDRAM_OUTPUT_IMPEDANCE_CALIBRATION_CONFIG]
	str	r1, emif_zqcfg_val
	ldr	r1, [r0, #EMIF_DDR_PHY_CTRL_1]
	str	r1, emif_rd_lat_val

	/* Put SDRAM in self-refresh */
	ldr	r1, [r0, #EMIF_POWER_MANAGEMENT_CONTROL]
	orr	r1, r1, #0xa0
	str	r1, [r0, #EMIF_POWER_MANAGEMENT_CTRL_SHDW]
	str	r1, [r0, #4]

	ldr	r1, dram_sync_word	@ a dummy access to DDR as per spec
	ldr	r2, [r1, #0]
	ldr	r1, [r0, #EMIF_POWER_MANAGEMENT_CONTROL]
	orr	r1, r1, #0x200
	str	r1, [r0, #EMIF_POWER_MANAGEMENT_CONTROL]

	mov	r1, #0x1000		@ Wait for system to enter SR
wait_sr:
	subs	r1, r1, #1
	bne	wait_sr

	ldr	r0, scratchpad_virt
	mov	r1, #0x4
	str	r1, [r0]

	/* Disable EMIF */
	ldr	r0, cpu_id
	cmp	r0, #0x1
	ldreq	r1, am33xx_virt_emif_clkctrl
	ldrne	r1, am43xx_virt_emif_clkctrl
	ldr	r2, [r1]
	bic	r2, r2, #0x03
	str	r2, [r1]

wait_emif_disable:
	ldr	r2, [r1]
	ldr	r3, module_disabled_val
	cmp	r2, r3
	bne	wait_emif_disable

	/*
	 * For the MPU WFI to be registered as an interrupt
	 * to WKUP_M3, MPU_CLKCTRL.MODULEMODE needs to be set
	 * to DISABLED
	 */
	ldr	r0, cpu_id
	cmp	r0, #0x1
	ldreq	r1, am33xx_virt_mpu_clkctrl
	ldrne	r1, am43xx_virt_mpu_clkctrl
	mov	r2, #0x0
	str	r2, [r1]

	/*
	 * Put MPU CLKDM to SW_SLEEP
	 */
	ldr	r0, cpu_id
	cmp	r0, #0x1
	ldreq	r1, am33xx_virt_mpu_clkstctrl
	ldrne	r1, am43xx_virt_mpu_clkstctrl
	mov	r2, #0x1
	str	r2, [r1]

	ldr	r0, scratchpad_virt
	mov	r1, #0x5
	str	r1, [r0]

	dsb
	dmb
	isb

	wfi
	/* NOPs to ensure the A8 pipeline is clean */
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	ldr	r0, scratchpad_virt
	mov	r1, #0x6
	str	r1, [r0]

	/* We come here in case of an abort due to a late interrupt */

	ldr	r0, cpu_id
	cmp	r0, #0x1
	ldreq	r1, am33xx_virt_mpu_clkstctrl
	ldrne	r1, am43xx_virt_mpu_clkstctrl
	mov	r2, #0x02
	str	r2, [r1]

	/* Set MPU_CLKCTRL.MODULEMODE back to ENABLE */
	ldr	r0, cpu_id
	cmp	r0, #0x1
	ldreq	r1, am33xx_virt_mpu_clkctrl
	ldrne	r1, am43xx_virt_mpu_clkctrl
	mov	r2, #0x02
	str	r2, [r1]

	/* Re-enable EMIF */
	ldr	r0, cpu_id
	cmp	r0, #0x1
	ldreq	r1, am33xx_virt_emif_clkctrl
	ldrne	r1, am43xx_virt_emif_clkctrl
	mov	r2, #0x02
	str	r2, [r1]
wait_emif_enable:
	ldr	r3, [r1]
	cmp	r2, r3
	bne	wait_emif_enable

	ldr	r0, scratchpad_virt
	mov	r1, #0x7
	str	r1, [r0]

	/* Disable EMIF self-refresh */
	ldr	r0, emif_addr_virt
	ldr	r1, [r0, #EMIF_POWER_MANAGEMENT_CONTROL]
	bic	r1, r1, #LP_MODE_MASK
	str	r1, [r0, #EMIF_POWER_MANAGEMENT_CONTROL]
	str	r1, [r0, #EMIF_POWER_MANAGEMENT_CTRL_SHDW]

	/*
	 * A write to SDRAM CONFIG register triggers
	 * an init sequence and hence it must be done
	 * at the end for DDR2
	 */
	ldr r0, emif_addr_virt
	add r0, r0, #EMIF_SDRAM_CONFIG
	ldr r4, emif_sdcfg_val
	str r4, [r0]

	/*
	 * Set SCTLR.C bit to allow data cache allocation
	 */
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #(1 << 2)	@ Enable the C bit
	mcr	p15, 0, r0, c1, c0, 0
	isb

	/* Kill some time for sanity to settle in */
	mov r0, #0x1000
wait_abt:
	subs   r0, r0, #1
	bne wait_abt

	ldr	r0, scratchpad_virt
	mov	r1, #0x8
	str	r1, [r0]

	/* Let the suspend code know about the abort */
	mov	r0, #1
	ldmfd	sp!, {r4 - r11, pc}	@ restore regs and return
ENDPROC(am33xx_do_wfi)

	.align
ENTRY(am33xx_resume_offset)
	.word . - am33xx_do_wfi

ENTRY(am33xx_resume_from_deep_sleep)
	/* Re-enable EMIF */

	ldr	r0, scratchpad
	mov	r1, #0x10
	str	r1, [r0]

	ldr	r0, cpu_id
	cmp	r0, #0x1
	ldreq	r1, am33xx_phys_emif_clkctrl
	ldrne	r1, am43xx_phys_emif_clkctrl
	mov	r2, #0x02
	str	r2, [r1]
wait_emif_enable1:
	ldr	r3, [r1]
	cmp	r2, r3
	bne	wait_emif_enable1

	ldr	r0, scratchpad
	mov	r1, #0x11
	str	r1, [r0]

	/* Config EMIF Timings */
	ldr	r0, emif_phys_addr
	ldr	r1, emif_rd_lat_val
	str	r1, [r0, #EMIF_DDR_PHY_CTRL_1]
	str	r1, [r0, #EMIF_DDR_PHY_CTRL_1_SHDW]
	ldr	r1, emif_timing1_val
	str	r1, [r0, #EMIF_SDRAM_TIMING_1]
	str	r1, [r0, #EMIF_SDRAM_TIMING_1_SHDW]
	ldr	r1, emif_timing2_val
	str	r1, [r0, #EMIF_SDRAM_TIMING_2]
	str	r1, [r0, #EMIF_SDRAM_TIMING_2_SHDW]
	ldr	r1, emif_timing3_val
	str	r1, [r0, #EMIF_SDRAM_TIMING_3]
	str	r1, [r0, #EMIF_SDRAM_TIMING_3_SHDW]
	ldr	r1, emif_ref_ctrl_val
	str	r1, [r0, #EMIF_SDRAM_REFRESH_CONTROL]
	str	r1, [r0, #EMIF_SDRAM_REFRESH_CTRL_SHDW]
	ldr	r1, emif_pmcr_val
	str	r1, [r0, #EMIF_POWER_MANAGEMENT_CONTROL]
	ldr	r1, emif_pmcr_shdw_val
	str	r1, [r0, #EMIF_POWER_MANAGEMENT_CTRL_SHDW]

	/*
	 * Output impedence calib needed only for DDR3
	 * but since the initial state of this will be
	 * disabled for DDR2 no harm in restoring the
	 * old configuration
	 */
	ldr	r1, emif_zqcfg_val
	str	r1, [r0, #EMIF_SDRAM_OUTPUT_IMPEDANCE_CALIBRATION_CONFIG]

	/* Write to SDRAM_CONFIG only for DDR2 */
	ldr	r2, mem_type
	cmp	r2, #MEM_TYPE_DDR2
	bne	resume_to_ddr

 	/*
	 * A write to SDRAM CONFIG register triggers
	 * an init sequence and hence it must be done
	 * at the end for DDR2
	 */
	ldr	r1, emif_sdcfg_val
	str	r1, [r0, #EMIF_SDRAM_CONFIG]

	/* TODO: Poll for CKE in AM437x */
resume_to_ddr:
	/* Back from la-la-land. Kill some time for sanity to settle in */
	mov	r0, #0x1000
wait_resume:
	subs	r0, r0, #1
	bne	wait_resume

	ldr	r0, scratchpad
	mov	r1, #0x12
	str	r1, [r0]

	ldr	r0, cpu_id
	cmp	r0, #0x1
	beq	skip_l2en

#ifdef CONFIG_CACHE_L2X0
	ldr	r2, l2_cache_base
	ldr	r0, [r2, #L2X0_CTRL]
	and	r0, #0x0f
	cmp	r0, #1
	beq	skip_l2en			@ Skip if already enabled
	ldr     r0, l2_prefetch_ctrl_val

	ldr	r12, l2_smc1
	dsb
	smc	#0
	dsb
set_aux_ctrl:
	ldr	r0, l2_aux_ctrl_val
	ldr	r12, l2_smc2
	dsb
	smc	#0
	dsb

	/* L2 invalidate on resume */
	ldr	r0, l2_val
	ldr	r2, l2_cache_base
	str	r0, [r2, #L2X0_INV_WAY]
wait2:
	ldr	r0, [r2, #L2X0_INV_WAY]
	ldr	r1, l2_val
	ands	r0, r0, r1
	bne	wait2
#ifdef CONFIG_PL310_ERRATA_727915
	mov	r0, #0x00
	mov	r12, #OMAP4_MON_L2X0_DBG_CTRL_INDEX
	dsb
	smc	#0
	dsb
#endif
l2x_sync2:
	ldr	r2, l2_cache_base
	mov	r0, #0x0
	str	r0, [r2, #L2X0_CACHE_SYNC]
sync2:
	ldr	r0, [r2, #L2X0_CACHE_SYNC]
	ands	r0, r0, #0x1
	bne	sync2

	mov	r0, #0x1
	ldr	r12, l2_smc3
	dsb
	smc	#0
	dsb
#endif
skip_l2en:

	ldr	r0, scratchpad
	mov	r1, #0x13
	str	r1, [r0]

	/* We are back. Branch to the common CPU resume routine */
	mov	r0, #0
	ldr	pc, resume_addr
ENDPROC(am33xx_resume_from_deep_sleep)


/*
 * Local variables
 */
	.align
resume_addr:
	.word	cpu_resume - PAGE_OFFSET + 0x80000000
kernel_flush:
	.word   v7_flush_dcache_all
emif_phys_addr:
	.word	AM33XX_EMIF_BASE

am33xx_virt_mpu_clkstctrl:
	.word	AM33XX_CM_MPU_CLKSTCTRL
am33xx_virt_mpu_clkctrl:
	.word	AM33XX_CM_MPU_MPU_CLKCTRL
am33xx_virt_emif_clkctrl:
	.word	AM33XX_CM_PER_EMIF_CLKCTRL
am33xx_phys_emif_clkctrl:
	.word	(AM33XX_CM_BASE + AM33XX_CM_PER_MOD + \
		AM33XX_CM_PER_EMIF_CLKCTRL_OFFSET)

am43xx_virt_mpu_clkstctrl:
	.word	AM43XX_CM_MPU_CLKSTCTRL
am43xx_virt_mpu_clkctrl:
	.word	AM43XX_CM_MPU_MPU_CLKCTRL
am43xx_virt_emif_clkctrl:
	.word	AM43XX_CM_PER_EMIF_CLKCTRL
am43xx_phys_emif_clkctrl:
	.word	(AM43XX_CM_BASE + AM43XX_CM_PER_INST + \
		AM43XX_CM_PER_EMIF_CLKCTRL_OFFSET)

scratchpad:
	.word	0x44e11344
scratchpad_virt:
	.word	AM33XX_L4_WK_IO_ADDRESS(0x44e11344)
module_disabled_val:
	.word	0x30000

/* L2 cache related defines for AM437x */
l2_aux_ctrl_val:
	.word	0xDEADBEEF
l2_prefetch_ctrl_val:
	.word	0xDEADBEEF
l2_cache_base:
	.word	OMAP44XX_L2CACHE_BASE
l2_val:
	.word	0xffff
l2_smc1:
	.word	OMAP4_MON_L2X0_PREFETCH_INDEX
l2_smc2:
	.word	OMAP4_MON_L2X0_AUXCTRL_INDEX
l2_smc3:
	.word	OMAP4_MON_L2X0_CTRL_INDEX

/* DDR related defines */
dram_sync_word:
	.word	0xDEADBEEF
mem_type:
	.word	0xDEADBEEF
l2_base_virt:
	.word	0xDEADBEEF
cpu_id:
	.word	0xDEADBEEF
emif_addr_virt:
	.word	0xDEADBEEF
emif_rd_lat_val:
	.word	0xDEADBEEF
emif_timing1_val:
	.word	0xDEADBEEF
emif_timing2_val:
	.word	0xDEADBEEF
emif_timing3_val:
	.word	0xDEADBEEF
emif_sdcfg_val:
	.word	0xDEADBEEF
emif_ref_ctrl_val:
	.word	0xDEADBEEF
emif_zqcfg_val:
	.word	0xDEADBEEF
emif_pmcr_val:
	.word	0xDEADBEEF
emif_pmcr_shdw_val:
	.word	0xDEADBEEF

	.align 3
ENTRY(am33xx_do_wfi_sz)
	.word	. - am33xx_do_wfi
