@echo off
cd bin
rename 052-p1.bin 052-p1.p1
rename 052-m1.bin 052-m1.m1
rename 052-v1.bin 052-v1.v1
rename 052-s1.bin 052-s1.s1
rename 052-c1.bin 052-c1.c1
rename 052-c2.bin 052-c2.c2
c:\SDK\tools\RomK.exe -invertng 052-p1.p1
c:\SDK\tools\RomK.exe -fillrom 052-p1.p1 524288
c:\SDK\tools\7z.exe a -R ssideki.zip 052-p1.p1 052-m1.m1 052-v1.v1 052-s1.s1 052-c1.c1 052-c2.c2
copy ssideki.zip c:\SDK\NeoGeo\emu\roms\ssideki.zip

