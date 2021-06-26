# Makefile for Icarus Verilog simulation
# 03-19-2021 E. Brombaugh

# sources
SOURCES = 	tb_osc2.v ../src/osc2.v ../src/pwm.v ../src/adc.v \
			../src/sine.v ../src/expo_calc.v ../src/pdm.v ../src/pulse_wave.v
        
# top level
TOP = tb_osc2
			
# Executables
TOOLS = /opt/openfpga/fpga-toolchain
VLOG = iverilog
WAVE = gtkwave
TECH_LIB = $(TOOLS)/share/yosys/ice40/cells_sim.v

# targets
all: $(TOP).vcd

wave: $(TOP).vcd $(TOP).gtkw
	$(WAVE) $(TOP).gtkw
	
$(TOP).vcd: $(TOP)
	./$(TOP)

$(TOP): $(SOURCES)
	$(VLOG) -D icarus -DNO_ICE40_DEFAULT_ASSIGNMENTS -l $(TECH_LIB) -o $(TOP) $(SOURCES)
	
clean:
	rm -rf a.out *.obj $(TOP) $(TOP).vcd
	
