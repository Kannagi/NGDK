
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"

int gb_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type)
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
/*
				for(i = 0;i < 4;i++)
				{
					l = i*3;
					if(b == (palette[l+0]) && g == (palette[l+1]) && r == (palette[l+2]))
						break;
				}

				if(ncolor > 3)
				{
					if(i != 0)
					{
						i = (r+g+b)/3;
						i = i >> 6;
					}else
					{
						if(type != 0)
						{
							i = (r+g+b)/3;
							i = i >> 6;
						}
					}
				}
*/

				i = (r+g+b)/3;
				i = i >> 6;
				i = 3-i;
				//printf("%d %d %d : %d\n",b,v,r,i);

				bin[0] += ( (i>>0) & 0x01 ) << (7 - x);
				bin[1] += ( (i>>1) & 0x01 ) << (7 - x);

			}

			for(i = 0;i < 2;i++)
			{
				fputc(bin[i],file);
			}

			size +=2;
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


int gb_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode)
{


	return 0;
}
