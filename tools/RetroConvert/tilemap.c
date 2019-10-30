#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"

void tilemap(SDL_Surface *image)
{
	int w = image->w,h = image->h;
	while(h > 256)
	{
		h = h>>1;
		w = w<<1;
	}

	SDL_Surface *copy = SDL_CreateRGBSurface(0,w,h,24,0,0,0,0);

    int i,l;
    unsigned char *pixel = image->pixels;
    unsigned char *pixelbloc;

	int nblocx =(image->w/16);
	int nblocy =(image->h/16);
	int nbloc = nblocx*nblocy;
	int x,y,cx,cy;

	printf("%d %d %d\n",nbloc,nblocx,nblocy);
	int *ibloc = malloc(nbloc*sizeof(int));

	for(x = 0;x < nbloc;x++)
		ibloc[x] = 0;

	int ix,iy,n = 0;
	int pixel1[256],pixel2[256];
	int pitchw = (image->w*3)-48;
/*
	int cw = ( (image->w*3)*16 ) - 48;
	int cw2 = ( (image->w*3)*15 );
*/
	for(y = 0;y < image->h;y += 16)
	{
		printf("ligne %d\n",y);
		for(x = 0;x < image->w;x += 16)
		{
			pixelbloc = pixel + (( x + (y*image->w) )*3);
			i = 0;
			l = 0;
			for(iy = 0;iy <16;iy++)
			{
				for(ix = 0;ix < 16;ix++)
				{
					pixel1[i] = pixelbloc[l+0] + (pixelbloc[l+1]<<8) + (pixelbloc[l+2]<<16);
					i++;
					l+=3;
				}
				l += pitchw;
			}


			l = 0;

			for(cy = 0;cy < image->h;cy += 16)
			{
				for(cx = 0;cx < image->w;cx += 16)
				{

					pixelbloc = pixel + (( cx + (cy*image->w) )*3);
					i = 0;
					l = 0;
					for(iy = 0;iy <16;iy++)
					{
						for(ix = 0;ix < 16;ix++)
						{
							pixel2[i] = pixelbloc[l+0] + (pixelbloc[l+1]<<8) + (pixelbloc[l+2]<<16);
							i++;
							l+=3;
						}
						l += pitchw;
					}

					for(i = 0;i < 256;i++)
					{
						if(pixel1[i] != pixel2[i])
							break;
					}

					if(i == 256)
						ibloc[n]++;

				}

			}

			n++;
		}

	}

	n =0;

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 16;
	rect.h = 16;

	SDL_Rect pos;
	pos.x = 0;
	pos.y = 0;
	for(i = 0;i < nbloc;i++)
	{
		//printf("%d,",ibloc[i]);
		if(ibloc[i] == 1)
		{
			rect.x = i%nblocx;
			rect.x *= 16;

			rect.y = i/nblocx;
			rect.y *= 16;



			SDL_BlitSurface(image,&rect,copy,&pos);
			pos.x += 16;
			if(pos.x > w)
			{
				pos.x = 0;
				pos.y += 16;
			}
			n++;
		}
	}


	//SDL_BlitSurface(copy,&rect,bloc,NULL);
	printf("\n%d\n",n);

	SDL_SaveBMP(copy,"ok.bmp");

}

