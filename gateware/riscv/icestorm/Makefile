# Makefile for icestorm tools + yosys + nextpnr
# Modified from examples in icestorm distribution
# 01-16-18 E. Brombaugh

# src directory
VPATH = ../src:../picorv32

SRC =	up5k_osc_top.v \
		../src/system.v ../src/acia.v ../src/acia_tx.v \
		../src/acia_rx.v ../src/bram_512x32.v \
		../src/spram_16kx32.v ../picorv32/picorv32.v
		
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
	$(MAKE) -C ../c/ main.hex
	cp ../c/main.hex ./$(REAL_HEX)
		
%.bin: %.asc $(REAL_HEX)
	$(ICEBRAM) $(FAKE_HEX) $(REAL_HEX) < $< > temp.asc
	$(ICEPACK) temp.asc $@

%.rpt: %.asc
	$(ICETIME) -d $(DEVICE) -mtr $@ $<

prog: $(PROJ).bin
	$(CDCPROG) -p /dev/ttyACM0 $<

recode:
	rm -f $(REAL_HEX) $(PROJ).bin
	$(MAKE) prog
    
flash: $(PROJ).bin
	$(CDCPROG) -w -p /dev/ttyACM0 $<

waves: wave_bank.bin
	$(CDCPROG) -a 0x200000 -w -p /dev/ttyACM0 $<

lint: $(SRC)
	$(VERILATOR) --lint-only -Wall --top-module $(PROJ) $(TECH_LIB) $(SRC)
	
test.json: $(SRC)
	$(YOSYS) -p 'synth_ice40 $(YOSYS_SYNTH_ARGS) -top system -json $@' $(SRC) > log.txt 2>&1

system_struct.v: $(SRC) $(REAL_HEX)
	mv $(REAL_HEX) $(FAKE_HEX)
	$(YOSYS) -p 'synth_ice40 $(YOSYS_SYNTH_ARGS) -top system ; write_verilog system_struct.v' $(SRC)

clean:
	$(MAKE) -C ../c/ clean
	rm -f *.json *.asc *.rpt *.bin *.hex

.SECONDARY:
.PHONY: all prog clean
