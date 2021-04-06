#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


#ifdef __MINGW32__
#undef main
#endif

#include "retro.h"

inline SDL_Rect convertRect(int x,int y,int w,int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

#define BLOC_CONVERT(X,Y,W,H) \
	rect = convertRect(X,Y,W,H); \
	SDL_BlitSurface(copy,&rect,copymap,NULL);  \
	retro_convert(copymap,address,addresspal,option,i);  \
	i++;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *image,*copy,*copymap;
	int i,ok = 0;

	int option[10];
	char address[500],addresspal[500];

	for(i = 0; i < 10;i++)
		option[i] = 0;

	strcpy(addresspal,"palette.png");
	address[0] = 0;

	for(i = 1; i < argc;i++)
	{
		if(argv[i][0] == '-')
		{
			if(strcmp(argv[i],"-nes") == 0) option[3] = 1;
			if(strcmp(argv[i],"-sms") == 0) option[3] = 2;
			if(strcmp(argv[i],"-pce") == 0) option[3] = 3;
			if(strcmp(argv[i],"-sfc") == 0) option[3] = 4;
			if(strcmp(argv[i],"-md")  == 0) option[3] = 5;
			if(strcmp(argv[i],"-ng")  == 0) option[3] = 6;

			if(strcmp(argv[i],"-gb")  == 0) option[3] = 7;

			if(strcmp(argv[i],"-noalpha") == 0) option[1] = 1;
			if(strcmp(argv[i],"-palette") == 0) option[2] = 1;
			if(strcmp(argv[i],"-paletteall") == 0) option[2] = 3;
			if(strcmp(argv[i],"-palettesp") == 0) option[7] = 1;
			if(strcmp(argv[i],"-tilemap")  == 0) option[9] = 3;

			//SNES option
			if(strcmp(argv[i],"-2bpp") == 0) option[0] = 4;
			if(strcmp(argv[i],"-4bpp") == 0) option[0] = 16;
			if(strcmp(argv[i],"-8bpp") == 0) option[0] = 256;

			if(strcmp(argv[i],"-map")   == 0) option[9] = 1;
			if(strcmp(argv[i],"-mode7") == 0) option[4] = 2;

			//PCE option
			if(strcmp(argv[i],"-bg")  == 0) option[4] = 2;
			if(strcmp(argv[i],"-spr") == 0) option[4] = 0;
			if(strcmp(argv[i],"-map2")   == 0) option[9] = 4;

			//Nes option
			if(strcmp(argv[i],"-customPal")  == 0) option[4] = 2;
			if(strcmp(argv[i],"-customBin")  == 0) option[4] = 3;
			if(strcmp(argv[i],"-customMesen")  == 0) option[4] = 4;

			//NG
			if(strcmp(argv[i],"-ngv")  == 0) option[4] = 2;

			ok = 0;

			if(strcmp(argv[i],"-loadpalette") == 0)
			{
				option[2] = 4;
				ok = 1;
			}

			//MD option
			if(strcmp(argv[i],"-h") == 0)
				ok = 2;

			//NG
			if(strcmp(argv[i],"-ngh")   == 0)
			{
				option[9] = 2;
				ok = 2;
			}

		}else
		{
			if(ok == 0) strcpy(address,argv[i]);
			if(ok == 1) strcpy(addresspal,argv[i]);
			if(ok == 2) option[5] = atoi(argv[i]);
			ok = 0;
		}
	}

	if(address[0] == 0)
	{
		printf("Enter a picture format .png,pcx,bmp, or jpg\n");
		printf("option target cible : -nes , -sms , -pce , -sfc , -md , -ng\n");
		printf("option palette :\n-palette (output palette only)\n-paletteall (output palette image)\n");
		printf("-loadpalette + arg , (load palette extern)\n");

		printf("\noption sfc : -2bpp,-4bpp ,-8bpp,-mode7,-map\n");
		printf("option pce : -bg,-spr\n");
		printf("option md : -h + arg\n");
		printf("option nes : -customBin (use Nes palette ,read 'nes_custom.pal' ) , -customPal (use Nes palette ,read 'nes_custom.png' ) , -customMesen (use Nes palette Mesen) \n");

		printf("\nExemple :\nRetroConvert -sfc -4bpp myimage.png\n");
		return 0;
	}

	image = IMG_Load(address);
	if(image == NULL)
	{
		printf("Image is not valide\n");
		return 0;
	}

	copy = SDL_CreateRGBSurface(0,image->w,image->h,24,0,0,0,0);

	SDL_Rect rect;
	SDL_BlitSurface(image,NULL,copy,NULL);




	if(option[9] != 0)
	{
		if(option[9] == 3)
			tilemap(copy,address);

		if(option[9] == 1)
		{

			copymap = SDL_CreateRGBSurface(0,128,64,24,0,0,0,0);
			rect.w = 128;
			rect.h = 64;
			for(i = 0; i < 7;i++)
			{
				if(i < 4)
				{
					rect.x = 0;
					rect.y = i*64;
				}else
				{
					rect.x = 128;
					rect.y = (i-4)*64;

					if(i == 6)
					{
						rect.h = 128;
						SDL_FreeSurface(copymap);
						copymap = SDL_CreateRGBSurface(0,128,128,24,0,0,0,0);
					}
				}

				SDL_BlitSurface(copy,&rect,copymap,NULL);
				retro_convert(copymap,address,addresspal,option,i);
				//sprintf(str,"test_%d.bmp",i);
				//SDL_SaveBMP(copy,str);
			}
			SDL_FreeSurface(copymap);
		}


		if(option[9] == 2)
		{
			if(option[5] == 0) option[5] = 16;
			int tmp = option[5]*16;
			copymap = SDL_CreateRGBSurface(0,image->w,option[5]*16,24,0,0,0,0);

			rect.w = image->w;
			rect.h = tmp;

			option[9] = 4;
			retro_convert(copy,address,addresspal,option,0);
			option[8] = 1;
			for(i = 0; i < image->h;i+=tmp)
			{
				if(i == 0) option[9] = 2;
				else option[9] = 3;

				rect.x = 0;
				rect.y = i;

				SDL_BlitSurface(copy,&rect,copymap,NULL);
				retro_convert(copymap,address,addresspal,option,i>>5);
/*
				char str[50];
				sprintf(str,"test_%d.bmp",i>>5);
				SDL_SaveBMP(copymap,str);*/
			}


			SDL_FreeSurface(copymap);
		}

		if(option[9] == 4)
		{
			option[9] = 1;
			i = 0;
			copymap = SDL_CreateRGBSurface(0,128,64,24,0,0,0,0);
			BLOC_CONVERT(0,0,128,64);
			BLOC_CONVERT(0,64,128,64);
			SDL_FreeSurface(copymap);

			copymap = SDL_CreateRGBSurface(0,128,32,24,0,0,0,0);
			BLOC_CONVERT(0,128,128,32);
			BLOC_CONVERT(0,128+(32*1),128,32);
			BLOC_CONVERT(0,128+(32*2),128,32);
			BLOC_CONVERT(0,128+(32*3),128,32);
			BLOC_CONVERT(0,128+(32*4),128,32);

			copymap = SDL_CreateRGBSurface(128,128,64,24,0,0,0,0);
			BLOC_CONVERT(128,0,128,64);
			BLOC_CONVERT(128,64,128,64);
			SDL_FreeSurface(copymap);

			copymap = SDL_CreateRGBSurface(128,128,32,24,0,0,0,0);
			BLOC_CONVERT(128,128,128,32);
			BLOC_CONVERT(128,128+(32*1),128,32);
			BLOC_CONVERT(128,128+(32*2),128,32);
			BLOC_CONVERT(128,128+(32*3),128,32);

			copymap = SDL_CreateRGBSurface(128,128,16,24,0,0,0,0);
			BLOC_CONVERT(128,256,128,16);
			BLOC_CONVERT(128,256+16,128,16);
		}

	}
	else
		retro_convert(copy,address,addresspal,option,0);

	//SDL_SaveBMP(image,"test.bmp");
	SDL_FreeSurface(copy);
	SDL_FreeSurface(image);
	SDL_Quit();

	return 0;

}


