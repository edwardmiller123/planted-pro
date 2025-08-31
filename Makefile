CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

SRCDIR := src

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall
CSRCS := $(filter-out $(SRCDIR)/interrupt_table.c, $(wildcard $(SRCDIR)/*.c))
COBJS := $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(CSRCS))

USE_MODE = $(if $(MODE),-DMODE=$(MODE),)

plant-monitor: $(COBJS)
	$(LD) $^ -T linker.ld -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(USE_MODE) -DGITSHA="git rev-parse --short HEAD" -c $^ -o $@ -g

.PHONY: clean

clean :
	rm -rf $(SRCDIR)/*.o plant-monitor