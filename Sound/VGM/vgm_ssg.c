#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"


void VGM_SSG(VGM *vgm,FILE *file,int adr,int data)
{

	if(adr == 0x07)
		data = data&0x3F;

	int j = adr;

	if( (data&0x07) == 0x07)
		vgm->SSG[j] = -1;

	if( (data&0x08) == 0x08)
		vgm->SSG[j] = -1;

	if( (data&0x09) == 0x09)
		vgm->SSG[j] = -1;

	if( (data&0x0A) == 0x0A)
		vgm->SSG[j] = -1;

	if(vgm->SSG[j] != data)
	{
		vgm->SSG[j] = data;

		fputc(j+0x10,file);
		fputc(data,file);

		vgm->cmde++;
		vgm->sizet[2]+=2;
		vgm->size+=2;
	}

}
