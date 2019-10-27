#!/bin/bash
mkdir WinKawaks
cp *.bin WinKawaks
cd WinKawaks
mv 052-c1.bin 052-c1.c1
mv 052-c2.bin 052-c2.c2
mv 052-p1.bin 052-p1.p1
mv 052-v1.bin 052-v1.v1
mv 052-s1.bin 052-s1.s1
mv 052-m1.bin 052-m1.m1

/usr/local/SDK/tools/RomK -fillrom 052-p1.p1 524288
zip -1 ssideki 052-c1.c1 052-c2.c2 052-p1.p1 052-v1.v1 052-s1.s1 052-m1.m1

rm 052-c1.c1
rm 052-c2.c2
rm 052-p1.p1
rm 052-v1.v1
rm 052-s1.s1
rm 052-m1.m1