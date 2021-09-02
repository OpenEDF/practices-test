# verilator makefile

SOURCES=sc_main.cpp our.v
VXX=verilator
VXX_FLAG=-Wall --sc --exe
TEMP_FLODER=obj_dir
TARGET=Vour
all:$(SOURCES)
	$(VXX) $(VXX_FLAG) $(SOURCES)

build:
	make -j -C $(TEMP_FLODER) -f Vour.mk $(TARGET)

run:
	$(TEMP_FLODER)/$(TARGET)

.PHONY:clean
clean:
	rm -rf obj_dir