void retro_convert(SDL_Surface *image,char *address,char *addresspal,int *option,int num)
{
	FILE *file;
	int ncolor = 0,type = 0;
	int size = 0,psize = 0;
	char str[300],sstr[200];

	unsigned char palette[0x300];
	static unsigned char palette_buf[0x300];
	static int ncolor_buf;

	int console = option[3];
	int mode = option[2];
	int bpp  = option[0];

	if(option[4] == 2) type = 1;

	if(option[7] == 0)
		ncolor = load_palette(image,palette,option[1]);
	else
		ncolor = load_palette_sp(image,palette,option[1]);

	//-------------------------------
	if(mode == 4)
	{
		ncolor = load_palette_ext(palette,addresspal);
		mode = 0;
	}

	int i;

	if(option[8] == 1)
	{
		for(i = 0; i < 0x300;i++)
			palette[i] = palette_buf[i];
		ncolor = ncolor_buf;
	}else
	{
		for(i = 0; i < 0x300;i++)
			palette_buf[i] = palette[i];
		ncolor_buf = ncolor;
	}

	if(option[9] == 4)
		return;

	//-------------------------------
	if(option[9] < 3)
		printf("color : %d\n",ncolor);
	if(bpp == 4) ncolor = 4;
	if(bpp == 16) ncolor = 16;
	if(bpp == 256) ncolor = 256;

	output_filename(address,sstr);
	//-------------------------------

	sprintf(str,"%s_%d.bmp",sstr,num);
	//SDL_SaveBMP(image,str);

	//Sprite
	if(option[9] == 1)
		sprintf(str,"%s_%d.spr",sstr,num);
	else
		sprintf(str,"%s.spr",sstr);




	if(option[9] == 3)
		file = fopen(str,"ab");
	else
		file = fopen(str,"wb");

	if(file == NULL)
	{
		printf("Write failed\n");
		return;
	}
	if(mode == 0)
	{
		if(console == 1)
			size = nes_write_rom(file,image,palette,ncolor,type);

		if(console == 2)
			size = sms_write_rom(file,image,palette,ncolor,type);

		if(console == 3)
			size = pce_write_rom(file,image,palette,ncolor,type);

		if(console == 4)
			size = snes_write_rom(file,image,palette,ncolor,type);

		if(console == 5)
			size = md_write_rom(file,image,palette,ncolor,option[5]);

		if(console == 6)
			size = ng_write_rom(file,image,palette,ncolor,type);

		if(console == 7)
			size = gb_write_rom(file,image,palette,ncolor,option[1]);
	}

	fclose(file);

	if(option[9] >= 3)
	{
		printf("size sprites : %d bytes\n",size);
		return;
	}

	//Palette
	if(option[9] == 1)
		sprintf(str,"%s_%d.pal",sstr,num);
	else
		sprintf(str,"%s.pal",sstr);

	if(console != 7)
		file = fopen(str,"wb");
	if(file == NULL)
	{
		printf("Write failed\n");
		return;
	}
	if(console == 1)
		psize = nes_write_pal(file,image,palette,ncolor,mode+(option[4]<<4));
	if(console == 2)
		psize = sms_write_pal(file,image,palette,ncolor,mode);
	if(console == 3)
		psize = pce_write_pal(file,image,palette,ncolor,mode);
	if(console == 4)
		psize = snes_write_pal(file,image,palette,ncolor,mode);
	if(console == 5)
		psize = md_write_pal(file,image,palette,ncolor,mode);
	if(console == 6)
		psize = ng_write_pal(file,image,palette,ncolor,mode);
	if(console != 7)
		fclose(file);

	printf("size sprites : %d bytes\n",size);
	printf("size palette : %d bytes\n",psize);
}

void output_filename(char *address,char *str)
{
	int l = 0;
	int i = 0;
	while(address[i] != 0 && address[i] != '.' )
	{
		str[l] = address[i];
		l++;

		if(address[i] == '/' || address[i] == '\\') l = 0;
		i++;
	}
	str[l] = 0;
}


