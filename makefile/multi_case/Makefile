#-----------------------------------------------------------
# makefile test
#-----------------------------------------------------------
inc_run_cases := add sub sll sii xor
case := add
run_cases := $(inc_run_cases)
pre_run := install com sim

ifneq ($(case), alls)
run_cases = $(case)
endif

all: build
	@echo "there is all"

define execute-command =
$(1) :
	@echo "there is install:$(1)"
	@echo "there is com"
	@echo "there is sim"
	@echo "this is a test:$(1)"
endef

$(foreach prog, $(run_cases), $(eval $(call execute-command, $(prog))))

run: $(run_cases)
	@echo "there is run $(run_cases)"
	@cat inst_test_report.log

install:
	@echo "there is install $(case)"

sim:
	@echo "there is sim"

com:
	@echo "there is com"

build:
	@echo "there is build"

clean:
	@echo "there is clean"

#-----------------------------------------------------------
