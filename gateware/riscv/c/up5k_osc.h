/*
 * up5k_osc.h - hardware definitions for up5k_riscv
 * 04-26-21 E. Brombaugh
 */

#ifndef __up5k_osc__
#define __up5k_osc__

#include <stdint.h>

// 32-bit parallel in / out
#define gp_out (*(volatile uint32_t *)0x20000000)
#define gp_in0 (*(volatile uint32_t *)0x20000000)
#define gp_in1 (*(volatile uint32_t *)0x20000004)
#define gp_in2 (*(volatile uint32_t *)0x20000008)
#define gp_in3 (*(volatile uint32_t *)0x2000000C)

// 32-bit clock counter
#define clkcnt_reg (*(volatile uint32_t *)0x50000000)

// ACIA serial
#define acia_ctlstat (*(volatile uint8_t *)0x30000000)
#define acia_data (*(volatile uint8_t *)0x30000004)

#endif