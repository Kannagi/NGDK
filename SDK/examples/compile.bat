@echo off
setlocal enabledelayedexpansion

:: # Prepare vars

set ROMNAME=ssideki
set ROMNUM=052


set "SILENT="
set RES=0

set BIN=%NGDK_WIN%\bin
set LIBINC=%NGDK_WIN%\inc
set LIBLIB=%NGDK_WIN%\lib
set TOOLS=%NGDK_WIN%\bin\tools

set SRC=src
set OBJ=obj
set OUT=out

set CC=%BIN%\m68k\bin\m68k-elf-gcc.exe
set LD=%BIN%\m68k\bin\m68k-elf-ld.exe

set SFLAGS=-O0 -m68000 -I"%LIBINC%" -I"data"
set CFLAGS=-O1 -m68000 -I"%LIBINC%" -I"data"

set LFLAGS=--oformat binary -Ttext -0x00 -Tbss 0x100020 -L"%LIBLIB%"

:: # Check arguments

if "%~1"=="" goto skipArguments

:loop

set p=%1

if "%p:~0,2%"=="-h" goto showHelp
if "%p:~0,2%"=="/h" goto showHelp
if "%p:~0,2%"=="/?" goto showHelp
if "%p:~0,2%"=="-?" goto showHelp

if "%p:~0,2%"=="-s" (
    echo # Silent mode enabled
    set SILENT=^> nul
    goto nextArg
)

if "%p:~0,5%"=="--res" (
    set RES=1
    goto nextArg
)

echo Unknown argument : %p:~0,4%
goto showHelp

:nextArg

shift

if not "%~1"=="" goto loop

:skipArguments

if %RES%==0 goto skipRessources

:: # Compile ressources
echo # Compiling ressources
cd data
del *.c /Q
del *.h /Q
del *.bin /Q
del *.ktm /Q
%TOOLS%\RetroRes.exe %TOOLS% %SILENT%
cd..

:skipRessources

:: # Create/Clear obj folder if needed
if not exist %OBJ%\ (
  echo # Creating %OBJ% folder
  mkdir %OBJ%
)
echo # Cleaning %OBJ% folder
del %OBJ%\*.* /Q

:: # Create out folder if needed
if not exist %OUT%\ (
  echo # Creating %OUT% folder
  mkdir %OUT%
)
echo # Cleaning out folder
del %OUT%%\*.* /Q

:: # Compile project
echo # Compiling project
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
  if NOT "%%~nI" == "_start" (
    set OBJ_LST=!OBJ_LST! %OBJ%\%%~nI%%~xI
  )
)
%LD% %LFLAGS% -o %OUT%\%ROMNUM%-p1.bin %OBJ%\_start.o %OBJ_LST% -lNGDK

:: # Build ROM file
echo # Building ROM file

copy samples\%ROMNUM%-v1.bin %OUT%\%ROMNUM%-v1.v1 %SILENT%
copy data\%ROMNUM%-c1.bin %OUT%\%ROMNUM%-c1.c1 %SILENT%
copy data\%ROMNUM%-c2.bin %OUT%\%ROMNUM%-c2.c2 %SILENT%
copy fix\%ROMNUM%-s1.s1 %OUT%\%ROMNUM%-s1.s1 %SILENT%
copy sound\%ROMNUM%-m1.m1 %OUT%\%ROMNUM%-m1.m1 %SILENT%

cd %OUT%
copy %ROMNUM%-p1.bin %ROMNUM%-p1.p1 %SILENT%

%TOOLS%\RomK.exe -invertng %ROMNUM%-p1.p1 %SILENT%
%TOOLS%\RomK.exe -fillrom %ROMNUM%-p1.p1 524288 %SILENT%

%TOOLS%\7z.exe a -R %ROMNAME%.zip %ROMNUM%-p1.p1 %ROMNUM%-m1.m1 %ROMNUM%-v1.v1 %ROMNUM%-s1.s1 %ROMNUM%-c1.c1 %ROMNUM%-c2.c2 %SILENT%

cd ..

echo # Done :)

goto exit

:: # Show help

:showHelp

echo Usage: compile [options[:value]]
echo Options:
echo  -h                Help (this text)
echo  -s                Silent
echo  --res             Compile ressources


:exit