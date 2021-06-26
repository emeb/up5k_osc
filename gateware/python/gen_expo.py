# gen_expo.py - generate exponential lookup table and constants
# 03-20-21 E. Brombaugh

import math as math
import numpy as np

# VCO constants 
Fsamp = 16e6 # DAC sample rate 
DDS_bits = 32
DDS_scale = 2**DDS_bits
Fmin = 10*8 # to max out resolution 

# LUT constants 
LUT_bits = 8
LUT_scale = 2**LUT_bits

# ADC constants 
ADC_bits = 12
ADC_scale = 2**ADC_bits
ADC_vref = 3.3
ADC_calbits = 16
ADC_calscale = 2**ADC_calbits

# Compute value of Fmin scaled to DDS 
Fmin_scale = DDS_scale * Fmin / Fsamp

# Compute ADC calibration constant, shift value &  address mask 
f_adc_cal = LUT_scale/math.floor(ADC_scale*0.3311/ADC_vref)
ADC_cal = math.floor(math.floor(f_adc_cal*ADC_calscale)/(2**(LUT_bits-8)))
#ADC_cal = 40916;

# build the LUT
n = np.arange(LUT_scale)/LUT_scale
y = 2**n
LUT = np.floor(Fmin_scale*y + 0.5)

# write the table as hex
hexfile = open('expo.hex', 'w')
for val in LUT:
    sval = "%x\n" % int(val)
    hexfile.write(sval)
hexfile.close()

# write the constants needed
incfile = open('expo.vinc', 'w')
incfile.write("`define ADC_cal %d\n" % ADC_cal)
incfile.write("`define ADC_shf %d\n" % (8-LUT_bits))
incfile.write("`define ADC_msk %d'h%2x\n" % (LUT_bits, (LUT_scale-1)))
incfile.close()