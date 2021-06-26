/*
 * clkcnt.h - clock cycle counter driver
 * 07-03-19 E. Brombaugh
 */

#ifndef __clkcnt__
#define __clkcnt__

#include "up5k_osc.h"

void clkcnt_wait(uint32_t clks);
void clkcnt_delayms(uint32_t ms);

#endif

