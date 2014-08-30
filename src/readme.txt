MSP-EXP430G2 LaunchPad Serial Comm Demo
1. Compiler requirements
 - msp430gcc compiler should be installed
 - msp430gcc header files should be installed with their include path added to the OS environment.
2. Using provided makefile:
 - Build with: make 
 - Clean with: make clean
3. Debug with:
 - mspdebug rf2500 [The emulator & usb drivers for the LaunchPad are similar to the ones for the ez430-rf2500]
 - Use mspdebug to start a gdbproxy & use msp430-gdb to connect & debug
4. Requires ragel.
