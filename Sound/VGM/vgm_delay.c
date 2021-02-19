#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"


void VGM_delay(VGM *vgm,FILE *file,int delaynew)
{
	static int delay = -1;
	if(delay != delaynew)
	{
		delay = delaynew;
	}

	vgm->wait++;

	int tmp;


	//if(vgm->cmde != 0)
	{
		if(delay == 735)
		{
			vgm->size+=1;
			vgm->sizet[6]+=1;
			fputc(0x0F,file);
		}else
		{

			tmp = 0.02275*(float)delay;
			tmp &= 0x07FF;

			fputc(tmp>>8,file);
			fputc(tmp&0xFF,file);
			vgm->size+=2;
			vgm->sizet[6]+=2;
		}

	}


	vgm->cmde = 0;

	return;
	if(vgm->wait >= 1000)
		return;

	tmp = 0.02275*(float)delay;
	tmp &= 0x07FF;
	//if(delay != 735)
	printf("cmd wait %f s   %dms\n",(1.0/44100.0)*delay,tmp);


}

