
#include <stdlib.h>
#include <stdio.h>


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"

int ng_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode)
{
	int i,n;
	int psize = 0;
	int color;
	int ngpal[4];

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
			if(n >= 0x300) break;
		}
		ncolor = n/3;
	}

	for(i = 0;i < ncolor;i++)
	{
		n = i*3;

		ngpal[0] = palette[n+0]>>3;
		ngpal[1] = palette[n+1]>>3;
		ngpal[2] = palette[n+2]>>3;

		color = ( (ngpal[0]& 0x1E) >> 1) + ( (ngpal[1]& 0x1E) << 3) + ( (ngpal[2]& 0x1E) << 7);
		color += ( (ngpal[0] & 0x01) << 12) + ((ngpal[1] & 0x01) << 13) + ((ngpal[0] & 0x01) << 14);

		if(i == 16)
		{
			fputc(0,file);
			fputc(0,file);
			psize += 2;
		}

		fputc(color>>8,file);
		fputc(color,file);

		psize += 2;
	}

	return psize;
}


int ng_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type)
{
	int casex,casey;
	int tiles[64];
	int ngpixel[4];
	int quad = 0,quadx,quady,x,y,i,l,ext = 0,size = 0;
	unsigned char *pixel = image->pixels;


	if(ncolor > 16) ext = 1;

	for(l = 0; l < ext+1;l++)
	{
		casex = 0;
		casey = 0;
		quad = 0;

		while(1)
		{
			if(quad == 0)
			{
				quadx = casex+8;
				quady = casey;
			}

			if(quad == 1)
			{
				quadx = casex+8;
				quady = casey+8;
			}

			if(quad == 2)
			{
				quadx = casex;
				quady = casey;
			}

			if(quad == 3)
			{
				quadx = casex;
				quady = casey+8;
			}

			bloc_palette(image,quadx,quady,8,pixel,palette,tiles);

			for(y = 0;y <8;y++)
			{
				ngpixel[0] = 0;
				ngpixel[1] = 0;
				ngpixel[2] = 0;
				ngpixel[3] = 0;

				for(x = 0;x < 8;x++)
				{
					i = tiles[x + (y*8)];

					if(l == 0 && i < 16)
					{
						ngpixel[0] += ( (i>>0) & 0x01 ) << (x);
						ngpixel[1] += ( (i>>1) & 0x01 ) << (x);
						ngpixel[2] += ( (i>>2) & 0x01 ) << (x);
						ngpixel[3] += ( (i>>3) & 0x01 ) << (x);
					}

					if(l == 1 && i > 15)
					{
						i -= 15;
						ngpixel[0] += ( (i>>0) & 0x01 ) << (x);
						ngpixel[1] += ( (i>>1) & 0x01 ) << (x);
						ngpixel[2] += ( (i>>2) & 0x01 ) << (x);
						ngpixel[3] += ( (i>>3) & 0x01 ) << (x);
					}

				}

				fputc(ngpixel[0],file);
				fputc(ngpixel[1],file);
				fputc(ngpixel[2],file);
				fputc(ngpixel[3],file);
			}
			size += 32;

			quad++;

			if((type&1) == 0)
			{
				if(quad > 3)
				{
					quad = 0;
					casey += 16;

					if(casey+16 > image->h)
					{
						casex += 16;
						casey = 0;
					}
				}

				if(casex+16 > image->w)  break;
			}else
			{
				if(quad > 3)
				{
					quad = 0;
					casex += 16;
					if(casex+16 > image->w)
					{
						casex = 0;
						casey += 16;
					}
				}

				if(casey+16 > image->h)  break;
			}



		}

	}


	return size;
}
