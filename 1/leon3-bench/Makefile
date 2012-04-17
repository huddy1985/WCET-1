# (c) 2011, Benedikt Huber, benedikt@vmars.tuwien.ac.at

# You need to have sparc-elf-gcc in PATH and $SPARC_ELF_INCLUDE set

MYLIB=bench.o
CC=sparc-elf-gcc
CFLAGS += -g -O1 -msoft-float -mv8 -std=gnu99 -Wall -fgnu89-inline

all: demo.elf $(MYLIB)

demo.elf: demo.o $(MYLIB)
	sparc-elf-gcc $(LDFLAGS) -o $@ $^

.PHONY:clean
clean:
	rm -f demo.elf *.o *.i
