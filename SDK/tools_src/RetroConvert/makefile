WORKDIR = `pwd`

CC = cc

INC = 
CFLAGS = 
RESINC = 
LIBDIR = 
LIB = -lSDL -lSDL_image -lm
LDFLAGS = -s

INC_BIN = $(INC)
CFLAGS_BIN = $(CFLAGS) -Wextra -Wall -fexceptions -O3
RESINC_BIN = $(RESINC)
RCFLAGS_BIN = $(RCFLAGS)
LIBDIR_BIN = $(LIBDIR)
LIB_BIN = $(LIB)
LDFLAGS_BIN = $(LDFLAGS) -s
DEP_BIN = 
OUT_BIN = bin/RetroConvert

OBJ_BIN = main.o md.o nes.o ng.o palette.o pce.o sms.o snes.o tilemap.o gb.o

all: $(OUT_BIN)

$(OUT_BIN): $(OBJ_BIN)
	mkdir -p bin
	$(CC) $(LIBDIR_BIN) -o $(OUT_BIN) $(OBJ_BIN) $(LDFLAGS_BIN) $(LIB_BIN)

main.o: src/main.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/main.c -o main.o

md.o: src/md.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/md.c -o md.o

nes.o: src/nes.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/nes.c -o nes.o

ng.o: src/ng.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/ng.c -o ng.o

palette.o: src/palette.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/palette.c -o palette.o

pce.o: src/pce.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/pce.c -o pce.o

sms.o: src/sms.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/sms.c -o sms.o

snes.o: src/snes.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/snes.c -o snes.o

gb.o: src/gb.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/gb.c -o gb.o

tilemap.o: src/tilemap.c
	$(CC) $(CFLAGS_BIN) $(INC_BIN) -c src/tilemap.c -o tilemap.o

clean: 
	rm -rf $(OBJ_BIN) $(OUT_BIN)
	rm -rf bin

.PHONY: clean
