# gen_sine.py - generate sine lookup table
# 03-20-21 E. Brombaugh

import math as math
import numpy as np

# build the sine LUT
LUT_scale = 256
n = 6.2832 * np.arange(LUT_scale) / LUT_scale
LUT = np.floor(32767 * np.sin(n) + 0.5)

# write the table as hex
hexfile = open('sine.hex', 'w')
for val in LUT:
    sval = "%x\n" % (int(val) & 65535)
    hexfile.write(sval)
hexfile.close()

