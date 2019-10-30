#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <SDL/SDL.h>


#ifdef __MINGW32__
#undef main
#endif

void neogeo_extract(int argc, char* argv);
void neogeo_extract_fix(int argc, char* argv);


int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	char str[200];
	if(argc > 1) strcpy(str,argv[1]);
	else strcpy(str,"052-c1.bin");

	neogeo_extract(argc,str);
	neogeo_extract_fix(argc,str);

	//system("./neogeoextract 065-c1.c1");

	SDL_Quit();

	return 0;

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


void neogeo_extract(int argc, char* argv)
{
	SDL_Surface *image,*ecran;
	FILE *file1,*file2;
	int octet4[4],octet = 0,noctet = 0,i,l = 0,couleur,ocmp = 0,n = 0,x = 8 ,y = 0,ok;
	unsigned char *pixels;
	char chaine[100];
	SDL_Rect position;
	char str[300],sstr[200];


	if(argc == 1)
	{
		file1 = fopen("052-c1.bin","rb");
		file2 = fopen("052-c2.bin","rb");
	}else
	{

		ok = 0;
		strcpy(chaine,argv);

		i = 0;
		while(argv[i] != 0)
		{
			//printf("%c",argv[i]);
			if(argv[i] == 'c' && argv[i+1] == '1')
			{
				chaine[i+1] = '2';
				ok = 1;
			}

			if(argv[i] == 'c' && argv[i+1] == '3')
			{
				chaine[i+1] = '4';
				ok = 1;
			}

			if(argv[i] == 'c' && argv[i+1] == '5')
			{
				chaine[i+1] = '6';
				ok = 1;
			}

			if(argv[i] == 'c' && argv[i+1] == '7')
			{
				chaine[i+1] = '8';
				ok = 1;
			}

			i++;
		}

		if(ok == 0) return;

		printf("%s %s\n",argv,chaine);
		file1 = fopen(argv,"rb");
		file2 = fopen(chaine,"rb");
	}



	if(file1 == NULL || file2 == NULL)
	{
		printf("Open failed c1/c2 or c3/c4 or c5/c6 or c7/c8\n");
		return;
	}

	int size = 0;

	fseek(file1,0,SEEK_END);
	size = ftell(file1);
	fseek(file2,0,SEEK_END);
	size += ftell(file2);

	printf("size : %d bytes \n",size);

	fseek(file1,0,SEEK_SET);
	fseek(file2,0,SEEK_SET);

	int w = 256,h = 1;

	while( (size/256) > h)
	{
		h = h<<1;
	}

	printf("%d / %d\n",w,h);
	ecran = SDL_CreateRGBSurface(0,w,h,24,0,0,0,0);
	image = SDL_CreateRGBSurface(0,16,16,24,0,0,0,0);
	pixels = image->pixels;
	position.x = 0;
	position.y = 0;

	while(octet != -1)
	{

		octet = fgetc(file1);
		octet4[0] = octet;

		octet = fgetc(file1);
		octet4[1] = octet;

		octet = fgetc(file2);
		octet4[2] = octet;

		octet = fgetc(file2);
		octet4[3] = octet;

		//printf("%d %d %d\n",l,noctet,n);



		if(noctet == 0x20)
		{
			x = 0;
			y = 0;
		}

		l = x*3 + y*3*16;

		for(i = 0;i < 8;i++)
		{


			//printf("%d %d\n",noctet ,l);

			couleur = ( (octet4[0] >> i) & 0x01 )*1 + ( (octet4[1] >> i) & 0x01 )*2 + ( (octet4[2] >> i) & 0x01 )*4 + ( (octet4[3] >> i) & 0x01 )*8;
			//printf("%d : %d %x %x %x %x\n",l,couleur,octet4[0],octet4[1],octet4[2],octet4[3]);


			pixels[l+0] = 255-(couleur*16);
			pixels[l+1] = 255-(couleur*16);
			pixels[l+2] = 255-(couleur*16);


			l+=3;
		}


		y++;



		noctet +=2;
		if(noctet >= 0x40)
		{
			noctet = 0;
			l = 0;
			x = 8;
			y = 0;

			SDL_BlitSurface(image,NULL,ecran,&position);
			position.x += 16;
			if(position.x > w)
			{
				position.x = 0;
				position.y += 16;
			}

			if(position.y >= h)
			{
				position.x = 0;
				position.y = 0;
				break;
			}
		}

		//if(n == 128) break;
	}

	output_filename(argv,sstr);
	sprintf(str,"%s.bmp",sstr);
	SDL_SaveBMP(ecran,str);

	fclose(file1);
	fclose(file2);

	SDL_FreeSurface(image);

}

void neogeo_extract_fix(int argc, char* argv)
{
	SDL_Surface *image,*ecran;
	FILE *file1;
	int octet4[4],octet = 0,noctet = 0,i,l = 0,couleur,ocmp = 0,n = 0,x = 4 ,y = 0,ok;
	unsigned char *pixels;
	char chaine[100];
	SDL_Rect position;

	//file1 = fopen("052-s1.bin","rb");

	if(argc == 1)
	{
		file1 = fopen("052-s1.bin","rb");
	}else
	{
		ok = 0;

		i = 0;
		while(argv[i] != 0)
		{
			if(argv[i] == 's' && argv[i+1] == '1')
			{
				ok = 1;
			}
			i++;
		}

		if(ok == 0) return;
		strcpy(chaine,argv);
		file1 = fopen(chaine,"rb");
	}

	if(file1 == NULL)
	{
		printf("Open failed s1\n");
		return;
	}


	ecran = SDL_CreateRGBSurface(0,256,256,24,0,0,0,0);
	image = SDL_CreateRGBSurface(0,8,8,24,0,0,0,0);
	pixels = image->pixels;
	position.x = 0;
	position.y = 0;
	int ignore = 0;
	int nt = 0;

	while(octet != -1)
	{
		octet = fgetc(file1);
		octet4[0] = octet;


		if(noctet < 8) x = 4;
		else if(noctet < 16) x = 6;
		else if(noctet < 24) x = 0;
		else if(noctet < 32) x = 2;


		l = x*3 + y*3*8;

		//printf("%d %d	 %d %d\n",x, y ,noctet ,l);

		for(i = 0;i < 2;i++)
		{
			if(i == 0) couleur = octet4[0] & 0x0F;
			if(i == 1) couleur = (octet4[0] & 0xF0) >> 4;


			//printf("%d : %d %d	 %d %d  : %d\n",nt,x, y ,noctet ,l ,couleur);

			pixels[l+0] = 255-(couleur*16);
			pixels[l+1] = 255-(couleur*16);
			pixels[l+2] = 255-(couleur*16);

			l += 3;
		}
		y++;

		if(y >= 8) y = 0;

		noctet++;


		if(noctet >= 32)
		{

			SDL_BlitSurface(image,NULL,ecran,&position);

			position.x += 8;
			if(position.x >= 256)
			{
				position.x = 0;
				position.y += 8;
			}

			if(position.y >= 256)
			{
				position.x = 0;
				position.y = 0;
				sprintf(chaine,"fixsave%d.bmp",n);
				SDL_SaveBMP(ecran,chaine);
				n++;
			}

			noctet = 0;
			nt++;
		}



	}


	fclose(file1);
	SDL_FreeSurface(image);

}
