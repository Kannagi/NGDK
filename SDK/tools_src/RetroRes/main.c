#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aura.h"


typedef struct
{
	char str_C1[20];
	char str_C2[20];
	char pathRomK[200];
	char pathRetroConvert[200];
	char pathBin2c[200];
	char pathNGextract[200];
	char pathRetroTMX[200];
	char dir[250];
}COMMAND;


int file_test(char *strin1,char *strin2)
{
	char str[400];
	FILE *file;
	sprintf(str,"%s.%s",strin1,strin2);
	file = fopen(str,"r");
	if(file == NULL) return 0;
	fclose(file);

	strcpy(strin1,str);

	return 1;
}


int file_size_spr(char *strin)
{
	char str[400];
	FILE *file;
	sprintf(str,"%s.spr",strin);
	file = fopen(str,"r");
	if(file == NULL) return 0;
	fseek(file,0,SEEK_END);
	int size = ftell(file);

	fclose(file);

	return size/128;
}
void init(COMMAND *command,char *path, int os)
{
    char ext[5];
	if(os == 0)
    {
	    strcpy(ext, "");
    }else
	{
	    strcpy(ext, ".exe");
	}

    sprintf(command->pathRetroConvert,"%s%s%s", path, "RetroConvert", ext);
    sprintf(command->pathRomK,"%s%s%s", path, "RomK", ext);
    sprintf(command->pathBin2c,"%s%s%s", path, "bin2c", ext);
    sprintf(command->pathNGextract,"%s%s%s", path, "neogeoextract", ext);
    sprintf(command->pathRetroTMX,"%s%s%s", path, "RetroTMX", ext);
}

