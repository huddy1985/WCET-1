# (c) 2011, Benedikt Huber, benedikt@vmars.tuwien.ac.at

# You need to have sparc-elf-gcc in PATH and $SPARC_ELF_INCLUDE set

MYLIB=bench.o md5.o
CC=sparc-elf-gcc
CFLAGS += -g -Os -msoft-float -mv8 -std=gnu99 -Wall -fgnu89-inline

all: measure.elf $(MYLIB)

md5.elf: measure.o $(MYLIB)
	sparc-elf-gcc $(LDFLAGS) -o $@ $^

.PHONY:clean measure
clean:
	rm -f measure.elf *.o *.i
measure: 
	ls