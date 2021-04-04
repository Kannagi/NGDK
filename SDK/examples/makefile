# MIT License
#  
# Copyright (c) 2019 Kannagi
# Copyright (c) 2021 Arthur Rodrigues de Carvalho Chagas
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#  
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#  
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

MAKEFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR = $(dir $(MAKEFILE_PATH))
NGDK = $(abspath $(MAKEFILE_DIR)/..)
ROMNAME = ssideki
ROMNUM = 052

SILENT =
RES = 0

BIN = $(NGDK)/bin
INC = $(NGDK)/inc
LIB = $(NGDK)/lib
TOOLS = $(NGDK)/bin/tools

SRC = src
OBJ = obj
OUT = out
DATA = data

FILE_ARCHIVER_PACK = bsdtar -a -cf
MKDIR = mkdir
CP = cp
RM = rm

CC = m68k-elf-gcc
LD = m68k-elf-ld

SFLAGS = -O0 -m68000 -I"$(INC)" -I"data"
CFLAGS = -O1 -m68000 -I"$(INC)" -I"data"

LFLAGS = --oformat binary -Ttext  -0x00 -Tbss 0x100020 -allow-multiple-definition -L"$(LIB)"

OBJ_S = $(patsubst $(SRC)/%.s,$(OBJ)/%.o,$(wildcard $(SRC)/*.s))
OBJ_C = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(wildcard $(SRC)/*.c))

all: ressources build

build: $(OUT) $(OBJ) _build make_rom

_build: $(OBJ_S) $(OBJ_C)
	$(LD) $(LFLAGS) -o $(OUT)/$(ROMNUM)-p1.bin $^ -lNGDK

$(OUT):
	$(MKDIR) $(OUT)

$(OBJ):
	$(MKDIR) $(OBJ)

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o : $(SRC)/%.s
	$(CC) $(SFLAGS) -c $< -o $@

ressources:
	cd $(DATA); \
	$(TOOLS)/RetroRes $(TOOLS)

make_rom:
	$(CP) samples/$(ROMNUM)-v1.bin $(OUT)/$(ROMNUM)-v1.v1
	$(CP) data/$(ROMNUM)-c1.bin $(OUT)/$(ROMNUM)-c1.c1
	$(CP) data/$(ROMNUM)-c2.bin $(OUT)/$(ROMNUM)-c2.c2
	$(CP) fix/$(ROMNUM)-s1.s1 $(OUT)/$(ROMNUM)-s1.s1
	$(CP) sound/$(ROMNUM)-m1.m1 $(OUT)/$(ROMNUM)-m1.m1
	$(CP) $(OUT)/$(ROMNUM)-p1.bin $(OUT)/$(ROMNUM)-p1.p1
	$(TOOLS)/RomK -invertng $(OUT)/$(ROMNUM)-p1.p1
	$(TOOLS)/RomK -fillrom $(OUT)/$(ROMNUM)-p1.p1 524288
	cd $(OUT); \
	$(FILE_ARCHIVER_PACK) $(ROMNAME).zip $(ROMNUM)-p1.p1 $(ROMNUM)-m1.m1 $(ROMNUM)-v1.v1 $(ROMNUM)-s1.s1 $(ROMNUM)-c1.c1 $(ROMNUM)-c2.c2

clean:
	$(RM) -f $(OBJ)/*.o
	$(RM) -f $(DATA)/*.bin
	$(RM) -f $(DATA)/*.c
	$(RM) -f $(DATA)/*.h
	$(RM) -f $(DATA)/*.ktm
	$(RM) -f $(OUT)/*
