#-----------------------------------------------------------
# makefile test
#-----------------------------------------------------------
include tb/Makefile.mk

case := add
run_cases := $(inc_run_cases)
pre_run := install com sim 

ifneq ($(case), alls)
run_cases = $(case)
endif

all: build
	@echo "there is all"

run: $(run_cases)
	@echo "there is run"
	@echo "there is run $(run_cases)"

include case/Makefile.mk

install:
	@echo "there is install"
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
