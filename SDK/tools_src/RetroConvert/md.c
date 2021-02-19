#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"

int md_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type)
{
    int casex,casey;
    int tiles[64];
    int l = 0;
    int x,y,size = 0,byte;
    unsigned char *pixel = image->pixels;

    casex = 0;
    casey = 0;

    if(type > 3) type = 0;

    while(1)
    {
        bloc_palette(image,casex,casey,8,pixel,palette,tiles);

        for(y = 0;y <8;y++)
		{
			for(x = 0;x < 8;x+=2)
			{
				byte = tiles[ (x+1) + (y*8)]+(tiles[ (x+0) + (y*8)]<<4);
				fputc(byte,file);
			}
		}

		if(type == 0) casex += 8;

		if(type == 1)
		{
			if(l == 0) casey += 8;
			if(l == 1)
			{
				casex += 8;
				casey -= 8;
			}
			l++;
			if(l == 2) l = 0;
		}

		if(type == 2)
		{
			if(l <= 1) casey += 8;
			if(l == 2)
			{
				casex += 8;
				casey -= 16;
			}
			l++;
			if(l == 3) l = 0;
		}

		if(type == 3)
		{
			if(l <= 2) casey += 8;
			if(l == 3)
			{
				casex += 8;
				casey -= 24;
			}
			l++;
			if(l == 4) l = 0;
		}

        if(casex+8 >image->w)
        {
            casex = 0;

            if(type == 0) casey += 8;
            if(type == 1) casey += 16;
            if(type == 2) casey += 24;
            if(type == 3) casey += 32;
        }

        if(casey+8 > image->h) break;
        size += 32;
    }

    return size;
}

int md_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode)
{
    int i,n = 0;
    int psize = 0;
    int color;
    int pal[4];

    int size = image->w*image->h*image->format->BytesPerPixel;
    unsigned char *pixel = image->pixels;

    if(mode == 3)
    {
        n= 0;
        for(i = 0;i < size;i += image->format->BytesPerPixel)
        {
            palette[n+0] = pixel[i+0];
            palette[n+1] = pixel[i+1];
            palette[n+2] = pixel[i+2];
            n +=3;
            if(ncolor >= 0x300) break;
        }
        ncolor = n/3;
    }

    for(i = 0;i < ncolor;i++)
    {
        n = i*3;

    	pal[0] = palette[n+0] >> 5;
    	pal[1] = palette[n+1] >> 5;
    	pal[2] = palette[n+2] >> 5;

    	color = (pal[2] << 1) + (pal[1] << 5) + (pal[0] << 9);

        fputc((color>>8)&0xFF,file);
        fputc(color&0xFF,file);

        psize += 2;
    }

    return psize;
}

