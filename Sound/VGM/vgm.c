#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"


int VGM_Load()
{
    FILE *file = NULL;
    //file = fopen("breeze2608.vgm","rb");
    //FILE *file = fopen("Ikari_Team.vgm","rb");
    //file = fopen("test.vgm","rb");
	//file = fopen("RudeBuster.vgm","rb");
	file = fopen("Megalopolis.vgm","rb");
	//file = fopen("Akira.vgm","rb");
    if(file == NULL)
	{
		printf("Error no open file!\n");
		return 1;
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);

	fseek(file, 0, SEEK_SET);

	unsigned char *buffer = (unsigned char *)malloc(size+3);
	if (buffer == NULL)
	{
		fclose(file);
		printf("Error malloc!\n");
		return 1;
	}

	fread(buffer, 1,size, file);
	fclose(file);

	VGM vgm;
	vgm.tag = 0;
	if(buffer[0] == 'V') vgm.tag++;
	if(buffer[1] == 'g') vgm.tag++;
	if(buffer[2] == 'm') vgm.tag++;
	if(buffer[3] == ' ') vgm.tag++;

	if(vgm.tag != 4)
	{
		printf("Error no VGM File!\n");
		return 1;
	}

	int *buffer32 = (void*)buffer;

	vgm.eof = buffer32[1];
	vgm.version = buffer32[2];

	vgm.gd3 = buffer32[5]+20-1;
	vgm.totalsample = buffer32[6];
	vgm.loopoffset = buffer32[7];

	vgm.loopsample = buffer32[8];
	vgm.rate = buffer32[9];

	vgm.data = buffer32[13];

	vgm.YM2608 = buffer32[18];
	vgm.YM2610 = buffer32[19];

	printf("EOF %x\n",vgm.eof);
	printf("Version %x\n",vgm.version);
	printf("GD3 %x\n",vgm.gd3);

	printf("Total Samples %d\n",vgm.totalsample);
	printf("Loop offset %x\n",vgm.loopoffset);
	printf("Loop Samples %d\n",vgm.loopsample);

	printf("Rate %d\n",vgm.rate);

	printf("Data offset %x\n",vgm.data+52);

	printf("YM2608 %d\n",vgm.YM2608);
	printf("YM2610 %d\n",vgm.YM2610);


	printf("----------\n");

	//PC-98
	if(vgm.YM2608 != 0)
	{
		VGM_Read_YM2608(vgm,buffer);
		return 0;
	}

	//NEO GEO
	if(vgm.YM2610 != 0)
	{
		//VGM_Read_YM2610(vgm,buffer);
		return 0;
	}

	printf("Error no VGM Supported!\n");

	return 1;
}


void VGM_Init(VGM *vgm)
{
	vgm->wait = 0;
	vgm->size = 0;
	vgm->cmde = 0;
	vgm->SSGslow = 0;

	int l;
	for(l = 0;l < 16;l++)
	{
		vgm->SSG[l] =-1;
		vgm->ADPCM_A[l] = -1;
		vgm->ADPCM_B[l] = -1;
	}



	for(l = 0;l < 0x90;l++)
	{
		vgm->FM1[l] = -1;
		vgm->FM2[l] = -1;
	}

	for(l = 0;l < 8;l++)
		vgm->sizet[l] = 0;
}


void VGM_Print_size(VGM *vgm)
{
	printf("ADPCM-A %d bytes\n",vgm->sizet[0]);
	printf("ADPCM-B %d bytes\n",vgm->sizet[1]);
	printf("SSG %d bytes\n",vgm->sizet[2]);
	printf("FM Commons %d bytes\n",vgm->sizet[3]);
	printf("FM OP %d bytes\n",vgm->sizet[4]);
	printf("Wait %d bytes\n",vgm->sizet[6]);

	printf("Total %d bytes\n",vgm->size);
}

