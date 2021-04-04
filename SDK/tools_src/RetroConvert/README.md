# RetroConvert
Converter png/bmp/pcx to Nes/SNES/Master System/Mega Drive/PC Engine/NeoGeo/ Game Boy

## Command
- RetroConvert -sfc myimage.png

## Option target cible
- -nes , -sms , -pce , -sfc , -md , -ng , -gb

## Option palette
- -palette (output palette only)
- -paletteall (output palette image)
- -loadpalette + arg , (load palette extern) , default load "palette.png"

## Option sfc
- -2bpp,-4bpp ,-8bpp,-mode7,-map

## Option pce
- -bg,-spr

## Option MD
- -h + arg , arg (0-3) (horizontal sprite (arg+1)*8 pixels)

## Option NeoGeo
- -ngv (vertical sprite)

## Option Nes
- -customMesen (use palette Mesen)
- -customBin (custom palette Nes , read "nes_custom.pal")
- -customPal (custom palette Nes , read "nes_custom.png")
