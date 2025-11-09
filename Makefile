
REV = 4
NAME = planted-pro
REVNAME = $(NAME)-main-board-r$(REV)

PCBDIR := pcb

PCB := $(wildcard $(PCBDIR)/*.kicad_pcb)

SCHEMATIC := $(wildcard $(PCBDIR)/*.kicad_sch)

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

FWDIR := fw

BUILDDIR := build
GERBERDIR := $(BUILDDIR)/gerber
OBJDIR := $(BUILDDIR)/obj

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall
CSRCS := $(filter-out $(FWDIR)/interrupt_table.c, $(wildcard $(FWDIR)/*.c))
COBJS := $(patsubst $(FWDIR)/%.c, $(OBJDIR)/%.o, $(CSRCS))

WITHLOGGING = $(if $(LOGLEVEL),-DLOGLEVEL=$(LOGLEVEL),)

COMMIT = $(shell git rev-parse --short HEAD)

all: fw pcb schematic

fw: $(NAME)-fw.elf
	$(info    FW build commit is $(COMMIT))

$(NAME)-fw.elf: $(COBJS)
	$(LD) $^ -T $(FWDIR)/linker.ld -o $@

$(OBJDIR)/%.o: $(FWDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(WITHLOGGING) -DGIT_SHA=\"$(COMMIT)\" -c $^ -o $@ -g

$(OBJDIR):
	mkdir -p $(OBJDIR)

pcb: $(PCB)
	mkdir -p $(GERBERDIR)
	kicad-cli pcb export drill -o $(GERBERDIR) --excellon-separate-th $<
	kicad-cli pcb export gerbers -o $(GERBERDIR) --board-plot-params $<
	zip -9 -j $(REVNAME) $(GERBERDIR)/*
	$(info    PCB revision $(REV))

schematic: $(SCHEMATIC)
	kicad-cli sch export pdf -o $(REVNAME).pdf $<

clean :
	rm -rf build *.elf *.zip *.pdf

.PHONY: clean pcb fw