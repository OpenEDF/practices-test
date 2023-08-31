case := add
all:
	iverilog -D INST_NAME=\"$(case)\" -o test tb.v
	vvp test
	@cat mhome_inst_test.rpt

clean:
	rm -rf mhome_inst_test.rpt test
