#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"

int sms_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type)
{
	int casex,casey;
	int tiles[64];
	int i,l;
	int x,y,size = 0;
	int bin[128];
	int b,g,r;

	unsigned char *pixel = image->pixels;

	casex = 0;
	casey = 0;

	while(1)
	{

		for(y = 0;y <8;y++)
		{
			bin[0] = 0;
			bin[1] = 0;
			bin[2] = 0;
			bin[3] = 0;

			for(x = 0;x < 8;x++)
			{
				i = x + (y*image->w) + casex + (casey*image->w);
				l = i*3;
				b = pixel[l+0];
				g = pixel[l+1];
				r = pixel[l+2];

				for(i = 0;i <16;i++)
				{
					l = i*3;
					if(b == (palette[l+0]) && g == (palette[l+1]) && r == (palette[l+2]))
						break;
				}

				//printf("%d %d %d : %d\n",b,v,r,i);

				bin[0] += ( (i>>0) & 0x01 ) << (7 - x);
				bin[1] += ( (i>>1) & 0x01 ) << (7 - x);
				bin[2] += ( (i>>2) & 0x01 ) << (7 - x);
				bin[3] += ( (i>>3) & 0x01 ) << (7 - x);
			}

			for(i = 0;i < 4;i++)
			{
				fputc(bin[i],file);
			}

			size +=4;
		}

		casex += 8;
		if(casex+8 >image->w)
		{
			casex = 0;
			casey += 8;

		}

		if(casey+8 >image->h) break;
	}

	return size;
}


int sms_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode)
{
	int i,n;
	int psize = 0;
	int color;
	int r,g,b;

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

	for(i = 0;i <ncolor;i++)
	{
		n = i*3;

		r = ( (palette[n+0])/64);
		g = ( (palette[n+1])/64);
		b = ( (palette[n+2])/64);

		//printf("%d , %d , %d\n",palette[n+0],palette[n+1],palette[n+2]);

		color = (r<<4) + (g<<2) + b;

		fputc(color&0xFF,file);
		psize++;
	}

	return psize;
}
