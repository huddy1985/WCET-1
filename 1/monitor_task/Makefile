# (c) 2011, Benedikt Huber, benedikt@vmars.tuwien.ac.at

# You need to have sparc-elf-gcc in PATH and $SPARC_ELF_INCLUDE set

MYLIB=bench.o
MYOBJS=fixedpoint.o sample_buffer.o task.o test_data.o test.o
CC=sparc-elf-gcc
#CC=llvm-gcc
CFLAGS += -g -Os -mv8 -msoft-float -std=gnu99 -Wall -fgnu89-inline

all: monitor_task

monitor_task: $(MYOBJS) $(MYLIB)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY:clean
clean:
	rm -f monitor_task *.o *.i
