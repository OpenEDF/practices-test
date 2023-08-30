module tb;
integer fd;
initial begin
	fd = $fopen("mhome_inst_test.rpt", "a+b");
	$fdisplay(fd, "mhome inst test: %s ... OK", `INST_NAME);
	$fclose(fd);
end

endmodule

// iverilog -D INST_NAME=\"jar\" -o test tb.v
// iverilog -D INST_NAME=\"add\" -o test tb.v
//
// vpp test
// cat mhome_inst_test.rpt
