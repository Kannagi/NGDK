#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aura.h"

#include "tmx.h"

void Load_TMX(TMX *tmx,char *address)
{
	FILE *file = fopen(address,"rb");
	if(file == NULL)
		return;

	fseek(file, 0, SEEK_END);
	unsigned long int size = ftell(file);

	char *buffer = malloc(size+1);
	if(buffer == NULL) return;
	fseek(file, 0, SEEK_SET);
	fread(buffer,1,size,file);
	fclose(file);

	int i = 0,n = size-1;
	AURA aura;
	AURA_Init(&aura);

	int arg = 0;
	int type = 0;

	int l = 0;
	int j = 0;
	int k,kn = 0;

	for(i = 0;i < n;i++)
	{
		int letter = buffer[i];

		AURA_Analyse_R(letter,&aura);
		AURA_Analyse_Text(letter,&aura);
		if(AURA_Analyse_Word(letter,&aura) == 1)
		{
			if(aura.bloc[0] > 0)
			{

				if(strcmp("xml",aura.str) == 0) type = 1;
				if(strcmp("map",aura.str) == 0) type = 2;
				if(strcmp("tileset",aura.str) == 0) type = 3;
				if(strcmp("image",aura.str) == 0) type = 4;
				if(strcmp("layer",aura.str) == 0) type = 5;
				if(strcmp("data",aura.str) == 0) type = 6;

				if(strcmp("version",aura.str) == 0) arg = 1;
				if(strcmp("encoding",aura.str) == 0) arg = 2;

				if(strcmp("tilewidth",aura.str) == 0) arg = 3;
				if(strcmp("tileheight",aura.str) == 0) arg = 4;
				if(strcmp("width",aura.str) == 0) arg = 5;
				if(strcmp("height",aura.str) == 0) arg = 6;

				if(strcmp("tiledversion",aura.str) == 0) arg = 7;
				if(strcmp("orientation",aura.str) == 0) arg = 8;
				if(strcmp("renderorder",aura.str) == 0) arg = 9;
				if(strcmp("infinite",aura.str) == 0) arg = 10;
				if(strcmp("nextobjectid",aura.str) == 0) arg = 11;
				if(strcmp("firstgid",aura.str) == 0) arg = 12;
				if(strcmp("name",aura.str) == 0) arg = 13;
				if(strcmp("tilecount",aura.str) == 0) arg = 14;
				if(strcmp("columns",aura.str) == 0) arg = 15;
				if(strcmp("source",aura.str) == 0) arg = 16;
				if(strcmp("trans",aura.str) == 0) arg = 17;



			}else
			{

				if(kn > 0)
				{
					tmx->layer[j].data[k] = atoi(aura.str);
					k++;
					kn--;
				}
			}


		}


		//Texte
		if(aura.com == 2)
		{

			int option = 0;
			if(strcmp("left-up",aura.text) == 0) option = 1;
			if(strcmp("orthogonal",aura.text) == 0) option = 1;
			if(strcmp("csv",aura.text) == 0) option = 1;
			if(strcmp("UTF-8",aura.text) == 0) option = 1;


			//xml
			if(type == 1)
			{

				if(arg == 1)
					tmx->xmlversion = atof(aura.text);

				if(arg == 2)
					tmx->encoding = option;

			}

			//map
			if(type == 2)
			{

				if(arg == 1)
					tmx->map.version = atof(aura.text);

				if(arg == 3)
					tmx->map.tilewidth = atoi(aura.text);

				if(arg == 4)
					tmx->map.tileheight = atoi(aura.text);

				if(arg == 5)
					tmx->map.width = atoi(aura.text);

				if(arg == 6)
					tmx->map.height = atoi(aura.text);

				if(arg == 7)
					tmx->map.tiledversion = atof(aura.text);

				if(arg == 8)
					tmx->map.orientation = option;

				if(arg == 9)
					tmx->map.renderorder = option;

				if(arg == 10)
					tmx->map.infinite = atoi(aura.text);

				if(arg == 11)
					tmx->map.nextobjectid = atoi(aura.text);

			}

			//tileset
			if(type == 3)
			{

				if(arg == 12)
					tmx->tileset[l].firstgid = atoi(aura.text);

				if(arg == 3)
					tmx->tileset[l].tilewidth = atoi(aura.text);

				if(arg == 4)
					tmx->tileset[l].tileheight = atoi(aura.text);

				if(arg == 14)
					tmx->tileset[l].tilecount = atoi(aura.text);

				if(arg == 13)
					strcpy(tmx->tileset[l].name,aura.text);

				if(arg == 15)
					tmx->tileset[l].columns = atoi(aura.text);
			}

			//image
			if(type == 4)
			{
				if(arg == 5)
					tmx->tileset[l].width = atoi(aura.text);

				if(arg == 6)
					tmx->tileset[l].height = atoi(aura.text);

				if(arg == 17)
					tmx->tileset[l].trans = atoi(aura.text);

				if(arg == 16)
					strcpy(tmx->tileset[l].source,aura.text);

			}


			//layer
			if(type == 5)
			{
				if(arg == 5)
					tmx->layer[j].width = atoi(aura.text);

				if(arg == 6)
					tmx->layer[j].height = atoi(aura.text);


				if(arg == 13)
					strcpy(tmx->layer[j].name,aura.text);

			}

			//data
			if(type == 6)
			{
				if(arg == 2)
				{
					tmx->layer[j].encoding = option;
					kn = tmx->layer[j].width * tmx->layer[j].height;
					k = 0;
					tmx->layer[j].data = malloc(sizeof (unsigned int)* kn);
				}

			}

		}

		//
		if(aura.com == 0)
		{
			if(letter == '/')
				aura.bloc[1]++;

			if(letter == '<')
				aura.bloc[0]++;

			if(letter == '>')
			{
				if( (type == 4) && (aura.bloc[1] > 0) )
					l++;

				if( (type == 5) && (aura.bloc[1] > 0) )
					j++;

				aura.bloc[1] = 0;
				aura.bloc[0]--;

				arg = 0;
				type = 0;
			}
		}
	}

	tmx->ntileset = l;
	tmx->nmap = j;

	free(buffer);
}
