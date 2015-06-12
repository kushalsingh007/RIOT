# name of the application
APPLICATION = test

# Current app directory
APPDIR ?= $(CURDIR)
APPDIR := $(abspath $(APPDIR))/

# If no BOARD is found in the environment, use this default:
export BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
export RIOTBASE ?= $(CURDIR)/../..

export RIOTBOARD ?= $(RIOTBASE)/boards
export RIOTBOARD := $(abspath $(RIOTBOARD))

export BINDIRBASE ?= $(APPDIR)/bin
export BINDIRBASE := $(abspath $(BINDIRBASE))

export BINDIR ?= $(BINDIRBASE)/$(BOARD)
export BINDIR := $(abspath $(BINDIR))/

export ELFFILE ?= $(BINDIR)$(APPLICATION).elf
export HEXFILE ?= $(ELFFILE:.elf=.hex)

# This has to be the path to the linker script containing folder
export LINKERBASE ?= $(RIOTBASE)/../../../linker
export LINKERBASE := $(abspath $(LINKERBASE))

# CPU Architecture used
export CPU_ARCH = cortex-m0plus

# Use TOOLCHAIN environment variable to select the toolchain to use.
# Default: gnu
TOOLCHAIN ?= gnu

# Import all toolchain settings
include $(RIOTBOARD)/Makefile.include.$(TOOLCHAIN)

# Import all openocd configuration
include $(RIOTBOARD)/Makefile.include.openocd

all: bin

bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin
	xxd -i test.bin > test.h
	arm-none-eabi-objcopy -O ihex test.elf test.hex
	mv test.bin $(CURDIR)/bin/$(BOARD)/
	mv test.hex $(CURDIR)/bin/$(BOARD)/
	mv test.h $(CURDIR)/bin/$(BOARD)/
	mv test.elf $(CURDIR)/bin/$(BOARD)/

flash : all
	command -v $(FLASHER) >/dev/null 2>&1 || \
		{ \
		'Flash program $(FLASHER) not found. Aborting'; \
		exit 1; }
	$(FLASHER) $(FFLAGS)


debug :
	command -v $(DEBUGGER) >/dev/null 2>&1 || \
		{ \
		'Debug program $(DEBUGGER) not found. Aborting'; \
	   	exit 1; }
	$(DEBUGGER) $(DEBUGGER_FLAGS)

test.elf : test.o
	arm-none-eabi-ld -T $(LINKERBASE)/kcortexm_base.ld -Map=test.map test.o -o test.elf
	mv test.o $(CURDIR)/bin/$(BOARD)/
	mv test.map $(CURDIR)/bin/$(BOARD)/

test.o : test.c
	mkdir -p $(CURDIR)/bin/$(BOARD)
	arm-none-eabi-gcc --specs=nosys.specs  -c -ggdb -g3 -fPIC test.c

clean:
	rm -f $(CURDIR)/bin/$(BOARD)/test.*
