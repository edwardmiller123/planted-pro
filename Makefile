CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

SRCDIR := src

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall
CSRCS := $(filter-out $(SRCDIR)/interrupt_table.c, $(wildcard $(SRCDIR)/*.c))
COBJS := $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(CSRCS))

USE_MODE = $(if $(MODE),-DMODE=$(MODE),)

COMMIT = $(shell git rev-parse --short HEAD)
$(info    Current commit is $(COMMIT))

plant-monitor: $(COBJS)
	$(LD) $^ -T $(SRCDIR)/linker.ld -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(USE_MODE) -DGIT_SHA=\"$(COMMIT)\" -c $^ -o $@ -g

.PHONY: clean

clean :
	rm -rf $(SRCDIR)/*.o plant-monitor