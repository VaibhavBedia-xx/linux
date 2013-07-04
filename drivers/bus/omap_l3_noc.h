/*
 * OMAP4XXX L3 Interconnect  error handling driver header
 *
 * Copyright (C) 2011 Texas Corporation
 *	Santosh Shilimkar <santosh.shilimkar@ti.com>
 *	sricharan <r.sricharan@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */
#ifndef __ARCH_ARM_MACH_OMAP2_L3_INTERCONNECT_3XXX_H
#define __ARCH_ARM_MACH_OMAP2_L3_INTERCONNECT_3XXX_H

//#define L3_MODULES			3
#define L3_MODULES			2
#define CLEAR_STDERR_LOG		(1 << 31)
#define CUSTOM_ERROR			0x2
#define STANDARD_ERROR			0x0
#define INBAND_ERROR			0x0
#define L3_APPLICATION_ERROR		0x0
#define L3_DEBUG_ERROR			0x1

/* L3 TARG register offsets */
#define L3_TARG_STDERRLOG_MAIN		0x48
#define L3_TARG_STDERRLOG_SLVOFSLSB	0x5c
#define L3_TARG_STDERRLOG_MSTADDR	0x68
#define L3_FLAGMUX_REGERR0		0xc

#define NUM_OF_L3_MASTERS	(sizeof(l3_masters)/sizeof(l3_masters[0]))

static u32 l3_flagmux[L3_MODULES] = {
#if 0
	0x500,
	0x1000,
	0X0200
#endif
	0x1000,
	0x0600,
};

/* L3 Target standard Error register offsets */
static u32 l3_targ_inst_clk1[] = {
#if 0
	0x100, /* DMM1 */
	0x200, /* DMM2 */
	0x300, /* ABE */
	0x400, /* L4CFG */
	0x600,  /* CLK2 PWR DISC */
	0x0,	/* Host CLK1 */
	0x900	/* L4 Wakeup */
#endif
#if 0	/* works for SA */
	0xf00, //emif_targ
	0xe00, //pcie_targ
	0x400, //ocmcram
	0x700, //tptc_targ1
	0x800, //tptc+targ2
	0x900, //tptc_targ3
	0xb00, //tpcc_targ
	0xd00, //debugss_targ
	0x600, //exp_targ
	0x200, //sha_targ
	0x300, //aes1_targ
	0x500, //aes0_targ
	0xa00, //l4fast_targ
	0xc00, //sgx530_targ
#endif
	0xf00, //emif_targ
	0x1200, //des_targ
	0x400, //ocmcram
	0x700, //tptc_targ1
	0x800, //tptc_targ2
	0x900, //tptc_targ3
	0xb00, //tpcc_targ
	0xd00, //debugss_targ
	0x600, //xxx reserved?
	0x200, //sha_targ
	0xc00, //sgx_targ
	0x500, //aes0_targ
	0xa00, //l4fast_targ
	0x300, //mpussl2ram_targ
	0x100, //icss_targ
};

