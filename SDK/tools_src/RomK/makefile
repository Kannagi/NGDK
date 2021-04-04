WORKDIR = `pwd`

CC = cc

INC = 
CFLAGS = -Wall -O3
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -s

INC_BIN = $(INC)
CFLAGS_BIN = $(CFLAGS)
RESINC_BIN = $(RESINC)
RCFLAGS_BIN = $(RCFLAGS)
LIBDIR_BIN = $(LIBDIR)
LIB_BIN = $(LIB)
LDFLAGS_BIN = $(LDFLAGS)
DEP_BIN = 
OUT_BIN = bin/RomK

OBJ_BIN = main.o

all: $(OUT_BIN)

$(OUT_BIN):  $(OBJ_BIN) $(DEP_BIN)
	mkdir -p bin
	$(CC) $(LIBDIR_BIN) -o $(OUT_BIN) $(OBJ_BIN) $(LDFLAGS_BIN) $(LIB_BIN)

main.o: main.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c main.c -o main.o

clean: 
	rm -f $(OBJ_BIN) $(OUT_BIN)
	rm -rf bin

.PHONY: clean
