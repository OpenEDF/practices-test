TEST1_SRC_DIR = test1
TEST2_SRC_DIR = test2
include $(TEST1_SRC_DIR)/Makefile.mk
include $(TEST2_SRC_DIR)/Makefile.mk

obj += main.o

build: $(obj)
	@echo $(obj)
	gcc -o output $(obj)

%.o : $(TEST1_SRC_DIR)/%.c
	gcc -c $< -o $@

%.o : $(TEST2_SRC_DIR)/%.c
	gcc -c $< -o $@

main.o : main.c
	gcc -c main.c -o main.o

clean:
	rm -rf *.o output
