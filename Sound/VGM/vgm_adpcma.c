#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"


void VGM_ADPCMA(VGM *vgm,FILE *file,int adr,int data)
{

	int j = adr-0x10;

	if(adr == 0x11) //Master volume
		return;

	if(adr == 0x10)
	{
		vgm->ADPCM_A[j] =-1;
	}

	if(vgm->ADPCM_A[j] != data)
	{
		vgm->ADPCM_A[j] = data;

		fputc(j+0x30,file);
		fputc(data,file);

		vgm->size+=2;
		vgm->sizet[0]+=2;

		//printf("PCM %d %x %x\n",vgm->wait,adr,data);
	}
}
