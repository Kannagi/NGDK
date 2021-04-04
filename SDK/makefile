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
NGDK = $(MAKEFILE_DIR)
BIN = $(NGDK)/bin/
INC = $(NGDK)/inc/

SRC = $(NGDK)/src/
OBJ = $(NGDK)/obj/
LIB = $(NGDK)/lib/

MKDIR = mkdir

CC = m68k-elf-gcc
AR = m68k-elf-ar

SFLAGS = -O0 -m68000 -I"$(INC)"
CFLAGS = -O1 -m68000 -I"$(INC)"
OBJ_C = $(patsubst $(SRC)%.c,$(OBJ)%.o,$(wildcard $(SRC)*.c))

all: lib tools examples

$(LIB):
	$(MKDIR) $(LIB)

$(OBJ):
	$(MKDIR) $(OBJ)

lib: $(LIB) $(OBJ) $(OBJ_C)
	$(AR) -r -s $(LIB)libNGDK.a $(OBJ_C)

lib_clean:
	$(RM) $(OBJ)*.o
	$(RM) -rf $(LIB)libNGDK.a

$(OBJ)%.o : $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: lib_clean tools_clean examples_clean
	$(RM) $(OBJ)*.o

# TOOLS INIT
tools_build:
	make -C $(NGDK)/tools_src/NGextract
	make -C $(NGDK)/tools_src/RetroConvert
	make -C $(NGDK)/tools_src/RetroRes
	make -C $(NGDK)/tools_src/RetroTMX
	make -C $(NGDK)/tools_src/RomK
	make -C $(NGDK)/tools_src/bin2c

tools_clean:
	make clean -C $(NGDK)/tools_src/NGextract
	make clean -C $(NGDK)/tools_src/RetroConvert
	make clean -C $(NGDK)/tools_src/RetroRes
	make clean -C $(NGDK)/tools_src/RetroTMX
	make clean -C $(NGDK)/tools_src/RomK
	make clean -C $(NGDK)/tools_src/bin2c
	rm -f $(NGDK)/bin/tools/*

tools: tools_build
# Copying files to NGDK bin directory
	mkdir -p                                $(NGDK)/bin/tools/
	cp $(NGDK)/tools_src/NGextract/bin/*    $(NGDK)/bin/tools/
	cp $(NGDK)/tools_src/RetroConvert/bin/* $(NGDK)/bin/tools/
	cp $(NGDK)/tools_src/RetroRes/bin/*     $(NGDK)/bin/tools/
	cp $(NGDK)/tools_src/RetroTMX/bin/*     $(NGDK)/bin/tools/
	cp $(NGDK)/tools_src/RomK/bin/*         $(NGDK)/bin/tools/
	cp $(NGDK)/tools_src/bin2c/bin2c        $(NGDK)/bin/tools/
# TOOLS END

# EXAMPLES INIT
examples: lib tools
	make -C $(NGDK)/examples
examples_clean:
	make clean -C $(NGDK)/examples
# EXAMPLES END
