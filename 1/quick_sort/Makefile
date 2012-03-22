# (c) 2011, Benedikt Huber, benedikt@vmars.tuwien.ac.at

# You need to have sparc-elf-gcc in PATH and $SPARC_ELF_INCLUDE set

MYLIB=bench.o
MYOBJS=quick_sort.o test.o
CC=sparc-elf-gcc
CFLAGS += -g -Os -fno-tree-ch -msoft-float -std=gnu99 -Wall -fgnu89-inline

all: quick_sort

quick_sort: $(MYOBJS) $(MYLIB)
	sparc-elf-gcc $(LDFLAGS) -o $@ $^

.PHONY:clean
clean:
	rm -f quick_sort *.o *.i
