#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"

int snes_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode)
{
	int i,n;
	int psize = 0;
	unsigned char color;
	int snespal[4];

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

		color = palette[n+2]/8;
		snespal[0] = color;

		color = palette[n+1]/8;
		snespal[0] += (0x07 & color) << 5;
		snespal[1] =  (0x18 & color) >> 3;

		color = palette[n+0]/8;
		snespal[1] += color << 2;

		fputc(snespal[0],file);
		fputc(snespal[1],file);

		psize += 2;
	}

	return psize;
}

int snes_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int npal,int type)
{
	int blocx,blocy;
	int tiles[64];
	int snespixel[8];
	int i;
	int x,y,size = 0;
	short binpixel[32];
	char *cbinpixel = (char*)binpixel;
	unsigned char *pixel = image->pixels;
	blocx = 0;
	blocy = 0;


	//int nbloc = (image->w*image->h)/4,ibloc = 0;
	//unsigned long long *bloc  = malloc(nbloc * sizeof(unsigned long long));

	while(1)
	{
		bloc_palette(image,blocx,blocy,8,pixel,palette,tiles);

		if(type == 0) //2,4,8 bpp
		{
			for(y = 0;y <8;y++)
			{
				for(i = 0;i < 8;i++)
					snespixel[i] = 0;

				for(x = 0;x < 8;x++)
				{
					i = tiles[x + (y*8)];

					if(i > npal-1) i = 1+i%(npal-1);
					snespixel[0] += ( (i>>0) & 0x01 ) << (7 - x);
					snespixel[1] += ( (i>>1) & 0x01 ) << (7 - x);
					snespixel[2] += ( (i>>2) & 0x01 ) << (7 - x);
					snespixel[3] += ( (i>>3) & 0x01 ) << (7 - x);

					snespixel[4] += ( (i>>4) & 0x01 ) << (7 - x);
					snespixel[5] += ( (i>>5) & 0x01 ) << (7 - x);
					snespixel[6] += ( (i>>6) & 0x01 ) << (7 - x);
					snespixel[7] += ( (i>>7) & 0x01 ) << (7 - x);
				}

				binpixel[y]	= snespixel[0] + (snespixel[1]<<8);
				binpixel[y+8]  = snespixel[2] + (snespixel[3]<<8);
				binpixel[y+16] = snespixel[4] + (snespixel[5]<<8);
				binpixel[y+24] = snespixel[6] + (snespixel[7]<<8);

				//bloc[ibloc] = binpixel[y] + ((unsigned long long)binpixel[y+8]<<16) + ((unsigned long long)binpixel[y+16]<<32) + ((unsigned long long)binpixel[y+24]<<0);
				//ibloc++;
			}

			fwrite(binpixel,2 ,8,file);
			if(npal > 4)  fwrite(&binpixel[8],2 ,8,file);
			if(npal > 16) fwrite(&binpixel[16],2 ,16,file);

			size += 16;
			if(npal > 4) size += 16;
			if(npal > 16) size += 32;

		}else //8pbb mode 7
		{
			for(y = 0;y <8;y++)
			{
				for(x = 0;x < 8;x++)
				{
					i = tiles[x + (y*8)];
					cbinpixel[x + (y*8)] =  i;

				}
			}

			fwrite(cbinpixel,1 ,64,file);

			size += 64;
		}

		blocx += 8;
		if(blocx+8 >image->w)
		{
			blocx = 0;
			blocy += 8;
		}

		if(blocy+8 >image->h) break;
	}

	//nbloc = ibloc;
	//free(bloc);
	return size;
}
