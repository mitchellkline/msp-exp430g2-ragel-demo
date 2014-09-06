# makfile configuration
NAME            = main
OBJECTS         = main.o board.o strbuf.o putchar.o parser.o temp.o
CPU             = msp430g2553

CFLAGS          = -mmcu=${CPU} -Wall -O3 -g #-DDEBUG

#switch the compiler (for the internal make rules)
CC              = msp430-gcc

.PHONY: all FORCE clean download download-jtag download-bsl dist

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf ${NAME}.a43 ${NAME}.lst

#confgigure the next line if you want to use the serial download
download: download-jtag
#download: download-bsl

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

${NAME}.a43: ${NAME}.elf
	msp430-objcopy -O ihex $^ $@

${NAME}.lst: ${NAME}.elf
	msp430-objdump -dSt $^ >$@

download-jtag: all
	msp430-jtag -e ${NAME}.elf

download-bsl: all
	msp430-bsl -e ${NAME}.elf

clean:
	rm -f ${NAME}.elf ${NAME}.a43 ${NAME}.lst ${OBJECTS} parser.c

#backup archive
dist:
	tar czf dist.tgz *.c *.h *.txt makefile

flash: all
	mspdebug rf2500 erase
	mspdebug rf2500 'load main.elf'

#dummy target as dependecy if something has to be build everytime
FORCE:

#project dependencies
parser.c: parser.rl
	ragel -G2 -o $@ $<
main.o: main.c
board.o: board.c board.h
strbuf.o: strbuf.c strbuf.h
putchar.o: putchar.c
parser.o: parser.c parser.h
temp.o: temp.c temp.h
