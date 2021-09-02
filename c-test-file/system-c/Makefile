# MAKE SYSTEM C

SYSTEMC_HOME ?= /usr/local/system233
TARGET = hello
SRCOUSER = hello.cpp

# Set variable
SYSTEMC_FLAG = -I. -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux -Wl,-rpath=$(SYSTEMC_HOME)/lib-linux
SYSTEMC_LM = -lsystemc -lm
SYSTEMC_CXX = g++

# Compiler and build
all: build

build: $(SRCOUSER)
	$(SYSTEMC_CXX) $(SYSTEMC_FLAG) -o $(TARGET) $(SRCOUSER) $(SYSTEMC_LM)

run:
	./$(TARGET)
.PHONY:clean
clean:
	rm -rf $(TARGET)