static u32 l3_targ_inst_clk2[] = {
#if 0
	0x500, /* CORTEX M3 */
	0x300, /* DSS */
	0x100, /* GPMC */
	0x400, /* ISS */
	0x700, /* IVAHD */
	0xD00, /* missing in TRM  corresponds to AES1*/
	0x900, /* L4 PER0*/
	0x200, /* OCMRAM */
	0x100, /* missing in TRM corresponds to GPMC sERROR*/
	0x600, /* SGX */
	0x800, /* SL2 */
	0x1600, /* C2C */
	0x1100,	/* missing in TRM corresponds PWR DISC CLK1*/
	0xF00, /* missing in TRM corrsponds to SHA1*/
	0xE00, /* missing in TRM corresponds to AES2*/
	0xC00, /* L4 PER3 */
	0xA00, /* L4 PER1*/
	0xB00, /* L4 PER2*/
	0x0, /* HOST CLK2 */
	0x1800, /* CAL */
	0x1700 /* LLI */
#endif
#if 0	/* works for SA */
	0x100, //l4_targ1
	0x200, //l4_targ2
	0x300, //l4_targ3
	0x400, //l4_targ4
	0x800, //mcasp1_targ
	0x900, //mcasp2_targ
	0xa00, //mcasp3_targ
	0x700, //gpcm_targ
	0xe00, //l4fwcfg_targ
	0xb00, //usb_targ
	0x500, //adctsc_targ
	0xe00, //l4wkup_targ
	0xc00, //mmchs2_targ
#endif
	0x100, //l4_targ1
	0x200, //l4_targ2
	0x300, //l4_targ3
	0x400, //l4_targ4
	0x800, //mcasp0_targ
	0x900, //mcasp1_targ
	0xc00, //mmchs2_targ
	0x700, //gpcm_targ
	0xd00, //l4fwcfg_targ
	0xb00, //usb_targ
	0x500, //adctsc_targ
	0xe00, //l4wkup_targ
	0xa00, //magcard_targ
};

#if 0
static u32 l3_targ_inst_clk3[] = {
	0x0100	/* EMUSS */,
	0x0300, /* DEBUGSS_CT_TBR */
	0x0 /* HOST CLK3 */
};
#endif

static struct l3_masters_data {
	u32 id;
	char name[20];
} l3_masters[] = {
#if 0
	{ 0x0 , "MPU"},
	{ 0x10, "CS_ADP"},
	{ 0x14, "xxx"},
	{ 0x20, "DSP"},
	{ 0x30, "IVAHD"},
	{ 0x40, "ISS"},
	{ 0x44, "DucatiM3"},
	{ 0x48, "FaceDetect"},
	{ 0x50, "SDMA_Rd"},
	{ 0x54, "SDMA_Wr"},
	{ 0x58, "xxx"},
	{ 0x5C, "xxx"},
	{ 0x60, "SGX"},
	{ 0x70, "DSS"},
	{ 0x80, "C2C"},
	{ 0x88, "xxx"},
	{ 0x8C, "xxx"},
	{ 0x90, "HSI"},
	{ 0xA0, "MMC1"},
	{ 0xA4, "MMC2"},
	{ 0xA8, "MMC6"},
	{ 0xB0, "UNIPRO1"},
	{ 0xC0, "USBHOSTHS"},
	{ 0xC4, "USBOTGHS"},
	{ 0xC8, "USBHOSTFS"}
#endif
#if 0	/* works for SA */
	{ 0x00, "MPU_128b"},
	{ 0x01, "MPU_64b"},
	{ 0x04, "DAP"},
	{ 0x05, "P1500"},
	{ 0x08, "Rsvd"},
	{ 0x0C, "ICSS0"},
	{ 0x0D, "ICSS1"},
	{ 0x0E, "ICSS2"},
	{ 0x0F, "ICSS3"},
	{ 0x10, "Rsvd2"},
	{ 0x14, "Rsvd3"},
	{ 0x18, "TPTC0_Rd"},
	{ 0x19, "TPTC0_Wr"},
	{ 0x1A, "TPTC1_Rd"},
	{ 0x1B, "TPTC1_Wr"},
	{ 0x1C, "TPTC2_Rd"},
	{ 0x1D, "TPTC2_Wr"},
	{ 0x20, "SGX530"},
	{ 0x24, "LCDC"},
	{ 0x28, "Crypto_DMA_Rd"},
	{ 0x29, "Crypto_DMA_Wr"},
	{ 0x2C, "Rsvd4"},
	{ 0x30, "GEMAC"},
	{ 0x32, "WKUP_M3"},
	{ 0x34, "USB_DMA"},
	{ 0x35, "USB_QMGR"},
	{ 0x3A, "Rsvd5"},
#endif
	{ 0x00, "MPU_128b"},
	{ 0x01, "MPU_64b"},
	{ 0x04, "DAP"},
	{ 0x05, "P1500"},
	{ 0x08, "Rsvd"},
	{ 0x0C, "ICSS0"},
	{ 0x0D, "ICSS1"},
	{ 0x0E, "Rsvd2"},
	{ 0x0F, "Rsvd3"},
	{ 0x10, "Rsvd4"},
	{ 0x14, "Rsvd5"},
	{ 0x18, "TPTC0_Rd"},
	{ 0x19, "TPTC0_Wr"},
	{ 0x1A, "TPTC1_Rd"},
	{ 0x1B, "TPTC1_Wr"},
	{ 0x1C, "TPTC2_Rd"},
	{ 0x1D, "TPTC2_Wr"},
	{ 0x20, "SGX530"},
	{ 0x24, "Rsvd6"},
	{ 0x25, "DSS"},
	{ 0x28, "Crypto_DMA_Rd"},
	{ 0x29, "Crypto_DMA_Wr"},
	{ 0x2C, "VPFE0"},
	{ 0x2D, "VPFE1"},
	{ 0x30, "GEMAC"},
	{ 0x32, "WKUP_M3"},
	{ 0x34, "USB0_DMA"},
	{ 0x35, "USB0_QMGR"},
	{ 0x36, "USB1_DMA"},
	{ 0x37, "USB1_QMGR"},
};

