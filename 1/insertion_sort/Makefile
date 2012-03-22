# (c) 2011, Benedikt Huber, benedikt@vmars.tuwien.ac.at

# You need to have sparc-elf-gcc in PATH and $SPARC_ELF_INCLUDE set

MYLIB=bench.o
CC=sparc-elf-gcc
CFLAGS += -g -Os -mv8 -msoft-float -std=gnu99 -Wall -fgnu89-inline

all: insertion_sort $(MYLIB)

insertion_sort: main.o insertion_sort.o $(MYLIB)
	sparc-elf-gcc $(LDFLAGS) -o $@ $^

.PHONY:clean
clean:
	rm -f insertion_sort *.o *.i
