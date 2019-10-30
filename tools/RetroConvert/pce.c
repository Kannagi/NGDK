#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"

int pce_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type)
{
    int casex,casey;
    int tiles[64];
    int i,l;
    int x,y,size = 0;
    int bin[128];
	int bx,by,bn;

	int nl = 4,ns = 128,casez = 16;
	unsigned char *pixel = image->pixels;

	if(type == 1)
	{
		nl = 1;
		ns = 32;
		casez = 8;
	}

    casex = 0;
    casey = 0;

    while(1)
    {
        for(i = 0;i < 128;i++)
			bin[i] = 0;


		for(l = 0;l < nl;l++)
		{

			if(l == 1)
			{
				casex+= 8;
			}

			if(l == 2)
			{
				casey += 8;
			}

			if(l == 3)
			{
				casex -= 8;
			}

			bloc_palette(image,casex,casey,8,pixel,palette,tiles);

			for(y = 0;y < 8;y++)
			{
				for(x = 0;x < 8;x++)
				{
					i = tiles[x + (y*8)];
					if(i > 15) i = 15;

					bx = x/8;
					by = y*2;
					bn = bx+by;

					if(type == 0)
					{

						if(l == 0) bn += 0x01;
						if(l == 2) bn += 0x10;
						if(l == 3) bn += 0x11;

						bin[bn+0x00] += ( (i&0x01)>>0 ) << (7 - x);
						bin[bn+0x20] += ( (i&0x02)>>1 ) << (7 - x);
						bin[bn+0x40] += ( (i&0x04)>>2 ) << (7 - x);
						bin[bn+0x60] += ( (i&0x08)>>3 ) << (7 - x);
					}else
					{
						bin[bn+0x00] += ( (i&0x01)>>0 ) << (7 - x);
						bin[bn+0x01] += ( (i&0x02)>>1 ) << (7 - x);
						bin[bn+0x10] += ( (i&0x04)>>2 ) << (7 - x);
						bin[bn+0x11] += ( (i&0x08)>>3 ) << (7 - x);
					}

					//if(i > npal-1) i = npal-1;
				}

			}

			if(l == 3)
			{
				casey -= 8;
			}

		}

		for(i = 0;i < ns;i++)
		{
			fputc(bin[i],file);
		}

		size += ns;

        casex += casez;
        if(casex+casez >image->w)
        {
            casex = 0;
            casey += casez;
        }

        if(casey+casez >image->h) break;

    }

    return size;
}


int pce_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode)
{
    int i,n;
    int psize = 0;
    int color;
    int pal[4];

    int size = image->w*image->h*image->format->BytesPerPixel;
    unsigned char *pixel = image->pixels;

    if(mode == 3)
    {
        n = 0;
        for(i = 0;i < size;i += image->format->BytesPerPixel)
        {
            palette[n+0] = pixel[i+0];
            palette[n+1] = pixel[i+1];
            palette[n+2] = pixel[i+2];
            n +=3;
            if(n > 768) break;
        }
        ncolor = n/3;
    }

    for(i = 0;i < ncolor;i++)
    {
        n = i*3;

        pal[0] = palette[n+0]>>5;
        pal[1] = palette[n+1]>>5;
		pal[2] = palette[n+2]>>5;

		color = (pal[1]<<6) + (pal[2]<<3) +pal[0];

		fputc(color&0xFF,file);
		fputc( (color&0xFF)>>8 ,file);

        psize += 2;
    }

    return psize;
}
