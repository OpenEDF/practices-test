module tb;
integer fd;
parameter string str = "add";
initial begin
	fd = $fopen("mhome_inst_test.rpt", "a+b");
	$fdisplay(fd, "mhome inst test: %s ... OK", str);
	$fclose(fd);
end

endmodule
