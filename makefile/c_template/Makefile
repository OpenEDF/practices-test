include test1/Makefile.mk
include test2/Makefile.mk

SRC = $(wildcard test1/*.c) \
	  $(wildcard test2/*.c) \
	  $(wildcard *.c)

obj += main.o

build: $(obj)
	@echo "this is a test!\n"
	@echo $(obj)
	@echo $(SRC)

%.o : test1/%.c
	gcc -c $< -o $@

%.o : test2/%.c
	gcc -c $< -o $@

clean:
	rm -rf *.o
