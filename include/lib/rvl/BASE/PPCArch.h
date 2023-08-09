#ifndef RVL_SDK_PPC_ARCH_H
#define RVL_SDK_PPC_ARCH_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

u32 PPCMfmsr(void);
void PPCMtmsr(u32 val);

u32 PPCMfhid0(void);
void PPCMthid0(u32 val);

u32 PPCMfl2cr(void);
void PPCMtl2cr(u32 val);

void PPCMtdec(u32 val);

void PPCSync(void);
void PPCHalt(void);

void PPCMtmmcr0(u32 val);
void PPCMtmmcr1(u32 val);

void PPCMtpmc1(u32 val);
void PPCMtpmc2(u32 val);
void PPCMtpmc3(u32 val);
void PPCMtpmc4(u32 val);

u32 PPCMffpscr(void);
void PPCMtfpscr(u32 val);

u32 PPCMfhid2(void);
void PPCMthid2(u32 val);

u32 PPCMfwpar(void);
void PPCMtwpar(u32 val);

void PPCDisableSpeculation(void);
void PPCSetFpNonIEEEMode(void);

void PPCMthid4(u32 val);

#ifdef __cplusplus
}
#endif
#endif

/**
 * MSR
 * Machine Status Register
 */
// Power management enable
#define MSR_POW (1 << (31 - 13))
// Exception little-endian mode
#define MSR_ILE (1 << (31 - 15))
// External interrupt enable
#define MSR_EE (1 << (31 - 16))
// Privilege level
#define MSR_PR (1 << (31 - 17))
// Floating-point available
#define MSR_FP (1 << (31 - 18))
// Machine check enable
#define MSR_ME (1 << (31 - 19))
// IEEE floating-point exception mode 0
#define MSR_FE0 (1 << (31 - 20))
// Single-step trace enable
#define MSR_SE (1 << (31 - 21))
// Branch trace enable
#define MSR_BE (1 << (31 - 22))
// IEEE floating-point exception mode 1
#define MSR_FE1 (1 << (31 - 23))
// Exception prefix
#define MSR_IP (1 << (31 - 25))
// Instruction address translation
#define MSR_IR (1 << (31 - 26))
// Data address translation
#define MSR_DR (1 << (31 - 27))
// Performance monitor marked mode
#define MSR_PM (1 << (31 - 29))
// Indicates whether system reset or machine check exception is recoverable
#define MSR_RI (1 << (31 - 30))
// Little-endian mode enable
#define MSR_LE (1 << (31 - 31))

/**
 * HID0
 * Hardware Implementation-Dependent Register 0
 */
// Enable MCP
#define HID0_EMCP (1 << (31 - 0))
// Enable/disable 60x bus address and data parity generation
#define HID0_DBP (1 << (31 - 1))
// Enable/disable 60x bus address parity checking
#define HID0_EBA (1 << (31 - 2))
// Enable 60x bus data parity checking
#define HID0_EBD (1 << (31 - 3))
// CLK_OUT enable
#define HID0_BCLK (1 << (31 - 4))
// CLK_OUT enable
#define HID0_ECLK (1 << (31 - 6))
// Disable precharge of ARTRY
#define HID0_PAR (1 << (31 - 7))
// Doze mode enable
#define HID0_DOZE (1 << (31 - 8))
// Nap mode enable
#define HID0_NAP (1 << (31 - 9))
// Sleep mode enable
#define HID0_SLEEP (1 << (31 - 10))
// Dynamic power management enable
#define HID0_DPM (1 << (31 - 11))
// Not hard reset (software-use only)
#define HID0_NHR (1 << (31 - 15))
// Instruction cache enable
#define HID0_ICE (1 << (31 - 16))
// Data cache enable
#define HID0_DCE (1 << (31 - 17))
// Instruction cache lock
#define HID0_ILOCK (1 << (31 - 18))
// Data cache lock
#define HID0_DLOCK (1 << (31 - 19))
// Instruction cache flash invalidate
#define HID0_ICFI (1 << (31 - 20))
// Data cache flash invalidate
#define HID0_DCFI (1 << (31 - 21))
// Speculative cache access disable
#define HID0_SPD (1 << (31 - 22))
// Enable M bit on bus for instruction fetches
#define HID0_IFEM (1 << (31 - 23))
// Store gathering enable
#define HID0_SGE (1 << (31 - 24))
// Data cache flush assist
#define HID0_DCFA (1 << (31 - 25))
// Branch Target Instruction Cache enable
#define HID0_BTIC (1 << (31 - 26))
// Address broadcast enable
#define HID0_ABE (1 << (31 - 28))
// Branch history table enable
#define HID0_BHT (1 << (31 - 29))
// No-op the data cache touch instructions
#define HID0_NOOPTI (1 << (31 - 31))

