#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "retro.h"
#include "fceux.c"
#include "mesen.c"

void RGBtoCIELAB(int R,int G,int B,double *CIELAB)
{
	//http://www.easyrgb.com/en/math.php#text2
	double X,Y,Z;
	double var_R,var_G,var_B;
	double var_X,var_Y,var_Z;

	//RGB -> XYZ
	var_R = ( (float)R / 255.0 );
	var_G = ( (float)G / 255.0 );
	var_B = ( (float)B / 255.0 );

	if ( var_R > 0.04045 ) var_R = pow ( ( ( var_R + 0.055 ) / 1.055 ) , 2.4);
	else				   var_R = var_R / 12.92;
	if ( var_G > 0.04045 ) var_G = pow ( ( ( var_G + 0.055 ) / 1.055 ) , 2.4);
	else				   var_G = var_G / 12.92;
	if ( var_B > 0.04045 ) var_B = pow ( ( ( var_B + 0.055 ) / 1.055 ) , 2.4);
	else				   var_B = var_B / 12.92;

	var_R = var_R * 100;
	var_G = var_G * 100;
	var_B = var_B * 100;

	X = (var_R * 0.4124) + (var_G * 0.3576) + (var_B * 0.1805);
	Y = (var_R * 0.2126) + (var_G * 0.7152) + (var_B * 0.0722);
	Z = (var_R * 0.0193) + (var_G * 0.1192) + (var_B * 0.9505);


	//XYZ -> CIELAB

	var_X = X / 95.047;  // Reference-X
	var_Y = Y / 100.0;   // Reference-Y
	var_Z = Z / 108.883; // Reference-Z

	if ( var_X > 0.008856 ) var_X = pow ( var_X , ( 1.0/3.0 ) );
	else					var_X = ( 7.787 * var_X ) + ( 16.0 / 116.0 );
	if ( var_Y > 0.008856 ) var_Y = pow ( var_Y , ( 1.0/3.0 ) );
	else					var_Y = ( 7.787 * var_Y ) + ( 16.0 / 116.0 );
	if ( var_Z > 0.008856 ) var_Z = pow ( var_Z , ( 1.0/3.0 ) );
	else					var_Z = ( 7.787 * var_Z ) + ( 16.0 / 116.0 );

	CIELAB[0] = ( 116.0 * var_Y ) - 16.0;
	CIELAB[1] = 500.0 * ( var_X - var_Y );
	CIELAB[2] = 200.0 * ( var_Y - var_Z );

}

unsigned char nes_pal(unsigned char *palette,unsigned char *data)
{
	unsigned char color = 0x0F;
	int r,g,b,i;

	double CIELAB1[3],CIELAB2[3];
	double L,A,B,deltaE,deltaEmin = 10000.0f;

	r = palette[2];
	g = palette[1];
	b = palette[0];

	RGBtoCIELAB(r,g,b,CIELAB1);
	for(i = 0;i < 192;i += 3)
	{
		RGBtoCIELAB(data[i+0],data[i+1],data[i+2],CIELAB2);
		L =  (CIELAB2[0]-CIELAB1[0]);
		A =  (CIELAB2[1]-CIELAB1[1]);
		B =  (CIELAB2[2]-CIELAB1[2]);

		deltaE = sqrt((L*L)+(A*A)+(B*B));
		if(deltaE < deltaEmin)
		{
			deltaEmin = deltaE;
			color = i/3;
		}
	}


	return color;
}

int nes_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode)
{
	int i,n;
	int psize = 0;
	unsigned char color;


	int size = image->w*image->h*image->format->BytesPerPixel;
	unsigned char *pixel = image->pixels;

	if( (mode&0x0F) == 3 )
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

	unsigned char *data = fceux_pal_data;
	unsigned char data_tmp[192];

	if( (mode&0xF0) == 0x20 )
	{
		SDL_Surface *image2,*copy;
		image2 = IMG_Load("nes_custom.png");
		if(image2 == NULL)
		{
			printf("Palette is not valide\n");
			return 0;
		}

		copy = SDL_CreateRGBSurface(0,image2->w,image2->h,24,0,0,0,0);
		SDL_BlitSurface(image2,NULL,copy,NULL);
		pixel = copy->pixels;
		for(i = 0;i < 192;i+=3)
		{
			data_tmp[i+0] = pixel[i+2];
			data_tmp[i+1] = pixel[i+1];
			data_tmp[i+2] = pixel[i+0];
		}
		data = data_tmp;
		SDL_FreeSurface(copy);
		SDL_FreeSurface(image2);
	}

	if( (mode&0xF0) == 0x30 )
	{
		FILE *filepal = fopen("nes_custom.pal","wb");
		if(filepal == NULL)
		{
			printf("Palette is not valide\n");
			return 0;
		}

		for(i = 0;i < 192;i+=3)
		{
			data_tmp[i+0] = fgetc(filepal);
			data_tmp[i+1] = fgetc(filepal);
			data_tmp[i+2] = fgetc(filepal);
		}
		data = data_tmp;
		fclose(filepal);
	}

	if( (mode&0xF0) == 0x40 )
	{
		for(i = 0;i < 192;i+=3)
		{
			data_tmp[i+0] = mesen_pal_data[i+2];
			data_tmp[i+1] = mesen_pal_data[i+1];
			data_tmp[i+2] = mesen_pal_data[i+0];
		}
		data = data_tmp;
	}

	for(i = 0;i < ncolor;i++)
	{
		n = i*3;

		color = nes_pal(&palette[n],data);
		fputc(color,file);

		psize ++;
	}

	return psize;
}

int nes_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int npal,int type)
{
	int blocx,blocy;
	int tiles[64];
	int nespixel[8];
	int i;
	int x,y,size = 0;
	unsigned char bin[16];
	unsigned char *pixel = image->pixels;
	blocx = 0;
	blocy = 0;

	while(1)
	{
		bloc_palette(image,blocx,blocy,8,pixel,palette,tiles);



		for(y = 0;y <8;y++)
		{
			for(i = 0;i < 8;i++)
				nespixel[i] = 0;

			for(x = 0;x < 8;x++)
			{
				i = tiles[x + (y*8)];

				if(i > 3) i = 3;

				nespixel[0] += ( (i&0x01)>>0 ) << (7 - x);
				nespixel[1] += ( (i&0x02)>>1 ) << (7 - x);
			}

			bin[y+0] = nespixel[0] ;
			bin[y+8] = nespixel[1] ;

		}

		fwrite(bin,1 ,16,file);


		size += 16;
		blocx += 8;
		if(blocx+8 >image->w)
		{
			blocx = 0;
			blocy += 8;
		}

		if(blocy+8 >image->h) break;
	}


	return size;
}
