# NGDK
SDK for Neo Geo (Beta version)

Discord : https://discord.gg/2VCJN5c

Fork of https://github.com/Kannagi/NGDK

This repository will only be used for the source code, for a binary installation, please use: https://github.com/BrokeStudio/NGDK  

NGDK has makefiles that should compile under Linux and Windows (With MSYS2 MinGW 64-bit/WSL)

## Dependencies
1. m68k-elf-gcc (To target the m68k processor) For an easier installation, use the toolchain (https://github.com/BotBitmap/CTMZ_Toolchain)
2. A C compiler of your choice targeting your machine (EX: GCC/Clang, to compile the tools)
5. libsdl1.2 and libsdl-image1.2 (To compile the tools)
4. bsdtar libarchive (For packing roms)
5. make (To compile everything)

Ubuntu dependencies (Except for m68k-elf-gcc)
```
apt-get install make gcc libarchive-tools libsdl1.2debian libsdl1.2-dev libsdl-image1.2 libsdl-image1.2-dev
```
Windows (MSYS2 MinGW 64-bit) dependencies (Except for m68k-elf-gcc)
```
pacman -S make mingw-w64-x86_64-gcc mingw-w64-x86_64-libarchive mingw-w64-x86_64-SDL mingw-w64-x86_64-SDL_image
```

## Building
```
cd SDK
make
```

## Code::Blocks

### Config
- The CodeBlock folder, has screens and text files to configure 

### Windows
- To run Winkawaws in Code::Block, you have to go to Tools -> configure tools -> add
- Name : WinKawaks
- executable : "C:\SDK\NeoGeo\emu\WinKawaks.exe"
- Tools -> WinKawaks

### Linux
- To run gngeo in Code::Block , Project -> Set program arguments -> bin(Linux)
- & gngeo --rompath=yourpath ssideki


## Running roms in MAME
```
mame $YOUR_ROM -bios $YOUR_BIOS
```

## The Neo Geo has 6 ROMs
- C1-C2 for Sprites
- S1 for Fix (HuD)
- M1 for Z80
- P1 for M68000
- V1 for Samples