int main(int argc, char** argv)
{
	FILE *file,*filemaptile,*efilemaptile;
	int i,n,l;
	int log = 1;

	char str[1000];
	char str2[210];
	char copystr[200];
	char option[200];
	char path[256];

	if(argc < 1) return 0;
	sprintf(path,"%s",argv[1]);

#ifdef LINUX
    if(path[strlen(path)-1]!='/') {
        path[strlen(path)] = '/';
        path[strlen(path)+1] = 0;
    }
#endif // LINUX

#ifdef WINDOWS
    if(path[strlen(path)-1]!='\\') {
        path[strlen(path)] = '\\';
        path[strlen(path)+1] = 0;
    }
#endif // WINDOWS

	sprintf(str,"%s","rescompile.txt");
	file = fopen(str,"r");
	if(file == NULL) return 0;

	filemaptile = fopen("data.h","w");
	if(filemaptile == NULL) return 0;

	efilemaptile = fopen("data_ext.h","w");
	if(efilemaptile == NULL) return 0;

	fputs("#ifndef _NG_DATA_\n#define _NG_DATA_\n",filemaptile);
	fputs("#ifndef _NG_DATA_EXT_\n#define _NG_DATA_EXT_\n",efilemaptile);



	fseek(file, 0, SEEK_END);
	unsigned long int size = ftell(file);

	char *buffer = malloc(size+1);
	if(buffer == NULL) return 0;
	fseek(file, 0, SEEK_SET);
	fread(buffer,1,size,file);
	fclose(file);

	AURA aura;
	AURA_Init(&aura);

	COMMAND command;
	float version = 0.8;
#ifdef LINUX
	printf("linux version : %.2f\n",version);
	init(&command,path, 0);
#endif

#ifdef WINDOWS
	printf("windows version : %.2f\n",version);
	init(&command,path, 1);
#endif

	n = size-1;
	int type = 0;
	int arg = 0;
	int bloc = 1;

	for(i = 0;i < n;i++)
	{
		int letter = buffer[i];

		AURA_Analyse_Com(letter,&aura);
		AURA_Analyse_R(letter,&aura);
		AURA_Analyse_Text(letter,&aura);
		if(AURA_Analyse_Word(letter,&aura) == 1)
		{

			if(arg == 0)
			{
				if(strcmp("PATHRomK",aura.str) == 0) type = 1;
				if(strcmp("PATHRetroConvert",aura.str) == 0) type = 2;
				if(strcmp("PATHBin2c",aura.str) == 0) type = 10;
				if(strcmp("PATHNGextract",aura.str) == 0) type = 11;
				if(strcmp("PATHRRetroTMX",aura.str) == 0) type = 12;

				if(strcmp("SIZE",aura.str) == 0) type = 3;
				if(strcmp("BG",aura.str) == 0) type = 4;
				if(strcmp("SPR",aura.str) == 0) type = 5;
				if(strcmp("PAL",aura.str) == 0) type = 9;
				if(strcmp("TMX",aura.str) == 0) type = 6;
				if(strcmp("BIN",aura.str) == 0) type = 8;
				option[0] = 0;

				if(type > 0)
					arg = 1;

			}else
			{

				strcpy(copystr,aura.str);
				//printf("%s\n",aura.str);
				//------------------------
				if(type == 3)
				{
					sprintf(str,"%s -rom8k rom.bin %d",command.pathRomK,atoi(aura.str));
					if(log != 0) printf("%s\n",str);
					system(str);
				}
				//------------------------
				if(type == 4)
				{
					int test = 0;
					test += file_test(aura.str,"bmp");
					test += file_test(aura.str,"pcx");
					test += file_test(aura.str,"png");

					if(test == 0)
						printf("Error can not open %s\n",aura.str);
					else
					{
						sprintf(str,"%s -ng -ngv %s %s",command.pathRetroConvert,option,aura.str);
						if(log != 0) printf("%s\n",str);
						system(str);

						sprintf(str,"%s -insertbloc rom.bin %s.spr %d",command.pathRomK,copystr,bloc);
						if(log != 0) printf("%s\n",str);
						system(str);

						sprintf(str,"#define %s_index %d\n",copystr,bloc);
						fputs(str,filemaptile);
						fputs(str,efilemaptile);

						bloc += file_size_spr(copystr);

						sprintf(str,"%s %s.pal %s.c %s_pal",command.pathBin2c,copystr,copystr,copystr);
						if(log != 0) printf("%s\n",str);
						system(str);

						//-----------------
						sprintf(str,"#include \"%s.c\"\n",copystr);
						fputs(str,filemaptile);

						sprintf(str,"extern u8 %s_pal_data[];\n",copystr);
						fputs(str,efilemaptile);
						//-----------------


						sprintf(str,"%s.spr",copystr);
						remove(str);

						sprintf(str,"%s.pal",copystr);
						remove(str);

					}
				}

				//------------------------
				if(type == 5)
				{
					int test = 0;
					test += file_test(aura.str,"bmp");
					test += file_test(aura.str,"pcx");
					test += file_test(aura.str,"png");

					if(test == 0)
						printf("Error can not open %s\n",aura.str);
					else
					{
						sprintf(str,"%s -ng %s %s",command.pathRetroConvert,option,aura.str);
						if(log != 0) printf("%s\n",str);
						system(str);

						sprintf(str,"%s -insertbloc rom.bin %s.spr %d",command.pathRomK,copystr,bloc);
						if(log != 0) printf("%s\n",str);
						system(str);

						sprintf(str,"#define %s_index %d\n",copystr,bloc);
						fputs(str,filemaptile);
						fputs(str,efilemaptile);

						bloc += file_size_spr(copystr);

						sprintf(str,"%s %s.pal %s.c %s_pal",command.pathBin2c,copystr,copystr,copystr);
						if(log != 0) printf("%s\n",str);
						system(str);

						//-----------------
						sprintf(str,"#include \"%s.c\"\n",copystr);
						fputs(str,filemaptile);

						sprintf(str,"extern u8 %s_pal_data[];\n",copystr);
						fputs(str,efilemaptile);
						//-----------------


						sprintf(str,"%s.spr",copystr);
						remove(str);

						sprintf(str,"%s.pal",copystr);
						remove(str);

					}
				}
				//------------------------
				if(type == 9)
				{
					int test = 0;
					test += file_test(aura.str,"bmp");
					test += file_test(aura.str,"pcx");
					test += file_test(aura.str,"png");

					if(test == 0)
						printf("Error can not open %s\n",aura.str);
					else
					{
						sprintf(str,"%s -ng %s %s",command.pathRetroConvert,option,aura.str);
						if(log != 0) printf("%s\n",str);
						system(str);

						sprintf(str,"%s %s.pal %s.c %s_pal",command.pathBin2c,copystr,copystr,copystr);
						if(log != 0) printf("%s\n",str);
						system(str);

						//-----------------
						sprintf(str,"#include \"%s.c\"\n",copystr);
						fputs(str,filemaptile);

						sprintf(str,"extern u8 %s_pal_data[];\n",copystr);
						fputs(str,efilemaptile);
						//-----------------

						sprintf(str,"%s.spr",copystr);
						remove(str);

						sprintf(str,"%s.pal",copystr);
						remove(str);

					}
				}
				//------------------------
				if(type == 6)
				{

					sprintf(str,"%s -ng %s %s.tmx",command.pathRetroTMX,option,copystr);
					if(log != 0) printf("%s\n",str);
					system(str);

					int test = 0;
					for(l = 0;l < 3;l++)
					{
						if(l == 0) sprintf(str2,"%s_A",copystr);
						if(l == 1) sprintf(str2,"%s_B",copystr);
						if(l == 2) sprintf(str2,"%s_C",copystr);
						strcpy(aura.str,str2);

						test = file_test(aura.str,"ktm");

						if(test != 0)
						{
							sprintf(str,"%s %s.ktm %s.c %s",command.pathBin2c,str2,str2,str2);
							if(log != 0) printf("%s\n",str);
							system(str);

							sprintf(str,"#include \"%s.c\"\n",str2);
							fputs(str,filemaptile);

							sprintf(str,"extern u8 %s_data[];\n",str2);
							fputs(str,efilemaptile);

							sprintf(str,"%s.ktm",copystr);
							remove(str);
						}
					}



					//-----------------
				}
				//------------------------
				if(type == 7)
				{

				}
				//------------------------
				if(type == 8)
				{

					sprintf(str,"%s %s.bin %s.c %s",command.pathBin2c,copystr,copystr,copystr);
					if(log != 0) printf("%s\n",str);
					system(str);

					//-----------------
					sprintf(str,"#include \"%s.c\"\n",copystr);
					fputs(str,filemaptile);

					sprintf(str,"extern u8 %s_data[];\n",copystr);
					fputs(str,efilemaptile);
					//-----------------

				}
				arg++;
			}

		}

		if(aura.com == 2)
		{
			strcpy(option,aura.text);

			if(type == 1)
				strcpy(command.pathRomK,aura.text);

			if(type == 2)
				strcpy(command.pathRetroConvert,aura.text);

			if(type == 10)
				strcpy(command.pathBin2c,aura.text);

			if(type == 11)
				strcpy(command.pathNGextract,aura.text);

			if(type == 12)
				strcpy(command.pathRetroTMX,aura.text);
		}

		if(aura.rinst == 0)
		{
			type = 0;
			arg = 0;
			option[0] = 0;
		}
	}

	sprintf(str,"%s -divrom2 rom.bin 2",command.pathRomK);
	if(log != 0) printf("%s\n",str);
	system(str);

	rename("rom0.bin","052-c1.bin");
	rename("rom1.bin","052-c2.bin");


/*
	sprintf(str,"%s",command.pathNGextract);
	if(log != 0) printf("%s\n",str);
	system(str);

*/


	//---------------------
	/*
	fputs("const u16 Map_tile_BG[] ={",filemaptile);
	for(i = 0;i < nmaptile;i++)
	{
		sprintf(str,"0x%x,",maptile[i]);
		fputs(str,filemaptile);
	}
	fputs("};\n",filemaptile);
	fputs("extern u16 Map_tile_BG[];\n",efilemaptile);

	fputs("const u16 Map_tile_SPR[] ={",filemaptile);
	for(i = 0;i < nmaptilespr;i++)
	{
		sprintf(str,"0x%x,",maptilespr[i]);
		fputs(str,filemaptile);
	}
	fputs("};\n",filemaptile);
	fputs("extern u16 Map_tile_SPR[];\n",efilemaptile);
*/
	//---------------------


	//---------------------
	fputs("#endif\n",filemaptile);
	fputs("#endif\n",efilemaptile);

	fclose(filemaptile);
	fclose(efilemaptile);

	free(buffer);


    return 0;
}
