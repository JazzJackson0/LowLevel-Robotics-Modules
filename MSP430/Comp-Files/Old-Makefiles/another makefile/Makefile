OBJECTS=main.o
MAP=main.map
MAKEFILE=Makefile

ifeq ($(OS),Windows_NT)
	ifeq ($(shell uname -o),Cygwin)
		RM= rm -rf
	else
		RM= del /q
	endif
else
	RM= rm -rf
endif

GCC_DIR = $(abspath $(dir $(lastword $(MAKEFILE)))/../../bin)
SUPPORT_FILE_DIRECTORY = $(abspath $(dir $(lastword $(MAKEFILE)))/../../include)

DEVICE  = MSP430FR5969
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb

CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -mlarge -mcode-region=either -mdata-region=lower -Og -Wall -g
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY) -Wl,-Map,$(MAP),--gc-sections 

all: ${OBJECTS}
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $(DEVICE).out

clean: 
	$(RM) $(OBJECTS)
	$(RM) $(MAP)
	$(RM) *.out

debug: all
	$(GDB) $(DEVICE).out

might need to use mspdebug for loading to board? Not sure. Haven't tried this yet.