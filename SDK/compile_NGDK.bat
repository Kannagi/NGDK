@echo off
setlocal enabledelayedexpansion

:: # Prepare vars

set BIN=%NGDK_WIN%\bin
set INC=%NGDK_WIN%\inc

set SRC=src
set OBJ=src\obj
set OUT=lib

set CC=%BIN%\m68k\bin\m68k-elf-gcc.exe
set AR=%BIN%\m68k\bin\m68k-elf-ar.exe

set SFLAGS=-O0 -m68000 -I"%INC%"
set CFLAGS=-O1 -m68000 -I"%INC%"

:: # Create/Clear obj folder if needed
if not exist %OBJ%\ (
  echo # Creating %OBJ% folder
  mkdir %OBJ%
)
echo # Cleaning obj folder
del %OBJ%\*.* /Q

:: # Create out folder if needed
if not exist %OUT%\ (
  echo # Creating %OUT% folder
  mkdir %OUT%
)
echo # Cleaning out folder
del %OUT%\*.* /Q

:: # Compile NGDK lib
echo # Compiling NGDK lib
for /r %SRC% %%I in (*.s) do (
  %CC% %SFLAGS% -c "%%~I" -o "%OBJ%\%%~nI.o"
)

for /r %SRC% %%I in (*.c) do (
  %CC% %CFLAGS% -c "%%~I" -o "%OBJ%\%%~nI.o"
)

:: # Link everything
echo # Linking
set "OBJ_LST="
for /r %OBJ% %%I in (*.o) do (
  set OBJ_LST=!OBJ_LST! %OBJ%\%%~nI%%~xI
)
%AR% -r -s %OUT%\libNGDK.a %OBJ_LST%