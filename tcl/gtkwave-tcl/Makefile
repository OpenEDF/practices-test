# verilator makefile

VCD_SOURCE = testbench.vcd
GTKWAVE_TCL = gtkwave.tcl

# tools
GTKWAVE = gtkwave

# use script configure gtkwave
GTKWAVE_FLAG += -S

display:$(GTKWAVE_TCL)
	@echo
	@echo "-- DISPLAY -----------------"
	$(GTKWAVE) $(GTKWAVE_FLAG) $(GTKWAVE_TCL) $(VCD_SOURCE) &

.PHONY:clean
clean:
	@echo
	@echo "-- CLEAN -------------------"	
	rm -rf *.vcd.pdf