/**
 * HID1
 * Hardware Implementation-Dependent Register 1
 */
// PLL configuration bits (read-only)
#define HID1_PC0 (1 << (31 - 0))
#define HID1_PC1 (1 << (31 - 1))
#define HID1_PC2 (1 << (31 - 2))
#define HID1_PC3 (1 << (31 - 3))
#define HID1_PC4 (1 << (31 - 4))

/**
 * L2CR
 * L2 Control Register
 */
// L2 enable
#define L2CR_L2E (1 << (31 - 0))
// L2 Checkstop enable
#define L2CR_L2CE (1 << (31 - 1))
// L2 data-only
#define L2CR_L2DO (1 << (31 - 9))
// L2 global invalidate
#define L2CR_L2I (1 << (31 - 10))
// L2 write-through
#define L2CR_L2WT (1 << (31 - 12))
// L2 test support
#define L2CR_L2TS (1 << (31 - 13))
// L2 global invalidate in progress (read only)
#define L2CR_L2IP (1 << (31 - 31))

/**
 * PMC1-PMC4
 * Performance Monitor Counter Registers
 */
// Overflow
#define PMC_OV (1 << (31 - 0))

/**
 * FPSCR
 * Floating-Point Status and Control Register
 */
// Floating-point exception summary
#define FPSCR_FX (1 << (31 - 0))
// Floating-point enabled exception summary
#define FPSCR_FEX (1 << (31 - 1))
// Floating-point invalid operation exception summary
#define FPSCR_VX (1 << (31 - 2))
// Floating-point overflow exception
#define FPSCR_OX (1 << (31 - 3))
// Floating-point underflow exception
#define FPSCR_UX (1 << (31 - 4))
// Floating-point zero divide exception
#define FPSCR_ZX (1 << (31 - 5))
// Floating-point inexact exception
#define FPSCR_XX (1 << (31 - 6))
// Floating-point invalid operation exception for SNaN
#define FPSCR_VXSNAN (1 << (31 - 7))
// Floating-point invalid operation exception for (inf - inf)
#define FPSCR_VXISI (1 << (31 - 8))
// Floating-point invalid operation exception for (inf / inf)
#define FPSCR_VXIDI (1 << (31 - 9))
// Floating-point invalid operation exception for (0 / 0)
#define FPSCR_VXZDZ (1 << (31 - 10))
// Floating-point invalid operation exception for (inf * 0)
#define FPSCR_VXIMZ (1 << (31 - 11))
// Floating-point invalid operation exception for invalid compare
#define FPSCR_VXVC (1 << (31 - 12))
// Floating-point fraction rounded
#define FPSCR_FR (1 << (31 - 13))
// Floating-point fraction inexact
#define FPSCR_FI (1 << (31 - 14))
// Floating-point result flags
#define FPSCR_FPRF (0x1F << (31 - 19))
// Unknown
#define FPSCR_UNK20 (1 << (31 - 20))
// Floating-point invalid operation exception for software request
#define FPSCR_VXSOFT (1 << (31 - 21))
// Floating-point invalid operation exception for invalid square root
#define FPSCR_VXSQRT (1 << (31 - 22))
// Floating-point invalid operation exception for invalid integer convert
#define FPSCR_VXCVI (1 << (31 - 23))
// Floating-point invalid operation exception enable
#define FPSCR_VE (1 << (31 - 24))
// IEEE floating-point overflow exception enable
#define FPSCR_OE (1 << (31 - 25))
//  IEEE floating-point underflow exception enable
#define FPSCR_UE (1 << (31 - 26))
// IEEE floating-point zero divide exception enable
#define FPSCR_ZE (1 << (31 - 27))
// Floating-point inexact exception enable
#define FPSCR_XE (1 << (31 - 28))
// Floating-point non-IEEE mode
#define FPSCR_NI (1 << (31 - 29))
// Floating-point rounding control
#define FPSCR_RN (0x3 << (31 - 31))

