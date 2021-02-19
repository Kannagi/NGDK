#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"


void VGM_ADPCMB(VGM *vgm,FILE *file,int adr,int data)
{

	int j = adr;

	if(adr == 0x00)
	{
		data &= 0x91;
		vgm->ADPCM_B[j] = -1;
		j = 0x02;
	}

	if(adr == 0x01)
	{
		data &= 0xC0;
		j = 0x03;
	}

	if( (adr >= 0x02) && (adr <= 0x5) ) //adress
	{
		//instrument
		return;
	}

	if( (adr >= 0x06) && (adr <= 0x8) ) //prescale / ADPCM DATA
	{
		//printf("ADPCM-B error 1\n");
		return;
	}


	if( (adr >= 0x09) && (adr <= 0xA) )
	{
		adr -= 4;
	}

	if(adr > 0x0B)
	{
		//printf("ADPCM-B error 2\n");
		return;
	}
	//return;
	//printf("ADPCM-B %d %x %x\n",vgm->wait,adr,data);
	if(vgm->ADPCM_B[j] != data)
	{
		vgm->ADPCM_B[j] = data;

		//fputc(j+0x30,file);
		//fputc(data,file);
		vgm->size+=2;
		vgm->cmde++;
		vgm->sizet[1]+=2;


	}
}
