CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

SRCDIR := src

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall
CSRCS := $(filter-out $(SRCDIR)/interrupt_table.c, $(wildcard $(SRCDIR)/*.c))
COBJS := $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(CSRCS))

plant-monitor: $(COBJS)
	$(LD) $^ -T linker.ld -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -g

.PHONY: clean

clean :
	rm -rf $(SRCDIR)/*.o plant-monitor