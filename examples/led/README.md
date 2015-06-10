The current error and Work status 
==========

-Used the -fPIC flag while creating object files  
-Removed the static flag , changing it to shared is causing problem.  
-Created a new linker script ( simple version ), which causes some issues in startup code ( See below )  
-Not able to utilize the -R flag of ld. Problem in adding the flag to the linker script.  
-Had some issues about the use of archived libraries.

Creating a default(new) linker script causes the symbols used in previous
linker script not getting used anymore.  
This causes undefined symbol references in startup code.


Here are the errors currently faced --
```
command -v arm-none-eabi-gcc >/dev/null 2>&1 || \
        { /bin/echo -e \
        '\033[1;31m Compiler arm-none-eabi-gcc is required but not found in PATH.  Aborting.\033[0m'; \
        exit 1; }
Building application "led" for "samr21-xpro" with MCU "samd21".

DIRS="" "make" -C /home/kushal/code/riot/RIOT/examples/led -f /home/kushal/code/riot/RIOT/Makefile.application
make[1]: Entering directory '/home/kushal/code/riot/RIOT/examples/led'
"make" -C /home/kushal/code/riot/RIOT/boards/samr21-xpro
"make" -C /home/kushal/code/riot/RIOT/core
make[2]: Entering directory '/home/kushal/code/riot/RIOT/core'
make[2]: Leaving directory '/home/kushal/code/riot/RIOT/core'
"make" -C /home/kushal/code/riot/RIOT/cpu/samd21
make[2]: Entering directory '/home/kushal/code/riot/RIOT/cpu/samd21'
"make" -C /home/kushal/code/riot/RIOT/cpu/cortexm_common
make[3]: Entering directory '/home/kushal/code/riot/RIOT/cpu/cortexm_common'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/cpu/cortexm_common'
"make" -C /home/kushal/code/riot/RIOT/cpu/samd21/periph
make[3]: Entering directory '/home/kushal/code/riot/RIOT/cpu/samd21/periph'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/cpu/samd21/periph'
make[2]: Leaving directory '/home/kushal/code/riot/RIOT/cpu/samd21'
"make" -C /home/kushal/code/riot/RIOT/drivers
make[2]: Entering directory '/home/kushal/code/riot/RIOT/drivers'
"make" -C /home/kushal/code/riot/RIOT/drivers/at86rf231
make[3]: Entering directory '/home/kushal/code/riot/RIOT/drivers/at86rf231'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/drivers/at86rf231'
"make" -C /home/kushal/code/riot/RIOT/drivers/netdev/802154
"make" -C /home/kushal/code/riot/RIOT/drivers/netdev/base
make[2]: Leaving directory '/home/kushal/code/riot/RIOT/drivers'
"make" -C /home/kushal/code/riot/RIOT/sys
make[2]: Entering directory '/home/kushal/code/riot/RIOT/sys'
"make" -C /home/kushal/code/riot/RIOT/sys/auto_init
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/auto_init'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/auto_init'
"make" -C /home/kushal/code/riot/RIOT/sys/compat/hwtimer
"make" -C /home/kushal/code/riot/RIOT/sys/net/link_layer/ieee802154
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/net/link_layer/ieee802154'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/net/link_layer/ieee802154'
"make" -C /home/kushal/code/riot/RIOT/sys/newlib
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/newlib'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/newlib'
"make" -C /home/kushal/code/riot/RIOT/sys/posix
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/posix'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/posix'
"make" -C /home/kushal/code/riot/RIOT/sys/ps
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/ps'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/ps'
"make" -C /home/kushal/code/riot/RIOT/sys/shell
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/shell'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/shell'
"make" -C /home/kushal/code/riot/RIOT/sys/shell/commands
"make" -C /home/kushal/code/riot/RIOT/sys/timex
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/timex'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/timex'
"make" -C /home/kushal/code/riot/RIOT/sys/transceiver
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/transceiver'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/transceiver'
"make" -C /home/kushal/code/riot/RIOT/sys/uart0
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/uart0'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/uart0'
"make" -C /home/kushal/code/riot/RIOT/sys/vtimer
make[3]: Entering directory '/home/kushal/code/riot/RIOT/sys/vtimer'
make[3]: Leaving directory '/home/kushal/code/riot/RIOT/sys/vtimer'
make[2]: Leaving directory '/home/kushal/code/riot/RIOT/sys'
make[1]: Leaving directory '/home/kushal/code/riot/RIOT/examples/led'
arm-none-eabi-gcc /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/newlib/syscalls.o /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/cpu/startup.o -o /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/led.elf -Wl,--start-group /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/at86rf231.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/auto_init.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/core.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/cortexm_common.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/cpu.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/hwtimer_compat.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/ieee802154.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/netdev_802154.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/netdev_base.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/periph.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/posix.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/ps.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/shell.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/shell_commands.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/sys.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/timex.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/transceiver.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/uart0.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/vtimer.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/samr21-xpro_base.a /home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/led.a  -lm -Wl,--end-group  -Wl,-Map=/home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/led.map -L/home/kushal/code/riot/RIOT/cpu/samd21/ldscripts -L/home/kushal/code/riot/RIOT/cpu/cortexm_common/ldscripts -T/home/kushal/code/riot/RIOT/cpu/samd21/ldscripts/samr21g18a.ld -Wl,--fatal-warnings  -mcpu=cortex-m0plus -mlittle-endian -mthumb -mfloat-abi=soft -mno-thumb-interwork -std=gnu99 -Wall -Wstrict-prototypes -Werror=implicit-function-declaration -lgcc -nostartfiles -specs=nano.specs -lc -lnosys
/home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/newlib/syscalls.o: In function `_sbrk_r':
/home/kushal/code/riot/RIOT/sys/newlib/syscalls.c:139: undefined reference to `_eheap'
/home/kushal/code/riot/RIOT/sys/newlib/syscalls.c:139: undefined reference to `_sheap'
/home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/newlib/syscalls.o:(.data.rel.heap_top+0x0): undefined reference to `_sheap'
/home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/cpu/startup.o: In function `reset_handler':
/home/kushal/code/riot/RIOT/cpu/samd21/startup.c:76: undefined reference to `_srelocate'
/home/kushal/code/riot/RIOT/cpu/samd21/startup.c:76: undefined reference to `_erelocate'
/home/kushal/code/riot/RIOT/cpu/samd21/startup.c:76: undefined reference to `_etext'
/home/kushal/code/riot/RIOT/cpu/samd21/startup.c:76: undefined reference to `_szero'
/home/kushal/code/riot/RIOT/cpu/samd21/startup.c:76: undefined reference to `_ezero'
/home/kushal/code/riot/RIOT/examples/led/bin/samr21-xpro/cpu/startup.o:(.vectors+0x0): undefined reference to `_estack'
collect2: error: ld returned 1 exit status
/home/kushal/code/riot/RIOT/examples/led/../../Makefile.include:186: recipe for target 'all' failed
make: *** [all] Error 1

``` 
-- Suggestions ?
IRC - kushalsingh007