static char *l3_targ_inst_name[L3_MODULES][21] = {
	{
#if 0
		"DMM1",
		"DMM2",
		"ABE",
		"L4CFG",
		"CLK2 PWR DISC",
		"HOST CLK1",
		"L4 WAKEUP"
#endif
#if 0	/* works for SA */
		"emif"
		"pcie"
		"ocmcram"
		"tptc1"
		"tptc2"
		"tptc3"
		"tpcc"
		"debugss"
		"exp"
		"sha"
		"aes1"
		"aes0"
		"l4fast"
		"sgx530"
#endif
		"EMIF",
		"DES",
		"OCMCRAM",
		"TPTC1",
		"TPTC2",
		"TPTC3",
		"TPCC",
		"DEBUGSS",
		"EXP",
		"SHA",
		"SGX",
		"AES0",
		"L4FAST",
		"MPUSSL3RAM",
		"ICSS",
	},
	{
#if 0
		"CORTEX M3" ,
		"DSS ",
		"GPMC ",
		"ISS ",
		"IVAHD ",
		"AES1",
		"L4 PER0",
		"OCMRAM ",
		"GPMC sERROR",
		"SGX ",
		"SL2 ",
		"C2C ",
		"PWR DISC CLK1",
		"SHA1",
		"AES2",
		"L4 PER3",
		"L4 PER1",
		"L4 PER2",
		"HOST CLK2",
		"CAL",
		"LLI"
#endif
#if 0	/* works for SA */
		"L41"
		"L42"
		"L43"
		"L44"
		"MCASP1"
		"MCASP2"
		"MCASP3"
		"GPMC"
		"L4FWCFG"
		"USB"
		"ADCTSC"
		"L4WKUP"
		"MMCHS2"
#endif
		"L41",
		"L42",
		"L43",
		"L44",
		"MCASP1",
		"MCASP2",
		"MCASP3",
		"GPMC",
		"L4FWCFG",
		"USB",
		"ADCTSC",
		"L4WKUP",
		"MMCHS2",
	},
#if 0
	{
		"EMUSS",
		"DEBUG SOURCE",
		"HOST CLK3"
	},
#endif
};

static u32 *l3_targ[L3_MODULES] = {
	l3_targ_inst_clk1,
	l3_targ_inst_clk2,
#if 0
	l3_targ_inst_clk3,
#endif
};

struct omap4_l3 {
	struct device *dev;
	struct clk *ick;

	/* memory base */
	void __iomem *l3_base[L3_MODULES];

	int debug_irq;
	int app_irq;
};
#endif
