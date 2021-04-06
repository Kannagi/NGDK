WORKDIR = `pwd`

CC = cc

INC = 
CFLAGS = -Wall
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_BIN = $(INC)
CFLAGS_BIN = $(CFLAGS) -O3
RESINC_BIN = $(RESINC)
RCFLAGS_BIN = $(RCFLAGS)
LIBDIR_BIN = $(LIBDIR)
LIB_BIN = $(LIB)
LDFLAGS_BIN = $(LDFLAGS) -s
OBJDIR_BIN = obj
DEP_BIN = 
OUT_BIN = bin/RetroTMX

OBJ_BIN = aura.o main.o tmx.o

all: $(OUT_BIN)

$(OUT_BIN):  $(OBJ_BIN) $(DEP_BIN)
	mkdir -p bin
	$(CC) $(LIBDIR_BIN) -o $(OUT_BIN) $(OBJ_BIN)  $(LDFLAGS_BIN) $(LIB_BIN)

aura.o: aura.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c aura.c -o aura.o

main.o: main.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c main.c -o main.o

tmx.o: tmx.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c tmx.c -o tmx.o

clean: 
	rm -f $(OBJ_BIN) $(OUT_BIN)
	rm -rf bin

.PHONY: clean