/**
 * HID2
 * Hardware Implementation-Dependent Register 2
 */
// Write pipe enable
#define HID2_WPE (1 << (31 - 1))
// Paired-single enable
#define HID2_PSE (1 << (31 - 2))
// Locked cache enable
#define HID2_LCE (1 << (31 - 3))
// DMA queue length (read only)
#define HID2_DMAQL                                                             \
    ((1 << (31 - 4)) | (1 << (31 - 5)) | (1 << (31 - 6)) | (1 << (31 - 7)))
// dcbz_l cache hit error
#define HID2_DCHERR (1 << (31 - 8))
// DMA access to normal cache error
#define HID2_DNCERR (1 << (31 - 9))
// DMA cache miss error
#define HID2_DCMERR (1 << (31 - 10))
// DMA queue overflow error
#define HID2_DQOERR (1 << (31 - 11))
// dcbz_l cache hit error enable
#define HID2_DCHEE (1 << (31 - 12))
// DMA access to normal cache error enable
#define HID2_DNCEE (1 << (31 - 13))
// DMA cache miss error enable
#define HID2_DCMEE (1 << (31 - 14))
// DMA queue overflow error enable
#define HID2_DQOEE (1 << (31 - 15))

/**
 * WPAR
 * Write Pipe Address Register
 */
// High order address bits of the data to be gathered
#define WPAR_GB_ADDR (0x07FFFFFF << (31 - 26))
// Buffer not empty (read only)
#define WPAR_BNE (1 << (31 - 31))

/**
 * DMAU
 * Direct Memory Access Upper Register
 */
// High order address bits of starting address in external memory of the DMA
// transfer
#define DMAU_MEM_ADDR (0x07FFFFFF << (31 - 26))
// High order bits of transfer length, in cache lines. Low order bits are in
// DMAL
#define DMAU_DMA_LEN_U (0x0000001F << (31 - 31))

/**
 * DMAL
 * Direct Memory Access Lower Register
 */
// High order address bits of starting address in locked cache of the DMA
// transfer
#define DMAL_LC_ADDR (0x07FFFFFF << (31 - 26))
// DMA load command
#define DMAL_DMA_LD (1 << (31 - 27))
// Low order bits of transfer length, in cache lines. High order bits are in
// DMAU
#define DMAL_DMA_LEN_L (0x3 << (31 - 29))
// Trigger bit
#define DMAL_DMA_T (1 << (31 - 30))
// Flush bit
#define DMAL_DMA_F (1 << (31 - 31))

/**
 * PVR
 * Processor Version Register
 */
// A 16-bit number that uniquely identifies a particular processor version
#define PVR_VER (0xFFFF0000)
// A 16-bit number that distinguishes between various releases of a particular
// version (that is, an engineering change level)
#define PVR_REV (0x0000FFFF)

/**
 * HID4
 * Hardware Implementation-Dependent Register 4
 */
// Unknown
#define HID4_H4A (1 << (31 - 0))
// L2 fetch mode
#define HID4_L2FM (0x3 << (31 - 2))
// Bus pipeline depth
#define HID4_BPD (0x3 << (31 - 4))
// L2 second castout buffer enable
#define HID4_BCO (1 << (31 - 5))
// Secondary BAT enable
#define HID4_SBE (1 << (31 - 6))
// Paired-singles control bit 1
#define HID4_PS1_CTL (1 << (31 - 7))
// Data bus parking
#define HID4_DBP (1 << (31 - 9))
// L2 MUM enable
#define HID4_L2MUM (1 << (31 - 10))
// L2CFI - L2 complete castout prior to L2 flash invalidate
#define HID4_L2_CCFI (1 << (31 - 11))
// Paired-singles control bit 2
#define HID4_PSS2_CTL (1 << (31 - 12))
