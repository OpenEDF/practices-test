/* Quartus Prime Version 18.0.0 Build 614 04/24/2018 SJ Standard Edition */
JedecChain;
	FileRevision(JESD32A);
	DefaultMfr(6E);

	P ActionCode(Cfg)
		Device PartName(10AX115N3F45) Path("./") File("hawk_top.sof") MfrSpec(OpMask(1));
	P ActionCode(Ign)
		Device PartName(5M2210Z) MfrSpec(OpMask(0) SEC_Device(CFI_2GB) Child_OpMask(1 0));

ChainEnd;

AlteraBegin;
	ChainType(JTAG);
AlteraEnd;
