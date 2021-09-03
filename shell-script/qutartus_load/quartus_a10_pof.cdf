/* Quartus Prime Version 18.0.0 Build 614 04/24/2018 SJ Standard Edition */
JedecChain;
	FileRevision(JESD32A);
	DefaultMfr(6E);

	P ActionCode(Ign)
		Device PartName(10AX115N3) MfrSpec(OpMask(0));
	P ActionCode(Ign)
		Device PartName(5M2210Z) MfrSpec(OpMask(0) SEC_Device(CFI_512MB) Child_OpMask(3 1 1 1) PFLPath("ready_pof_file.pof"));

ChainEnd;

AlteraBegin;
	ChainType(JTAG);
AlteraEnd;
