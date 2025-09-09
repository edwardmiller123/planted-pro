
REV = 1
NAME = planted-pro
REV_NAME = $(NAME)-r$(REV)
$(info    PCB revision $(REV))

PCBDIR := pcb

PCB := $(wildcard $(PCBDIR)/*.kicad_pcb)

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

SRCDIR := src
GERBER_DIR := pcb/gerber

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall
CSRCS := $(filter-out $(SRCDIR)/interrupt_table.c, $(wildcard $(SRCDIR)/*.c))
COBJS := $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(CSRCS))

USE_MODE = $(if $(MODE),-DMODE=$(MODE),)

COMMIT = $(shell git rev-parse --short HEAD)
$(info    Current commit is $(COMMIT))

all: $(NAME)-fw.elf pcb

$(NAME)-fw.elf: $(COBJS)
	$(LD) $^ -T $(SRCDIR)/linker.ld -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(USE_MODE) -DGIT_SHA=\"$(COMMIT)\" -c $^ -o $@ -g

pcb: $(PCB)
	mkdir -p $(GERBER_DIR)
	kicad-cli pcb export drill -o $(GERBER_DIR) --excellon-separate-th $<
	kicad-cli pcb export gerbers -o $(GERBER_DIR) --board-plot-params $<
	zip -9 -j $(REV_NAME) $(GERBER_DIR)/*

clean :
	rm -rf $(SRCDIR)/*.o pep-fw

.PHONY: clean pcb