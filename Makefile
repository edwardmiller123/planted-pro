CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall
CSRCS := $(filter-out interrupt_table.c, $(wildcard *.c))
COBJS := $(patsubst %.c, %.o, $(CSRCS))

ASMSRCS := $(wildcard *.s)
ASMOBJS := $(patsubst %.s, %.o, $(ASMSRCS))

plant-monitor: $(COBJS) $(ASMOBJS)
	$(LD) $^ -T linker.ld -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -g

%.o: %.s
	$(CC) $(CFLAGS) -c $^ -o $@ -g

.PHONY: clean

clean :
	rm -rf *.o plant-monitor