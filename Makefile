CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

CFLAGS = -mcpu=cortex-m4 -mthumb -g
CSRCS := $(filter-out interrupt_table.c, $(wildcard *.c))
COBJS := $(patsubst %.c, %.o, $(CSRCS))

ASMSRCS := $(wildcard *.s)
ASMOBJS := $(patsubst %.s, %.o, $(ASMSRCS))

prog: $(COBJS) $(ASMOBJS)
	$(LD) $^ -T linker.ld -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -g

%.o: %.s
	$(CC) $(CFLAGS) -c $^ -o $@ -g

clean :
	rm -rf *.o prog