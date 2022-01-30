#-----------------------------------------------------------
# Author: Macro
# Date:   2022-01-30
# Brief:  product initrd img file
#-----------------------------------------------------------
GCC  = gcc
FLAG = -static
SRC  = init.c
OUTPUT = init

all: com cpio

com:$(SRC)
	@$(GCC) $(FLAG) $(SRC) -o $(OUTPUT)
	@echo "    CC $(SRC) ..."

cpio:
	strip $(OUTPUT)
	find . | cpio -o --format=newc > initrd.img

.PHONY: clean
clean:
	rm -rf $(OUTPUT) initrd.img

#-----------------------------------------------------------
