#=======================================================================
# Makefrag for rv32ui tests
#-----------------------------------------------------------------------
rv32ui_sc_tests = \
	simple \
	add addi \
	and andi \
	auipc \
	beq bge bgeu blt bltu bne \
	fence_i \
	jal jalr \
	lb lbu lh lhu lw \
	lui \
	ma_data \
	or ori \
	sb sh sw \
	sll slli \
	slt slti sltiu sltu \
	sra srai \
	srl srli \
	sub \
	xor xori \

rv32ui_p_tests = $(addprefix rv32ui-p-, $(rv32ui_sc_tests))
rv32ui_v_tests = $(addprefix rv32ui-v-, $(rv32ui_sc_tests))
rv32ui_p_testss = $(addsuffix .verilog, $(rv32ui_p_tests))
rv32ui_v_testss = $(addsuffix .verilog, $(rv32ui_v_tests))
build:
	@echo $(rv32ui_p_tests)
	@echo $(rv32ui_v_tests)
	@echo $(rv32ui_p_testss)
	@echo $(rv32ui_v_testss)

#-----------------------------------------------------------------------
