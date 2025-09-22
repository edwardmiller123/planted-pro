
REV = 4
NAME = planted-pro
REV_NAME = $(NAME)-main-board-r$(REV)

PCB_DIR := pcb

PCB := $(wildcard $(PCB_DIR)/*.kicad_pcb)

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

SRC_DIR := src

BUILD_DIR := build
GERBER_DIR := $(BUILD_DIR)/gerber
OBJ_DIR := $(BUILD_DIR)/obj

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall
CSRCS := $(filter-out $(SRC_DIR)/interrupt_table.c, $(wildcard $(SRC_DIR)/*.c))
COBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CSRCS))

USE_MODE = $(if $(MODE),-DMODE=$(MODE),)

COMMIT = $(shell git rev-parse --short HEAD)

all: fw pcb

fw: $(NAME)-fw.elf
	$(info    FW build commit is $(COMMIT))

$(NAME)-fw.elf: $(COBJS)
	$(LD) $^ -T $(SRC_DIR)/linker.ld -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(USE_MODE) -DGIT_SHA=\"$(COMMIT)\" -c $^ -o $@ -g

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

pcb: $(PCB)
	mkdir -p $(GERBER_DIR)
	kicad-cli pcb export drill -o $(GERBER_DIR) --excellon-separate-th $<
	kicad-cli pcb export gerbers -o $(GERBER_DIR) --board-plot-params $<
	zip -9 -j $(REV_NAME) $(GERBER_DIR)/*
	$(info    PCB revision $(REV))

clean :
	rm -rf build *.elf *.zip

.PHONY: clean pcb fw