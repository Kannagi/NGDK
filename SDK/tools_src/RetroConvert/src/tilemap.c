#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"



void tilemap(SDL_Surface *imagein,char *address)
{
	int w = imagein->w,h = imagein->h;
	while(w > 512)
	{
		h = h<<1;
		w = w>>1;
	}

	SDL_Surface *copy = SDL_CreateRGBSurface(0,w,h,16,0,0,0,0);
	SDL_Surface *image = SDL_CreateRGBSurface(0,imagein->w,imagein->h,16,0,0,0,0);
	SDL_FillRect(copy, NULL, SDL_MapRGB(copy->format, 255, 0, 255));


	SDL_BlitSurface(imagein,NULL,image,NULL);


	int i,l;
	unsigned int *pixel = image->pixels;
	unsigned int *pixelbloc;

	int nblocx =(image->w/16);
	int nblocy =(image->h/16);
	int nbloc = nblocx*nblocy;
	int x,y,cx,cy;

	printf("%d %d %d\n",nbloc,nblocx,nblocy);
	int *ibloc = malloc(nbloc*sizeof(int));

	for(x = 0;x < nbloc;x++)
		ibloc[x] = 0;

	int ix,iy,n = 0;
	unsigned int pixel1[256],pixel2[256];
	int pitchw = (image->w/2);
	int pitch = (image->w/2);
/*
	int cw = ( (image->w*3)*16 ) - 48;
	int cw2 = ( (image->w*3)*15 );
*/
	pixelbloc = pixel;
	int j = 0;
	int px = 0,py = 0;
	x = 0;
	y = 0;



	for(y = 0;y < image->h;y += 16)
	{
		//printf("line %d\n",y);
		for(x = 0;x < image->w;x += 16)
		{

			i = 0;
			l = 0;
			pixelbloc = pixel + (( x + (y*image->w) )>>1);
			for(iy = 0;iy <16;iy++)
			{
				pixel1[i+0] = pixelbloc[l+0];
				pixel1[i+1] = pixelbloc[l+1];
				pixel1[i+2] = pixelbloc[l+2];
				pixel1[i+3] = pixelbloc[l+3];
				pixel1[i+4] = pixelbloc[l+4];
				pixel1[i+5] = pixelbloc[l+5];
				pixel1[i+6] = pixelbloc[l+6];
				pixel1[i+7] = pixelbloc[l+7];

				i += 8;
				l += pitchw;
			}

			for(cy = 0;cy < image->h;cy += 16)
			{
				for(cx = 0;cx < image->w;cx += 16)
				{
					l = 0;
					i = 0;
					pixelbloc = pixel + (( cx + (cy*image->w) )>>1);
					for(iy = 0;iy <16;iy++)
					{
						pixel2[i+0] = pixelbloc[l+0];
						pixel2[i+1] = pixelbloc[l+1];
						pixel2[i+2] = pixelbloc[l+2];
						pixel2[i+3] = pixelbloc[l+3];
						pixel2[i+4] = pixelbloc[l+4];
						pixel2[i+5] = pixelbloc[l+5];
						pixel2[i+6] = pixelbloc[l+6];
						pixel2[i+7] = pixelbloc[l+7];

						i += 8;
						l += pitchw;
					}

					for(i = 0;i < 128;i++)
					{
						if(pixel1[i] != pixel2[i])
							break;
					}

					if(i == 128)
					{
						ibloc[n] = ( cx + ( (cy>>4)*image->w) )>>4;
						cx = cy = 1000000;
					}
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
	i = 0;
	l = 0;


	for(i = 0;i < nbloc;i++)
	{
		//printf("%d,",ibloc[i]);
		n = ibloc[i];

		if(ibloc[i] == i)
		{
			rect.x = n%nblocx;
			rect.x *= 16;

			rect.y = n/nblocx;
			rect.y *= 16;

			SDL_BlitSurface(image,&rect,copy,&pos);
			pos.x += 16;
			if(pos.x > w)
			{
				pos.x = 0;
				pos.y += 16;
			}
			ibloc[i] = l;
			l++;
		}
	}


	printf("%d bloc\n",l);


	copy->h = pos.y+16;

	char sstr[300],str[200];

	output_filename(address,str);
	sprintf(sstr,"%s_tilemap.bmp",str);
	SDL_SaveBMP(copy,sstr);

	sprintf(sstr,"%s_tilemap.bin",str);
	FILE *file = fopen(sstr,"wb");

	for(x = 0;x < nblocx;x ++)
	for(y = 0;y < nblocy;y ++)
	{
		i = x + (y*nblocx);
		n = ibloc[i];

		fputc(n>>8,file);
		fputc(n,file);
	}

	fclose(file);

}

void tilemap8(SDL_Surface *imagein,char *address)
{
	int w = imagein->w,h = imagein->h;

	SDL_Surface *copy = SDL_CreateRGBSurface(0,w,h,16,0,0,0,0);
	SDL_Surface *image = SDL_CreateRGBSurface(0,imagein->w,imagein->h,16,0,0,0,0);
	SDL_FillRect(copy, NULL, SDL_MapRGB(copy->format, 255, 0, 255));


	SDL_BlitSurface(imagein,NULL,image,NULL);


	int i,l;
	unsigned int *pixel = image->pixels;
	unsigned int *pixelbloc;

	int nblocx =(image->w/8);
	int nblocy =(image->h/8);
	int nbloc = nblocx*nblocy;
	int x,y,cx,cy;

	printf("%d %d %d\n",nbloc,nblocx,nblocy);
	int *ibloc = malloc(nbloc*sizeof(int));

	for(x = 0;x < nbloc;x++)
		ibloc[x] = 0;

	int ix,iy,n = 0;
	unsigned int pixel1[256],pixel2[256];
	int pitchw = (image->w/2);

	pixelbloc = pixel;
	int j = 0;
	int px = 0,py = 0;
	x = 0;
	y = 0;


	for(y = 0;y < image->h;y += 8)
	{
		//printf("line %d\n",y);
		for(x = 0;x < image->w;x += 8)
		{

			i = 0;
			l = 0;
			pixelbloc = pixel + (( x + (y*image->w) )>>1);
			for(iy = 0;iy <8;iy++)
			{
				pixel1[i+0] = pixelbloc[l+0];
				pixel1[i+1] = pixelbloc[l+1];
				pixel1[i+2] = pixelbloc[l+2];
				pixel1[i+3] = pixelbloc[l+3];

				i += 4;
				l += pitchw;
			}

			for(cy = 0;cy < image->h;cy += 8)
			{
				for(cx = 0;cx < image->w;cx += 8)
				{
					l = 0;
					i = 0;
					pixelbloc = pixel + (( cx + (cy*image->w) )>>1);
					for(iy = 0;iy <8;iy++)
					{
						pixel2[i+0] = pixelbloc[l+0];
						pixel2[i+1] = pixelbloc[l+1];
						pixel2[i+2] = pixelbloc[l+2];
						pixel2[i+3] = pixelbloc[l+3];

						i += 4;
						l += pitchw;
					}

					for(i = 0;i < 32;i++)
					{
						if(pixel1[i] != pixel2[i])
							break;
					}

					if(i == 32)
					{
						ibloc[n] = ( cx + ( (cy>>3)*image->w) )>>3;
						cx = cy = 1000000;
					}
				}

			}

			n++;
		}

	}

	n =0;

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 8;
	rect.h = 8;

	SDL_Rect pos;
	pos.x = 0;
	pos.y = 0;
	i = 0;
	l = 0;


	for(i = 0;i < nbloc;i++)
	{
		//printf("%d,",ibloc[i]);
		n = ibloc[i];

		if(ibloc[i] == i)
		{
			rect.x = n%nblocx;
			rect.x *= 8;

			rect.y = n/nblocx;
			rect.y *= 8;

			SDL_BlitSurface(image,&rect,copy,&pos);
			pos.x += 8;
			if(pos.x > w)
			{
				pos.x = 0;
				pos.y += 8;
			}
			ibloc[i] = l;
			l++;
		}
	}


	printf("%d bloc\n",l);


	copy->h = pos.y+8;

	char sstr[300],str[200];

	output_filename(address,str);
	sprintf(sstr,"%s_tilemap.bmp",str);
	SDL_SaveBMP(copy,sstr);

	sprintf(sstr,"%s_tilemap.bin",str);
	FILE *file = fopen(sstr,"wb");

	for(x = 0;x < nblocx;x ++)
	for(y = 0;y < nblocy;y ++)
	{
		i = x + (y*nblocx);
		n = ibloc[i];

		fputc(n>>8,file);
		fputc(n,file);
	}

	fclose(file);

}
