# Makefile for icestorm tools + yosys + nextpnr
# Modified from examples in icestorm distribution
# 01-16-18 E. Brombaugh

# src directory
VPATH = ../src:../riscv/src:../riscv/picorv32

SRC =	up5k_osc_top.v ../src/osc2.v ../src/adc.v ../src/pwm.v \
		../src/expo_calc.v ../src/sine.v ../src/pdm.v ../src/pulse_wave.v \
		../src/toggle3pos.v ../src/diag_serial.v ../riscv/src/acia_tx.v \
		../riscv/src/system.v ../riscv/src/acia.v \
		../riscv/src/acia_rx.v ../riscv/src/bram_512x32.v \
		../riscv/src/spram_16kx32.v ../riscv/picorv32/picorv32.v
		
# preparing the machine code
FAKE_HEX =	rom.hex
REAL_HEX =  code.hex

# project stuff
PROJ = up5k_osc_top
PIN_DEF = up5k_osc_top.pcf
SDC = up5k_osc_top.sdc
DEVICE = up5k
PACKAGE = sg48

TOOLS = /opt/openfpga/fpga-toolchain
YOSYS = $(TOOLS)/bin/yosys
YOSYS_SYNTH_ARGS = -dsp -relut -dffe_min_ce_use 4
NEXTPNR = $(TOOLS)/bin/nextpnr-ice40
NEXTPNR_ARGS = --pre-pack $(SDC) --placer heap
ICEPACK = $(TOOLS)/bin/icepack
ICETIME = $(TOOLS)/bin/icetime
ICEPROG = $(TOOLS)/bin/iceprog
ICEBRAM = $(TOOLS)/bin/icebram
CDCPROG = cdc_prog
DFU_UTIL = dfu-util
VERILATOR = verilator
TECH_LIB = $(TOOLS)/share/yosys/ice40/cells_sim.v

all: $(PROJ).bin

$(FAKE_HEX):
	$(ICEBRAM) -g 32 2048 > $(FAKE_HEX)

%.json: $(SRC) $(FAKE_HEX)
	$(YOSYS) -p 'synth_ice40 $(YOSYS_SYNTH_ARGS) -top $(PROJ) -json $@' $(SRC)

%.asc: %.json $(PIN_DEF) 
	$(NEXTPNR) $(NEXTPNR_ARGS) --$(DEVICE) --package $(PACKAGE) --json $< \
	--pcf $(PIN_DEF) --asc $@

$(REAL_HEX):
	$(MAKE) -C ../riscv/c/ main.hex
	cp ../riscv/c/main.hex ./$(REAL_HEX)
		
%.bin: %.asc $(REAL_HEX)
	$(ICEBRAM) $(FAKE_HEX) $(REAL_HEX) < $< > temp.asc
	$(ICEPACK) temp.asc $@

%.rpt: %.asc
	$(ICETIME) -d $(DEVICE) -mtr $@ $<

prog: $(PROJ).bin
	$(CDCPROG) -p /dev/ttyACM0 $<

flash: $(PROJ).bin
	$(CDCPROG) -w -p /dev/ttyACM0 $<

dfu: $(PROJ).bin
	sudo $(DFU_UTIL) dfu-util -d1d50:6146 -a0 -D $<

recode:
	rm -f $(REAL_HEX) $(PROJ).bin
	$(MAKE) prog
    
waves: wave_bank.bin
	$(CDCPROG) -a 0x200000 -w -p /dev/ttyACM0 $<

lint: $(SRC)
	$(VERILATOR) --lint-only -Wall --top-module $(PROJ) $(TECH_LIB) $(SRC)

clean:
	rm -f *.json *.asc *.rpt *.bin *.hex

.SECONDARY:
.PHONY: all prog clean
