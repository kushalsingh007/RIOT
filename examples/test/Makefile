# name of the application
APPLICATION = test

# If no BOARD is found in the environment, use this default:
BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

RIOTBOARD ?= $(RIOTBASE)/boards
RIOTBOARD := $(abspath $(RIOTBOARD))

# This has to be the absolute path to the linker script containing folder
LINKERBASE ?= $(HOME)/linker
LINKERBASE := $(abspath $(LINKERBASE))

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
	sudo openocd -f $(RIOTBOARD)/$(BOARD)/dist/openocd.cfg -c "program $(CURDIR)/bin/samr21-xpro/test.hex; reset"

test.elf : test.o
	arm-none-eabi-ld -T $(LINKERBASE)/kcortexm_base.ld -Map=test.map test.o -o test.elf
	mv test.o $(CURDIR)/bin/$(BOARD)/
	mv test.map $(CURDIR)/bin/$(BOARD)/

test.o : test.c
	mkdir -p $(CURDIR)/bin/$(BOARD)
	arm-none-eabi-gcc --specs=nosys.specs  -c -ggdb -g3 -fPIC test.c

clean:
	rm $(CURDIR)/bin/$(BOARD)/test.o
	rm $(CURDIR)/bin/$(BOARD)/test.bin
	rm $(CURDIR)/bin/$(BOARD)/test.h
	rm $(CURDIR)/bin/$(BOARD)/test.elf
	rm $(CURDIR)/bin/$(BOARD)/test.map
	rm $(CURDIR)/bin/$(BOARD)/test.hex
